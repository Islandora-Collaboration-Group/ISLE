<?php
/**
 * @file
 * Holds the overview form for the list of configurations for CSV exporting.
 */

/**
 * Overview form which lists all the configurations present.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   The form to be rendered.
 */
function islandora_bookmark_csv_exports_overview_form($form, $form_state) {
  module_load_include('inc', 'islandora_bookmark_csv_exports', 'includes/db');
  $configurations = islandora_bookmark_csv_exports_get_configurations();
  $rows = array();
  foreach ($configurations as $config) {
    $rows[] = array(
      l($config['name'], "admin/islandora/tools/islandora-bookmark/csv_exports/config/{$config['id']}"),
      $config['descrip'],
      $config['machine_name'],
    );
  }
  $form['table'] = array(
    '#title' => t('CSV Export configurations'),
    '#markup' => theme('table', array(
      'header' => array(
        t('Name'),
        t('Description'),
        t('Machine name'),
      ),
      'rows' => $rows,
      'empty' => t('No associations currently present.'),
    )),
  );
  $form['add_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['add_configuration']['configuration_name'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Configuration name'),
  );
  $form['add_configuration']['machine_name'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Machine name'),
    '#description' => t('A unique machine name used in the exportation of features'),
  );
  $form['add_configuration']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Add configuration'),
  );
  return $form;
}

/**
 * Validation for the overview form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_bookmark_csv_exports_overview_form_validate($form, $form_state) {
  if (empty($form_state['values']['configuration_name'])) {
    form_set_error('configuration_name', t('Please enter a non-empty configuration name!'));
  }
  if (empty($form_state['values']['machine_name'])) {
    form_set_error('machine_name', t('Please enter a non-empty machine name!'));
  }
  else {
    module_load_include('inc', 'islandora_bookmark_csv_exports', 'includes/db');
    $config_exists = islandora_bookmark_csv_exports_get_configuration_from_machine_name($form_state['values']['machine_name']);
    if ($config_exists !== FALSE) {
      form_set_error('machine_name', t('The machine name of @machine already exists in the database!', array('@machine' => $form_state['values']['machine_name'])));
    }
  }
}

/**
 * Submit handler for the overview form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_bookmark_csv_exports_overview_form_submit($form, $form_state) {
  module_load_include('inc', 'islandora_bookmark_csv_exports', 'includes/db');
  islandora_bookmark_csv_exports_add_configuration($form_state['values']['configuration_name'], $form_state['values']['machine_name']);
  drupal_set_message(t('A new empty configuration has been created for @config_name', array('@config_name' => $form_state['values']['configuration_name'])));
}
