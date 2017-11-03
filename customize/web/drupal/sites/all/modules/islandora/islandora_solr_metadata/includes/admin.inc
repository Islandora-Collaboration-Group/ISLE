<?php

/**
 * @file
 * Contains page callback for admin screen.
 */

/**
 * Page callback for admin screen.
 *
 * Vertical tabs containing multiple forms.
 */
function islandora_solr_metadata_admin_page_callback() {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/config.admin');
  module_load_include('inc', 'islandora_solr_metadata', 'includes/general.admin');
  return array(
    'tabset' => array(
      '#type' => 'vertical_tabs',
      'field_config' => array(
        '#type' => 'fieldset',
        '#title' => t('Field Configuration'),
        '#group' => 'tabset',
        'form' => drupal_get_form('islandora_solr_metadata_admin_form'),
      ),
      'general_config' => array(
        '#type' => 'fieldset',
        '#title' => t('General Configuration'),
        '#group' => 'tabset',
        'form' => drupal_get_form('islandora_solr_metadata_general_admin_form'),
      ),
    ),
  );
}
