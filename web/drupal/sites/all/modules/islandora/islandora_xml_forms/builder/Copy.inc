<?php

/**
 * @file
 * Callbacks and functions used in the Copy Form Page.
 */

/**
 * Get the Copy Page Form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal Form State.
 * @param string $form_name
 *   The name of the form to copy.
 *
 * @return array
 *   The Drupal Form.
 */
function xml_form_builder_copy($form, array &$form_state, $form_name) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormRepository');
  if (isset($_POST['cancel'])) {
    drupal_goto(XML_FORM_BUILDER_MAIN_MENU);
  }
  if (!XMLFormRepository::Exists($form_name)) {
    drupal_set_message(t('Form "%name" does not exist.', array('%name' => $form_name)), 'error');
    drupal_not_found();
    exit();
  }
  return array(
    'original' => array(
      '#type' => 'hidden',
      '#value' => $form_name,
    ),
    'form_name' => array(
      '#type' => 'textfield',
      '#title' => t('Form Name'),
      '#required' => TRUE,
      '#element_validate' => array('xml_form_builder_copy_validate_name'),
    ),
    'copy' => array(
      '#type' => 'submit',
      '#value' => t('Copy'),
      '#name' => 'copy',
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#name' => 'cancel',
    ),
  );
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
function xml_form_builder_copy_validate_name(array $element, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormRepository');
  $form_name = &$form_state['values']['form_name'];
  if (XMLFormRepository::Exists($form_name)) {
    $error_msg = t('The given form name "%name" is already in use. Form names must be unique.', array('%name' => $form_name));
    form_set_error('form_name', $error_msg);
  }
}

/**
 * Submit the Copy form. Copies a existing Form definition within the database.
 *
 * If the form definition was successfully copied, redirect the user to the edit
 * form. If not, or if the cancel button was clicked, return the user to the
 * main menu.
 *
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function xml_form_builder_copy_submit(array $form, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormRepository');
  if ($form_state['clicked_button']['#name'] == 'copy') {
    $original = $form_state['values']['original'];
    $form_name = $form_state['values']['form_name'];
    if (XMLFormRepository::Copy($original, $form_name)) {
      drupal_set_message(t('Successfully copied form "%name".', array('%name' => $form_name)));
      $form_state['redirect'] = xml_form_builder_get_edit_form_path($form_name);
      return;
    }
    drupal_set_message(t('Failed to copy form "%name".', array('%name' => $form_name)), 'error');
  }
  $form_state['redirect'] = XML_FORM_BUILDER_MAIN_MENU;
}
