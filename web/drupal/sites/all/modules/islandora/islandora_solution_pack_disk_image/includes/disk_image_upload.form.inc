<?php

/**
 * @file
 * Handles the uploading of the disk image files.
 */

/**
 * Defines a file upload form for uploading the disk image files.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return string
 *   The drupal form definition.
 */
function islandora_disk_image_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('dd aff exx iso raw');
  $upload_required = variable_get('islandora_require_obj_upload', TRUE);
  $form['disk_image_file'] = array(
    '#title' => t('Disk Image'),
    '#type' => 'managed_file',
    '#required' => $upload_required,
    '#description' => t('Select disk image to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array(
      '@size' => $upload_size,
      '@ext' => $extensions[0],
    )),
    '#default_value' => isset($form_state['values']['disk_image_file']) ? $form_state['values']['disk_image_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['upload_photo'] = array(
    '#title' => t('Upload a photo?'),
    '#type' => 'checkbox',
  );
  $form['photo_file'] = array(
    '#title' => t('Photo of Disk Image'),
    '#type' => 'managed_file',
    '#description' => t('Select photo to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>gif, png, jpg, jpeg.</b>', array('@size' => $upload_size)),
    '#default_value' => isset($form_state['values']['photo_file']) ? $form_state['values']['photo_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="upload_photo"]' => array('checked' => TRUE),
      ),
    ),
  );
  return $form;
}

/**
 * Validation for the islandora_disk_image upload form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_disk_image_upload_form_validate(array $form, array &$form_state) {
  if ($form_state['values']['upload_photo']) {
    if ($form_state['values']['photo_file'] == 0) {
      form_error($form['photo_file'], t('No photo uploaded!'));
    }
  }
}

/**
 * Submit handler, adds uploaded file to ingest object.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_disk_image_upload_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);
  if ($form_state['values']['disk_image_file']) {
    if (empty($object['OBJ'])) {
      $disk_image_ds = $object->constructDatastream('OBJ', 'M');
      $object->ingestDatastream($disk_image_ds);
    }
    else {
      $disk_image_ds = $object['OBJ'];
    }
    $disk_image_file = file_load($form_state['values']['disk_image_file']);
    $disk_image_ds->setContentFromFile($disk_image_file->uri, FALSE);
    $disk_image_ds->label = $disk_image_file->filename;
    $disk_image_ds->mimetype = $disk_image_file->filemime;
  }

  // See if there's an attached PHOTO to process.
  if ($form_state['values']['upload_photo']) {
    if (empty($object['PHOTO'])) {
      $photo_ds = $object->constructDatastream('PHOTO', 'M');
      $object->ingestDatastream($photo_ds);
    }
    else {
      $photo_ds = $object['PHOTO'];
    }
    $photo_file = file_load($form_state['values']['photo_file']);
    $photo_ds->setContentFromFile($photo_file->uri, FALSE);
    $photo_ds->label = $photo_file->filename;
    $photo_ds->mimetype = $photo_file->filemime;
  }
}
