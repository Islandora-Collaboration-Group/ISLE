<?php
/**
 * @file
 * Hook descriptions.
 */

/**
 * Display element alter.
 *
 * @param array $elements
 *   The associative array of parameters as used by the
 *   'islandora_solr_metadata_display' element, including:
 *   - islandora_object: The object for which the display is being rendered.
 *   - print: A boolean flag indicating if the display is to be rendered in a
 *     printer-friendly manner.
 *   - associations: An array of metadata configuration IDs, similar to the
 *     return of islandora_solr_metadata_get_associations_by_cmodels().
 */
function hook_islandora_solr_metadata_display_elements_alter(&$elements) {
  // Artificial example: Get rid of associations if the object is not active.
  if ($elements['islandora_object']->state != 'A') {
    $elements['associations'] = array();
  }
}

/**
 * Description element alter.
 *
 * @param array $elements
 *   The associative array of parameters as used by the
 *   'islandora_solr_metadata_display' element, including:
 *   - islandora_object: The object for which the display is being rendered.
 *   - associations: An array of metadata configuration IDs, similar to the
 *     return of islandora_solr_metadata_get_associations_by_cmodels().
 */
function hook_islandora_solr_metadata_description_elements_alter(&$elements) {
  // Artificial example: Get rid of associations if the object is not active.
  if ($elements['islandora_object']->state != 'A') {
    $elements['associations'] = array();
  }
}
