<?php

/**
 * @file
 * Batch definition and callbacks.
 */

/**
 * Get the our basic batch structure.
 *
 * @param int $jump
 *   The number of records to grab each iteration. Defaults to 100.
 * @param int $cutoff
 *   The number of records to process per batch. -1 means to attempt to process
 *   ALL records, NULL to process the default quatitiy, as specified by the
 *   'islandora_xmlsitemap_number_of_pids_to_process' variable.
 *
 * @return array
 *   An array representing the batch.
 */
function islandora_xmlsitemap_get_batch($jump = 100, $cutoff = NULL) {
  $mod_path = drupal_get_path('module', 'islandora_xmlsitemap');
  if ($cutoff === NULL) {
    $cutoff = variable_get('islandora_xmlsitemap_number_of_pids_to_process', 1000);
  }
  $batch = array(
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'operations' => array(
      array('islandora_xmlsitemap_batch_operation', array($jump, $cutoff)),
    ),
    'file' => "$mod_path/includes/batch.inc",
  );
  return $batch;
}

/**
 * Batch operation.
 *
 * Grab a number of records from Solr at a time, until we hit the cutoff
 * number.
 *
 * @param int $jump
 *   The number of records to grab each iteration.
 * @param int $cutoff
 *   The number of records to process per batch.
 */
function islandora_xmlsitemap_batch_operation($jump, $cutoff, &$context) {
  $sandbox =& $context['sandbox'];

  if (!isset($sandbox['offset'])) {
    $sandbox['offset'] = 0;

    $sandbox['last_modified'] = variable_get('islandora_xmlsitemap_last_modified_value', NULL);

    $date_field = variable_get('islandora_xmlsitemap_last_modified_field', 'fgs_lastModifiedDate_dt');

    $qp = $sandbox['query_processor'] = new IslandoraSolrQueryProcessor();
    $qp->solrLimit = $cutoff > 0 ?
      min($jump, $cutoff) :
      $jump;
    $qp->solrQuery = isset($sandbox['last_modified']) ?
      "$date_field:[{$sandbox['last_modified']} TO *]" :
      "$date_field:[* TO *]";
    $qp->solrParams['sort'] = "$date_field asc";
    $qp->solrParams['fq'] = array();
    $qp->solrParams['fl'] = 'PID';

    $namespaces_enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
    if ($namespaces_enforced) {
      $namespace_map = function ($namespace) {
        return 'PID:' . Apache_Solr_Service::escape("$namespace:") . '*';
      };
      module_load_include('inc', 'islandora', 'includes/utilities');
      $qp->solrParams['fq'][] = implode(' OR ', array_map($namespace_map, islandora_get_allowed_namespaces()));
    }
  }
  else {
    $sandbox['offset'] += $jump;
    $qp = $sandbox['query_processor'];
  }

  $qp->solrStart = $sandbox['offset'];

  // Query for $count PIDs, starting from $offset.
  $qp->executeQuery(FALSE, TRUE);
  $results = $qp->islandoraSolrResult;

  $sandbox['total'] = $cutoff > 0 ?
    min((int) $results['response']['numFound'], $cutoff) :
    (int) $results['response']['numFound'];
  if ($results === NULL || $sandbox['total'] === 0) {
    $context['message'] = t('No results selected, or errored...');
    // Stash the most current value, so we can hopefully pick up where we left
    // off last time.
    variable_set('islandora_xmlsitemap_last_modified_value', $sandbox['last_modified']);
    return;
  }

  // Add/update each result in the custom table.
  module_load_include('inc', 'islandora_xmlsitemap', 'includes/utilities');
  foreach ($results['response']['objects'] as $result) {
    islandora_xmlsitemap_add_or_update_link($result['PID'], $sandbox);
  }
  $qp->resetResults();

  $context['message'] = t('Processed @count of @total.', array(
    '@count' => min($sandbox['offset'] + $jump, $sandbox['total']),
    '@total' => $sandbox['total'],
  ));
  $context['finished'] = $sandbox['offset'] / $sandbox['total'];
  if ($context['finished'] >= 1) {
    variable_set('islandora_xmlsitemap_last_modified_value', $sandbox['last_modified']);
  }
}
