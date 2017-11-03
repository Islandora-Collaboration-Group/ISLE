<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Prepares variables for islandora_book_book templates.
 *
 * Default template: islandora-book-book.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_islandora_book_book(array &$variables) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/metadata');
  module_load_include('inc', 'islandora', 'includes/datastream');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');

  $object = $variables['object'];

  $variables['viewer_id'] = islandora_get_viewer_id('islandora_book_viewers');
  $variables['viewer_params'] = array(
    'object' => $object,
    'pages' => islandora_paged_content_get_pages($object),
    'page_progression' => islandora_paged_content_get_page_progression($object),
  );

  $variables['display_metadata'] = variable_get('islandora_book_metadata_display', FALSE);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);
}

/**
 * Processes variables for islandora_book_book templates.
 *
 * Default template: islandora-book-book.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 *   - viewer_id: The "ID" of the viewer to use.
 *   - viewer_params: An (associative) array of parameters to pass to the
 *     viewer.
 */
function template_process_islandora_book_book(array &$variables) {
  if ($variables['viewer_id'] && $variables['viewer_id'] != 'none') {
    $viewer_callback = islandora_get_viewer_callback($variables['viewer_id']);
    if (function_exists($viewer_callback)) {
      $viewer = $viewer_callback($variables['viewer_params'], $variables['object']);
      if ($viewer) {
        $variables['viewer'] = $viewer;
      }
    }
  }
}

/**
 * Prepares variables for islandora_book_page templates.
 *
 * Default template: islandora-book-page.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_islandora_book_page(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $object = $variables['object'];
  $results = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOf');
  $result = reset($results);
  $variables['book_object_id'] = $result ? $result['object']['value'] : FALSE;

  $params = array();
  if (isset($object['JP2']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JP2'])) {
    // Get token to allow access to XACML protected datastreams.
    // Always use token authentication in case there is a global policy.
    module_load_include('inc', 'islandora', 'includes/authtokens');
    $token = islandora_get_object_token($object->id, 'JP2', 2);
    $jp2_url = url("islandora/object/{$object->id}/datastream/JP2/view", array(
      'absolute' => TRUE,
      'query' => array('token' => $token),
    ));
    $params['jp2_url'] = $jp2_url;
  }

  $variables['viewer_id'] = islandora_get_viewer_id('islandora_book_page_viewers');
  $variables['viewer_params'] = $params;
}

/**
 * Processes variables for islandora_book_page templates.
 *
 * Default template: islandora-book-page.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 *   - viewer_id: The "ID" of the viewer to use.
 *   - viewer_params: An (associative) array of parameters to pass to the
 *     viewer.
 */
function template_process_islandora_book_page(array &$variables) {
  if ($variables['viewer_id'] && $variables['viewer_id'] != 'none') {
    $viewer_callback = islandora_get_viewer_callback($variables['viewer_id']);
    if (function_exists($viewer_callback)) {
      $viewer = $viewer_callback($variables['viewer_params'], $variables['object']);
      if ($viewer) {
        $variables['viewer'] = $viewer;
      }
    }
  }
}
