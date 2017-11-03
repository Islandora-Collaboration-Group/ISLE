<?php

/**
 * @file
 * Drush command/hook implementation for updating existing video objects.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_video_drush_command() {
  $commands = array();

  $commands['islandora-video-update-videos'] = array(
    'description' => dt('Update existing videos to remove OGG derivatives as they are no longer needed.'),
    'drupal dependencies' => array(
      'islandora',
      'islandora_video',
    ),
    'options' => array(),
    'examples' => array(
      'drush -u 1 islandora-video-update-videos' => dt('Removing OGGs from existing video objects.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $commands;
}

/**
 * Command callback to update citations with PDF derivatives.
 */
function drush_islandora_video_update_videos() {
  batch_set(islandora_video_update_videos_create_batch());
  drush_backend_batch_process();
}

/**
 * Constructs a batch used to update things via Drush.
 */
function islandora_video_update_videos_create_batch() {
  return array(
    'operations' => array(
      array('islandora_video_update_videos_batch_operation', array()),
    ),
    'title' => t('Removing OGG derivatives from existing video objects...'),
    'init_message' => t('Preparing to delete OGGs.'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_video') . '/islandora_video.drush.inc',
  );
}

/**
 * Constructs and performs the citation batch operation.
 *
 * @param array $context
 *   The context of the Drupal batch.
 */
function islandora_video_update_videos_batch_operation(&$context) {
  $update_offset = 25;
  $base_query = <<<EOQ
SELECT ?pid ?date FROM <#ri>
WHERE {
  ?pid <fedora-model:hasModel> <info:fedora/islandora:sp_videoCModel> ;
       <fedora-view:disseminates> ?ds .
  ?ds <fedora-view:disseminationType> <info:fedora/*/OGG> ;
      <fedora-view:lastModifiedDate> ?date
}
ORDER BY ?date
EOQ;
  $sandbox = &$context['sandbox'];
  if (isset($sandbox['last_date'])) {
    $last_date = $sandbox['last_date'];
    if ($last_date) {
      $operations_query = <<<EOQ
      SELECT ?pid ?date FROM <#ri>
    WHERE {
      ?pid <fedora-model:hasModel> <info:fedora/islandora:sp_videoCModel> ;
           <fedora-view:disseminates> ?ds .
      ?ds <fedora-view:disseminationType> <info:fedora/*/OGG> ;
          <fedora-view:lastModifiedDate> ?date
      FILTER(?date > '$last_date'^^xsd:dateTime)
    }
    ORDER BY ?date
    LIMIT $update_offset
EOQ;
    }
  }
  $connection = islandora_get_tuque_connection();

  if (!isset($sandbox['total'])) {
    $sparql_count = $connection->repository->ri->countQuery($base_query, 'sparql');
    $sandbox['total'] = $sparql_count;
    if ($sandbox['total'] === 0) {
      return;
    }
  }

  if (isset($sandbox['last_date'])) {
    $results = $connection->repository->ri->sparqlQuery($operations_query);

    $context['message'] = t('Processing results after @date.', array(
      '@date' => $sandbox['last_date'],
    ));
  }
  else {
    $context['message'] = t('Processing initial results.', array(
      '@date' => $sandbox['last_date'],
    ));
    $base_query .= <<<EOQ
  LIMIT $update_offset
EOQ;

    $results = $connection->repository->ri->sparqlQuery($base_query);
  }
  $last_date = FALSE;
  foreach ($results as $result) {
    $object = islandora_object_load($result['pid']['value']);
    $last_date = $result['date']['value'];
    $success = TRUE;
    try {
      $object->purgeDatastream('OGG');
    }
    catch (Exception $e) {
      $success = FALSE;
    }

    if ($success) {
      drush_log(dt("OGG derivative deletion succeeded for @pid.", array('@pid' => $object->id)), 'success');
    }
    else {
      drush_log(dt("OGG derivative deletion failed for @pid. Check the Drupal watchdog for detailed errors.", array('@pid' => $object->id)), 'error');
    }
  }
  if ($last_date) {
    $sandbox['last_date'] = $last_date;
  }
  $context['finished'] = empty($results);
}
