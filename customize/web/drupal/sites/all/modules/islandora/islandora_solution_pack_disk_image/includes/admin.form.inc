<?php
/**
 * @file
 * Holds administration functionality for the disk image solution pack.
 */

/**
 * Islandora Disk Image administration settings.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The state of the form.
 *
 * @return array
 *   The form definition.
 */
function islandora_disk_image_admin($form, &$form_state) {
  $form = array();
  module_load_include('inc', 'islandora', 'includes/solution_packs');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_disk_image_admin_submit'),
  );

  $form['islandora_disk_image_fiwalk_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to fiwalk executable.'),
    '#default_value' => variable_get('islandora_disk_image_fiwalk_path', '/usr/local/bin/fiwalk'),
    '#description' => t('Path to fiwalk on your server.'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Form submit for allowing the deletion of the viewer variable.
 */
function islandora_disk_image_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_disk_image_fiwalk_path');
      break;
  }
}
