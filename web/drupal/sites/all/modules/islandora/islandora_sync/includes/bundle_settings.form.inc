<?php

/**
 * @file
 * Islandora Sync bundle settings form.
 */

/**
 * Islandora Sync specific settings form per node bundle (type).
 *
 * @param array $form
 *   Form structure.
 * @param array $form_state
 *   The form state.
 * @param string $bundle
 *   The bundle being modified.
 *
 * @return array
 *   Form structure.
 */
function islandora_sync_bundle_settings_form($form, &$form_state, $bundle) {
  form_load_include($form_state, 'inc', 'islandora', 'includes/utilities');
  $bundle = str_replace('-', '_', $bundle);
  $settings = islandora_sync_get_bundle_settings($bundle);

  if (isset($settings->bundle)) {
    $selected_cmodels = islandora_sync_get_bundle_cmodels($bundle);
    $form['exists'] = array(
      '#type' => 'value',
      '#value' => $settings->bundle,
    );
  }
  else {
    $selected_cmodels = array();
  }

  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );

  $form['sync_fedora'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync nodes to Fedora'),
  );
  if (isset($settings->sync_fedora)) {
    $form['sync_fedora']['#default_value'] = $settings->sync_fedora;
  }
  $form['sync_fedora_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync to Fedora Settings'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="sync_fedora"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['sync_fedora_group']['namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace'),
    '#description' => t('Object ID namespace'),
  );
  if (isset($settings->namespace)) {
    $form['sync_fedora_group']['namespace']['#default_value'] = $settings->namespace;
  }
  $form['sync_fedora_group']['sync_fedora_timing'] = array(
    '#type' => 'radios',
    '#title' => t('Sync Timing'),
    '#description' => t('When should Drupal nodes be synced to Fedora Commons?'),
    '#options' => array(
      'real_time' => t('Real time'),
      'cron' => t('Cron'),
    ),
  );
  if (isset($settings->sync_fedora_timing) && $settings->sync_fedora_timing) {
    $form['sync_fedora_group']['sync_fedora_timing']['#default_value'] = $settings->sync_fedora_timing;
  }
  $form['sync_fedora_group']['sync_fedora_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync node creation'),
  );
  $form['sync_fedora_group']['sync_fedora_create']['#default_value'] = isset($settings->sync_fedora_create) ?
    $settings->sync_fedora_create : TRUE;
  $form['sync_fedora_group']['sync_fedora_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync node updates'),
  );
  $form['sync_fedora_group']['sync_fedora_update']['#default_value'] = isset($settings->sync_fedora_update) ?
    $settings->sync_fedora_update : TRUE;
  $form['sync_fedora_group']['sync_fedora_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync node deletion'),
  );
  $form['sync_fedora_group']['sync_fedora_delete']['#default_value'] = isset($settings->sync_fedora_delete) ?
    $settings->sync_fedora_delete : TRUE;

  $form['sync_drupal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync objects to Drupal'),
  );
  if (isset($settings->sync_drupal)) {
    $form['sync_drupal']['#default_value'] = $settings->sync_drupal;
  }
  $form['sync_drupal_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync to Drupal Settings'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="sync_drupal"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['sync_drupal_group']['sync_drupal_timing'] = array(
    '#type' => 'radios',
    '#title' => t('Sync Timing'),
    '#description' => t('When should Fedora Commons objects be synced to Drupal?'),
    '#options' => array(
      'real_time' => t('Real time'),
      'cron' => t('Cron'),
    ),
  );
  if (isset($settings->sync_drupal_timing) && $settings->sync_drupal_timing) {
    $form['sync_drupal_group']['sync_drupal_timing']['#default_value'] = $settings->sync_drupal_timing;
  }
  $form['sync_drupal_group']['sync_drupal_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync object creation'),
  );
  $form['sync_drupal_group']['sync_drupal_create']['#default_value'] = isset($settings->sync_drupal_create) ?
    $settings->sync_drupal_create : TRUE;
  $form['sync_drupal_group']['sync_drupal_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync object updates'),
  );
  $form['sync_drupal_group']['sync_drupal_update']['#default_value'] = isset($settings->sync_drupal_update) ?
    $settings->sync_drupal_update : TRUE;
  $form['sync_drupal_group']['sync_drupal_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync object deletion'),
  );
  $form['sync_drupal_group']['sync_drupal_delete']['#default_value'] = isset($settings->sync_drupal_delete) ?
    $settings->sync_drupal_delete : TRUE;

  $labellize = function ($model_info) {
    return format_string('!label (!pid)', array(
      '!label' => $model_info['label'],
      '!pid' => $model_info['pid'],
    ));
  };
  $content_models = array_map($labellize, islandora_get_content_models());

  $form['cmodel'] = array(
    '#type' => 'radios',
    '#title' => t('Content model'),
    '#description' => t('The content model that this node type should be mapped to/from.'),
    '#options' => $content_models,
    '#multiple' => TRUE,
    '#states' => array(
      'invisible' => array(
        ':input[name="sync_fedora"]' => array('checked' => FALSE),
        ':input[name="sync_drupal"]' => array('checked' => FALSE),
      ),
    ),
  );
  if ($selected_cmodels) {
    $form['cmodel']['#default_value'] = $selected_cmodels[0];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Type Settings'),
  );
  return $form;
}

/**
 * Form validation.
 */
function islandora_sync_bundle_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  module_load_include('inc', 'islandora', 'includes/utilities');
  // Require valid namespace.
  if (!empty($values['namespace']) && !islandora_is_valid_namespace($values['namespace'])) {
    form_set_error('namespace', t('Please enter a valid namespace'));
  }

  // Require a content model if syncing from Fedora.
  if ($values['sync_fedora'] && is_null($values['cmodel'])) {
    form_set_error('cmodel', t('Content model is required.'));
  }

  // Require sync_timing if parent sync selected.
  if ($values['sync_fedora'] && is_null($values['sync_fedora_timing'])) {
    form_set_error('sync_fedora_timing', t('Sync Timing is required.'));
  }
  if ($values['sync_drupal'] && is_null($values['sync_drupal_timing'])) {
    form_set_error('sync_drupal_timing', t('Sync Timing is required.'));
  }
}

/**
 * Form submit.
 */
function islandora_sync_bundle_settings_form_submit($form, $form_state) {
  $record = $form_state['values'];
  $record['entity_type'] = 'node';

  if (isset($form_state['values']['exists'])) {
    drupal_write_record('islandora_sync_type_settings', $record, 'bundle');
  }
  else {
    drupal_write_record('islandora_sync_type_settings', $record);
  }

  $existing_cmodels = islandora_sync_get_bundle_cmodels($record['bundle']);

  // Insert new cmodels.
  foreach (array($form_state['values']['cmodel']) as $cmodel) {
    if (!in_array($cmodel, $existing_cmodels)) {
      $cmodel_record = array('bundle' => $record['bundle'], 'cmodel' => $cmodel);
      drupal_write_record('islandora_sync_type_cmodel', $cmodel_record);
    }
  }

  // Delete cmodels.
  $deleted_cmodels = array_diff($existing_cmodels, array($form_state['values']['cmodel']));
  foreach ($deleted_cmodels as $deleted) {
    db_query('DELETE FROM {islandora_sync_type_cmodel} WHERE bundle = :bundle AND cmodel = :cmodel',
      array(':bundle' => $record['bundle'], ':cmodel' => $deleted)
    );
  }

  drupal_set_message(t('Settings updated.'));
}
