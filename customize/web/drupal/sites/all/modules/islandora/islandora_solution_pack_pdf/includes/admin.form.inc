<?php

/**
 * @file
 * This file contains all admin functions.
 */

/**
 * Admin form function.
 */
function islandora_pdf_admin($form, &$form_state) {

  if (isset($form_state['values']['islandora_pdf_path_to_pdftotext'])) {
    $islandora_pdf_path_to_pdftotext = $form_state['values']['islandora_pdf_path_to_pdftotext'];
  }
  else {
    $islandora_pdf_path_to_pdftotext = variable_get('islandora_pdf_path_to_pdftotext', '/usr/bin/pdftotext');
  }
  exec($islandora_pdf_path_to_pdftotext, $output, $return_value);
  $pdftotext_confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>'
      . t('pdftotext executable found at !url', array('!url' => "<strong>$islandora_pdf_path_to_pdftotext</strong>"));
  if ($return_value != 99) {
    $pdftotext_confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> '
        . t('Unable to find pdftotext executable at !url', array('!url' => "<strong>$islandora_pdf_path_to_pdftotext</strong>"));
  }

  if (isset($form_state['values']['islandora_pdf_path_to_gs'])) {
    $islandora_pdf_path_to_gs = $form_state['values']['islandora_pdf_path_to_gs'];
  }
  else {
    $islandora_pdf_path_to_gs = variable_get('islandora_pdf_path_to_gs', '/usr/bin/gs');
  }
  $gs_test_command = $islandora_pdf_path_to_gs . ' --help';
  exec($gs_test_command, $output, $return_value);
  $gs_confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>' . t('ghostscript executable found at !url', array('!url' => "<strong>$islandora_pdf_path_to_gs</strong>"));
  if ($return_value != 0) {
    $gs_confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> ' . t('Unable to find ghotscript executable at !url', array('!url' => "<strong>$islandora_pdf_path_to_gs</strong>"));
  }

  $form = array();

  // AJAX wrapper for url checking.
  $form['islandora_pdf_url_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('TEXT'),
  );

  $form['islandora_pdf_url_fieldset']['islandora_pdf_allow_text_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to upload .txt files with PDFs"),
    '#description' => t("Uploaded text files are appended to PDFs as FULL_TEXT datastreams and are indexed into Solr."),
    '#default_value' => variable_get('islandora_pdf_allow_text_upload', FALSE),
  );

  $form['islandora_pdf_url_fieldset']['islandora_pdf_create_fulltext'] = array(
    '#type' => 'checkbox',
    '#title' => t("Extract text streams from PDFs using pdftotext"),
    '#description' => t("Extracted text streams are appended to PDFs as FULL_TEXT datastreams and are indexed into Solr. Uploading a text file takes priority over text stream extraction.
                           </br><strong>Note:</strong> PDFs that contain visible text do not necessarily contain text streams (e.g. images scanned and saved as PDFs). Consider converting text-filled images with no text streams to TIFFs and using the !book with !ocr enabled.", array(
                             '!book' => l(t('Book Solution Pack'), 'https://wiki.duraspace.org/display/ISLANDORA711/Book+Solution+Pack'),
                             '!ocr' => l(t('OCR'), 'https://wiki.duraspace.org/display/ISLANDORA711/Islandora+OCR'),
                           )),
    '#default_value' => variable_get('islandora_pdf_create_fulltext', FALSE),
  );

  $form['islandora_pdf_url_fieldset']['islandora_pdf_create_pdfa'] = array(
    '#type' => 'checkbox',
    '#title' => t("Create PDF/A archival derivative from PDF"),
    '#description' => t("Create a PDF/A version of any uploaded PDF. PDF/A is a restrictive standard that prohibits more easily broken components of the PDF spec, such as fillable forms and DRM. The PDF/A derivative will not be used for display. Requires ghostscript to be installed on the server."),
    '#default_value' => variable_get('islandora_pdf_create_pdfa', FALSE),
  );

  $form['islandora_pdf_url_fieldset']['wrapper'] = array(
    '#prefix' => '<div id="islandora-url">',
    '#suffix' => '</div>',
    '#type' => 'markup',
  );
  $form['islandora_pdf_url_fieldset']['wrapper'] = array(
    '#prefix' => '<div id="islandora-url">',
    '#suffix' => '</div>',
    '#type' => 'markup',
  );

  $form['islandora_pdf_url_fieldset']['wrapper']['islandora_pdf_path_to_pdftotext'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdftotext executable'),
    '#default_value' => $islandora_pdf_path_to_pdftotext,
    '#description' => t('!pdftotext_confirmation_message',
        array(
          '!pdftotext_confirmation_message' => $pdftotext_confirmation_message)
    ),
    '#ajax' => array(
      'callback' => 'islandora_update_pdftotext_url_div',
      'wrapper' => 'islandora-url',
      'effect' => 'fade',
      'event' => 'blur',
      'progress' => array('type' => 'throbber'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_pdf_create_fulltext"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['islandora_pdf_url_fieldset']['wrapper']['islandora_pdf_path_to_gs'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to ghostscript executable'),
    '#default_value' => $islandora_pdf_path_to_gs,
    '#description' => t('!gs_confirmation_message',
      array(
        '!gs_confirmation_message' => $gs_confirmation_message,
      )
    ),
    '#ajax' => array(
      'callback' => 'islandora_update_gs_url_div',
      'wrapper' => 'islandora-url',
      'effect' => 'fade',
      'event' => 'blur',
      'progress' => array('type' => 'throbber'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_pdf_create_pdfa"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['islandora_pdf_thumbnail_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thumbnail'),
    '#description' => t('Settings for creating PDF thumbnail derivatives'),
  );

  $form['islandora_pdf_thumbnail_fieldset']['islandora_pdf_thumbnail_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The width of the thumbnail in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_thumbnail_width', 200),
    '#size' => 5,
  );

  $form['islandora_pdf_thumbnail_fieldset']['islandora_pdf_thumbnail_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The height of the thumbnail in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_thumbnail_height', 200),
    '#size' => 5,
  );

  $form['islandora_pdf_preview_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview image'),
    '#description' => t('Settings for creating PDF preview image derivatives'),
  );

  $form['islandora_pdf_preview_fieldset']['islandora_pdf_preview_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Max width'),
    '#description' => t('The maximum width of the preview in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_preview_width', 500),
    '#size' => 5,
  );

  $form['islandora_pdf_preview_fieldset']['islandora_pdf_preview_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Max height'),
    '#description' => t('The maximum height of the preview in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_pdf_preview_height', 700),
    '#size' => 5,
  );

  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_pdf_viewers', 'application/pdf');
  return system_settings_form($form);
}

/**
 * Admin form validate function.
 */
function islandora_pdf_admin_validate($form, &$form_state) {
  if ($form_state['values']['islandora_pdf_create_fulltext']) {
    $islandora_pdf_path_to_pdftotext = $form_state['values']['islandora_pdf_path_to_pdftotext'];
    exec($islandora_pdf_path_to_pdftotext, $output, $return_value);
    if ($return_value != 99) {
      form_set_error('', t('Cannot extract text from PDF without a valid path to pdftotext.'));
    }
  }
  if ($form_state['values']['islandora_pdf_create_pdfa']) {
    $islandora_pdf_path_to_gs = $form_state['values']['islandora_pdf_path_to_gs'];
    $gs_test_command = $islandora_pdf_path_to_gs . ' --help';
    exec($gs_test_command, $output, $return_value);
    if ($return_value != 0) {
      form_set_error('', t('Cannot create PDF/A without ghostscript.'));
    }
  }
}

/**
 * Function updates PDF to text URL div.
 */
function islandora_update_pdftotext_url_div($form, $form_state) {
  unset($form_state['submit_handlers']);
  $form_state['rebuild'] = TRUE;

  return $form['islandora_pdf_url_fieldset']['wrapper'];
}

/**
 * Function updates ghostscript URL div.
 */
function islandora_update_gs_url_div($form, $form_state) {
  unset($form_state['submit_handlers']);
  $form_state['rebuild'] = TRUE;

  return $form['islandora_pdf_url_fieldset']['wrapper'];
}
