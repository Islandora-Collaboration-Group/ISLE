<?php

/**
 * @file
 * Callbacks and functions used in the Preview Page.
 */

/**
 * Render the preview form.
 *
 * This shows the user what their form will look like. When it is submitted it
 * will show the user what the generated XML looks like.
 *
 * @param array $form
 *   Any wrapping form elements.
 * @param array $form_state
 *   The Drupal Form State.
 * @param string $form_name
 *   The name of the form to preview.
 *
 * @return array
 *   A Drupal Form that represents the given form, identified by $form_name.
 */
function xml_form_builder_preview(array $form, array &$form_state, $form_name) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'Preview');
  $form = xml_form_builder_get_form($form, $form_state, $form_name);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Submit the preview form.
 *
 * Render the generated metadata document.
 *
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function xml_form_builder_preview_submit(array $form, array &$form_state) {
  try {
    $xml_form = new XMLForm($form_state);
    $document = $xml_form->submit($form, $form_state);
    dom_document_pretty_print($document->document);
    exit();
  }
  catch (Exception $e) {
    $message = 'File: ' . $e->getFile() . '</br>';
    $message .= 'Line: ' . $e->getLine() . '</br>';
    $message .= 'Error: ' . $e->getMessage() . '</br>';
    drupal_set_message(filter_xss($message), 'error');
    $form_state['rebuild'] = TRUE;
  }
}
