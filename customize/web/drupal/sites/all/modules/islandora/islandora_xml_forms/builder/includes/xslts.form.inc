<?php

/**
 * @file
 * Holds XSLT and default DC mapping administration forms.
 */

/**
 * Default DC transforms form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form.
 */
function xml_form_builder_xslts_form($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');

  $form['xml_form_builder_xslts_vertical_tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['xml_form_builder_xslts_vertical_tabs']['xslts'] = array(
    '#type' => 'fieldset',
    '#title' => t('XSLTs'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['xml_form_builder_xslts_vertical_tabs']['dc_transforms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default DC Transforms'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $xslts = xml_form_builder_get_xslts();
  $xslts_options = array();
  foreach ($xslts as $xslt) {
    $xslts_options[$xslt['xslt_id']] = array($xslt['name']);
  }
  $xslts_header = array('XSLT');

  $mappings = xml_form_builder_get_default_dc_xslt_mappings_with_xslt_name();
  $mappings_options = array();
  foreach ($mappings as $mapping) {
    $mappings_options[$mapping['id']] = array(
      $mapping['name'],
      $mapping['content_model'],
      $mapping['dsid'],
      $mapping['xslt_name'],
    );
  }
  $mappings_header = array('Default', 'Content Model', 'DSID', 'XSLT');

  $form['xml_form_builder_xslts_vertical_tabs']['xslts']['xslts_table'] = array(
    '#type' => 'tableselect',
    '#header' => $xslts_header,
    '#options' => $xslts_options,
    '#empty' => t('No XSLTs available.'),
  );
  $form['xml_form_builder_xslts_vertical_tabs']['xslts']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete XSLT'),
  );

  $form['xml_form_builder_xslts_vertical_tabs']['dc_transforms']['transforms_table'] = array(
    '#type' => 'tableselect',
    '#header' => $mappings_header,
    '#options' => $mappings_options,
    '#empty' => t('No defaults set.'),
  );
  $form['xml_form_builder_xslts_vertical_tabs']['dc_transforms']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Default'),
  );

  return $form;
}

/**
 * Default DC transforms form submit handler.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_xslts_form_submit($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');
  if (in_array('dc_transforms', $form_state['clicked_button']['#array_parents'])) {
    foreach ($form_state['values']['transforms_table'] as $uid => $delete) {
      if ($delete) {
        xml_form_builder_remove_default_dc_transform_mapping($uid);
      }
    }
  }
  elseif (in_array('xslts', $form_state['clicked_button']['#array_parents'])) {
    foreach ($form_state['values']['xslts_table'] as $uid => $delete) {
      if ($delete) {
        xml_form_builder_remove_xslt($uid);
      }
    }
  }
}

/**
 * XSLT form validator.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_xslts_form_validate($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');
  if (in_array('xslts', $form_state['clicked_button']['#array_parents'])) {
    foreach ($form_state['values']['xslts_table'] as $uid => $delete) {
      if ($delete) {
        if (xml_form_builder_xslt_is_a_default($uid)) {
          if (!isset($message_set)) {
            form_set_error('xslts_table', t('Please delete any transforms using the XSLTs first.'));
            $message_set = TRUE;
          }
        }
      }
    }
    if (!array_filter($form_state['values']['xslts_table'])) {
      form_set_error('xslts_table', t('Please select an XSLT to delete.'));
    }
  }
  elseif (in_array('dc_transforms', $form_state['clicked_button']['#array_parents'])) {
    if (!array_filter($form_state['values']['transforms_table'])) {
      form_set_error('transforms_table', t('Please select a transform to delete.'));
    }
  }
}

/**
 * Add an XSLT form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form.
 */
function xml_form_builder_add_xslt_form($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');

  $form['name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
  );
  $form['xslt'] = array(
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#title' => t('XSLT'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl xslt'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Add an XSLT form submit handler.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_add_xslt_form_submit($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');
  $file = file_load($form_state['values']['xslt']);
  xml_form_builder_add_xslt(
    file_get_contents($file->uri),
    $form_state['values']['name']
  );
  file_delete($file);
  $form_state['redirect'] = XML_FORM_BUILDER_XSLTS_MENU;
}

/**
 * Add a default DC transform form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form.
 */
function xml_form_builder_add_default_dc_transform_mapping_form($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');
  $xslts = xml_form_builder_get_xslts();
  $options = array();
  foreach ($xslts as $xslt) {
    $options[$xslt['xslt_id']] = $xslt['name'];
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
  );
  $form['content_model'] = array(
    '#type' => 'textfield',
    '#title' => t('Content Model'),
    '#required' => TRUE,
    '#autocomplete_path' => ISLANDORA_CONTENT_MODELS_AUTOCOMPLETE,
  );
  $form['DSID'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('DSID'),
  );
  $form['xslt'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#required' => TRUE,
    '#title' => t('XSLT'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  return $form;
}

/**
 * Validate the default DC transform form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_add_default_dc_transform_mapping_form_validate($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');
  if (xml_form_builder_get_default_transform($form_state['values']['content_model'], $form_state['values']['DSID'])) {
    form_set_error('content_model', t('There already exists a mapping for this content model and DSID.'));
    form_set_error('DSID', '');
  }
}

/**
 * Add a default DC transform form submit handler.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form.
 */
function xml_form_builder_add_default_dc_transform_mapping_form_submit($form, &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');
  xml_form_builder_add_default_dc_transform_mapping(
    $form_state['values']['content_model'],
    $form_state['values']['DSID'],
    $form_state['values']['xslt'],
    $form_state['values']['name']
  );
  $form_state['redirect'] = XML_FORM_BUILDER_XSLTS_MENU;
}
