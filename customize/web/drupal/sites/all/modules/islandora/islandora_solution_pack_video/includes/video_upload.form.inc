<?php

/**
 * @file
 * Handles the uploading of the video file.
 */

/**
 * Defines a file upload form for uploading the islandora video.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_video_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('ogg mp4 mov qt m4v avi mkv');
  $upload_required = variable_get('islandora_require_obj_upload', TRUE);
  return array(
    'file' => array(
      '#title' => t('Video'),
      '#type' => 'managed_file',
      '#required' => $upload_required,
      '#description' => t('Select video to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => $extensions[0])),
      '#default_value' => isset($form_state['values']['file']) ? $form_state['values']['file'] : NULL,
      '#upload_location' => 'temporary://',
      '#upload_validators' => array(
        'file_validate_extensions' => $extensions,
         // Assume its specified in MB.
        'file_validate_size' => array($upload_size * 1024 * 1024),
      ),
    ),
  );
}

/**
 * Submit handler, adds uploaded file to the video object.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_video_upload_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);
  if ($form_state['values']['file']) {
    if (empty($object['OBJ'])) {
      $ds = $object->constructDatastream('OBJ', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['OBJ'];
    }
    $file = file_load($form_state['values']['file']);
    $path = drupal_realpath($file->uri);
    $ds->setContentFromFile($path, FALSE);
    $ds->label = $file->filename;
    $ds->mimetype = $file->filemime;
  }
}
