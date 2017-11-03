<?php

/**
 * @file
 * Functions used in the islandora_entities module
 */

/**
 * Prints out citations as rss information for the given entity.
 *
 * @param string $pid
 *   The PID of the object to create markup for.
 */
function islandora_entities_rss($pid) {
  // Change the content type to be application/rss+xml and not http.
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  $object = islandora_object_load($pid);
  $results = "";
  if ($object['MADS']) {
    $mads = $object['MADS']->content;
    $simplexml = simplexml_load_string($mads);
    $identifiers = $simplexml->identifier;
    foreach ($identifiers as $identifier) {
      if ($identifier['type'] == 'u1') {
        $results = islandora_entities_get_rss_related((string) $identifier, 'citations');
      }
    }
  }
  // Get the global base url.
  global $base_url;
  // Set the channel elements for the RSS feed.
  // The title of the RSS channel to be the site name or Drupal by default.
  $title = t('@site_name aggregator', array('@site_name' => variable_get('site_name', 'Drupal')));
  // The link to be the base url.
  $link = $base_url;
  // The description of the RSS channel.
  $description = t('Aggregated results of: @name', array('@name' => isset($object->label) ? $object->label : "Scholar"));

  // Build the xml string to be printed.
  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel($title, $link, $description, $results);
  $output .= "</rss>\n";
  // Print the xml.
  print $output;
  exit;
}

/**
 * Find citations and create rss items out of them.
 *
 * @param string $identifier
 *   Scholar's unique ID.
 * @param string $type
 *   Specified content model ('citations' or 'theses').
 *
 * @return string
 *   The string formatted RSS items.
 */
function islandora_entities_get_rss_related($identifier, $type) {
  module_load_include('inc', 'islandora_solr', 'includes/query_processor');
  global $base_url;
  $mappings = array(
    'citations' => 'ir:citationCModel',
    'theses' => 'ir:thesisCModel',
  );
  $content_model = $mappings[$type];
  $entities_linking_field = variable_get(
      'islandora_entities_linking_content_solr_field', 'mods_name_personal_displayForm_mt'
  );
  $query = "RELS_EXT_hasModel_uri_mt:\"$content_model\" AND {$entities_linking_field}:\"($identifier)\"";

  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery($query);
  $qp->solrParams['fl'] = "dc.description, dc.title, PID";
  $qp->executeQuery(FALSE);
  try {
    $results = $qp->islandoraSolrResult['response']['objects'];
  }
  catch (Exception $e) {
    watchdog_exception('Islandora Entities', $e, 'Got an exception while searching an entities citations.', array(), WATCHDOG_ERROR);
    $results = array();
  }
  $return = "";
  if ($results) {
    foreach ($results as $choice) {
      if (isset($choice['solr_doc']['dc.title'])) {
        $description = isset($choice['solr_doc']['dc.description'][0]) ? $choice['solr_doc']['dc.description'][0] : "No Description";
        $return .= format_rss_item($choice['solr_doc']['dc.title'][0], "$base_url/islandora/object/" . $choice['PID'], $description);
      }
    }
  }
  return $return;
}
