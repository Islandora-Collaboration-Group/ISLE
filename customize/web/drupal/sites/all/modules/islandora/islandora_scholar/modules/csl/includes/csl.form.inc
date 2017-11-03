<?php

/**
 * @file
 * Managed the installed CSL styles.
 */

/**
 * A form to list/add/remove the installed CSL 1.0 Styles.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function csl_manage_form(array $form, array &$form_state) {
  form_load_include($form_state, 'inc', 'csl', 'includes/csl.form.inc');
  $form['hidden_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Hidden Submit'),
    '#attributes' => array(
      'style' => array(
        'display: none;',
      ),
    ),
    '#validate' => array('csl_manage_add_style_validate'),
    '#submit' => array('csl_manage_add_style_submit'),
  );
  $table = array(
    '#header' => array(t('Style Name'), '', t('Delete')),
    '#theme' => CSL_MANAGE_TABLE_THEME,
    '#tree' => TRUE,
    'rows' => array(),
  );
  $styles = CSL::GetNames();
  $default_style = CSL::GetDefaultName();
  foreach ($styles as $style) {
    $is_default = array(
      '#markup' => t('Is Default'),
    );
    $set_default = array(
      '#type' => 'submit',
      '#value' => t('Set Default'),
      '#name' => 'row-' . count($table['rows']) . '-set-default',
      '#style' => $style,
      '#submit' => array('csl_manage_set_default_style_submit'),
    );
    $table['rows'][] = array(
      'name' => array('#markup' => $style),
      'default' => ($default_style == $style) ? $is_default : $set_default,
      'delete' => array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#name' => 'row-' . count($table['rows']) . '-delete',
        '#style' => $style,
        '#access' => $default_style != $style,
        '#submit' => array('csl_manage_delete_style_submit'),
      ),
    );
  }
  $table['rows'][] = array(
    '#tree' => FALSE,
    'name' => array(
      '#type' => 'textfield',
      '#title' => t('Name'),
    ),
    'file' => array(
      '#type' => 'file',
      '#title' => t('Upload A CSL Style'),
    ),
    'add' => array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#validate' => array('csl_manage_add_style_validate'),
      '#submit' => array('csl_manage_add_style_submit'),
    ),
  );
  $form['table'] = $table;
  return $form;
}

/**
 * Submit call back for deleting a CSL Style from the manage form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function csl_manage_delete_style_submit(array $form, array &$form_state) {
  $style = $form_state['clicked_button']['#style'];
  $style_t = array('%style' => $style);
  if (count(CSL::GetNames()) == 1) {
    drupal_set_message(t("Can't delete the last style: %style, there must always be at least one.", $style_t), 'error');
  }
  elseif (CSL::Delete($style)) {
    drupal_set_message(t('Successfully deleted style: %style', $style_t));
  }
  else {
    drupal_set_message(t('Failed to deleted style: %style', $style_t), 'error');
  }
}

/**
 * Submit callback that sets the default style.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function csl_manage_set_default_style_submit(array $form, array &$form_state) {
  $style_name = $form_state['clicked_button']['#style'];
  CSL::SetDefaultName($style_name);
  drupal_set_message(t('Successfully changed default style: %style', array('%style' => $style_name)));
}

/**
 * Validation hook for adding a CSL Style from the manage form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function csl_manage_add_style_validate(array $form, array &$form_state) {
  $name = $form_state['values']['name'];
  $file_was_uploaded = isset($_FILES['files']['error']['file']) && $_FILES['files']['error']['file'] === 0;
  if (empty($name)) {
    form_set_error('name', t('Field is required.'));
  }
  elseif (CSL::Exists($name)) {
    form_set_error('name', t('The style %name already exists.', array('%name' => $name)));
  }
  elseif (!$file_was_uploaded) {
    form_set_error('file', t('A valid CSL 1.0 Style must be uploaded.'));
  }
}

/**
 * Submit call back for deleting a CSL Style from the manage form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function csl_manage_add_style_submit(array $form, array &$form_state) {
  module_load_include('inc', 'csl', 'includes/csl');
  $style_name = $form_state['values']['name'];
  $definition = new DOMDocument();
  $definition->load($_FILES['files']['tmp_name']['file']);
  CSL::Create($style_name, $definition);
}
