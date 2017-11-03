<?php

/**
 * @file
 * Populator selection form and associated callbacks.
 */

/**
 * Form building function; allow a "populator" to be selected.
 */
function islandora_populator_select_form($form, &$form_state, array $populators) {
  $form['selected'] = array(
    '#title' => t('Prepopulate metadata from source:'),
    '#type' => 'radios',
    '#multiple' => FALSE,
    '#options' => $populators,
    '#default_value' => 'default',
  ) + element_info('radios');
  array_unshift($form['selected']['#process'], 'islandora_populator_expand_radios');

  return $form;
}

/**
 * Process "#options" before core "radios" processing.
 *
 * Wanting to add descriptions... Great fun!
 */
function islandora_populator_expand_radios($element) {
  foreach ($element['#options'] as $key => &$values) {
    $element[$key]['#description'] = isset($values['description']) ?
      $values['description'] :
      t('No description available.');
    $values = $values['title'];
  }
  unset($values);

  return $element;
}

/**
 * Form submission handler; stash the selected "populator".
 */
function islandora_populator_select_form_submit(&$form, &$form_state) {
  $step_storage =& islandora_ingest_form_get_step_storage($form_state);

  $step_storage['selected'] = $form_state['values']['selected'];
}
