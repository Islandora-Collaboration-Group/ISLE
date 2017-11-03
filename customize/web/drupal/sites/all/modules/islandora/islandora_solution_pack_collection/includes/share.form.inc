<?php

/**
 * @file
 * Shares an object that is not a collection to an additional collection.
 */


/**
 * Form setup.
 *
 * @param array $form
 *   Form variable
 * @param array $form_state
 *   The context of this form.
 * @param object $islandora_object
 *   The object to perform the operation on.
 */
function islandora_basic_collection_share_item_form($form, &$form_state, $islandora_object) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Share single item'),
  );
  $form['new_collection_name'] = array(
    '#autocomplete_path' => 'islandora/basic_collection/find_collections',
    '#type' => 'textfield',
    '#title' => t('Name'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Share Object',
  );
  $form_state['basic_collection_share'] = $islandora_object->id;
  return $form;
}

/**
 * Validates the form.
 *
 * @param array $form
 *   Form variable
 * @param array $form_state
 *   The context of this form.
 */
function islandora_basic_collection_share_item_form_validate($form, &$form_state) {
  $new_collection = islandora_object_load($form_state['values']['new_collection_name']);
  $collection_models = islandora_basic_collection_get_collection_content_models();
  $is_a_collection = FALSE;
  if (is_object($new_collection)) {
    $is_a_collection = (
      (count(array_intersect($collection_models, $new_collection->models)) > 0) &&
      isset($new_collection['COLLECTION_POLICY'])
    );
  }
  if (!$is_a_collection) {
    form_set_error('new_collection_name', t('Not a valid collection'));
  }
  $has_ingest_permissions = islandora_object_access(ISLANDORA_INGEST, $new_collection);
  if (!$has_ingest_permissions) {
    form_set_error('new_collection_name', t('You do not have permission to ingest objects to this collection'));
  }
}

/**
 * Processes the form and migrates the objects.
 *
 * @param array $form
 *   Form variable
 * @param array $form_state
 *   The context of this form.
 */
function islandora_basic_collection_share_item_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_basic_collection', 'includes/utilities');
  $object = islandora_object_load($form_state['basic_collection_share']);
  $new_collection = islandora_object_load($form_state['values']['new_collection_name']);

  if ($object && $new_collection) {
    islandora_basic_collection_add_to_collection($object, $new_collection);
    $message = t('The object @object has been added to @collection', array(
                 '@object' => $object->label,
                 '@collection' => $new_collection->label));
    drupal_set_message($message);
  }

}
