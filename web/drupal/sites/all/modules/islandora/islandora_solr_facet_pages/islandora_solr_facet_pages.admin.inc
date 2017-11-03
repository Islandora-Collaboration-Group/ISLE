<?php

/**
 * @file
 * Admin functionality.
 */

/**
 * Admin form building function.
 */
function islandora_solr_facet_pages_admin_settings($form, &$form_state) {
  // Include admin CSS file.
  $admin_css = drupal_get_path('module', 'islandora_solr_facet_pages') . '/css/islandora_solr_facet_pages.admin.css';
  drupal_add_css($admin_css);

  $form['facet_pages'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Fields.
  $form['facet_pages']['islandora_solr_facet_pages_fields_data'] = array(
    '#type' => 'item',
    '#title' => t('Facet pages'),
    '#description' => t('Configure facet pages. Save settings for additional empty fields.'),
    '#tree' => TRUE,
    '#theme' => 'islandora_solr_facet_pages_admin_table',
  );

  // Get fields from variable.
  $fields_data = variable_get('islandora_solr_facet_pages_fields_data', array());

  // Add 3 empty fields.
  for ($i = 1; $i <= 3; $i++) {
    $fields_data[] = array('');
  }

  $fields = array();
  foreach ($fields_data as $key => $value) {
    $field = array(
      'solr_field' => array(
        '#type' => 'textfield',
        '#default_value' => isset($value['solr_field']) ? $value['solr_field'] : '',
        '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
      ),
      'label' => array(
        '#type' => 'textfield',
        '#default_value' => isset($value['label']) ? $value['label'] : '',
      ),
      'path' => array(
        '#type' => 'textfield',
        '#default_value' => isset($value['path']) ? $value['path'] : '',
      ),
    );
    $fields[] = $field;
  }

  // Add fields.
  $form['facet_pages']['islandora_solr_facet_pages_fields_data']['fields'] = $fields;

  // Limit results per page.
  $form['facet_pages']['islandora_solr_facet_pages_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Results per page'),
    '#size' => 5,
    '#default_value' => variable_get('islandora_solr_facet_pages_limit', 25),
    '#description' => t('The amount of values returned per page. Leave empty to return all fields'),
  );

  // Limit maximum results to be returned.
  $form['facet_pages']['islandora_solr_facet_pages_facet_limit'] = array(
    '#type' => 'select',
    '#title' => t('Maximum searchable facet values'),
    '#multiple' => FALSE,
    '#options' => array(),
    '#default_value' => variable_get('islandora_solr_facet_pages_facet_limit', 100000),
    '#description' => t('The maximum amount of values returned per page. A higher number cause slow page loads.'),
  );
  $options = array(
    1000,
    5000,
    10000,
    100000,
    250000,
    500000,
    750000,
    1000000,
    2500000,
    5000000,
    7500000,
    10000000,
  );

  foreach ($options as $o) {
    $form['facet_pages']['islandora_solr_facet_pages_facet_limit']['#options'][$o] = number_format($o);
  }

  // Display search form on facet page.
  $form['facet_pages']['islandora_solr_facet_pages_search_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Facet search form'),
    '#default_value' => variable_get('islandora_solr_facet_pages_search_form', FALSE),
    '#description' => t('Display a search form on the facet page.'),
  );

  $form['facet_pages']['islandora_solr_facet_pages_lucene_syntax_escape'] = array(
    '#title' => t('Allow the use of lucene syntax string escaping on search terms'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('islandora_solr_facet_pages_lucene_syntax_escape', FALSE),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_solr_facet_pages_search_form"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['facet_pages']['islandora_solr_facet_pages_lucene_escape_regex'] = array(
    '#type' => 'textfield',
    '#title' => t('Default regular expression evaluated on search term'),
    '#default_value' => variable_get('islandora_solr_facet_pages_lucene_regex_default', ISLANDORA_SOLR_QUERY_FACET_LUCENE_ESCAPE_REGEX_DEFAULT),
    '#description' => t(
      "The default regular expression, used to escape characters when found in search terms. Defaults to @regex",
      array(
        '@regex' => ISLANDORA_SOLR_QUERY_FACET_LUCENE_ESCAPE_REGEX_DEFAULT,
      )
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_solr_facet_pages_lucene_syntax_escape"]' => array('checked' => TRUE),
        ':input[name="islandora_solr_facet_pages_search_form"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 50,
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 51,
  );

  return $form;
}

/**
 * Theme callback.
 */
function theme_islandora_solr_facet_pages_admin_table(&$vars) {
  // Set variable.
  $rows = array();
  $form = $vars['form'];

  // Render islandora_solr_primary_display_table.
  foreach ($form['fields'] as $key => $elements) {

    $row = array();
    // Check if $key is really the array we need. we shouldn't select the
    // #parent array for example.
    if (is_array($elements) && element_child($key)) {

      $row[] = array('data' => drupal_render($form['fields'][$key]['solr_field']));
      $row[] = array('data' => drupal_render($form['fields'][$key]['label']));
      $row[] = array('data' => drupal_render($form['fields'][$key]['path']));
      $rows[] = $row;
    }
  }

  // Individual table headers.
  $header = array();
  $header[] = array('data' => t('Solr field'));
  $header[] = array('data' => t('Label'));
  $header[] = array('data' => t('Path'));

  // Render table.
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'islandora-solr-facet-pages-admin-table',
    ),
  ));

  return $output;
}

/**
 * Implements hook_validate().
 */
function islandora_solr_facet_pages_admin_settings_validate($form, &$form_state) {

  // On save.
  if ($form_state['clicked_button']['#id'] == 'edit-submit') {

    // Check for valid paths.
    foreach ($form_state['values']['islandora_solr_facet_pages_fields_data']['fields'] as $key => $value) {
      if (!preg_match("/^[a-zA-Z0-9-_]*$/", $value['path'])) {
        form_set_error('islandora_solr_facet_pages_fields_data][fields][' . $key . '][path', t('The path can only contain the following characters: a-z, A-Z, 0-9, - and _'));
      }
    }

    // Get limit value.
    $limit = $form_state['values']['islandora_solr_facet_pages_limit'];
    $limit = trim($limit);
    // Check numeric.
    if (!is_numeric($limit)) {
      form_set_error('islandora_solr_facet_pages_limit', t('Exposed results must be numeric.'));
    }
    // Check for no decimals.
    if (strpos($limit, '.')) {
      form_set_error('islandora_solr_facet_pages_limit', t('Exposed results cannot be decimals.'));
    }
  }
}

/**
 * Implements hook_submit().
 */
function islandora_solr_facet_pages_admin_settings_submit($form, &$form_state) {

  // Get operation.
  $op = $form_state['triggering_element']['#id'];

  switch ($op) {
    case 'edit-submit':
      // Set variables.
      // Clean up array.
      $fields_data = $form_state['values']['islandora_solr_facet_pages_fields_data']['fields'];
      foreach ($fields_data as $key => $value) {
        if (empty($value['solr_field']) AND empty($value['label']) AND empty($value['path'])) {
          unset($fields_data[$key]);
        }
      }
      variable_set('islandora_solr_facet_pages_fields_data', $fields_data);
      variable_set('islandora_solr_facet_pages_limit', trim($form_state['values']['islandora_solr_facet_pages_limit']));
      variable_set('islandora_solr_facet_pages_facet_limit', $form_state['values']['islandora_solr_facet_pages_facet_limit']);
      variable_set('islandora_solr_facet_pages_search_form', $form_state['values']['islandora_solr_facet_pages_search_form']);

      variable_set('islandora_solr_facet_pages_lucene_regex_default', $form_state['values']['islandora_solr_facet_pages_lucene_escape_regex']);
      variable_set('islandora_solr_facet_pages_lucene_syntax_escape', $form_state['values']['islandora_solr_facet_pages_lucene_syntax_escape']);

      drupal_set_message(t('The configuration options have been saved.'));
      break;

    case 'edit-reset':
      // Empty variables.
      // Remove variables.
      $variables = array(
        'islandora_solr_facet_pages_fields_data',
        'islandora_solr_facet_pages_limit',
        'islandora_solr_facet_pages_facet_limit',
      );
      foreach ($variables as $variable) {
        variable_del($variable);
      }

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }
}
