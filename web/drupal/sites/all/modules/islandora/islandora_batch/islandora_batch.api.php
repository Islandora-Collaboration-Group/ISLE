<?php

/**
 * @file
 * This file documents all available hook functions to manipulate data.
 */

/**
 * A hook to allow for events directly after a islandora batch has finished.
 *
 * @param array $sets
 *   An array of batch set ids that have successfully ingested all objects at
 *   the end of batch processing.
 */
function hook_islandora_batch_process_finished($sets) {
  if (!empty($sets)) {
    module_load_include('inc', 'islandora_batch', 'includes/db');
    foreach ($sets as $set) {
      // Do something to the set, in this example it is completely deleting it
      // from the drupal database.
      islandora_batch_delete_set($set);
    }
  }
}

/**
 * A hook to allow for events after an item has been processed.
 *
 * @param AbstractObject $ingest_object
 *   The object that has been processed for ingestion.
 * @param int $state
 *   The the state after ingestion attempt. 1 for successful ingestion, 0 for
 *   failed ingestion.
 */
function hook_islandora_batch_object_processed($ingest_object, $state) {
  // Send an email when an object fails batch processing.
  $site_email = variable_get('site_mail', '');
  if ($state === 0 && $site_email) {
    $message = drupal_mail('islandora_batch_report', 'islandora_batch_failed_ingest', $site_email, language_default(), array(), NULL, FALSE);
    $message['subject'] = 'Object Failed Ingesting';
    $message['body'] = array();
    $message['body'][] = 'Failed to ingest object ' . $ingest_object->label . ' with PID ' . $ingest_object->id;

    // Retrieve the responsible implementation for this message.
    $system = drupal_mail_system($module, $key);

    // Format the message body.
    $message = $system->format($message);

    // Send e-mail.
    $message['result'] = $system->mail($message);
  }
}
