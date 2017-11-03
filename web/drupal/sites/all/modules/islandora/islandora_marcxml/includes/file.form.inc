<?php

/**
 * @file
 * Upload MARCXML file.
 */

/**
 * Upload MARCXML form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   A Drupal form definition.
 */
function islandora_marcxml_file_form(array $form, array &$form_state) {
  return array(
    'file' => array(
      '#type' => 'managed_file',
      '#title' => t('MARCXML File'),
      '#description' => t('A file containing a MARCXML record, to be transformed to MODS. Click "Next" to skip this step and create a record from scratch.'),
      '#upload_validators' => array(
        'file_validate_extensions' => array('xml'),
      ),
    ),
  );
}

/**
 * Populate the MODS datastream with the transformed MARCXML.
 *
 * Only populates the MODS datastream if a file was uploaded.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_marcxml_file_form_submit(array &$form, array &$form_state) {
  module_load_include('inc', 'islandora_marcxml', 'includes/utilities');
  $file = file_load($form_state['values']['file']);
  if ($file) {
    $marcxml_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_marcxml_upload');
    $object = islandora_ingest_form_get_object($form_state);
    $marcxml_step_storage['created']['MODS'] = islandora_marcxml_populate_mods($object, $file);
    // To ensure the file is only used once, delete it.
    file_delete($file);
    unset($form_state['values']['file']);
  }
}

/**
 * Undo's any changes the regular submit handler did.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_marcxml_file_form_undo_submit(array &$form, array &$form_state) {
  $marcxml_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_marcxml_upload');
  $object = islandora_ingest_form_get_object($form_state);
  if (isset($marcxml_step_storage['created']['MODS']) && $marcxml_step_storage['created']['MODS']) {
    $object->purgeDatastream('MODS');
  }
  unset($marcxml_step_storage['created']);
}
