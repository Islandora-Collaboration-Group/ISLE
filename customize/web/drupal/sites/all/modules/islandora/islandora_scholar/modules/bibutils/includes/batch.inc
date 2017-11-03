<?php

/**
 * @file
 * Functions for batch use of Bibutils.
 *
 * While Bibutils doesn't provide any batch operations directly, these
 * functions will be used by other modules for Doing batch ingests/exports
 * and what not.
 */

/**
 * Removes a single record from the EndNote XML file and returns it.
 *
 * @param string $filename
 *   The filename of a EndNote XML Document.
 *
 * @return DOMDocument
 *   A single EndNote XML record if found, FALSE otherwise.
 */
function bibutils_batch_get_endnote_record($filename) {
  $endnote = new DOMDocument();
  $endnote->load($filename);
  $xpath = new DOMXPath($endnote);
  $results = $xpath->query('/xml/records/record');
  $documents = array();
  if ($results->length >= 1) {
    $doc = new DOMDocument();
    $child = $results->item(0);
    $doc->loadXML('<xml><records>' . $endnote->saveXML($child) . '</records></xml>');
    $child->parentNode->removeChild($child);
    $endnote->save($filename);
    return $doc;
  }
  return FALSE;
}

/**
 * Gets the number of records in the EndNote XML file.
 *
 * @param string $filename
 *   The filename of the EndNote XML file.
 *
 * @return int
 *   The number of Records in the EndNote XML file.
 */
function bibutils_batch_get_number_endnote_records($filename) {
  $endnote = new DOMDocument();
  $endnote->load($filename);
  $xpath = new DOMXPath($endnote);
  $results = $xpath->query('/xml/records/record');
  return $results ? $results->length : 0;
}

/**
 * Removes a single record from the RIS file and returns it.
 *
 * @param string $filename
 *   The filename of the RIS file.
 *
 * @return string
 *   The RIS record if successful, FALSE otherwise.
 */
function bibutils_batch_get_ris_record($filename) {
  // Get the first Record from the RIS File.
  $file = fopen($filename, 'r');
  $record = '';
  while (($line = fgets($file)) !== FALSE) {
    $trimed_line = trim($line);
    // Ignore blank lines.
    if (!empty($trimed_line)) {
      // Store the line in outgoing record.
      $record .= $line;
      if ($trimed_line == 'ER  -') {
        break;
      }
    }
  }
  // Put the remaining text into a temp file.
  $temp_file_dir = file_directory_temp();
  $temp_filename = file_create_filename("ris_temp.ris", $temp_file_dir);
  $temp_file = fopen($temp_filename, 'w');
  while (!feof($file)) {
    fwrite($temp_file, fread($file, 1024));
  }
  fclose($file);
  fclose($temp_file);
  // Replace the RIS file with the shorter temp file.
  drupal_unlink($filename);
  rename($temp_filename, $filename);
  return (trim($record) == '') ? FALSE : $record;
}

/**
 * Gets the number of records in the RIS file.
 *
 * @param string $filename
 *   The filename of the RIS file.
 *
 * @return int
 *   The number of Records in the RIS file.
 */
function bibutils_batch_get_number_ris_records($filename) {
  $count = 0;
  $file = fopen($filename, 'r');
  while (($line = fgets($file)) !== FALSE) {
    if (trim($line) == 'ER  -') {
      $count++;
    }
  }
  fclose($file);
  return $count;
}
