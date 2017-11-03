<?php

/**
 * @file
 * Functions used in populating datastreams.
 */

/**
 * Element validation callback; ensure the submitted DOI is valid.
 *
 * Since we end up doing the full request, also stash the result in the
 * $form_state.
 */
function doi_populator_validate_id($element, &$form_state, $form) {
  module_load_include('inc', 'islandora_doi', 'includes/utilities');
  $mods = islandora_doi_get_mods($element['#value']);
  if ($mods === FALSE) {
    form_error($element, t('"@value" does not appear to be a valid DOI.', array(
      '@value' => $element['#value'],
    )));
  }
  else {
    $storage =& islandora_ingest_form_get_step_storage($form_state);
    $storage['doi_populator']['file'] = file_save_data($mods->saveXML(), 'temporary://mods.xml');
    $storage['doi_populator']['file']->status &= ~FILE_STATUS_PERMANENT;
    file_save($storage['doi_populator']['file']);
  }
}

/**
 * Population callback; generate the MODS, stash in a file and return it.
 */
function doi_populator_generate_mods(&$form, &$form_state) {
  $storage =& islandora_ingest_form_get_step_storage($form_state);
  return $storage['doi_populator']['file'];
}
