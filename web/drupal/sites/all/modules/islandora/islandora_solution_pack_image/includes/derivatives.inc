<?php

/**
 * @file
 * Functions deriving images for objects.
 */

/**
 * Scales the given image.
 *
 * @param object $file
 *   The image file to scale.
 * @param int $width
 *   The width to scale the derived image to.
 * @param int $height
 *   The height to scale the derived image to.
 * @param bool $upscale
 *   Whether to upscale or not.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_basic_image_scale_image($file, $width, $height, $upscale = TRUE) {
  $real_path = drupal_realpath($file->uri);
  $image = image_load($real_path);
  try {
    if (!empty($image)) {
      $scale = image_scale($image, $width, $height, $upscale);
      if ($scale) {
        return image_save($image);
      }
    }
  }
  catch (exception $e) {
    drupal_set_message(t(
        "Basic image failed to scale image with message: '@message'",
        array("@message" => $e->getMessage())));
    watchdog(
      'islandora_basic_image',
      'Basic image failed to scale image.<br/> With stack: @trace',
      array('@trace' => $e->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
  return FALSE;
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file
 *   The URI of the file to be added as the datastream content.
 *
 * @return bool|string
 *   TRUE if the datastream was added/modified successfully, the error message
 *   otherwise.
 */
function islandora_basic_image_add_datastream(AbstractObject $object, $dsid, $file) {
  try {
    $ingest = !isset($object[$dsid]);
    if ($ingest) {
      $ds = $object->constructDatastream($dsid, 'M');
      $ds->label = $dsid;
    }
    else {
      $ds = $object[$dsid];
    }
    if ($ds->mimeType != $file->filemime) {
      $ds->mimeType = $file->filemime;
    }
    $ds->setContentFromFile(drupal_realpath($file->uri));
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
    file_delete($file);
    return TRUE;
  }
  catch (exception $e) {
    $variables = array(
      '@ret' => $e->getTraceAsString(),
    );
    watchdog('islandora_basic_image', 'Basic image failed to add data stream.<br/>Error: @ret', $variables, WATCHDOG_ERROR);
    $message = $e->getMessage();
    return $message;
  }
}

/**
 * Creates the TN for basic image.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 * @param bool $force
 *   Whether derivative generation is being forced or not.
 *
 * @return array
 *   An array describing the outcome of the thumbnail creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_basic_image_create_thumbnail(AbstractObject $object, $force = FALSE) {
  $return = '';
  if (!isset($object['TN']) || (isset($object['TN']) && $force === TRUE)) {
    $original_file = islandora_basic_image_copy_obj_datastream($object);
    if ($original_file === FALSE) {
      return islandora_basic_image_no_obj_datastream($object->id);
    }
    else {
      $tn_file = file_copy($original_file['file'], 'temporary://' . $original_file['filename'] . 'TN.' . $original_file['extension']);
      if (islandora_basic_image_scale_image($tn_file, 200, 200)) {
        $added_successfully = islandora_basic_image_add_datastream($object, 'TN', $tn_file);
        if ($added_successfully !== TRUE) {
          $return = islandora_basic_image_failed_adding($added_successfully);
        }
        else {
          $return = array(
            'success' => TRUE,
            'messages' => array(
              array(
                'message' => t('The TN datastream was added successfully for @pid!'),
                'message_sub' => array('@pid' => $object->id),
                'type' => 'dsm',
              ),
            ),
          );
        }
      }
      else {
        $return = islandora_basic_image_failed_scaling();
      }
    }
    if (isset($original_file)) {
      file_delete($original_file['file']);
    }
    return $return;
  }
}


/**
 * Creates the MEDIUM_SIZE derivative for basic image.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 * @param bool $force
 *   Whether derivative generation is being forced or not.
 *
 * @return array
 *   An array describing the outcome of the thumbnail creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_basic_image_create_medium_size(AbstractObject $object, $force = FALSE) {
  $return = '';
  if (!isset($object['MEDIUM_SIZE']) || (isset($object['MEDIUM_SIZE']) && $force === TRUE)) {
    $original_file = islandora_basic_image_copy_obj_datastream($object);
    if ($original_file === FALSE) {
      return islandora_basic_image_no_obj_datastream($object->id);
    }
    else {
      $medium_file = file_copy($original_file['file'], 'temporary://' . $original_file['filename'] . 'TN.' . $original_file['extension']);
      $upscale = variable_get('islandora_basic_image_upscale_images', TRUE);
      if (islandora_basic_image_scale_image($medium_file, 500, 700, $upscale)) {
        $added_successfully = islandora_basic_image_add_datastream($object, 'MEDIUM_SIZE', $medium_file);
        if ($added_successfully !== TRUE) {
          $return = islandora_basic_image_failed_adding($added_successfully);
        }
        else {
          $return = array(
            'success' => TRUE,
            'messages' => array(
              array(
                'message' => t('The MEDIUM_SIZE datastream was added successfully for @pid!'),
                'message_sub' => array('@pid' => $object->id),
                'type' => 'dsm',
              ),
            ),
          );
        }
      }
      else {
        $return = islandora_basic_image_failed_scaling();
      }
    }
    if (isset($original_file)) {
      file_delete($original_file['file']);
    }
    return $return;
  }
}

/**
 * Copies the OBJ to the file system.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array|stdClass
 *   FALSE when the object doesn't have the OBJ, an array otherwise.
 */
function islandora_basic_image_copy_obj_datastream(AbstractObject $object) {
  $mime_detect = new MimeDetect();
  if (!isset($object['OBJ'])) {
    return FALSE;
  }
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);

  // Create a file object we can save.
  $file_uri = file_create_filename("{$file_name}OBJ.{$ext}", 'temporary://');
  $file = new stdClass();
  $file->uri = $file_uri;
  $file->filename = $file_name;
  $file->filemime = $object['OBJ']->mimeType;
  // Temporary.
  $file->status = 0;
  $object['OBJ']->getContent($file_uri);
  $original_file = file_save($file);
  return array(
    'file' => $original_file,
    'extension' => $ext,
    'filename' => $file_name,
  );
}

/**
 * Helper function that returns a message when no OBJ datastream exists.
 *
 * @param string $pid
 *   The PID of the object that derivatives are being generated on.
 *
 * @return array
 *   An array describing the failure of the operation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_basic_image_no_obj_datastream($pid) {
  return array(
    'success' => FALSE,
    'messages' => array(
      array(
        'message' => t('Could not create image derivatives for %s. No image file was uploaded.'),
        'message_sub' => array('%s' => $pid),
        'type' => 'watchdog',
        'severity' => WATCHDOG_ERROR,
      ),
    ),
  );
}

/**
 * Helper function that returns a message when the image fails to scale.
 *
 * @return array
 *   An array describing the failure of the operation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_basic_image_failed_scaling() {
  return array(
    'success' => FALSE,
    'messages' => array(
      array(
        'message' => t('Unable to scale the image, the datastream was not added'),
        'type' => 'dsm',
        'severity' => 'warning',
      ),
    ),
  );
}

/**
 * Helper function describing failure when adding/modifying a datastream.
 *
 * @param string $message
 *   The error message returned when attempting to add/modify.
 *
 * @return array
 *   An array describing the failure of the operation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_basic_image_failed_adding($message) {
  return array(
    'success' => FALSE,
    'messages' => array(
      array(
        'message' => $message,
        'type' => 'watchdog',
        'severity' => WATCHDOG_ERROR,
      ),
    ),
  );
}
