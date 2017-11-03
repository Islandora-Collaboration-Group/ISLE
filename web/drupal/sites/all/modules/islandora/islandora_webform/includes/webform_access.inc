<?php

/**
 * Utility function to determine if user can access and submit this webform.
 *
 * This code is mostly lifted from the webform module's webform_node_view
 * function.
 *
 * @param object $node
 *   The webform node object
 * @param null $user
 *   A user object, or NULL
 *
 * @return mixed bool|array
 *   Array of draft submission for the user, if present, otherwise boolean:
 *   If true, the user can submit; if false, the user cannot submit.
 *
 */
function islandora_webform_user_can_submit_webform($node, $user = NULL, $pid = NULL) {

  if (!$user) {
    global $user;
  }

  $page = node_is_page($node);
  $enabled = TRUE;
  $allowed_roles = array();

  // Check if the user's role can submit this webform.
  if (variable_get('webform_submission_access_control', 1)) {
    foreach ($node->webform['roles'] as $rid) {
      $allowed_roles[$rid] = isset($user->roles[$rid]) ? TRUE : FALSE;
    }

    if (count(array_filter($allowed_roles)) == 0 && $user->uid != 1) {
      $enabled = FALSE;
    }
  }

  // Check if this page is cached or not.
  $cached = $user->uid == 0 && (variable_get('cache', 0) || drupal_page_is_cacheable() === FALSE);

  // Check if the user can add another submission.
  if ($enabled && $node->webform['submit_limit'] > -1) { // -1: Submissions are never throttled.
    $user_limit_exceeded = islandora_webform_submission_user_limit_check($node, $pid);
    module_load_include('inc', 'webform', 'includes/webform.submissions');
      if($pid && $node->webform['submit_interval'] == ISLANDORA_WEBFORM_SUBMIT_PER_OBJECT_LIMIT_ID) {
        // Per user and object limit is in place.
        // See how many submissions for this user and compare to limit.
        $ws_limits = @unserialize($node->webform['user_ws_limits']);
        $user_submissions = islandora_webform_object_submission_count($node->nid, $pid, $node->webform['submit_interval'], $user->uid, $ws_limits);
        $draft_user_submissions = array_filter($user_submissions['results'], function($r) {return $r->is_draft == 1;});
        $draft_user_submissions_count = count($draft_user_submissions);
        if (empty($draft_user_submissions_count) && $node->webform['submit_limit'] <= $user_submissions['count']) {
          $enabled = FALSE;
        }


    }
    // Disable the form if the limit is exceeded and page cache is not active.
    elseif ($user_limit_exceeded && !$cached) {
      $enabled = FALSE;
    }
  }

  // Check if the user can add another submission if there is a limit on total
  // submissions.
  if ($enabled && $node->webform['total_submit_limit'] > -1) { // -1: Submissions are never throttled.
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    $total_limit_exceeded = islandora_webform_submission_total_limit_check($node, $pid);

    if($pid && $node->webform['total_submit_interval'] == ISLANDORA_WEBFORM_SUBMIT_PER_OBJECT_LIMIT_ID) {
      // Total object limit is in place.
      $ws_limits = @unserialize($node->webform['total_ws_limits']);
      $submission_count = islandora_webform_object_submission_count($node->nid, $pid, $node->webform['total_submit_interval'], NULL, $ws_limits);
      if ($node->webform['total_submit_limit'] <= $submission_count['count']) {
        $enabled = FALSE;
      }
    }
    // Disable the form if the limit is exceeded and page cache is not active.
    if ($total_limit_exceeded && !$cached) {
      $enabled = FALSE;
    }
  }

  if (!empty($draft_user_submissions) && !empty($node->webform['draft_access'])) {
    $enabled = $draft_user_submissions;
  }
  return $enabled;

}

/**
 * Count the number of submissions that meet the limit criteria.
 *
 *
 * @param       $nid
 *   The webform node id.
 * @param       $pid
 *   The islandora object pid.
 * @param null  $interval
 *   The webform submit interval
 * @param null  $uid
 *   If $uid is provided, then constrain submission count to those by that user.
 * @param array $ws_filter
 *   Array of workflow state identifiers that are being filtered against.
 *
 *
 * @return array
 *   'count' => the number of submissions
 *   'results' =>
 */
function islandora_webform_object_submission_count($nid, $pid = NULL, $interval = NULL, $uid = NULL, $ws_filter = array()) {

  // Only do this once for a given set of function args.
  $results = &drupal_static(__FUNCTION__ . md5(serialize(array($nid, $pid, $interval, $uid, $ws_filter))));

  if (!isset($results)) {

    $tracking_mode = webform_variable_get('webform_tracking_mode');

    $query = db_select('webform_submissions', 's');
    $query->fields('s', array('sid', 'is_draft'));
    $query->condition('s.nid', $nid);
    $query->distinct();
    if ($pid) {
      $query->join('webform_component', 'c', "c.nid = s.nid and c.form_key = 'islandora_object_pid'");
      $query->join('webform_submitted_data', 'd', "d.sid = s.sid and d.cid = c.cid");
      $query->condition('d.data', $pid);
    }

    if ($uid) {
      $query->condition('s.uid', $uid);
    }
    // Handle if user provided is anonymous but has a cookie that matches. We search against
    // the submission.modified time, if so.
    elseif ($uid === 0 && ($tracking_mode === 'cookie' || $tracking_mode === 'strict')) {
      $cookie_name = 'webform-' . $nid . '-' . $pid;
      if (isset($_COOKIE[$cookie_name]) && is_array($_COOKIE[$cookie_name])) {
        $query->condition('s.modified', end($_COOKIE[$cookie_name]));
        $query->condition('s.uid', $uid);
      }
      else {
        // Anonymous user has no cookies that match. No need to continue. Just
        // return empty result.
        return array('count' => 0, 'results' => array());
      }
    }

    if (!empty($ws_filter)) {
      $ws_conditions = db_or();
      // Separate out draft, completed and ingested from webform workflow states filter.
      $core_states = array();
      $core_states['draft'] = array_search('draft', $ws_filter);
      $core_states['completed'] = array_search('completed', $ws_filter);
      $core_states['ingested'] = array_search('ingested', $ws_filter);
      $core_states['not-ingested'] = array_search('not-ingested', $ws_filter);
      $core_states = array_filter($core_states);
      foreach ($core_states as $id => $core_state) {
        switch ($id) {
          case 'draft':
            $ws_conditions->condition('s.is_draft', 1);
            break;
          case 'completed':
            $ws_conditions->condition('s.is_draft', 0);
            break;
          case 'ingested':
            $query->leftJoin('islandora_webform_ingestions', 'iwi', 'iwi.sid = s.sid AND iwi.nid = s.nid');
            $ws_conditions->isNotNull('iwi.sid');
            $iwi_table_joined = TRUE;
            break;
          case 'not-ingested':
            $current_tables = $query->getTables();
            if (empty($current_tables['iwi'])) {
              $query->leftJoin('islandora_webform_ingestions', 'iwi', 'iwi.sid = s.sid AND iwi.nid = s.nid');
            }
            $ws_conditions->isNotNull('iwi.sid');
            break;
        }
        unset($ws_filter[$id]);
      }

      // Handle rwemaining webform workflow state filters.
      if (!empty($ws_filter)) {
        $query->leftJoin('webform_workflow_submissions', 'ws', "ws.sid = s.sid");
        $ws_conditions->condition('ws.wsid', $ws_filter);
      }
      $query->condition($ws_conditions);
    }

    // Permit modules to add additional logic to the submissions count query.
    $params = array(
      'nid' => $nid,
      'pid' => $pid,
      'interval' => $interval,
      'uid' => $uid,
    );

    // Enable to be altered by hook_query_alter.
    $query->addTag('islandora_webform_object_submission_count');
    $query->addMetaData('islandora_webform_object_submission_count_query_params', $params);

    drupal_alter('islandora_webform_object_submission_query_count', $query, $params);

    $results = $query->execute()->fetchAll();
  }
  return array('count' => count($results), 'results' => $results);
}

/** Check if the current user has exceeded the limit on this form.
 * This is a modified version of webform_submission_user_limit_check().
 *
 * @param $node
*   The webform node to be checked.
 * @param $account
*   Optional parameter. Specify the account you want to check the limit
  *   against.
 *
 * @return
 *   Boolean TRUE if the user has exceeded their limit. FALSE otherwise.
 */
function islandora_webform_submission_user_limit_check($node, $pid = NULL, $account = NULL) {

  if (!isset($account)) {
    global $user;
    $account = $user;
  }
  // We can only check access for anonymous users through their cookies.
  if ($user->uid !== 0 && $account->uid === 0) {
    watchdog('webform', 'Unable to check anonymous user submission limit when logged in as user @uid.', array('@uid' => $user->uid), WATCHDOG_WARNING);
    return FALSE;
  }
  // Check if submission limiting is enabled.
  if ($node->webform['submit_limit'] == '-1') {
    return FALSE; // No check enabled.
  }

  $qualified_count = islandora_webform_object_submission_count($node->nid, $pid, $node->webform['submit_interval'], $account->uid, unserialize($node->webform['user_ws_limits']));
  $num_submissions_database = $qualified_count['count'];

  if ($num_submissions_database >= $node->webform['submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}

/**
 * Check if the total number of submissions has exceeded the limit on this form.
 * This is a modified version of webform_submission_total_limit_check().
 *
 * @param $node
 *   The webform node to be checked.
 * @return
 *   Boolean TRUE if the form has exceeded it's limit. FALSE otherwise.
 */
function islandora_webform_submission_total_limit_check($node, $pid = NULL) {
  // Check if submission limiting is enabled.
  if ($node->webform['total_submit_limit'] == '-1') {
    return FALSE; // No check enabled.
  }

    $qualified_count = islandora_webform_object_submission_count($node->nid, $pid, $node->webform['total_submit_interval'], NULL, unserialize($node->webform['total_ws_limits']));
    $num_submissions_database = $qualified_count['count'];

  if ($num_submissions_database >= $node->webform['total_submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}
