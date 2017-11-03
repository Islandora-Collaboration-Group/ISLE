<?php

/**
 * @file
 * The admin form for the islandora checksum module.
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
function islandora_checksum_admin_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };

  $collections = islandora_checksum_get_collections();

  $form['islandora_checksum_checksum_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checksum configuration'),
  );

  $form['islandora_checksum_checksum_configuration']['islandora_checksum_enable_checksum'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable checksum'),
    '#description' => t('Enables checksum creation on datastreams.'),
    '#default_value' => variable_get('islandora_checksum_enable_checksum', FALSE),
  );

  $form['islandora_checksum_checksum_configuration']['islandora_checksum_checksum_type'] = array(
    '#type' => 'fieldset',
    '#type' => 'select',
    '#title' => t('Checksum type'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_checksum_enable_checksum"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('islandora_checksum_checksum_type', 'DISABLED'),
    '#multiple' => FALSE,
    '#options' => array(
      'DISABLED' => t('DISABLED'),
      'MD5' => t('MD5'),
      'SHA-1' => t('SHA-1'),
      'SHA-256' => t('SHA-256'),
      'SHA-384' => t('SHA-384'),
      'SHA-512' => t('SHA-512'),
    ),
  );

  $form['islandora_checksum_checksum_configuration']['islandora_checksum_dsids_to_check'] = array(
    '#title' => t('Datastreams to Checksum'),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('islandora_checksum_dsids_to_check', ''),
    '#description' => t("A comma-separated list of DSIDs. Leave empty to checksum all datastreams."),
    '#maxlength' => 255,
  );

  $form['islandora_checksum_checksum_configuration']['islandora_checksum_deriv_regeneration_bypass'] = array(
    '#title' => t('Bypass derivative regeneration when updating only checksums'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('islandora_checksum_deriv_regeneration_bypass', FALSE),
    '#description' => t('Note that this setting will need to be saved first before attempting to retroactively enable checksums using the controls below.'),
  );

  $form['islandora_checksum_checksum_retro_enable'] = array(
    '#type' => 'fieldset',
    '#title' => t('Apply or re-apply checksums'),
    '#description' => t('Checksums will be applied to datastreams without checksums, or re-applied to datastreams whose checksumType does not match the site configured checksum type. Derivatives will be generated unless the above bypass has been enabled previously.'),
  );
  $form['islandora_checksum_checksum_retro_enable']['collection'] = array(
    '#type' => 'select',
    '#title' => t('Collection'),
    '#options' => $collections,
    '#description' => t('Select a collection to apply or re-apply checksums on.'),
  );
  $form['islandora_checksum_checksum_retro_enable']['islandora_checksum_log_retro_errors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log errors'),
    '#default_value' => variable_get('islandora_checksum_log_retro_errors', 1),
    '#description' => t("Log errors to watchdog."),
  );
  $form['islandora_checksum_checksum_retro_enable']['apply_batch'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('islandora_checksum_apply_retro'),
  );
  $form['islandora_checksum_checksum_retro_enable']['contact_information'] = array(
    '#markup' => t('This may take a while. Please be patient.'),
  );
  return system_settings_form($form);
}
