<?php

/**
 * @file
 * Admin form for the islandora_scholar.module
 */

/**
 * Created a system form element that defaults to a textfield.
 *
 * @param string $name
 *   The name of the form element/variable.
 * @param mixed $default
 *   The default value for the given form element/variable.
 * @param array $form_controls
 *   Form controls for the form element.
 *
 * @return array
 *   A system form element definition.
 */
function _islandora_scholar_add_variable($name, $default = '', $form_controls = array()) {
  $form_controls += array(
    '#type' => 'textfield',
    '#title' => $name,
  );
  $form_controls['#default_value'] = variable_get($name, $default);
  return array(
    $name => $form_controls,
  );
}

/**
 * Defines the admin settings form.
 *
 * @return array
 *   Drupal form definition.
 */
function islandora_scholar_admin_form() {
  $form = array(
    'romeo' => array(
      '#type' => 'fieldset',
      '#title' => t('RoMEO configuration'),
      '#description' => t('Various parameters used by RoMEO'),
      '#collapsible' => TRUE,
    ),
    'google_scholar_search' => array(
      '#type' => 'fieldset',
      '#title' => t('Google Scholar Search configuration'),
      '#description' => t('Various parameters used by Google Scholar Search'),
      '#collapsible' => TRUE,
    ),
  );

  $form['google_scholar_search'] += _islandora_scholar_add_variable('islandora_scholar_google_scholar_search_enabled', FALSE, array(
    '#type' => 'checkbox',
    '#title' => t('Render Google Scholar Search link'),
    '#description' => t('Enable Searching in Google Scholar.'),
  ));
  $form['google_scholar_search'] += _islandora_scholar_add_variable('islandora_scholar_google_scholar_primary_search_xpath', "//mods:identifier[@type=\"doi\"]", array(
    '#type' => 'textfield',
    '#title' => t('Google Scholar Primary Search XPath'),
    '#description' => t('The primary xpath you want to use to search.'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_scholar_google_scholar_search_enabled"]' => array('checked' => TRUE),
      ),
    ),
  ));
  $form['google_scholar_search'] += _islandora_scholar_add_variable('islandora_scholar_google_scholar_default_search_xpath', "//mods:mods[1]/mods:titleInfo/mods:title", array(
    '#type' => 'textfield',
    '#title' => t('Google Scholar Default Search XPath'),
    '#description' => t('The default xpath you want to use to search'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_scholar_google_scholar_search_enabled"]' => array('checked' => TRUE),
      ),
    ),
  ));
  $form['romeo'] += _islandora_scholar_add_variable('islandora_scholar_romeo_enable', FALSE, array(
    '#type' => 'checkbox',
    '#title' => t('Enable RoMEO attempts.'),
    '#description' => t('Whether or not we should even try getting information from RoMEO.'),
  ));
  $form['romeo'] += _islandora_scholar_add_variable('islandora_scholar_romeo_url', 'http://www.sherpa.ac.uk/romeo/api29.php', array(
    '#title' => t('Sherpa/RoMEO URL'),
    '#description' => t('The URL to which to make requests.'),
  ));
  $form['romeo'] += _islandora_scholar_add_variable('islandora_scholar_romeo_key', '', array(
    '#title' => t('Sherpa/RoMEO API Key'),
    '#description' => t("Your institution's key to access RoMEO."),
  ));
  $form['romeo'] += _islandora_scholar_add_variable('islandora_scholar_issn_cache_time', 2, array(
    '#title' => t('ISSN cache time'),
    '#description' => t('The amount of time in days for which to maintain an ISSN value obtained by scanning an object.'),
  ));
  $form['romeo'] += _islandora_scholar_add_variable('islandora_scholar_romeo_cache_time', 2, array(
    '#title' => t('RoMEO cache time'),
    '#description' => t('The amount of time in days for which to maintain a copy of the markup for a given ISSN.'),
  ));

  $form['islandora_scholar_use_standard_metadata_display'] = array(
    '#title' => t('Use Standard Metadata Display'),
    '#description' => t('Check to use standard Islandora metadata display instead of COinS.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('islandora_scholar_use_standard_metadata_display', FALSE),
  );

  $form['islandora_scholar_users_choose_display_csl'] = array(
    '#title' => t('Let users choose display CSL'),
    '#description' => t('In object view let the users choose which CSL to use.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('islandora_scholar_users_choose_display_csl', FALSE),
  );

  $form['islandora_scholar_specify_document_versions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Specify Document Versions'),
    '#description' => t('Use your own document versions when uploading a PDF'),
    '#default_value' => variable_get('islandora_scholar_specify_document_versions', FALSE),
  );

  $form['islandora_scholar_document_versions'] = array(
    '#type' => 'textarea',
    '#title' => t('Document Versions'),
    '#description' => t('Enter the document versions you would like to choose from. Enter one choice per line.'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_scholar_specify_document_versions"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('islandora_scholar_document_versions', "Author's Original\nSubmitted Manuscript Under Review\nAccepted Manuscript\nProof\nVersion of Record\nCorrected Version of Record\nEnhanced Version of Record"),
  );

  $form['islandora_scholar_specify_use_permissions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Specify Use Permissions'),
    '#description' => t('Use your own use permissions when uploading a PDF'),
    '#default_value' => variable_get('islandora_scholar_specify_use_permissions', FALSE),
  );

  $form['islandora_scholar_use_permissions'] = array(
    '#type' => 'textarea',
    '#title' => t('Use Permissions'),
    '#description' => t('Enter the use permissions you would like to choose from. Enter one choice per line.'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_scholar_specify_use_permissions"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('islandora_scholar_use_permissions', "Publisher\nAuthor"),
  );

  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_scholar_viewers', 'application/pdf');

  return system_settings_form($form);
}

/**
 * Defines the Islandora Scholar PDF derivative options form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array representing the Drupal form state.
 *
 * @return array
 *   An array representing a Drupal form.
 */
function islandora_scholar_admin_pdf_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'islandora_scholar', 'includes/admin.form');
  // @TODO: Consolidate all this into derivatives library code so we don't
  // have similar code within PDF and Scholar.
  if (isset($form_state['values']['islandora_scholar_path_to_pdftotext'])) {
    $islandora_scholar_path_to_pdftotext = $form_state['values']['islandora_scholar_path_to_pdftotext'];
  }
  else {
    $islandora_scholar_path_to_pdftotext = variable_get('islandora_scholar_path_to_pdftotext', '/usr/bin/pdftotext');
  }
  exec($islandora_scholar_path_to_pdftotext, $output, $return_value);
  $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>'
    . t('pdftotext executable found at !url', array('!url' => "<strong>$islandora_scholar_path_to_pdftotext</strong>"));
  if ($return_value != 99) {
    $confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/> '
      . t('Unable to find pdftotext executable at !url', array('!url' => "<strong>$islandora_scholar_path_to_pdftotext</strong>"));
  }

  $form = array();

  // AJAX wrapper for url checking.
  $form['islandora_scholar_url_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('TEXT'),
  );

  $form['islandora_scholar_url_fieldset']['islandora_scholar_create_fulltext'] = array(
    '#type' => 'checkbox',
    '#title' => t("Extract text streams from PDFs using pdftotext"),
    '#description' => t(
      "The extracted text stream is added as FULL_TEXT datastreams and are indexed into Solr. Uploading a text file takes priority over text stream extraction.
      </br><strong>Note:</strong> PDFs that contain visible text do not necessarily contain text streams (e.g. images scanned and saved as PDFs). Consider converting text-filled images with no text streams to TIFFs and using the !book with !ocr enabled.",
      array(
        '!book' => l(t('Book Solution Pack'), 'https://wiki.duraspace.org/display/ISLANDORA711/Book+Solution+Pack'),
        '!ocr' => l(t('OCR'), 'https://wiki.duraspace.org/display/ISLANDORA711/Islandora+OCR'),
      )
    ),
    '#default_value' => variable_get('islandora_scholar_create_fulltext', TRUE),
  );

  $form['islandora_scholar_url_fieldset']['wrapper'] = array(
    '#prefix' => '<div id="islandora-url">',
    '#suffix' => '</div>',
    '#type' => 'markup',
  );
  $form['islandora_scholar_url_fieldset']['wrapper'] = array(
    '#prefix' => '<div id="islandora-url">',
    '#suffix' => '</div>',
    '#type' => 'markup',
  );

  $form['islandora_scholar_url_fieldset']['wrapper']['islandora_scholar_path_to_pdftotext'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to pdftotext executable'),
    '#default_value' => $islandora_scholar_path_to_pdftotext,
    '#description' => t('!confirmation_message',
      array(
        '!confirmation_message' => $confirmation_message)
    ),
    '#ajax' => array(
      'callback' => 'islandora_scholar_update_pdftotext_url_div',
      'wrapper' => 'islandora-url',
      'effect' => 'fade',
      'event' => 'blur',
      'progress' => array('type' => 'throbber'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_scholar_create_fulltext"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['islandora_scholar_thumbnail_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thumbnail'),
    '#description' => t('Settings for creating PDF thumbnail derivatives'),
  );

  $form['islandora_scholar_thumbnail_fieldset']['islandora_scholar_thumbnail_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The width of the thumbnail in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_scholar_thumbnail_width', 200),
    '#size' => 5,
  );

  $form['islandora_scholar_thumbnail_fieldset']['islandora_scholar_thumbnail_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The height of the thumbnail in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_scholar_thumbnail_height', 200),
    '#size' => 5,
  );

  $form['islandora_scholar_preview_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview image'),
    '#description' => t('Settings for creating PDF preview image derivatives'),
  );

  $form['islandora_scholar_preview_fieldset']['islandora_scholar_preview_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Max width'),
    '#description' => t('The maximum width of the preview in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_scholar_preview_width', 500),
    '#size' => 5,
  );

  $form['islandora_scholar_preview_fieldset']['islandora_scholar_preview_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Max height'),
    '#description' => t('The maximum height of the preview in pixels.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_scholar_preview_height', 700),
    '#size' => 5,
  );
  $form['islandora_scholar_preview_fieldset']['islandora_scholar_preview_density'] = array(
    '#type' => 'textfield',
    '#title' => t('Density used to generate preview'),
    '#description' => t('The imagemagick density value used to generate the preview. Generally, higher numbers are better but require more computation time.'),
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('islandora_scholar_preview_density', 72),
    '#size' => 5,
  );
  return system_settings_form($form);
}

/**
 * Admin form validate function.
 */
function islandora_scholar_admin_pdf_form_validate($form, &$form_state) {
  if ($form_state['values']['islandora_scholar_create_fulltext']) {
    $islandora_scholar_path_to_pdftotext = $form_state['values']['islandora_scholar_path_to_pdftotext'];
    exec($islandora_scholar_path_to_pdftotext, $output, $return_value);
    if ($return_value != 99) {
      form_set_error('islandora_scholar_path_to_pdftotext', t('Cannot extract text from PDF without a valid path to pdftotext.'));
    }
  }
}

/**
 * Function updates PDF to text URL div.
 */
function islandora_scholar_update_pdftotext_url_div($form, $form_state) {
  $form_state['rebuild'] = TRUE;
  return $form['islandora_scholar_url_fieldset']['wrapper'];
}
