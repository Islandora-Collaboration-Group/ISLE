<?php

/**
 * @file
 * Collection of functions related to importing Fedora content to Drupal.
 */

/**
 * Entry point in creation and updating of Drupal nodes based on a Fedora PID.
 *
 * @param string $pid
 *   Fedora object pid.
 * @param string $type
 *   Drupal bundle type (node type).
 * @param int $nid
 *   Drupal node ID, use for content updates.
 * @param string $ds_id
 *   The Fedora object datastream ID, use to limit scope of a content update.
 *
 * @return int
 *   Node ID of created/updated Drupal node.
 */
function islandora_sync_import_from_pid($pid, $type, $nid = FALSE, $ds_id = NULL) {
  $object = islandora_object_load($pid);

  if (!$nid) {
    $node = new stdClass();
    $node->type = $type;
    $node->language = LANGUAGE_NONE;
    node_object_prepare($node);
  }
  else {
    $node = node_load($nid);
    $node->revision = 1;
    $node->log = 'Updated by islandora_sync';
  }

  if (!isset($node->title)) {
    $node->title = $object->label;
  }

  $author = user_load_by_name($object->owner);
  if ($author) {
    $node->uid = $author->uid;
  }
  else {
    $node->uid = 1;
  }
  // Call node_save to get a nid if it doesn't exist.
  if (!$nid) {
    node_save($node);
  }

  // Populate the node's fields.
  islandora_sync_build_node_fields($node, $object, $ds_id);
  node_save($node);

  if (!$nid) {
    $map['pid'] = $pid;
    $map['type'] = 'node';
    $map['bundle'] = $type;
    $map['entity_id'] = $node->nid;
    drupal_write_record('islandora_sync_map', $map);
  }

  return $node->nid;
}

/**
 * Populate a node's fields based on mappings of Fedora content to Drupal.
 *
 * @param Node $node
 *   The node object that is being updated.
 * @param AbstractObject $object
 *   The Fedora object being synced to Drupal.
 * @param string $ds_id
 *   The Fedora object datastream ID.
 */
function islandora_sync_build_node_fields(&$node, $object, $ds_id) {
  // Check to see if a folder already exists for this node, if not create it.
  $node_path = "public://islandora_sync/$node->nid";
  if (!file_prepare_directory($node_path)) {
    drupal_mkdir($node_path);
  }
  $fields_to_populate_query = db_select('islandora_sync_fields', 'isf');
  $fields_to_populate_query->leftJoin('field_config', 'fc', 'fc.field_name = isf.field');
  $fields_to_populate_query->fields('isf')
    ->fields('fc', array('type'))
    ->condition('isf.bundle', $node->type, '=');

  if (!is_null($ds_id)) {
    $fields_to_populate_query->condition('isf.dsid', $ds_id, '=');
  }

  $fields_to_populate = $fields_to_populate_query->execute()->fetchAll();

  foreach ($fields_to_populate as $field) {
    $return = module_invoke_all('islandora_sync_node_field_build', $node, $object, $field, $ds_id);

    if (!$return) {
      $values = islandora_sync_get_field_values($field, $object, $node->type, array());
      $existing_title = $node->title;
      $node->{$field->field} = array();
      $node->title = $existing_title;
      if ($field->field != 'title') {
        foreach ($values as $value) {
          islandora_sync_save_field($field, $node, $value);
        }
      }
      else {
        if (isset($values[0]) && $values[0] != $node->title) {
          $node->title = $values[0];
        }
      }
    }
  }
}

/**
 * Extract content from Fedora data structures.
 *
 * The extracted content is based on defined mappings to Drupal fields.
 *
 * @param stdObject $field
 *   Field object record as stored in islandora_sync_fields table.
 * @param AbstractObject $object
 *   The Fedora object containing values.
 * @param string $bundle
 *   The bundle (node type).
 * @param array $parents
 *   Parent field names.
 *
 * @return array
 *   An array containing the field values.
 */
function islandora_sync_get_field_values($field, $object, $bundle, $parents = array()) {
  $values = array();

  switch ($field->export_type) {
    case 'xml_value':
      $root = db_query("SELECT isxdn.prefix, isxd.root FROM {islandora_sync_xml_ds_namespaces} isxdn
        LEFT JOIN {islandora_sync_xml_ds} isxd ON isxd.namespace_uri = isxdn.uri
        WHERE isxd.ds_id = :ds_id",
        array(':ds_id' => $field->dsid)
      )->fetchObject();

      if (isset($root->prefix) && isset($root->root)) {
        $read_path = '/' . $root->prefix . ':' . $root->root;
      }
      else {
        $read_path = '//';
      }

      $field_path = db_query("SELECT read_path FROM {islandora_sync_fields} WHERE field = :field AND bundle = :bundle",
        array(':field' => $field->field, ':bundle' => $field->bundle)
      )->fetchField();
      if ($field->read_context == 'parent') {
        $parent_path = islandora_sync_get_parent_path($field, $bundle, $parents);
        $read_path .= $parent_path;
        if ($field_path) {
          $read_path .= '/' . $field_path;
        }
      }
      else {
        $read_path = $field_path;
      }

      $ds_content = FALSE;
      try {
        if (isset($object[$field->dsid])) {
          $ds_content = $object[$field->dsid]->content;
        }
      }
      catch (Exception $e) {
        watchdog_exception('islandora_sync', $e);
      }

      if ($ds_content) {
        $doc = new DOMDocument();
        $doc->loadXML($ds_content);
        if ($doc) {
          $xpath = new DOMXPath($doc);
          $xml_values = $xpath->query($read_path);
          foreach ($xml_values as $xml_value) {
            $values[] = $xml_value->nodeValue;
          }
        }
      }

      break;

    case 'managed_datastream':
      try {
        if (isset($object[$field->dsid])) {
          // @TODO: check for memory problems.
          $values[] = array(
            'content' => $object[$field->dsid]->content,
            'mimetype' => $object[$field->dsid]->mimetype,
          );
        }
      }
      catch (Exception $e) {
        watchdog_exception('islandora_sync', $e);
      }
      break;

    case 'relationship':
      $rels = $object->relationships->get($field->predicate_uri, $field->predicate);
      foreach ($rels as $rel) {
        $values[] = $rel['object']['value'];
      }
      break;
  }

  return $values;
}

/**
 * Format a node's field structure for saving.
 *
 * This is only for core module fields.
 *
 * @param stdObject $field_info
 *   Field object record as stored in islandora_sync_fields table.
 * @param Node $entity
 *   The entity (node) being populated.
 * @param array $value
 *   The field value.
 */
function islandora_sync_save_field($field_info, &$entity, $value) {
  $lang = isset($entity->language) ? $entity->language : LANGUAGE_NONE;
  $id = isset($entity->nid) ? $entity->nid : user_password();
  $prepared_field = &$entity->{$field_info->field}[$lang];
  switch ($field_info->type) {
    case 'image':
    case 'file':
      module_load_include('inc', 'islandora', 'mime_detect');
      $mime_detect = new MimeDetect();
      $extension = $mime_detect->getExtension($value['mimetype']);
      $file_name = $field_info->dsid . '-' . $entity->type;
      $params = array(
        'field_info' => $field_info,
        'entity' => $entity,
        'value' => $value,
      );
      drupal_alter('islandora_sync_file_name', $file_name, $params);
      $full_file = "{$file_name}-{$id}.{$extension}";
      $file_uri = "public://islandora_sync/$id/$full_file";
      if (module_exists('image')) {
        image_path_flush($file_uri);
      }
      $file = file_save_data($value['content'], $file_uri, FILE_EXISTS_REPLACE);
      $file->display = 1;
      $prepared_field[] = (array) $file;
      break;

    case 'taxonomy_term_reference':
      $term_field_info = db_query("SELECT * FROM {field_config_instance} WHERE field_name = :field_name AND bundle = :bundle",
        array(':field_name' => $field_info->field, ':bundle' => $field_info->bundle)
      )->fetchObject();
      $term_data = unserialize($term_field_info->data);
      $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE name = :name",
        array(':name' => $term_data['label'])
      )->fetchField();

      $terms = taxonomy_get_term_by_name($value);
      if (!empty($terms)) {
        $term_id = key($terms);
      }
      else {
        if (!empty($value)) {
          $term = new stdClass();
          $term->name = $value;
          $term->vid = $vid;
          taxonomy_term_save($term);
          $term_id = $term->tid;
        }
        else {
          break;
        }
      }
      $prepared_field[]['tid'] = $term_id;
      break;

    default:
      $prepared_field[]['value'] = $value;
  }
}

/**
 * Get the parent field path of a field.
 *
 * @param stdObject $field
 *   Field object record as stored in islandora_sync_fields table.
 * @param string $bundle
 *   The node's bundle.
 * @param array $parents
 *   Known parents.
 *
 * @return string
 *   The path to parent field.
 */
function islandora_sync_get_parent_path($field, $bundle, $parents = array()) {
  $path = '';
  $parents = islandora_sync_get_parent_fields($field->field, $field->bundle, $parents);

  $parents = array_reverse($parents);
  foreach ($parents as $parent) {
    $field_read = db_query("SELECT read_path, read_context
      FROM {islandora_sync_fields}
      WHERE field = :field
      AND bundle = :bundle",
      array(':field' => $parent, ':bundle' => $bundle)
    )->fetchObject();
    if ($field_read && $field_read->read_path != $path) {
      if ($field_read->read_context == 'parent') {
        $path .= '/' . $field_read->read_path;
      }
      elseif ($field_read->read_context == 'document') {
        $path .= '/' . $field_read->read_path;
        return $path;
      }
    }
  }

  return $path;
}

/**
 * Retrieves parent fields.
 *
 * @param string $field
 *   The name of the field.
 * @param string $bundle
 *   The name of the bundle.
 * @param array $parents
 *   The known parents.
 *
 * @return array
 *   All parent fields.
 */
function islandora_sync_get_parent_fields($field, $bundle, $parents) {
  $start_count = count($parents);

  if (module_exists('field_group')) {

    ctools_include('export');
    $groups = ctools_export_load_object('field_group', 'conditions', array(
      'entity_type' => 'node',
      'bundle' => $bundle,
    ));
    foreach ($groups as $group) {
      if (in_array($field, $group->data['children'])) {
        $parents[] = $group->group_name;
        if ($group->parent_name) {
          $parents[] = $group->parent_name;
        }
      }
    }
  }

  $current_count = count($parents);
  if ($current_count > $start_count) {
    $parents = islandora_sync_get_parent_fields($parents[$current_count - 1], $bundle, $parents);
  }

  return $parents;
}
