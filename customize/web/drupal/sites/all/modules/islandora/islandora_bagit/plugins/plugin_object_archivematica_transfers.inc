<?php

/**
 * @file
 * Plugin for the Islandora BagIt Drupal module.
 *
 * Creates Bags compatible with Archivematica (http://archivematica.org)
 * transfer Bags by creating 'metadata' and 'objects' subdirectories, then
 * moving the DC datastream into the 'metadata' directory and the OBJ datastream
 * into the 'objects' directory.
 */

/**
 * Returns an array of source and destination file paths.
 *
 * @param object $islandora_object
 *   The Islandora object to create a Bag for.
 *
 * @param string $tmp_ds_directory
 *   The temporary directory where the datastream files have been downloaded.
 *
 * @return array|bool
 *   An array of source and destination file paths, or FALSE
 *   if no datastream files are present.
 */
function islandora_bagit_plugin_object_archivematica_transfers_init($islandora_object, $tmp_ds_directory) {
  $files_to_add = array();
  // In this plugin, we only want the DC and OBJ datastreams.
  $ds_files = islandora_bagit_retrieve_datastreams($islandora_object, $tmp_ds_directory,
    array('OBJ', 'DC'));

  // Create a subdirectory in $tmp_ds_directory.
  mkdir($tmp_ds_directory . '/metadata', 0777, TRUE);
  mkdir($tmp_ds_directory . '/objects', 0777, TRUE);
  // Move the DC datastream into this subdirectory and add the moved file to
  // $files_to_add.
  rename($tmp_ds_directory . '/DC.xml', $tmp_ds_directory . '/metadata/dublincore.xml');
  $files_to_add[] = array(
    'source' => $tmp_ds_directory . '/metadata/dublincore.xml',
    'dest' => 'metadata/dublincore.xml',
  );

  // Loop through all the other files and move them into the 'objects'
  // subdirectory.
  foreach ($ds_files as $ds_filename) {
    // $ds_file = $tmp_ds_directory . DIRECTORY_SEPARATOR . $ds_filename;
    if (file_exists($ds_filename) && is_file($ds_filename)) {
      rename($ds_filename, $tmp_ds_directory . '/objects/' . basename($ds_filename));
      // Add each file in the directory to $files_to_add.
      $files_to_add[] = array(
        'source' => $tmp_ds_directory . '/objects/' . basename($ds_filename),
        'dest' => 'objects/' . basename($ds_filename),
      );
    }
  }

  if (count($files_to_add)) {
    return $files_to_add;
  }
  else {
    return FALSE;
  }
}
