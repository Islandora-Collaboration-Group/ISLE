<?php

/**
 * @file
 * Batch used to delete a newspaper object and all its children.
 */

/**
 * Deletes all issues and pages from a newspaper object within batches.
 *
 * @param string $pid
 *   The pid of the newspaper object which we are deleting.
 */
function islandora_newspaper_delete_newspaper_batch_operation($pid) {
  module_load_include('inc', 'islandora_newspaper', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/batch');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/datastream');
  $object = islandora_object_load($pid);
  $get_pid = function($o) {
    return $o['pid'];
  };
  $issues = islandora_newspaper_get_issues($object);

  foreach ($issues as $issue) {
    $issue_object = islandora_object_load($issue['pid']);
    $pages = array_values(array_map($get_pid, islandora_paged_content_get_pages($issue_object)));
    $batch_delete = islandora_paged_content_delete_pages_batch($object, $pages);
    $batch_delete['operations'][] = array('islandora_paged_content_delete_parent_object_operation', array($issue_object->id));
    batch_set($batch_delete);
  }
  batch_set(islandora_newspaper_delete_newspaper_object_batch($pid));
}

/**
 * The main batch operation from which the other batch operations start from.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 *
 * @return array
 *   An array defining the newspaper object and children deletion batch.
 */
function islandora_newspaper_delete_newspaper_batch(AbstractObject $object) {
  return array(
    'operations' => array(
      array('islandora_newspaper_delete_newspaper_batch_operation', array(
        $object->id)),
    ),
    'title' => t('Deleting newspaper object and all its children'),
    'init_message' => t('Preparing to delete newspaper object and all its children...'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_newspaper') . '/includes/batch.inc',
  );
}

/**
 * Creates a batch to go out and delete the newspaper object.
 *
 * @param string $pid
 *   The pid of the newspaper object to be deleted.
 *
 * @return array
 *   An array defining the newspaper object deletion batch.
 */
function islandora_newspaper_delete_newspaper_object_batch($pid) {
  return array(
    'operations' => array(
      array('islandora_newspaper_delete_newspaper_object', array($pid)),
    ),
    'title' => t('Deleting newspaper object'),
    'init_message' => t('Preparing to delete newspaper object...'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_newspaper') . '/includes/batch.inc',
  );
}

/**
 * Utility function to delete the newspaper object.
 *
 * @param string $pid
 *   The pid of the newspaper object being deleted.
 */
function islandora_newspaper_delete_newspaper_object($pid) {
  $object = islandora_object_load($pid);
  islandora_delete_object($object);
}
