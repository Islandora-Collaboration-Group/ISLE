<?php

/**
 * @file
 * drush integration for islandora_videojs.
 */

/**
 * The Video.js plugin URI.
 */
define('VIDEOJS_DOWNLOAD_URI', 'https://github.com/videojs/video.js/releases/download/v5.10.2/video-js-5.10.2.zip');
/**
 * The initial Video.js directory
 */
define('VIDEOJS_ORIGINAL_DIR', 'video-js');

/**
 * Implements hook_drush_command().
 */
function islandora_videojs_drush_command() {
  $items = array();
  // The key in the $items array is the name of the command.
  $items['videojs-plugin'] = array(
    'callback' => 'drush_islandora_videojs_plugin',
    'description' => dt('Download and install the Video.js plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Video.js plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('videojsplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function islandora_videojs_drush_help($section) {
  switch ($section) {
    case 'drush:videojs-plugin':
      return dt('Download and install the Video.js plugin, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the Video.js plugin.
 */
function drush_islandora_videojs_plugin() {
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
  if ($filepath = drush_download_file(VIDEOJS_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = VIDEOJS_ORIGINAL_DIR;

    // Remove any existing Video.js plugin directory.
    if (is_dir($dirname) || is_dir('video-js')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('video-js', TRUE);
      drush_log(dt('A existing Video.js plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename, 'video-js');

    // Change the directory name to "video-js" if needed.
    if ($dirname != 'video-js') {
      drush_move_dir($dirname, 'video-js', TRUE);
      $dirname = 'video-js';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Video.js plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the Video.js plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
