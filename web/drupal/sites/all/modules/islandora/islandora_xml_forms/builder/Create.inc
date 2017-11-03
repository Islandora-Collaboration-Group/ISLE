<?php

/**
 * @file
 * Callbacks and functions used in the Create Form Page.
 */

/**
 * Get the Create Page Form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal Form State.
 * @param string $type
 *   The form type.
 *
 * @return array
 *   The Drupal Form.
 */
function xml_form_builder_create($form, array &$form_state, $type) {
  if (isset($_POST['cancel'])) {
    drupal_goto(XML_FORM_BUILDER_MAIN_MENU);
  }
  $return = array();

  $return['#attributes'] = array(
    'enctype' => "multipart/form-data",
  );

  $return['form_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Form Name'),
    '#required' => TRUE,
    '#maxlength' => 128,
    '#element_validate' => array('xml_form_builder_create_validate_name'),
  );

  if ($type == 'import') {
    $return['file'] = array(
      '#type' => 'file',
      '#title' => t('Form Definition'),
      '#size' => 64,
      '#description' => t('An optional XML form definition template.'),
    );
  }

  $return['create'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
    '#name' => 'create',
  );

  $return['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'cancel',
  );

  return $return;
}

/**
 * Validates the name Drupal Form Element.
 *
 * Ensures that the name doesn't match any existing form names.
 *
 * @param array $element
 *   The name Drupal Form Element.
 * @param array $form_state
 *   The Drupal Form State.
 */
function xml_form_builder_create_validate_name(array $element, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormRepository');
  $form_name = &$form_state['values']['form_name'];
  if (XMLFormRepository::Exists($form_name)) {
    $error_msg = t('The given form name "%name" is already in use. Form names must be unique.', array('%name' => $form_name));
    form_set_error('form_name', $error_msg);
  }
}

/**
 * Validate the create form.
 *
 * Makes sure the uploaded file is valid.
 *
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 *
 * @throws Exception
 *   If unable to load the uploaded file as XML, or if the form definition is
 *   invalid.
 */
function xml_form_builder_create_validate(array $form, array &$form_state) {
  $file_uploaded = isset($_FILES['files']['error']['file']) && ($_FILES['files']['error']['file'] == 0);
  if ($file_uploaded) {
    module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
    $filename = $_FILES['files']['tmp_name']['file'];
    $definition = new DOMDocument();
    try {
      $definition->load($filename);
    }
    catch (Exception $e) {
      form_set_error('files', t("Could not load uploaded file as XML, with error: %error.", array('%error' => $e->getMessage())));
    }
    try {
      $version = XMLFormDefinition::getVersion($definition);
      if (!XMLFormDefinition::isValid($definition, $version)) {
        form_set_error('files', t('The given form definition is not valid.'));
      }
    }
    catch (Exception $e) {
      form_set_error('files', $e->getMessage());
    }
  }
}

/**
 * Submit the Create form. Creates a new Form definition in the database.
 *
 * If the form definition was successfully created, redirect the user to the
 * edit form. If not, or if the cancel button was clicked, return the user to
 * the main menu.
 *
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function xml_form_builder_create_submit(array $form, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormRepository');
  $form_name = $form_state['values']['form_name'];
  if ($form_state['clicked_button']['#name'] == 'create') {
    $definition = xml_form_builder_create_get_uploaded_file();
    $definition = $definition ? $definition : xml_form_api_get_empty_form_definition();
    if (XMLFormRepository::Create($form_name, $definition)) {
      drupal_set_message(t('Successfully created form "%name".', array('%name' => $form_name)));
      $form_state['redirect'] = xml_form_builder_get_edit_form_path($form_name);
      return;
    }
    else {
      drupal_set_message(t('Failed to create form %name.', array('%name' => $form_name)), 'error');
    }
  }
  $form_state['redirect'] = XML_FORM_BUILDER_MAIN_MENU;
}

/**
 * Gets the uploaded file from the create form.
 *
 * @return DOMDocument
 *   The uploaded file if it exists, FALSE otherwise.
 */
function xml_form_builder_create_get_uploaded_file() {
  $file_uploaded = isset($_FILES['files']['error']['file']) && ($_FILES['files']['error']['file'] == 0);
  if ($file_uploaded) {
    $definition = new DOMDocument();
    $definition->load($_FILES['files']['tmp_name']['file']);
    return $definition;
  }
  return FALSE;
}
