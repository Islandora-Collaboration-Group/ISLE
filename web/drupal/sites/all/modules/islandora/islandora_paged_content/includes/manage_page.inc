<?php

/**
 * @file
 * The page management menu.
 */

/**
 * Displays all the page management forms in a set of vertical tabs.
 *
 * @param AbstractObject $object
 *   The page object to manage.
 *
 * @return array
 *   A renderable array containing all the management forms related to
 *   page objects.
 */
function islandora_paged_content_manage_page_menu(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/breadcrumb');
  drupal_set_breadcrumb(islandora_get_breadcrumbs($object));
  drupal_set_title($object->label);
  $manage_tabs = array(
    'manage_page' => array(
      '#type' => 'vertical_tabs',
      'pdf' => array(
        '#access' => islandora_object_access(ISLANDORA_ADD_DS, $object),
        '#title' => t('Create a PDF'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_page_pdf_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
      'ocr' => array(
        '#access' => islandora_object_access(ISLANDORA_ADD_DS, $object),
        '#title' => t('Perform OCR'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_page_ocr_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      ),
      'images' => array(
        '#access' => islandora_object_access(ISLANDORA_ADD_DS, $object),
        '#title' => t('Create Images'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_page_images_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
      'edit_ocr' => array(
        '#access' => isset($object['OCR']) && isset($object['JPG']) && islandora_datastream_access(ISLANDORA_PAGED_CONTENT_EDIT_OCR, $object['OCR']),
        '#title' => t('Edit OCR'),
        '#type' => 'fieldset',
        'form' => isset($object['OCR']) && isset($object['JPG']) ? drupal_get_form('islandora_paged_content_manage_page_edit_ocr_form', $object) : NULL,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
    ),
  );
  $context = array('object' => $object);
  drupal_alter('islandora_paged_content_page_management_tabs', $manage_tabs, $context);
  return $manage_tabs;
}

/**
 * Updates this objects derived OCR datastreams.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The page object to be updated.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_paged_content_manage_page_ocr_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_ocr', 'includes/utilities');
  $can_derive = islandora_paged_content_can_derive($object, 'OCR');
  $languages = module_exists('islandora_ocr') ? islandora_ocr_get_enabled_tesseract_languages() : array('eng' => t('English'));
  unset($languages['no_ocr']);
  return array(
    'description' => array(
      '#type' => 'item',
      '#description' => t('You must have <b>Islandora OCR</b> installed to create OCR/HOCR files.'),
    ),
    'language' => array(
      '#title' => t('Language'),
      '#type' => 'select',
      '#description' => t('Please select the language the page is written in.'),
      '#options' => $languages,
    ),
    'submit' => array(
      '#disabled' => !$can_derive,
      '#type' => 'submit',
      '#value' => t('Perform OCR'),
    ),
    'pid' => array(
      '#type' => 'hidden',
      '#value' => $object->id,
    ),
  );
}

/**
 * Submit handler for the manage page OCR form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_page_ocr_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = islandora_object_load($form_state['values']['pid']);
  $options = array(
    'language' => $form_state['values']['language'],
    'preprocess' => FALSE,
  );
  if (islandora_paged_content_page_derive_ocr_datastreams($object, $options)) {
    drupal_set_message(t('Successfully performed OCR.'), 'status');
  }
  else {
    drupal_set_message(t('Failed to perform OCR.'), 'error');
  }
}

/**
 * Derives the given pages objects image datastreams.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The page object to be updated.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_paged_content_manage_page_images_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $can_derive = islandora_paged_content_can_derive($object, 'JP2');
  return array(
    'description' => array(
      '#type' => 'item',
      '#description' => t('You must have the <b>Large Image Solution Pack</b> installed to create images.<br/>This will update the TN, JPG and JP2 datastreams.'),
    ),
    'submit' => array(
      '#disabled' => !$can_derive,
      '#type' => 'submit',
      '#value' => t('Create Images'),
    ),
    'pid' => array(
      '#type' => 'hidden',
      '#value' => $object->id,
    ),
  );
}

/**
 * Submit handler for the manage page images form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_page_images_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = islandora_object_load($form_state['values']['pid']);
  if (islandora_paged_content_page_derive_image_datastreams($object)) {
    drupal_set_message(t('Successfully created images.'), 'status');
  }
  else {
    drupal_set_message(t('Failed to created images.'), 'error');
  }
}

/**
 * Derives the given page objects PDF datastream.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The page object to be updated.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_paged_content_manage_page_pdf_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $form_state['object'] = $object;
  $can_derive = islandora_paged_content_can_derive($object, 'PDF');
  return array(
    'description' => array(
      '#type' => 'item',
      '#description' => t('You must have <b>ImageMagick</b> installed to create PDF files.'),
    ),
    'dpi' => array(
      '#type' => 'select',
      '#title' => t('DPI (Dots Per Inch)'),
      '#description' => t('Set the DPI for the generated PDF.'),
      '#options' => drupal_map_assoc(array('72', '96', '300', '600')),
    ),
    'submit' => array(
      '#disabled' => !$can_derive,
      '#type' => 'submit',
      '#value' => t('Create PDF'),
      'pid' => array(
        '#type' => 'hidden',
        '#value' => $object->id,
      ),
    ),
  );
}

/**
 * Submit handler for the manage page pdf form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_page_pdf_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = islandora_object_load($form_state['values']['pid']);
  $options = array(
    '-density' => $form_state['values']['dpi'],
    '-compress' => 'LZW',
  );
  if (islandora_paged_content_page_derive_pdf_datastream($object, $options)) {
    drupal_set_message(t('Successfully created PDF.'), 'status');
  }
  else {
    drupal_set_message(t('Failed to created PDF.'), 'error');
  }
}

/**
 * Updates this objects OCR datastreams.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The page object to be updated.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_paged_content_manage_page_edit_ocr_form(array $form, array &$form_state, AbstractObject $object) {
  $pid = $object->id;
  $jpg_ds = $object['JPG'];
  $dimensions = $jpg_ds->relationships->get();
  // Safety value if rels-int doesn't exist.  This should never be necessary.
  $width = 500;
  if (is_array($dimensions)) {
    foreach ($dimensions as $dimension) {
      if ($dimension['predicate']['value'] == 'width') {
        $width = $dimension['object']['value'] + 25;
      }
    }
  }
  drupal_add_library('system', 'ui.dialog');
  $path = drupal_get_path('module', 'islandora_paged_content');
  drupal_add_js(array('pid' => $pid, 'width' => $width), 'setting');
  drupal_add_js("$path/js/islandora_page_ocr_edit.js");
  drupal_add_css("$path/css/islandora_paged_content.css");

  $form_state['object'] = $object;
  $form = array();
  $img = theme('image', array(
    'path' => "islandora/object/$pid/datastream/JPG/view",
    'alt' => $object->label,
    'attributes' => array('id' => 'source_image'),
  ));
  $label = $object->label;
  $prompt = t('Show reference image');
  $form['image'] = array(
    '#title' => t('Reference Image'),
    '#type' => 'markup',
    '#markup' => "<div id ='ref_image' title='$prompt'><a class='preview' href='#'>$prompt</a> <div class='ref_image' title = '$label'>$img</div>",
    '#attributes' => array('class' => array('ocr_image')),
  );

  $form['ocr'] = array(
    '#title' => t('Current OCR'),
    '#type' => 'textarea',
    '#default_value' => $object['OCR']->content,
    '#rows' => 40,
    '#cols' => 20,
    '#attributes' => array('class' => array('ocr_window')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update OCR'),
  );

  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $object->id,
  );

  return $form;
}

/**
 * Submit handler for the edit form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_page_edit_ocr_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = islandora_object_load($form_state['values']['pid']);
  $success = TRUE;
  try {
    $object["OCR"]->content = $form_state['values']['ocr'];
  }
  catch (Exception $e) {
    $success = FALSE;
    drupal_set_message(t("OCR update failed."));
  }
  if ($success) {
    drupal_set_message(t("OCR successfully updated."));
  }
}
