<?php

/**
 * @file
 * Expose views provided in code to the views module.
 */

/**
 * Implements hook_views_default_views().
 */
function islandora_sync_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'islandora_sync') . '/views', '/\.view/');
  foreach ($files as $absolute => $file) {
    require $absolute;
    if (isset($view)) {
      $views[$file->name] = $view;
    }
  }
  return $views;
}
