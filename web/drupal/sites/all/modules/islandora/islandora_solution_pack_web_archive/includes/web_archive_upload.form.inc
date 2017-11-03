<?php

/**
 * @file
 * Handles the uploading of the warc file.
 */

/**
 * Defines a file upload form for uploading the warc.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return string
 *   The drupal form definition.
 */
function islandora_web_archive_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('warc');
  $upload_required = variable_get('islandora_require_obj_upload', TRUE);
  $form['warc_file'] = array(
    '#title' => t('Warc'),
    '#type' => 'managed_file',
    '#required' => $upload_required,
    '#description' => t('Select warc to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['warc_file']) ? $form_state['values']['warc_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['upload_screenshot'] = array(
    '#title' => t('Upload a screenshot?'),
    '#type' => 'checkbox',
  );
  $form['screenshot_file'] = array(
    '#title' => t('Screenshot'),
    '#type' => 'managed_file',
    '#description' => t('Select screenshot to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>JPEG, JPG, PNG.</strong>', array('@size' => $upload_size)),
    '#default_value' => isset($form_state['values']['screenshot_file']) ? $form_state['values']['screenshot_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('jpg jpeg png'),
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="upload_screenshot"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['upload_pdf'] = array(
    '#title' => t('Upload a PDF?'),
    '#type' => 'checkbox',
  );
  $form['pdf_file'] = array(
    '#title' => t('PDF'),
    '#type' => 'managed_file',
    '#description' => t('Select screenshot to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>PDF.</strong>', array('@size' => $upload_size)),
    '#default_value' => isset($form_state['values']['pdf_file']) ? $form_state['values']['pdf_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('pdf'),
      // Assume its specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="upload_pdf"]' => array('checked' => TRUE),
      ),
    ),
  );
  return $form;
}

/**
 * Validation for the islandora_web_archive upload form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_web_archive_upload_form_validate(array $form, array &$form_state) {
  if ($form_state['values']['upload_screenshot']) {
    if ($form_state['values']['screenshot_file'] == 0) {
      form_error($form['screenshot_file'], t('No screenshot uploaded!'));
    }
  }

  if ($form_state['values']['upload_pdf']) {
    if ($form_state['values']['pdf_file'] == 0) {
      form_error($form['screenshot_file'], t('No PDF uploaded!'));
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
function islandora_web_archive_upload_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);
  if ($form_state['values']['warc_file']) {
    if (empty($object['OBJ'])) {
      $warc_ds = $object->constructDatastream('OBJ', 'M');
      $object->ingestDatastream($warc_ds);
    }
    else {
      $warc_ds = $object['OBJ'];
    }
    $warc_file = file_load($form_state['values']['warc_file']);
    $warc_ds->setContentFromFile($warc_file->uri, FALSE);
    $warc_ds->label = $warc_file->filename;
    $warc_ds->mimetype = $warc_file->filemime;
  }

  // See if there's an attached screenshot image to process.
  if ($form_state['values']['upload_screenshot']) {
    if (empty($object['SCREENSHOT'])) {
      $screenshot_ds = $object->constructDatastream('SCREENSHOT', 'M');
      $object->ingestDatastream($screenshot_ds);
    }
    else {
      $screenshot_ds = $object['SCREENSHOT'];
    }
    $image_file = file_load($form_state['values']['screenshot_file']);
    $screenshot_ds->setContentFromFile($image_file->uri, FALSE);
    $screenshot_ds->label = $image_file->filename;
    $screenshot_ds->mimetype = $image_file->filemime;
  }
  // See if there's an uploaded PDF to process.
  if ($form_state['values']['upload_pdf']) {
    if (empty($object['PDF'])) {
      $pdf_ds = $object->constructDatastream('PDF', 'M');
      $object->ingestDatastream($pdf_ds);
    }
    else {
      $pdf_ds = $object['PDF'];
    }
    $pdf_file = file_load($form_state['values']['pdf_file']);
    $pdf_ds->setContentFromFile($pdf_file->uri, FALSE);
    $pdf_ds->label = $pdf_file->filename;
    $pdf_ds->mimetype = $pdf_file->filemime;
  }
}
