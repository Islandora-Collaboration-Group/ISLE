<?php
/**
 * @file
 * Drush command/hook implementation for updating existing citation objects.
 */

/**
 * Implements hook_drush_command().
 *
 * @see islandora_scholar_embargo_update_7001()
 */
function islandora_scholar_embargo_drush_command() {
  return array(
    'islandora_scholar_embargo_lift_embargoes_7001' => array(
      'description' => dt("Lift all expired Islandora Scholar embargoes. This
        command is meant to accompany update 7001. In the normal case Drupal's
        cron or the GUI should be used to lift embargoes.  There is an edge
        case where this script will be ineffective: if the versioning
        information for the RELS DSs are missing. There is an edge case where
        this script will be destructive: If an XACML policy that looks like an
        embargo has been placed on a previously embargoed obeject.  If either of
        these edge cases apply to you, you should use a manual process."),
      'examples' => array('drush -v -u 1 islandora_scholar_embargo_lift_embargoes_7001'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
      'callback' => 'islandora_scholar_embargo_lift_embargoes_update',
    ),
    'islandora_scholar_embargo_reapply_embargoes' => array(
      'description' => dt('Applies the list of roles that can administer embargoes to all embargoed objects in the repository.'),
      'examples' => array('drush -v -u 1 islandora_scholar_embargo_reapply_embargoes'),
      'aliases' => array('ise-reapply'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
      'callback' => 'islandora_scholar_embargo_reapply_embargoes',
    ),
  );
}

/**
 * Sets a batch to lift existing expired embargoes.
 */
function islandora_scholar_embargo_lift_embargoes_update() {
  batch_set(
    array(
      'operations' => array(array('islandora_scholar_embargo_lift_embargoes_update_operation', array())),
      'title' => t('Lift Expired Embargoes'),
      'error_message' => t('An error has occurred'),
      'init_message' => t('Preparing to lift embargoes'),
      'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
      'file' => drupal_get_path('module', 'islandora_scholar_embargo') . '/islandora_scholar_embargo.drush.inc',
    )
  );
  drush_backend_batch_process();
}

/**
 * Runs the batch to apply whitelisted roles to embargoed objects.
 */
function islandora_scholar_embargo_reapply_embargoes() {
  module_load_include('inc', 'islandora_scholar_embargo', 'includes/batch');
  batch_set(islandora_scholar_embargo_reapply_embargoes_batch());
  drush_backend_batch_process();
}

/**
 * Batch operation for lifting embargoes for the update.
 *
 * Assumes being ran in drush.
 *
 * @param array $context
 *   Context array for this batch. Used to tell batch when we're finally
 *   finished.
 */
function islandora_scholar_embargo_lift_embargoes_update_operation(&$context) {
  module_load_include('inc', 'islandora_scholar_embargo', 'includes/embargo');
  module_load_include('inc', 'islandora', 'includes/datastream.version');
  $tuque = islandora_get_tuque_connection();
  $sandbox = &$context['sandbox'];
  $step = 10;
  $suspect_query = <<<EOQ
SELECT ?object
FROM <#ri>
WHERE {
  ?object <fedora-view:disseminates> ?ds .
  ?ds <fedora-view:disseminationType> <info:fedora/*/POLICY>
}
EOQ;

  // Initial setup.
  if (!isset($sandbox['total'])) {
    $sandbox['total'] = $tuque->repository->ri->countQuery($suspect_query, 'sparql');
    $sandbox['current'] = 0;
    if ($sandbox['total'] == 0) {
      $context['finished'] = 1;
      return;
    }
  }
  // Are we going to be finished this iteration?
  $limited_suspect_query = $suspect_query . "
    LIMIT $step
    OFFSET {$sandbox['current']}
  ";
  $sandbox['current'] += $step;
  $context['finished'] = $sandbox['current'] / $sandbox['total'];

  // Do checks for this step. RELS have been stripped so getting all objects
  // with POLICY DSs.
  $suspect_results = $tuque->repository->ri->sparqlQuery($limited_suspect_query);
  // Verify suspects.
  $suspects = array();
  foreach ($suspect_results as $suspect) {
    $pid = $suspect['object']['value'];
    $object = islandora_object_load($pid);
    // If currently embargoed pass.
    if (islandora_scholar_embargo_get_embargoed($object)) {
      continue;
    }
    $was_embargoed = FALSE;
    $dsid = NULL;
    // Assume only PDF DSs.
    if (isset($object['RELS-INT']) && isset($object['PDF'])) {
      foreach ($object['RELS-INT'] as $rels_int_ds) {
        $rels_int = new FedoraRelsInt($object['PDF']);
        $rels_int->datastream = $rels_int_ds;
        if ($rels_int->get(ISLANDORA_SCHOLAR_EMBARGO_RELS_URI, ISLANDORA_SCHOLAR_EMBARGO_EXPIRY_PRED)) {
          $dsid = 'PDF';
          $was_embargoed = TRUE;
          break;
        }
      }
    }
    elseif (isset($object['RELS-EXT'])) {
      foreach ($object['RELS-EXT'] as $rels_ext_ds) {
        $rels_ext = new FedoraRelsExt($object);
        $rels_ext->datastream = $rels_ext_ds;
        if ($rels_ext->get(ISLANDORA_SCHOLAR_EMBARGO_RELS_URI, ISLANDORA_SCHOLAR_EMBARGO_EXPIRY_PRED)) {
          $was_embargoed = TRUE;
          break;
        }
      }
    }
    if ($was_embargoed) {
      $suspects[$pid] = isset($dsid) ? $dsid : NULL;
    }
  }
  // Remove embargoes.
  foreach ($suspects as $pid => $dsid) {
    drush_log(dt('Clearing embargo on !pid.', array('!pid' => $pid)));
    islandora_scholar_embargo_lift_embargo($pid, $dsid);
  }
}
