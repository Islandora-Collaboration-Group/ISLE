<?php

/**
 * @file
 * Module used to create the PDF upload step for Scholar.
 */

/**
 * Gets the upload form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_scholar_pdf_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));

  // Pull document versions from the module settings if available.
  $document_versions = get_document_versions();

  // Pull use permissions from the module settings if available.
  $use_permissions = get_use_permissions();

  $form['upload_pdf_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Would you like to include a PDF document for this citation?'),
  );
  $form['upload_document'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload Document'),
    '#states' => array(
      'visible' => array(
        ':input[name="upload_pdf_checkbox"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['upload_document']['file'] = array(
    '#type' => 'managed_file',
    '#title' => 'PDF file',
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('pdf'),
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['upload_document']['version'] = array(
    '#type' => 'radios',
    '#title' => t('Document Version'),
    '#options' => $document_versions,
  );
  $form['upload_document']['usage'] = array(
    '#type' => 'radios',
    '#title' => t('Use Permission'),
    '#options' => $use_permissions,
  );
  $form['upload_document']['certifying'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Certify'),
    '#options' => array(
      'certify' => t('I certify that I have the right to upload this version.'),
    ),
  );
  return $form;
}

/**
 * Validation for the Islandora Scholar PDF upload form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_scholar_pdf_upload_form_validate($form, $form_state) {
  if ($form_state['values']['upload_pdf_checkbox']) {
    if (empty($form_state['values']['file'])) {
      form_set_error('file', t('A file must be uploaded!'));
    }
    if (empty($form_state['values']['version'])) {
      form_set_error('version', t('A document version must be selected!'));
    }
    if (empty($form_state['values']['usage'])) {
      form_set_error('usage', t('A usage permission must be selected!'));
    }
    if (empty($form_state['values']['certifying']['certify'])) {
      form_set_error('certifying', t('You must certify that you have the right to upload this PDF!'));
    }
  }
}

/**
 * Upload form Submit handler, adds PDF datastream.
 *
 * Also updates the MODS usage and version for the PDF datastream.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_scholar_pdf_upload_form_submit(array $form, array &$form_state) {
  if ($form_state['values']['upload_pdf_checkbox']) {
    $object = islandora_ingest_form_get_object($form_state);
    module_load_include('inc', 'islandora_scholar', 'includes/upload.tab');
    $file = file_load($form_state['values']['file']);
    $datastream = $object->constructDatastream('PDF', 'M');
    $datastream->label = $file->filename;
    $datastream->mimetype = 'application/pdf';
    $datastream->setContentFromFile($file->uri, FALSE);
    $object->ingestDatastream($datastream);
    islandora_scholar_add_usage_and_version_elements_to_mods($object, $form_state['values']['usage'], $form_state['values']['version']);
  }
}

/**
 * Undo submit handler for the PDF upload ingest step for Scholar.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_scholar_pdf_upload_form_undo_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);
  $object->purgeDatastream('PDF');
}
