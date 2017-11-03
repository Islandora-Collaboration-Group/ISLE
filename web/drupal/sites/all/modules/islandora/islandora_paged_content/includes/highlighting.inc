<?php
/**
 * @file
 * This file contains the solr highlighting query.
 */

/**
 * Performs a Solr query, adding in some defaults we need for highlighting.
 *
 * @param string $query
 *   A Solr query string.
 * @param array $params
 *   An array of Solr parameters. Probably want to throw in at least an fq, to
 *   restrict the results to those you are interested in.
 *
 * @return array
 *   An array as per islandora_paged_content_parse_solr_highlighting().
 */
function islandora_paged_content_perform_solr_highlighting_query($query, $params = array()) {
  $query = trim($query);
  if (empty($query) || !module_exists('islandora_ocr')) {
    return array();
  }
  module_load_include('inc', 'islandora_ocr', 'includes/solr');
  $sequence_field = variable_get('islandora_paged_content_sequence_number_field', 'RELS_EXT_isSequenceNumber_literal_ms');
  $params['fl'] = "PID,{$sequence_field}";
  $solr_results = islandora_ocr_highlighted_solr_search($query, 0, -1, $params);
  $results = islandora_ocr_map_highlighted_solr_results_to_bounding_boxes($solr_results);
  foreach ($solr_results['response']['docs'] as $doc) {
    $id = $doc['PID'];
    if (isset($doc[$sequence_field])) {
      // Cast to array as it my be a multivalued field.
      $sequence_numbers = (array) $doc[$sequence_field];
      $results[$id]['page']['sequence_number'] = reset($sequence_numbers);
    }
  }
  return $results;
}
