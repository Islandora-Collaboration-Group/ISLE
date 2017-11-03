<?php

/**
 * @file
 * includes/admin.form.inc
 *
 * Module admin form.
 */

/**
 * Module admin form.
 */
function islandora_compound_object_admin_form($form, &$form_state) {

  $backend_options = module_invoke_all('islandora_compound_object_query_backends');
  $map_to_title = function ($backend) {
    return $backend['title'];
  };

  $form = array();

  $form['islandora_compound_object_compound_children'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only allow compound objects to have child objects associated with them'),
    '#description' => t('If unchecked, all objects may have child objects.'),
    '#default_value' => variable_get('islandora_compound_object_compound_children', TRUE),
  );

  $form['islandora_compound_object_thumbnail_child'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate a thumbnail for compound objects from their first child'),
    '#description' => t('If checked, the thumbnail for a compound object will be generated from its first child object.'),
    '#default_value' => variable_get('islandora_compound_object_thumbnail_child', TRUE),
  );

  $form['islandora_compound_object_hide_child_objects_ri'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide child objects in RI results'),
    '#description' => t('If checked, child objects will be hidden. Only visible within the compound context.'),
    '#default_value' => variable_get('islandora_compound_object_hide_child_objects_ri', TRUE),
  );

  $form['islandora_compound_object_hide_child_objects_solr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide child objects in Solr results'),
    '#description' => t('If checked, child objects will be hidden. Only visible within the compound context.'),
    '#default_value' => variable_get('islandora_compound_object_hide_child_objects_solr', TRUE),
  );

  $form['islandora_compound_object_solr_fq'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr filter query'),
    '#description' => t('Filter compound object children from Solr results.'),
    '#default_value' => variable_get('islandora_compound_object_solr_fq', '-RELS_EXT_isConstituentOf_uri_mt:[* TO *]'),
    '#element_validate' => array('islandora_compound_object_solr_fq_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_compound_object_hide_child_objects_solr"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="islandora_compound_object_hide_child_objects_solr"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['islandora_compound_object_relationship'] = array(
    '#type' => 'textfield',
    '#title' => t('Child relationship predicate'),
    '#description' => t('Changing this after objects have been created will break functionality. Should be part of info:fedora/fedora-system:def/relations-external#'),
    '#default_value' => variable_get('islandora_compound_object_relationship', 'isConstituentOf'),
    '#required' => TRUE,
  );

  $form['islandora_compound_object_use_jail_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use alternative, autoloading display for compounds?'),
    '#description' => t('<b>Requires</b> <a href="@url">JAIL</a> library to be present.', array(
      '@url' => url('https://github.com/sebarmeli/JAIL', array('absolute' => TRUE)),
    )),
    '#default_value' => variable_get('islandora_compound_object_use_jail_view', FALSE),
    '#element_validate' => array('islandora_compound_object_admin_form_jail_validation'),
  );

  $form['islandora_compound_object_tn_deriv_hooks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use derivative hooks for parent thumbnail updates?'),
    '#description' => t('Changes to the TN datastream of a compounds first child will be reflected on the parent.'),
    '#default_value' => variable_get('islandora_compound_object_tn_deriv_hooks', FALSE),
  );

  $form['islandora_compound_object_show_compound_parents_in_breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display compound object parents in the breadcrumbs on children objects.'),
    '#default_value' => variable_get('islandora_compound_object_show_compound_parents_in_breadcrumbs', FALSE),
  );

  $form['islandora_compound_object_redirect_to_first'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect to first child when a compound object is requested.'),
    '#description' => t('Users will be redirected to the first child of a Compound Object when enabling this setting.'),
    '#default_value' => variable_get('islandora_compound_object_redirect_to_first', FALSE),
    '#element_validate' => array('islandora_compound_object_admin_form_redirect_to_first_validation'),
  );

  $form['islandora_compound_object_query_backend'] = array(
    '#type' => 'radios',
    '#title' => t('Compound Member Query'),
    '#description' => t('Select the method that will be used to find the children of the compound objects.'),
    '#options' => array_map($map_to_title, $backend_options),
    '#default_value' => variable_get('islandora_compound_object_query_backend', ISLANDORA_COMPOUND_OBJECT_LEGACY_BACKEND),
  );

  return system_settings_form($form);
}

/**
 * Check if the JAIL is available.
 *
 * @param array $element
 *   The element to check.
 * @param array $form_state
 *   The Drupal form state.
 * @param array $form
 *   The Drupal form definition.
 */
function islandora_compound_object_admin_form_jail_validation(array $element, array &$form_state, array $form) {
  if ($element['#value'] == 1 && !libraries_get_path('JAIL')) {
    $element['#value'] = 0;
    form_set_value($element, $element['#value'], $form_state);
    drupal_set_message(t('The <a href="@url">JAIL</a> library must be present in the libraries folder to use this display.', array(
      '@url' => url('https://github.com/sebarmeli/JAIL', array('absolute' => TRUE)),
    )), 'error');
  }
}

/**
 * Warn if both redirect to first and compound parent in breadcrumb are enabled.
 *
 * @param array $element
 *   The element to check.
 * @param array $form_state
 *   The Drupal form state.
 * @param array $form
 *   The Drupal form definition.
 */
function islandora_compound_object_admin_form_redirect_to_first_validation(array $element, array &$form_state, array $form) {
  if ($element['#value'] == 1) {
    if (isset($form_state['values']['islandora_compound_object_show_compound_parents_in_breadcrumbs'])
        && $form_state['values']['islandora_compound_object_show_compound_parents_in_breadcrumbs'] == 1) {
      $first_title = rtrim($element['#title'], '.!?');
      $parent_title = rtrim($form['islandora_compound_object_show_compound_parents_in_breadcrumbs']['#title'], '.!?');
      drupal_set_message(t("Enabling both %first and %parent is not recommended, because the compound object parent in the breadcrumb will be redirect to the first child, so cannot be actually reached.",
        array('%first' => $first_title, '%parent' => $parent_title)), 'warning');
    }
  }
}

/**
 * Solr fq element state validation callback.
 *
 * @param array $element
 *   The form element.
 * @param array $form_state
 *   The form state.
 */
function islandora_compound_object_solr_fq_validate($element, &$form_state) {
  if ($form_state['values']['islandora_compound_object_hide_child_objects_solr'] &&
    empty($form_state['values']['islandora_compound_object_solr_fq'])) {
    form_error($element, t('Solr filter query field is required.'));
  }
}
