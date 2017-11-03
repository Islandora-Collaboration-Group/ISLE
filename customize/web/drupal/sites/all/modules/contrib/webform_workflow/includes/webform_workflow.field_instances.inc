<?php
/**
 * @file
 * Default field instances for the Webform Workflow module.
 */

/**
 * Get default field instances for nodes.
 *
 * @return array
 *   An array of field instance definitions.
 */
function _webform_workflow_node_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-webform-webform_workflow_states'
  $field_instances['node-webform-webform_workflow_states'] = array(
    'bundle' => 'webform',
    'deleted' => 0,
    'description' => 'Configure the workflow states that can be assigned to this form\'s submissions.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'webform_workflow_states',
    'label' => 'Submission states',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'inline_entity_form',
      'settings' => array(
        'fields' => array(),
        'type_settings' => array(
          'allow_existing' => 1,
          'delete_references' => 1,
          'label_plural' => 'states',
          'label_singular' => 'state',
          'match_operator' => 'CONTAINS',
          'override_labels' => 1,
        ),
      ),
      'type' => 'inline_entity_form',
      'weight' => -4,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Configure the workflow states that can be assigned to this form\'s submissions.');
  t('Submission states');

  return $field_instances;
}

/**
 * Get default field instances for workflow states.
 *
 * @return array
 *   An array of field instance definitions.
 */
function _webform_workflow_state_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'webform_workflow_state-webform_workflow_state-ww_state_notify_os'
  $field_instances['webform_workflow_state-webform_workflow_state-ww_state_notify_os'] = array(
    'bundle' => 'webform_workflow_state',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Notify the original submitter when a submission is changed to this state.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'webform_workflow_state',
    'field_name' => 'ww_state_notify_os',
    'label' => 'Notify the original submitter',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'webform_workflow_state-webform_workflow_state-ww_state_notify_users'
  $field_instances['webform_workflow_state-webform_workflow_state-ww_state_notify_users'] = array(
    'bundle' => 'webform_workflow_state',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Specify which users should be notified when a submission is changed to this state.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'webform_workflow_state',
    'field_name' => 'ww_state_notify_users',
    'label' => 'Users to notify',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Notify the original submitter');
  t('Notify the original submitter when a submission is changed to this state.');
  t('Specify which users should be notified when a submission is changed to this state.');
  t('Users to notify');

  return $field_instances;
}
