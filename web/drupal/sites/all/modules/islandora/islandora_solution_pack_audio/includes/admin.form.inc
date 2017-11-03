<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_audio_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  $lame = $get_default_value('islandora_lame_url', '/usr/bin/lame');
  $form['derivatives'] = array(
    '#type' => 'fieldset',
    '#title' => t('Derivatives'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['derivatives'] += array(
    'islandora_audio_defer_derivatives_on_ingest' => array(
      '#type' => 'checkbox',
      '#title' => t('Defer audio derivative generation during ingest'),
      '#description' => t('Prevent audio derivatives from running during ingest,
            useful if derivatives are to be created by an external service.'),
      '#default_value' => variable_get('islandora_audio_defer_derivatives_on_ingest', FALSE),
    ),
    'islandora_lame_url' => array(
      '#type' => 'textfield',
      '#title' => t("Path to LAME"),
      '#description' => t('!LAME is required to create derivatives files.<br/>!msg',
                      array(
                        '!LAME' => l(t('LAME'), 'http://lame.sourceforge.net/'),
                        '!msg' => islandora_executable_available_message($lame),
                      )),
      '#default_value' => $lame,
      '#size' => 20,
      '#prefix' => '<div id="lame-wrapper">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'islandora_audio_admin_form_lame_ajax_callback',
        'wrapper' => 'lame-wrapper',
        'effect' => 'fade',
        'event' => 'change',
      ),
    ),
    'islandora_audio_vbr_quality' => array(
      '#type' => 'textfield',
      '#title' => t('MP3 derivative quality'),
      '#description' => t('Variable Bit Rate quality setting (0=highest quality, 9.999=lowest). Default = 5.'),
      '#size' => 5,
      '#default_value' => variable_get('islandora_audio_vbr_quality', 5),
      '#element_validate' => array('element_validate_number', 'islandora_audio_vbr_quality_validate'),

    ),
    'islandora_audio_obj_fallback' => array(
      '#type' => 'checkbox',
      '#title' => t('Use original file as fallback'),
      '#description' => t('Attempt to play the OBJ datastream in the player if the PROXY_MP3 derivative is not present.'),
      '#default_value' => $get_default_value('islandora_audio_obj_fallback', TRUE),
    ),
  );
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_audio_viewers', 'audio/mpeg');
  return system_settings_form($form);
}

/**
 * Ajax callback for the Lame textfield.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The element to render as part the ajax callback.
 */
function islandora_audio_admin_form_lame_ajax_callback(array $form, array $form_state) {
  return $form['islandora_lame_url'];
}

/**
 * Validation callback for the VBR Quality field.
 *
 * @param array $element
 *   The Drupal element definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_audio_vbr_quality_validate(array $element, array &$form_state) {
  if (!empty($element['#value']) || $element['#value'] === "0") {
    $quality = floatval($element['#value']);
    if ($quality >= 0 && $quality <= 9.999) {
      return;
    }
    else {
      form_error($element, t('Please enter a VBR quality value between 0 and 9.999.'));
    }
  }
  else {
    form_set_value($element, 5, $form_state);
  }

}
