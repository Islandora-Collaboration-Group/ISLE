<?php

/**
 * @file
 * DB access/management functions.
 */

/**
 * Get populators.
 *
 * @param string|array $id
 *   An optional string or array of IDs to restrict the list of populators
 *   returned.
 *
 * @return array
 *   An associative array mapping the numerical populator ID to an associative
 *   mapping out the fields from the islandora_xslt_populator database table.
 */
function islandora_xslt_populator_get_populators($id = NULL) {
  $query = db_select('islandora_xslt_populator', 'p')
    ->fields('p', array());
  if ($id !== NULL) {
    $query->condition('id', $id);
  }

  $result = $query->execute();
  return $result->fetchAllAssoc('id', PDO::FETCH_ASSOC);
}

/**
 * Remove populator(s).
 *
 * Remove from the DB and remove the file usage entries.
 *
 * @param string|array $ids
 *   Either a single ID or an array of IDs to remove.
 */
function islandora_xslt_populator_remove_populator($ids) {
  module_load_include('inc', 'islandora_xslt_populator', 'includes/utilities');
  $populators = islandora_xslt_populator_get_populators($ids);
  foreach ($populators as $id => $info) {
    $file = file_load($info['xslt']);
    db_delete('islandora_xslt_populator')
      ->condition('id', $id)
      ->execute();
    file_usage_delete($file, 'islandora_xslt_populator', 'populator', $id);
    $file->status &= ~FILE_STATUS_PERMANENT;
    file_save($file);
    file_delete($file);
  }
}

/**
 * Add populator.
 *
 * Adds to the DB and registered the file usage.
 *
 * @param string $title
 *   The title of the populator.
 * @param string $description
 *   The description for the populator.
 * @param int|string $xslt
 *   The file ID of the XSLT.
 * @param string $datastream_id
 *   The datastream ID into which the output should be stored.
 * @param string $machine_name
 *   A name for the populator, to identify in features.
 * @param NULL|string $original_dsid
 *   NULL if not storing the original file, string of the DSID to be saved into
 *   otherwise.
 *
 * @return int
 *   The ID of the populator as it was entered into the database.
 */
function islandora_xslt_populator_add_populator($title, $description, $xslt, $datastream_id, $machine_name = NULL, $original_dsid = NULL) {
  $id = db_insert('islandora_xslt_populator')
    ->fields(array(
      'title' => $title,
      'description' => $description,
      'xslt' => $xslt,
      'datastream_id' => $datastream_id,
      'machine_name' => $machine_name,
      'original_dsid' => $original_dsid,
    ))
    ->execute();

  $file = file_load($xslt);
  file_usage_add($file, 'islandora_xslt_populator', 'populator', $id);
  $file->status |= FILE_STATUS_PERMANENT;
  file_save($file);

  return $id;
}

/**
 * Get XSLT populators with machine names.
 *
 * @return array
 *   An associative array mapping machine names to associative arrays of
 *   populator info.
 */
function islandora_xslt_populator_get_named_populators() {
  $all_populators = islandora_xslt_populator_get_populators();
  $populators = array();
  foreach ($all_populators as $id => $info) {
    if (isset($info['machine_name'])) {
      $populators[$info['machine_name']] = $info;
    }
  }
  return $populators;
}
