<?php

/**
 * @file
 * Features hook implementations.
 */

/**
 * Implements hook_features_export_options().
 */
function islandora_solr_fields_features_export_options() {
  $field_types = array(
    'all_solr_fields' => t('All Solr fields from custom table'),
  );

  return $field_types;
}

/**
 * Implements hook_features_export().
 */
function islandora_solr_fields_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['islandora_solr'] = 'islandora_solr';

  foreach ($data as $component) {
    $export['features']['islandora_solr_fields'][$component] = $component;
  }
}

/**
 * Implements hook_features_export_render().
 */
function islandora_solr_fields_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  if (in_array('all_solr_fields', $data)) {
    $code = islandora_solr_get_fields(NULL, FALSE, FALSE);
  }

  $code = "  return " . features_var_export($code) . ";";
  return array('islandora_solr_fields_features_default_fields' => $code);
}

/**
 * Implements hook_features_revert().
 */
function islandora_solr_fields_features_revert($module_name) {
  islandora_solr_fields_features_rebuild($module_name);
}

/**
 * Helper function to serialize a particular array entry.
 *
 * @param array $field
 *   An array describing a field in the "islandora_solr_fields" table, with the
 *   "solr_field_settings" entry being an array. If it's a string, we let it
 *   pass through untouched.
 *
 * @return array
 *   The input with "solr_field_settings" serialize with serialize().
 */
function islandora_solr_serialize_solr_field_settings($field) {
  if (!is_string($field['solr_field_settings'])) {
    $field['solr_field_settings'] = serialize($field['solr_field_settings']);
  }

  return $field;
}

/**
 * Implements hook_features_rebuild().
 */
function islandora_solr_fields_features_rebuild($module_name) {
  $values = module_invoke($module_name, 'islandora_solr_fields_features_default_fields');
  if (!empty($values)) {
    // Get rid of old/existing values.
    db_delete('islandora_solr_fields')
      ->execute();

    // Serialize the "solr_field_settings" bit.
    $values = array_map('islandora_solr_serialize_solr_field_settings', $values);

    // Add in the new values.
    $insert_query = db_insert('islandora_solr_fields')
      ->fields(array(
        'solr_field',
        'field_type',
        'weight',
        'solr_field_settings',
    ));

    // Write to our database table.
    foreach ($values as $field) {
      $insert_query->values($field);
    }
    $insert_query->execute();
  }
}
