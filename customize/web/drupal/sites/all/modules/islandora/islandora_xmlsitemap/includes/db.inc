<?php

/**
 * @file
 * Functionality that touches the database.
 */

/**
 * Inserts a record into the autoincrementing ID table.
 *
 * @param string $value
 *   The value to insert into the 'pid' column.
 */
function islandora_xmlsitemap_insert_increment_record($value) {
  db_insert('islandora_xmlsitemap_entry_ids')
    ->fields(array('pid' => $value))
    ->execute();
}

/**
 * Deletes a record from the autoincrementing ID table.
 *
 * @param string $value
 *   The value to match in the 'pid' column for record deletion.
 */
function islandora_xmlsitemap_delete_increment_record($value) {
  db_delete('islandora_xmlsitemap_entry_ids')
    ->condition('pid', $value)
    ->execute();
}

/**
 * Gets a record from the autoincrementing ID table.
 *
 * @param string $value
 *   The record to get an autoincrementing ID for.
 *
 * @return int
 *   The ID for that record.
 */
function islandora_xmlsitemap_get_id_for_record($value) {
  $id = db_select('islandora_xmlsitemap_entry_ids', 'x')
    ->fields('x', array('id'))
    ->condition('pid', $value)
    ->execute()
    ->fetchField();
  return $id;
}
