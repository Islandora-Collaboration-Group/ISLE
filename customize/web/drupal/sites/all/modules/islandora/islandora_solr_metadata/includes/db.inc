<?php

/**
 * @file
 * This file contains DB query functions
 */

/**
 * Build up the key for a configuration name.
 *
 * @param int $config_id
 *   The ID of the configuration of which the name is to be translated.
 *
 * @return string
 *   A colon-separated string as used by the
 *   i18n_string_{update,remove,translate} functions.
 */
function islandora_solr_metadata_config_i18n_id($config_id) {
  return format_string('islandora_solr_metadata:configuration_name:@id', array(
    '@id' => $config_id,
  ));
}

/**
 * Build up the key for a field.
 *
 * @param int $config_id
 *   The ID of the configuration with which the given field is associated.
 * @param string $field_name
 *   The field of which the label is to be translated.
 *
 * @return string
 *   A colon-separated string as used by the
 *   i18n_string_{update,remove,translate} functions.
 */
function islandora_solr_metadata_field_i18n_id($config_id, $field_name) {
  return format_string('islandora_solr_metadata:field_display_label:@id:@field', array(
    '@id' => $config_id,
    '@field' => $field_name,
  ));
}

/**
 * Build up the key for the description label.
 *
 * @param int $config_id
 *   The ID of the configuration with which the given field is associated.
 *
 * @return string
 *   A colon-separated string as used by the
 *   i18n_string_{update,remove,translate} functions.
 */
function islandora_solr_metadata_description_i18n_id($config_id) {
  return format_string('islandora_solr_metadata:description_label:@id', array(
    '@id' => $config_id,
  ));
}

/**
 * Get all configurations available.
 *
 * @return array
 *   An array of associative arrays, each of which contains:
 *   - name: The name of the given configuration, possibly translated via
 *     i18n_string.
 *   - id: The ID of the given configuration.
 */
function islandora_solr_metadata_get_associations() {
  $associations = array();
  $result = db_query('SELECT configuration_id, configuration_name, machine_name FROM {islandora_solr_metadata_associations}');
  foreach ($result as $record) {
    $config_name = $record->configuration_name;
    if (module_exists('i18n_string')) {
      $i18n_id = islandora_solr_metadata_config_i18n_id($record->configuration_id);
      $config_name = i18n_string_translate($i18n_id, $config_name);
    }

    $associations[] = array(
      'name' => $config_name,
      'id' => $record->configuration_id,
      'machine_name' => $record->machine_name,
    );
  }
  return $associations;
}

/**
 * Get all display configurations based upon an object's content models.
 *
 * @param array $cmodels
 *   An array containing content models of the object we are retrieving from.
 *
 * @return array
 *   An array of strings, representing display configuration IDs.
 */
function islandora_solr_metadata_get_associations_by_cmodels(array $cmodels) {
  return db_select('islandora_solr_metadata_cmodels', 'i')
    ->fields('i', array('configuration_id'))
    ->condition('cmodel', $cmodels)
    ->execute()
    ->fetchAllAssoc('configuration_id', PDO::FETCH_ASSOC);
}

/**
 * Get models associated with the given configuration.
 *
 * @param int $configuration_id
 *   The ID of the configuration for which to grab the content models.
 *
 * @return array
 *   An associative array mapping content model PIDs to an associative array
 *   containing:
 *   - cmodel: The PID.
 */
function islandora_solr_metadata_get_cmodels($configuration_id) {
  $result = db_query('SELECT cmodel FROM {islandora_solr_metadata_cmodels} WHERE configuration_id = :configuration_id', array(':configuration_id' => $configuration_id));
  return $result->fetchAllAssoc('cmodel', PDO::FETCH_ASSOC);
}

/**
 * Create a new configuration.
 *
 * @param string $configuration_name
 *   The name of a configuration to add the the DB.
 * @param string $machine_name
 *   The unique machine name to add the the DB.
 *
 * @return int
 *   The ID with which the configuration was created in the database.
 */
function islandora_solr_metadata_add_configuration($configuration_name, $machine_name) {
  $id = db_insert('islandora_solr_metadata_associations')
    ->fields(
      array(
        'configuration_name' => $configuration_name,
        'machine_name' => $machine_name,
      )
    )
    ->execute();

  if (module_exists('i18n_string')) {
    $i18n_id = islandora_solr_metadata_config_i18n_id($id);
    i18n_string_update($i18n_id, $configuration_name);
  }

  return $id;
}

/**
 * Get all fields in the given display.
 *
 * @param int $configuration_id
 *   The ID for the configuration for which to get the list of fields.
 * @param bool $translate
 *   Whether or not to attempt to translate the "display labels" with i18n.
 *   Defaults to TRUE.
 *
 * @return array
 *   An associative array of associative arrays, representing Solr fields
 *   mapped to the details about the given field, including:
 *   - configuration_id
 *   - display_label
 *   - weight
 *   - solr_field
 */
function islandora_solr_metadata_get_fields($configuration_id, $translate = TRUE) {
  $result = db_query('SELECT * FROM {islandora_solr_metadata_fields} WHERE configuration_id = :configuration_id ORDER BY weight', array(':configuration_id' => $configuration_id));

  $fields = $result->fetchAllAssoc('solr_field', PDO::FETCH_ASSOC);
  $deserialize_data = function ($field) {
    // Merge unserialized blob into field array.
    $field += unserialize($field['data']);

    // Add in defaults.
    $field += array(
      'display_label' => $field['solr_field'],
      'hyperlink' => 0,
    );

    return $field;
  };
  $fields = array_map($deserialize_data, $fields);

  if ($translate && module_exists('i18n_string')) {
    foreach ($fields as $field => &$field_info) {
      $i18n_id = islandora_solr_metadata_field_i18n_id($configuration_id, $field);
      $field_info['display_label'] = i18n_string_translate($i18n_id, $field_info['display_label']);
    }
    // XXX: foreach with references can be silly.
    unset($field_info);
  }
  return $fields;
}

/**
 * Get the name for the given configuration.
 *
 * @param int $configuration_id
 *   The ID for a configuration for which to grab the name.
 *
 * @return string
 *   The name of the configuration. Will be translated with i18n if the
 *   i18n_string module is enabled.
 */
function islandora_solr_metadata_retrieve_configuration_name($configuration_id) {
  $result = db_query('SELECT configuration_name from {islandora_solr_metadata_associations} WHERE configuration_id = :configuration_id', array(':configuration_id' => $configuration_id));
  $config_name = $result->fetchColumn();

  if (module_exists('i18n_string')) {
    $i18n_id = islandora_solr_metadata_config_i18n_id($configuration_id);
    $config_name = i18n_string_translate($i18n_id, $config_name);
  }

  return $config_name;
}

/**
 * Get the ID for the given configuration.
 *
 * @param string $machine_name
 *   The machine name of the configuration ID we are grabbing.
 *
 * @return bool|int
 *   The configuration ID if it exists; otherwise, FALSE.
 */
function islandora_solr_metadata_retrieve_configuration_from_machine_name($machine_name) {
  $result = db_query('SELECT configuration_id from {islandora_solr_metadata_associations} WHERE machine_name = :machine_name', array(':machine_name' => $machine_name));
  $config_id = $result->fetchColumn();
  return $config_id;
}

/**
 * Delete the given configuration indicated by the given ID.
 *
 * @param int $configuration_id
 *   The ID for a configuration to remove.
 */
function islandora_solr_metadata_delete_configuration($configuration_id) {
  if (module_exists('i18n_string')) {
    $fields = islandora_solr_metadata_get_fields($configuration_id, FALSE);
    foreach (array_keys($fields) as $field) {
      $i18n_id = islandora_solr_metadata_field_i18n_id($configuration_id, $field);
      i18n_string_remove($i18n_id);
    }
  }
  db_delete('islandora_solr_metadata_fields')
    ->condition('configuration_id', $configuration_id)
    ->execute();

  db_delete('islandora_solr_metadata_cmodels')
    ->condition('configuration_id', $configuration_id)
    ->execute();

  if (module_exists('i18n_string')) {
    $i18n_id = islandora_solr_metadata_config_i18n_id($configuration_id);
    $i18n_description_id = islandora_solr_metadata_description_i18n_id($configuration_id);
    i18n_string_remove($i18n_id);
    i18n_string_remove($i18n_description_id);
  }

  db_delete('islandora_solr_metadata_associations')
    ->condition('configuration_id', $configuration_id)
    ->execute();
}

/**
 * Check that a configuration exists with the given ID.
 *
 * @param int $configuration_id
 *   An ID to test.
 *
 * @return bool
 *   Whether or not the ID exists.
 */
function islandora_solr_metadata_configuration_exists($configuration_id) {
  $result = db_query('SELECT COUNT(*) FROM {islandora_solr_metadata_associations} WHERE configuration_id = :configuration_id', array(':configuration_id' => $configuration_id));
  return intval($result->fetchField()) > 0;
}

/**
 * Add content models to a specified configuration ID.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to add to.
 * @param array $content_models
 *   An associative array mapping content model PIDs to an associative array
 *   containing:
 *   - cmodel: The PID.
 */
function islandora_solr_metadata_add_content_models($configuration_id, $content_models) {
  $query = db_insert('islandora_solr_metadata_cmodels')
    ->fields(array('configuration_id', 'cmodel'));
  foreach ($content_models as $content_model) {
    $query->values(array(
      'configuration_id' => $configuration_id,
      'cmodel' => $content_model['cmodel'],
    ));
  }
  $query->execute();
}

/**
 * Delete content models from a specified configuration ID.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to add to.
 * @param array $content_models
 *   An associative array mapping content model PIDs to an associative array
 *   containing:
 *   - cmodel: The PID.
 */
function islandora_solr_metadata_delete_content_models($configuration_id, $content_models) {
  db_delete('islandora_solr_metadata_cmodels')
    ->condition('cmodel', array_keys($content_models))
    ->condition('configuration_id', $configuration_id)
    ->execute();
}

/**
 * Add fields to a specified configuration ID.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to add to.
 * @param array $fields
 *   An associative array of associative arrays, containing details about the
 *   given field being added, including:
 *   - configuration_id
 *   - display_label
 *   - weight
 *   - solr_field
 */
function islandora_solr_metadata_add_fields($configuration_id, $fields) {
  $query = db_insert('islandora_solr_metadata_fields')
    ->fields(array(
        'configuration_id',
        'solr_field',
        'weight',
        'data',
      )
    );

  foreach ($fields as $field) {
    $query->values(array(
      'configuration_id' => $configuration_id,
      'solr_field' => $field['solr_field'],
      'weight' => $field['weight'],
      'data' => serialize(array_diff_key($field, islandora_solr_metadata_fields_non_data_fields())),
    ));
  }
  $query->execute();
  islandora_solr_metadata_update_i18n_strings($configuration_id, $fields);
}

/**
 * Delete fields from a configuration given configuration ID.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to add to.
 * @param array $fields
 *   An associative array of associative arrays, representing Solr fields
 *   mapped to the details about the given field, including:
 *   - configuration_id
 *   - display_label
 *   - weight
 *   - solr_field
 */
function islandora_solr_metadata_delete_fields($configuration_id, $fields) {
  db_delete('islandora_solr_metadata_fields')
    ->condition('solr_field', array_keys($fields))
    ->condition('configuration_id', $configuration_id)
    ->execute();
  islandora_solr_metadata_remove_i18n_strings($configuration_id, $fields);
}

/**
 * Updates existing fields in a configuration given configuration ID.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to add to.
 * @param array $fields
 *   An associative array of associative arrays, representing Solr fields
 *   mapped to the details about the given field, including:
 *   - configuration_id
 *   - display_label
 *   - weight
 *   - solr_field
 */
function islandora_solr_metadata_update_fields($configuration_id, $fields) {
  $not_data = islandora_solr_metadata_fields_non_data_fields();
  foreach ($fields as $field) {
    db_update('islandora_solr_metadata_fields')
      ->fields(array(
        'weight' => $field['weight'],
        'data' => serialize(array_diff_key($field, $not_data)),
      ))
      ->condition('solr_field', $field['solr_field'])
      ->condition('configuration_id', $configuration_id)
      ->execute();
  }
  islandora_solr_metadata_update_i18n_strings($configuration_id, $fields);
}

/**
 * Updates existing fields in a configuration given configuration ID.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to add to.
 * @param bool $translate
 *   Whether the output needs to be translated.
 *
 * @return array
 *   An associative array containing:
 *   -description_field: The Solr field used to generate the description.
 *   -description_label: The display label associated with the description.
 *   -description_data: An array of extra data for custom solr metadata display.
 *     -default_values: An array of saved field values for default display.
 *       -truncation_type: As excepted by truncate_utf8().
 *       -max_length: As excepted by truncate_utf8().
 *       -word_safe: As excepted by truncate_utf8().
 *       -ellipsis: As excepted by truncate_utf8().
 *       -min_wordsafe_length: As excepted by truncate_utf8().
 *     -min_wordsafe_length_input_path: The ajax input name path for wordsafe
 * length.
 */
function islandora_solr_metadata_retrieve_description($configuration_id, $translate = TRUE) {
  $result = db_query('SELECT description_field, description_label, description_data FROM {islandora_solr_metadata_associations} WHERE configuration_id = :configuration_id', array(':configuration_id' => $configuration_id));
  $description = $result->fetchAssoc();
  if ($translate && module_exists('i18n_string')) {
    $i18n_id = islandora_solr_metadata_description_i18n_id($configuration_id);
    $description['description_label'] = i18n_string_translate($i18n_id, $description['description_label']);
  }

  if (!empty($description['description_data'])) {
    $description['description_data'] = unserialize($description['description_data']);
  }

  return $description;
}

/**
 * Updates the description field and label in the database.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to update.
 * @param string $description_field
 *   The Solr field which we are grabbing the description value from.
 * @param string $description_label
 *   The label for the description metadata.
 * @param array $description_data
 *   An array of extra data to be stored for custom solr metadata display.
 */
function islandora_solr_metadata_update_description($configuration_id, $description_field, $description_label, $description_data = array()) {
  if (empty($description_field)) {
    $description_field = NULL;
    $description_label = NULL;
    $description_data = NULL;
  }
  db_update('islandora_solr_metadata_associations')
    ->fields(array(
      'description_field' => $description_field,
      'description_label' => $description_label,
      'description_data' => serialize($description_data),
    ))
    ->condition('configuration_id', $configuration_id)
    ->execute();
  islandora_solr_metadata_update_i18n_description($configuration_id, $description_label);
}

/**
 * Update i18n entries for the given fields.
 *
 * @param int $id
 *   An ID for the configuration.
 * @param array $fields
 *   An associative array of associative arrays, representing Solr fields
 *   mapped to the details about the given field, including:
 *   - configuration_id
 *   - display_label
 *   - weight
 *   - solr_field
 */
function islandora_solr_metadata_update_i18n_strings($id, $fields) {
  if (module_exists('i18n_string')) {
    foreach ($fields as $field => $info) {
      $i18n_id = islandora_solr_metadata_field_i18n_id($id, $info['solr_field']);
      i18n_string_update($i18n_id, $info['display_label']);
    }
  }
}

/**
 * Remove i18n entries for the given fields.
 *
 * @param int $id
 *   An ID for the configuration.
 * @param array $fields
 *   An associative array of associative arrays, representing Solr fields
 *   mapped to the details about the given field, including:
 *   - configuration_id
 *   - display_label
 *   - weight
 *   - solr_field
 */
function islandora_solr_metadata_remove_i18n_strings($id, $fields) {
  if (module_exists('i18n_string')) {
    foreach ($fields as $field => $info) {
      $i18n_id = islandora_solr_metadata_field_i18n_id($id, $info['solr_field']);
      i18n_string_remove($i18n_id);
    }
  }
}

/**
 * Update i18n entries for the description.
 *
 * @param int $id
 *   An ID for the configuration.
 * @param string $description_label
 *   The description label that is being added or updated.
 */
function islandora_solr_metadata_update_i18n_description($id, $description_label) {
  if (module_exists('i18n_string')) {
    $i18n_id = islandora_solr_metadata_description_i18n_id($id);
    i18n_string_update($i18n_id, $description_label);
  }
}

/**
 * Remove i18n entries for the description.
 *
 * @param int $id
 *   An ID for the configuration.
 */
function islandora_solr_metadata_remove_i18n_description($id) {
  if (module_exists('i18n_string')) {
    $i18n_id = islandora_solr_metadata_description_i18n_id($id);
    i18n_string_remove($i18n_id);
  }
}

/**
 * Helper to list the actual columns of "islandora_solr_metadata_fields".
 *
 * That is to say, the values which are their own columns in the
 * "islandora_solr_metadata_fields",
 */
function islandora_solr_metadata_fields_non_data_fields() {
  $schema = drupal_get_schema('islandora_solr_metadata_fields');
  return $schema['fields'];
}
