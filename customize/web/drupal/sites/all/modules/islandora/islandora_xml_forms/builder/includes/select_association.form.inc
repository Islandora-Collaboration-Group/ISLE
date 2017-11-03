<?php

/**
 * @file
 * Select association form.
 */

/**
 * The select association form.
 *
 * This is used as part of the ingest process.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param array $associations
 *   The form associations to select from.
 *
 * @return array
 *   A Drupal form definition.
 */
function xml_form_builder_select_association_form(array $form, array &$form_state, array $associations) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/select_association.form');
  $get_default = function($name, $default) use(&$form_state) {
    // @XXX: This used to be using input instead of values but it has been
    // switched back. The original input change was to fix a problem with
    // AJAX not populating the dependent drop down at ingest time as of
    // the 7.x-1.2 release. If this happens again this is the code to look at.
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : $default;
  };
  $form_state['associations'] = $associations;
  $models = xml_form_builder_select_association_form_get_models($associations);
  $selected_model = $get_default('models', key($models));
  $forms = xml_form_builder_select_association_form_get_forms($associations, $selected_model);
  $selected_form = $get_default('available_forms', key($forms));
  // We check if the form exists in case the model was changed.
  $selected_form = in_array($selected_form, $forms) ? $selected_form : $forms[current($forms)];

  $multiple_forms = count($forms) > 1;
  $multiple_models = count($models) > 1;
  return array(
    'models' => array(
      '#title' => t('Select a Content Model'),
      '#type' => 'select',
      '#access' => $multiple_models,
      '#options' => $models,
      '#default_value' => $selected_model,
      '#ajax' => array(
        'callback' => 'xml_form_builder_select_association_form_ajax_callback',
        'wrapper' => 'forms_wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    ),
    'available_forms' => array(
      '#access' => $multiple_forms,
      '#title' => t('Select a Form'),
      '#prefix' => '<div id="forms_wrapper">',
      '#suffix' => '</div>',
      '#type' => 'radios',
      '#options' => $forms,
      '#default_value' => $selected_form,
    ),
    'association' => array(
      '#type' => 'value',
      '#default_value' => current($associations),
      '#value_callback' => 'xml_form_builder_select_association_form_value_callback',
    ),
  );
}

/**
 * Get the models to select from.
 *
 * @param array $associations
 *   The form associations to select from.
 *
 * @return array
 *   An associative array where the keys is the model ID and the value is the
 *   models label.
 */
function xml_form_builder_select_association_form_get_models(array $associations) {
  $get_model = function($o) {
    return $o['content_model'];
  };
  $models = array_map($get_model, $associations);
  $models = array_unique($models);
  $get_label = function($o) {
    return islandora_object_load($o)->label;
  };
  $model_labels = array_map($get_label, $models);
  return array_combine($models, $model_labels);
}

/**
 * Get the forms to select from for the given content model.
 *
 * @param array $associations
 *   The form associations to select from.
 *
 * @return array
 *   An associative array where the keys/value is the form name.
 */
function xml_form_builder_select_association_form_get_forms(array $associations, $selected_model) {
  $get_selected_models = function($o) use($selected_model) {
    return $o['content_model'] == $selected_model;
  };
  $selected_model_associations = array_filter($associations, $get_selected_models);
  $get_form = function($o) {
    return $o['form_name'];
  };
  $forms = array_map($get_form, $selected_model_associations);
  return drupal_map_assoc($forms);
}

/**
 * The AJAX callback for the models select field.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   A Drupal form definition.
 */
function xml_form_builder_select_association_form_ajax_callback(array $form, array $form_state) {
  if ($form['available_forms']['#access']) {
    return $form['available_forms'];
  }
  else {
    return array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#attributes' => array(
        'id' => 'forms_wrapper',
      ),
    );
  }
}

/**
 * Populates the association field.
 *
 * @param array $element
 *   The assocoiation element.
 * @param mixed $input
 *   The input for the element.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_select_association_form_value_callback(array $element, $input, array &$form_state) {
  $association_matches = function($o) use($form_state) {
    return $o['content_model'] == $form_state['values']['models'] && $o['form_name'] == $form_state['values']['available_forms'];
  };
  $filtered_associations = array_filter($form_state['associations'], $association_matches);
  return array_pop($filtered_associations);
}

/**
 * Set the selected association in both the association/metadata steps.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_select_association_form_submit(array $form, array &$form_state) {
  $association_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_association_step');
  $association_step_storage['association'] = $form_state['values']['association'];
}

/**
 * Undo's any changes the regular submit handler did.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_select_association_form_undo_submit(array $form, array &$form_state) {
  $association_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_association_step');
  unset($association_step_storage['association']);
}
