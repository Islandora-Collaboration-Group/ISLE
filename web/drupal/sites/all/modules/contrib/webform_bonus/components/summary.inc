<?php

/**
 * @file
 *   Webform module summary component.
 */

/**
 * Create a default summary component.
 */
function _webform_defaults_summary() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'components' => '',
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_summary($component) {
  $form = array();

  $node = node_load($component['nid']);
  $options = array();
  foreach ($node->webform['components'] as $id => $c) {
    $options[$id] = $c['name'];
  }

  $form['extra']['components'] = array(
    '#type' => 'select',
    '#title' => t('Select components to show in summary'),
    '#default_value' => $component['extra']['components'],
    '#options' => $options,
    '#multiple' => TRUE,
    '#description' => t('Select component to which current component should be mapped'),
  );
  
  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_summary($component) {
  $element = array(
    '#type' => 'item',
    '#weight' => $component['weight'],
    '#title' => $component['extra']['title_display'] == 'none' ? '' : $component['name'],
    '#markup' => '',
    '#cid' => $component['cid'],
    '#after_build' => array('_set_summary'),  //Set the value property by calling _set_summary
  );      
  return $element;
}

/**
 * Form after_build callback function. Set the value property of the form item according to the submitted values.
 */
function _set_summary($form_element, &$form_state) {
  global $user;
  module_load_include('inc', 'webform', 'includes/webform.submissions');
  
  $submitted = $form_state['storage']['submitted'];
  $node = clone $form_state['build_info']['args'][0];
  $allowed_components = $node->webform['components'][$form_element['#cid']]['extra']['components'];

  // Show only selected components
  foreach ($submitted as $key => $component) {
    if (!in_array($key, $allowed_components)) {
      unset($submitted[$key]);
    }
  }
  foreach ($node->webform['components'] as $key => $component) {
    if (!in_array($key, $allowed_components)) {
      unset($node->webform['components'][$key]);
    }
  }

  $submission = (object) array(
    'nid' => $node->nid,
    'uid' => $user->uid,
    'submitted' => REQUEST_TIME,
    'remote_addr' => ip_address(),
    'is_draft' => TRUE,
    'data' => webform_submission_data($node, $submitted),
  ); 
  $form_element['#markup'] = webform_submission_render($node, $submission, NULL, 'html');
  return $form_element;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_summary($section) {
  switch ($section) {
    case 'admin/settings/webform#summary_description':
      return t('Displays previously submitted values in the form; does not render a field. Only values of fields contained in a fieldgroup on the first level are displayed, the name of the fieldgroup is used as caption.');
  }
}