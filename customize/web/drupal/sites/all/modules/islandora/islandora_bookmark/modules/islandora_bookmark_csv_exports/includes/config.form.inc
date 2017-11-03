<?php
/**
 * @file
 * Holds the form for configuring an individual export.
 */

/**
 * Constructs the configuration form for a Bookmark CSV export.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param string $configuration_id
 *   The ID of the configuration being modified.
 *
 * @return array
 *   An array to be rendered.
 */
function islandora_bookmark_csv_exports_config_form($form, &$form_state, $configuration_id) {
  form_load_include($form_state, 'inc', 'islandora_bookmark_csv_exports', 'includes/config.form');
  module_load_include('inc', 'islandora_bookmark_csv_exports', 'includes/db');
  $db_values = islandora_bookmark_csv_exports_get_values($configuration_id);
  $default_fields = !empty($db_values['config_fields']) ? implode($db_values['config_fields'], ', ') : '';
  $fields = isset($form_state['input']['fields']) ? $form_state['input']['fields'] : $default_fields;

  if (isset($form_state['triggering_element'])) {
    if ($form_state['triggering_element']['#name'] == 'islandora-bookmark-csv-exports-solr-add' && !empty($form_state['values']['solr_autocomplete'])) {
      if (!empty($fields)) {
        $fields = $fields . ', ' . $form_state['input']['solr_autocomplete'];
      }
      else {
        $fields = $form_state['input']['solr_autocomplete'];
      }
    }
  }
  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#collapsed' => TRUE,
    '#collapsible' => FALSE,
  );
  $default_delimiter = !empty($db_values['delimiter']) ? $db_values['delimiter'] : '\n';
  $form['configuration']['delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('Multi-valued delimiter'),
    '#required' => TRUE,
    '#description' => t('This will be the delimiter value used as a glue in between multi valued fields pulled from Solr. Defaults to "\n"'),
    '#default_value' => $default_delimiter,
  );
  $form['configuration']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('A description used to describe the CSV export, used in the UI.'),
    '#default_value' => $db_values['descrip'],
    '#required' => TRUE,
  );
  $form['configuration']['fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Fields to be exported'),
    '#description' => t('The order in which the fields appear separated by spaces or commas will dictate the write order in the CSV.'),
    '#value' => $fields,
    '#prefix' => '<div id="islandora-bookmark-csv-exports-solr-wrapper">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  $form['configuration']['solr_autocomplete'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field'),
    '#description' => t('This will be used to pull content when generating the CSV for export.'),
    '#size' => 105,
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#default_value' => '',
  );
  $form['configuration']['solr_add'] = array(
    '#type' => 'button',
    '#value' => t('Add'),
    '#ajax' => array(
      'callback' => 'islandora_bookmark_csv_exports_solr_ajax',
      'wrapper' => 'islandora-bookmark-csv-exports-solr-wrapper',
    ),
    '#name' => 'islandora-bookmark-csv-exports-solr-add',
  );
  $form['configuration_id'] = array(
    '#type' => 'value',
    '#value' => $configuration_id,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('islandora_bookmark_csv_exports_config_form_submit'),
    '#limit_validation_errors' => array(array('configuration_id')),
  );
  return $form;
}

/**
 * Validation for the configuration form.
 */
function islandora_bookmark_csv_exports_config_form_validate($form, $form_state) {
  module_load_include('inc', 'islandora_csv_exports', 'includes/db');
  if ($form_state['triggering_element']['#parents'] == array('save')) {
    // See if the description is unique.
    if (!empty($form_state['values']['description'])) {
      if (islandora_bookmark_csv_exports_description_exists($form_state['values']['description'], $form_state['values']['configuration_id'])) {
        form_error($form['configuration']['description'], t('A configuration with the description of @description already exists.', array('@description' => $form_state['values']['description'])));
      }
    }
    if (empty($form_state['input']['fields'])) {
      form_error($form['configuration']['fields'], t('Fields field is required.'));
    }
  }
}

/**
 * Submit handler for the configuration form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_bookmark_csv_exports_config_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_csv_exports', 'includes/db');
  if ($form_state['triggering_element']['#parents'] == array('save')) {
    // Munge the textarea into a form we can expect.
    $fields = array_filter(preg_split('/[,\s]/', $form_state['input']['fields']));
    islandora_bookmark_csv_exports_add_values($form_state['values']['configuration_id'], array(
      'description' => $form_state['values']['description'],
      'fields' => serialize($fields),
      'delimiter' => $form_state['values']['delimiter'],
    ));
    drupal_set_message(t('Configuration saved.'));
  }
  else {
    islandora_bookmark_csv_exports_delete_configuration($form_state['values']['configuration_id']);
    drupal_set_message(t('Configuration deleted.'));
    $form_state['redirect'] = 'admin/islandora/tools/islandora-bookmark/csv_exports';
  }
}

/**
 * AJAX callback for Solr field adding.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   The portion of the form to be rendered.
 */
function islandora_bookmark_csv_exports_solr_ajax(&$form, &$form_state) {
  return $form['configuration']['fields'];
}
