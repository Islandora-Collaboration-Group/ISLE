<?php

/**
 * @file
 * This file contains all the functions for screen capture manipulation used in
 * the web archive solution pack.
 */

/**
 * Fetches the uploaded image file from Fedora, and saves it to a temp location.
 *
 * @param AbstractObject $object
 *   The object to fetch the uploaded image file from.
 * @param string $base_name
 *   The base name to use in creating temp files.
 *
 * @return string
 *   The file path to the temp file if successful, otherwise FALSE.
 */
function islandora_web_archive_get_uploaded_screenshot(AbstractObject $object, $base_name) {
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($object['SCREENSHOT']->mimeType);
  $filename = file_create_filename("{$base_name}_SCREENSHOT.{$ext}", 'temporary://');
  $object['SCREENSHOT']->getContent($filename);
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
function islandora_web_archive_create_JPG_derivative(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['JPG'])) {
    if (!isset($object['SCREENSHOT'])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No SCREENSHOT datastream present for object @pid - JPG creation was skipped.'),
            'message_sub' => array('@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $base_name = str_replace(':', '-', $object->id);
      $upload_screenshot = islandora_web_archive_get_uploaded_screenshot($object, $base_name);
      $args = array();
      $args[] = '-resize ' . escapeshellarg("600 x 800");
      $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
      $derivative_file = islandora_web_archive_imagemagick_convert($upload_screenshot, "temporary://{$base_name}_MED.jpg", $args);
      file_unmanaged_delete($upload_screenshot);
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
        return islandora_web_archive_add_datastream($object, 'JPG', $derivative_file, 'image/jpeg', t('Medium sized JPEG'));
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
function islandora_web_archive_create_TN_derivative(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['TN'])) {
    if (!isset($object['SCREENSHOT'])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No SCREENSHOT datastream present for object @pid - TN creation was skipped.'),
            'message_sub' => array('@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $base_name = str_replace(':', '-', $object->id);
      $upload_screenshot = islandora_web_archive_get_uploaded_screenshot($object, $base_name);
      $args = array();
      $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
      $args[] = '-resize ' . escapeshellarg("200 x 200");
      $derivative_file = islandora_web_archive_imagemagick_convert($upload_screenshot, "temporary://{$base_name}_TN.jpg", $args);
      file_unmanaged_delete($upload_screenshot);
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
        return islandora_web_archive_add_datastream($object, 'TN', $derivative_file, 'image/jpeg', t('Thumbnail'));
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
function islandora_web_archive_imagemagick_convert($src, $dest, $args) {
  $src = drupal_realpath($src);
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
    watchdog('islandora_web_archive', "@message  @variables", WATCHDOG_ERROR);
    return FALSE;
  }
  return $dest;
}

/**
 * Fetches the uploaded warc file from Fedora, and saves it to a temp location.
 *
 * @param AbstractObject $object
 *   The object to fetch the uploaded image file from.
 * @param string $warc_name
 *   The base name to use in creating temp files.
 *
 * @return string
 *   The file path to the temp file if successful, otherwise FALSE.
 */
function islandora_web_archive_get_uploaded_warc(AbstractObject $object, $warc_name) {
  $filename = file_create_filename("{$warc_name}_OBJ.warc", 'temporary://');
  $object['OBJ']->getContent($filename);
  return $filename;
}

/**
 * Creates the WARC_CSV derived datatstream from the uploaded warc file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return array
 *   An array describing the outcome of the WARC_CSV creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_web_archive_create_warc_csv_derivative(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['WARC_CSV'])) {
    if (!isset($object['OBJ'])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No OBJ datastream present for object @pid - WARC_CSV creation was skipped.'),
            'message_sub' => array('@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $warc_name = str_replace(':', '-', $object->id);
      $uploaded_warc = islandora_web_archive_get_uploaded_warc($object, $warc_name);
      $warcindex = variable_get('islandora_web_archive_warcindex_path', '/usr/local/bin/warcindex');
      $csv_file = drupal_realpath("temporary://{$warc_name}_WARC_CSV.csv");
      $warc_file = drupal_realpath($uploaded_warc);
      $command = "$warcindex $warc_file > $csv_file";
      $derivative_file = exec($command, $output);
      file_unmanaged_delete($uploaded_warc);
      if ($derivative_file === FALSE) {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('warcindex failed for object @pid - WARC_CSV creation was skipped.'),
              'message_sub' => array('@pid' => $object->id),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
      else {
        return islandora_web_archive_add_datastream($object, 'WARC_CSV', $csv_file, 'text/plain', t('WARC Index'));
      }
    }
  }
}

/**
 * Creates the WARC_FILTERED derived datatstream from the uploaded warc file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return array
 *   An array describing the outcome of the WARC_FILTERED creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_web_archive_create_warc_filter_derivative(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['WARC_FILTERED'])) {
    if (!isset($object['OBJ'])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No OBJ datastream present for object @pid - WARC_CSV creation was skipped.'),
            'message_sub' => array('@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $warc_name = str_replace(':', '-', $object->id);
      $uploaded_warc = islandora_web_archive_get_uploaded_warc($object, $warc_name);
      $warcfilter = variable_get('islandora_web_archive_warcfilter_path', '/usr/local/bin/warcfilter');
      $filtered_file = drupal_realpath("temporary://{$warc_name}_WARC_FILTERED.warc");
      $warc_file = drupal_realpath($uploaded_warc);
      $command = "$warcfilter -H text $warc_file > $filtered_file";
      $derivative_file = exec($command, $output);
      file_unmanaged_delete($uploaded_warc);
      if ($derivative_file === FALSE) {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('warcfilter failed for object @pid - WARC_FILTERED creation was skipped.'),
              'message_sub' => array('@pid' => $object->id),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
      else {
        return islandora_web_archive_add_datastream($object, 'WARC_FILTERED', $filtered_file, 'application/warc', t('WARC Filtered'));
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
function islandora_web_archive_add_datastream(AbstractObject $object, $dsid, $file_uri, $mimetype, $label) {
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
          'message' => t('Web archive solution pack failed to add @dsid datastream for @pid. Error message: @message<br/>Stack: @stack'),
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


/**
 * Uses Imagemagick's identify to examine the file in question.
 *
 * @param string $file
 *   A file-system path to the file in question.
 *
 * @return bool
 *   A bool indicating if the file contains a TIFF which is not compressed,
 *   (if it could be reencoded by the "free" version of Kakadu.
 */
function islandora_web_archive_is_uncompressed_png($file) {
  $identify = islandora_large_image_get_identify();
  $file = drupal_realpath($file);
  $compression = exec(escapeshellcmd("$identify -format \"%C\" $file"));
  $compressed = (strtolower($compression) != 'none');
  return !$compressed && islandora_web_archive_is_png($file);
}

/**
 * Uses Imagemagick's identify to determine if the file is a PNG.
 *
 * @param string $file
 *   A file-system path to the file in question.
 *
 * @return bool
 *   A bool indicating if the file contains a PNG.
 */
function islandora_web_archive_is_png($file) {
  $identify = islandora_web_archive_get_identify();
  $file = drupal_realpath($file);
  $codec = exec(escapeshellcmd("$identify -format \"%m\" $file"));
  $is_png = (strtolower($codec) == 'png');
  return $is_png;
}

/**
 * Attempt to get the "identify" executable...
 *
 * Assume convert is sanely named, and that identify can be obtained from the
 * same place.
 *
 * @return string
 *   The command to run for "identify".
 */
function islandora_web_archive_get_identify() {
  // Get path for convert.
  $convert = variable_get('imagemagick_convert', 'convert');

  // Replace "convert" with "identify".
  $identify = str_replace('convert', 'identify', $convert);
  return $identify;
}
