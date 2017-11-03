<?php

/**
 * @file
 * Drush integration file for the Islandora Checksum Checker module.
 *
 * Drupal 7's cron can only be run as an anonymous user. The Islandora
 * Checksum Checker module uses Fedora's API-M to retrieve information
 * about datastreams and to update objects. Anonymous Drupal users may
 * not have sufficient privileges to perform these tasks. This drush
 * script offers an alternative to having Drupal 7 cron populate and
 * process the Islandora Checksum Checker job queue. It uses configuration
 * options defined in the Islandora Checksum Checker module's admin
 * settings.
 *
 * This script should be run as a Linux cron job with the drush --user
 * parameter indicating an Islandora user with sufficient privileges to
 * access all objects in your Fedora repository and the --root parameter
 * indicating the path to your Drupal installation root.
 *
 * A typical Linux crontab entry (in this case, to run every hour) for
 * this drush script is ('\' at end of first line is to break command
 * to meet Drupal's 80-character line limitation):
 *
 * 0 * * * * /usr/bin/drush --root=/var/www --user=fedoraAdmin \
 *   run-islandora-checksum-queue
 */

/**
 * Implements hook_drush_help().
 */
function islandora_checksum_checker_drush_help($command) {
  switch ($command) {
    case 'drush:run-islandora-checksum-queue':
      return dt('Populate and process items in the Islandora checksum validation queue. ' .
        'Processes the number of items specified in the module admin page, unless the ' .
        'days-to-complete and cmd-run-frequency optional parameters are passed in. If the optional ' .
        'parameters are passed in then the command will attempt to process the correct number of items.');
  }
}

/**
 * Implements hook_drush_command().
 */
function islandora_checksum_checker_drush_command() {
  $items = array();
  $items['run-islandora-checksum-queue'] = array(
    'description' => dt('Populate and process items in the Islandora checksum validation queue.'),
    'examples' => array(
      'Standard example' => 'drush --user=fedoraAdmin run-islandora-checksum-queue',
      'Alias example' => 'drush --user=fedoraAdmin ricq',
      'Options example' => 'drush --user=fedoraAdmin --days-to-complete=180 --cmd-run-frequency=12 run-islandora-checksum-queue',
    ),
    'aliases' => array('ricq'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    'options' => array(
      'days-to-complete' => array(
        'description' => 'The number of days it should take to check all the checksums.',
        'example-value' => 365,
        'value' => 'required',
      ),
      'cmd-run-frequency' => array(
        'description' => 'The number of hours between checksum runs (required if --days-to-complete is set).',
        'example-value' => 12,
        'value' => 'required',
      ),
    ),
  );
  return $items;
}

/**
 * Validation function for drush run-islandora-checksum-queue.
 */
function drush_islandora_checksum_checker_run_islandora_checksum_queue_validate() {
  if (variable_get('islandora_checksum_checker_queue_cron_method', 'drupal') != 'drush') {
    return drush_set_error('CHECKSUM_CHECKER_NOT_ENABLED', dt('Please enable the drush method in the module config.'));
  }

  $days = drush_get_option('days-to-complete');
  $hours = drush_get_option('cmd-run-frequency');

  if ($days != NULL && (!is_numeric($days) || intval($days) != $days || $days <= 0)) {
    return drush_set_error('CHECKSUM_CHECKER_DAYS', dt('The option --days-to-complete must be a positive integer.'));
  }

  if ($hours != NULL && (!is_numeric($hours) || intval($hours) != $hours || $hours <= 0)) {
    return drush_set_error('CHECKSUM_CHECKER_DAYS', dt('The option --cmd-run-frequency must be a positive integer.'));
  }

  if ($hours == NULL XOR $days == NULL) {
    return drush_set_error('CHECKSUM_CHECKER_OPTIONS', dt('Both --days-to-complete and --cmd-run-frequency must be specified, or neither specified.'));
  }
}

/**
 * Callback function for drush run-islandora-checksum-queue.
 */
function drush_islandora_checksum_checker_run_islandora_checksum_queue() {
  $days = drush_get_option('days-to-complete');
  $hours = drush_get_option('cmd-run-frequency');
  $time_based_items = $days AND $hours;

  // Populate the queue with the next $objects_to_check object PIDs.
  $queue = DrupalQueue::get('validateIslandoraChecksums');
  $items_still_in_queue = $queue->numberOfItems();

  if ($time_based_items) {
    $repository_items = islandora_checksum_checker_get_number_objects();
    $limit = ceil(($repository_items / $days) * ($hours / 24));
  }
  else {
    $limit = variable_get('islandora_checksum_checker_items_per_cron', '50');
    if ($limit > $items_still_in_queue) {
      $limit = (int) $limit - (int) $items_still_in_queue;
    }
  }

  $objects_to_check = islandora_checksum_checker_get_objects($limit);

  foreach ($objects_to_check as $object) {
    $queue->createItem($object);
  }

  // Process the items just added to the queue.
  while ($item = $queue->claimItem()) {
    // $item->data will be an Islandora object's PID.
    if (islandora_checksum_checker_validate_checksum($item->data)) {
      // If success, remove the item from queue.
      $queue->deleteItem($item);
    }
    else {
      // Log failure to watchdog.
      watchdog('islandora_checksum_checker', 'Checksums not verified for !object, will attempt again.',
        array('!object' => $item->data), WATCHDOG_WARNING);
      // Release, not delete the item so it remains in the queue.
      $queue->releaseItem($item);
    }
  }
  islandora_checksum_checker_send_mismatch_notice();
}
