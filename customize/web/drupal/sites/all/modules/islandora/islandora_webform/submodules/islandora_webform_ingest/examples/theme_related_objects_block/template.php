<?php

/**
 * Implements theme_preprocess_islandora_basic_collection_grid.
 *
 * Adds mods_abstract field data to the objects.
 */
function YOUR_THEME_preprocess_islandora_basic_collection_grid(&$vars) {

  // Only preprocess if the block module and delta values are present. This
  // ensures that we only modify the collected objects when they are being
  // displayed in an associated objects block.
  // You might want to add a test to make sure that this applies only to a
  // particular relationship by filtering on the value for
  // $vars['block']['delta'] ("delta" is the name that you defined for the
  // relationship).
  if (!empty($vars['block']['module']) && !empty($vars['block']['delta'])) {

    // Loop through all the objects being displayed.
    foreach ($vars['associated_objects_array'] as &$object_data) {

      // This function provides a handy way to get particular data for an
      // object. Separate multiple fields by commas.
      // Note that in this example, we are grabbing note and name fields from
      // the example simple text content model's mods schema.
      $solr_data = iw_get_solr_data_array($object_data['object']->id, 'mods_relatedItem_host_note_ms, mods_relatedItem_host_name_namePart_ms');
      $object_data['solr_data'] = $solr_data[0]['solr_doc'];
    }
  }
}
