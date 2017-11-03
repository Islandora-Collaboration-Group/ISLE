<?php

/**
 * @file
 * drush integration for islandora_pdfjs.
 */

/**
 * The PDF.js plugin URI.
 */
define('PDFJS_DOWNLOAD_URI', 'https://github.com/mozilla/pdf.js/releases/download/v1.4.20/pdfjs-1.4.20-dist.zip');


/**
 * The initial PDF.js directory
 */
define('PDFJS_ORIGINAL_DIR', 'pdfjs');

/**
 * Implements hook_drush_command().
 */
function islandora_pdfjs_drush_command() {
  $items = array();
  // The key in the $items array is the name of the command.
  $items['pdfjs-plugin'] = array(
    'callback' => 'drush_islandora_pdfjs_plugin',
    'description' => dt('Download and install the PDF.js plugin.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the PDF.js plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('pdfjsplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function islandora_pdfjs_drush_help($section) {
  switch ($section) {
    case 'drush:pdfjs-plugin':
      return dt('Download and install the PDF.js plugin, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the PDF.js plugin.
 */
function drush_islandora_pdfjs_plugin() {
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
  if ($filepath = drush_download_file(PDFJS_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = PDFJS_ORIGINAL_DIR;
    $pathdir = $path . '/' . $dirname;

    // Remove any existing PDF.js plugin directory.
    if (is_dir($dirname) || is_dir('pdfjs')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('pdfjs', TRUE);
      drush_log(dt('A existing PDF.js plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename, $pathdir);

    // Change the directory name to "pdfjs" if needed.
    if ($dirname != 'pdfjs') {
      drush_move_dir($dirname, 'pdfjs', TRUE);
      $dirname = 'pdfjs';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('PDF.js plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the PDF.js plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
