<?php

/**
 * @file
 * Database interface.
 */

/**
 * Stash a set of PIDs in the database.
 *
 * @param array $pids
 *   An array of PIDs to stash.
 *
 * @return string
 *   The ID of the set in the database, from which the PIDs can be
 *   retrieved.
 */
function citation_exporter_create_set(array $pids = array()) {
  $id = db_insert('citation_exporter_sets')
    ->fields(array('updated' => time()))
    ->execute();

  foreach (array_chunk($pids, 1024) as $chunk) {
    $insert = db_insert('citation_exporter_pids')
      ->fields(array('sid', 'pid'));
    foreach ($chunk as $pid) {
      $insert->values(array(
        'sid' => $id,
        'pid' => $pid,
      ));
    }
    $insert->execute();
  }

  return $id;
}

/**
 * Get PIDs from a set.
 *
 * @param string $set_id
 *   The set from which to get PIDs.
 * @param int $count
 *   The number of PIDs to get.
 *
 * @return array
 *   An array of PIDs. There may be fewer than $count.
 */
function citation_exporter_get_pids($set_id, $count = 32) {
  db_update('citation_exporter_sets')
    ->fields(array(
      'updated' => time(),
    ))
    ->condition('id', $set_id)
    ->execute();
  return db_select('citation_exporter_pids', 'p')
    ->fields('p', array('pid'))
    ->condition('sid', $set_id)
    ->range(0, $count)
    ->execute()
    ->fetchCol();
}

/**
 * Drop the given set from the database.
 *
 * @param string $set_id
 *   The ID of the set to delete.
 */
function citation_exporter_drop_set($set_id) {
  db_delete('citation_exporter_pids')
    ->condition('sid', $set_id)
    ->execute();
  db_delete('citation_exporter_sets')
    ->condition('id', $set_id)
    ->execute();
}

/**
 * Remove PIDs from a set.
 *
 * @param string $set_id
 *   The set from which to remove the PIDs.
 * @param array $pids
 *   An array of PIDs to remove from the set.
 */
function citation_exporter_drop_pids($set_id, array $pids) {
  db_update('citation_exporter_sets')
    ->fields(array(
      'updated' => time(),
    ))
    ->condition('id', $set_id)
    ->execute();
  db_delete('citation_exporter_pids')
    ->condition('sid', $set_id)
    ->condition('pid', $pids)
    ->execute();
}
