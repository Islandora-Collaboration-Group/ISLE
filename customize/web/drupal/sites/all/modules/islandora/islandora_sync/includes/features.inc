<?php

/**
 * @file
 * Features integration; hooks used to build and process islandora_sync feature
 * components.
 */

/**
 * Implements hook_features_export().
 */
function islandora_sync_field_datastreams_features_export($data, &$export, $module_name) {
  foreach ($data as $component) {
    $export['features']['islandora_sync_field_datastreams'][$component] = $component;
  }
  return array();
}

/**
 * Implements hook_features_export_options().
 */
function islandora_sync_field_datastreams_features_export_options() {
  $options = array();
  $fields = db_query("SELECT * FROM {islandora_sync_fields}")->fetchAll();
  foreach ($fields as $field) {
    $name = $field->bundle . ':' . $field->field;
    $options[$name] = $name;
  }
  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function islandora_sync_field_datastreams_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '$islandora_sync_field_datastreams = array();';
  foreach ($data as $name) {
    list($bundle, $field_name) = explode(':', $name);
    $field = db_query("SELECT * FROM {islandora_sync_fields}
      WHERE bundle = :bundle
       AND field = :field",
      array(':bundle' => $bundle, ':field' => $field_name)
    )->fetchObject();
    $code[] = "  \$islandora_sync_field_datastreams['{$name}'] = " . features_var_export($field) . ";";
  }
  $code[] = "return \$islandora_sync_field_datastreams;";
  $code = implode("\n", $code);
  return array('islandora_sync_field_datastreams_defaults' => $code);
}

/**
 * Implements hook_features_revert().
 */
function islandora_sync_field_datastreams_features_revert($module_name) {
  islandora_sync_field_datastreams_features_rebuild($module_name);
}

/**
 * Implements hook_features_rebuild().
 */
function islandora_sync_field_datastreams_features_rebuild($module_name) {
  islandora_sync_field_datastreams_features_disable_feature($module_name);
  $fields = module_invoke($module_name, 'islandora_sync_field_datastreams_defaults');
  if (!empty($fields)) {
    foreach ($fields as $field) {
      drupal_write_record('islandora_sync_fields', $field);
    }
  }
}

/**
 * Implements hook_features_enable_feature().
 */
function islandora_sync_field_datastreams_features_enable_feature($module_name) {
  islandora_sync_field_datastreams_features_rebuild($module_name);
}

/**
 * Implements hook_features_disable_feature().
 */
function islandora_sync_field_datastreams_features_disable_feature($module_name) {
  $fields = module_invoke($module_name, 'islandora_sync_field_datastreams_defaults');
  foreach ($fields as $field) {
    db_delete('islandora_sync_fields')->condition('bundle', $field['bundle'])->condition('field', $field['field'])->execute();
  }
}

/**
 * Implements hook_features_export().
 */
function islandora_sync_bundle_settings_features_export($data, &$export, $module_name) {
  foreach ($data as $component) {
    $export['features']['islandora_sync_bundle_settings'][$component] = $component;
  }
  return array();
}

/**
 * Implements hook_features_export_options().
 */
function islandora_sync_bundle_settings_features_export_options() {
  $options = array();
  $bundles = db_query("SELECT * FROM {islandora_sync_type_settings}")->fetchAll();
  foreach ($bundles as $bundle) {
    $name = $bundle->bundle;
    $options[$name] = $name;
  }
  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function islandora_sync_bundle_settings_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '$islandora_sync_bundle_settings = array();';
  foreach ($data as $name) {
    $bundle = db_query("SELECT * FROM {islandora_sync_type_settings} WHERE bundle = :bundle",
      array(':bundle' => $name)
    )->fetchObject();
    $code[] = "  \$islandora_sync_bundle_settings['{$name}'] = " . features_var_export($bundle) . ";";
    $bundle_cmodels = db_query("SELECT * FROM {islandora_sync_type_cmodel} WHERE bundle = :bundle",
      array(':bundle' => $name)
    )->fetchAll();
    $code[] = "  \$islandora_sync_bundle_settings['{$name}']['cmodels'] = " . features_var_export($bundle_cmodels) . ";";
  }
  $code[] = "return \$islandora_sync_bundle_settings;";
  $code = implode("\n", $code);
  return array('islandora_sync_bundle_settings_defaults' => $code);
}

/**
 * Implements hook_features_revert().
 */
function islandora_sync_bundle_settings_features_revert($module_name) {
  islandora_sync_bundle_settings_features_rebuild($module_name);
}

/**
 * Implements hook_features_rebuild().
 */
function islandora_sync_bundle_settings_features_rebuild($module_name) {
  islandora_sync_bundle_settings_features_disable_feature($module_name);
  $bundles = module_invoke($module_name, 'islandora_sync_bundle_settings_defaults');
  if (!empty($bundles)) {
    foreach ($bundles as $bundle) {
      foreach ($bundle['cmodels'] as $cmodel) {
        $bundle_cmodel = array('bundle' => $bundle['bundle'], 'cmodel' => $cmodel['cmodel']);
        drupal_write_record('islandora_sync_type_cmodel', $bundle_cmodel);
      }
      unset($bundle['cmodels']);
      drupal_write_record('islandora_sync_type_settings', $bundle);
    }
  }
}

/**
 * Implements hook_features_enable_feature().
 */
function islandora_sync_bundle_settings_features_enable_feature($module_name) {
  islandora_sync_bundle_settings_features_rebuild($module_name);
}

/**
 * Implements hook_features_disable_feature().
 */
function islandora_sync_bundle_settings_features_disable_feature($module_name) {
  $bundles = module_invoke($module_name, 'islandora_sync_bundle_settings_defaults');
  foreach ($bundles as $bundle) {
    foreach ($bundle['cmodels'] as $cmodel) {
      db_delete('islandora_sync_type_cmodel')->condition('bundle', $bundle['bundle'])->condition('cmodel', $cmodel['cmodel'])->execute();
    }
    db_delete('islandora_sync_type_settings')->condition('bundle', $bundle['bundle'])->execute();
  }
}

/**
 * Implements hook_features_export().
 */
function islandora_sync_xml_ds_settings_features_export($data, &$export, $module_name) {
  foreach ($data as $component) {
    $export['features']['islandora_sync_xml_ds_settings'][$component] = $component;
  }
  return array();
}

/**
 * Implements hook_features_export_options().
 */
function islandora_sync_xml_ds_settings_features_export_options() {
  $options = array();
  $datastreams = db_query("SELECT * FROM {islandora_sync_xml_ds}")->fetchAll();
  foreach ($datastreams as $datastream) {
    $name = $datastream->ds_id;
    $options[$name] = $name;
  }
  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function islandora_sync_xml_ds_settings_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '$islandora_sync_xml_ds_settings = array();';
  foreach ($data as $name) {
    $ds = db_query("SELECT * FROM {islandora_sync_xml_ds} WHERE ds_id = :ds_id",
      array(':ds_id' => $name)
    )->fetchObject();
    $code[] = "  \$islandora_sync_xml_ds_settings['{$name}'] = " . features_var_export($ds) . ";";
    $ds_namespaces = db_query("SELECT * FROM {islandora_sync_xml_ds_namespaces} WHERE ds_id = :ds_id",
      array(':ds_id' => $name)
    )->fetchAll();
    $code[] = "  \$islandora_sync_xml_ds_settings['{$name}']['namespaces'] = " . features_var_export($ds_namespaces) . ";";
  }
  $code[] = "return \$islandora_sync_xml_ds_settings;";
  $code = implode("\n", $code);
  return array('islandora_sync_xml_ds_settings_defaults' => $code);
}

/**
 * Implements hook_features_revert().
 */
function islandora_sync_xml_ds_settings_features_revert($module_name) {
  islandora_sync_xml_ds_settings_features_rebuild($module_name);
}

/**
 * Implements hook_features_rebuild().
 */
function islandora_sync_xml_ds_settings_features_rebuild($module_name) {
  islandora_sync_xml_ds_settings_features_disable_feature($module_name);
  $datastreams = module_invoke($module_name, 'islandora_sync_xml_ds_settings_defaults');
  if (!empty($datastreams)) {
    foreach ($datastreams as $datastream) {
      foreach ($datastream['namespaces'] as $namespace) {
        $ds_namespace = array(
          'ds_id' => $namespace['ds_id'],
          'prefix' => $namespace['prefix'],
          'uri' => $namespace['uri'],
        );
        drupal_write_record('islandora_sync_xml_ds_namespaces', $ds_namespace);
      }
      unset($datastream['namespaces']);
      drupal_write_record('islandora_sync_xml_ds', $datastream);
    }
  }
}

/**
 * Implements hook_features_enable_feature().
 */
function islandora_sync_xml_ds_settings_features_enable_feature($module_name) {
  islandora_sync_xml_ds_settings_features_rebuild($module_name);
}

/**
 * Implements hook_features_disable_feature().
 */
function islandora_sync_xml_ds_settings_features_disable_feature($module_name) {
  $datastreams = module_invoke($module_name, 'islandora_sync_xml_ds_settings_defaults');
  foreach ($datastreams as $datastream) {
    db_delete('islandora_sync_xml_ds_namespaces')->condition('ds_id', $datastream['ds_id'])->execute();
    db_delete('islandora_sync_xml_ds')->condition('ds_id', $datastream['ds_id'])->execute();
  }
}
