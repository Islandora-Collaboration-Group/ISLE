<?php

/**
 * @file
 * Drush command/hook implementation for updating existing citation objects.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_scholar_drush_command() {
  $commands = array();

  $commands['islandora-scholar-update-citations'] = array(
    'description' => dt('Update existing citations to generate PDF derivatives for facilitating new theme changes. Any existing PDF derivatives on citation objects will not be overwritten. As such, subsquent runs of this script will not overwrite existing content.'),
    'drupal dependencies' => array(
      'islandora',
      'islandora_scholar',
      'imagemagick',
    ),
    'options' => array(
      'force' => array(
        'description' => 'Whether we are forcing the creation of derivatives or not.',
      ),
    ),
    'examples' => array(
      'drush -u 1 islandora-scholar-update-citations' => dt('Updating existing citations with PDF derivatives.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $commands;
}

/**
 * Command callback to update citations with PDF derivatives.
 */
function drush_islandora_scholar_update_citations() {
  batch_set(islandora_scholar_citation_update_create_batch());
  drush_backend_batch_process();
}

/**
 * Constructs a batch used to update things via Drush.
 */
function islandora_scholar_citation_update_create_batch() {
  return array(
    'operations' => array(
      array('islandora_scholar_update_citation_batch_operation', array()),
    ),
    'title' => t('Updating PDF derivatives for citations...'),
    'init_message' => t('Preparing to update derivatives.'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_scholar') . '/islandora_scholar.drush.inc',
  );
}

/**
 * Constructs and performs the citation batch operation.
 *
 * @param array $context
 *   The context of the Drupal batch.
 */
function islandora_scholar_update_citation_batch_operation(&$context) {
  $citation_update = 10;
  $query = <<<EOQ
SELECT ?pid FROM <#ri>
WHERE {
  ?pid <fedora-model:hasModel> <info:fedora/ir:citationCModel> ;
       <fedora-view:disseminates> ?ds .
  ?ds <fedora-view:disseminationType> <info:fedora/*/PDF> .
}
EOQ;
  $connection = islandora_get_tuque_connection();

  $sandbox = &$context['sandbox'];
  if (!isset($sandbox['offset'])) {
    $sparql_count = $connection->repository->ri->countQuery($query, 'sparql');
    $sandbox['offset'] = 0;
    $sandbox['total'] = $sparql_count;
    if ($sandbox['total'] === 0) {
      return;
    }
  }

  $context['message'] = t('Processing results @start to @end.', array(
    '@start' => $sandbox['offset'],
    '@end' => min($sandbox['offset'] + $citation_update, $sandbox['total']),
  ));

  $offset_start = $sandbox['offset'];
  $query .= "
  LIMIT $citation_update
  OFFSET $offset_start
  ";
  module_load_include('inc', 'islandora', 'includes/derivatives');
  $results = $connection->repository->ri->sparqlQuery($query);
  foreach ($results as $result) {
    $object = islandora_object_load($result['pid']['value']);
    $derivative_results = islandora_do_derivatives($object, array(
      'force' => drush_get_option('force', FALSE),
      'source_dsid' => 'PDF',
    ));
    $success = TRUE;
    foreach ($derivative_results as $log) {
      if (!$log['success']) {
        $success = FALSE;
        break;
      }
    }
    if ($success) {
      drush_log(dt("PDF derivative creation succeeded for @pid.", array('@pid' => $object->id)), 'success');
    }
    else {
      drush_log(dt("PDF derivative creation failed for @pid. Check the Drupal watchdog for detailed errors.", array('@pid' => $object->id)), 'error');
    }
  }
  $sandbox['offset'] += $citation_update;
  $context['finished'] = $sandbox['offset'] / $sandbox['total'];
}
