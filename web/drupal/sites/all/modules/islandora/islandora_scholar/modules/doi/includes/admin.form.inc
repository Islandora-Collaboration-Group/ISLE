<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_doi_admin_form(array $form, array &$form_state) {
  $form += array(
    'islandora_doi_openurl' => array(
      '#type' => 'textfield',
      '#title' => t('DOI OpenURL'),
      '#description' => t('The URL which we will query to obtain our CrossREF information.'),
      '#default_value' => variable_get('islandora_doi_openurl', 'http://www.crossref.org/openurl'),
    ),
    'islandora_doi_openurl_pid' => array(
      '#type' => 'textfield',
      '#title' => t('OpenURL PID'),
      '#description' => t('An identifier to call yourself, for the OpenURL endpoint. To use this service you first need to register for an account here: http://www.crossref.org/requestaccount/'),
      '#default_value' => variable_get('islandora_doi_openurl_pid', 'user@example.com'),
    ),
  );

  return system_settings_form($form);
}
