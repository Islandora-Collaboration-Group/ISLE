<?php

/**
 * @file
 * Document hooks.
 */

/**
 * Alter rendered RSS item elements.
 *
 * @param string $rendered_item
 *   The RSS item which will be output for the given document.
 * @param array $doc
 *   A Solr result.
 */
function hook_islandora_solr_config_rss_item_post_render_alter(&$rendered_item, &$doc) {
  $item = new DOMDocument();
  $item->loadXML($rendered_item);

  // Would do some manipulation of the DOM before this...
  $rendered_item = $item->saveXML($item->documentElement);
}

/**
 * Alter various values "higher" in the RSS feed.
 *
 * @param array $root_attributes
 *   The array of attributes to add to the root RSS element, as accepted by
 *   drupal_attributes().
 * @param array $channel_info
 *   An associative array containing the channel:
 *   - title
 *   - url
 *   - description
 *   - langcode
 *   - args
 *   See format_rss_channel() for more info.
 * @param string $items
 *   The rendered items to add in the channel.
 */
function hook_islandora_solr_config_rss_root_element_attributes_alter(&$root_attributes, &$channel_info, &$items) {
  // Could add namespaces to the root element or mess with the values to output
  // for the channel or all the items... If trying to parse $items, one would
  // have to treat it as a document fragment (its not valid XML on
  // its own).
}
