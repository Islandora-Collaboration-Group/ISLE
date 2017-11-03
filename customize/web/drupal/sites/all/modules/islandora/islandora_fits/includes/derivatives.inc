<?php

/**
 * @file
 * Miscellaneous helper functions for creating/ingesting FITS documents.
 */

/**
 * Creates all technical metadata derivatives as datastreams for given object.
 *
 * @param AbstractObject $object
 *   The object that will be used to generate/store the derivatives.
 * @param bool $force
 *   Whether derivative generation is being forced or not.
 * @param array $hook
 *   The derivative definition.
 *
 * @return array
 *   An array describing the outcome of the TECHMD creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_fits_create_techmd(AbstractObject $object, $force = FALSE, $hook = array()) {
  $techmd_dsid = variable_get('islandora_fits_techmd_dsid', 'TECHMD');
  if ($force || !isset($object[$techmd_dsid])) {
    if (!isset($object[$hook['source_dsid']])) {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('No @source datastream present for object @pid - technical metadata extraction was skipped.'),
            'message_sub' => array('@source' => $hook['source_dsid'], '@pid' => $object->id),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    $mime_detect = new MimeDetect();
    $ext = $mime_detect->getExtension($object[$hook['source_dsid']]->mimeType);
    $file_name = str_replace(':', '-', $object->id);
    $out_file = drupal_realpath("temporary://{$file_name}.{$hook['source_dsid']}.{$ext}");
    $object[$hook['source_dsid']]->getContent($out_file);

    $fits_file = islandora_fits_create_fits($out_file);
    if ($fits_file === FALSE) {
      $success = array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('Failed to create technical metadata with FITS script.'),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
    else {
      $success = islandora_fits_add_datastream($object, variable_get('islandora_fits_techmd_dsid', 'TECHMD'), $fits_file);
      drupal_unlink($fits_file);
    }

    // Add any more processors here - there probably won't be any.
    drupal_unlink($out_file);
    return $success;
  }
}

/**
 * Creates the technical metadata derivative from the given file.
 *
 * @param string $file
 *   The URI to the file from which the derivative will be generated.
 *
 * @return string
 *   A URI to the generated derivative if successful, FALSE otherwise.
 */
function islandora_fits_create_fits($file) {
  $output = array();
  $outfile = "{$file}.tech.xml";
  $fits = variable_get('islandora_fits_executable_path', 'fits.sh');
  $command = "$fits -i $file -xc -o $outfile";
  exec($command, $output, $ret);
  if ($ret == '0') {
    if (islandora_fits_verify_fits_xml($outfile)) {
      return $outfile;
    }
  }
  $output = array();
  // It failed, lets try a simpler command (-x instead of -xc).
  $command = "$fits -i $file -x -o $outfile";
  exec($command, $output, $ret);
  if ($ret == '0') {
    if (islandora_fits_verify_fits_xml($outfile)) {
      return $outfile;
    }
  }
  $output = array();
  // In case of disaster, fall back to the simplest possibly command line.
  $command = "$fits -i $file -o $outfile";
  exec($command, $output, $ret);
  if ($ret == '0') {
    if (islandora_fits_verify_fits_xml($outfile)) {
      return $outfile;
    }
  }
  return FALSE;
}

/**
 * Helper to verify the fits XML file size is greater then 0.
 *
 * This function does not verify the contents of the provided xml file.
 * WARNING: This function will delete said file if its file size is equal to 0.
 *
 * @param string $xml_file
 *   The path to the created xml file to validate
 *
 * @return bool
 *   TRUE if the generated XML has size, FALSE otherwise.
 */
function islandora_fits_verify_fits_xml($xml_file) {
  if (filesize($xml_file) > 0) {
    return TRUE;
  }
  else {
    // Fits wont write to a file if it already exists.
    if (file_exists($xml_file)) {
      drupal_unlink($xml_file);
    }
    return FALSE;
  }
}

/**
 * Adds the given file as a datastream to the given object.
 *
 * @param AbstractObject $object
 *   The object to add the datasteam to.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 *
 * @return array
 *   An array describing the outcome of the datastream addition.
 *
 * @see hook_islandora_derivative()
 */
function islandora_fits_add_datastream($object, $datastream_id, $file_uri) {
  try {
    $ingest = !isset($object[$datastream_id]);
    $mime_detector = new MimeDetect();
    if ($ingest) {
      $ds = $object->constructDatastream($datastream_id, 'M');
      $ds->label = $datastream_id;
    }
    else {
      $ds = $object[$datastream_id];
    }
    if ($ds->mimetype != $mime_detector->getMimetype($file_uri)) {
      $ds->mimetype = $mime_detector->getMimetype($file_uri);
    }
    $ds->setContentFromFile(drupal_realpath($file_uri));
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
    return array(
      'success' => TRUE,
      'messages' => array(
        array(
          'message' => t('Created TECHMD derivative for (@pid).'),
          'message_sub' => array(
            '@pid' => $object->id,
          ),
          'type' => 'dsm',
        ),
      ),
    );
  }
  catch (exception $e) {
    return array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('Failed to create TECHMD derivative for @pid.'),
          'message_sub' => array(
            '@pid' => $object->id,
          ),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ),
      ),
    );
  }
}
