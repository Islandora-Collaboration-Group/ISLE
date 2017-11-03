<?php
/**
 * @file
 * Batch functions for the Islandora Simple Workflow module.
 */

/**
 * Constructs the batch that will go out and publish objects.
 *
 * @param array $pids
 *   The array of pids to be published.
 *
 * @return array
 *   An array detailing the batch that is about to be ran.
 */
function islandora_simple_workflow_publish_create_batch($pids) {
  // Set up a batch operation.
  $batch = array(
    'operations' => array(
      array('islandora_simple_workflow_publish_batch_operation', array($pids)),
    ),
    'title' => t('Publishing the selected objects...'),
    'init_message' => t('Preparing to publish objects.'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'finished' => 'islandora_simple_workflow_publish_batch_finished',
    'file' => drupal_get_path('module', 'islandora_simple_workflow') . '/includes/batch.inc',
  );
  return $batch;
}

/**
 * Constructs and performs the publishing batch operation.
 *
 * @param array $pids
 *   An array of pids to be published.
 * @param array $context
 *   The context of the Drupal batch.
 */
function islandora_simple_workflow_publish_batch_operation($pids, &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['pids'] = $pids;
    $context['sandbox']['total'] = count($pids);
    $context['results']['success'] = array();
  }

  if (!empty($context['sandbox']['pids'])) {
    $target_pid = array_pop($context['sandbox']['pids']);
    $target_object = islandora_object_load($target_pid);
    $context['message'] = t('Publishing @label (@current of @total)...', array(
        '@label' => $target_object->label,
        '@current' => $context['sandbox']['progress'],
        '@total' => $context['sandbox']['total'],
    ));
    $target_object->state = 'A';
    $context['results']['success'][] = $target_pid;
    $context['sandbox']['progress']++;
  }
  $context['finished'] = ($context['sandbox']['total'] == 0) ? 1 : ($context['sandbox']['progress'] / $context['sandbox']['total']);
}

/**
 * Finished function for the simple workflow publish batch.
 *
 * @param bool $success
 *   Whether the batch was successful or not.
 * @param array $results
 *   An array containing the results of the batch operations.
 * @param array $operations
 *   The operations array that was used in the batch.
 */
function islandora_simple_workflow_publish_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results['success']), 'One object published.', '@count objects published.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}
