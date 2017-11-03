<?php

/**
 * @file
 * Drush hooks.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_batch_drush_command() {
  $items = array();

  $items['islandora_batch_ingest'] = array(
    'aliases' => array('ibpro', 'ibi'),
    'description' => 'Process and ingest preprocessed entries.',
    'drupal dependencies' => array('islandora_batch'),
    'examples' => array(
      'drush -v --user=admin --uri=http://digital.library.yorku.ca islandora_batch_ingest',
    ),
    'options' => array(
      'timeout' => array(
        'description' => 'The max amount of time (in seconds) for which we ' .
        'should process.  When the time is exceeded, the current object ' .
        'will complete before terminating execution. If not provided, ' .
        'should process until all available preprocessed objects are ' .
        'exhausted.',
        'value' => 'optional',
      ),
      'ingest_set' => array(
        'description' => 'Identify a particular set to process. The default is to process ALL objects in the queue, independent of sets.',
        'value' => 'optional',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  $items['islandora_batch_scan_preprocess'] = array(
    'aliases' => array('ibsp'),
    'description' => 'Preprocess assets by scanning either a directory or a ' .
    'ZIP archive.',
    'drupal dependencies' => array('islandora_batch'),
    'examples' => array(
      format_string('drush -v --user=admin --uri=http://digital.library.yorku.ca islandora_batch_scan_preprocess --content_models=islandora:sp_large_image_cmodel --parent=yul:F0433 --parent_relationship_pred=isMemberOfCollection --type=directory --@target=/tmp/batch_ingest',
      array(
        '@target' => DRUSH_VERSION >= 7 ? 'scan_target' : 'target',
      )),
    ),
    'options' => array(
      'type' => array(
        'description' => 'Either "directory" or "zip". The zip importer is' .
        " unstable with large files (~2GB). Consider alternative methods" .
        " such as unzipping your Zip file and using Islandora Batch's" .
        "`--directory` option.",
        'required' => TRUE,
      ),
      'content_models' => array(
        'description' => 'Supports one or multiple comma-separated content ' .
        'models which are all applied to each ingested object.',
        'value' => 'optional',
      ),
      'parent' => array(
        'description' => 'The collection to which the generated items should ' .
        'be added. Defaults to the root Islandora repository PID.',
        'value' => 'optional',
      ),
      'parent_relationship_uri' => array(
        'description' => 'The namespace URI of the relationship to the parent.' .
        ' Defaults to "info:fedora/fedora-system:def/relations-external#".',
        'value' => 'optional',
      ),
      'parent_relationship_pred' => array(
        'description' => 'The predicate of the relationship to the parent. ' .
        'Defaults to "isMemberOfCollection".',
        'value' => 'optional',
      ),
      'namespace' => array(
        'description' => 'Namespace of objects to create. ' .
        'Defaults to namespace specified in Fedora configuration.',
        'value' => 'optional',
      ),
      'zip_encoding' => array(
        'description' => 'The encoding of filenames contained in ZIP ' .
        'archives:Only relevant with --scan_target=zip. Defaults to the native ' .
        'encoding being used by PHP.',
        'value' => 'optional',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  // XXX: The target parameter became reserved in Drush 7 and above, for
  // backwards compatibility both will be supported. Not using
  // strict-option-handling (http://www.drush.org/en/master/strict-options) as
  // it requires manual argument parsing.
  if (DRUSH_VERSION >= 7) {
    $items['islandora_batch_scan_preprocess']['options']['scan_target'] = array(
      'description' => 'The target to directory or zip file to scan. Requires the full path to your archive from root directory. e.g. /var/www/drupal/sites/archive.zip',
      'required' => TRUE,
    );
  }
  else {
    $items['islandora_batch_scan_preprocess']['options']['target'] = array(
      'description' => 'The target to directory or zip file to scan. Requires the full path to your archive from root directory. e.g. /var/www/drupal/sites/archive.zip',
      'required' => TRUE,
    );
  }
  $items['islandora_batch_cleanup_processed_sets'] = array(
    'aliases' => array('ibcps'),
    'description' => dt('Cleans up processed sets that have existed greater than a specified time.'),
    'drupal dependencies' => array('islandora_batch'),
    'examples' => array(
      'drush -v -u1 islandora_batch_cleanup_processed_sets --time=1438179447',
    ),
    'options' => array(
      'time' => array(
        'description' => dt('The amount of time since completion to compare against. The timestamp to be used to query against. Can use date +%s on the command line to get the current time.'),
        'required' => TRUE,
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Implements drush_COMMAND_validate() for islandora_batch_scan_preprocess.
 */
function drush_islandora_batch_scan_preprocess_validate() {
  module_load_include('inc', 'islandora_batch', 'includes/utilities');
  $encoding_check = islandora_batch_scan_check_encoding(drupal_strtolower(drush_get_option('type')), drush_get_option('zip_encoding'));
  if ($encoding_check !== NULL) {
    drush_set_error('INVALID ENCODING', $encoding_check);
  }

  $path = drush_get_option('scan_target') ? drush_get_option('scan_target') : drush_get_option('target');
  $target_checks = islandora_batch_scan_check_target(drupal_strtolower(drush_get_option('type')), $path);
  if (count($target_checks)) {
    foreach ($target_checks as $target_check) {
      drush_set_error($target_check['error'], NULL, $target_check['label']);
    }
  }

  $cmodels = explode(',', drush_get_option('content_models'));
  foreach ($cmodels as $cmodel) {
    if (!islandora_object_load($cmodel)) {
      drush_set_error('INVALID CONTENT MODEL', NULL, dt('"!cmodel" is not a valid content model. ', array('!cmodel' => $cmodel)));
    }
  }
  $parent = drush_get_option('parent');
  if (!islandora_object_load($parent)) {
    drush_set_error('INVALID PARENT', NULL, dt('"!parent" does not exist or is not accessible. ', array('!parent' => $parent)));
  }
}

/**
 * Implements hook_islandora_batch_scan_preprocess().
 *
 * Builds a preprocessor, and passes it off to a preprocessor handler.
 */
function drush_islandora_batch_scan_preprocess() {
  // XXX: Due to how Drush bootstrapping works, the connection may be created
  // without credentials (when your site's front page is
  // 'islandora/object/some:object', for example). Resetting to ensure a new
  // connection gets created should fix it.
  drupal_static_reset('islandora_get_tuque_connection');
  $connection = islandora_get_tuque_connection();

  $parameters = array(
    'type' => drush_get_option('type'),
    'target' => DRUSH_VERSION >= 7 ? drush_get_option('scan_target') : drush_get_option('target'),
    'parent' => drush_get_option('parent', variable_get('islandora_repository_pid', 'islandora:root')),
    'parent_relationship_uri' => drush_get_option('parent_relationship_uri', 'info:fedora/fedora-system:def/relations-external#'),
    'parent_relationship_pred' => drush_get_option('parent_relationship_pred', 'isMemberOfCollection'),
    'namespace' => drush_get_option('namespace'),
    'zip_encoding' => drush_get_option('zip_encoding'),
  );

  if ($content_models = drush_get_option('content_models', FALSE)) {
    $parameters['content_models'] = explode(',', $content_models);
  }
  else {
    $parameters['content_models'] = array();
  }

  // Could use a drush_multiple_choice() or whatever to get the list of
  // content models, if none were provided?
  $preprocessor = new IslandoraScanBatch($connection, $parameters);

  // Pass the preprocessor off to run.
  $preprocessed = islandora_batch_handle_preprocessor($preprocessor);
  drush_log(t("SetId: @s", array('@s' => $preprocessor->getSetId())), "ok");
}

/**
 * Implements hook_islandora_batch_ingest().
 *
 * Ingests for the specified amount of time, if there is anything already
 * preprocessed.
 */
function drush_islandora_batch_ingest() {
  $parameters = array(
    'timeout' => drush_get_option('timeout', NULL),
    'ingest_set' => drush_get_option('ingest_set', NULL),
  );

  $parameters = array_filter($parameters);

  islandora_batch_set_operations($parameters);

  drush_backend_batch_process();
}

/**
 * Command callback; clean up processed sets.
 */
function drush_islandora_batch_cleanup_processed_sets() {
  $batch = array(
    'operations' => array(
      array(
        'islandora_batch_cleanup_processed_sets_batch_operation',
        array(
          drush_get_option('time', TRUE),
        ),
      ),
    ),
  );
  batch_set($batch);
  drush_backend_batch_process();
}

/**
 * Batch operation; clean up processed sets greater than a duration.
 */
function islandora_batch_cleanup_processed_sets_batch_operation($time, &$context) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  $sandbox =& $context['sandbox'];

  if (!isset($sandbox['total'])) {
    $sets_to_cleanup = islandora_batch_get_sets($time);
    $sandbox['progress'] = 0;
    $sandbox['total'] = count($sets_to_cleanup);
    if ($sandbox['total'] == 0) {
      // Nothing to process.
      $context['finished'] = 1;
      $context['message'] = t('No sets to cleanup.');
      return;
    }
    $sandbox['set_stash'] = $sets_to_cleanup;
  }
  $set = array_shift($sandbox['set_stash']);
  islandora_batch_delete_set($set);
  $sandbox['progress'] = min($sandbox['total'], $sandbox['progress'] + 1);
  $context['finished'] = $sandbox['progress'] / $sandbox['total'];
  $context['message'] = t('Cleaned up set @progress of @total.', array(
    '@progress' => $sandbox['progress'],
    '@total' => $sandbox['total'],
  ));
}
