<?php

/**
 * @file
 * Admin form functions for Islandora Altmetrics.
 */

/**
 * Admin form for Islandora Badges.
 */
function islandora_badges_admin_form($form, $form_settings) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $all_cmodels = islandora_get_content_models();
  $already_chosen = variable_get('islandora_badges_selected_cmodels', array('ir:citationCModel', 'ir:thesisCModel'));
  $selected_rows = drupal_map_assoc($already_chosen);

  $form['islandora_badges_doi_xpath'] = array(
    '#type' => 'textfield',
    '#title' => t('DOI XPath'),
    '#description' => t('MODS XPath for retrieving the DOI.'),
    '#default_value' => variable_get('islandora_badges_doi_xpath', '/mods:mods/mods:identifier[@type="doi"]'),
  );

  $form['content_model_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure which content models will display badges.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['content_model_wrapper']['badges_table'] = array(
    '#type' => 'tableselect',
    '#title' => t('Enable custom pathauto settings for these content models'),
    '#tree' => TRUE,
    '#multiple' => TRUE,
    '#header' => array(
      'label' => array('data' => t('Content-Model')),
    ),
    '#default_value' => $selected_rows,
  );

  $options = array();
  foreach ($all_cmodels as $cmodel) {
    $pid = $cmodel['pid'];
    $label = $cmodel['label'];
    $options[$pid] = array('label' => array('data' => $label));
  }
  $form['content_model_wrapper']['badges_table']['#options'] = $options;

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#weight' => 0,
  );

  return $form;
}

/**
 * Drupal form validation handler.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_badges_admin_form_validate(array $form, array &$form_state) {
  if (!isset($form_state['values']['islandora_badges_doi_xpath']) ||
    empty($form_state['values']['islandora_badges_doi_xpath'])) {
    form_set_error(
      'islandora_badges_doi_xpath',
      t('You must set the XPath to find the DOI identifier in your MODS.')
    );
  }
  if (!array_keys(array_filter($form_state['values']['badges_table']))) {
    form_set_error(
      'badges_table',
      t("Please select at least one content-model to display the badges on.")
    );
  }
}

/**
 * Submit handler for admin form.
 */
function islandora_badges_admin_form_submit(array $form, array &$form_state) {
  $enabled = array_keys(array_filter($form_state['values']['badges_table']));
  variable_set('islandora_badges_selected_cmodels', $enabled);
  variable_set('islandora_badges_doi_xpath', $form_state['values']['islandora_badges_doi_xpath']);
  drupal_set_message(t("Configuration saved."));
}
