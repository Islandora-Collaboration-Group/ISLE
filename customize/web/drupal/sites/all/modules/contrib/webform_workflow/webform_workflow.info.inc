<?php
/**
 * @file
 * Entity property information for Webform Workflow States.
 */

/**
 * Implements hook_entity_property_info().
 */
function webform_workflow_entity_property_info() {
  $info = array();
  $info['webform_workflow_state']['properties'] = array(
    'wsid' => array(
      'label' => t('State ID'),
      'description' => t('Primary key'),
      'type' => 'integer',
      'schema field' => 'wsid',
      'required' => TRUE,
    ),
    'uid' => array(
      'label' => t('Owner'),
      'description' => t('The user who created or who owns the state'),
      'type' => 'user',
      'schema field' => 'uid',
      'required' => TRUE,
    ),
    'label' => array(
      'label' => t('Label'),
      'description' => t('The name of the state'),
      'type' => 'text',
      'schema field' => 'label',
      'required' => TRUE,
    ),
    'color' => array(
      'label' => t('Color'),
      'description' => t('The state color'),
      'type' => 'text',
      'schema field' => 'color',
      'required' => FALSE,
      'options list' => 'webform_workflow_state_color_options_list',
    ),
    'permissions' => array(
      'label' => t('Permissions'),
      'description' => t('Permissions for the state'),
      'schema field' => 'permissions',
      'required' => FALSE,
      'type' => 'struct',
      'property info' => array(
        'view' => array(
          'label' => t('View'),
          'description' => t('Roles with permission to view the submission in this state'),
          'type' => 'list<role>',
        ),
        'edit' => array(
          'label' => t('Edit'),
          'description' => t('Roles with permission to edit the submission in this state'),
          'type' => 'list<role>',
        ),
        'from' => array(
          'label' => t('Change from'),
          'description' => t('Roles with permission to change a submission from this state'),
          'type' => 'list<role>',
        ),
        'to' => array(
          'label' => t('Change to'),
          'description' => t('Roles with permission to change a submission to this state'),
          'type' => 'list<role>',
        ),
      ),
    ),
    'created' => array(
      'label' => t('Created date'),
      'description' => t('The date when the state was created'),
      'type' => 'date',
      'schema field' => 'created',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'changed' => array(
      'label' => t('Last modified date'),
      'description' => t('The date when the state was last updated'),
      'type' => 'date',
      'schema field' => 'changed',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
  $info['webform_workflow_submission']['properties'] = array(
    'sid' => array(
      'label' => t('Submission ID'),
      'description' => t('Primary key'),
      'type' => 'integer',
      'schema field' => 'sid',
      'required' => TRUE,
    ),
    'node' => array(
      'label' => t('Webform node'),
      'description' => t('The webform node associated with this submission'),
      'type' => 'node',
      'schema field' => 'nid',
      'required' => TRUE,
    ),
    'user' => array(
      'label' => t('Submitter'),
      'description' => t('The user who made the submission'),
      'type' => 'user',
      'schema field' => 'uid',
      'required' => TRUE,
    ),
    'serial' => array(
      'label' => t('Serial number'),
      'description' => t('The serial number of the submission'),
      'type' => 'integer',
      'schema field' => 'serial',
      'required' => TRUE,
    ),
    'submitted' => array(
      'label' => t('Submitted timestamp'),
      'description' => t('The timestamp of the submission'),
      'type' => 'date',
      'schema field' => 'submitted',
      'required' => TRUE,
    ),
    'is_draft' => array(
      'label' => t('Draft'),
      'description' => t('Whether the submission is a draft'),
      'type' => 'boolean',
      'schema field' => 'is_draft',
      'required' => TRUE,
    ),
  );
  return $info;
}
