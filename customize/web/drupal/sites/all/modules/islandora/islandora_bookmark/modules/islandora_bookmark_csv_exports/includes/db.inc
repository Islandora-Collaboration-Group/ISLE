<?php
/**
 * @file
 * Holds database helper functions for the module.
 */

/**
 * Get all configurations available.
 *
 * @return array
 *   An array of associative arrays, each of which contains:
 *   - name: The name of the given configuration.
 *   - id: The ID of the given configuration.
 *   - machine_name: The machine name of the given configuration.
 *   - descrip: The description of the configuration.
 */
function islandora_bookmark_csv_exports_get_configurations() {
  $configurations = array();
  $result = db_query('SELECT configuration_id, configuration_name, machine_name, descrip, config_fields FROM {islandora_bookmark_csv_exports}');
  foreach ($result as $record) {
    $config_name = $record->configuration_name;
    $configurations[] = array(
      'name' => $config_name,
      'id' => $record->configuration_id,
      'machine_name' => $record->machine_name,
      'descrip' => $record->descrip,
      'fields' => $record->config_fields,
    );
  }
  return $configurations;
}

/**
 * Gets configuration by ID.
 *
 * @param string $configuration_id
 *   The configuration ID of the export to get.
 *
 * @return array
 *   An array describing the configuration.
 */
function islandora_bookmark_csv_exports_get_configuration_by_id($configuration_id) {
  return db_select('islandora_bookmark_csv_exports', 'i')
    ->fields('i', array())
    ->condition('i.configuration_id', $configuration_id)
    ->execute()
    ->fetchAssoc();
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
function islandora_bookmark_csv_exports_get_configuration_from_machine_name($machine_name) {
  $result = db_query('SELECT configuration_id from {islandora_bookmark_csv_exports} WHERE machine_name = :machine_name', array(':machine_name' => $machine_name));
  $config_id = $result->fetchColumn();
  return $config_id;
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
function islandora_bookmark_csv_exports_add_configuration($configuration_name, $machine_name) {
  $id = db_insert('islandora_bookmark_csv_exports')
    ->fields(
      array(
        'configuration_name' => $configuration_name,
        'machine_name' => $machine_name,
      )
    )
    ->execute();
  return $id;
}

/**
 * Get fields associated with the given configuration.
 *
 * @param int $configuration_id
 *   The ID of the configuration for which to grab the fields.
 *
 * @return array
 *   An array containing the unserialized fields for the config.
 */
function islandora_bookmark_csv_exports_get_values($configuration_id) {
  $result = db_query('SELECT descrip, config_fields FROM {islandora_bookmark_csv_exports} WHERE configuration_id = :configuration_id', array(':configuration_id' => $configuration_id));
  $values = $result->fetchAssoc();
  $values['config_fields'] = unserialize($values['config_fields']);
  return $values;
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
function islandora_bookmark_csv_exports_configuration_exists($configuration_id) {
  $result = db_query('SELECT COUNT(*) FROM {islandora_bookmark_csv_exports} WHERE configuration_id = :configuration_id', array(':configuration_id' => $configuration_id));
  return intval($result->fetchField()) > 0;
}

/**
 * Get the name for the given configuration.
 *
 * @param int $configuration_id
 *   The ID for a configuration for which to grab the name.
 *
 * @return string
 *   The name of the configuration.
 */
function islandora_bookmark_csv_exports_retrieve_configuration_name($configuration_id) {
  $result = db_query('SELECT configuration_name from {islandora_bookmark_csv_exports} WHERE configuration_id = :configuration_id', array(':configuration_id' => $configuration_id));
  $config_name = $result->fetchColumn();
  return $config_name;
}

/**
 * Add things to the database.
 *
 * @param int $configuration_id
 *   The configuration ID of the configuration which to add to.
 * @param array $values
 *   An associative array containing values of things:
 *   - fields: A serialize array of Solr fields.
 *   - description: A string describing the configuration.
 *   - delimiter: A string used as glue for multi valued Solr fields.
 */
function islandora_bookmark_csv_exports_add_values($configuration_id, $values) {
  db_update('islandora_bookmark_csv_exports')
    ->fields(array(
        'config_fields' => $values['fields'],
        'descrip' => $values['description'],
        'delimiter' => $values['delimiter'],
      ))
    ->condition('configuration_id', $configuration_id)
    ->execute();
}

/**
 * Delete the given configuration indicated by the given ID.
 *
 * @param int $configuration_id
 *   The ID for a configuration to remove.
 */
function islandora_bookmark_csv_exports_delete_configuration($configuration_id) {
  db_delete('islandora_bookmark_csv_exports')
    ->condition('configuration_id', $configuration_id)
    ->execute();
}

/**
 * Sees if the description exists on a configuration.
 *
 * @param string $description
 *   The description to check.
 * @param string $configuration_id
 *   The configuration ID of the configuration being checked.
 *
 * @return bool
 *   Whether or not the description is unique.
 */
function islandora_bookmark_csv_exports_description_exists($description, $configuration_id) {
  return intval(db_select('islandora_bookmark_csv_exports', 'i')
  ->condition('configuration_id', $configuration_id, '!=')
  ->condition('descrip', $description)
  ->fields('i', array('configuration_id'))
  ->execute()
  ->fetchCol());
}
