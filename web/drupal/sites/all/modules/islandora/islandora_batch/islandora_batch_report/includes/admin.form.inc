<?php

/**
 * @file
 * The admin form.
 */

/**
 * Admin form for the batch.
 */
function islandora_batch_report_admin_form($form, $form_state) {
  $form['islandora_batch_report_send_report_after'] = array(
    '#type' => 'textfield',
    '#title' => t('A Time to send daily batch report after'),
    '#description' => t('Using a 24-hour clock (in the format hh:mm), specify the earliest time to send daily reports after, so that batch reports are not sent throughout the day.'),
    '#required' => TRUE,
    '#default_value' => variable_get('islandora_batch_report_send_report_after', '23:00'),
  );
  return system_settings_form($form);
}

/**
 * Admin form validate function.
 */
function islandora_batch_report_admin_form_validate($form, &$form_state) {
  // Check that the time submitted is a valid time.
  if (!preg_match("/^(([01]?[0-9])|([2][0-3])):([0-5][0-9])$/", $form_state['values']['islandora_batch_report_send_report_after'])) {
    form_set_error('islandora_batch_report_send_report_after', t('Invalid time.'));
  }
  else {
    // Reset last sent report to prevent skipping a report when the admin
    // changes the report after time.
    variable_set('islandora_batch_report_daily_report_last_sent', '');
  }
}
