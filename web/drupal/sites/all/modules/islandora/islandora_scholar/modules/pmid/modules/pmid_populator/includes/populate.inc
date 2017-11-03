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
function pmid_populator_validate_id($element, &$form_state, $form) {
  module_load_include('inc', 'islandora_pmid', 'includes/utilities');
  $mods = islandora_pmid_get_mods($element['#value']);
  if ($mods === FALSE) {
    form_error($element, t('"@value" does not appear to be a valid PubMed Identifier.', array(
      '@value' => $element['#value'],
    )));
  }
  else {
    $storage =& islandora_ingest_form_get_step_storage($form_state);
    $storage['pmid_populator']['file'] = file_save_data($mods->saveXML(), 'temporary://mods.xml');
    $storage['pmid_populator']['file']->status &= ~FILE_STATUS_PERMANENT;
    file_save($storage['pmid_populator']['file']);
  }
}

/**
 * Population callback; generate the MODS, stash in a file and return it.
 */
function pmid_populator_generate_mods(&$form, &$form_state) {
  $storage =& islandora_ingest_form_get_step_storage($form_state);
  return $storage['pmid_populator']['file'];
}
