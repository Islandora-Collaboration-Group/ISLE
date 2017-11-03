<?php
/**
 * @file
 * The main administrative form for islandora_solr_metadata.
 */

/**
 * The main administrative overview for the islandora_solr_metadata module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   An array representing the administration form.
 */
function islandora_solr_metadata_admin_form($form, &$form_state) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  $associations = islandora_solr_metadata_get_associations();
  $form = array();
  $rows = array();
  foreach ($associations as $association) {
    $cmodels = islandora_solr_metadata_get_cmodels($association['id']);
    $rows[] = array(
      l($association['name'], "admin/islandora/search/islandora_solr_metadata/config/{$association['id']}"),
      empty($cmodels) ? t('No content models currently associated') : theme('item_list', array(
        'items' => array_keys($cmodels),
      )),
      $association['machine_name'],
    );
  }
  $form['table'] = array(
    '#title' => t('Solr metadata associations'),
    '#markup' => theme('table', array(
      'header' => array(
        t('Name'),
        t('Associated content models'),
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
  $form['add_configuration']['save_content_model'] = array(
    '#type' => 'submit',
    '#value' => t('Add configuration'),
    '#name' => 'islandora_solr_metadata_add_configuration',
  );
  return $form;
}

/**
 * Validation for the islandora_solr_metadata administration form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_solr_metadata_admin_form_validate($form, $form_state) {
  if (empty($form_state['values']['configuration_name'])) {
    form_set_error('configuration_name', t('Please enter a non-empty configuration name!'));
  }
  if (empty($form_state['values']['machine_name'])) {
    form_set_error('machine_name', t('Please enter a non-empty machine name!'));
  }
  else {
    module_load_include('inc', 'islandora_solr_metadata', 'db');
    $config_exists = islandora_solr_metadata_retrieve_configuration_from_machine_name($form_state['values']['machine_name']);
    if ($config_exists !== FALSE) {
      form_set_error('machine_name', t('The machine name of @machine already exists in the database!', array('@machine' => $form_state['values']['machine_name'])));
    }
  }
}

/**
 * Submit handler for the islandora_solr_metadata administration form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_solr_metadata_admin_form_submit($form, $form_state) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  islandora_solr_metadata_add_configuration($form_state['values']['configuration_name'], $form_state['values']['machine_name']);
  drupal_set_message(t('A new empty configuration has been created for @config_name', array('@config_name' => $form_state['values']['configuration_name'])));
}
