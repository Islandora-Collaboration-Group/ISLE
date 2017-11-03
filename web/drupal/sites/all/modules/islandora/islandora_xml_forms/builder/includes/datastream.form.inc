<?php

/**
 * @file
 * Allows for adding / editing XML datastreams.
 */

/**
 * Datastream form.
 *
 * Displays a select association form if more than one association is defined
 * for the given datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param AbstractObject $object
 *   The object to add the datastream to.
 * @param string $dsid
 *   The datastream to add / edit.
 *
 * @return array
 *   The form for selecting the association to add the datastream, or the XML
 *   form for adding the datastream.
 */
function xml_form_builder_datastream_form(array $form, array &$form_state, AbstractObject $object, $dsid) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/datastream.form');
  // Leave this here for legacy reasons.
  $form_state['datastream'] = isset($object[$dsid]) ? $object[$dsid] : FALSE;
  $associations = xml_form_builder_datastream_form_get_associations($form_state, $object->models, $dsid);
  $association = xml_form_builder_datastream_form_get_association($form_state, $associations);
  return isset($association) ?
    xml_form_builder_datastream_form_metadata_form($form, $form_state, $object, $association) :
    xml_form_builder_datastream_form_select_association_form($form, $form_state, $associations);
}

/**
 * Gets the form associations for the given content models and datastream.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param string[] $models
 *   The content models to match against.
 * @param string $dsid
 *   The Datastream ID to match against.
 *
 * @return array
 *   An array of found associations.
 */
function xml_form_builder_datastream_form_get_associations(array &$form_state, $models, $dsid) {
  module_load_include('inc', 'xml_form_builder', 'includes/associations');
  if (isset($form_state['associations'])) {
    return $form_state['associations'];
  }
  return xml_form_builder_get_associations(array(), $models, array($dsid));
}

/**
 * Gets the selected form association.
 *
 * If only one association exists, it is used.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param array $associations
 *   A passed in array of associations.
 *
 * @return array|NULL
 *   The selected association, or NULL if no association is selected.
 */
function xml_form_builder_datastream_form_get_association(array &$form_state, array $associations) {
  if (isset($form_state['association'])) {
    return $form_state['association'];
  }
  return count($associations) == 1 ? current($associations) : NULL;
}

/**
 * Gets the select associations form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param array $associations
 *   The associations to choose from.
 *
 * @return array
 *   The form for selecting the association to edit the datastream.
 */
function xml_form_builder_datastream_form_select_association_form(array $form, array &$form_state, array $associations) {
  module_load_include('inc', 'xml_form_builder', 'includes/select_association.form');
  $form_state['associations'] = $associations;
  $form = xml_form_builder_select_association_form($form, $form_state, $associations);
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
    '#name' => 'next',
    '#submit' => array('xml_form_builder_datastream_form_select_association_form_submit'),
  );
  return $form;
}

/**
 * Selects an association.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function xml_form_builder_datastream_form_select_association_form_submit(array $form, array &$form_state) {
  $model = $form_state['values']['models'];
  $form_name = $form_state['values']['available_forms'];
  $association_matches = function($o) use($model, $form_name) {
    return $o['content_model'] == $model && $o['form_name'] == $form_name;
  };
  $matching_associations = array_filter($form_state['associations'], $association_matches);
  $form_state['association'] = array_pop($matching_associations);
  $form_state['rebuild'] = TRUE;
}

/**
 * The edit datastream form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param AbstractObject $object
 *   The object to which the datastream belongs.
 * @param array $association
 *   The association to use for creating the edit datastream form.
 *
 * @return array
 *   The Drupal form.
 */
function xml_form_builder_datastream_form_metadata_form(array $form, array &$form_state, AbstractObject $object, array $association) {
  $form_state['pid'] = $object->id;
  $form_state['association'] = $association;
  $form_name = $association['form_name'];
  $dsid = $association['dsid'];
  $xml = NULL;
  if (isset($object[$dsid])) {
    $xml = $object[$dsid]->content;
  }
  $form = xml_form_builder_get_form($form, $form_state, $form_name, $xml);
  // Make the Object label field required.
  if ($association['title_field']) {
    $title_field_parents = $association['title_field'];
    $title_field_parents[] = '#required';
    drupal_array_set_nested_value($form, $title_field_parents, TRUE);
  }
  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#name' => 'update',
    '#submit' => array('xml_form_builder_datastream_form_metadata_form_submit'),
  );
  return $form;
}

/**
 * Updates the datastream's content with generated xml from the metadata form.
 *
 * Also updates the objects label, and applies a transform to update the DC.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function xml_form_builder_datastream_form_metadata_form_submit(array $form, array &$form_state) {
  $object = islandora_object_load($form_state['pid']);
  $association = $form_state['association'];
  $label = NULL;
  if ($association['title_field']) {
    $title_field = drupal_array_get_nested_value($form, $association['title_field']);
    $label = $title_field['#value'];
  }
  $xml_form = new XMLForm($form_state);
  $document = $xml_form->submit($form, $form_state);
  xml_form_builder_update_object($object, $association, $document, $label);
  $form_state['redirect'] = "islandora/object/{$object->id}";
}

/**
 * Form for selecting which metadata datastream to create.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param AbstractObject $object
 *   Object to add the new metadata datastream to.
 *
 * @return array
 *   An array containing the form to be rendered.
 */
function xml_form_builder_create_metadata_form($form, &$form_state, $object) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/datastream.form');
  form_load_include($form_state, 'inc', 'islandora', 'includes/utilities');

  $datastreams = xml_form_builder_empty_metadata_datastreams($object);
  $options = array_combine($datastreams, $datastreams);
  if ($options) {
    $form_state['object_id'] = $object->id;
    $form['dsid'] = array(
      '#title' => t('Datastream ID'),
      '#type' => 'select',
      '#options' => $options,
      '#description' => t('Select the ID of a datastream to create new metadata.'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#submit' => array('xml_form_builder_create_metadata_form_submit'),
      '#value' => t('Create'),
    );
  }
  return $form;
}

/**
 * Redirects the user to the xml form association form.
 */
function xml_form_builder_create_metadata_form_submit($form, &$form_state) {
  $form_state['redirect'] = "islandora/edit_form/{$form_state['object_id']}/{$form_state['values']['dsid']}";
}

/**
 * A form for adding datastreams to an object.
 */
function xml_form_builder_add_datastream_page(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/add_datastream.form');
  return array(
    'core_form' => drupal_get_form('islandora_add_datastream_form', $object),
    'xml_form_fieldset' => (xml_form_builder_empty_metadata_datastreams($object) ?
      array(
        '#type' => 'fieldset',
        '#title' => t('Add A Metadata Datastream'),
        'xml_form' => drupal_get_form('xml_form_builder_create_metadata_form', $object),
      ) :
      array()
    ),
  );
}

/**
 * Get empty metadata datastreams on an object.
 */
function xml_form_builder_empty_metadata_datastreams($object) {
  module_load_include('inc', 'xml_form_builder', 'includes/associations');
  $datastreams = array();
  foreach (islandora_get_datastreams_requirements($object) as $dsid => $datastream) {
    if (!isset($object[$dsid])) {
      $datastreams[] = $dsid;
    }
  }
  $filtered_datastreams = array();
  foreach (xml_form_builder_get_associations(array(), $object->models, $datastreams) as $key => $value) {
    $filtered_datastreams[] = $value['dsid'];
  }
  return $filtered_datastreams;
}
