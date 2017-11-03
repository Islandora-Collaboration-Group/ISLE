<?php

/**
 * @file
 * Admin form and associated functions.
 */

/**
 * Form building functions; allow addition and removal of XSLT populators.
 */
function islandora_xslt_populator_admin_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'islandora_xslt_populator', 'includes/db');
  form_load_include($form_state, 'inc', 'islandora_xslt_populator', 'includes/admin.form');
  $populators = islandora_xslt_populator_get_populators();
  foreach ($populators as &$populator) {
    if (is_null($populator['original_dsid'])) {
      $populator['original_dsid'] = t('Not storing original.');
    }
    $populator['description'] = check_plain($populator['description']);
    $populator['xslt'] = theme('file_link', array(
      'file' => file_load($populator['xslt']),
    ));
  }
  $form['#tree'] = TRUE;
  $form['populators'] = array(
    '#type' => 'vertical_tabs',
  );
  $destination = 'public://islandora_xslt_populator_xslts';
  file_prepare_directory($destination);
  $form += array(
    'existing' => array(
      '#type' => 'fieldset',
      '#group' => 'populators',
      '#title' => t('Existing XSLT Populators'),
      'table' => array(
        '#type' => 'tableselect',
        '#header' => array(
          'title' => t('Title'),
          'description' => t('Description'),
          'xslt' => t('XSLT File'),
          'datastream_id' => t('Generated Datastream ID'),
          'original_dsid' => t('Stored Datastream ID'),
          'machine_name' => t('Machine Name'),
        ),
        '#options' => $populators,
        '#empty' => t('None configured.'),
      ),
      'remove' => array(
        '#type' => 'submit',
        '#value' => t('Remove Selected'),
        '#limit_validation_errors' => array(
          array('existing'),
        ),
        '#validate' => array(
          'islandora_xslt_populator_admin_form_remove_validate',
        ),
        '#submit' => array(
          'islandora_xslt_populator_admin_form_remove_submit',
        ),
      ),
    ),
    'new' => array(
      '#type' => 'fieldset',
      '#title' => t('Add New XSLT Populator'),
      '#group' => 'populators',
      'title' => array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#required' => TRUE,
      ),
      'description' => array(
        '#type' => 'textarea',
        '#title' => t('Description'),
        '#required' => TRUE,
      ),
      'xslt' => array(
        '#type' => 'managed_file',
        '#title' => t('XSLT File'),
        '#required' => TRUE,
        '#islandora_plupload_do_not_alter' => TRUE,
        '#upload_location' => $destination,
        '#upload_validators' => array(
          'file_validate_extensions' => array('xsl xslt xml'),
          'islandora_xslt_populator_file_appears_to_be_xslt' => array(),
        ),
      ),
      'datastream_id' => array(
        '#type' => 'textfield',
        '#title' => t('Datastream ID'),
        '#description' => t('The datastream identifier into which the result of the XSLT will be stored.'),
        '#required' => TRUE,
      ),
      'machine_name' => array(
        '#type' => 'textfield',
        '#title' => t('Machine Name'),
        '#description' => t('A machine name for use in features. If not provided, the relevent configuration will not be available for export in features.'),
        '#required' => FALSE,
      ),
      'store_original' => array(
        '#type' => 'checkbox',
        '#title' => t('Store copy of original file'),
        '#description' => t('Creates a datastream to store a copy of the original XML file uploaded.'),
      ),
      'original_dsid' => array(
        '#type' => 'textfield',
        '#title' => t('Original Datastream ID'),
        '#description' => t('The datastream identifier into which the original file will be stored.'),
        '#states' => array(
          'visible' => array(
            ':input[name="new[store_original]"]' => array(
              'checked' => TRUE,
            ),
          ),
        ),
      ),
      'add' => array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#limit_validation_errors' => array(
          array('new'),
        ),
        '#validate' => array(
          'islandora_xslt_populator_admin_form_add_validate',
        ),
        '#submit' => array(
          'islandora_xslt_populator_admin_form_add_submit',
        ),
      ),
    ),
  );
  return $form;
}

/**
 * Form validation handler; ensure a populator has been selected.
 */
function islandora_xslt_populator_admin_form_remove_validate(&$form, &$form_state) {
  $selected = array_filter($form_state['values']['existing']['table']);
  if (empty($selected)) {
    form_error($form['existing']['table'], t('No populator was selected to remove.'));
  }
}

/**
 * Form submission handler; remove selected populators.
 */
function islandora_xslt_populator_admin_form_remove_submit(&$form, &$form_state) {
  islandora_xslt_populator_remove_populator(array_keys(array_filter($form_state['values']['existing']['table'])));
}

/**
 * Form validation handler; ensure the given datastream ID is valid.
 */
function islandora_xslt_populator_admin_form_add_validate(&$form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!islandora_is_valid_dsid($form_state['values']['new']['datastream_id'])) {
    form_error($form['new']['datastream_id'], t('The given datastream ID does not appear to be valid.'));
  }
  if ($form_state['values']['new']['store_original'] && (empty($form_state['values']['new']['original_dsid']) || !islandora_is_valid_dsid($form_state['values']['new']['original_dsid']))) {
    form_error($form['new']['original_dsid'], t('The given datastream ID does not appear to be valid.'));
  }
}

/**
 * Form submission handler; add the populator.
 */
function islandora_xslt_populator_admin_form_add_submit(&$form, &$form_state) {
  islandora_xslt_populator_add_populator(
    $form_state['values']['new']['title'],
    $form_state['values']['new']['description'],
    $form_state['values']['new']['xslt'],
    $form_state['values']['new']['datastream_id'],
    $form_state['values']['new']['machine_name'],
    $form_state['values']['new']['store_original'] ? $form_state['values']['new']['original_dsid'] : NULL
  );
}

/**
 * Test if the contents of a file appears to be an XSLT.
 *
 * @param object $file
 *   A Drupal file-object.
 *
 * @return array
 *   An array of error messages; if empty, the file should be good.
 */
function islandora_xslt_populator_file_appears_to_be_xslt($file) {
  $doc = new DOMDocument();
  if (!@$doc->load(drupal_realpath($file->uri))) {
    return array(
      t('The file could not be parsed as XML.'),
    );
  }

  $element = $doc->documentElement;
  if (!($element->namespaceURI == 'http://www.w3.org/1999/XSL/Transform' &&
    $element->localName == 'stylesheet' &&
    $element->getAttribute('version') == '1.0')) {
    return array(
      t('The file does not appear to be a valid version 1.0 XSLT.'),
    );
  }

  return array();
}
