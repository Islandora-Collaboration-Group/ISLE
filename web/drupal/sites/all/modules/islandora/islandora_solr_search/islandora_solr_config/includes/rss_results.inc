<?php

/**
 * @file
 * Contains methods to search solr and display results.  depends on
 * Apache_Solr_Php client.
 */

/**
 * Extention of IslandoraSolrResults for templating purposes.
 * This overrides the displayResults function to provide an alternate display
 * type.
 */

class IslandoraSolrResultsRSS extends IslandoraSolrResults {

  /**
   * Outputs results basically in the normal way.
   *
   * Thumbnails pulled from the Fedora repository.
   *
   * @param object $islandora_solr_query
   *   A solr query object.
   *
   * @return string
   *   html output for the resultset. Note: we currently create this
   *   output manually, should refactor to use drupal forms api.
   */
  public function printRSS($islandora_solr_query, $title = "Search Results") {

    global $base_url;

    drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');

    // Get raw results.
    $solr_result = $islandora_solr_query->islandoraSolrResult;

    // All results.
    $docs = $solr_result['response']['objects'];

    // Loop over results.
    $items = NULL;
    foreach ($docs as $doc) {
      // Turn arrays into strings.
      foreach ($doc['solr_doc'] as $key => $value) {
        if (is_array($value)) {
          // Turn array into comma separated string and trim.
          $doc[$key] = trim(implode(', ', $value));
        }
        else {
          // Give it a trim.
          $doc[$key] = trim($value);
        }
      }
      // Get the variables for the <item> element.
      $item = $this->rssItem($doc);

      // Hook alter to change the rssItem before formatting.
      drupal_alter('islandora_solr_search_rss_item', $item, $doc);

      // Render rss item.
      $rendered_item = format_rss_item($item['title'], $item['link'], $item['description'], $item['items']);

      // ... allow it to be altered...
      drupal_alter('islandora_solr_config_rss_item_post_render', $rendered_item, $doc);

      // ... and add to items string.
      $items .= "$rendered_item\n";
    }

    // Query search terms:
    $query = $islandora_solr_query->solrQuery;

    // Get the variables for the <channel> element.
    $channel = $this->rssChannel($query);
    $rss_attributes = array('version' => '2.0');
    drupal_alter('islandora_solr_config_rss_root_element_attributes', $rss_attributes, $channel, $items);

    // Give the results clean variable names.
    $title = $channel['title'];
    $url = $channel['url'];
    $description = $channel['description'];
    $langcode = $channel['langcode'];
    $args = $channel['args'];

    $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    $output .= '<rss' . drupal_attributes($rss_attributes) . ">\n";
    $output .= format_rss_channel($title, $url, $description, $items, $langcode, $args);
    $output .= "</rss>\n";

    print $output;
    exit;
  }


  /**
   * Function for setting the values of the <item> elements for the RSS display.
   *
   * @tutorial http://feed2.w3.org/docs/rss2.html#hrelementsOfLtitemgt
   *
   * @return array
   *   variable that holds all values to be rendered into <item> elements
   */
  public function rssItem($doc) {
    // Set variables.
    global $base_url;

    // Get variables.
    $rss_item = variable_get('islandora_solr_config_rss_item', array(
      'title' => 'fgs_label_s',
      'description' => '',
      'author' => '',
      'category' => '',
      'pubDate' => 'fgs_createdDate_s',
      'enclosure_dsid' => 'TN',
    ));

    // Object url.
    $object_url = url('islandora/object/' . htmlspecialchars($doc['PID'], ENT_QUOTES, 'utf-8'), array('absolute' => TRUE));
    // Enclosure file url (thumbnail by default).
    $dsid = ($rss_item['enclosure_dsid'] && isset($rss_item['enclosure_dsid'])) ? $rss_item['enclosure_dsid'] : 'TN';
    $doc['datastreams'] = isset($doc['datastreams']) ? $doc['datastreams'] : array();
    $is_datastream = in_array($dsid, $doc['datastreams']);

    if ($is_datastream) {
      $enclosure_url = $object_url . '/datastream/' . $dsid;
    }

    $rss_source = variable_get('site_name', "Default site name");

    // Set the variables to be rendered as elements in the <item> element.
    $result = array();
    $result['title'] = ($rss_item['title'] && isset($doc[$rss_item['title']])) ? $doc[$rss_item['title']] : '';
    $result['link'] = $object_url;
    $result['description'] = ($rss_item['description'] && isset($doc[$rss_item['description']])) ? $doc[$rss_item['description']] : '';
    $result['items'] = array(
      array(
        'key' => 'author',
        'value' => ($rss_item['author'] && isset($doc[$rss_item['author']])) ? $doc[$rss_item['author']] : ''),
      array(
        'key' => 'guid',
        'value' => $doc['PID'],
        'attributes' => array('isPermaLink' => 'false')),
      array(
        'key' => 'pubDate',
        'value' => ($rss_item['pubDate'] && isset($doc[$rss_item['pubDate']])) ? $doc[$rss_item['pubDate']] : ''),
      array(
        'key' => 'category',
        'value' => ($rss_item['category'] && isset($doc[$rss_item['category']])) ? $doc[$rss_item['category']] : ''),
      array(
        'key' => 'comments',
        'value' => ''),
      array(
        'key' => 'source',
        'value' => $rss_source, 'attributes' => array('url' => $base_url)),
    );

    if ($is_datastream) {
      $result['items'][] = array(
        'key' => 'enclosure',
        'value' => '', 'attributes' => array(
          'url' => $enclosure_url, 'length' => '',
          'type' => ''));
    }

    return $result;
  }

  /**
   * Function to set values of the <channel> elements for the RSS display.
   *
   * @tutorial http://feed2.w3.org/docs/rss2.html#requiredChannelElements
   *
   * @return array
   *   variable that holds all values to be rendered into <channel> elements
   */
  public function rssChannel($query) {
    // Set variables.
    global $base_url;
    $rss_channel = variable_get('islandora_solr_config_rss_channel', array(
      'copyright' => '',
      'managingEditor' => '',
      'webMaster' => '',
    ));

    // Set the variables to be rendered as elements in the <channel> element.
    $result = array();
    $result['title'] = t('@site_name aggregator', array('@site_name' => variable_get('site_name', 'Drupal')));
    $result['url'] = $base_url;
    $result['description'] = t('Aggregated search results of: @query', array('@query' => $query));
    $result['langcode'] = NULL;
    $result['args'] = array(
      array(
        'key' => 'copyright',
        'value' => $rss_channel['copyright']),
      array(
        'key' => 'managingEditor',
        'value' => $rss_channel['managingEditor']),
      array(
        'key' => 'webMaster',
        'value' => $rss_channel['webMaster']),
    );
    return $result;
  }
}
