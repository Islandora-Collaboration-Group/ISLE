<?php
/**
 * @file
 * Defines the form for uploading a PDF for extraction.
 */

/**
 * Form used to accept an uploaded PDF and user input for ingest.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param string $model
 *   A string specifying the content model of the page(s) to be ingested.
 *
 * @return array
 *   An array containing the form to be rendered.
 */
function islandora_paged_content_upload_pdf_form($form, &$form_state, $model) {
  module_load_include('inc', 'islandora', 'includes/ingest.form');
  $form['pdf_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('PDF File'),
    '#description' => t('You can optionally upload a PDF file, from which the pages of the document will be generated.'),
    '#islandora_plupload_do_not_alter' => TRUE,
    '#upload_validators' => array(
      'file_validate_extensions' => array('pdf'),
    ),
  );
  $form['image_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Image Settings'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['image_settings']['image_format'] = array(
    '#type' => 'radios',
    '#title' => t('Image Format'),
    '#options' => array(
      'tiffgray' => t('8-bit grayscale'),
      'tiff12nc' => t('12-bit RGB'),
      'tiff24nc' => t('24-bit RGB'),
      'tiff48nc' => t('48-bit RGB'),
      'tiff32nc' => t('32-bit CMYK'),
      'tiff64nc' => t('64-bit CMYK'),
    ),
    '#default_value' => 'tiff32nc',
    '#description' => t('You can choose the format for the page images extracted from the uploaded PDF. We recommend 32-bit CMYK for most cases.'),
  );
  $dpi = array(
    '72',
    '96',
    '150',
    '300',
    '600',
  );
  $form['image_settings']['resolution'] = array(
    '#type' => 'radios',
    '#title' => t("Resolution (DPI)"),
    '#description' => t('The higher the resolution selected, the more legible the text will be, but be aware it takes longer to process higher resolution images, and they will consume more space.'),
    '#options' => array_combine($dpi, $dpi),
    '#default_value' => '300',
  );
  $form['text_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page OCR / Text Settings'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $ocr_options = array(
    'none' => t('Do not extract text.'),
  );
  $states = array();
  $languages = array();
  $default_value = 'none';
  if (module_exists('islandora_ocr')) {
    module_load_include('inc', 'islandora_ocr', 'includes/utilities');
    $ocr_options['ocr'] = t('Perform OCR on the PDF.');
    $states[] = array('value' => 'ocr');
    $languages = islandora_ocr_get_enabled_tesseract_languages();
    // Don't need the no OCR option.
    unset($languages['no_ocr']);
  }
  if (islandora_paged_content_pdftotext_availability()) {
    $ocr_options['extract'] = t('Extract text from the PDF.');
    $default_value = 'extract';
    $states[] = array('value' => 'extract');
    // Need to fallback if tesseract isn't available for future OCR.
    $languages = array(
      'eng' => t('English'),
      'fra' => t('French'),
      'deu-frak' => t('German'),
      'por' => t('Portugese'),
      'spa' => t('Spanish'),
      'hin' => t('Hindi'),
      'jpn' => t('Japanese'),
      'ita' => t('Italian'),
    );
  }
  $form['text_settings']['extract_text'] = array(
    '#type' => 'radios',
    '#title' => t('Extract Text'),
    '#options' => $ocr_options,
    '#default_value' => $default_value,
    '#description' => t("Extracted text is used to aid in the discovery of this object when searching. If the uploaded PDF contains text and not just images of text, we recommend you try to extract it from the PDF. On the other hand if the uploaded PDF is composed of Images of text we and doesn't contain text we recommend you perform OCR."),
  );
  if (variable_get('islandora_paged_content_pdftotext_use_raw', FALSE)) {
    $form['text_settings']['rawtext'] = array(
      '#type' => 'radios',
      '#title' => t('Use the raw text of the PDF'),
      '#description' => t('This will pass the -raw parameter off to pdftotext when extracting text. By default, pdftotext extracts text in natural reading order. In edge case documents, where PDF creation tools have made blocks in errorneous order, text extraction will yield unexpected results. As such, this parameter will pull out text in the order that the PDF creation tool wrote it (layout ignored). This is not a valid default option to have due to that PDF creation tools are not constrained to writing blocks in the order they appear and it is up to the PDF reader to render them correctly.'),
      '#options' => array(
        'yes' => t('Yes'),
        'no' => t('No'),
      ),
      '#default_value' => 'no',
      '#states' => array(
        'visible' => array(
          'input[name="extract_text"]' => array('value' => 'extract'),
        ),
      ),
    );
  }
  $form['text_settings']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t('Please select the language the page is written in.'),
    '#options' => $languages,
    '#states' => array(
      'visible' => array(
        ':input[name="extract_text"]' => $states,
      ),
    ),
  );
  $form['model'] = array(
    '#type' => 'value',
    '#value' => $model,
  );
  return $form;
}

/**
 * Validation for the PDF upload form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_paged_content_upload_pdf_form_validate($form, &$form_state) {
  // Only do things if the user has uploaded a PDF.
  if ($form_state['values']['pdf_file']) {
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    $pdf_file = $form_state['values']['pdf_file'];
    $file_obj = file_load($pdf_file);
    $num_pages = islandora_paged_content_length_of_pdf($file_obj->uri);
    if ($num_pages) {
      $form_state['number_of_pages'] = $num_pages;
    }
    else {
      form_error($form['pdf_file'], t('Unable to extract required information from the supplied PDF.'));
    }
  }
}

/**
 * Submit handler for the PDF upload form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_paged_content_upload_pdf_form_submit($form, $form_state) {
  // Only do things if the user has uploaded a PDF.
  if ($form_state['values']['pdf_file']) {
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    module_load_include('inc', 'islandora_paged_content', 'includes/batch');
    module_load_include('inc', 'islandora', 'includes/ingest.form');
    module_load_include('inc', 'islandora', 'includes/utilities');
    $file_obj = file_load($form_state['values']['pdf_file']);
    $object = islandora_ingest_form_get_object($form_state);
    islandora_paged_content_add_pdf_to_object($object, $file_obj);
    $num_pages = $form_state['number_of_pages'];
    $pages = range(1, $num_pages);
    $options = array_intersect_key($form_state['values'], drupal_map_assoc(array(
      'image_format',
      'resolution',
      'language',
      'extract_text',
      'model',
    ))) + array('pdf_file' => $file_obj, 'parent' => $object->id);
    if (variable_get('islandora_paged_content_pdftotext_use_raw', FALSE)) {
      $options['rawtext'] = $form_state['values']['rawtext'] == 'yes' ? TRUE : FALSE;
    }
    else {
      $options['rawtext'] = FALSE;
    }
    $batch = islandora_paged_content_create_paged_content_from_pdf_batch($object, $pages, $options);
    batch_set($batch);
  }
}
