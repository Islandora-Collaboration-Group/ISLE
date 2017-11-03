<?php

/**
 * @file
 * Plugin for the Islandora BagIt Drupal module.
 *
 * Illustrates how to add a file to the Bag that is not a datastream.
 * In this case, the file is called 'foo.xml'.
 *
 * Also registers all the datastreams in an Islandora object so they are
 * copied into the 'data' directory.
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
function islandora_bagit_plugin_object_ds_add_file_init($islandora_object, $tmp_ds_directory) {
  $files_to_add = array();
  $ds_files = islandora_bagit_retrieve_datastreams($islandora_object, $tmp_ds_directory);

  // Generate a file called foo.xml and add it to the list of files that
  // should be added to the Bag. Adding this extra file is the only
  // difference between this plugin and plugin_copy_ds_basic.inc.
  $foo_file_contents = islandora_bagit_generate_foo_xml($ds_files);
  $foo_file_path = $tmp_ds_directory . DIRECTORY_SEPARATOR . 'foo.xml';
  file_put_contents($foo_file_path, $foo_file_contents);
  $files_to_add[] = array(
    'source' => $foo_file_path,
    'dest' => 'foo.xml',
  );

  // Add file source and dest paths for each datastream to the $files_to_add
  // array. $files_to_add['dest'] must be relative to the Bag's data
  // subdirectory.
  foreach ($ds_files as $ds_filename) {
    // Add each file in the directory to $files_to_add.
    $source_file_to_add = $ds_filename;
    if (file_exists($source_file_to_add) && is_file($source_file_to_add)) {
      $files_to_add[] = array(
        'source' => $source_file_to_add,
        // We use filename here since there are no subdirectories in in the
        // Bag's 'data' directory.
        'dest' => basename($ds_filename),
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

/**
 * Generates the sample file we are including in the Bag.
 *
 * Uses XMLWriter to generate foo.xml, which contains a simple
 * list of all the datastream files.
 *
 * @param array $file_list
 *   List of datastream files.
 *
 * @return string
 *   The serialized XML file.
 */
function islandora_bagit_generate_foo_xml($file_list) {
  $foo = new XMLWriter();
  $foo->openMemory();
  $foo->setIndent(TRUE);
  $foo->startDocument('1.0', 'utf-8', 'yes');
  $foo->writeComment('This is just an example file.');
  $foo->startElement('files');
  foreach ($file_list as $filename) {
    $foo->writeElement('file', basename($filename));
  }
  $foo->endElement();
  $foo->endDocument();
  return $foo->outputMemory();
}
