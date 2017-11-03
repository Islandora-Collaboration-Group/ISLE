<?php

/**
 * @file
 * Contains all the functions required for adding derivatives for the PDF SP.
 */

/**
 * Gets the source of all derivatives as an unmanaged file.
 *
 * @param AbstractObject $object
 *   The object to generate/add the deriviative datastreams to.
 *
 * @return string
 *   The file path to the unmanaged derivative source file.
 */
function islandora_pdf_get_derivative_source_as_unmanaged_file(AbstractObject $object) {
  $mime_detect = new MimeDetect();
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);
  $file_uri = file_create_filename("{$file_name}.OBJ.{$ext}", 'temporary://');
  $object['OBJ']->getContent($file_uri);
  return $file_uri;
}

/**
 * Creates a JPG deriviative for the TN datastream.
 *
 * @param AbstractObject $object
 *   The object to add the deriviative datastreams to.
 * @param bool $force
 *   Whether the derivative generation is being forced or not.
 *
 * @return array|NULL
 *   An array describing the outcome of the thumbnail creation or NULL if no
 *   derivatives are to be created or updated.
 *
 * @see hook_islandora_derivative()
 */
function islandora_pdf_add_tn_derivative(AbstractObject $object, $force = FALSE) {
  if ($force || !isset($object['TN'])) {
    $width = variable_get('islandora_pdf_thumbnail_width', 200);
    $height = variable_get('islandora_pdf_thumbnail_height', 200);
    $file_uri = islandora_pdf_get_derivative_source_as_unmanaged_file($object);
    $results = islandora_pdf_add_jpg_derivative($object, $file_uri, 'TN', $width, $height, $force);
    file_unmanaged_delete($file_uri);
    return $results;
  }
}

/**
 * Creates a textual derivative for the FULL_TEXT datastream.
 *
 * @param AbstractObject $object
 *   The object to add the deriviative datastreams to.
 * @param bool $force
 *   Whether the derivative generation is being forced or not.
 *
 * @return array|NULL
 *   An array describing the outcome of the FULL_TEXT datastream creation or
 *   NULL if no derivatives are to be created or updated.
 *
 * @see hook_islandora_derivative()
 */
function islandora_pdf_add_fulltext_derivative(AbstractObject $object, $force = FALSE) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!isset($object['FULL_TEXT']) || (isset($object['FULL_TEXT']) && $force === TRUE)) {
    if (!isset($object['OBJ'])) {
      return islandora_pdf_missing_obj_datastream($object->id);
    }
    $file_uri = islandora_pdf_get_derivative_source_as_unmanaged_file($object);
    $dsid = "FULL_TEXT";
    $source = $source = drupal_realpath($file_uri);
    $executable = variable_get('islandora_pdf_path_to_pdftotext', '/usr/bin/pdftotext');
    $temp = file_create_filename('fulltext.txt', 'temporary://');
    $derivative_file_uri = drupal_realpath($temp);
    $command = "$executable $source $derivative_file_uri";
    exec($command, $execout, $returncode);
    file_unmanaged_delete($file_uri);
    $success = ($returncode === 0);

    if (!$success) {
      $success = array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('`%command` returned an error when creating %dsid derivative of PID %pid: %error'),
            'message_sub' => array(
              '%command' => $command,
              '%dsid' => $dsid,
              '%pid' => $object->id,
              '%error' => $execout,
            ),
            'type' => 'dsm',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      // Before we add this datastream let's rip out non valid XML characters.
      $full_text = file_get_contents($derivative_file_uri);
      $sanitized_full_text = islandora_sanitize_input_for_valid_xml($full_text);
      file_put_contents($derivative_file_uri, $sanitized_full_text);
      $success = islandora_pdf_add_datastream($object, $dsid, $derivative_file_uri);
      if ($success === TRUE) {
        return array(
          'success' => TRUE,
          'messages' => array(
            array(
              'message' => t('Created FULL_TEXT derivative.'),
              'type' => 'dsm',
            ),
          ),
        );
      }
      else {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('Failed to create FULL_TEXT derivative for @pid. Error message: @error'),
              'message_sub' => array(
                '@pid' => $object->id,
                '@error' => $success,
              ),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
    }
    file_unmanaged_delete($temp);
    return $success;
  }
}

/**
 * Creates a PDFA archival derivative and datastream.
 */
function islandora_pdf_add_pdfa_derivative(AbstractObject $object, $force = FALSE) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!isset($object['OBJ'])) {
    return islandora_pdf_missing_obj_datastream($object->id);
  }
  $file_uri = islandora_pdf_get_derivative_source_as_unmanaged_file($object);
  $dsid = "PDFA";
  $source = $source = drupal_realpath($file_uri);
  $executable = variable_get('islandora_pdf_path_to_gs', '/usr/bin/gs');
  $temp = file_create_filename('pdfa.pdf', 'temporary://');
  $derivative_file_uri = drupal_realpath($temp);
  $command = "$executable -dPDFA -dNOOUTERSAVE -dUseCIEColor -sProcessColorModel=DeviceRGB -sDEVICE=pdfwrite -o $derivative_file_uri -dPDFACompatibilityPolicy=1 $source ";
  exec($command, $execout, $returncode);
  file_unmanaged_delete($file_uri);
  $success = ($returncode === 0);

  if (!$success) {
    $success = array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('`%command` returned an error when creating %dsid derivative of PID %pid: %error'),
          'message_sub' => array(
            '%command' => $command,
            '%dsid' => $dsid,
            '%pid' => $object->id,
            '%error' => $execout,
          ),
          'type' => 'dsm',
          'severity' => WATCHDOG_ERROR,
        ),
      ),
    );
  }
  else {
    $success = islandora_pdf_add_datastream($object, $dsid, $derivative_file_uri);
    if ($success === TRUE) {
      return array(
        'success' => TRUE,
        'messages' => array(
          array(
            'message' => t('Created PDFA derivative.'),
            'type' => 'dsm',
          ),
        ),
      );
    }
    else {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('Failed to create PDFA derivative for @pid. Error message: @error'),
            'message_sub' => array(
              '@pid' => $object->id,
              '@error' => $success,
            ),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
  }
  file_unmanaged_delete($temp);
  return $success;
}

/**
 * Creates a JPG deriviative for the PREVIEW datastream.
 *
 * @param AbstractObject $object
 *   The object to add the deriviative datastreams to.
 * @param bool $force
 *   Whether the derivative generation is being forced or not.
 *
 * @return array|NULL
 *   An array describing the outcome of the PREVIEW datastream creation or NULL
 *   if no derivatives are to be created or updated.
 *
 * @see hook_islandora_derivative()
 */
function islandora_pdf_add_preview_derivative(AbstractObject $object, $force = FALSE) {
  if ($force || !isset($object['PREVIEW'])) {
    $width = variable_get('islandora_pdf_preview_width', 500);
    $height = variable_get('islandora_pdf_preview_height', 700);
    $file_uri = islandora_pdf_get_derivative_source_as_unmanaged_file($object);
    $results = islandora_pdf_add_jpg_derivative($object, $file_uri, 'PREVIEW', $width, $height, $force);
    file_unmanaged_delete($file_uri);
    return $results;
  }
}

/**
 * Creates a JPG deriviative of the given file.
 *
 * @param AbstractObject $object
 *   The object to add the deriviative datastreams to.
 * @param string $file_uri
 *   The URI to the PDF file the derivative will be generated from.
 * @param string $dsid
 *   The datastream ID to use for the derived datastream.
 * @param int $width
 *   The width to make the derived datastream.
 * @param int $height
 *   The height to make the derived datastream.
 * @param bool $force
 *   Whether the derivative generation is being forced or not.
 *
 * @return array|NULL
 *   An array describing the outcome of adding the JPG derivative or NULL if no
 *   derivatives are to be created or updated.
 *
 * @see hook_islandora_derivative()
 */
function islandora_pdf_add_jpg_derivative(AbstractObject $object, $file_uri, $dsid, $width, $height, $force) {
  if (!isset($object[$dsid]) || (isset($object[$dsid]) && $force === TRUE)) {
    if (!isset($object['OBJ'])) {
      return islandora_pdf_missing_obj_datastream($object->id);
    }
    $derivative_file_uri = islandora_pdf_create_jpg_derivative($file_uri, $dsid, $width, $height);
    file_unmanaged_delete($file_uri);
    // Receive a valid file URI to add or an error message otherwise.
    if (!is_array($derivative_file_uri) && file_valid_uri($derivative_file_uri)) {
      $success = islandora_pdf_add_datastream($object, $dsid, $derivative_file_uri);
      file_unmanaged_delete($derivative_file_uri);
      if ($success === TRUE) {
        return array(
          'success' => TRUE,
          'messages' => array(
            array(
              'message' => t('Created JPG derivative for @dsid (@pid).'),
              'message_sub' => array(
                '@dsid' => $dsid,
                '@pid' => $object->id,
              ),
              'type' => 'dsm',
            ),
          ),
        );
      }
      else {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('Failed to create JPG derivative for @dsid (@pid). Error message: @message'),
              'message_sub' => array(
                '@dsid' => $dsid,
                '@pid' => $object->id,
                '@message' => $success,
              ),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
    }
    else {
      return $derivative_file_uri;
    }
  }
}

/**
 * Creates a jpg derivative from the given pdf file.
 *
 * @param string $file_uri
 *   The URI to the pdf file the derivative will be generated from.
 * @param string $dsid
 *   Used to identify the generated file.
 * @param int $width
 *   The width to make the derived datastream.
 * @param int $height
 *   The height to make the derived datastream.
 *
 * @return string|array
 *   A URI to the generated derivative if successful, error message otherwise.
 *
 * @see hook_islandora_derivative()
 */
function islandora_pdf_create_jpg_derivative($file_uri, $dsid, $width, $height) {
  $source = drupal_realpath($file_uri) . '[0]';
  $matches = array();
  // Get the base name of the source file.
  $base = pathinfo($source, PATHINFO_FILENAME);
  $temp = file_create_filename("$base.$dsid.jpg", 'temporary://');
  $dest = drupal_realpath($temp);
  $args['quality'] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
  $args['previewsize'] = '-resize ' . escapeshellarg("{$width}x{$height}");
  $args['colors'] = '-colorspace RGB';
  $args['flatten'] = '-flatten';
  $context = array(
    'source' => $source,
    'destination' => $dest,
  );
  drupal_alter('imagemagick_arguments', $args, $context);
  // To make use of ImageMagick 6's parenthetical command grouping we need to
  // make the $source image the first parameter and $dest the last.
  // See http://www.imagemagick.org/Usage/basics/#cmdline
  $command = escapeshellarg($source) . ' ' . implode(' ', $args) . ' ' . escapeshellarg("jpg:$dest");
  $output = '';
  $ret = -1;
  if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
    return array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('ImageMagick failed to create derivatives for @pid<br/>Error: @ret<br/>Command: @command<br/>Output: @output'),
          'message_sub' => array(
            '@ret' => $ret,
            '@command' => $command,
            '@output' => $output,
          ),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ),
      ),
    );
  }
  return $temp;
}

/**
 * Adds the file as a datastream to the object using the DSID to identify it.
 *
 * @param AbstractObject $object
 *   The object to add the datastream to.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 *
 * @return bool|string
 *   TRUE if successfully added/modified the datastream, error message if not.
 */
function islandora_pdf_add_datastream($object, $datastream_id, $file_uri) {
  $return = TRUE;
  try {
    $ingest = !isset($object[$datastream_id]);
    $mime_detector = new MimeDetect();
    if ($ingest) {
      $ds = $object->constructDatastream($datastream_id, "M");
      $ds->label = $datastream_id;
    }
    else {
      $ds = $object[$datastream_id];
    }
    if ($ds->mimetype != $mime_detector->getMimetype($file_uri)) {
      $ds->mimetype = $mime_detector->getMimetype($file_uri);
    }
    $ds->setContentFromFile(drupal_realpath($file_uri), FALSE);
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
  }
  catch (exception $e) {
    $return = $e->getMessage();
  }
  file_unmanaged_delete($file_uri);
  return $return;
}

/**
 * Helper function declaring that no OBJ datastream is present.
 *
 * @param string $pid
 *   The pid of the ingested object.
 *
 * @return array
 *   An array describing the failure of the operation..
 *
 * @see hook_islandora_derivative()
 */
function islandora_pdf_missing_obj_datastream($pid) {
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
