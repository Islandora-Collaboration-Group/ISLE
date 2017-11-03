<?php

/**
 * @file
 * Houses the administration for the islandora_oai module.
 */

/**
 * Creates the administration form for the islandora_oai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the islandora_oai administration form.
 */
function islandora_oai_admin_form($form, $form_state) {
  global $base_url;
  $form = array(
    '#tree' => TRUE,
  );
  $form['islandora_oai_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
  );
  $form['islandora_oai_configuration']['islandora_oai_repository_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository Name'),
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_repository_name', 'Repository'),
  );
  $form['islandora_oai_configuration']['islandora_oai_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to the Repository'),
    '#field_prefix' => $base_url . '/',
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_path', 'oai2'),
    '#description' => t('The path where the OAI-PMH service will respond, e.g. @base_url/oai2', array('@base_url' => $base_url)),
  );
  $form['islandora_oai_configuration']['islandora_oai_repository_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository unique identifier'),
    '#required' => TRUE,
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_repository_identifier', 'drupal-site.org'),
    '#description' => t('The identifier for this repository, e.g. oai:<strong>drupal-site.org</strong>:123.'),
  );
  $form['islandora_oai_configuration']['islandora_oai_admin_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator Email'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_admin_email', 'N/A'),
  );
  $form['islandora_oai_configuration']['islandora_oai_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Response Size'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_max_size', '20'),
    '#description' => t('The maximum number of records to issue per response. If the result set is larger than this number, a resumption token will be issued'),
  );
  $form['islandora_oai_configuration']['islandora_oai_expire_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Time'),
    '#size' => '50',
    '#default_value' => variable_get('islandora_oai_expire_time', '86400'),
    '#description' => t('The amount of time a resumption token will remain valid, in seconds. Defaults to one day (86400s).'),
  );
  $form['islandora_oai_configuration']['islandora_oai_query_backend'] = array(
    '#type' => 'radios',
    '#title' => t('Query Backend'),
    '#default_value' => variable_get('islandora_oai_query_backend', 'sparql'),
    '#description' => t('For larger repositories, OAI may perform poorly when attempting to perform the SPARQL queries it requires. In these cases, using the Solr backend may provide better results.'),
    '#options' => array(
      'sparql' => t('SPARQL'),
      'solr' => t('Solr'),
    ),
  );
  $solr_config_states = array(
    'visible' => array(
      ':input[name="islandora_oai_configuration[islandora_oai_query_backend]"]' => array('value' => 'solr'),
    ),
  );
  $form['islandora_oai_configuration']['islandora_oai_solr_state_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Object State Field'),
    '#default_value' => variable_get('islandora_oai_solr_state_field', 'fgs_state_s'),
    '#description' => t("The field in Solr that holds a Fedora object's state ('Active', 'Inactive', or 'Deleted')."),
    '#states' => $solr_config_states,
  );
  $form['islandora_oai_configuration']['islandora_oai_solr_collection_description_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Collection Description Field'),
    '#default_value' => variable_get('islandora_oai_solr_collection_description_field', 'dc.description'),
    '#description' => t('The field in Solr to use for collection descriptions.'),
    '#states' => $solr_config_states,
  );
  $form['islandora_oai_configuration']['islandora_oai_solr_object_ancestors_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Object Ancestors Field'),
    '#default_value' => variable_get('islandora_oai_solr_object_ancestors_field', ''),
    '#description' => t("A multivalued string The field in Solr that defines an object's ancestors. If left blank, Solr will recurse manually to get the child tree of a particular set. Use of this field may return a different set of children than the recursive option; it is the responsibility of the implementer to ensure the ancestors field returns an appropriate hierarchy of parents."),
    '#states' => $solr_config_states,
  );

  // Build up the available request handlers.
  $defined_handlers = module_invoke_all(ISLANDORA_OAI_REQUEST_HANDLER_HOOK);
  if (!empty($defined_handlers)) {
    $form['islandora_oai_configuration']['handlers'] = array(
      '#type' => 'item',
      '#title' => t('Select an OAI request handler'),
      '#description' => t('Preferred OAI request handler for Islandora. These may be provided by third-party modules.'),
      '#tree' => TRUE,
      '#theme' => 'islandora_viewers_table',
    );
    foreach ($defined_handlers as $name => $profile) {
      $options[$name] = '';
      $form['islandora_oai_configuration']['handlers']['name'][$name] = array(
        '#type' => 'hidden',
        '#value' => $name,
      );
      $form['islandora_oai_configuration']['handlers']['label'][$name] = array(
        '#type' => 'item',
        '#markup' => $profile['label'],
      );
      $form['islandora_oai_configuration']['handlers']['description'][$name] = array(
        '#type' => 'item',
        '#markup' => $profile['description'],
      );
      $form['islandora_oai_configuration']['handlers']['configuration'][$name] = array(
        '#type' => 'item',
        '#markup' => (isset($profile['configuration']) AND $profile['configuration'] != '') ? l(t('configure'), $profile['configuration']) : '',
      );
    }
    $form['islandora_oai_configuration']['handlers']['default'] = array(
      '#type' => 'radios',
      '#options' => isset($options) ? $options : array(),
      '#default_value' => variable_get('islandora_oai_request_handler', 'islandora_oai'),
    );
  }
  else {
    $form['islandora_oai_configuration']['handlers']['no_viewers'] = array(
      '#markup' => t('No viewers detected.'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler for the islandora_oai_admin form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_oai_admin_form_submit($form, $form_state) {
  variable_set('islandora_oai_repository_name', $form_state['values']['islandora_oai_configuration']['islandora_oai_repository_name']);
  variable_set('islandora_oai_path', $form_state['values']['islandora_oai_configuration']['islandora_oai_path']);
  variable_set('islandora_oai_repository_identifier', $form_state['values']['islandora_oai_configuration']['islandora_oai_repository_identifier']);
  variable_set('islandora_oai_admin_email', $form_state['values']['islandora_oai_configuration']['islandora_oai_admin_email']);
  variable_set('islandora_oai_max_size', $form_state['values']['islandora_oai_configuration']['islandora_oai_max_size']);
  variable_set('islandora_oai_expire_time', $form_state['values']['islandora_oai_configuration']['islandora_oai_expire_time']);
  variable_set('islandora_oai_request_handler', $form_state['values']['islandora_oai_configuration']['handlers']['default']);
  variable_set('islandora_oai_query_backend', $form_state['values']['islandora_oai_configuration']['islandora_oai_query_backend']);
  variable_set('islandora_oai_solr_state_field', $form_state['values']['islandora_oai_configuration']['islandora_oai_solr_state_field']);
  variable_set('islandora_oai_solr_collection_description_field', $form_state['values']['islandora_oai_configuration']['islandora_oai_solr_collection_description_field']);
  variable_set('islandora_oai_solr_object_ancestors_field', $form_state['values']['islandora_oai_configuration']['islandora_oai_solr_object_ancestors_field']);
  // Because of the dynamic pathing of the OAI path we need to rebuild the
  // menus.
  menu_rebuild();
  drupal_set_message(t('The configuration options have been saved.'));
}
