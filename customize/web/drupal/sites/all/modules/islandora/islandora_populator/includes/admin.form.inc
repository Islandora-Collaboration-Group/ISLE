<?php
/**
 * @file
 * Admin form functions.
 */

/**
 * Admin form function.
 */
function islandora_populator_admin_form($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $form['islandora_populator_enable_cmodel_limiting'] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit Content Models'),
    '#description' => t("Restrict populator to specified content models."),
    '#default_value' => variable_get('islandora_populator_enable_cmodel_limiting', FALSE),
    // Using ajax because states weren't working for the tableselect.
    '#ajax' => array(
      'callback' => 'islandora_populator_cmodel_ajax',
      'wrapper' => 'enabled_cmodels',
    ),
  );

  $models = islandora_get_content_models();
  ksort($models);
  $form['islandora_populator_enabled_cmodels'] = array(
    '#prefix' => '<div id="enabled_cmodels">',
    '#suffix' => '</div>',
  );
  $form_cmodel_value_isset = isset($form_state['values']['islandora_populator_enable_cmodel_limiting']);
  if ((!$form_cmodel_value_isset && variable_get('islandora_populator_enable_cmodel_limiting', FALSE))
      || ($form_cmodel_value_isset && $form_state['values']['islandora_populator_enable_cmodel_limiting'])) {
    $form['islandora_populator_enabled_cmodels'] = $form['islandora_populator_enabled_cmodels'] + array(
      '#type' => 'tableselect',
      '#title' => t('Content Models'),
      '#description' => t('The populator will be enabled on objects for the models selected.'),
      '#header' => array(
        'pid' => t('PID'),
        'label' => t('Label'),
      ),
      '#options' => $models,
      '#default_value' => variable_get('islandora_populator_enabled_cmodels', array()),
      '#theme_wrappers' => array(
        'form_element',
      ),
    );
  }
  return system_settings_form($form);
}

/**
 * Ajax callback for enabled content model tableselect.
 */
function islandora_populator_cmodel_ajax($form, $form_state) {
  return $form['islandora_populator_enabled_cmodels'];
}
