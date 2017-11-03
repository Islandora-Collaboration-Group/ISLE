<?php

/**
 * @file
 * Functions for generating a batch export of citations to a single PDF file.
 */

module_load_include('inc', 'citation_exporter', 'includes/batch.export.rtf');

/**
 * Get the batch operations required to generate an PDF file.
 *
 * @param array $pids
 *   The citation to export to the PDF file.
 * @param sting $style
 *   The style to apply to the citation before exporting to the PDF.
 *
 * @return array
 *   The batch operations to create the PDF file.
 */
function citation_exporter_batch_get_pdf_operations(array $pids, $style) {
  $temp_file_dir = file_directory_temp();
  $temp_text_file = file_create_filename("pdf_export.txt", $temp_file_dir);
  module_load_include('inc', 'citation_exporter', 'includes/db');
  $set_id = citation_exporter_create_set($pids);
  $total = count($pids);
  $operations = array();
  $queries = $_GET;
  unset($queries['q']);
  $operations[] = array(
    'citation_exporter_batch_append_citations_to_text_file',
    array($set_id, $total, $temp_text_file, $style),
  );
  $operations[] = array('citation_exporter_batch_convert_text_to_pdf', array($temp_text_file));
  $operations[] = array(
    'citation_exporter_batch_redirect',
    array(
      url(request_path(), array('query' => $queries)),
      array($temp_text_file),
      $set_id,
    ),
  );
  return $operations;
}

/**
 * Converts an RTF document to a PDF.
 *
 * We can't directly generate the PDF so this is a good work around.
 *
 * @param array $context
 *   The batch context.
 */
function citation_exporter_batch_convert_text_to_pdf($filename, array &$context) {
  module_load_include('inc', 'citation_exporter', 'includes/pdf');
  $text = file_get_contents($filename);
  // XXX: Strip out tags.
  $text = filter_xss($text, array('br', 'i', 'b'));
  $temp_file_dir = file_directory_temp();
  $temp_file = file_create_filename("pdf_export.pdf", $temp_file_dir);
  citation_exporter_create_pdf($text, $temp_file);
  $context['results']['filename'] = $temp_file;
  $context['results']['download_filename'] = 'export.pdf';
  $context['results']['mime_type'] = 'application/pdf';
}
