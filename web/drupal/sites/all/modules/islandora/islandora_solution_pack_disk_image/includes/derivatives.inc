<?php

/**
 * @file
 * This file contains all the functions for disk image manipulation used in
 * the disk image solution pack.
 */

/**
 * Fetches the uploaded photo file from Fedora, and saves it to a temp location.
 *
 * @param AbstractObject $object
 *   The object to fetch the uploaded photo file from.
 * @param string $base_name
 *   The base name to use in creating temp files.
 *
 * @return string
 *   The file path to the temp file if successful, otherwise FALSE.
 */
function islandora_disk_image_get_uploaded_photo(AbstractObject $object, $base_name) {
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($object['PHOTO']->mimeType);
  $filename = file_create_filename("{$base_name}_PHOTO.{$ext}", 'temporary://');
  $object['PHOTO']->getContent($filename);
  return $filename;
}

/**
 * Creates the JPG derived datatstream from the uploaded image file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return array
 *   An array describing the outcome of the JPG creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_disk_image_create_JPG_derivative(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['JPG'])) {
    if (!isset($object['PHOTO'])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No PHOTO datastream present for object @pid - JPG creation was skipped.'),
            'message_sub' => array('@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $base_name = str_replace(':', '-', $object->id);
      $uploaded_photo = islandora_disk_image_get_uploaded_photo($object, $base_name);
      $args = array();
      $args[] = '-resize ' . escapeshellarg("600 x 800");
      $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
      $derivative_file = islandora_disk_image_imagemagick_convert($uploaded_photo, "temporary://{$base_name}_JPG.jpg", $args);
      file_unmanaged_delete($uploaded_photo);
      if ($derivative_file === FALSE) {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('Failed to create JPG derivative for @pid.'),
              'message_sub' => array('@pid' => $object->id),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
      else {
        return islandora_disk_image_add_datastream($object, 'JPG', $derivative_file, 'image/jpeg', t('Medium sized JPEG'));
      }
    }
  }
}

/**
 * Creates the TN derived datatstream from the uploaded image file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return array
 *   An array describing the outcome of the JPG creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_disk_image_create_TN_derivative(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['TN'])) {
    if (!isset($object['PHOTO'])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No PHOTO datastream present for object @pid - TN creation was skipped.'),
            'message_sub' => array('@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $base_name = str_replace(':', '-', $object->id);
      $uploaded_photo = islandora_disk_image_get_uploaded_photo($object, $base_name);
      $args = array();
      $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
      $args[] = '-resize ' . escapeshellarg("200 x 200");
      $derivative_file = islandora_disk_image_imagemagick_convert($uploaded_photo, "temporary://{$base_name}_TN.jpg", $args);
      file_unmanaged_delete($uploaded_photo);
      if ($derivative_file === FALSE) {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('Failed to create TN derivative for @pid.'),
              'message_sub' => array('@pid' => $object->id),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
      else {
        return islandora_disk_image_add_datastream($object, 'TN', $derivative_file, 'image/jpeg', t('Thumbnail'));
      }
    }
  }
}

/**
 * Calls imagemagick's convert command with the given arguments.
 *
 * @param string $src
 *   The source file uri to convert from.
 * @param string $dest
 *   The destination file uri to convert into.
 * @param string $args
 *   The source file to convert from.
 *
 * @return string
 *   The destination file path if successful otherwise FALSE.
 */
function islandora_disk_image_imagemagick_convert($src, $dest, $args) {
  $src = drupal_realpath($src) . '[0]';
  $dest = drupal_realpath($dest);
  $context = array(
    'source' => $src,
    'destination' => $dest,
  );
  drupal_alter('imagemagick_arguments', $args, $context);
  // To make use of ImageMagick 6's parenthetical command grouping we need to.
  // make the $source image the first parameter and $dest the last.
  // See http://www.imagemagick.org/Usage/basics/#cmdline.
  $command = escapeshellarg($src) . ' ' . implode(' ', $args) . ' ' . escapeshellarg($dest);
  $output = '';
  $ret = -1;
  if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
    $message = 'ImageMagick failed to create derivatives<br/>Error: @ret<br/>Command: @command<br/>Output: @output';
    $variables = array(
      '@ret' => $ret,
      '@command' => $command,
      '@output' => $output,
    );
    watchdog('islandora_disk_image', "@message  @variables", WATCHDOG_ERROR);
    return FALSE;
  }
  return $dest;
}

/**
 * Fetches the uploaded disk image from Fedora and saves it to a temp location.
 *
 * @param AbstractObject $object
 *   The object to fetch the uploaded image file from.
 * @param string $disk_image_name
 *   The base name to use in creating temp files.
 *
 * @return string
 *   The file path to the temp file if successful, otherwise FALSE.
 */
function islandora_disk_image_get_uploaded_disk_image(AbstractObject $object, $disk_image_name) {
  $filename = file_create_filename("{$disk_image_name}_OBJ", 'temporary://');
  $object['OBJ']->getContent($filename);
  return $filename;
}

/**
 * Creates the TECHMD_DFXML derived datatstream from the uploaded disk image.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return array
 *   An array describing the outcome of the TECHMD_DFXML creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_disk_image_create_DFXML_derivative(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['TECHMD_DFXML'])) {
    if (!isset($object['OBJ'])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No OBJ datastream present for object @pid - TECHMD_DFXML creation was skipped.'),
            'message_sub' => array('@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $disk_image_name = str_replace(':', '-', $object->id);
      $uploaded_disk_image = islandora_disk_image_get_uploaded_disk_image($object, $disk_image_name);
      $fiwalk = variable_get('islandora_disk_image_fiwalk_path', '/usr/local/bin/fiwalk');
      $dfxml_file = drupal_realpath("temporary://{$disk_image_name}_TECHMD_DFXML.xml");
      $disk_image_file = drupal_realpath($uploaded_disk_image);
      $command = "$fiwalk -fx $disk_image_file > $dfxml_file";
      $derivative_file = exec($command, $output);
      file_unmanaged_delete($uploaded_disk_image);
      if ($derivative_file === FALSE) {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('TECHMD_DFXML failed for object @pid - TECHMD_DFXML creation was skipped.'),
              'message_sub' => array('@pid' => $object->id),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
      else {
        return islandora_disk_image_add_datastream($object, 'TECHMD_DFXML', $dfxml_file, 'application/xml', t('DFXML'));
      }
    }
  }
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file_uri
 *   The URI of the file to be added as the datastream content.
 * @param string $mimetype
 *   The mimetype of the file.
 * @param string $label
 *   The label for the datastream.
 *
 * @return array
 *   An array describing the outcome of the datastream addition.
 */
function islandora_disk_image_add_datastream(AbstractObject $object, $dsid, $file_uri, $mimetype, $label) {
  try {
    $ingest = !isset($object[$dsid]);

    if ($ingest) {
      $ds = $object->constructDatastream($dsid, "M");
      $ds->label = $label;
    }
    else {
      $ds = $object[$dsid];
    }
    $ds->mimetype = $mimetype;
    $ds->setContentFromFile(drupal_realpath($file_uri));
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
    file_unmanaged_delete($file_uri);
    return array(
      'success' => TRUE,
      'messages' => array(
        array(
          'message' => t('Created @dsid derivative for @pid.'),
          'message_sub' => array(
            '@dsid' => $dsid,
            '@pid' => $object->id,
          ),
          'type' => 'dsm',
        ),
      ),
    );
  }
  catch (exception $e) {
    file_unmanaged_delete($file_uri);
    return array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('Disk image solution pack failed to add @dsid datastream for @pid. Error message: @message<br/>Stack: @stack'),
          'message_sub' => array(
            '@dsid' => $dsid,
            '@pid' => $object->id,
            '@message' => $e->getmessage(),
            '@stack' => $e->getTraceAsString(),
          ),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ),
      ),
    );
  }
}
