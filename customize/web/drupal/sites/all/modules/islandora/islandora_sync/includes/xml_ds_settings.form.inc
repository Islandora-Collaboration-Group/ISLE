<?php

/**
 * @file
 * XML datastream settings form.
 */

/**
 * Menu callback for XML datastreams settings.
 */
function islandora_sync_xml_ds_page() {
  $datastreams = db_query('SELECT * FROM {islandora_sync_xml_ds}')->fetchAll();

  $header = array(t('Datastream'), t('Edit'), t('Delete'));
  $rows = array();
  foreach ($datastreams as $datastream) {
    $rows[] = array(
      check_plain($datastream->ds_id),
      l(t('Edit'), "admin/islandora/tools/sync/xml-datastreams/$datastream->ds_id/edit"),
      l(t('Delete'), "admin/islandora/tools/sync/xml-datastreams/$datastream->ds_id/delete"),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Form to add or edit XML datastream settings.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param string $ds_id
 *   The datastream ID.
 *
 * @return array
 *   Form structure.
 */
function islandora_sync_xml_ds_add($form, &$form_state, $ds_id = NULL) {
  $form = array();
  $form['#tree'] = TRUE;

  $namespaces = array();
  if (!is_null($ds_id)) {
    $ds_settings = db_query("SELECT * FROM {islandora_sync_xml_ds} WHERE ds_id = :ds_id",
      array(':ds_id' => $ds_id)
    )->fetchObject();
    $namespaces_objs = db_query("SELECT * FROM {islandora_sync_xml_ds_namespaces} WHERE ds_id = :ds_id",
      array(':ds_id' => $ds_id)
    )->fetchAll();

    // Normalize to array.
    foreach ($namespaces_objs as $namespace) {
      $namespaces[] = array('prefix' => $namespace->prefix, 'uri' => $namespace->uri);
    }

    $form['ds_id_update'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
  }
  $form['ds_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Datastream ID'),
    '#description' => t('The DSID for this XML datastream'),
    '#required' => TRUE,
  );
  if (isset($ds_settings->ds_id)) {
    $form['ds_id']['#default_value'] = $ds_settings->ds_id;
  }
  $form['root'] = array(
    '#type' => 'textfield',
    '#title' => t('Root Element'),
    '#description' => t('The root element of the XML document created for this datastream'),
    '#required' => TRUE,
  );
  if (isset($ds_settings->root)) {
    $form['root']['#default_value'] = $ds_settings->root;
  }
  $form['namespace_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace URI'),
  );
  if (isset($ds_settings->namespace_uri)) {
    $form['namespace_uri']['#default_value'] = $ds_settings->namespace_uri;
  }
  $form['namespaces'] = array(
    '#type' => 'fieldset',
    '#title' => t('Namespaces'),
  );

  if (!empty($namespaces) && empty($form_state['input']['namespaces'])) {
    $form_state['namespace_count'] = count($namespaces);
    $form['existing_namespaces'] = array(
      '#type' => 'value',
      '#value' => $namespaces,
    );
  }
  if (!empty($form_state['input']['namespaces'])) {
    $namespaces = islandora_sync_xml_ds_normalize_namespace_values($form_state['input']['namespaces']);
    $form_state['namespace_count'] = count($namespaces);
  }

  if (empty($form_state['namespace_count'])) {
    $form_state['namespace_count'] = 0;
  }

  $namespaces = islandora_sync_xml_ds_normalize_namespace_values($namespaces);

  for ($i = 0; $i <= $form_state['namespace_count']; $i++) {
    $form['namespaces'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Namespace @num', array('@num' => ($i + 1))),
    );
    $form['namespaces'][$i]['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
    );
    if (isset($namespaces[$i]['prefix'])) {
      $form['namespaces'][$i]['prefix']['#default_value'] = $namespaces[$i]['prefix'];
    }
    $form['namespaces'][$i]['uri'] = array(
      '#type' => 'textfield',
      '#title' => t('URI'),
    );
    if (isset($namespaces[$i]['uri'])) {
      $form['namespaces'][$i]['uri']['#default_value'] = $namespaces[$i]['uri'];
    }
  }
  $form['namespaces']['add_namespace'] = array(
    '#type' => 'submit',
    '#value' => t('Add Namespace'),
    '#submit' => array('islandora_sync_xml_ds_add_namespace_add'),
    '#limit_validation_errors' => array(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Datastream Settings'),
  );
  return $form;
}

/**
 * Button callback to add another namespace.
 */
function islandora_sync_xml_ds_add_namespace_add($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  drupal_set_message(t('Namespace added.'));
}

/**
 * Form validation.
 */
function islandora_sync_xml_ds_add_validate($form, &$form_state) {
  if (!isset($form_state['values']['ds_id_update'])) {
    // @TODO: handle ds_id case?
    $dsid_exists = db_query('SELECT 1 FROM {islandora_sync_xml_ds} WHERE ds_id = :ds_id',
      array('ds_id' => $form_state['values']['ds_id'])
    )->fetchAll();
    if ($dsid_exists) {
      form_set_error('ds_id', t('An entry for the @ds datastream already exists!.', array('@ds' => $form_state['values']['ds_id'])));
    }
  }
}

/**
 * Form submit.
 */
function islandora_sync_xml_ds_add_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = array(
    'ds_id' => $values['ds_id'],
    'root' => $values['root'],
    'namespace_uri' => $values['namespace_uri'],
  );

  if (isset($values['ds_id_update']) && $values['ds_id_update']) {
    drupal_write_record('islandora_sync_xml_ds', $record, 'ds_id');
  }
  else {
    drupal_write_record('islandora_sync_xml_ds', $record);
  }

  db_query('DELETE FROM {islandora_sync_xml_ds_namespaces}
    WHERE ds_id = :ds_id',
    array(':ds_id' => $form_state['values']['ds_id'])
  );

  foreach (islandora_sync_xml_ds_normalize_namespace_values($values['namespaces']) as $namespace) {
    if (is_array($namespace)) {
      $record = array(
        'ds_id' => $values['ds_id'],
        'prefix' => $namespace['prefix'],
        'uri' => $namespace['uri'],
      );
      drupal_write_record('islandora_sync_xml_ds_namespaces', $record);
    }
  }
  drupal_set_message(t('Datastream settings saved'));
  $form_state['redirect'] = 'admin/islandora/tools/sync/xml-datastreams';
}

/**
 * Callback form for settings deletion.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param string $ds_id
 *   The datastream ID.
 *
 * @return mixed
 *   Form structure.
 */
function islandora_sync_xml_ds_delete($form, $form_state, $ds_id) {
  $confirm_text = t('Delete datastream settings?');
  $form['ds_id'] = array(
    '#type' => 'value',
    '#value' => $ds_id,
  );
  return confirm_form($form, $confirm_text, 'admin/islandora/tools/sync/xml-datastreams');
}

/**
 * Delete settings submit.
 */
function islandora_sync_xml_ds_delete_submit($form, &$form_state) {
  db_query('DELETE FROM {islandora_sync_xml_ds}
    WHERE ds_id = :ds_id',
    array(':ds_id' => $form_state['values']['ds_id'])
  );
  db_query('DELETE FROM {islandora_sync_xml_ds_namespaces}
    WHERE ds_id = :ds_id',
    array(':ds_id' => $form_state['values']['ds_id'])
  );
  drupal_set_message(t('Datastream settings deleted'));
  $form_state['redirect'] = 'admin/islandora/tools/sync/xml-datastreams';
}

/**
 * Normalize a group of fields by removing empty groups.
 */
function islandora_sync_xml_ds_normalize_namespace_values($values) {
  $ret_values = array();
  foreach ($values as $k => $v) {
    $values = (is_array($v)) ? implode('', array_values($v)) : '';
    if (is_numeric($k) && !empty($values)) {
      $ret_values[$k] = $v;
    }
  }
  return $ret_values;
}
