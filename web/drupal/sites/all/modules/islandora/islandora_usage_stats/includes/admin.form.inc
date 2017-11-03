<?php

/**
 * @file
 * Administration page callbacks for the islandora usage stats module.
 */

/**
 * Usage stats administration and configuration form.
 *
 * @ingroup forms
 *
 * @todo break out non-settings elements into their own form.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_usage_stats_admin_form() {
  $form = array();

  $form['usage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Omit IPs'),
    '#description' => t('Add IP addresses that you want to omit from usage tracking'),
  );

  $form['usage']['islandora_usage_stats_ip_list'] = array(
    '#type' => 'textarea',
    '#description' => t('Add IPs separated by spaces'),
    '#default_value' => variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'),
  );

  $form['usage']['islandora_usage_stats_cooldown_seconds'] = array(
    '#type' => 'textfield',
    '#title' => t('Cooldown time'),
    '#description' => t('The number of seconds to wait before counting a repeated view request from the same IP'),
    '#default_value' => variable_get('islandora_usage_stats_cooldown_seconds', '300'),
  );

  $form['usage']['islandora_usage_stats_exclude_bots'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude bots'),
    '#description' => t('Exclude common bots to avoid artifical view count inflation.'),
    '#default_value' => variable_get('islandora_usage_stats_exclude_bots', '1'),
  );

  $form['clear'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear values'),
  );

  $form['clear']['usage']['access_log'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access log'),
  );

  $form['clear']['usage']['access_log']['clear_object_access_log_submit'] = array(
    '#type' => 'submit',
    '#name' => 'clearObjectAccessLogSubmit',
    '#default_value' => t('Clear object views access log'),
  );

  $form['clear']['usage']['access_log']['clear_ds_access_log_submit'] = array(
    '#type' => 'submit',
    '#name' => 'clearDSAccessLogSubmit',
    '#default_value' => t('Clear datastream download log'),
  );

  $form['clear']['usage']['views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Object Views'),
  );

  $form['clear']['usage']['views']['clear_view_by_pid'] = array(
    '#type' => 'textfield',
    '#title' => t('PID'),
    '#description' => t('This should be in the format <em>islandora:1</em>'),
  );

  $form['clear']['usage']['views']['clear_view_by_pid_submit'] = array(
    '#type' => 'submit',
    '#name' => 'clearViewByPIDSubmit',
    '#default_value' => t('Clear object view log'),
  );

  $form['clear']['usage']['ds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Datastream Downloads'),
  );

  $form['clear']['usage']['ds']['clear_ds_by_pid'] = array(
    '#type' => 'textfield',
    '#title' => t('PID'),
    '#description' => t('This should be in the format <em>islandora:1</em>'),
  );

  $form['clear']['usage']['ds']['clear_ds_by_pid_submit'] = array(
    '#type' => 'submit',
    '#name' => 'clearDSByPIDSubmit',
    '#default_value' => t('Clear datastream download log'),
  );

  $form['save_configuration'] = array(
    '#type' => 'submit',
    '#name' => 'saveConfiguration',
    '#default_value' => t('Save configuration'),
  );

  return $form;
}
/**
 * Validation function for the Usage Stats admin form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_usage_stats_admin_form_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#id'] === 'edit-clear-view-by-pid-submit') {
    if (empty($form_state['values']['clear_view_by_pid'])) {
      form_set_error('clear_view_by_pid', t("Please enter a PID."));
    }
  }
  if ($form_state['triggering_element']['#id'] === 'edit-clear-ds-by-pid-submit') {
    if (empty($form_state['values']['clear_ds_by_pid'])) {
      form_set_error('clear_ds_by_pid', t("Please enter a PID."));
    }
  }
}
/**
 * Submit handler for the admin form for the islandora_usage_stats.
 *
 * @param array $form
 *   An array representing a form in Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_usage_stats_admin_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_usage_stats', 'includes/db');
  $button = $form_state['triggering_element']['#id'];
  switch ($button) {
    case 'edit-clear-object-access-log-submit':
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_access_log');
      if ($result) {
        drupal_set_message(t("Object view access logs have been cleared"));
      }
      break;

    case 'edit-clear-ds-access-log-submit':
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_ds_access_log');
      if ($result) {
        drupal_set_message(t("Datastream download access logs have been cleared"));
      }
      break;

    case 'edit-clear-view-by-pid-submit':
      $pid = $form_state['values']['clear_view_by_pid'];
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_access_log', $pid);
      if ($result) {
        drupal_set_message(
          t("Object tracking for PID '@pid' deleted!", array('@pid' => $pid))
        );
      }
      break;

    case 'edit-clear-ds-by-pid-submit':
      $pid = $form_state['values']['clear_ds_by_pid'];
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_ds_access_log', $pid);
      if ($result) {
        drupal_set_message(
          t("Datastream tracking for PID '@pid' deleted!", array('@pid' => $pid))
        );
      }
      break;

    default:
      variable_set('islandora_usage_stats_ip_list', $form_state['values']['islandora_usage_stats_ip_list']);
      variable_set('islandora_usage_stats_cooldown_seconds', $form_state['values']['islandora_usage_stats_cooldown_seconds']);
      variable_set('islandora_usage_stats_exclude_bots', $form_state['values']['islandora_usage_stats_exclude_bots']);
      drupal_set_message(
        t("The configuration options have been saved.")
      );
      break;
  }
}
