<?php

/**
 * @file
 * Functions used in populating datastreams.
 */

/**
 * Element validation callback; ensure the submitted PMID is valid.
 *
 * Since we end up doing the full request, also stash the result in the
 * $form_state.
 */
function endnotexml_populator_validate_file($element, &$form_state, $form) {
  module_load_include('inc', 'islandora_endnotexml', 'includes/utilities');
  $file = file_load($element['#value']['fid']);
  $mods = $file ?
    islandora_endnotexml_get_mods($file) :
    FALSE;
  if ($mods === FALSE) {
    form_error($element, t('The uploaded file does not appear to be a valid EndNote XML record.'));
  }
  else {
    $storage =& islandora_ingest_form_get_step_storage($form_state);
    $storage['endnotexml_populator']['file'] = file_save_data($mods, 'temporary://mods.xml');
    $storage['endnotexml_populator']['file']->status &= ~FILE_STATUS_PERMANENT;
    file_save($storage['endnotexml_populator']['file']);
  }
}

/**
 * Population callback; generate the MODS, stash in a file and return it.
 */
function endnotexml_populator_generate_mods(&$form, &$form_state) {
  $storage =& islandora_ingest_form_get_step_storage($form_state);
  return $storage['endnotexml_populator']['file'];
}
