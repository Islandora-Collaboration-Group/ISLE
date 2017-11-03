<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Transform the given EndNoteXML string into MODS.
 *
 * @param string $xml_string
 *   A string containing EndNote XML.
 *
 * @return string
 *   A string containing MODS XML.
 */
function islandora_endnotexml_transform_to_mods($xml_string) {
  $enxml_file = file_save_data($xml_string, "temporary://temp_en.xml");
  $enxml_file->status &= ~FILE_STATUS_PERMANENT;
  file_save($enxml_file);
  $mods_file = drupal_tempnam('temporary://', 'temp_mods.xml');

  module_load_include('inc', 'bibutils', 'includes/utilities');
  Bibutils::Convert(
    drupal_realpath($enxml_file->uri),
    'EndNoteXML',
    drupal_realpath($mods_file),
    'MODS'
  );

  $to_return = file_get_contents($mods_file);
  module_load_include('inc', 'islandora', 'includes/utilities');
  $mods_file = islandora_temp_file_entry($mods_file, 'text/xml');

  file_delete($enxml_file);
  file_delete($mods_file);

  return $to_return;
}

/**
 * Extract the first EndNote XML record into its own document.
 *
 * @param DOMDocument $endnote
 *   A DOMDocument containing the parsed EndNote XML. We will remove the record
 *   we extract from it.
 *
 * @return string|bool
 *   Either a string containing the extracted record, or boolean FALSE if there
 *   was none to extract.
 */
function islandora_endnotexml_extract_first(DOMDocument $endnote) {
  $xpath = new DOMXPath($endnote);
  $results = $xpath->query('/xml/records/record');
  $documents = array();
  if ($results->length >= 1) {
    // Get Record.
    $child = $results->item(0);
    $record = '<xml><records>' . $endnote->saveXML($child) . '</records></xml>';
    // Remove Record.
    $child->parentNode->removeChild($child);
    return $record;
  }
  return FALSE;
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
function islandora_endnotexml_get_mods($file) {
  $doc = new DOMDocument();
  $doc->load($file->uri);
  $endnotexml_record = islandora_endnotexml_extract_first($doc);
  return $endnotexml_record ?
    islandora_endnotexml_transform_to_mods($endnotexml_record) :
    FALSE;
}
