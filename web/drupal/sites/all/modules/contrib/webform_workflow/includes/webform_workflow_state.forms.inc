<?php
/**
 * @file
 * State-related forms.
 */

/**
 * Form for editing or adding a state.
 */
function webform_workflow_state_form($form, &$form_state, $state = NULL) {

  if (empty($state)) {
    $state = entity_create('webform_workflow_state', array());
  }

  form_load_include($form_state, 'inc', 'webform_workflow', 'includes/webform_workflow_state.forms');

  field_attach_form('webform_workflow_state', $state, $form, $form_state);

  webform_workflow_state_form_attach($form, $form_state, $state);

  $form['buttons'] = array('#type' => 'actions');
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save state'),
  );

  return $form;
}

/**
 * Attach form elements for a workflow state form.
 */
function webform_workflow_state_form_attach(&$form, &$form_state, $state) {
  $form_state['state'] = $state;

  if (!$state->wsid && !isset($state->uid)) {
    $state->uid = $GLOBALS['user']->uid;
  }

  $ops = array(
    'view' => t('View submission'),
    'edit' => t('Edit submission'),
    'from' => t('Change from this state'),
    'to' => t('Change to this state'),
  );

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $state->label,
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['color'] = array(
    '#type' => 'select',
    '#title' => t('Color'),
    '#default_value' => $state->color,
    '#options' => webform_workflow_state_color_options_list(),
    '#attributes' => array('class' => array('webform-workflow-state-color-list')),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'webform_workflow') . '/includes/webform_workflow.js',
      ),
    ),
  );

  $form['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Permissions'),
    '#description' => t('Use permissions to grant additional access permissions to the specified user roles. Note: you can grant additional access using these permissions, but you cannot deny access that would otherwise be granted.'),
    '#tree' => TRUE,
    '#attributes' => array('class' => array('webform-workflow-state-permissions')),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $drupal_roles = user_roles(TRUE);
  $drupal_roles[WEBFORM_WORKFLOW_ORIGINAL_SUBMITTER] = 'original submitter';
  $role_checkboxes = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $drupal_roles,
  );

  foreach ($ops as $op => $label) {
    $form['permissions'][$op] = $role_checkboxes + array(
      '#title' => $label,
      '#default_value' => $state->permissions[$op],
    );
  }
}

/**
 * Submit callback for the webform workflow state form.
 */
function webform_workflow_state_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $state = $form_state['state'];
  $instances = field_info_instances('webform_workflow_state', 'webform_workflow_state');

  $values_excluding_fields = array_diff_key($values, $instances);
  foreach ($values_excluding_fields as $property => $value) {
    $state->$property = $value;
  }

  if ($instances) {
    field_attach_submit('webform_workflow_state', $state, $form, $form_state);
  }
  entity_get_controller('webform_workflow_state')->save($state);
}
