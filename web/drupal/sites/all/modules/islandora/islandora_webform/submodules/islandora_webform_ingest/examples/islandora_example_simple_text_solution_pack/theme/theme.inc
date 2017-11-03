<?php

/**
 * @file
 * This file contains all theme and preprocess functions.
 */

/**
 * Implements hook_preprocess().
 */
function islandora_example_simple_text_preprocess_islandora_example_html_description(array &$variables) {
  $islandora_object = $variables['islandora_object'];
  if (islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $islandora_object['HTML'])) {
    try {
      $html = $islandora_object['HTML']->content;
    }
    catch (Exception $e) {
      drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
    }
  }
  $variables['html_description'] = isset($html) ? $html : '';
}
