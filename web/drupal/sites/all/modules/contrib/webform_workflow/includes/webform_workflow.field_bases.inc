<?php
/**
 * @file
 * Default field bases for the Webform Workflow module.
 */

/**
 * Get default field bases.
 *
 * @return array
 *   An array of field base definitions.
 */
function _webform_workflow_field_bases() {
  $field_bases = array();

  // Exported field_base: 'webform_workflow_states'
  $field_bases['webform_workflow_states'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(
      0 => 'node',
    ),
    'field_name' => 'webform_workflow_states',
    'foreign keys' => array(
      'webform_workflow_state' => array(
        'columns' => array(
          'target_id' => 'wsid',
        ),
        'table' => 'webform_workflow_state',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'direction' => 'ASC',
          'property' => 'label',
          'type' => 'property',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'webform_workflow_state',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'ww_state_notify_os'
  $field_bases['ww_state_notify_os'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array('webform_workflow_state'),
    'field_name' => 'ww_state_notify_os',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'No',
        1 => 'Yes',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'ww_state_notify_users'
  $field_bases['ww_state_notify_users'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array('webform_workflow_state'),
    'field_name' => 'ww_state_notify_users',
    'foreign keys' => array(
      'users' => array(
        'columns' => array(
          'target_id' => 'uid',
        ),
        'table' => 'users',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'user',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}
