<?php

/**
 * @file
 * Admin form for setting embargo view/manage roles.
 */

/**
 * Embargo roles admin management form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The form definition.
 */
function islandora_scholar_embargo_manage_roles_admin_form($form, $form_state) {
  module_load_include('inc', 'islandora_scholar_embargo', 'includes/embargo');
  return array(
    'roles_wrapper' => array(
      '#type' => 'fieldset',
      '#title' => t('Roles'),
      '#collapsible' => FALSE,
      'islandora_scholar_embargo_whitelisted_roles' => array(
        '#type' => 'checkboxes',
        '#description' => t('Check off all the roles that should have access to embargoed items. <strong>NOTE:</strong> since embargo XACML role entries are whitelists, embargoed objects in a Fedora repository shared between Drupal sites will be visible to the same role in both sites. Consider using site-unique roles when dealing with shared repositories.'),
        '#options' => user_roles(TRUE, ISLANDORA_SCHOLAR_EMBARGO_CAN_EMBARGO_ANY),
        '#default_value' => array_keys(variable_get('islandora_scholar_embargo_whitelisted_roles', array())),
      ),
    ),
    'note' => array(
      '#markup' => t('Saving and applying will initiate a batch to modify the XACML policies of all embargoed objects. Bear in mind that this can take a long time to execute depending on the number of items to be embargoed, and that it may be more prudent to run the Drush command <code>islandora_scholar_embargo_apply_roles_to_embargoed_objects</code> instead. Number of embargoed objects at time of page load: <strong>@number</strong>', array(
        '@number' => number_format(islandora_scholar_embargo_get_all_embargoed(TRUE)),
      )),
    ),
    'submit_buttons' => array(
      '#type' => 'actions',
      'save' => array(
        '#type' => 'submit',
        '#value' => t('Save Without Applying'),
        '#name' => 'save',
      ),
      'save_and_apply' => array(
        '#type' => 'submit',
        '#value' => t('Save And Apply'),
        '#name' => 'save_and_apply',
      ),
    ),
  );
}

/**
 * Submit handler for the roles management form.
 *
 * @param array $form
 *   The submitted roles admin form.
 * @param array $form_state
 *   The state of the submitted form.
 */
function islandora_scholar_embargo_manage_roles_admin_form_submit($form, &$form_state) {
  // Attach role names to role IDs to make it more useful as a variable.
  $whitelist = array();
  foreach (array_filter($form_state['values']['islandora_scholar_embargo_whitelisted_roles']) as $rid => $checked) {
    $whitelist[$rid] = $form['roles_wrapper']['islandora_scholar_embargo_whitelisted_roles']['#options'][$rid];
  }
  variable_set('islandora_scholar_embargo_whitelisted_roles', $whitelist);

  // Kick off the batch if we were asked.
  if ($form_state['triggering_element']['#name'] == 'save_and_apply') {
    module_load_include('inc', 'islandora_scholar_embargo', 'includes/batch');
    batch_set(islandora_scholar_embargo_reapply_embargoes_batch());
  }
}
