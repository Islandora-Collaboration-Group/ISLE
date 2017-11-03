<?php
/**
 * @file
 * Holds administration functionality for the web archive solution pack.
 */

/**
 * Islandora video administration settings.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The state of the form.
 *
 * @return array
 *   The form definition.
 */
function islandora_web_archive_admin($form, &$form_state) {
  $form = array();
  module_load_include('inc', 'islandora', 'includes/solution_packs');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_web_archive_admin_submit'),
  );

  $form['islandora_web_archive_warcfilter_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to warcfilter executable.'),
    '#default_value' => variable_get('islandora_web_archive_warcfilter_path', '/usr/local/bin/warcfilter'),
    '#description' => t('Path to warcfilter on your server.'),
    '#required' => TRUE,
  );

  $form['islandora_web_archive_warcindex_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to warcindex executable.'),
    '#default_value' => variable_get('islandora_web_archive_warcindex_path', '/usr/local/bin/warcindex'),
    '#description' => t('Path to warcindex on your server.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Form submit for allowing the deletion of the viewer variable.
 */
function islandora_web_archive_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_web_archive_warcfilter_path');
      variable_del('islandora_web_archive_warcindex_path');
      break;
  }
}
