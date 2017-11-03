<?php

/**
 * @file
 * This file contains all the batch operations for collection objects.
 */

require_once dirname(__FILE__) . '/utilities.inc';

/**
 * Starts the batch operation.
 *
 * @param array $operands
 *   All the objects to operate on.
 * @param array $context
 *   The context of this batch operation.
 */
function islandora_webform_ingest_start_operation(array $operands, array &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = count($operands);
  }
  return @$operands[$context['sandbox']['progress']];
}

/**
 * Ends the batch operation.
 *
 * @param array $context
 *   The context of this batch operation.
 *
 * @return bool
 *   TRUE or FALSE depending on the context of the batch.
 */
function islandora_webform_ingest_end_operation(array &$context) {
  $context['sandbox']['progress']++;
  if ($context['sandbox']['progress'] < $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['total'];
    return FALSE;
  }
  return TRUE;
}

/**
 * The batch 'finished' callback.
 */
function islandora_webform_ingest_batch_finished($success, $results, $operations) {
  if (!$success) {
    // Generic Error Message.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
}

/**
 * Delete the given child objects within the collection.
 *
 * @param string $pid
 *   The pid of the object to purge the children from.
 *
 * @return array
 *   The batch definition.
 */
function islandora_webform_ingest_delete_related_batch($pid) {
  $batch = array(
    'finished' => 'islandora_webform_ingest_batch_finished',
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_webform_ingest') . '/includes/batch.inc',
  );


  $message_parameters = array('@object' => $pid);
  $batch += array(
    'operations' => array(
      array(
        'islandora_webform_ingest_delete_all_related_batch_operation',
        array($pid),
      ),
    ),
    'title' => t('Deleting all objects related to @object.', $message_parameters),
    'init_message' => t('Preparing to delete all objects related to @object.', $message_parameters),
  );

  return $batch;
}

/**
 * Deletes all children from the given object.
 *
 * @param string $pid
 *   The parent object to purge the children from.
 * @param array $context
 *   The batch context.
 */
function islandora_webform_ingest_delete_all_related_batch_operation($pid, array &$context) {
  if (!isset($context['sandbox']['total_count'])) {
    $context['sandbox']['deleted'] = 0;
    $context['sandbox']['total_count'] = count(iwi_query_related_objects($pid));
    if ($context['sandbox']['total_count'] === 0) {
      $context['finished'] = 1;
      return;
    }
    $context['sandbox']['current'] = 0.0;
  }

  $relations = db_select('islandora_webform_ingest_webforms', 'w')
    ->distinct()
    ->fields('w', array('relation'))
    ->execute()
    ->fetchCol();
  $predicate_filters = array();
  foreach ($relations as $relation) {
    $predicate_filters[] = 'fedora-system:def/relations-external#' . $relation;
  }
  $deleted_pids = array();

  // Each time through the batch, get first 10 objects.
  $query_results = iwi_query_related_objects($pid, 0, 10);
  $count = count($query_results);
  foreach ($query_results as $queryresult) {
    $object = islandora_object_load($queryresult['object']['value']);
    $this_pid = $object->id;
    $this_label = $object->label;
    if (islandora_object_access(ISLANDORA_PURGE, $object) && in_array($queryresult['predicate']['value'], $predicate_filters)) {
      islandora_delete_object($object);
      $deleted_pids[] = $this_pid;
      watchdog('islandora_webform_ingest', 'Deleted related object "@label" (PID=@pid).', array('@label' => $this_label, '@pid' => $this_pid), WATCHDOG_NOTICE);
    }
    else {
      watchdog('islandora_webform_ingest', 'Permission denied to delete related object "@label" (PID=@pid).', array('@label' => $this_label, '@pid' => $this_pid), WATCHDOG_ERROR);
    }

  }

  if (count($deleted_pids) > 0) {
    // Remove iw-ingestion records for all child objects.
    db_delete('islandora_webform_ingestions')
      ->condition('pid', $deleted_pids)
      ->execute();
  }

  $context['sandbox']['current'] += $count;
  $context['sandbox']['deleted'] += count($deleted_pids);
  if ($count === 0) {
    $context['finished'] = 1;
  }
  else {
    $context['finished'] = $context['sandbox']['current'] / $context['sandbox']['total_count'];
  }

  if (!$query_results || $context['finished'] == 1) {
    drupal_set_message(format_plural($context['sandbox']['deleted'],
      'Deleted 1 child from <a href="@collection-url">@collection</a>.',
      'Deleted @count children from <a href="@collection-url">@collection</a>.',
      array(
        '@collection' => $pid,
        '@collection-url' => url("islandora/object/{$pid}"),
      )
    ));
  }
}
