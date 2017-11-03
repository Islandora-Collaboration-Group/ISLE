<?php

/**
 * @file
 * Module used to create an upload tab.
 */

/**
 * Display a form to add/modify the objects PDF datastream.
 *
 * Show the form to upload if the PDF does not exist on the given object.
 * Show the form to modify if there is a PDF.
 *
 * @param AbstractObject $object
 *   The object.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_scholar_upload_view(AbstractObject $object) {
  $form_name = 'islandora_scholar_upload_form';
  if (isset($object['PDF'])) {
    $form_name = 'islandora_scholar_modify_form';
  }
  return drupal_get_form($form_name, $object);
}

/**
 * Gets the modify form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_scholar_modify_form(array $form, array &$form_state, AbstractObject $object) {
  $mods_str = trim($object['MODS']->content);

  $mods_doc = new DOMDocument();
  $mods_doc->loadXML($mods_str);
  $mods_xpath = new DOMXPath($mods_doc);
  $mods_xpath->registerNamespace('m', 'http://www.loc.gov/mods/v3');
  $usage = t('No usage statement');
  $mods_usages = $mods_xpath->query('//m:mods/m:accessCondition[@type="use and reproduction"]');
  if ($mods_usages->length > 0) {
    $usage = $mods_usages->item(0)->textContent;
  }
  $version = t('No version statement');
  $mods_versions = $mods_xpath->query('//m:mods/m:physicalDescription[@authority="local"]');
  if ($mods_versions->length > 0) {
    $version = $mods_versions->item(0)->textContent;
  }

  return array(
    'pid' => array(
      '#type' => 'value',
      '#value' => $object->id,
    ),
    'dsid' => array(
      '#type' => 'value',
      '#value' => 'PDF',
    ),
    'usage' => array(
      '#type' => 'item',
      '#title' => t('Usage'),
      '#markup' => $usage,
    ),
    'version' => array(
      '#type' => 'item',
      '#title' => t('Publication version'),
      '#markup' => $version,
    ),
    'delete_document' => array(
      '#name' => 'delete_document',
      '#type' => 'submit',
      '#value' => t('Delete Document/PDF'),
    ),
  );
}

/**
 * Submit handler for modify form, delete's the PDF datastream.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_scholar_modify_form_submit(array $form, array &$form_state) {
  extract($form_state['values']);

  $object = islandora_object_load($pid);
  if ($form_state['clicked_button']['#name'] == 'delete_document') {
    try {
      $object->purgeDatastream($dsid);
      if (isset($object['TN'])) {
        $object->purgeDatastream('TN');
      }
      if (isset($object['PREVIEW'])) {
        $object->purgeDatastream('PREVIEW');
      }
      if (isset($object['FULL_TEXT'])) {
        $object->purgeDatastream('FULL_TEXT');
      }
    }
    catch (Exception $e) {
      drupal_set_message(t('Failed to purge datastream @dsid from @pid</br>code: @code<br/>message: @msg', array(
          '@pid' => $pid,
          '@dsid' => $dsid,
          '@code' => $e->getCode(),
          '@msg' => $e->getMessage())), 'error');
    }
  }
}

/**
 * Gets the upload form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_scholar_upload_form(array $form, array &$form_state, AbstractObject $object) {
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : $default;
  };
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));

  module_load_include('inc', 'islandora_scholar', 'includes/utilities');

  // Pull document versions from the module settings if available.
  $document_versions = get_document_versions();

  // Pull use permissions from the module settings if available.
  $use_permissions = get_use_permissions();

  return array(
    '#type' => 'form',
    'upload_document' => array(
      '#type' => 'fieldset',
      '#title' => t('Upload Document'),
      'pid' => array(
        '#type' => 'value',
        '#value' => $object->id,
      ),
      'dsid' => array(
        '#type' => 'value',
        '#value' => 'PDF',
      ),
      'file' => array(
        '#type' => 'managed_file',
        '#title' => 'PDF file',
        '#required' => TRUE,
        '#default_value' => $get_default_value('file', NULL),
        '#upload_location' => 'temporary://',
        '#upload_validators' => array(
          'file_validate_extensions' => array('pdf'),
          'file_validate_size' => array($upload_size * 1024 * 1024),
        ),
      ),
      'version' => array(
        '#type' => 'radios',
        '#title' => t('Document Version'),
        '#options' => $document_versions,
        '#required' => TRUE,
      ),
      'usage' => array(
        '#type' => 'radios',
        '#title' => t('Use Permission'),
        '#options' => $use_permissions,
        '#required' => TRUE,
      ),
      'certifying' => array(
        '#type' => 'checkboxes',
        '#title' => t('Certify'),
        '#options' => array(
          'certify' => t('I certify that I have the right to upload this version.'),
        ),
        '#required' => TRUE,
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Upload'),
      ),
    ),
  );
}

/**
 * Updates the MODS datastream with the given usage and version.
 *
 * Removes any other usage or version elements.
 *
 * @param AbstractObject $object
 *   The object.
 * @param string $usage
 *   The usage either the 'publisher' or 'author' holds the copyright.
 * @param string $version
 *   The version of the uploaded document either 'PRE-PUBLICATION',
 *  'PUBLISHED', 'POST-PUBLICATION', 'OTHER'.
 */
function islandora_scholar_add_usage_and_version_elements_to_mods(AbstractObject $object, $usage, $version) {
  $refuse_msg = t('Refusing to add usage and publication version statements alone');
  if (empty($object['MODS'])) {
    drupal_set_message(t('No MODS in object @pid. %refuse.', array('@pid' => $object->id, '%refuse' => $refuse_msg)), 'error');
    return;
  }
  $namespace = 'http://www.loc.gov/mods/v3';
  $doc = new DOMDocument();
  $doc->loadXML($object['MODS']->content);
  $xpath = new DOMXPath($doc);
  $xpath->registerNamespace('mods', $namespace);
  // Remove all instances of mods:accessCondition and mods:physicalDescription
  // from every mods:mods element instance.
  $access_condition_query = '//mods:accessCondition[@type="use and reproduction"]';
  $physical_description_query = '//mods:physicalDescription[@authority="local"]';
  $results = $xpath->query("{$access_condition_query} | {$physical_description_query}");
  foreach ($results as $result) {
    $result->parentNode->removeChild($result);
  }
  // Regardless of the number of mods:mods elements in the document we only
  // add the usage and version to the first one.
  $results = $xpath->query('//mods:mods[1]');
  if ($results->length == 0) {
    drupal_set_message(t('Could not find MODS element in object @pid is empty. %refuse.', array('@pid' => $object->id, '%refuse' => $refuse_msg)), 'error');
    return;
  }
  $mods_element = $results->item(0);
  // Add mods:accessCondition to the first mods:mods element.
  $access_condition_element = $doc->createElementNS($namespace, 'accessCondition');
  $access_condition_element->setAttribute('type', 'use and reproduction');
  $access_condition_element->nodeValue = $usage;
  $mods_element->appendChild($access_condition_element);
  // Add mods:physicalDescription to the first mods:mods element.
  $physical_description_element = $doc->createElementNS($namespace, 'physicalDescription');
  $physical_description_element->setAttribute('authority', 'local');
  $physical_description_element->nodeValue = $version;
  $mods_element->appendChild($physical_description_element);
  // Commit changes to datastream.
  $object['MODS']->setContentFromString($doc->saveXML());

}

/**
 * Upload form Submit handler, adds PDF datastream.
 *
 * Also updates the MODS usage and version for the PDF datastream.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_scholar_upload_form_submit(array $form, array &$form_state) {
  extract($form_state['values']);

  $object = islandora_object_load($pid);

  try {
    $datastream = $object->constructDatastream($dsid, 'M');
    $datastream->label = 'PDF Datastream';
    $datastream->mimetype = 'application/pdf';
    $file = file_load($file);
    $path = drupal_realpath($file->uri);
    $datastream->setContentFromFile($path);

    $object->ingestDatastream($datastream);

    islandora_scholar_add_usage_and_version_elements_to_mods($object, $usage, $version);
  }
  catch (Exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
  }
}
