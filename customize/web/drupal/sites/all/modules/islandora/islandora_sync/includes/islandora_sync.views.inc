<?php

/**
 * @file
 * Expose Islandora Sync database tables to views.
 */

/**
 * Implements hook_views_data().
 */
function islandora_sync_views_data() {
  $data = array();
  $data['islandora_sync_map']['table']['group'] = t('Islandora');
  $data['islandora_sync_map']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'left_field' => 'nid',
      'field' => 'entity_id',
    ),
  );
  $data['islandora_sync_map']['entity_id'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID of the node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['islandora_sync_map']['pid'] = array(
    'title' => t('Fedora Object ID'),
    'help' => t('The Fedora Commons object PID.'),
    'field' => array(
      'handler' => 'views_handler_field_file',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  return $data;
}
