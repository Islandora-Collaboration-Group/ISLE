<?php

/**
 * @file
 * Islandora Compound Object Drush
 * Drush statements to update a compound objects relations with children
 * from 'isPartOf' to 'isConstituentOf'.
 *
 * Hook implementations and general functions.
 */

/**
 * Implements hook_drush_help().
 */
function islandora_webform_ingest_drush_help($section) {
  switch ($section) {
    case 'drush:iwi-repair-relations':
      return dt("repair relationship predicate on objects that are members of collections and retroactively add tracking tags to objects ingested using islandora_webform_ingest RC1.");
  }
}

/**
 * Set up the drush command.
 *
 * @return array
 *   The drush command in an array.
 */
function islandora_webform_ingest_drush_command() {
  $items = array();
  $items['iwi-repair-relations'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    'callback' => 'islandora_webform_ingest_setup_batch',
    'description' => dt('Repair repository relations impacted by RC1 version of the iwlandora_webform_ingest module'),
    'aliases' => array('iwirr'),
  );
  return $items;
}

/**
 * Call both repair phases.
 */
function islandora_webform_ingest_setup_batch() {
  module_load_include('inc', 'islandora_webform_ingest', 'includes/fix_ingest_relations');
  iwi_retroactive_fix_all_collection_predicates();
  iwi_retroactive_fix_all_iw_relation_tags();

  $batch = &batch_get();
  $batch['progressive'] = FALSE;
  drush_backend_batch_process();

}
