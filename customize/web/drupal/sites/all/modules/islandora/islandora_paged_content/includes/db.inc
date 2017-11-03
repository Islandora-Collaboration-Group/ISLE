<?php
/**
 * @file
 * Database interaction helpers.
 */

/**
 * Keeps track of a PDF to be deleted down the road.
 *
 * @param string $pid
 *   The PID of the object being ingested.
 *
 * @param int $fid
 *   The ID of the source PDF file being uploaded.
 */
function islandora_paged_content_track_pdf_ingestion($pid, $fid) {
  db_insert('islandora_paged_content_pdf_ingest')
    ->fields(array('pid' => $pid, 'fid' => $fid))
    ->execute();
}

/**
 * Deletes the associated PDF source file if it exists.
 *
 * @param string $pid
 *   The PID of the object that was ingested.
 */
function islandora_paged_content_cleanup_source_pdf($pid) {
  $result = db_select('islandora_paged_content_pdf_ingest', 'i')
    ->fields('i', array('fid'))
    ->condition('pid', $pid)
    ->execute();
  $record = $result->fetchAssoc();
  if (isset($record['fid'])) {
    $file = file_load($record['fid']);
    file_delete($file);
    db_delete('islandora_paged_content_pdf_ingest')
      ->condition('pid', $pid)
      ->execute();
  }
}
