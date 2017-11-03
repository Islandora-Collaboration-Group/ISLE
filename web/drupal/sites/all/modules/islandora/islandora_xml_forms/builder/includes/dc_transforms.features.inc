<?php

/**
 * @file
 * Features hook implementations.
 */

/**
 * Helper function so we can so we can identify things.
 */
function xml_form_builder_default_dc_config_get_mapped_features() {
  $mapped =& drupal_static(__FUNCTION__, array());

  if (empty($mapped)) {
    $query = db_select('xml_form_builder_default_xslts', 'mappings')
      ->fields('mappings', array());

    $alias = $query->join('xml_form_builder_xslts', 'x', 'x.xslt_id = mappings.xslt_id');
    $query->addField($alias, 'name', 'xslt_name');
    $query->addField($alias, 'xslt');

    $configs = $query->execute()
      ->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    foreach ($configs as $config) {
      $mapped["{$config['content_model']}--{$config['dsid']}"] = $config;
    }
  }
  return $mapped;
}

/**
 * Implements hook_features_export_options().
 */
function xml_form_builder_default_dc_config_features_export_options() {
  $configs = xml_form_builder_default_dc_config_get_mapped_features();

  $map_to_name = function ($config) {
    return $config['name'];
  };
  $features_config = array_map($map_to_name, $configs);

  return $features_config;
}

/**
 * Implements hook_features_export().
 */
function xml_form_builder_default_dc_config_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['xml_form_builder'] = 'xml_form_builder';
  foreach ($data as $component) {
    $export['features']['xml_form_builder_default_dc_config'][$component] = $component;
  }

  // Add in XSLT names to attempt to produce conflicts, as appropriate.
  $map_xslt_name = function ($config) {
    $configs = xml_form_builder_default_dc_config_get_mapped_features();
    return isset($configs[$config]) ? $configs[$config]['xslt_name'] : FALSE;
  };
  $xslt_names = array_unique(array_map($map_xslt_name, $data));
  $export['features']['xml_form_builder_default_dc_xslt'] = drupal_map_assoc($xslt_names);
}

/**
 * Implements hook_features_export_render().
 */
function xml_form_builder_default_dc_config_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');

  $code = array();
  $drop_fields = drupal_map_assoc(array('id', 'xslt_id'));
  foreach (array_intersect_key(xml_form_builder_default_dc_config_get_mapped_features(), drupal_map_assoc($data)) as $key => $item) {
    $code[$key] = array_diff_key($item, $drop_fields);
  }

  $serialized_code = 'return ' . features_var_export($code) . ';';
  return array('xml_form_builder_default_dc_config' => $serialized_code);
}

/**
 * Implements hook_features_revert().
 */
function xml_form_builder_default_dc_config_features_revert($module_name) {
  db_truncate('xml_form_builder_default_xslts');
  db_truncate('xml_form_builder_xslts');
  drupal_static_reset('xml_form_builder_default_dc_config_get_mapped_features');
  xml_form_builder_default_dc_config_features_rebuild(NULL);
}

/**
 * Implements hook_features_rebuild().
 */
function xml_form_builder_default_dc_config_features_rebuild($module_name) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');

  // We pass NULL when reverting, after nuking the relevant DB tables, so we do
  // a full rebuild of the relevant features.
  $values = $module_name === NULL ?
    module_invoke_all('xml_form_builder_default_dc_config') :
    module_invoke($module_name, 'xml_form_builder_default_dc_config');

  if (!empty($values)) {
    $xslts = db_select('xml_form_builder_xslts', 'x')
      ->fields('x', array())
      ->execute()
      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);

    foreach (array_diff_key($values, xml_form_builder_default_dc_config_get_mapped_features()) as $key => $info) {
      $old = $delete_old = FALSE;

      if (isset($xslts[$info['xslt_name']])) {
        $old = $xslts[$info['xslt_name']];
        if ($old['xslt'] != $info['xslt']) {
          // Stash old content we're replacing for a while, to allow a chance
          // to recover if things don't work.
          $file = file_save_data($info['xslt'], 'public://');
          $file->status &= ~FILE_STATUS_PERMANENT;
          $file = file_save($file);
          features_log(t('Replacing !current_content of @xslt_name with feature content.', array(
            '!current_content' => l(t('current content'), file_create_url($file->uri)),
            '@xslt_name' => $info['xslt_name'],
          )), 'warning');

          // XSLT exists with same name, but different contents; replace it.
          // Change name of old entry.
          $old_name_replacement = str_rot13($old['name']);
          while (isset($xslts[$old_name_replacement])) {
            // If there happens to be a collision, generate a random name.
            $old_name_replacement = bin2hex(drupal_random_bytes(8));
          }
          db_update('xml_form_builder_xslts')
            ->fields(array(
              'name' => $old_name_replacement,
            ))
            ->condition('xslt_id', $old['xslt_id'])
            ->execute();
          $delete_old = TRUE;
        }
      }

      if (!$old || $delete_old) {
        // Add new one.
        $xslts[$info['xslt_name']] = array(
          'name' => $info['xslt_name'],
          'xslt' => $info['xslt'],
          'xslt_id' => xml_form_builder_add_xslt($info['xslt'], $info['xslt_name']),
        );
      }

      if ($old && $delete_old) {
        // Update mappings to point to new XSLT.
        db_update('xml_form_builder_default_xslts')
          ->fields(array(
            'xslt_id' => $xslt[$info['xslt_name']],
          ))
          ->condition('xslt_id', $old['xslt_id'])
          ->execute();
        // Remove old one.
        db_delete('xml_form_builder_xslts')
          ->condition('xslt_id', $old['xslt_id'])
          ->execute();
      }

      xml_form_builder_add_default_dc_transform_mapping(
        $info['content_model'],
        $info['dsid'],
        $xslts[$info['xslt_name']]['xslt_id'],
        $info['name']
      );
    }
  }
}
