<?php

/**
 * @file
 * Field settings form.
 */

/**
 * Field settings form for Islandora Sync.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The form state.
 * @param string $type
 *   The node type (bundle).
 * @param string $field
 *   The field name.
 *
 * @return array
 *   Form structure.
 */
function islandora_sync_field_settings_form($form, $form_state, $type, $field) {
  $type = str_replace('-', '_', $type);
  $field_settings = islandora_sync_get_field_settings($type, $field);
  if (empty($field_settings)) {
    $settings = new stdClass();
    $settings->new = TRUE;
  }
  else {
    $settings = $field_settings[0];
    $settings->new = FALSE;
  }

  $form = array();
  $form['settings'] = array(
    '#type' => 'value',
    '#value' => $settings,
  );
  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['field'] = array(
    '#type' => 'value',
    '#value' => $field,
  );
  $form['export'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync with Fedora Commons?'),
    '#description' => t('Should this field be synced with Fedora Commons?'),
  );
  if (isset($settings->export)) {
    $form['export']['#default_value'] = $settings->export;
  }

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Fedora Commons Settings'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="export"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['general']['export_type'] = array(
    '#type' => 'select',
    '#title' => t('Fedora Commons data structure'),
    '#description' => t('The type of data this field should be exported as.'),
    '#options' => array(
      'xml_value' => t('XML value'),
      'managed_datastream' => t('Managed datastream'),
      'relationship' => t('Relationship'),
    ),
    '#empty_option' => t('- Select -'),
  );
  if (isset($settings->export_type)) {
    $form['general']['export_type']['#default_value'] = $settings->export_type;
  }
  $form['general']['dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('Datastream ID'),
    '#states' => array(
      'visible' => array(
        ':input[name="export_type"]' => array(
          array('value' => 'xml_value'),
          array('value' => 'managed_datastream'),
        ),
      ),
    ),
  );
  if (isset($settings->dsid)) {
    $form['general']['dsid']['#default_value'] = $settings->dsid;
  }
  $form['xml'] = array(
    '#type' => 'fieldset',
    '#title' => t('XML Settings'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="export_type"]' => array('value' => 'xml_value'),
      ),
    ),
  );
  $form['xml']['create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create'),
    '#description' => t('If checked, create an XML element when creating or updating a node.'),
  );
  if (isset($settings->create_value)) {
    $form['xml']['create']['#default_value'] = TRUE;
  }
  $form['xml']['create_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create Settings'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="create"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['xml']['create_group']['create_context'] = array(
    '#type' => 'select',
    '#title' => t('Path context'),
    '#options' => array(
      'parent' => t('Parent'),
      'document' => t('Document'),
    ),
    '#empty_option' => t('- Select -'),
  );
  if (isset($settings->create_context)) {
    $form['xml']['create_group']['create_context']['#default_value'] = $settings->create_context;
  }
  $form['xml']['create_group']['create_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
  );
  if (isset($settings->create_path)) {
    $form['xml']['create_group']['create_path']['#default_value'] = $settings->create_path;
  }
  $form['xml']['create_group']['create_type'] = array(
    '#type' => 'select',
    '#title' => t('XML field type'),
    '#options' => array(
      'element' => t('Element'),
      'attribute' => t('Attribute'),
      'xml' => t('XML'),
    ),
    '#empty_option' => t('- Select -'),
  );
  if (isset($settings->create_type)) {
    $form['xml']['create_group']['create_type']['#default_value'] = $settings->create_type;
  }
  $form['xml']['create_group']['create_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Create'),
  );
  if (isset($settings->create_value)) {
    $form['xml']['create_group']['create_value']['#default_value'] = $settings->create_value;
  }
  $form['xml']['read'] = array(
    '#type' => 'checkbox',
    '#title' => t('Read'),
    '#description' => t('If checked, read an XML value when importing a Fedora Commons object.'),
  );
  if (isset($settings->read_path)) {
    $form['xml']['read']['#default_value'] = TRUE;
  }
  $form['xml']['read_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Read Settings'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="read"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['xml']['read_group']['read_context'] = array(
    '#type' => 'select',
    '#title' => t('Path context'),
    '#options' => array(
      'parent' => t('Parent'),
      'document' => t('Document'),
    ),
    '#empty_option' => t('- Select -'),
  );
  if (isset($settings->read_context)) {
    $form['xml']['read_group']['read_context']['#default_value'] = $settings->read_context;
  }
  $form['xml']['read_group']['read_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Read'),
    '#description' => t("If Document context, don't include the root element."),
  );
  if (isset($settings->read_path)) {
    $form['xml']['read_group']['read_path']['#default_value'] = $settings->read_path;
  }
  $form['relationship'] = array(
    '#type' => 'fieldset',
    '#title' => t('Relationship Settings'),
    '#collapsible' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="export_type"]' => array('value' => 'relationship'),
      ),
    ),
  );
  $form['relationship']['predicate'] = array(
    '#type' => 'textfield',
    '#title' => t('Relationship Predicate'),
  );
  if (isset($settings->predicate)) {
    $form['relationship']['predicate']['#default_value'] = $settings->predicate;
  }
  $form['relationship']['predicate_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Predicate URI'),
  );
  if (isset($settings->predicate_uri)) {
    $form['relationship']['predicate_uri']['#default_value'] = $settings->predicate_uri;
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Field Settings'),
  );
  return $form;
}

/**
 * Form validation.
 */
function islandora_sync_field_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['export_type'] == 'xml_value') {
    $dsid = $form_state['values']['dsid'];
    $field = db_query("SELECT ds_id FROM {islandora_sync_xml_ds} WHERE ds_id = :dsid",
        array(':dsid' => $dsid)
      )->fetchField();
    if (empty($field)) {
      form_set_error('dsid', t('No XML value defined for the @ds datastream!', array('ds' => $dsid)));
    }
  }
}

/**
 * Form submit.
 */
function islandora_sync_field_settings_form_submit($form, $form_state) {
  $values = $form_state['values'];

  // Update settings.
  if (!$values['settings']->new) {
    unset($values['settings']);
    drupal_write_record('islandora_sync_fields', $values, array('bundle', 'field'));
  }
  else {
    // Insert settings.
    unset($values['settings']);
    drupal_write_record('islandora_sync_fields', $values);
  }
  drupal_set_message(t('Fedora Commons settings saved.'));
}
