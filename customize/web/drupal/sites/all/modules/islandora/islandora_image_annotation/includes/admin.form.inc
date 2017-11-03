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
function islandora_image_annotation_admin_settings_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_image_annotation', 'includes/utils');
  $content_models = islandora_image_annotation_get_content_models();
  $taxonomies = islandora_image_annotation_get_taxonomies();
  $annotation_mappings = variable_get('islandora_image_annotation_mappings', array());
  $options = array();
  foreach ($content_models as $pid => $label) {
    $options[$pid] = array(
      'content_model' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => filter_xss($label),
          '#href' => "islandora/object/{$pid}",
        ),
      ),
      'pid' => check_plain($pid),
      'dsid' => array(
        'data' => array(
          '#type' => 'select',
          '#name' => "dsid[$pid]",
          '#attributes' => array('style' => 'width: 120px'),
          '#options' => islandora_image_annotation_get_dsids_from_dscomp($pid) + array('TN' => 'TN'),
          '#value' => isset($annotation_mappings[$pid]['DSID']) ? $annotation_mappings[$pid]['DSID'] : '',
        ),
      ),
      'taxonomy' => array(
        'data' => array(
          '#type' => 'select',
          '#name' => "taxonomy[$pid]",
          '#attributes' => array('style' => 'width: 120px'),
          '#options' => $taxonomies,
          '#value' => isset($annotation_mappings[$pid]['TAX']) ? $annotation_mappings[$pid]['TAX'] : '',
        ),
      ),
    );
  }
  $solr_enabled = module_exists('islandora_solr');
  $entities_enabled = module_exists('islandora_entities');
  // Some fields can only be populated if Solr is installed.
  return system_settings_form(array(
    'content_models' => array(
      '#type' => 'fieldset',
      '#title' => t('Configure Content Models'),
      '#description' => t('Allow annotation of objects associated with these content models.'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      'islandora_image_annotation_mappings' => array(
        '#type' => 'tableselect',
        '#header' => array(
          'content_model' => t('Content Model'),
          'pid' => t('PID'),
          'dsid' => t('DSID'),
          'taxonomy' => t('Taxonomy'),
        ),
        '#options' => $options,
        '#default_value' => array_filter($annotation_mappings),
        '#empty' => t('No content models available.'),
      ),
      // Required to include fields in a select table. They are just temporary,
      // and should not be submitted otherwise it will cause them to be saved as
      // variables.
      'dsid' => array(
        '#type' => 'value',
      ),
      // Required to include fields in a select table. They are just temporary,
      // and should not be submitted otherwise it will cause them to be saved as
      // variables.
      'taxonomy' => array(
        '#type' => 'value',
      ),
    ),
    'annotation_categories' => array(
      '#type' => 'fieldset',
      '#title' => t('Annotation categories'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      'islandora_image_annotation_enforce_taxonomy' => array(
        '#type' => 'radios',
        '#title' => t('Annotation categories'),
        '#options' => array(0 => t('User defined'), 1 => t("Administrator defined")),
        '#description' => t('Allows administrators to select predefined taxonomies to determine annotation categorization terminology, see the table above.'),
        '#default_value' => variable_get('islandora_image_annotation_enforce_taxonomy', 0),
      ),
      'islandora_image_annotation_solr_field_type' => array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
        '#title' => t('Annotation type search field'),
        '#description' => t("This field will be used to autocomplete the 'type' field. This <b>must</b> be a single valued Solr field."),
        '#default_value' => variable_get('islandora_image_annotation_solr_field_type', 'RELS_EXT_isAnnotationType_literal_s'),
        '#weight' => 3,
        '#states' => array(
          'invisible' => array(
            ':input[name="islandora_image_annotation_enforce_taxonomy"]' => array('value' => 1),
          ),
        ),
      ),
    ),
    'annotation_colors' => array(
      '#type' => 'fieldset',
      '#title' => t('Annotation stroke & colors'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      'islandora_image_annotation_choose_color' => array(
        '#type' => 'radios',
        '#title' => t('Annotation colors'),
        '#options' => array(0 => t('User defined (user selects when creating annotation)'), 1 => t("Optimized (Colors are selected randomly. Recommended for overlapping annotations)")),
        '#default_value' => variable_get('islandora_image_annotation_choose_color', TRUE),
      ),
      'islandora_image_annotation_stroke_width' => array(
        '#type' => 'textfield',
        '#title' => t('Annotation stroke widths for drop down'),
        '#required' => TRUE,
        '#default_value' => variable_get('islandora_image_annotation_stroke_width', '.3 .6 .9 1.2 1.5'),
        '#description' => t('The width of the lines to use when drawing the annotations. Must be a number, can include a decimal. For instance .5 is valid. You can Enter several values here separated by a space.'),
        '#element_validate' => array('islandora_image_annotation_admin_settings_form_stroke_width_validate'),
      ),
    ),
    'islandora_image_annotation_use_entity_tagging' => array(
      '#access' => $entities_enabled,
      '#type' => 'checkbox',
      '#title' => t('Allow entity tagging on images.'),
      '#default_value' => variable_get('islandora_image_annotation_use_entity_tagging', FALSE),
      '#description' => t('If checked, users will be able to tag image annotations with an entity.'),
    ),
    'islandora_image_annotation_use_title_vocabulary' => array(
      '#access' => $solr_enabled,
      '#type' => 'checkbox',
      '#title' => t('Use a controlled vocabulary for annotations'),
      '#description' => t('If checked you must configure a Solr field to retrieve the titles from.'),
      '#default_value' => variable_get('islandora_image_annotation_use_title_vocabulary', FALSE),
    ),
    'controlled_vocabulary' => array(
      '#access' => $solr_enabled && $entities_enabled,
      '#type' => 'fieldset',
      '#title' => t('Controlled Vocabularies'),
      '#description' => t('These fields will auto-populate new annotations fields.'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#states' => array(
        // Hide the settings when the cancel notify checkbox is disabled.
        'visible' => array(
          ':input[name="islandora_image_annotation_use_title_vocabulary"]' => array('checked' => TRUE),
        ),
      ),
      'islandora_image_annotation_solr_field_title' => array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
        '#title' => t('Annotation title search field'),
        '#description' => t("This field will be used to autocomplete the 'title' field. This <b>must</b> be a single valued Solr field."),
        '#default_value' => variable_get('islandora_image_annotation_solr_field_title', 'mods_titleInfo_title_s'),
      ),
      'islandora_image_annotation_solr_field_genre' => array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
        '#title' => t('Annotation genre search field'),
        '#description' => t("This field will be used to limit the results for the 'title' field to ones that have the user selected 'type' in this solr field."),
        '#default_value' => variable_get('islandora_image_annotation_solr_field_genre', 'mods_genre_s'),
      ),
    ),
  ));
}

/**
 * Checks to make sure only valid values are used for the stroke width.
 *
 * @param array $element
 *   The textfield containing the stroke width's to validate.
 * @param array $form_state
 *   The Drupal form state.
 * @param array $form
 *   The Drupal form definition.
 */
function islandora_image_annotation_admin_settings_form_stroke_width_validate(array $element, array &$form_state, array $form) {
  $widths = explode(' ', $element['#value']);
  foreach ($widths as $width) {
    $is_float = filter_var($width, FILTER_VALIDATE_FLOAT) !== FALSE;
    $is_int = filter_var($width, FILTER_VALIDATE_INT) !== FALSE;
    $valid = $is_float || $is_int;
    if (!$valid) {
      form_error($element, t('"%value" is not valid stroke width.', array('%value' => $width)));
      break;
    }
  }
}

/**
 * Maps the values for dsid and taxonomy to the appropriate variable.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_image_annotation_admin_settings_form_validate(array $form, array &$form_state) {
  $value = array();
  $selected = array_filter($form_state['values']['islandora_image_annotation_mappings']);
  foreach ($selected as $pid) {
    $value[$pid] = array(
      'DSID' => $form_state['values']['dsid'][$pid],
      'TAX' => $form_state['values']['taxonomy'][$pid],
    );
  }
  form_set_value($form['content_models']['islandora_image_annotation_mappings'], $value, $form_state);
  // These two fields should NOT get saved as variables.
  unset($form_state['values']['dsid']);
  unset($form_state['values']['taxonomy']);
}
