<?php
/**
 * @file
 * Menu alter hook callbacks.
 */

/**
 * Menu alter replacement callback for islandora/search.
 *
 * @param string $term
 *   The search term to track.
 */
function islandora_usage_stats_track_search_results($term = NULL) {
  module_load_include('module', 'islandora_solr', "islandora_solr");
  if (!empty($term)) {
    module_load_include('inc', 'islandora_usage_stats', 'includes/utilities');
    islandora_usage_stats_track_search_term($term);
  }
  return islandora_solr($term);
}

/**
 * Menu alter replacement callback for ds downloads.
 *
 * @param AbstractDatastream $datastream
 *   The datastream being downloaded.
 */
function islandora_usage_stats_track_downloads(AbstractDatastream $datastream) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora_usage_stats', 'includes/utilities');
  islandora_usage_stats_log_datastream_download($datastream->parent->id, $datastream->id);
  islandora_download_datastream($datastream);
}
