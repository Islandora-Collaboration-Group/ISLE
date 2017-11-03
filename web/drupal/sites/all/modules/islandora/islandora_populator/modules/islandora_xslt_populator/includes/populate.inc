<?php

/**
 * @file
 * Functions pertaining to the population of the datastream itself.
 */

/**
 * Element validation callback; ensure the submitted XML (and XSLT) is valid.
 *
 * Since we end up doing the full request, also stash the result in the
 * $form_state.
 */
function islandora_xslt_populator_validate_file($element, &$form_state, $form) {
  $file = file_load($element['#value']['fid']);
  $info = json_decode($form_state['values']['info'], TRUE);
  $output = $file ?
    islandora_xslt_populator_run_xslt($file, file_load($info['xslt'])) :
    FALSE;
  if ($output === FALSE) {
    form_error($element, t('The uploaded file could not be transformed.'));
  }
  else {
    $storage =& islandora_ingest_form_get_step_storage($form_state);
    $storage['islandora_xslt_populator']['file'] = file_save_data($output, 'temporary://output.xml');
    $storage['islandora_xslt_populator']['file']->status &= ~FILE_STATUS_PERMANENT;
    file_save($storage['islandora_xslt_populator']['file']);
  }
}

/**
 * Populator callback; grab the value populated during validation.
 */
function islandora_xslt_populator_populate(&$form, &$form_state) {
  $storage =& islandora_ingest_form_get_step_storage($form_state);
  return $storage['islandora_xslt_populator']['file'];
}

/**
 * Populator callback; populate any stored original datastream values.
 */
function islandora_xslt_populator_populate_original_dsid(&$form, &$form_state) {
  return file_load($form_state['values']['file']['fid']);
}

/**
 * Helper function to run an XSLT.
 *
 * @param object $input_file
 *   A Drupal file-object describing the file to process.
 * @param object $xslt_file
 *   A Drupal file-object describing the file to be used to perform the
 *   processing.
 *
 * @return string|bool
 *   A string containing the transformed XML, or FALSE in the event of an
 *   error.
 */
function islandora_xslt_populator_run_xslt($input_file, $xslt_file) {
  $xsl = new DOMDocument();
  $xsl->load(drupal_realpath($xslt_file->uri));
  $input = new DOMDocument();
  $input->load(drupal_realpath($input_file->uri));
  $processor = new XSLTProcessor();
  $processor->importStylesheet($xsl);

  return $processor->transformToXML($input);
}
