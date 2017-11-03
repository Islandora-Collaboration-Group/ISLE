<?php
/**
 * @file
 * Holds helper functions for embargo.
 */

/**
 * Provides all possible datastreams that can be embargoed.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array
 *   An array representing all the datastreams that are possible to be
 *   embargoed.
 */
function islandora_scholar_embargo_get_embargoable_datastreams(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $requirements = drupal_map_assoc(array_keys(islandora_get_datastreams_requirements($object)));
  $current_ds = drupal_map_assoc(array_keys(iterator_to_array($object->getIterator())));
  return array_merge($requirements, $current_ds);
}
