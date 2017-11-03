<?php

/**
 * @file
 * Page callbacks and relevant forms (and friends).
 */

/**
 * Bit of a helper function for transforming to strings.
 *
 * Select options often have issues with "0", which happens to be one of our
 * states... Map around it to avoid issues.
 *
 * @param array $form_state
 *   Reference to form state, where we can stash a mapping for use in
 *   islandora_batch_map_value().
 *
 * @return array
 *   An array of two arrays:
 *   - the first maps state number to our key used for the "options" arrays.
 *   - the second maps our key in an "options" array to a human-readable
 *     version, for use as the actual options array.
 */
function islandora_batch_get_mappings(&$form_state) {
  $keys = array(
    ISLANDORA_BATCH_STATE__ERROR => 'error',
    ISLANDORA_BATCH_STATE__NOT_READY => 'not-ready',
    ISLANDORA_BATCH_STATE__READY => 'ready',
    ISLANDORA_BATCH_STATE__PENDING_CHILDREN => 'pending-children',
    ISLANDORA_BATCH_STATE__DONE => 'done',
  );
  $strings = array(
    ISLANDORA_BATCH_STATE__ERROR => t('Error'),
    ISLANDORA_BATCH_STATE__NOT_READY => t('Not ready to ingest'),
    ISLANDORA_BATCH_STATE__READY => t('Ready to ingest'),
    ISLANDORA_BATCH_STATE__PENDING_CHILDREN => t('Not ready to ingest; children pending'),
    ISLANDORA_BATCH_STATE__DONE => t('Ingested'),
  );
  $options = array();
  foreach ($keys as $key => $value) {
    $options[$value] = $strings[$key];
  }
  $form_state['stash']['keys'] = array_flip($keys);

  return array($keys, $options);
}

/**
 * Map option value back to state number.
 *
 * @param string $value
 *   A value for one of the options dealt with in
 *   islandora_batch_get_mappings().
 * @param array $form_state
 *   The form state, so we can access our "stashed" mapping.
 *
 * @return int
 *   The mapped state number, corresponding to $value.
 */
function islandora_batch_map_value($value, $form_state) {
  return $form_state['stash']['keys'][$value];
}

/**
 * Menu page callback to delete a single item.
 */
function islandora_batch_delete_item_page_callback($id) {
  return drupal_get_form('islandora_batch_delete_item_form', $id);
}

/**
 * Form building function to delete a single item.
 */
function islandora_batch_delete_item_form($form, &$form_state, $id) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/menu');
  return confirm_form(
    $form,
    t('Are you sure you want to delete "@item" from the ingest queue?', array(
        '@item' => $id,
    )),
    'islandora_batch/reports/queue',
    t('This will delete the item from the batch queue. Ingested objects will not be deleted from your repository.')
  );
}

/**
 * Form submission handler, to delete and single item.
 */
function islandora_batch_delete_item_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  list($id) = $form_state['build_info']['args'];
  islandora_batch_delete_object($id);
  drupal_set_message(t('Deleted "@item" from queue.', array(
    '@item' => $id,
  )));

}

/**
 * Menu page callback, to set the state for a single item.
 */
function islandora_batch_set_item_state_page_callback($id) {
  return drupal_get_form('islandora_batch_set_item_state_form', $id);
}

/**
 * Form buidling function, to set the state for a single item.
 */
function islandora_batch_set_item_state_form($form, &$form_state, $id) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  list($keys, $options) = islandora_batch_get_mappings($form_state);
  $current = islandora_batch_get_object_state($id);
  $form['state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#description' => t('The state of the given object in the batch queue.'),
    '#default_value' => $keys[(int) $current],
    '#options' => $options,
  );
  return confirm_form($form, t('Are you sure you want to change the state of "@item" in the queue?', array(
    '@item' => $id,
  )), 'islandora_batch/reports/queue');
}

/**
 * Form submission handler, to set the state for a single item.
 */
function islandora_batch_set_item_state_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  list($id) = $form_state['build_info']['args'];
  $state = $form_state['values']['state'];
  islandora_batch_update_object_state($id, islandora_batch_map_value($state, $form_state));
  drupal_set_message(t('Updated "@item" to "@state", in queue.', array(
    '@item' => $id,
    '@state' => $form['state']['#options'][$state],
  )));
}

/**
 * Menu callback, to set the state for all items in a batch set.
 */
function islandora_batch_set_set_items_state_page_callback($id, $management = FALSE) {
  return drupal_get_form('islandora_batch_set_set_items_state_form', $id, $management);
}

/**
 * Form building function, to set the state for all items in a batch set.
 */
function islandora_batch_set_set_items_state_form($form, &$form_state, $id, $management) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/menu');
  list($keys, $options) = islandora_batch_get_mappings($form_state);

  $form['source_state'] = array(
    '#type' => 'select',
    '#title' => t('Change from'),
    '#description' => t('Items in the set with this state will be changed.'),
    '#empty_option' => t('-- Any --'),
    '#empty_value' => 'any',
    '#options' => $options,
  );
  $form['dest_state'] = array(
    '#type' => 'select',
    '#title' => t('Change to'),
    '#description' => t('Items will be set to this state.'),
    '#options' => $options,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to change the state of items in set "@set" in the queue?', array(
      '@set' => $id,
    )),
    $management ? "admin/reports/islandora_batch_queue/$id" : "islandora_batch/reports/queue/$id"
  );
}

/**
 * Form submission handler, to set the state for all items in a batch set.
 */
function islandora_batch_set_set_items_state_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  list($id, $management) = $form_state['build_info']['args'];
  $source_state = $form_state['values']['source_state'];
  $dest_state = $form_state['values']['dest_state'];
  // Update object states.
  $updated = islandora_batch_update_set_object_states($id,
    islandora_batch_map_value($dest_state, $form_state),
    $source_state,
    islandora_batch_map_value($source_state, $form_state)
  );
  $form_state['redirect'] = array($management ? "admin/reports/islandora_batch_queue/$id" : "islandora_batch/reports/queue/$id");
  drupal_set_message(format_plural($updated, 'Updated 1 item in queue to "@state".', 'Updated @count items in queue to "@state".', array(
    '@state' => $form['dest_state']['#options'][$dest_state],
  )));
}

/**
 * Menu page callback, to delete a set and all contained items.
 */
function islandora_batch_delete_set_page_callback($id, $management = FALSE) {
  return drupal_get_form('islandora_batch_delete_set_form', $id, $management);
}

/**
 * Form building function, to delete a set and all contained items.
 */
function islandora_batch_delete_set_form($form, &$form_state, $id, $management) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/menu');
  return confirm_form(
    $form,
    t('Are you sure you want to delete the items in set "@set" from the queue?', array(
      '@set' => $id,
    )),
    $management ? "admin/reports/islandora_batch_queue/$id" : "islandora_batch/reports/queue/$id",
    t('This will delete the set from the batch queue. Ingested objects will not be deleted from your repository.')
  );
}

/**
 * Form submission handler, to delete a set and all contained items.
 */
function islandora_batch_delete_set_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  list($id, $management) = $form_state['build_info']['args'];
  $deleted = islandora_batch_delete_set($id);
  $form_state['redirect'] = array(
    $management ? 'admin/reports/islandora_batch_sets' : 'islandora_batch/reports/set',
  );
  drupal_set_message(format_plural($deleted, 'Deleted 1 item from queue.', 'Deleted @count items from queue.'));
}

/**
 * Page callback to (re)start a batch on a set.
 */
function islandora_batch_set_restart_batch_page_callback($id, $management = FALSE) {
  return drupal_get_form('islandora_batch_set_restart_batch_form', $id, $management);
}

/**
 * Form building function to (re)start a batch on a set.
 */
function islandora_batch_set_restart_batch_form($form, &$form_state, $id, $management) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/ingest.batch');
  $lock_name = islandora_batch_get_lock_name($id);

  $form['info'] = array(
    '#type' => 'item',
    '#markup' => t('This will attempt to process all items in the "Ready" and "Not ready to ingest; children pending" states.'),
  );

  $form['global_notice'] = array(
    '#type' => 'item',
    '#title' => t('Global batch ingest lock'),
    '#markup' => lock_may_be_available(ISLANDORA_BATCH_LOCK_NAME) ? t('Available') : t('Locked'),
    '#description' => t('If this is locked, there may be a general batch being run over all items in the queue (in which case we will have to wait until it completes), or it may be possible that another user has just started a batch (in which case reloading the page may show that it is now available).'),
  );

  $set_lock = lock_may_be_available($lock_name);
  $form['set_notice'] = array(
    '#type' => 'item',
    '#title' => t('Set batch ingest lock'),
    '#markup' => $set_lock ? t('Available') : t('Locked'),
    '#description' => t('The lock for the current set, to prevent other entities from trying to process the same set of items concurrently.'),
  );
  $form['break_set_lock'] = array(
    '#type' => 'checkbox',
    '#title' => t('Break set ingest lock'),
    '#description' => t('If the batch failed due to some error condition and you want to restart it now, you can break the lock; otherwise, breaking the lock and trying to batch when somebody else is processing the set will result in undefined behaviour.'),
    '#access' => !$set_lock,
  );

  return confirm_form(
    $form,
    t('Restart processing set "@set"?', array(
      '@set' => $id,
    )),
    $management ? "admin/reports/islandora_batch_queue/$id" : "islandora_batch/reports/queue/$id",
    t('A long time might be spent waiting to acquire locks if both locks are not available.'),
    t('Start batch processing')
  );
}

/**
 * Form submission callback to (re)start a batch on a set.
 */
function islandora_batch_set_restart_batch_form_submit(&$form, &$form_state) {
  list($id, $management) = $form_state['build_info']['args'];

  $form_state['redirect'] = array($management ? "admin/reports/islandora_batch_queue/$id" : "islandora_batch/reports/queue/$id");
  if (isset($form_state['values']['break_set_lock']) && $form_state['values']['break_set_lock']) {
    $lock_name = islandora_batch_get_lock_name($id);
    $deleted = db_delete('semaphore')
      ->condition('name', $lock_name)
      ->execute();

    if ($deleted) {
      drupal_set_message(t('Broke lock for "@id".', array(
        '@id' => $id,
      )));
    }
    else {
      drupal_set_message(t('Attempted to break lock for "@id"; however, a lock did not exist.', array(
        '@id' => $id,
      )));
    }
  }

  islandora_batch_set_operations(array(
    'ingest_set' => $id,
  ));
}

/**
 * Page callback to purge ingested objects that belong to a set.
 */
function islandora_batch_purge_set_object_page_callback($id, $management = FALSE) {
  return drupal_get_form('islandora_batch_purge_set_object_form', $id, $management);
}

/**
 * Form building function to purge ingested objects.
 */
function islandora_batch_purge_set_object_form($form, &$form_state, $id, $management) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/ingest.batch');
  $lock_name = islandora_batch_get_lock_name($id);

  $form['info'] = array(
    '#type' => 'item',
    '#markup' => t('WARNING: THIS ACTION CAN NOT BE UNDONE. This will purge ingested objects in set @set from the repository.
      Any modifications to objects ingested from set @set will be lost.</br>This process does not remove the batch set or items from it.', array(
        '@set' => $id,
      )),
  );

  $form['global_notice'] = array(
    '#type' => 'item',
    '#title' => t('Global batch ingest lock'),
    '#markup' => lock_may_be_available(ISLANDORA_BATCH_LOCK_NAME) ? t('Available') : t('Locked'),
    '#description' => t('If this is locked, there may be a general batch being run over all items in the queue (in which case we will have to wait until it completes), or it may be possible that another user has just started a batch (in which case reloading the page may show that it is now available).'),
  );

  $set_lock = lock_may_be_available($lock_name);
  $form['set_notice'] = array(
    '#type' => 'item',
    '#title' => t('Set batch ingest lock'),
    '#markup' => $set_lock ? t('Available') : t('Locked'),
    '#description' => t('The lock for the current set, to prevent other entities from trying to process the same set of items concurrently.'),
  );

  $form['lock_info'] = array(
    '#type' => 'item',
    '#markup' => t('If the lock is not avaliable, the purge process will attempt to obtain one before proceeding.'),
  );

  return confirm_form(
    $form,
    t('Purge ingested objects in set "@set"?', array(
      '@set' => $id,
    )),
    $management ? "admin/reports/islandora_batch_queue/$id" : "islandora_batch/reports/queue/$id",
    NULL,
    t('Purge objects from repository')
  );
}

/**
 * Form submission callback to delete ingested objects.
 */
function islandora_batch_purge_set_object_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'islandora_batch', 'includes/purge-set-object.batch');
  list($id, $management) = $form_state['build_info']['args'];
  $form_state['redirect'] = array($management ? "admin/reports/islandora_batch_queue/$id" : "islandora_batch/reports/queue/$id");
  islandora_batch_batch_purge_object_in_set($id);
}
