<?php

/**
 * @file
 * Features hook implementations.
 */

/**
 * Implements hook_features_export_options().
 */
function islandora_solr_metadata_configurations_features_export_options() {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  $configs = islandora_solr_metadata_get_associations();
  $features_config = array();
  foreach ($configs as $config) {
    $features_config[$config['machine_name']] = $config['name'];
  }
  return $features_config;
}


/**
 * Implements hook_features_export().
 */
function islandora_solr_metadata_configurations_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['islandora_solr_metadata'] = 'islandora_solr_metadata';
  foreach ($data as $component) {
    $export['features']['islandora_solr_metadata_configurations'][$component] = $component;
  }
}

/**
 * Implements hook_features_export_render().
 */
function islandora_solr_metadata_configurations_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  $code = array();
  foreach ($data as $machine_name) {
    $config_id = islandora_solr_metadata_retrieve_configuration_from_machine_name($machine_name);
    $fields = islandora_solr_metadata_get_fields($config_id, FALSE);
    foreach ($fields as &$field) {
      unset($field['configuration_id']);
    }
    unset($field);
    $code[$machine_name] = array(
      'fields' => $fields,
      'cmodels' => islandora_solr_metadata_get_cmodels($config_id),
      'description' => islandora_solr_metadata_retrieve_description($config_id, FALSE),
      'name' => islandora_solr_metadata_retrieve_configuration_name($config_id),
    );
  }

  $code = " return " . features_var_export($code) . ";";
  return array('islandora_solr_metadata_configurations_default_fields' => $code);
}

/**
 * Implements hook_features_revert().
 */
function islandora_solr_metadata_configurations_features_revert($module_name) {
  islandora_solr_metadata_configurations_features_rebuild($module_name);
}

/**
 * Implements hook_features_rebuild().
 */
function islandora_solr_metadata_configurations_features_rebuild($module_name) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  $values = module_invoke($module_name, 'islandora_solr_metadata_configurations_default_fields');
  if (!empty($values)) {
    foreach ($values as $machine_name => $config_info) {
      $exists = islandora_solr_metadata_retrieve_configuration_from_machine_name($machine_name);
      if ($exists !== FALSE) {
        // Update.
        // Delete given field and cmodels.
        $config_id = $exists;
        if (module_exists('i18n_string')) {
          $fields = islandora_solr_metadata_get_fields($config_id, FALSE);
          foreach (array_keys($fields) as $field) {
            $i18n_id = islandora_solr_metadata_field_i18n_id($config_id, $field);
            i18n_string_remove($i18n_id);
          }
        }
        db_delete('islandora_solr_metadata_fields')
          ->condition('configuration_id', $config_id)
          ->execute();

        db_delete('islandora_solr_metadata_cmodels')
          ->condition('configuration_id', $config_id)
          ->execute();
      }
      else {
        // Create our config.
        $config_id = islandora_solr_metadata_add_configuration($config_info['name'], $machine_name);
      }
      // Add fields and cmodels, related to config_id.
      islandora_solr_metadata_update_description($config_id, $config_info['description']['description_field'], $config_info['description']['description_label'], $config_info['description']['description_data']);
      islandora_solr_metadata_add_content_models($config_id, array_map(function ($e) {
        return array('cmodel' => $e);
      }, $config_info['cmodels']));
      islandora_solr_metadata_add_fields($config_id, $config_info['fields']);
    }
  }
}
