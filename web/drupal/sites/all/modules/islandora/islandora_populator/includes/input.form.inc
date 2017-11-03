<?php

/**
 * @file
 * Form to allow input from the selected handler and associated functions.
 */

/**
 * Form building function; dump out the "inline" form.
 */
function islandora_populator_input_form($form, &$form_state, $selected, array $populator) {
  $storage =& islandora_ingest_form_get_step_storage($form_state);
  $storage['populator'] = $populator;
  $form += $populator['form'];
  return $form;
}

/**
 * Form submission handler; run callbacks and create relevant datastreams.
 */
function islandora_populator_input_form_submit(&$form, &$form_state) {
  $storage =& islandora_ingest_form_get_step_storage($form_state);
  $storage['created'] = array();
  $populator = $storage['populator'];
  $object = islandora_ingest_form_get_object($form_state);

  foreach ($populator['output'] as $datastream_id => $info) {
    $output = NULL;

    $initially_existing = isset($object[$datastream_id]);

    if (is_callable($info['callback'])) {
      $output = $info['callback']($form, $form_state);
    }

    if ($output === FALSE) {
      continue;
    }
    elseif ($output !== NULL) {
      if ($initially_existing) {
        // Already existing... could possibly throw a warning? Dunno...
        $datastream = $object[$datastream_id];
      }
      else {
        $datastream = $object->constructDatastream($datastream_id, 'M');
      }

      $datastream->label = (isset($info['label callback']) && is_callable($info['label callback'])) ?
        $info['label callback']($form, $form_state, $output) :
        "$datastream_id datastream";
      $datastream->mimetype = isset($info['mimetype']) ?
        $info['mimetype'] :
        'text/xml';

      if (is_object($output)) {
        // Objects returned should be a Drupal file-object (already registered
        // as temporary). Set the content of the datastream with it.
        $datastream->setContentFromFile($output->uri, FALSE);
      }
      else {
        // Not null and not an object, should be a string containing content.
        $datastream->content = $output;
      }

      if (!$initially_existing) {
        $object->ingestDatastream($datastream);
      }
    }

    if (!$initially_existing && isset($object[$datastream_id])) {
      $storage['created'][] = $datastream_id;
    }
  }
}

/**
 * Form "undo" submission callback; nuke any datastreams we've created.
 */
function islandora_populator_input_form_undo_submit(&$form, &$form_state) {
  $storage =& islandora_ingest_form_get_step_storage($form_state);
  $object = islandora_ingest_form_get_object($form_state);
  foreach ($storage['populator']['output'] as $datastream_id => $info) {
    if (isset($info['undo callback']) && is_callable($info['undo callback'])) {
      $info['undo callback']($form, $form_state, $datastream_id, $info);
      $storage['created'] = array_diff($storage['created'], array($datastream_id));
    }
  }
  foreach ($storage['created'] as $datastream_id) {
    $object->purgeDatastream($datastream_id);
  }
}
