<?php

/**
 * @file
 * Collection of functions for exporting Drupal content to Fedora Commons.
 * Currently incomplete.
 */

/**
 * Export to Fedora Commons from a Drupal node.
 *
 * @param stdObject $node
 *   The node to sync to Fedora.
 * @param stdObject $type_info
 *   The islandora_sync settings for this content type (bundle).
 */
function islandora_sync_export_from_node($node, $type_info) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $cmodels = islandora_sync_get_bundle_cmodels($type_info->bundle);
  $namespace = $type_info->namespace ? $type_info->namespace : 'default';
  $object = islandora_prepare_new_object($namespace, $node->title, array(), $cmodels, array());

  $user = user_load($node->uid);
  $object->owner = $user->name;

  $object = islandora_add_object($object);

  // Build datastreams.
  $datastreams = islandora_sync_build_datastreams($node);

  foreach ($datastreams as $ds_id => $datastream) {
    // @TODO: proper datastream construction.
    $ds = $object->constructDatastream($ds_id, 'M');
    $ds->label = $ds_id . ' Record';
    $ds->mimetype = 'text/xml';
    $ds->setContentFromString($datastream['content']);
    $object->ingestDatastream($ds);
  }
  // Log state of sync.
}

/**
 * Construct the Fedora object's datastreams.
 *
 * @param stdObject $node
 *   The node to sync.
 *
 * @return array
 *   Pseudo-datastream structure.
 */
function islandora_sync_build_datastreams($node) {
  $fields = db_query("SELECT * FROM {islandora_sync_fields} WHERE bundle = :bundle AND export = 1",
    array(':bundle' => $node->type)
  )->fetchAll();

  $datastreams = array();
  foreach ($fields as $field) {
    // @TODO: proper language handling.
    $values = field_get_items('node', $node, $field->field);
    if ($values) {
      $dsid = $field->dsid;
      if (!isset($datastreams[$dsid])) {
        $datastreams[$dsid]['type'] = $field->export_type;
        $datastreams[$dsid]['content'] = '';
      }
      foreach ($values as $value) {
        $datastreams[$dsid]['content'] = islandora_sync_build_ds_content($datastreams[$dsid]['content'], reset($value), $field);
      }
    }
  }

  return $datastreams;
}

/**
 * Build the content of a datastream.
 *
 * @param string $content
 *   The field content.
 * @param string $node_field
 *   The fie
 * @param stdObject $field_info
 *   The field info from islandora_sync_fields.
 *
 * @return string
 *   The content.
 */
function islandora_sync_build_ds_content($content, $node_field, $field_info) {
  switch ($field_info->export_type) {
    case 'xml_value':
      if (empty($content)) {
        $content = islandora_sync_init_xml_datastream($field_info);
      }
      $content = islandora_sync_build_xml_datastream($content, $node_field, $field_info);
      break;

    case 'managed_datastream':
      break;
  }
  return $content;
}

/**
 * Create the initial XML document.
 *
 * @param stdObject $field_info
 *   The field info from islandora_sync_fields.
 *
 * @return string
 *   The XML content.
 */
function islandora_sync_init_xml_datastream($field_info) {
  $ds_info = db_query('SELECT * FROM {islandora_sync_xml_ds} WHERE ds_id = :ds_id',
    array('ds_id' => $field_info->dsid)
  )->fetchAll();
  $namespaces = db_query('SELECT * FROM {islandora_sync_xml_ds_namespaces} WHERE ds_id = :ds_id',
    array('ds_id' => $field_info->dsid)
  )->fetchAll();

  $doc = new DOMDocument();
  $root = $doc->createElement($ds_info[0]->root);
  if ($ds_info[0]->namespace_uri) {
    $root->setAttribute('xmlns', $ds_info[0]->namespace_uri);
  }

  foreach ($namespaces as $namespace) {
    $root->setAttribute('xmlns:' . $namespace->prefix, $namespace->uri);
  }

  $doc->appendChild($root);
  // @TODO: pass around DOMDocument rather than string
  return $doc->saveXML();
}

/**
 * Populate an XML document with elements.
 *
 * @param string $content
 *   The XML content as string.
 * @param string $node_field_value
 *   The field value.
 * @param stdObject $field_info
 *   Field info from islandora_sync_fields.
 *
 * @return string
 *   The XML content.
 */
function islandora_sync_build_xml_datastream($content, $node_field_value, $field_info) {
  $xml_doc = new DOMDocument();
  $xml_doc->loadXML($content);
  switch ($field_info->create_type) {
    case 'element':
      $dom_xpath = new DOMXPath($xml_doc);
      $path_part = explode('/', $field_info->create_value);
      $current_path = '';
      $part_count = count($path_part);
      $parent = NULL;
      for ($i = 0; $i < $part_count; $i++) {
        $path = $path_part[$i];
        $value = NULL;
        if (empty($current_path)) {
          // @TODO: check if XML root has been provided if document context.
          $current_path .= '/';
        }
        if ($i == ($part_count - 1)) {
          $value = $node_field_value;
        }

        $current_path = $current_path . $path;
        $xpath = $dom_xpath->query($current_path);
        if (!$xpath->length) {
          // Need to create.
          $namespaced = explode(':', $path);
          if (count($namespaced) > 1) {
            $ns_uri = _get_ns_uri($namespaced[0]);
            $new_element = $xml_doc->createElementNS($ns_uri, $path, $value);
          }
          else {
            $new_element = $xml_doc->createElement($path, $value);
          }
          if ($i > 0 && !is_null($parent)) {
            // @TODO: parent vs doc.
            $parent->appendChild($new_element);
          }
          else {
            $xml_doc->documentElement->appendChild($new_element);
          }
          $parent = $new_element;
        }
      }
      break;

    case 'attribute':
      break;

    case 'xml':
      $value = $field_info->create_value;
      $value = str_replace('%value%', $node_field_value, $value);
      $fragment = $xml_doc->createDocumentFragment();
      $fragment->appendXML($value);
      $xml_doc->documentElement->appendChild($fragment);
      break;
  }
  $content = $xml_doc->saveXML();
  return $content;
}

/**
 * Get the uri for a namespace.
 *
 * @param string $namespace
 *   The namespace to lookup.
 *
 * @return string
 *   The namespace URI.
 */
function _get_ns_uri($namespace) {
  $ns = db_query("SELECT uri FROM {islandora_sync_xml_ds_namespaces} WHERE prefix = :prefix",
    array(':prefix' => $namespace)
  )->fetchAll();
  return $ns[0]->uri;
}
