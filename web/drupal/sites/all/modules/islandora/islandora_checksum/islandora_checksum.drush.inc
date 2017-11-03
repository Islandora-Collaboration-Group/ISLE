<?php

/**
 * @file
 * Drush command for enabling checksums.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_checksum_drush_command() {
  return array(
    'islandora-checksum-update-checksums' => array(
      'aliases' => array('icuc'),
      'description' => dt('Updates the checksumType setting for datastreams of objects in the given collection.'),
      'drupal dependencies' => array(
        'islandora_checksum',
      ),
      'options' => array(
        'collections' => array(
          'description' => dt('A comma-separated list of collection PIDs to update checksums in.'),
          'required' => TRUE,
        ),
        'dsids' => array(
          'description' => dt('A comma-separated list of DSIDs to update on the given collection. Omit to use the default configured ones.'),
          'required' => FALSE,
        ),
        'checksum' => array(
          'description' => dt('The checksum configuration to apply to the datastreams found. Omit to use the default configured value. Valid options are DISABLED, MD5, SHA-1, SHA-256, SHA-384, and SHA-512.'),
          'required' => FALSE,
        ),
        'limit' => array(
          'description' => dt('The number of checksums to apply in each iteration of the batches. Defaults to 10.'),
          'required' => FALSE,
        ),
      ),
      'examples' => array(
        'Update basic image checksums to SHA-512' => 'drush islandora-checksum-update-checksums --collection=islandora:sp_basic_image_collection --checksum=SHA-512',
      ),
    ),
  );
}

/**
 * Sets the batch for updating checksums.
 */
function drush_islandora_checksum_update_checksums() {
  $collection_pids = explode(',', trim(drush_get_option('collections')));
  $dsids = drush_get_option('dsids', array());
  if (!is_array($dsids)) {
    $dsids = explode(',', trim($dsids));
  }
  $limit = drush_get_option('limit', 10);
  $batch = FALSE;
  foreach ($collection_pids as $collection_pid) {
    $collection = islandora_object_load($collection_pid);
    if (!$collection) {
      drush_print(dt('No such collection !id; skipping ...', array(
        '!id' => $collection_pid,
      )));
    }
    else {
      batch_set(islandora_checksum_get_collection_checksum_update_batch($collection_pid, $limit, $dsids));
      $batch = TRUE;
    }
  }
  if ($batch) {
    drush_backend_batch_process();
  }
}
