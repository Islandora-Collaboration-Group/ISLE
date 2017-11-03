<?php

/**
 * @file
 * Database Helper Functions.
 */

/**
 * Delete batch set from the drupal database and mark files for removal.
 *
 * @param int $set_id
 *   An integer identifying a batch set.
 *
 * @return int
 *   An integer of objects removed from queue.
 */
function islandora_batch_delete_set($set_id) {
  $deleted = 0;
  if ($set_id !== NULL) {
    module_load_include('inc', 'islandora_batch', 'includes/utilities');
    $transaction = db_transaction();

    $s = db_select('islandora_batch_queue', 'q')
      ->fields('q', array('id'))
      ->condition('sid', $set_id);
    $items = $s->execute()->fetchCol();
    foreach ($items as $item) {
      islandora_batch_remove_file_usages($item);
    }
    db_delete('islandora_batch_state')
      ->condition('id', $s, 'IN')
      ->execute();
    db_delete('islandora_batch_queue_messages')
      ->condition('id', $s, 'IN')
      ->execute();
    db_delete('islandora_batch_resources')
      ->condition('id', $s, 'IN')
      ->execute();
    $deleted = db_delete('islandora_batch_queue')
      ->condition('sid', $set_id)
      ->execute();
    db_delete('islandora_batch_set')
      ->condition('id', $set_id)
      ->execute();

    // Mark any files as being unused--free to be delete.
    $q = db_select('file_usage', 'u')
      ->fields('u', array('fid'))
      ->condition('module', 'islandora_batch')
      ->condition('type', 'islandora_batch_set')
      ->execute();
    $files = file_load_multiple($q->fetchCol());
    foreach ($files as $file) {
      file_usage_delete($file, 'islandora_batch', 'islandora_batch_set', $set_id);
    }
  }
  return $deleted;
}

/**
 * Delete single object from the Drupal database and mark file for removal.
 *
 * @param string $pid
 *   Object PID to remove from queue.
 */
function islandora_batch_delete_object($pid) {
  if ($pid !== NULL) {
    module_load_include('inc', 'islandora_batch', 'includes/utilities');
    $transaction = db_transaction();
    islandora_batch_remove_file_usages($pid);
    db_delete('islandora_batch_state')
      ->condition('id', $pid)
      ->execute();
    db_delete('islandora_batch_resources')
      ->condition('id', $pid)
      ->execute();
    db_delete('islandora_batch_queue_messages')
      ->condition('id', $pid)
      ->execute();
    db_delete('islandora_batch_queue')
      ->condition('id', $pid)
      ->execute();
  }
}

/**
 * Get all Batch sets stored in the database.
 *
 * @param int|bool|NULL $completed
 *   An optional timestamp:
 *   - An integer timestamp will return sets dependent on $op.
 *   - Boolean TRUE will provide all completed sets.
 *   - Boolean FALSE will provide all incomplete sets.
 *   - NULL (the default) will provide ALL sets.
 * @param string $op
 *   Only used with non-boolean/NULL values of $completed; represents the
 *   operator to use in the comparison. Defaults to '<=', to select
 *   objects completed before (or on) a specific timestamp.
 *
 * @return array
 *   An array of batch set ids for batches that exist in the database.
 */
function islandora_batch_get_sets($completed = NULL, $op = '<=') {
  $query = db_select('islandora_batch_set', 's')
    ->fields('s', array('id'));
  if ($completed !== NULL) {
    if ($completed === TRUE) {
      $query->isNotNull('completed');
    }
    elseif ($completed === FALSE) {
      $query->isNull('completed');
    }
    else {
      $query->condition('completed', $completed, $op);
    }
  }
  $sets = $query->execute();
  return $sets->fetchCol();
}

/**
 * Get all objects queued for a specified set.
 *
 * @param int $set_id
 *   An integer identifying a batch set.
 *
 * @return array
 *   An array of objects queued for a specific set.
 */
function islandora_batch_get_queued_items_for_set($set_id) {
  $results = array();
  $q = db_select('islandora_batch_queue', 'q');
  $q->join('islandora_batch_state', 's', 's.id = q.id');
  $items = $q->fields('q', array('id'))
    ->condition('q.sid', $set_id)
    ->execute();
  while ($item = $items->fetchAssoc()) {
    $results[] = $item['id'];
  }
  return $results;
}

/**
 * Insert or update the data of a queued object.
 *
 * @param string $pid
 *   The object id identifying the queued batch object.
 * @param string $data
 *   Updated data value.
 */
function islandora_batch_merge_queued_object_data($pid, $data) {
  db_merge('islandora_batch_queue')
    ->key(array('id' => $pid))
    ->fields(array(
      'data' => $data,
    ))
    ->execute();
}

/**
 * Insert record or update the state of a queued object.
 *
 * @param string $pid
 *   The object id identifying the queued batch object.
 * @param int $state
 *   Updated state value.
 */
function islandora_batch_merge_object_state($pid, $state) {
  db_merge('islandora_batch_state')
    ->key(array('id' => $pid))
    ->fields(array(
      'state' => $state,
    ))
    ->execute();
}

/**
 * Insert record or update the message of a queued object.
 *
 * @param string $pid
 *   The object id identifying the queued batch object.
 * @param string $message
 *   Message for the object.
 */
function islandora_batch_merge_object_message($pid, $message) {
  db_merge('islandora_batch_queue_messages')
    ->key(array('id' => $pid))
    ->fields(array('message' => $message))
    ->execute();
}

/**
 * Get the batch set id related to a queued object.
 *
 * @param string $pid
 *   The object id identifying the queued batch object.
 *
 * @return int
 *   The id of the batch set the object is apart of.
 */
function islandora_batch_get_set_for_object($pid) {
  $set_id = db_select('islandora_batch_queue', 'q')
    ->fields('q', array('sid'))
    ->condition('id', $pid)
    ->execute()
    ->fetchField();
  return $set_id;
}

/**
 * Get the batch id related to a queued object.
 *
 * @param string $pid
 *   The object id identifying the queued batch object.
 *
 * @return int
 *   The batch id for the object.
 */
function islandora_batch_get_batch_id_for_object($pid) {
  $id = db_select('islandora_batch_queue', 'q')
    ->fields('q', array('bid'))
    ->condition('id', $pid)
    ->execute()
    ->fetchField();
  return $id;
}

/**
 * Get the count of queued objects in the specified batch set.
 *
 * @param int $set_id
 *   An integer identifying a batch set.
 *
 * @return int
 *   The count of queued objects in the specified batch set.
 */
function islandora_batch_get_count_of_queued_set_objects($set_id) {
  $count = db_select('islandora_batch_queue', 'q')
    ->fields('q', array())
    ->condition('sid', $set_id)
    ->countQuery()
    ->execute()
    ->fetchField();
  return $count;
}

/**
 * Update the state of all objects in the specified set.
 *
 * @param int $id
 *   The set id identifying the queued batch set.
 * @param int $new_state
 *   The new state value to update to.
 * @param string $source_state
 *   The selected source state.
 * @param int $filter_on_state
 *   The old state id to use for filtering.
 *
 * @return int
 *   The number of updated objects.
 */
function islandora_batch_update_set_object_states($id, $new_state, $source_state, $filter_on_state) {
  $s = db_select('islandora_batch_queue', 'q')
    ->fields('q', array('id'))
    ->condition('sid', $id);
  $q = db_update('islandora_batch_state')
    ->fields(array('state' => $new_state))
    ->condition('id', $s, 'IN');

  if ($source_state !== 'any') {
    $q->condition('state', $filter_on_state);
  }

  $updated = $q->execute();
  return $updated;
}

/**
 * Update the state of a queued object.
 *
 * @param string $pid
 *   The object id identifying the queued batch object.
 * @param int $state
 *   Updated state value.
 */
function islandora_batch_update_object_state($pid, $state) {
  db_update('islandora_batch_state')
    ->fields(array('state' => $state))
    ->condition('id', $pid)
    ->execute();
}

/**
 * Get the current state of an object.
 *
 * @param string $pid
 *   The object id identifying the queued batch object.
 */
function islandora_batch_get_object_state($pid) {
  $state = db_select('islandora_batch_state', 's')
    ->fields('s', array('state'))
    ->condition('id', $pid)
    ->execute()
    ->fetchField();
  return $state;
}

/**
 * Check to see if a batch set was fully ingested without errors.
 *
 * @param int $set_id
 *   An integer identifying a batch set.
 *
 * @return bool
 *   TRUE if the batch sets was completely ingested without errors or not
 *   importing all objects in the set; otherwise, FALSE.
 */
function islandora_batch_check_if_set_is_fully_ingested($set_id) {
  $query = db_select('islandora_batch_queue', 'q');
  $query->join('islandora_batch_state', 's', 's.id = q.id');
  $result = $query->fields('q', array('id'))
    ->condition('q.sid', $set_id)
    ->condition('s.state', ISLANDORA_BATCH_STATE__DONE, '<')
    ->countQuery()
    ->execute()
    ->fetchField();
  if ($result == 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Check to see if a batch set has failed ingesting any objects.
 *
 * @param int $set_id
 *   An integer identifying a batch set.
 *
 * @return bool
 *   TRUE if the batch sets has any failed to ingest objects.
 */
function islandora_batch_check_if_set_failed_ingest($set_id) {
  $query = db_select('islandora_batch_queue', 'q');
  $query->join('islandora_batch_state', 's', 's.id = q.id');
  $result = $query->fields('q', array('id'))
    ->condition('q.sid', $set_id)
    ->condition('s.state', ISLANDORA_BATCH_STATE__ERROR)
    ->countQuery()
    ->execute()
    ->fetchField();
  if ($result) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Get the count of queued sets that are ready for processing.
 *
 * @return int
 *   The count of queued sets that are ready for processing.
 */
function islandora_batch_get_count_of_queued_sets_ready_to_process() {
  $count = 0;
  // Get all the sets in the queue and check each one to see if they are ready.
  $sets = islandora_batch_get_sets();
  if (!empty($sets)) {
    foreach ($sets as $set) {
      if (islandora_batch_check_set_for_objects_ready_for_processing($set)) {
        $count++;
      }
    }
  }
  return $count;
}

/**
 * Get the count of queued objects ready for processing.
 *
 * @return int
 *   The count of all queued objects ready for processing.
 */
function islandora_batch_get_count_of_queued_objects_ready_to_process() {
  $count = db_select('islandora_batch_state', 'q')
    ->fields('q', array())
    ->condition('state', array(ISLANDORA_BATCH_STATE__READY, ISLANDORA_BATCH_STATE__PENDING_CHILDREN), 'IN')
    ->countQuery()
    ->execute()
    ->fetchField();
  return $count;
}

/**
 * Check to see if a batch set has objects that need processing.
 *
 * @param int $set_id
 *   An integer identifying a batch set.
 *
 * @return bool
 *   TRUE if the batch sets has objects that are ready for ingesting; otherwise,
 *   FALSE.
 */
function islandora_batch_check_set_for_objects_ready_for_processing($set_id) {
  $query = db_select('islandora_batch_queue', 'q');
  $query->join('islandora_batch_state', 's', 's.id = q.id');
  $result = $query->fields('q', array('id'))
    ->condition('q.sid', $set_id)
    ->condition('s.state', array(ISLANDORA_BATCH_STATE__READY, ISLANDORA_BATCH_STATE__PENDING_CHILDREN), 'IN')
    ->countQuery()
    ->execute()
    ->fetchField();
  if ($result > 0) {
    return TRUE;
  }
  return FALSE;
}
