<?php
/**
 * @file
 * Management form for the Islandora Simple Workflow module.
 */

/**
 * Builds the management form for the Islandora Simple Workflow module.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   An array containing the form to be rendered.
 */
function islandora_simple_workflow_manage_form($form, $form_state) {
  module_load_include('inc', 'islandora_simple_workflow', 'includes/utilities');

  if (isset($form_state['show_confirm'])) {
    $form['confirm_message'] = array(
      '#type' => 'item',
      '#markup' => format_plural(count($form_state['pids_to_publish']),
      'Are you sure you want to publish the 1 object?',
      'Are you sure you want to publish the @count objects?'),
    );
    $form['confirm_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
      '#weight' => 2,
      '#submit' => array('islandora_simple_workflow_manage_confirm_submit'),
    );
    $form['cancel_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#weight' => 3,
    );
  }
  else {
    $inactive_objects = islandora_simple_workflow_get_inactive_objects();
    $rows = array();
    foreach ($inactive_objects as $inactive_object) {
      $pid = $inactive_object['object']['value'];
      $rows[$pid] = array(l($inactive_object['title']['value'] . " (" . $pid . ")", "islandora/object/$pid"));
    }
    $form['management_table'] = array(
      '#type' => 'tableselect',
      '#header' => array(t('Object')),
      '#options' => $rows,
      '#attributes' => array(),
      '#empty' => t('No inactive objects were found.'),
    );
    if (!empty($rows)) {
      $form['submit_selected'] = array(
        '#type' => 'submit',
        '#name' => 'islandora-simple-workflow-submit-selected',
        '#validate' => array('islandora_simple_workflow_manage_publish_selected_validate'),
        '#submit' => array('islandora_simple_workflow_manage_publish_submit'),
        '#value' => t('Publish Selected'),
      );
      $form['submit_all'] = array(
        '#type' => 'submit',
        '#name' => 'islandora-simple-workflow-submit-all',
        '#submit' => array('islandora_simple_workflow_manage_publish_submit'),
        '#value' => t('Publish All'),
      );
    }
  }
  return $form;
}

/**
 * Validation for the Islandora Simple Workflow management form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_publish_selected_validate($form, $form_state) {
  $selected = array_filter($form_state['values']['management_table']);
  if (empty($selected)) {
    form_error($form['management_table'], t('At least one object must be selected to publish!'));
  }
}

/**
 * Submit handler for the publish buttons in the workflow management form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_publish_submit(&$form, &$form_state) {
  if ($form_state['triggering_element']['#name'] == 'islandora-simple-workflow-submit-selected') {
    $selected = array_keys(array_filter($form_state['values']['management_table']));
  }
  else {
    $selected = array_keys($form_state['values']['management_table']);
  }
  $form_state['pids_to_publish'] = $selected;
  // Rebuild to show the confirm form.
  $form_state['rebuild'] = TRUE;
  $form_state['show_confirm'] = TRUE;
}

/**
 * Submit handler for the workflow management confirm form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_confirm_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_simple_workflow', 'includes/batch');
  $batch = islandora_simple_workflow_publish_create_batch($form_state['pids_to_publish']);
  batch_set($batch);
}
