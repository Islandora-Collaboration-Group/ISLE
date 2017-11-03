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
function zip_importer_admin_settings_form(array $form, array &$form_state) {
  $form = array(
    'zip_importer_use_filenames_as_labels' => array(
      '#type' => 'checkbox',
      '#title' => t('Use filename as Datastream label'),
      '#description' => t('If set uploaded files will use their filename as the label of the datastream they represent. Note that some files that get transformed like MARC to MODS will be ignored.'),
      '#default_value' => variable_get('zip_importer_use_filenames_as_labels', FALSE),
    ),
  );
  return system_settings_form($form);
}
