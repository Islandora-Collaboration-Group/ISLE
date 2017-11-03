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
function islandora_ocr_admin_settings_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_ocr', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  $tesseract = $get_default_value('islandora_ocr_tesseract', '/usr/bin/tesseract');
  $version = islandora_ocr_get_tesseract_version($tesseract);
  $installed_languages = islandora_ocr_get_tesseract_installed_languages($tesseract);
  $enabled_languages = $get_default_value('islandora_ocr_tesseract_enabled_languages', drupal_map_assoc($installed_languages));
  $solr_enabled = module_exists('islandora_solr');
  return system_settings_form(array(
      '#prefix' => '<div id="tesseract-wrapper">',
      '#suffix' => '</div>',
      'islandora_ocr_tesseract' => array(
        '#type' => 'textfield',
        '#title' => t('Tesseract'),
        '#description' => t('Tesseract is used to generate the OCR and coordinate data.<br/>!msg', array(
                          '!msg' => islandora_executable_available_message($tesseract, $version, islandora_ocr_required_tesseract_version()))),
        '#default_value' => $tesseract,
        '#ajax' => array(
          'callback' => 'islandora_ocr_admin_settings_form_ajax_callback',
          'wrapper' => 'tesseract-wrapper',
          'effect' => 'fade',
          'event' => 'change',
        ),
      ),
      'islandora_ocr_tesseract_enabled_languages' => array(
        '#type' => 'checkboxes',
        '#title' => t('Languages available for OCR'),
        '#options' => drupal_map_assoc($installed_languages, 'islandora_ocr_tesseract_language_name'),
        '#description' => t('Select from the language packs available on your
                             processing server.<br/> These are normally found in
                             <strong>/usr/local/share/tessdata/</strong>.<br/>
                             Check with your systems admininistrator if you are
                             unsure of availablilty.'),
        '#default_value' => $enabled_languages,
      ),
      'islandora_ocr_solr_hocr_highlighting_use_fast' => array(
        '#access' => $solr_enabled,
        '#type' => 'checkbox',
        '#title' => t('Enable Solr Fast Vector Highlighting'),
        '#description' => t('This requires the field below to use !fl_link.', array(
                          '!fl_link' => l(t('Fast Vector Highlighting'), 'http://wiki.apache.org/solr/HighlightingParameters', array(
                                        'fragment' => 'hl.useFastVectorHighlighter',
                                      )),
                        )),
        '#default_value' => $get_default_value('islandora_ocr_solr_hocr_highlighting_use_fast', TRUE),
      ),
      'islandora_ocr_solr_hocr_highlighting_field' => array(
        '#access' => $solr_enabled,
        '#type' => 'textfield',
        '#title' => t('Solr field containing OCR text'),
        '#description' => t('The Solr field to use for highlighting.<br/>If Fast Vector Highlighting is enabled, some special requirements must be met.'),
        '#default_value' => $get_default_value('islandora_ocr_solr_hocr_highlighting_field', 'text_nodes_HOCR_hlt'),
      ),
      'islandora_ocr_solr_hocr_result_count' => array(
        '#access' => $solr_enabled,
        '#type' => 'textfield',
        '#title' => t('Maximum number of results to return in a Solr query'),
        '#description' => t('The maximum number of pages that will be returned by a full text search.'),
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $get_default_value('islandora_ocr_solr_hocr_result_count', '32'),
      ),
    ));
}

/**
 * Ajax callback for the Tesseract textfield.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The element to render as part the ajax callback.
 */
function islandora_ocr_admin_settings_form_ajax_callback(array $form, array $form_state) {
  return $form;
}
