<?php

/**
 * @file
 * Administration form.
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
function bibutils_admin_form(array $form, array &$form_state) {
  $form['bibutils_endx2xml_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Endnote XML to MODS executable'),
    '#description' => t('The path to endx2xml.'),
    '#default_value' => variable_get('bibutils_endx2xml_path', 'endx2xml'),
  );
  $form['bibutils_ris2xml_path'] = array(
    '#type' => 'textfield',
    '#title' => t('RIS to MODS executable'),
    '#description' => t('The path to ris2xml.'),
    '#default_value' => variable_get('bibutils_ris2xml_path', 'ris2xml'),
  );

  return system_settings_form($form);
}
