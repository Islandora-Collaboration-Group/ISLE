<?php

/**
 * @file
 * Webform module mapping component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_mapping() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'items' => '',
      'mapped_component' => ''
    ),
  );
}

/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_mapping() {
  return array(
    'webform_display_mapping' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_mapping($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#weight' => 0,
  );

  $node = node_load($component['nid']);
  $options = array();
  foreach ($node->webform['components'] as $id => $c) {
    $options[$id] = $c['name'];
  }

  $form['extra']['mapped_component'] = array(
    '#type' => 'select',
    '#title' => t('Select Mapped component'),
    '#default_value' => $component['extra']['mapped_component'],
    '#options' => $options,
    '#description' => t('Select component to which current component should be mapped'),
  );

  $form['extra']['items'] = array(
    '#type' => 'textarea',
    '#title' => t('Mapping options'),
    '#default_value' => $component['extra']['items'],
    '#description' => t('<strong>Key-value pairs MUST be specified as "value of mapped component|return value"</strong>. One pair per line.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
    '#element_validate' => array('_webform_edit_validate_mapping'),
  );
  
  $form['extra']['description'] = array(); // Hide the description box.
  $form['display'] = array('#type' => 'markup'); // Hide the display options.
  $form['display']['title_display'] = array();
  return $form;
}


/**
 * Element validation callback. Ensure keys are not duplicated.
 */
function _webform_edit_validate_mapping($element, &$form_state) {
  // Check for duplicate key values to prevent unexpected data loss. Require
  // all options to include a safe_key.
  if (!empty($element['#value'])) {
    $lines = explode("\n", trim($element['#value']));
    $missing_keys = array();
    $long_keys = array();
    $group = '';
    foreach ($lines as $line) {
      $matches = array();
      $line = trim($line);
      if (preg_match('/^\<([^>]*)\>$/', $line, $matches)) {
        $group = $matches[1];
        $key = NULL; // No need to store group names.
      }
      elseif (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
        $key = $matches[1];
        if (strlen($key) > 128) {
          $long_keys[] = $key;
        }
      }
      else {
        $missing_keys[] = $line;
      }
    }

    if (!empty($missing_keys)) {
      form_error($element, t('Every option must have a key specified. Specify each option as "safe_key|Some readable option".'));
    }

    if (!empty($long_keys)) {
      form_error($element, t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys));
    }

  }

  return TRUE;
}


/**
 * Implementation of _webform_render_component().
 */
function _webform_render_mapping($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type' => 'hidden',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#default_value' => $filter ? _webform_filter_values($component['value']) : $component['value'],
    '#weight' => $component['weight'],
  );

  if (isset($value[0])) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}


/**
 * Implementation of _webform_display_component().
 */
function _webform_display_mapping($component, $value, $format = 'html') {
  $element = array(
    '#title' => t('!name (hidden)', array('!name' => $component['name'])),
    '#markup' => isset($value[0]) ? $value[0] : NULL,
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_mapping',
    '#format' => $format,
    '#theme' => 'webform_display_mapping',
    '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
    '#webform_component' => $component,
  );

  // TODO: This check is unusual. It shows hidden fields in e-mails but not
  // when viewing in the browser unless you're an administrator. This should be
  // a more logical check. See these related issues:
  // http://drupal.org/node/313639
  // http://drupal.org/node/781786
  if ($format == 'html') {
    $element['#access'] = user_access('edit all webform submissions') || user_access('access all webform results');
  }

  return $element;
}

function theme_webform_display_mapping($variables) {
  $element = $variables['element'];
  
  return $element['#format'] == 'html' ? check_plain($element['#markup']) : $element['#markup'];
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_mapping($component, $sids = array()) {
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (" . implode(",", $placeholders) . ")" : "";
  $query = 'SELECT data ' .
    ' FROM {webform_submitted_data} ' .
    ' WHERE nid = %d ' .
    ' AND cid = %d ' . $sidfilter;
  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  while ($data = db_fetch_array($result)) {
    if ( strlen(trim($data['data'])) > 0 ) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }

  $rows[0] = array( t('Empty'), ($submissions - $nonblanks));
  $rows[1] = array( t('Non-empty'), $nonblanks);
  $rows[2] = array( t('Average submission length in words (ex blanks)'),
                    ($nonblanks !=0 ? number_format($wordcount/$nonblanks, 2) : '0'));
  return $rows;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_table_mapping($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_headers_mapping($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_mapping($component, $export_options, $value) {
  return empty($value[0]) ? '' : $value[0];
}
