<?php

/**
 * @file
 * Batch functions for purging ingested objects via set.
 */


/**
 * Batch to purge objects ingested via a batch set.
 *
 * @param int $set_id
 *   Batch set ID.
 */
function islandora_batch_batch_purge_object_in_set($set_id) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  $batch = array(
    'title' => t('Purge objects from set'),
    'finished' => 'islandora_batch_batch_purge_object_in_set_finished',
    'init_message' => t('Initializing...'),
    'operations' => array(),
    'progress_message' => t('Time elapsed: @elapsed.'),
    'file' => drupal_get_path('module', 'islandora_batch') . '/includes/purge-set-object.batch.inc',
  );

  $batch['operations'][] = array('islandora_batch_batch_purge_object_in_set_operation', array($set_id));
  batch_set($batch);
}

/**
 * Batch operation to purge object ingested via batch.
 *
 * @param int $set_id
 *   Batch Set ID.
 *
 * @param array $context
 *   Batch context.
 */
function islandora_batch_batch_purge_object_in_set_operation($set_id, &$context) {
  module_load_include('inc', 'islandora_batch', 'includes/ingest.batch');
  module_load_include('inc', 'islandora_batch', 'includes/db');
  $sandbox =& $context['sandbox'];
  if (!islandora_batch_get_lock($set_id)) {
    // Failed to acquire the lock... Try again in another iteration...
    $context['finished'] = 0;
    return;
  }

  if (!isset($sandbox['total'])) {
    $sandbox['items'] = islandora_batch_get_queued_items_for_set($set_id);
    $sandbox['total'] = count($sandbox['items']);
    $sandbox['progress'] = 0;
    if ($sandbox['total'] === 0) {
      $context['message'] = t('Nothing to process...');
      return;
    }
  }

  $slice_size = 30;
  $slice_stop = min($sandbox['progress'] + $slice_size, $sandbox['total']);

  for ($i = $sandbox['progress']; $i < $slice_stop; $i++) {
    $purge = FALSE;
    $item_id = $sandbox['items'][$i];
    $object = islandora_object_load($item_id);
    if ($object) {
      if ($object->repository->purgeObject($object->id)) {
        $purge = TRUE;
      }
    }
    if ($purge) {
      $context['results']['purged'][] = $item_id;
    }
    else {
      $context['results']['purged_fail'][] = $item_id;
    }
  }
  $sandbox['progress'] = $i;
  $context['finished'] = $sandbox['progress'] / $sandbox['total'];
}

/**
 * Finished function for deleting ingested objects.
 *
 * @param array $success
 *   An array of success passed from the batch.
 * @param array $results
 *   An array of results passed from the batch.
 * @param array $operations
 *   An array of operations passed from the batch.
 */
function islandora_batch_batch_purge_object_in_set_finished($success, $results, $operations) {
  if (isset($results['purged'])) {
    $purged_count = count($results['purged']);
    drupal_set_message(t('Purged @count @label.', array(
      '@count' => $purged_count,
      '@label' => format_plural($purged_count, 'object', 'objects'),
    )));
  }
  else {
    drupal_set_message(t('No objects were purged'));
  }
  if (isset($results['purged_fail'])) {
    $file_uri = drupal_tempnam('public://', 'batch-set-object-purge-fail-');
    $file = new stdClass();
    $file->uri = $file_uri;
    $file->filename = drupal_basename($file_uri);
    $file->filemime = 'application/octet-stream';
    $file->status = 0;
    file_put_contents($file_uri, implode("\n", $results['purged_fail']));
    file_save($file);

    $failed_count = count($results['purged_fail']);
    drupal_set_message(t('Failed to purge @count @label. Download report <a href="@report_url">here</a>.', array(
      '@count' => $failed_count,
      '@label' => format_plural($failed_count, 'object', 'objects'),
      '@report_url' => file_create_url($file_uri),
    )), 'warning');
  }
}
