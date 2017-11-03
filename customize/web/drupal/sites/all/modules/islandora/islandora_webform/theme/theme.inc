<?php

/**
 * Renders webform links for an object as appropriate.
 *
 * @param array $vars
 *   An array containing:
 *   - webform_data: Data from islandora_webform_webforms table.
 *   - islandora_object: The fedora object.
 *
 * @return string
 *   Themed links
 */
function theme_islandora_webform_links(array $vars) {
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_css(drupal_get_path('module', 'islandora_webform') . '/css/islandora_webform.css');

  $webforms = $vars['webform_data'];
  $object = $vars['islandora_object'];
  $pid = $object->id;
  module_load_include('inc', 'islandora_webform', 'includes/utilities');
  module_load_include('inc', 'webform', 'includes/webform.submissions');
  module_load_include('inc', 'islandora_webform', 'includes/webform_access');

  $render = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'islandora_webform_object_tag_links',
    ),
  );

  $destination = drupal_get_destination();

  $webform_link_options = array(
    'html' => TRUE,
    'query' => array(
      'pid' => $pid,
      $destination,
    ),
  );

  $tag_link_options = array(
    'attributes' => array(
      'class' => array(
        'tag-link',
        'use-ajax',
      ),
    ),
    'html' => TRUE,
    'query' => $destination,
  );
  $untag_link_options = array(
    'attributes' => array(
      'class' => array(
        'untag-link',
        'use-ajax',
      ),
    ),
    'html' => TRUE,
    'query' => $destination,
  );

  $link_mode = variable_get('islandora_webform_link_target', 'new');
  foreach ($webforms as &$webform) {

    // Lets load the node.
    $swebform_node = node_load($webform->entity_id);
    $webform_node_data = $swebform_node->webform;

    $jqid = 'iw_webform_links_' . $webform->entity_id;

    $render[$jqid] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => $jqid,
        'class' => array(
          'webform-links-wrapper'
        ),
      ),
      '#entity_id' => $webform->entity_id,
      '#entity' => $swebform_node,
      'webform-links' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array(
            'webform-links',
          ),
        ),
      ),
    );

    // Add submissions for this webform and this object.
    $sids_query = db_select('webform_submissions', 'ws')->fields('ws', array('sid'));
    $sids_query->distinct();
    $sids_query->join('webform_submitted_data', 'wd', "wd.sid = ws.sid");
    $sids_query->join('webform_component', 'wc', "wc.cid = wd.cid AND wc.nid = ws.nid AND wc.form_key = 'islandora_object_pid'");
    $sids_query->condition('ws.nid', $webform->entity_id);
    $sids_query->condition('wd.data', $pid);
    $wosids = $sids_query->execute()->fetchCol();
    $render[$jqid]['#submissions'] = empty($wosids) ? array() : webform_get_submissions(array(
      'nid' => $swebform_node->nid,
      'sid' => $wosids
    ));

    if(module_exists('webform_workflow')) {
      foreach ($render[$jqid]['#submissions'] as &$workflow_state_submission) {
        webform_workflow_state_load_by_submission($workflow_state_submission);
      }
    }

    $inline = module_exists('webform_ajax') && isset($webform_node_data['webform_ajax']) ? $webform_node_data['webform_ajax'] != WEBFORM_AJAX_NO_AJAX : FALSE;
    if ($inline) {
      $link_mode = 'inline';
      $webform_link_options['attributes']['class'] = array(
        'webform-link',
        'use-ajax'
      );
      unset($webform_link_options['query']['destination']);
    }
    else {
      $webform_link_options['attributes']['class'] = array('webform-link');
      if ($link_mode != 'same') {
        $webform_link_options['attributes']['target'] = '_islandora_webform_page';
      }
    }

    // Show previous submissions by this user if they can be edited.
    global $user;
    $tracking_mode = webform_variable_get('webform_tracking_mode');
    $cookie_name = 'webform-' . $swebform_node->nid;
    if (!empty($pid)) {
      $cookie_name .= '-' . $pid;
    }
    // Get a list of submission ids that this user has created for this object
      // and this webform.
    $query = $sids_query;

    // Handle different kinds of users.
    ///////////////////////////////////////
    // If uid is greater than zero, use it.
    if ($user->uid) {
      $query->condition('uid', $user->uid);
    }
    // Handle if user provided is anonymous but has a cookie that matches. We search against
    // the submission.modified time, if so.
    elseif ($user->uid === 0
      && ($tracking_mode === 'cookie' || $tracking_mode === 'strict')
      && isset($_COOKIE[$cookie_name])
      && is_array($_COOKIE[$cookie_name])
    ) {
      $query->condition('ws.modified', end($_COOKIE[$cookie_name]));
      $query->condition('ws.uid', $user->uid);
    }
    // If user is anonymous and has no cookie, filter by their ip address.
    else {
      $query->condition('ws.remote_addr', ip_address());
      $query->condition('ws.uid', $user->uid);
    }

      $sids = $query->execute()->fetchCol();

      $submissions = empty($sids) ? array() : webform_get_submissions(array(
        'nid' => $swebform_node->nid,
        'sid' => $sids
      ));

      // Check if this user has edit access to this submission.
      if (!empty($submissions)) {
        module_load_include('inc', 'islandora_webform', 'includes/inline_webform');
        foreach ($submissions as $key => $submission) {
          // The webform_submission_access function doesn't let modules deny access
          // if there is a general permission granting access. We want to deny access
          // to folks who can edit their submissions generally, depending on
          // whether it is in draft, or completed.
          $submissions[$key]->access = islandora_webform_webform_submission_access($swebform_node, $submission, 'edit', $user);
        }
      }

      // If the number of previous submissions is more than one, display links to
      // edit the submissions.
      if (count($submissions)) {
        foreach ($submissions as $editable_submission) {
          if($editable_submission->access) {
            $render[$jqid]['webform-links'][$editable_submission->sid] = islandora_webform_theme_webform_link($webform, $webform_link_options, $pid, $link_mode, $editable_submission->sid);
            $render[$jqid]['webform-links'][$editable_submission->sid]['#status'] = 'editable submission';
            $render[$jqid]['webform-links'][$editable_submission->sid]['#submission'] = $editable_submission;
            $render[$jqid]['webform-links'][$editable_submission->sid]['#link_mode'] = $link_mode;
          }
        }
      }
    // If the webform's scope does not require it to be tagged, check to see if
    // the user is permitted to create a new submission. If so, add a link to do that.
    if ($webform->scope != 'tagged') {
      if (islandora_webform_user_can_submit_webform($swebform_node, NULL, $pid) === TRUE) {
        $render[$jqid]['webform-links'][$pid] = islandora_webform_theme_webform_link($webform, $webform_link_options, $pid, $link_mode);
        $render[$jqid]['webform-links'][$pid]['#status'] = 'new submission';
        $render[$jqid]['webform-links'][$pid]['#submission'] = NULL;
        $render[$jqid]['webform-links'][$pid]['#link_mode'] = $link_mode;
      }
    }
    else {
      // Determine if this webform is tagged to this object.
      module_load_include('inc', 'islandora_webform', 'includes/tag_object');
      $tagged = islandora_webform_is_webform_tagged($object, $webform->entity_id);

      // If so, add the webform link and the remove-tag link for content
      // managers.
      if ($tagged) {

        if (islandora_webform_user_can_submit_webform($swebform_node, NULL, $pid) === TRUE) {
          $render[$jqid]['webform-links'][$pid] = islandora_webform_theme_webform_link($webform, $webform_link_options, $pid, $link_mode);
        }

        $path = 'islandora_webform/' . $webform->entity_id . '/untag_object/' . $pid . '/nojs';

        $untag_link_options['query']['tok'] = drupal_get_token('untag_link_' . $pid . '_' . $webform->entity_id);
        $untag_link_options['html'] = FALSE;

        // drupal_valid_path() checks to see if the current user has access to
        // the add/remove tag links.
        if (drupal_valid_path($path, TRUE)) {
          $tag_link_options['query'] = $destination;
          $untag_link_render = array(
            '#theme' => 'link',
            '#text' => t('<- remove this link'),
            '#path' => $path,
            '#options' => $untag_link_options,
          );

          $render[$jqid]['untag-link'] = $untag_link_render;
        }
      }
      else {
        // Otherwise, add the add-tag link for managers.
        $path = 'islandora_webform/' . $webform->entity_id . '/tag_object/' . $pid . '/nojs';

        // Add a token to the query.
        $tag_link_options['query']['tok'] = drupal_get_token('tag_link_' . $pid . '_' . $webform->entity_id);
        $tag_link_options['html'] = FALSE;

        // Check access.
        if (drupal_valid_path($path, TRUE)) {
          $tag_link_render = array(
            '#theme' => 'link',
            '#text' => t('add "!text" form link', array('!text' => $webform->link_text)),
            '#path' => $path,
            '#options' => $tag_link_options,
          );
          $render[$jqid]['tag-link'] = $tag_link_render;
        }
      }
    }
    $webform->user_submissions = $submissions;
  }
  $render['inline-webform-target'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'iw_inline_webform_target',
    ),
  );

  $alter_data = array(
    'islandora_object' => $object,
    'webforms' => $webforms,
    'submissions' => !empty($submissions) ? $submissions : array(),
    'user-submissions' => !empty($webform->user_submissions) ? $webform->user_submissions : array(),
  );
  // Let other modules modify the complete webform links render array.
  drupal_alter('islandora_theme_webform_links', $render, $alter_data);
  return $render;
}

/**
 * Utility function to return a themed webform link for the current user.
 *
 * Return empty string if the user can't submit the webform
 *
 * @param object $islandora_webform_record
 *   one complete row from the islandora_webform_webforms table
 * @param array  $webform_link_options
 *   an options array for the l() function. We expect it to have the "pid" query parameter
 *   included to make this a valid call to a islandora-enabled webform.
 * @param string $link_mode
 *   If value is 'inline', then an ajax link is created.
 * @param string $sid
 *   If the link is to edit a previous submission, this is that submission's sid
 * @param boolean $alter
 *   If TRUE, other modules can override the output of this function. If a module wishes
 *   to call this function from within their hook_islandora_webform_theme_webform_link_alter(),
 *   they should set $alter to FALSE to avoid infinite recursion.
 * @param boolean $force
 *   If TRUE, render the link whether or not the user is permitted a new submission. This can
 *   be used to display an edit link to an existing submission when a new submission is not
 *   allowed, or any other content in this place.
 *
 * @return string
 *   The themed link, or empty if no access
 */
function islandora_webform_theme_webform_link($islandora_webform_record, $webform_link_options = array(), $pid, $link_mode = 'inline', $sid = NULL, $alter = TRUE, $force = FALSE) {
  global $user;
  $webform_link = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('webform-link'),
    ),
  );
  $islandora_webform_record->link_mode = $link_mode;
  $node = node_load($islandora_webform_record->entity_id);
  module_load_include('inc', 'islandora_webform', 'includes/webform_access');
  if ($node->status) {
    $can_submit = islandora_webform_user_can_submit_webform($node, NULL, $pid);
    if (is_array($can_submit) && !empty($can_submit)) {
      foreach ($can_submit as $draft_submission) {
        $submission = webform_get_submission($node->nid, $draft_submission->sid);
        $link_path = $link_mode == 'inline' ? 'islandora_webform/retrieve_form/' . $islandora_webform_record->entity_id . '/nojs' : drupal_get_path_alias('node/' . $islandora_webform_record->entity_id);
        $link_path .= '/submission/' . $draft_submission->sid . '/edit';
        $when = max($submission->submitted, $submission->completed, $submission->modified);
        $link_text = t('!type of %webformname, last worked on !when by !who (click to edit)', array(
          '!type' => $submission->completed ? 'Submission' : 'Draft',
          '%webformname' => !empty($islandora_webform_record->link_text) ? $islandora_webform_record->link_text : $node->title,
          '!when' => format_date($when),
          '!who' => $user->uid === $submission->uid ? 'you' :  $submission->name,
        ));
        $class = array('webform-link');
        if($link_mode == 'inline') {
          $class[] = 'use-ajax';
        }

        $webform_link[] = array(
          '#theme' => 'link',
          '#text' => $link_text,
          '#path' => $link_path,
          '#options' => $webform_link_options,
          '#iw_link_mode' => $link_mode,
          '#submission' => $submission,
        );
      }
    }
    elseif ($can_submit || $force) {
      $link_path = $link_mode == 'inline' ? 'islandora_webform/retrieve_form/' . $islandora_webform_record->entity_id . '/nojs' : drupal_get_path_alias('node/' . $islandora_webform_record->entity_id);
      $link_text = $islandora_webform_record->link_text;
      if ($sid) {
        $submission = webform_get_submission($node->nid, $sid);

        $link_path .= '/submission/' . $sid . '/edit';
        $when = max($submission->submitted, $submission->completed, $submission->modified);
        $link_text = t('!type of %webformname, last worked on !when by !who', array(
          '!type' => $submission->completed ? 'Submission' : 'Draft',
          '%webformname' => !empty($islandora_webform_record->link_text) ? $islandora_webform_record->link_text : $node->title,
          '!when' => format_date($when),
          '!who' => $submission->name,
        ));
        $islandora_webform_record->link_help = t('Click to modify or add to this previous !type.',
          array(
            '!type' => $submission->completed ? 'submission' : 'draft',
          ));
        $webform_link_options['attributes']['class'][] = $submission->completed ? 'submission' : 'draft';
      }
      $webform_link_options['attributes']['title'] = $islandora_webform_record->link_help;
      $webform_link[] = array(
        '#theme' => 'link',
        '#text' => $link_text,
        '#path' => $link_path,
        '#options' => $webform_link_options,
        '#iw_link_mode' => $link_mode,
        '#submission' => !empty($submission) ? $submission : NULL,
      );
    }

  }
  // Let other modules modify the webform link.
  if ($alter) {
    drupal_alter('islandora_webform_theme_webform_link', $webform_link, $islandora_webform_record, $pid, $sid);
  }
  return $webform_link;
}
