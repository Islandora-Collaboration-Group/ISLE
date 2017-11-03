<?php

/**
 * @file
 * Handles islandora-specific configuration for webforms.
 */

/**
 * Create a multistep islandora configuration form webforms.
 * this a wrapper form
 *
 * @param array $form
 *   A drupal form array
 * @param array $form_state
 *   A drupal form_state array
 * @param object $node
 *   The webform node object
 *
 * @return array
 *   A drupal form render array
 */
function islandora_webform_configure_stepped_form($form, &$form_state, $node) {
  if (!isset($form_state['step'])) { 
    $form_state['step'] = 'configure'; 
  }
  if ($form_state['step'] == 'configure') {
     return islandora_webform_configure_form($form, $form_state, $node);
  }
  return $form;
}


/**
 * Create a islandora configuration page for webforms.
 *
 * @param array $form
 *   A drupal form array
 * @param array $form_state
 *   A drupal form_state array
 * @param object $node
 *   The webform node object
 *
 * @return array
 *   A drupal form render array
 */
function islandora_webform_configure_form($form, &$form_state, $node) {

  $form['#submit'] = array(
    'islandora_webform_configure_form_submit',
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $form_state['create_islandora_webform_data'] = FALSE;
  if (!empty($node->nid)) {
    $defaults = db_select('islandora_webform_webforms', 'w')
      ->fields('w')
      ->condition('w.entity_id', $node->nid)
      ->execute()->fetchAssoc();
  }
  if (empty($defaults)) {
    $form_state['create_islandora_webform_data'] = TRUE;

    $defaults = array(
      'enabled' => variable_get('islandora_webform_default_enabled', 1),
      'cmodel_filter' => variable_get('islandora_webform_default_cmodel_filter', ''),
      'collection_filter' => variable_get('islandora_webform_default_collection_filter', ''),
      'pid_filter' => variable_get('islandora_webform_default_pid_filter', ''),
      'scope' => variable_get('islandora_webform_default_scope', 'tagged'),
      'link_text' => variable_get('islandora_webform_default_link_text', 'Go to web form'),
      'link_help' => variable_get('islandora_webform_default_link_help', 'Your input is requested!'),
    );
  }

  $cmodel_options = array('' => '- Any -');
  form_load_include($form_state, 'inc', 'islandora', 'includes/utilities');
  $cmodels = islandora_get_content_models();
  asort($cmodels);
  foreach ($cmodels as $cmodel) {
    $cmodel_options[$cmodel['pid']] = $cmodel['label'];
  }

  $collection_options = array('' => '- Any -');
  form_load_include($form_state, 'inc', 'islandora_webform', 'includes/utilities');
  $collections = iw_get_collections_or_content_models('collections');
  asort($collections);
  $collection_options = array_merge($collection_options, $collections);

  $form['islandora'] = array(
    '#type' => 'fieldset',
    '#title' => 'Islandora Settings',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#group' => 'additional_settings',
    'enabled' => array(
      '#type' => 'checkbox',
      '#title' => 'Enabled',
      '#description' => t('Check to permit this webform to be linked from Islandora objects'),
      '#default_value' => $defaults['enabled'],
    ),
    'cmodel_filter' => array(
      '#type' => 'select',
      '#title' => t('Content model filter'),
      '#description' => '',
      '#default_value' => $defaults['cmodel_filter'],
      '#options' => $cmodel_options,
    ),
    'collection_filter' => array(
      '#type' => 'select',
      '#title' => t('Collection filter'),
      '#description' => '',
      '#default_value' => $defaults['collection_filter'],
      '#options' => $collection_options,
    ),
    'pid_filter' => array(
      '#type' => 'textfield',
      '#title' => 'PID Search String',
      '#description' => t('Enter a regex pattern to match against Fedora object PIDs (optional)'),
      '#maxlength' => 200,
      '#default_value' => $defaults['pid_filter'],
    ),
    'scope' => array(
      '#type' => 'select',
      '#title' => t('Add a link to...'),
      '#description' => '',
      '#default_value' => $defaults['scope'],
      '#options' => array(
        'all' => 'All matched objects',
        'tagged' => 'Only those objects that are manually tagged for this webform',
      ),
    ),
    'link_text' => array(
      '#type' => 'textfield',
      '#title' => 'Link text',
      '#description' => t('The text that will appear in the link to this webform'),
      '#maxlength' => 60,
      '#default_value' => $defaults['link_text'],
    ),
    'link_help' => array(
      '#type' => 'textfield',
      '#title' => 'Link help text',
      '#description' => t('The text that will appear as help/description text for the link'),
      '#maxlength' => 2000,
      '#default_value' => $defaults['link_help'],
    ),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler for islandora webform configuration page.
 *
 * @param object $node
 *   The webform node
 * @param array $form_state
 *   The Drupal form_state array
 */
function islandora_webform_configure_form_submit($node, &$form_state) {

  $nid = $node['nid']['#value'];
  _islandora_webform_insert_pid_component($nid);
  if (db_merge('islandora_webform_webforms')
    ->key(array('entity_id' => $nid))
    ->fields(array(
      'enabled' => $form_state['values']['islandora']['enabled'],
      'cmodel_filter' => $form_state['values']['islandora']['cmodel_filter'],
      'collection_filter' => $form_state['values']['islandora']['collection_filter'],
      'pid_filter' => $form_state['values']['islandora']['pid_filter'],
      'scope' => $form_state['values']['islandora']['scope'],
      'link_text' => $form_state['values']['islandora']['link_text'],
      'link_help' => $form_state['values']['islandora']['link_help'],
    ))
    ->execute()
  ) {
    drupal_set_message(t('Islandora configuration saved.'));
  }
  else {
    drupal_set_message(t('Islandora configuration could not be saved.'));
  }
}


/**
 * Adds an islandora_object_pid textfield to a webform.
 *
 * Only add if one does not already exist.
 *
 * @param int $nid
 *   The node id for the webform
 *
 * @throws Exception
 */
function _islandora_webform_insert_pid_component($nid) {

  $webform = node_load($nid);
  if ($webform) {
    $component = FALSE;

    // Look to see if the component already exists, if so, then do nothing.
    if (!empty($webform->webform['components'])) {
      foreach ($webform->webform['components'] as $test_component) {
        if ($test_component['form_key'] == 'islandora_object_pid') {
          $component = $test_component;
          break;
        }
      }
    }

    // If no component found, then add it and save.
    if (!$component) {
      $component = array(
        'name' => 'Islandora object PID',
        'form_key' => 'islandora_object_pid',
        'type' => 'textfield',
        'mandatory' => 1,
        'weight' => 15,
        'extra' => array(
          'title_display' => 'inline',
          'private' => 0,
        ),
      );
      webform_component_defaults($component);

      $webform->webform['components'][] = $component;
      node_save($webform);
    }
  }
}
