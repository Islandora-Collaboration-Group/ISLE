<?php

/**
 * @file
 * Handles the uploading of the audio file as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the audio file.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_audio_audio_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $audio_extensions = array('wav mp3');
  $thumbnail_extensions = array('gif jpg png jpeg');
  $upload_required = variable_get('islandora_require_obj_upload', TRUE);

  return array(
    'audio_file' => array(
      '#title' => t('Audio File'),
      '#type' => 'managed_file',
      '#required' => $upload_required,
      '#description' => t('Select a file to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => $audio_extensions[0])),
      '#default_value' => isset($form_state['values']['audio_file']) ? $form_state['values']['audio_file'] : NULL,
      '#upload_location' => 'temporary://',
      '#upload_validators' => array(
        'file_validate_extensions' => $audio_extensions,
         // Assume its specified in MB.
        'file_validate_size' => array($upload_size * 1024 * 1024),
      ),
    ),
    'supply_thumbnail' => array(
      '#type' => 'checkbox',
      '#title' => t('Upload Thumbnail'),
    ),
    'thumbnail_section' => array(
      'thumbnail_file' => array(
        '#title' => t('Thumbnail File'),
        '#type' => 'managed_file',
        '#description' => t('Select a file to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => $thumbnail_extensions[0])),
        '#default_value' => isset($form_state['values']['thumbnail_file']) ? $form_state['values']['thumbnail_file'] : NULL,
        '#upload_location' => 'temporary://',
        '#upload_validators' => array(
          'file_validate_extensions' => $thumbnail_extensions,
           // Assume its specified in MB.
          'file_validate_size' => array($upload_size * 1024 * 1024),
        ),
      ),
      'scale_thumbnail' => array(
        '#type' => 'checkbox',
        '#title' => t('Scale Thumbnail'),
        '#attributes' => array('checked' => 'checked'),
      ),
      '#type' => 'item',
      '#states' => array(
        'visible' => array('#edit-supply-thumbnail' => array('checked' => TRUE)),
      ),
    ),
  );
}

/**
 * Validation for audio upload form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_audio_audio_upload_form_validate(array $form, array &$form_state) {
  if ($form_state['values']['supply_thumbnail'] &&
    !$form_state['values']['thumbnail_file']) {
    form_set_error('thumbnail_file', t('If you select "Upload Thumbnail" please supply a file.'));
  }
}

/**
 * Adds the uploaded file into the ingestable objects 'OBJ' datastream.
 *
 * May also populate the TN datatstream.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_audio_audio_upload_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $object = islandora_ingest_form_get_object($form_state);
  if ($form_state['values']['audio_file']) {
    if (empty($object['OBJ'])) {
      $obj = $object->constructDatastream('OBJ', 'M');
      $object->ingestDatastream($obj);
    }
    else {
      $obj = $object['OBJ'];
    }
    $audio_file = file_load($form_state['values']['audio_file']);
    $obj->setContentFromFile(drupal_realpath($audio_file->uri), FALSE);
    if ($obj->label != $audio_file->filename) {
      $obj->label = $audio_file->filename;
    }
    if ($obj->mimetype != $audio_file->filemime) {
      $obj->mimetype = $audio_file->filemime;
    }
  }
  if ($form_state['values']['supply_thumbnail']) {
    $thumbnail_file = file_load($form_state['values']['thumbnail_file']);
    if ($form_state['values']['scale_thumbnail']) {
      islandora_scale_thumbnail($thumbnail_file, 200, 200);
    }

    if (empty($object['TN'])) {
      $tn = $object->constructDatastream('TN', 'M');
      $object->ingestDatastream($tn);
    }
    else {
      $tn = $object['TN'];
    }
    $tn->setContentFromFile(drupal_realpath($thumbnail_file->uri), FALSE);
    if ($tn->label != $thumbnail_file->filename) {
      $tn->label = $thumbnail_file->filename;
    }
    if ($tn->mimetype != $thumbnail_file->filemime) {
      $tn->mimetype = $thumbnail_file->filemime;
    }
  }
}
