<?php

/**
 * @file
 * Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_basic_collection_drush_command() {
  return array(
    'islandora-basic-collection-generate-thumbs-from-children' => array(
      'description' => dt('Recursively grabs a thumbnail from a collections children for its thumbnail.'),
      'examples' => array('drush -u 1 islandora-basic-collection-generate-thumbs-from-children --collection=islandora:root'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
      'drupal dependencies' => array(
        'islandora',
        'islandora_basic_collection',
      ),
      'options' => array(
        'collection' => array(
          'description' => dt('Where to start creating thumbnails.'),
          'required' => TRUE,
        ),
      ),
    ),
  );
}

/**
 * Drush command for islandora-basic-collection-generate-thumbs-from-children.
 */
function drush_islandora_basic_collection_generate_thumbs_from_children() {
  module_load_include('inc', 'islandora_basic_collection', 'includes/thumbs.batch');
  batch_set(islandora_basic_collection_thumbs_batch(drush_get_option('collection')));
  drush_backend_batch_process();
}
