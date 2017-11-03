<?php

/**
 * @file
 * Hooks provided by Islandora Bookmark.
 */

/**
 * Hook to collect exports functions available for use.
 *
 * @return array
 *   Returns an array with the name of the module and the function name to call
 *   to handle exportation. In the form of 'module name' => 'export function'.
 */
function hook_islandora_bookmark_export_handler() {
}

/**
 * Hook to alter the options returned for exportation.
 *
 * @param array $output
 *   An array in the form of 'module name' => 'export function'.
 *
 * @return array
 *   Returns an array with the name of the module and the function name to call
 *   to handle exportation. In the form of 'module name' => 'export function'.
 */
function hook_islandora_bookmark_export_handler_alter(&$output) {
}

/**
 * Hook to generate markup for displaying a Fedora object within a table.
 *
 * @param AbstractObject $fedora_object
 *   An AbstractObject representing a Fedora object.
 *
 * @param array $object_url_info
 *   An associative array contains information to build an anchor tag.
 *   - path: A string containing the path/
 *   - params: An array of paramaters to be passed.
 *   - markup: A string containing the label for the anchor tag.
 *
 * @return array
 *   Returns an array containing the markup for displaying the Fedora object.
 */
function hook_islandora_bookmark_object_markup(AbstractObject $fedora_object, $object_url_info) {
}

/**
 * Hook to alter existing markup for displaying a Fedora object within a table.
 *
 * @param AbstractObject $fedora_object
 *   An AbstractObject representing a Fedora object.
 *
 * @param array $object_url_info
 *   An associative array contains information to build an anchor tag.
 *   - path: A string containing the path/
 *   - params: An array of paramaters to be passed.
 *   - markup: A string containing the label for the anchor tag.
 *
 * @return array
 *   Returns an array containing the markup for displaying the Fedora object.
 */
function hook_islandora_bookmark_object_markup_alter(AbstractObject $fedora_object, $object_url_info) {
}

/**
 * Hook to generate markup for displaying a Fedora object within a table.
 *
 * Content model specific.
 *
 * @param AbstractObject $fedora_object
 *   An AbstractObject representing a Fedora object.
 *
 * @param array $object_url_info
 *   An associative array contains information to build an anchor tag.
 *   - path: A string containing the path/
 *   - params: An array of paramaters to be passed.
 *   - markup: A string containing the label for the anchor tag.
 *
 * @return array
 *   Returns an array containing the markup for displaying the Fedora object.
 */
function hook_cmodel_pid_islandora_bookmark_object_markup(AbstractObject $fedora_object, $object_url_info) {
}

/**
 * Hook to collect additional style options used in exporting.
 *
 * @param string $option
 *   The currently selected option we are trying to get additional styles for.
 *
 * @return array
 *   Returns an array containing the additional styles for export.
 */
function hook_islandora_bookmark_export_styles($option) {
}

/**
 * Hook to change or add values to RSS fields.
 *
 * Sometimes you might want to alter fields for an rss item.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 *
 * @return array
 *   Returns an array containing the additional changes to the rss item.
 */
function hook_islandora_bookmark_rss_item(AbstractObject $object) {

  // Create an associative array for the required elements
  // for a valid bookmark RSS item.
  $rss_item = array();
  // The title of the item.
  $rss_item['title'] = 'Altered Title';
  // The link of the item.
  $rss_item['link'] = 'Altered Link';
  // The description of the item.
  $rss_item['description'] = 'Altered description';

  // Set the source attribute.
  $rss_item['items'] = array(
    array(
      'key' => 'source',
      'value' => 'source value', 'attributes' => array('url' => 'url')),
  );

  // Return the RSS item.
  return $rss_item;
}

/**
 * Get the mapping of types so we can instantiate different classes.
 *
 * The "bookmark" class just saves to the DB... We may want to add in other
 * things to occur on different actions, like changing something on an object
 * when we add an object to a list.
 *
 * @return array
 *   An associative array mapping the "type" column in the
 *   "islandora_bookmark_list_names" to a class to use to interact with the
 *   given list.
 */
function hook_islandora_bookmark_database_types() {
  return array(
    'my_type' => 'my_awesome_bookmark_class',
  );
}

/**
 * Allows modules to modify values on creation of database lists.
 *
 * @param string $name
 *   The name for the list.
 * @param string $type
 *   A string indicating the type of list. Default is 'bookmark'.
 * @param object $owner
 *   A user object from which we can grab the uid.
 */
function hook_islandora_bookmark_create_new_database_list_alter(&$name, &$type, &$owner) {

}
