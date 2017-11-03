<?php

/**
 * @file
 * This file documents all available hook functions to manipulate data.
 */

/**
 * Enables modules to prepare/build and save node fields.
 *
 * @param stdObject $node
 *   The Node object to be saved.
 * @param FedoraObject $object
 *   The Fedora Commons object that is being synced.
 * @param stdObject $field
 *   The node field that has been configured to be synced from Fedora Commons.
 */
function hook_islandora_sync_node_field_build($node, $object, $field) {
  $values = islandora_sync_get_field_values($field, $object, $node->type);
  $node->{$field->field} = array();
  if ($field->field != 'title') {
    foreach ($values as $value) {
      islandora_sync_save_field($field, $node, $value);
    }
  }
}

/**
 * Allows modules to change file names before they are saved.
 *
 * @param string $file_name
 *   The current file URI.
 * @param array $params
 *   An array containing:
 *   -field_info (stdObject): Field object record as stored in
 *   islandora_sync_fields table.
 *   -entity (Node): The entity (node) being populated.
 *   -value (array): The field value.
 */
function hook_islandora_sync_file_name_alter(&$file_name, $params) {
  $file_name = 'test.mime';
}
