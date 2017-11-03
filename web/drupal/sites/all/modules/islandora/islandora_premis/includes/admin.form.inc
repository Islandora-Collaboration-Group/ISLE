<?php

/**
 * @file
 * The admin form for the islandora premis module.
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
function islandora_premis_admin_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };

  $form['islandora_premis_agent_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Organization Agent configuration'),
  );

  $form['islandora_premis_agent_configuration']['islandora_premis_agent_name'] = array(
    '#type' => 'textfield',
    '#title' => t('agentName'),
    '#description' => t('Sample: York University Library'),
    '#default_value' => variable_get('islandora_premis_agent_name', 'Your Organization'),
  );

  $form['islandora_premis_agent_configuration']['islandora_premis_agent_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('agentIdentifier'),
    '#description' => t('Sample: http://library.yorku.ca'),
    '#default_value' => variable_get('islandora_premis_agent_identifier', 'Your_Organization'),
  );

  $form['islandora_premis_agent_configuration']['islandora_premis_agent_identifier_type'] = array(
    '#type' => 'textfield',
    '#title' => t('agentIdentifierType'),
    '#description' => t('Sample: MARC Organization Codes'),
    '#default_value' => variable_get('islandora_premis_agent_identifier_type', 'MARC Organization Codes'),
    '#description' => t('See PREMIS 3.1.1. for usage.'),
  );

  $form['islandora_premis_agent_configuration']['islandora_premis_agent_type'] = array(
    '#type' => 'textfield',
    '#title' => t('agentType'),
    '#description' => t('Sample: organization'),
    '#default_value' => variable_get('islandora_premis_agent_type', 'organization'),
  );

  return system_settings_form($form);
}
