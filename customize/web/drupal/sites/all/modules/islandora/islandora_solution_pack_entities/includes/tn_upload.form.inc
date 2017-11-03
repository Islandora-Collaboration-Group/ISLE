<?php

/**
 * @file
 * Handles the uploading of the TN file as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the TN file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_entities_tn_upload_form(array $form, array &$form_state) {
  $form = islandora_entities_get_tn_upload_form($form_state);
  return $form;
}

/**
 * Creates the form for the tn upload page.
 *
 * @param string $form_state
 *   The current form state, if any.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_entities_get_tn_upload_form($form_state = NULL) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('jpg jpeg png');
  $form = array();
  $form['file'] = array(
    '#title' => t('Add Optional Thumbnail'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Select file to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume it's specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  return $form;
}

/**
 * Adds the uploaded file to the ingestible objects 'TN' datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_entities_tn_upload_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  // Is this a thumbnail update?
  $factory_object = islandora_object_load($form_state['islandora']['objects'][0]->id);
  if (is_object(islandora_object_load($factory_object))) {
    $object = $factory_object;
  }
  else {
    $object = islandora_ingest_form_get_object($form_state);
  }

  $tn_file = file_load($form_state['values']['file']);
  if (isset($tn_file->uri)) {
    islandora_temp_file_entry($tn_file->uri);
    $tn_path = drupal_realpath($tn_file->uri);
    $image = image_load($tn_path);
    image_scale($image, 150, NULL);
    image_save($image);
    if (empty($object['TN'])) {
      $ds = $object->constructDatastream('TN', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['TN'];
    }
    $ds->setContentFromFile($tn_path, FALSE);
    $ds->label = $tn_file->filename;
    $ds->mimetype = $tn_file->filemime;
  }
}

/**
 * Update thumbnail from manage page.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param AbstractObject $object
 *   The active fedora object.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_entities_update_tn(array $form, array &$form_state, $object) {
  $form_state['islandora']['objects'][0] = $object;
  $form = islandora_entities_get_tn_upload_form($form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * Update tn form submit handler.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_entities_update_tn_submit(array $form, array &$form_state) {
  islandora_entities_tn_upload_form_submit($form, $form_state);
  drupal_set_message(t("Thumbnail has been updated"), "status");
}
