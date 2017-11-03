<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_disk_image(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/authtokens');
  module_load_include('inc', 'islandora', 'includes/metadata');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $object = $variables['object'];
  // We should eventually remove the DC object and dc_array code as it only
  // exists to not break legacy implementations.
  try {
    if (isset($object['DC']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['DC'])) {
      $dc = $object['DC']->content;
      $dc_object = DublinCore::importFromXMLString($dc);
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->asArray() : array();
  $variables['islandora_object_label'] = $object->label;
  $variables['islandora_datastream_label'] = $object['OBJ']->label;
  $variables['theme_hook_suggestions'][] = 'islandora_disk_image__' . str_replace(':', '_', $object->id);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);

  // Original.
  if (isset($object['OBJ']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['OBJ'])) {
    $disk_image_url = url("islandora/object/{$object->id}/datastream/OBJ/view", array('absolute' => TRUE));
    $variables['islandora_disk_image'] = $disk_image_url;
  }
  // Thumbnail.
  if (isset($object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN'])) {
    $thumbnail_size_url = url("islandora/object/{$object->id}/datastream/TN/view");
    $params = array(
      'title' => $object->label,
      'path' => $thumbnail_size_url,
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }
  // Download disk image.
  if (isset($object['OBJ']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['OBJ'])) {
    $disk_image_url = url("islandora/object/{$object->id}/datastream/OBJ/{$object->label}", array('absolute' => TRUE));
    $variables['islandora_disk_image'] = l($variables['islandora_datastream_label'], $disk_image_url, array('html' => TRUE));
  }
  // Photo of disk image.
  if (isset($object['PHOTO']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PHOTO'])) {
    $photo_url = url("islandora/object/{$object->id}/datastream/PHOTO/view");
    $params = array(
      'title' => $object->label,
      'path' => $photo_url,
    );
    $variables['islandora_photo'] = theme('image', $params);
    if (isset($photo_url)) {
      $variables['islandora_content'] = l($variables['islandora_photo'], $photo_url, array('html' => TRUE));
    }
    else {
      $variables['islandora_content'] = $variables['islandora_photo'];
    }
  }
}
