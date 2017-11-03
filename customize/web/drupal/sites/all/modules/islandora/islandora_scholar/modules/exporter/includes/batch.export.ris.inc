<?php

/**
 * @file
 * Functions for generating a batch export of citations to a single RIS file.
 */

/**
 * Gets the operations required to batch export citations to an RIS file.
 *
 * @param array $pids
 *   The citation objects to convert into an RIS file.
 *
 * @return array
 *   The operations required to generate a RIS file from the given citations.
 */
function citation_exporter_batch_get_ris_operations(array $pids) {
  module_load_include('inc', 'citation_exporter', 'includes/db');
  $temp_file_dir = file_directory_temp();
  $temp_mods_file = file_create_filename("mods.xml", $temp_file_dir);
  $set_id = citation_exporter_create_set($pids);
  $total = count($pids);

  $queries = $_GET;
  unset($queries['q']);
  $operations = array();
  $operations[] = array(
    'citation_exporter_batch_combine_mods',
    array($set_id, $total, $temp_mods_file),
  );
  $operations[] = array(
    'citation_exporter_batch_convert_mods_to_ris',
    array($temp_mods_file),
  );
  $operations[] = array(
    'citation_exporter_batch_redirect',
    array(
      url(request_path(), array('query' => $queries)),
      array($temp_mods_file),
      $set_id,
    ),
  );
  return $operations;
}

/**
 * Combined MODS batch operation.
 */
function citation_exporter_batch_combine_mods($set_id, $total, $mods_filename, array &$context) {
  module_load_include('inc', 'citation_exporter', 'includes/db');
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = $total;
    $context['sandbox']['mods_fragment'] = file_create_filename('mods-fragment', 'temporary://');
  }

  $pids = citation_exporter_get_pids($set_id);
  _citation_exporter_batch_combine_mods($context['sandbox']['mods_fragment'], $pids);
  citation_exporter_drop_pids($set_id, $pids);
  $context['sandbox']['progress'] += count($pids);

  if ($context['sandbox']['progress'] < $context['sandbox']['total']) {
    $context['message'] = t('Gathered metadata: @progress/@total', array(
      '@progress' => $context['sandbox']['progress'],
      '@total' => $context['sandbox']['total'],
    ));
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['total'];
  }
  else {
    $mods = fopen($mods_filename, 'wb');
    $fragment = fopen($context['sandbox']['mods_fragment'], 'rb');

    fwrite($mods, '<?xml version="1.0" encoding="UTF-8"?>');
    fwrite($mods, '<modsCollection xmlns="http://www.loc.gov/mods/v3">');
    stream_copy_to_stream($fragment, $mods);
    fwrite($mods, '</modsCollection>');
    fclose($mods);
    fclose($fragment);
    file_unmanaged_delete($context['sandbox']['mods_fragment']);
  }
}

/**
 * Fetch the MODS datastreams from the given $pids and combined them.
 */
function _citation_exporter_batch_combine_mods($filename, array $pids) {
  $fp = fopen($filename, 'ab');
  foreach ($pids as $pid) {
    $object = islandora_object_load($pid);
    if (isset($object) && isset($object['MODS']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['MODS'])) {
      $datastream = trim($object['MODS']->content);
      if (!empty($datastream)) {
        $doc = new DOMDocument();
        $doc->loadXML($datastream);
        fwrite($fp, $doc->saveXML($doc->documentElement));
      }
    }
  }
  fclose($fp);
}

/**
 * Converts the given MODS file to RIS using bibutils.
 *
 * @param string $mods_filename
 *   The name of the MODS file to convert to RIS.
 * @param array $context
 *   The batch processes context.
 */
function citation_exporter_batch_convert_mods_to_ris($mods_filename, array &$context) {
  module_load_include('inc', 'bibutils', 'includes/utilities');
  $temp_file_dir = file_directory_temp();
  $ris_filename = file_create_filename("ris_export.ris", $temp_file_dir);
  if (Bibutils::Convert($mods_filename, 'MODS', $ris_filename, 'RIS')) {
    $context['results']['filename'] = $ris_filename;
    $context['results']['mime_type'] = 'application/x-Research-Info-Systems';
    $context['results']['download_filename'] = 'export.ris';
  }
}
