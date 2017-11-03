<?php

/**
 * @file
 * Utitlity functions.
 */

/**
 * Returns view parameters.
 *
 * @param AbstractObject $object
 *   object representing Fedora Object
 *
 * @return array
 *   array of values to construct view
 */
function islandora_scholar_get_view(AbstractObject $object) {
  $i = 1;
  $classes = str_replace(array(':', '-', '.'), '_', implode(' ', $object->models));
  $display = array(
    'prefix' => array(
      '#markup' => "<div class='{$classes}'>",
      '#weight' => $i++,
    ),
    'citation' => array(
      '#prefix' => '<span class="citation">',
      '#suffix' => '</span>',
      '#weight' => $i++,
    ),
    'preview' => array(
      '#weight' => $i++,
    ),
    'pdf_download' => array(
      '#weight' => $i++,
    ),
    'metadata' => islandora_scholar_get_metadata_display($object, $i++),
    'suffix' => array(
      '#markup' => "</div>",
      '#weight' => $i++,
    ),
  );
  if (isset($object['MODS']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['MODS'])) {
    module_load_include('inc', 'islandora_scholar', 'includes/csl_select.form');
    $display['citation_select'] = drupal_get_form('islandora_scholar_citation_select_form', $object->id);
    $display['citation']['#markup'] = citeproc_bibliography_from_mods(
      citeproc_default_style(), $object['MODS']->content
    );

    if (variable_get('islandora_scholar_google_scholar_search_enabled', FALSE)) {
      // Google Scholar Search.
      $mods_xml = simplexml_load_string($object['MODS']->content);
      $mods_xml->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');

      // Search for primary search term (usually DOI).
      $search_term = NULL;
      $primary_search = $mods_xml->xpath("" . variable_get('islandora_scholar_google_scholar_primary_search_xpath', '//mods:identifier[@type="doi"]'));
      if ($primary_search) {
        $search_term = (string) reset($primary_search);
      }
      if (!$search_term) {
        // Search for default search term (usually title).
        $default_search = $mods_xml->xpath("" . variable_get('islandora_scholar_google_scholar_default_search_xpath', '//mods:mods[1]/mods:titleInfo/mods:title'));
        $search_term = (string) reset($default_search);
      }
      if (!$search_term) {
        $search_term = $object->label;
      }

      $display['google_scholar_search'] = array(
        '#type' => 'item',
        '#markup' => l(t('Search for this publication on Google Scholar'), "http://scholar.google.ca/scholar?q=\"$search_term\""),
        '#weight' => 0,
      );
    }
  }
  else {
    $display['citation']['#markup'] = t('Unable to load MODS.');
  }

  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $viewer = islandora_get_viewer(array(), 'islandora_scholar_viewers', $object);

  if ($viewer) {
    $display['preview'] = array(
      '#markup' => $viewer,
      '#weight' => $display['preview']['#weight'],
    );
  }
  elseif (isset($object['PREVIEW']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PREVIEW'])) {
    $image = theme('image', array(
        'path' => "islandora/object/$object->id/datastream/PREVIEW/view",
      ));
    if (isset($object['PDF']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PDF'])) {
      $display['preview'] = array(
        '#markup' => l($image, "islandora/object/$object->id/datastream/PDF/view", array(
            'html' => TRUE,
          )),
        '#weight' => $display['preview']['#weight'],
      );
    }
    else {
      $display['preview'] = array(
        '#theme' => 'image',
        '#path' => "islandora/object/$object/datastream/PREVIEW/view",
        '#weight' => $display['preview']['#weight'],
      );
    }
  }

  if (isset($object['PDF']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PDF'])) {
    $display['pdf_download'] = array(
      '#type' => 'item',
      '#title' => t('Download'),
      '#markup' => l(t('PDF'), "islandora/object/$object->id/datastream/PDF/download/citation.pdf"),
      '#weight' => $display['pdf_download']['#weight'],
    );
  }
  return array('citation.tab' => $display);
}

/**
 * Gets steps required for object ingest.
 *
 * @return array
 *   values required for upload form
 */
function islandora_scholar_get_ingest_steps() {
  return array(
    'islandora_scholar_file_upload' => array(
      'weight' => 10,
      'type' => 'form',
      'form_id' => 'islandora_scholar_pdf_upload_form',
      'module' => 'islandora_scholar',
      'file' => 'includes/pdf_upload.form.inc',
    ),
  );
}

/**
 * Returns values required for Islandora derivative hook.
 *
 * @return array
 *   values required for Islandora derivative hook
 */
function islandora_scholar_get_derivatives() {
  $derivatives = array();
  $derivatives[] = array(
    'source_dsid' => 'PDF',
    'destination_dsid' => 'TN',
    'weight' => '0',
    'function' => array(
      'islandora_scholar_add_tn_derivative',
    ),
    'file' => drupal_get_path('module', 'islandora_scholar') . '/includes/derivatives.inc',
  );
  $derivatives[] = array(
    'source_dsid' => 'PDF',
    'destination_dsid' => 'PREVIEW',
    'weight' => '1',
    'function' => array(
      'islandora_scholar_add_preview_derivative',
    ),
    'file' => drupal_get_path('module', 'islandora_scholar') . '/includes/derivatives.inc',
  );
  if (variable_get('islandora_scholar_create_fulltext', TRUE)) {
    $derivatives[] = array(
      'source_dsid' => 'PDF',
      'destination_dsid' => 'FULL_TEXT',
      'weight' => '2',
      'function' => array(
        'islandora_scholar_add_fulltext_derivative',
      ),
      'file' => drupal_get_path('module', 'islandora_scholar') . '/includes/derivatives.inc',
    );
  }
  return $derivatives;
}

/**
 * Will get the metadata display.
 *
 * @param AbstractObject $object
 *   The object to get the display for.
 * @param int $weight
 *   The weight of the element to return.
 *
 * @return array
 *   A renderable array.
 */
function islandora_scholar_get_metadata_display($object, $weight) {
  if (variable_get('islandora_scholar_use_standard_metadata_display', FALSE)) {
    module_load_include('inc', 'islandora', 'includes/metadata');
    // Borrowing core JS for fieldsets.
    drupal_add_js('misc/form.js');
    drupal_add_js('misc/collapse.js');
    $metadata = islandora_retrieve_metadata_markup($object);
    $description = islandora_retrieve_description_markup($object);
    $display = array(
      '#weight' => $weight,
      '#markup' => $description . $metadata,
    );
  }
  else {
    module_load_include('inc', 'islandora_scholar', 'includes/coins');
    $display = array(
      '#type' => 'fieldset',
      '#title' => t('Metadata'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => $weight,
      'value' => islandora_scholar_details($object),
    );
  }
  return $display;
}

/**
 * Get a list of document versions for PDF upload.
 *
 * @return array
 *   document versions to choose from during document ingest
 */
function get_document_versions() {
  if (variable_get('islandora_scholar_specify_document_versions', FALSE)) {
    $document_version_names = array_map('trim', explode("\n", variable_get('islandora_scholar_document_versions', "Author's Original\nSubmitted Manuscript Under Review\nAccepted Manuscript\nProof\nVersion of Record\nCorrected Version of Record\nEnhanced Version of Record")));
    return array_combine($document_version_names, $document_version_names);
  }
  else {
    return array(
      'PRE-PUBLICATION' => t('Pre-Publication'),
      'PUBLISHED' => t('Published'),
      'POST-PUBLICATION' => t('Post-Publication'),
      'OTHER' => t('Other'),
    );
  }
}

/**
 * Get a list of use permissions for PDF upload.
 *
 * @return array
 *   use permissions to choose from during document ingest
 */
function get_use_permissions() {
  if (variable_get('islandora_scholar_specify_use_permissions', FALSE)) {
    $use_permission_names = array_map('trim', explode("\n", variable_get('islandora_scholar_use_permissions', "Publisher\nAuthor")));
    return array_combine($use_permission_names, $use_permission_names);
  }
  else {
    return array(
      'publisher' => t('Contact Publisher (I do not hold copyright).'),
      'author' => t('Contact Author (I hold the copyright and wish to retain all rights).'),
    );
  }
}
