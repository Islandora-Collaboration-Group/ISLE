<?php

/**
 * @file
 * Drush integration for islandora_openseadragon.
 */

/**
 * The Openseadragon plugin URI.
 */
define('OPENSEADRAGON_DOWNLOAD_URI', 'https://github.com/openseadragon/openseadragon/releases/download/v2.2.1/openseadragon-bin-2.2.1.zip');

/**
 * The original OpenSeadragon directory.
 */
define('OPENSEADRAGON_ORIGINAL_DIR', 'openseadragon-bin-2.2.1');

/**
 * Implements hook_drush_command().
 */
function islandora_openseadragon_drush_command() {
  $items = array();
  // The key in the $items array is the name of the command.
  $items['openseadragon-plugin'] = array(
    'callback' => 'drush_islandora_openseadragon_plugin',
    'description' => dt('Download and install the Openseadragon plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Openseadragon plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('openseadragonplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function islandora_openseadragon_drush_help($section) {
  switch ($section) {
    case 'drush:openseadragon-plugin':
      return dt('Download and install the Openseadragon plugin, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Openseadragon plugin.
 */
function drush_islandora_openseadragon_plugin() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = _drush_core_directory("@site:sites/all/libraries");
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Download the zip archive.
  if ($filepath = drush_download_file(OPENSEADRAGON_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = OPENSEADRAGON_ORIGINAL_DIR;

    // Remove any existing Openseadragon plugin directory.
    if (is_dir($dirname) || is_dir('openseadragon')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('openseadragon', TRUE);
      drush_log(dt('A existing Openseadragon plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "openseadragon" if needed.
    if ($dirname != 'openseadragon') {
      drush_move_dir($dirname, 'openseadragon', TRUE);
      $dirname = 'openseadragon';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Openseadragon plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Openseadragon plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
