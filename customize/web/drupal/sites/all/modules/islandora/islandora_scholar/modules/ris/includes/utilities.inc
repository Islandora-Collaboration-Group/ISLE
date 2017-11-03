<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Transform the given RIS string into MODS.
 *
 * @param string $ris_string
 *   A string containing an RIS record.
 *
 * @return string
 *   A string containing MODS XML.
 */
function islandora_ris_transform_to_mods($ris_string) {
  $ris_file = file_save_data($ris_string, "temporary://temp.ris");
  $ris_file->status &= ~FILE_STATUS_PERMANENT;
  file_save($ris_file);
  $mods_file = drupal_tempnam('temporary://', 'temp_mods.xml');

  module_load_include('inc', 'bibutils', 'includes/utilities');
  Bibutils::Convert(
    drupal_realpath($ris_file->uri),
    'RIS',
    drupal_realpath($mods_file),
    'MODS'
  );

  $to_return = file_get_contents($mods_file);
  module_load_include('inc', 'islandora', 'includes/utilities');
  $mods_file = islandora_temp_file_entry($mods_file, 'text/xml');

  file_delete($ris_file);
  file_delete($mods_file);

  return $to_return;
}

/**
 * Extract the first RIS record into its own document.
 *
 * @param object $file
 *   A Drupal file-object containing RIS records. We will remove the record
 *   we extract from it.
 *
 * @return string|bool
 *   Either a string containing the extracted record, or boolean FALSE if there
 *   was none to extract.
 */
function islandora_ris_extract_first($file) {
  $record = '';

  // Get the first Record from the RIS File.
  $fp = fopen($file->uri, 'r');
  while (($line = fgets($fp)) !== FALSE) {
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

  $remaining = stream_get_contents($fp);
  fclose($fp);
  file_put_contents($file->uri, $remaining);
  file_save($file);

  $record = trim($record);
  return empty($record) ? FALSE : $record;
}

/**
 * Convenience function; extract and transform from the given file.
 *
 * @param object $file
 *   A Drupal file-object.
 *
 * @return string|bool
 *   Either a string containing the transformed MODS or FALSE in the event of a
 *   failure.
 */
function islandora_ris_get_mods($file) {
  $ris_record = islandora_ris_extract_first($file);
  return $ris_record ?
    islandora_ris_transform_to_mods($ris_record) :
    FALSE;
}
