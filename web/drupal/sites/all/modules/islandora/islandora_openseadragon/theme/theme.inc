<?php

/**
 * @file
 * This file contains all theme and preprocess functions.
 */

/**
 * Implements template_preprocess_HOOK().
 *
 * @param array $variables
 *   The theme template variables.
 */
function template_preprocess_islandora_openseadragon_viewer(array &$variables) {
  module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');
  $library_path = libraries_get_path('openseadragon');
  if (isset($variables['fedora_object']) && $variables['fedora_object'] !== NULL) {
    $message = t("The parameter 'fedora_object' in theme('islandora_openseadragon_viewer') has been deprecated. As of 7.x-1.10, please update your code before the next release.");
    trigger_error(check_plain($message), E_USER_DEPRECATED);
    // Attempt to convert 'fedora_object', to 'pid'.
    $variables['pid'] = $variables['fedora_object']->id;
  }
  if (isset($variables['uri']) && $variables['uri'] !== '') {
    $message = t("The parameter 'uri' in theme('islandora_openseadragon_viewer') has been deprecated. As of 7.x-1.10, please update your code before the next release.");
    trigger_error(check_plain($message), E_USER_DEPRECATED);
    $variables['tile_sources'] = array(islandora_openseadragon_identifier_tile_source($variables['uri']));
  }
  $variables['id'] = $variables['viewer_id'] = 'islandora-openseadragon';
  $variables['settings'] = array(
    'pid' => $variables['pid'],
    'imageServer' => variable_get('islandora_openseadragon_tilesource', 'djatoka'),
    'djatokaServerBaseURL' => url(variable_get('islandora_openseadragon_djatoka_url', 'adore-djatoka/resolver'),
      array('absolute' => TRUE)
    ),
    'iiifServerBaseURL' => url(variable_get('islandora_openseadragon_iiif_url', 'iiif')),
    'fitToAspectRatio' => variable_get('islandora_openseadragon_fit_to_aspect_ratio', FALSE),
    'options' => array(
      'id' => $variables['id'],
      'prefixUrl' => file_create_url("{$library_path}/images/"),
      'tileSources' => $variables['tile_sources'],
      'overlays' => islandora_openseadragon_viewer_query_solr_for_overlays($variables['pid']),
    ) + islandora_openseadragon_get_settings(),
  );
}

/**
 * Implements template_process_HOOK().
 *
 * @param array $variables
 *   The theme template variables.
 */
function template_process_islandora_openseadragon_viewer(array &$variables) {
  $library_path = libraries_get_path('openseadragon');
  $module_path = drupal_get_path('module', 'islandora_openseadragon');

  drupal_add_js(array(
    'islandoraOpenSeadragon' => $variables['settings'],
  ), 'setting');

  drupal_add_js("$library_path/openseadragon.js", array('weight' => -4));
  if (islandora_openseadragon_use_djatoka_server()) {
    drupal_add_js("$module_path/js/djtilesource.js", array('weight' => -3));
  }
  drupal_add_js("$module_path/js/islandora_openseadragon.js", array('weight' => -2));
  drupal_add_css("$module_path/css/islandora_openseadragon.theme.css");
}

/**
 * Theme function to create a clipper link.
 *
 * @param array $variables
 *   The theme function variables.
 *
 * @return string
 *   The clipped image rendered into html.
 */
function theme_islandora_openseadragon_clipper(array &$variables) {
  $image = theme(
    'image',
    array(
      'path' => drupal_get_path('module', 'islandora_openseadragon') . '/images/clip_icon.png',
    )
  );
  return l(
    $image,
    "islandora/object/{$variables['pid']}/print",
    array(
      'attributes' => array(
        'title' => t('Clip Image'),
        'id' => 'clip',
      ),
      'html' => TRUE,
    )
  );
}
