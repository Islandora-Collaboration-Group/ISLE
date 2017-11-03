<?php

/**
 * @file
 * This file contains all theme and preprocess functions
 */

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_basic_collection_grid(&$variables) {
  template_preprocess_islandora_basic_collection($variables);
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_basic_collection_wrapper(&$variables) {
  $page_number = (empty($_GET['page'])) ? 0 : $_GET['page'];
  $page_size = (empty($_GET['pagesize'])) ? variable_get('islandora_basic_collection_page_size', '12') : $_GET['pagesize'];
  $islandora_object = $variables['islandora_object'];
  try {
    $dc = $islandora_object['DC']->content;
    $dc_object = DublinCore::importFromXMLString($dc);
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->asArray() : array();
  $variables['islandora_object_label'] = $islandora_object->label;
  $variables['display_metadata'] = variable_get('islandora_collection_metadata_display', FALSE);
  if ($variables['display_metadata']) {
    drupal_add_js('misc/form.js');
    drupal_add_js('misc/collapse.js');
    module_load_include('inc', 'islandora', 'includes/metadata');
    $variables['parent_collections'] = islandora_get_parents_from_rels_ext($islandora_object);
    $variables['metadata'] = islandora_retrieve_metadata_markup($islandora_object);
    $variables['description'] = islandora_retrieve_description_markup($islandora_object);
  }
  list($total_count, $results) = islandora_basic_collection_get_member_objects($islandora_object, $page_number, $page_size);

  pager_default_initialize($total_count, $page_size);
  $variables['collection_pager'] = theme('pager', array('quantity' => 10));
  $display = (empty($_GET['display'])) ? variable_get('islandora_basic_collection_default_view', 'grid') : $_GET['display'];
  $link_text = (empty($_GET['display'])) ? 'grid' : $_GET['display'];
  $query_params = drupal_get_query_parameters($_GET);

  $query_params['display'] = 'list';
  $list_link = array(
    'title' => t('List view'),
    'attributes' => array(
      'href' => url("islandora/object/$islandora_object->id", array('query' => $query_params)),
      'class' => array('islandora-view-list'),
    ),
  );
  $query_params['display'] = 'grid';
  $grid_link = array(
    'title' => t('Grid view'),
    'attributes' => array(
      'href' => url("islandora/object/$islandora_object->id", array('query' => $query_params)),
      'class' => array('islandora-view-grid'),
    ),
  );

  if ($display == 'grid') {
    $grid_link['attributes']['class'][] = 'active';
    $collection_content = theme('islandora_basic_collection_grid', array(
      'islandora_object' => $islandora_object,
      'collection_results' => $results,
      )
    );
  }
  else {
    $list_link['attributes']['class'][] = 'active';
    $collection_content = theme('islandora_basic_collection', array(
      'islandora_object' => $islandora_object,
      'collection_results' => $results,
    ));
  }

  $variables['view_links'] = array($grid_link, $list_link);
  $variables['collection_content'] = $collection_content;
}

/**
 * Returns HTML for the metadata_table_drag_components weight table.
 *
 * @param array $vars
 *   An associative array containing:
 *   - element: Table element array.
 *   - theme_hook_original: Initial theme hook.
 *
 * @return string
 *   Themed table html.
 *
 * @ingroup themeable
 */
function theme_islandora_basic_collection_metadata_table_drag_components($vars) {
  $element = $vars['element'];
  $clean_id = drupal_html_id('islandora_basic_collection_meta_page_element_table');
  drupal_add_tabledrag($clean_id, 'order', 'sibling', 'item-row-weight');

  $header = array(
    'label' => t('Attribute'),
    'textfield' => t('Description'),
    'weight' => t('Weight'),
    'omit' => t('Hide from display'),
  );

  $rows = array();
  foreach (element_children($element) as $key) {
    $row = array();
    $row['data'] = array();
    foreach ($header as $fieldname => $title) {
      $row['data'][] = drupal_render($element[$key][$fieldname]);
      $row['class'] = array('draggable');
    }
    $rows[] = $row;
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $clean_id),
  ));
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_basic_collection(&$variables) {
  $islandora_object = $variables['islandora_object'];

  try {
    $dc = $islandora_object['DC']->content;
    $dc_object = DublinCore::importFromXMLString($dc);
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $page_number = (empty($_GET['page'])) ? 0 : $_GET['page'];
  $page_size = (empty($_GET['pagesize'])) ? variable_get('islandora_basic_collection_page_size', '12') : $_GET['pagesize'];
  $results = $variables['collection_results'];
  $total_count = count($results);
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : array();
  $variables['islandora_object_label'] = $islandora_object->label;
  $display = (empty($_GET['display'])) ? 'list' : $_GET['display'];
  if ($display == 'grid') {
    $variables['theme_hook_suggestions'][] = 'islandora_basic_collection_grid__' . str_replace(':', '_', $islandora_object->id);
  }
  else {
    $variables['theme_hook_suggestions'][] = 'islandora_basic_collection__' . str_replace(':', '_', $islandora_object->id);
  }
  if (isset($islandora_object['OBJ'])) {
    $full_size_url = url("islandora/object/{$islandora_object->id}/datastram/OBJ/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $full_size_url);
    $variables['islandora_full_img'] = theme('image', $params);
  }
  if (isset($islandora_object['TN'])) {
    $full_size_url = url("islandora/objects/{$islandora_object->id}/datastream/TN/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $full_size_url);
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }
  if (isset($islandora_object['MEDIUM_SIZE'])) {
    $full_size_url = url("islandora/object/{$islandora_object->id}/datastream/MEDIUM_SIZE/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $full_size_url);
    $variables['islandora_medium_img'] = theme('params', $params);
  }

  $associated_objects_array = array();

  foreach ($results as $result) {
    $pid = $result['object']['value'];
    $fc_object = islandora_object_load($pid);
    if (!is_object($fc_object)) {
      // NULL object so don't show in collection view.
      continue;
    }
    $associated_objects_array[$pid]['object'] = $fc_object;
    try {
      $dc = $fc_object['DC']->content;
      $dc_object = DublinCore::importFromXMLString($dc);
      $associated_objects_array[$pid]['dc_array'] = $dc_object->asArray();
    }
    catch (Exception $e) {
      drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
    }
    $object_url = 'islandora/object/' . $pid;

    $title = $result['title']['value'];
    $associated_objects_array[$pid]['pid'] = $pid;
    $associated_objects_array[$pid]['path'] = $object_url;
    $associated_objects_array[$pid]['title'] = $title;
    $associated_objects_array[$pid]['class'] = drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $pid));
    if (isset($fc_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $fc_object['TN'])) {
      $thumbnail_img = theme('image', array('path' => "$object_url/datastream/TN/view", 'alt' => $title));
    }
    else {
      $image_path = drupal_get_path('module', 'islandora');
      $thumbnail_img = theme('image', array('path' => "$image_path/images/folder.png", 'alt' => $title));
    }
    $associated_objects_array[$pid]['thumbnail'] = $thumbnail_img;
    $associated_objects_array[$pid]['title_link'] = l($title, $object_url, array('html' => TRUE, 'attributes' => array('title' => $title)));
    $associated_objects_array[$pid]['thumb_link'] = l($thumbnail_img, $object_url, array('html' => TRUE, 'attributes' => array('title' => $title)));
  }
  $variables['associated_objects_array'] = $associated_objects_array;
}

/**
 * Theme the collection policy management table.
 *
 * @param array $variables
 *   Variables passed to this theme function.
 *
 * @return string
 *   Markup representing the table for rendering.
 */
function theme_islandora_basic_collection_policy_management_table(array $variables) {
  // Manually add the table select javascript as we are using a custom table.
  drupal_add_js('misc/tableselect.js');
  $table = $variables['table'];
  $row_elements = $table['rows'];
  $rows = array();
  foreach (element_children($row_elements) as $key) {
    $columns = array();
    $row_element = $row_elements[$key];
    foreach (element_children($row_element) as $key) {
      $column_element = $row_element[$key];
      $class = "collection_policy_table_${key}";
      $columns[] = array(
        'data' => drupal_render($column_element),
        'class' => isset($cell_element['#attributes']['class']) ? $column_element['#attributes']['class'] : $class,
      );
    }
    $rows[] = $columns;
  }
  $variables = array(
    'header' => $table['#header'],
    'rows' => $rows,
    'attributes' => $table['#attributes'],
    'caption' => NULL,
    'colgroups' => NULL,
    'sticky' => NULL,
    'empty' => t("No content models"),
  );
  return theme('table', $variables);
}
