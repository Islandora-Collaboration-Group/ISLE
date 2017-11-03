<?php
/**
 * @file
 * Houses the administration form for the Islanora XACML Editor.
 */

/**
 * Admin settings form.
 */
function islandora_xacml_editor_settings() {
  drupal_add_css(drupal_get_path('module', 'islandora_xacml_editor') . '/css/islandora_xacml_editor.css');
  $form = array();

  $form['islandora_xacml_editor_show_dsidregex'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the DSID regex textfield?'),
    '#default_value' => variable_get('islandora_xacml_editor_show_dsidregex', 1),
  );
  $form['islandora_xacml_editor_show_mimeregex'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the MIME type regex textfield?'),
    '#default_value' => variable_get('islandora_xacml_editor_show_mimeregex', 1),
  );
  $form['islandora_xacml_editor_restrictions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Restrictions for DSID and MIME type'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'DSIDs and MIMEs that will not appear in the autocomplete fields or be allowed as filters.',
  );
  $form['islandora_xacml_editor_restrictions']['islandora_xacml_editor_restricted_dsids'] = array(
    '#type' => 'textarea',
    '#title' => t('DSID'),
    '#default_value' => variable_get('islandora_xacml_editor_restricted_dsids', ''),
  );
  $form['islandora_xacml_editor_restrictions']['islandora_xacml_editor_restricted_mimes'] = array(
    '#type' => 'textarea',
    '#title' => t('MIME type'),
    '#default_value' => variable_get('islandora_xacml_editor_restricted_mimes', ''),
  );
  $form['islandora_xacml_editor_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default users and roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('The users and roles that will appear as the default selected unless there is a existing XACML policy attached to an object.'),
  );

  // Get the user list.
  $users = array();
  $result = db_query('SELECT u.uid, u.name FROM {users} u');
  foreach ($result as $user) {
    $user->uid == 0 ? $users['anonymous'] = 'anonymous' : $users[$user->name] = $user->name;
    if ($user->uid == 1) {
      $admin_user = $user->name;
      $form_state['islandora_xacml']['admin_user'] = $user->name;
    }
  }

  // Get role list.
  $roles = array();
  $result = db_query('SELECT r.rid, r.name FROM {role} r');
  foreach ($result as $role) {
    $role->rid == 0 ? $roles['anonymous'] = 'anonymous' : $roles[$role->name] = $role->name;
  }

  $form['islandora_xacml_editor_defaults']['islandora_xacml_editor_default_users'] = array(
    '#type' => 'select',
    '#title' => t('Users'),
    '#options' => $users,
    '#default_value' => variable_get('islandora_xacml_editor_default_users', 'admin'),
    '#multiple' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="islandora_xacml_selects">',
  );
  $form['islandora_xacml_editor_defaults']['islandora_xacml_editor_default_roles'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#default_value' => variable_get('islandora_xacml_editor_default_roles', 'administrator'),
    '#options' => $roles,
    '#multiple' => TRUE,
    '#size' => 10,
    '#suffix' => '</div>',
  );
  return system_settings_form($form);
}
