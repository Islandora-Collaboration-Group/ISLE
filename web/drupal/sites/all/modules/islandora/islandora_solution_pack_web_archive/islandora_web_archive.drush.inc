<?php

/**
 * @file
 * Implementation of Drush hooks.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_web_archive_drush_command() {
  return array(
    'islandora_web_archive_migrate_png_to_screenshot' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_MAX,
      'options' => array(
        'limit' => array(
          'value' => 'optional',
          'description' => dt('The max number of objects for which to query at a time. (affects memory usage, defaults to 100)'),
        ),
      ),
      'drupal_dependencies' => array(
        'islandora',
      ),
      'description' => 'Iterates each web archive object and copies the PNG datastream into a SCREENSHOT datastream.',
      'examples' => array(
        'drush -u 1 islandora_web_archive_migrate_png_to_screenshot',
        'drush -u 1 islandora_web_archive_migrate_png_to_screenshot --limit=200',
      ),
    ),
  );
}

/**
 * Implements hook_drush_command().
 */
function drush_islandora_web_archive_migrate_png_to_screenshot() {
  $batch = array(
    'operations' => array(
      array(
        'islandora_web_archive_migrate_png_to_screenshot_operation',
        array(
          drush_get_option('limit', 100),
        ),
      ),
    ),
  );
  batch_set($batch);
  drush_backend_batch_process();
}

/**
 * Batch operation; copy a PNG datastream into a SCREENSHOT datastream.
 */
function islandora_web_archive_migrate_png_to_screenshot_operation($limit, &$context) {
  $sandbox =& $context['sandbox'];
  drupal_static_reset('islandora_get_tuque_connection');
  $tuque = islandora_get_tuque_connection();
  // Get all islandora web archive objects.
  $query = <<<EOQ
SELECT ?object ?created
FROM <#ri>
WHERE {
  ?object <fedora-view:disseminates> ?ds ;
          <fedora-model:createdDate> ?created ;
          <fedora-model:hasModel> <info:fedora/islandora:sp_web_archive> .
  ?ds <fedora-view:disseminationType> <info:fedora/*/PNG> .
  !filter
}
ORDER BY ?created

EOQ;
  if (!isset($sandbox['total'])) {
    $sandbox['progress'] = 0;
    $sandbox['total'] = $tuque->repository->ri->countQuery(format_string($query, array(
      '!filter' => '',
    )), 'sparql');
    if ($sandbox['total'] == 0) {
      // Nothing to process.
      $context['finished'] = 1;
      $context['message'] = t('Nothing to fix.');
      return;
    }
    $sandbox['result_stash'] = array();
  }

  if (empty($sandbox['result_stash'])) {
    $limited_query = format_string($query, array(
        '!filter' => (isset($sandbox['last_date']) ?
          "FILTER(?created > \"{$sandbox['last_date']}\"^^<xsd:dateTime>)" :
          ''),
      )) . <<<EOQ
LIMIT $limit
EOQ;
    drush_print(dt("\nQuerying for @count objects.", array('@count' => $limit)));
    $sandbox['result_stash'] = $tuque->repository->ri->sparqlQuery($limited_query);
    if (empty($sandbox['result_stash'])) {
      // Ran out of items early?
      $context['finished'] = 1;
      $context['message'] = t('Updated @count PID(s) and ran out of items early... Somebody manually updated a document?', array(
        '@count' => $sandbox['total'],
      ));
      return;
    }
  }
  $result = array_shift($sandbox['result_stash']);
  $updating_pid = $result['object']['value'];
  $sandbox['last_date'] = $result['created']['value'];
  $object_to_update = islandora_object_load($updating_pid);
  if (!isset($object_to_update['SCREENSHOT'])) {
    module_load_include('inc', 'islandora_web_archive', 'includes/utilities');
    islandora_web_archive_clone_datastream($object_to_update, $object_to_update['PNG'], 'SCREENSHOT');
    drush_print(dt("Successfully migrated !SRC to !DEST datastream for object: !PID.", array(
      '!PID' => $updating_pid,
      '!SRC' => 'PNG',
      '!DEST' => 'SCREENSHOT',
    )));
  }
  else {
    drush_print(dt("No migration required !DS datastream already exists for object: !PID.", array(
      '!PID' => $updating_pid,
      '!DS' => 'SCREENSHOT',
    )));
  }
  $sandbox['progress'] = min($sandbox['total'], $sandbox['progress'] + 1);
  $context['finished'] = $sandbox['progress'] / $sandbox['total'];
  $context['message'] = t('Processed @progress of @total.', array(
    '@progress' => $sandbox['progress'],
    '@total' => $sandbox['total'],
  ));
}
