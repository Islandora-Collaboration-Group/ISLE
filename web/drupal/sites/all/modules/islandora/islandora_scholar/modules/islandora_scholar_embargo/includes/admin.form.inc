<?php

/**
 * @file
 * This file holds supporting admin functions for the embargo module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_embargo_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $options = islandora_get_content_models();
  $selected = variable_get('islandora_embargo_content_models', array('ir:citationCModel', 'ir:thesisCModel'));
  foreach ($selected as $cmodel) {
    if (isset($options[$cmodel])) {
      $options = array($cmodel => $options[$cmodel]) + $options;
    }
  }
  foreach ($options as $key => $value) {
    $rows[$key] = array(
      'pid' => $key,
      'title' => $value['label'],
    );
    in_array($key, $selected) ? $defaults[$key] = TRUE : $defaults[$key] = FALSE;
  }
  $header = array(
    'pid' => array('data' => t('PID')),
    'title' => array('data' => t('Content Model')),
  );
  $form = array();
  $form['email'] = array(
    '#type' => 'checkbox',
    '#title' => 'Suppress default email notifications for embargoes',
    '#description' => 'Check if you have custom embargo notifications and would like to stop the default email notifications from sending.',
    '#default_value' => variable_get('islandora_scholar_embargo_suppress_default_emails', FALSE),
  );
  $form['intro'] = array(
    '#type' => 'item',
    '#title' => 'Select Content Models for objects to be embargoed',
    '#description' => t('Objects defined by selected Content Models will be eligible for embargoing.'),
  );
  $form['the_table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#default_value' => $defaults,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['configure'] = array(
    '#type' => 'submit',
    '#value' => t('Configure Embargo'),
    '#weight' => 1,
    '#submit' => array('islandora_embargo_admin_submit'),
  );

  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to default'),
    '#weight' => 2,
    '#submit' => array('islandora_embargo_admin_submit'),
  );
  return $form;
}

/**
 * Submit handler reset all system settings.
 */
function islandora_embargo_admin_submit(array $form, array &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case 'Reset to default':
      variable_del('islandora_embargo_content_models');
      variable_del('islandora_scholar_embargo_suppress_default_emails');
      break;

    default:
      $enabled = array_filter($form_state['values']['the_table']);
      variable_set('islandora_embargo_content_models', $enabled);
      variable_set('islandora_scholar_embargo_suppress_default_emails', $form_state['values']['email']);
  }
}
