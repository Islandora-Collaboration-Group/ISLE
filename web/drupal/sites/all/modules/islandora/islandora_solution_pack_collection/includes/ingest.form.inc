<?php

/**
 * @file
 * Handles the ingestion of child objects that are not collections.
 */

// Constants.
define('ISLANDORA_BASIC_COLLECTION_FAKE_PID', 'fake:pid');

/**
 * Manage action that for ingestion of an object into the given collection.
 *
 * @param AbstractObject $collection
 *   The collection to ingest into.
 *
 * @return array
 *   The ingest form.
 */
function islandora_basic_collection_ingest_action(AbstractObject $collection) {
  if (($configuration = islandora_basic_collection_get_ingest_configuration($collection)) !== FALSE) {
    module_load_include('inc', 'islandora', 'includes/ingest.form');
    return drupal_get_form('islandora_ingest_form', $configuration);
  }
  drupal_not_found();
}

/**
 * Generates an ingest configuration from the given collection object.
 *
 * @see islandora_ingest_form()
 *
 * @param AbstractObject $collection
 *   A collection object to generate the configuration from.
 *
 * @return array
 *   An ingest configuration array as defined by islandora_ingest_form().
 */
function islandora_basic_collection_get_ingest_configuration(AbstractObject $collection) {
  $is_collection_object = in_array('islandora:collectionCModel', $collection->models);
  $has_policy = isset($collection['COLLECTION_POLICY']);
  if (!$is_collection_object || !$has_policy) {
    return array();
  }
  // The ISLANDORA_BASIC_COLLECTION_FAKE_PID is needed such that
  // constructObject() won't call getNextIdentifier(), needlessly.
  $object = $collection->repository->constructObject(ISLANDORA_BASIC_COLLECTION_FAKE_PID);
  $object->label = 'New Object';

  module_load_include('inc', 'islandora_basic_collection', 'includes/utilities');
  islandora_basic_collection_add_to_collection($object, $collection);
  $policy = new CollectionPolicy($collection['COLLECTION_POLICY']->content);
  return array(
    'collection' => $collection,
    'models' => array_keys($policy->getContentModels()),
    'objects' => array($object),
    'parent' => $collection->id,
  );
}

/**
 * Defines the select content model ingest step form.
 *
 * Assumes that only a single content model can be selected, and only a single
 * object will be ingested.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param array $models
 *   The models to include in the form.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_basic_collection_select_content_model_form(array $form, array &$form_state, array $models) {
  module_load_include('inc', 'islandora', 'includes/ingest.form');
  $options = array();
  $steps = islandora_ingest_form_get_shared_storage($form_state);
  $parent_pid = $steps['parent'];

  $collection = islandora_object_load($parent_pid);
  $policy = new CollectionPolicy($collection['COLLECTION_POLICY']->content);
  $cmodels = $policy->getContentModels();
  foreach ($models as $pid) {
    $object = islandora_object_load($pid);
    if ($object) {
      $options[$pid] = isset($cmodels[$pid]['name']) ? $cmodels[$pid]['name'] : $object->label;
    }
  }

  $model = isset($form_state['values']['models']) ? $form_state['values']['models'] : key($options);
  $shared_storage = &islandora_ingest_form_get_shared_storage($form_state);
  $shared_storage['models'] = array($model);
  $return_form = array(
    '#prefix' => '<div id="islandora-select-content-model-wrapper">',
    '#suffix' => '</div>',
    'models' => array(
      '#type' => 'select',
      '#title' => t('Select a Content Model to Ingest'),
      '#options' => $options,
      '#default_value' => $model,
      '#ajax' => array(
        'callback' => 'islandora_basic_collection_select_content_model_form_ajax_callback',
        'wrapper' => 'islandora-select-content-model-wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    ),
  );
  return $return_form;
}

/**
 * Ajax callback for the select content model form element.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_basic_collection_select_content_model_form_ajax_callback(array $form, array &$form_state) {
  return $form;
}

/**
 * Select a content model for the ingest object.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_basic_collection_select_content_model_form_submit(array $form, array &$form_state) {
  $model = $form_state['values']['models'];
  islandora_basic_collection_ingest_form_select_model($form_state, $model);
}

/**
 * Select a content model for the ingest object.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_basic_collection_select_content_model_form_undo_submit(array $form, array &$form_state) {
  islandora_basic_collection_ingest_form_unselect_model($form_state);
}

/**
 * Callback step for setting the ingest object's content model.
 *
 * @param array $form_state
 *   The Drupal form state.
 * @param string $model
 *   The model to select.
 */
function islandora_basic_collection_set_content_model_callback(array &$form_state, $model) {
  islandora_basic_collection_ingest_form_select_model($form_state, $model);
}

/**
 * Undo callback step for setting the ingest object's content model.
 *
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_basic_collection_set_content_model_undo_callback(array &$form_state) {
  islandora_basic_collection_ingest_form_unselect_model($form_state);
}

/**
 * Update the ingest object with collection policy data for the given model.
 *
 * @param array $form_state
 *   The Drupal form state.
 * @param string $model
 *   The PID of the selected model.
 */
function islandora_basic_collection_ingest_form_select_model(array &$form_state, $model) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $shared_storage = &islandora_ingest_form_get_shared_storage($form_state);
  $collection = $shared_storage['collection'];
  $object = islandora_ingest_form_get_object($form_state);
  // Set the has model relationship.
  $object->relationships->remove(FEDORA_MODEL_URI, 'hasModel');
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $model);
  // Set the objects namespace.
  $policy = new CollectionPolicy($collection['COLLECTION_POLICY']->content);
  $models = $policy->getContentModels();
  $model = $models[$model];
  $namespace = islandora_get_namespace($model['namespace']);
  // Ideally we wouldn't have to do this, we could have the identifier generated
  // at ingest time. Hell ideally Fedora would do it when just provided with a
  // namespace at ingest time.
  if ($object->id == ISLANDORA_BASIC_COLLECTION_FAKE_PID || islandora_get_namespace($object->id) !== $namespace) {
    $object->id = $object->repository->getNextIdentifier($namespace, variable_get('islandora_basic_collection_generate_uuid', FALSE));
  }
  // Limit the list of models to the selected model. We may not want to do this
  // in the future when we'll support ingesting multiple content models, at
  // such a time we can create a form for selecting content models and
  // have it control what models are availible. Until then this will have to
  // stand in.
  $shared_storage['models'] = array($model['pid']);
}

/**
 * Undo changes to the ingest object.
 *
 * We don't remove the objects PID here, as in most cases the namespace won't
 * change and there is no need for wasting good PID's.
 *
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_basic_collection_ingest_form_unselect_model(array &$form_state) {
  // Remove hasModel relationships.
  $object = islandora_ingest_form_get_object($form_state);
  $object->relationships->remove(FEDORA_MODEL_URI, 'hasModel');
  // Restore the list of all models to the shared storage.
  $shared_storage = &islandora_ingest_form_get_shared_storage($form_state);
  $step_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_basic_collection_select_content_model');
  $shared_storage['models'] = $step_storage['models'];
}
