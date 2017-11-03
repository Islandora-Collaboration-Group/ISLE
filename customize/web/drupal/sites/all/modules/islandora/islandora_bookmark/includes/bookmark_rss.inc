<?php

/**
 * @file
 * Functions used in the islandora_bookmark module
 */

/**
 * Prints out the rss information from the bookmark list id.
 *
 * @param string $list_id
 *   The PID of the object to create markup for.
 */
function islandora_bookmark_rss($list_id) {

  module_load_include('inc', 'islandora_bookmark', 'includes/api');

  // Change the content type to be application/rss+xml and not http.
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');

  // Get bookmark based on boookmark list id.
  $bookmark = islandora_bookmark_get_bookmark_by_number($list_id);
  // Get all the pid belonging to that bookmark.
  $all_pids = $bookmark->getPids();

  // Loop through all the pids and build the formatted RSS items.
  $items = NULL;
  foreach ($all_pids as $pid) {
    // Load fedora object from pid.
    $fedora_object = islandora_object_load($pid);
    if ($fedora_object) {
      $items .= islandora_bookmark_create_rss_item($fedora_object);
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
  $description = t('Aggregated results of: @bookmarkName', array('@bookmarkName' => $bookmark->bookmarkName));

  // Build the xml string to be printed.
  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel($title, $link, $description, $items);
  $output .= "</rss>\n";

  // Print the xml.
  print $output;
  exit;
}

/**
 * Creates an bookmark RSS item.
 *
 * @param AbstractObject $object
 *   The object to create RSS item.
 *
 * @return string
 *   The formated RSS item.
 */
function islandora_bookmark_create_rss_item(AbstractObject $object) {

  module_load_include('inc', 'islandora', 'includes/utilities');

  // Create rss item array.
  $rss_item = array();

  // Invokes the islandora_bookmark_rss_item hook.
  foreach (islandora_build_hook_list('islandora_bookmark_rss_item', $object->models) as $hook) {
    $temp = module_invoke_all($hook, $object);

    // If something returns then merge it with the $rss_ite.
    if (!empty($temp)) {
      $rss_item = array_merge_recursive($rss_item, $temp);
    }
  }

  // If $rss_item is empty then call the default RSS item.
  if (empty($rss_item)) {
    $rss_item = islandora_bookmark_rss_item($object);
  }

  // Return the formatted RSS item.
  return format_rss_item($rss_item['title'], $rss_item['link'], $rss_item['description'], $rss_item['items']);
}

/**
 * Function for setting the values of the <item> elements for the default RSS.
 *
 * @tutorial http://feed2.w3.org/docs/rss2.html#hrelementsOfLtitemgt
 *
 * @tutorial http://feed2.w3.org/docs/rss2.html#requiredChannelElements
 *
 * @param AbstractObject $object
 *   An abstract object
 *
 * @return array
 *   an associative array containing the rss item to be displayed
 */
function islandora_bookmark_rss_item(AbstractObject $object) {

  // Global variables.
  global $base_url;

  // Create an associative array for the required elements
  // for a valid bookmark RSS item.
  $rss_item = array();
  // The default title of the item will be the objects label.
  $rss_item['title'] = $object->label;
  // The default link will be to the objects page.
  $rss_item['link'] = url('islandora/object/' . htmlspecialchars($object->id, ENT_QUOTES, 'utf-8'), array('absolute' => TRUE));
  // The default description will be No description.
  $rss_item['description'] = t('No description');

  // Get the site name to be the value of the source.
  $rss_source = variable_get('site_name', "Default site name");

  // Set the default source attribute to be to be the base url.
  $rss_item['items'] = array(
    array(
      'key' => 'source',
      'value' => $rss_source, 'attributes' => array('url' => $base_url)),
  );

  // Returns the the rss default item.
  return $rss_item;
}
