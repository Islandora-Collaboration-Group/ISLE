<?php
/**
 * @file
 * Houses the confirm_form for the islandora_solr_metadata configuration form.
 */

/**
 * The delete confirm_form for the islandora_solr_metadata module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param int $configuration_id
 *   The ID of the configuration we are currently editing.
 *
 * @return array
 *   An array representing the configuration form.
 */
function islandora_solr_metadata_delete_config_form($form, &$form_state, $configuration_id) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  $form_state['configuration_id'] = $configuration_id;
  $configuration_name = islandora_solr_metadata_retrieve_configuration_name($configuration_id);
  $form_state['configuration_name'] = $configuration_name;
  return confirm_form($form,
    t('Are you sure you want to delete the @configuration_name display configuration?', array('@configuration_name' => $configuration_name)),
    "admin/islandora/search/islandora_solr_metadata/config/$configuration_id",
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for the islandora_solr_metadata delete confirm_form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_solr_metadata_delete_config_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  islandora_solr_metadata_delete_configuration($form_state['configuration_id']);
  $form_state['redirect'] = 'admin/islandora/search/islandora_solr/metadata';
  drupal_set_message(t('The @configuration_name display configuration has been deleted!', array('@configuration_name' => $form_state['configuration_name'])));
}
