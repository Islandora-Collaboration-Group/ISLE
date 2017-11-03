<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
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
function islandora_bookmark_admin_settings(array $form, array &$form_state) {
  $form = array(
    'list_type' => array(
      '#type' => 'textfield',
      '#title' => t('The name of the type of list.'),
      '#description' => t('The unpluralized name of the type of lists such as bookmark or favorite.'),
      '#default_value' => variable_get('islandora_bookmark_type', 'bookmark'),
    ),
    'anonymous_owner' => array(
      '#type' => 'textfield',
      '#title' => t('The owner name to display for anonymous users.'),
      '#description' => t('The name to display in the owner column for users who are not logged in.'),
      '#default_value' => variable_get('islandora_bookmark_owner', 'Everyone'),
    ),
    'overview_page_elements' => array(
      '#type' => 'textfield',
      '#title' => t('Bookmarks overview table'),
      '#description' => t('Number of Bookmarks shown per page'),
      '#default_value' => variable_get('islandora_bookmark_overview_page_elements', 10),
      '#element_validate' => array('element_validate_integer_positive'),
      '#size' => 4,
    ),
    'detailed_page_elements' => array(
      '#type' => 'textfield',
      '#title' => t('Detailed bookmark table'),
      '#description' => t('Number of objects shown per page'),
      '#default_value' => variable_get('islandora_bookmark_detailed_page_elements', 10),
      '#element_validate' => array('element_validate_integer_positive'),
      '#size' => 4,
    ),
    'create_user_default_lists' => array(
      '#type' => 'checkbox',
      '#title' => t('Create a default list for users?'),
      '#description' => t('The list will be created the next time they log in.'),
      '#default_value' => variable_get('islandora_bookmark_create_user_default_lists', FALSE),
    ),
    'default_list_name' => array(
      '#type' => 'textfield',
      '#title' => t('Default list name'),
      '#description' => t("The name of the default user list. To use the user's name in the field use '@username'."),
      '#default_value' => variable_get('islandora_bookmark_default_list_name', "@username's Default List"),
      '#states' => array(
        'visible' => array(
          ':input[name="create_user_default_lists"]' => array('checked' => TRUE),
        ),
      ),
    ),
    'share_with_everyone' => array(
      '#type' => 'checkbox',
      '#title' => t('Share new default lists with everyone?'),
      '#default_value' => variable_get('islandora_bookmark_share_default_list', FALSE),
      '#states' => array(
        'visible' => array(
          ':input[name="create_user_default_lists"]' => array('checked' => TRUE),
        ),
      ),
    ),
    'islandora_bookmark_default_list_view' => array(
      '#type' => 'select',
      '#title' => t('Default bookmark list view.'),
      '#default_value' => variable_get('islandora_bookmark_default_list_view', 'manage'),
      '#options' => array(
        'manage' => t('Administration Table'),
        'islandora_bookmark_list_columns' => t('Columns'),
      ),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    ),
    'reset' => array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
    ),
  );
  return $form;
}

/**
 * Validation function for the Bookmark admin form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_bookmark_admin_settings_validate(array $form, array &$form_state) {
  if ($form_state['triggering_element']['#id'] != 'edit-reset' && $form_state['values']['create_user_default_lists']) {
    if (empty($form_state['values']['default_list_name'])) {
      form_set_error('default_list_name', t('Please provide a name for default user lists.'));
    }
  }
}

/**
 * Function that sets the Drupal variables with user's input.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_bookmark_admin_settings_submit(array $form, array &$form_state) {
  drupal_set_message(t('The settings have been updated!'));
  $id = $form_state['triggering_element']['#id'];
  switch ($id) {
    case 'edit-submit':
      variable_set('islandora_bookmark_overview_page_elements', $form_state['values']['overview_page_elements']);
      variable_set('islandora_bookmark_detailed_page_elements', $form_state['values']['detailed_page_elements']);
      variable_set('islandora_bookmark_type', $form_state['values']['list_type']);
      variable_set('islandora_bookmark_owner', $form_state['values']['anonymous_owner']);
      variable_set('islandora_bookmark_create_user_default_lists', $form_state['values']['create_user_default_lists']);
      if ($form_state['values']['create_user_default_lists']) {
        variable_set('islandora_bookmark_default_list_name', $form_state['values']['default_list_name']);
        variable_set('islandora_bookmark_share_default_list', $form_state['values']['share_with_everyone']);
      }
      variable_set('islandora_bookmark_default_list_view', $form_state['values']['islandora_bookmark_default_list_view']);
      break;

    case 'edit-reset':
      variable_del('islandora_bookmark_overview_page_elements');
      variable_del('islandora_bookmark_detailed_page_elements');
      variable_del('islandora_bookmark_type');
      variable_del('islandora_bookmark_owner');
      variable_del('islandora_bookmark_create_user_default_lists');
      variable_del('islandora_bookmark_default_list_name');
      variable_del('islandora_bookmark_share_default_list');
      variable_del('islandora_bookmark_default_list_view');
      break;
  }
}
