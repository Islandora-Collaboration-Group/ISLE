<?php

/**
 * @file
 * Features hook implementations.
 */

/**
 * Implements hook_features_export_options().
 */
function islandora_xslt_populator_definitions_features_export_options() {
  module_load_include('inc', 'islandora_xslt_populator', 'includes/db');
  $named_populators = islandora_xslt_populator_get_named_populators();
  $title_map = function ($populator) {
    return $populator['title'];
  };
  $populators = array_map($title_map, $named_populators);
  return $populators;
}

/**
 * Implements hook_features_export().
 */
function islandora_xslt_populator_definitions_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['islandora_xslt_populator'] = 'islandora_xslt_populator';
  foreach ($data as $component) {
    $export['features']['islandora_xslt_populator_definitions'][$component] = $component;
  }
}

/**
 * Implements hook_features_export_render().
 */
function islandora_xslt_populator_definitions_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'islandora_xslt_populator', 'includes/db');
  $all_populators = islandora_xslt_populator_get_named_populators();
  $code = array();
  foreach (array_intersect_key($all_populators, drupal_map_assoc($data)) as $machine_name => $info) {
    unset($info['id']);
    $file = file_load($info['xslt']);
    unset($info['xslt']);
    $info['xslt_uri'] = $file->uri;
    $info['xslt_content'] = file_get_contents($file->uri);
    $code[$machine_name] = $info;
  }

  $code = '  return ' . features_var_export($code) . ';';
  return array(
    'islandora_xslt_populator_definition_defaults' => $code,
  );
}

/**
 * Implements hook_features_revert().
 */
function islandora_xslt_populator_definitions_features_revert($module_name) {
  islandora_xslt_populator_definitions_features_rebuild($module_name);
}

/**
 * Implements hook_features_rebuild().
 */
function islandora_xslt_populator_definitions_features_rebuild($module_name) {
  module_load_include('inc', 'islandora_xslt_populator', 'includes/db');
  $values = module_invoke($module_name, 'islandora_xslt_populator_definition_defaults');

  $id_map = function ($populator) {
    return $populator['id'];
  };

  // Removing existing ones we're going to replace.
  $current_populators = islandora_xslt_populator_get_named_populators();
  $overlap = array_intersect_key($current_populators, $values);
  $overlapping_ids = array_map($id_map, $overlap);
  if ($overlapping_ids) {
    islandora_xslt_populator_remove_populator($overlapping_ids);
  }

  // Add ALL the things!
  foreach ($values as $machine_name => $info) {
    $file = file_save_data($info['xslt_content'], $info['xslt_uri']);
    islandora_xslt_populator_add_populator(
      $info['title'],
      $info['description'],
      $file->fid,
      $info['datastream_id'],
      $machine_name,
      isset($info['original_dsid']) ? $info['original_dsid'] : NULL
    );
  }
}
