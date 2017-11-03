<?php

/**
 * @file
 * Callbacks and functions for the Associations Page.
 */

/**
 * Show the Associations page.
 *
 * Here, the user can view which forms are enabled for each content model.
 *
 * @return array
 *   The table to display.
 */
function xml_form_builder_list_associations() {

  module_load_include('inc', 'xml_form_builder', 'includes/associations');

  $associations_list = array(
    '#theme' => 'item_list',
    '#items' => array(),
  );

  $associations = xml_form_builder_get_associations(array(), array(), array(), TRUE);
  $map = array();

  foreach ($associations as $association) {
    $cmodel = $association['content_model'];
    $form = $association['form_name'];
    if (!isset($map[$cmodel])) {
      $map[$cmodel] = array();
    }
    $map[$cmodel][] = $form;
  }
  ksort($map);

  /**
   * Returns a link to the edit associations form for form $form_name.
   */
  function create_form_association_link($form_name) {
    return array(l($form_name, xml_form_builder_get_associate_form_path($form_name)));
  }

  foreach ($map as $cmodel => $forms) {
    $form_table = array(
      '#theme' => 'table',
      '#rows' => array_map('create_form_association_link', $forms),
    );
    $object = islandora_object_load($cmodel);
    if ($object) {
      $label = $object->label . " ($cmodel)";
    }
    else {
      $label = $cmodel;
    }
    $associations_list['#items'][] = $label . drupal_render($form_table);
  }

  return array($associations_list);
}
