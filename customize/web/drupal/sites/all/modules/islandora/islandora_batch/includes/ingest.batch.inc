<?php

/**
 * @file
 * Batch ingest batch operations.
 */

// Some names which get reused.
define('ISLANDORA_BATCH_LOCK_NAME', 'islandora_batch_ingest');
define('ISLANDORA_BATCH_TIMER_NAME', 'islandora_batch_timer');
define('ISLANDORA_BATCH_LOCK_TIMER', 'islandora_batch_lock_timer');

// Ints indicating the state.
define('ISLANDORA_BATCH_LOCK_TIMING', 0);
define('ISLANDORA_BATCH_FINISHED_SET', 1);
define('ISLANDORA_BATCH_EMPTY_SET', 2);

/**
 * Function to get the average.
 *
 * @param array $context
 *   The context
 */
function islandora_batch_get_average($context) {
  if ($context['results']['count'] > 0) {
    return $context['results']['sum'] / $context['results']['count'];
  }
}

/**
 * Batch operation to run a preprocessor.
 */
function islandora_batch_ingest_preprocess($preprocessor, &$context) {
  islandora_batch_handle_preprocessor($preprocessor);
  $context['results']['ingest_set'] = $preprocessor->getSetId();
  $context['message'] = t('Preprocessed target file.');
}

/**
 * Get the name of the lock to acquire/release.
 *
 * @param string|int|NULL $ingest_set
 *   A string or integer identifying an ingest set to process. NULL indicates
 *   that it is a general/set-independent batch, processing everything in the
 *   queue.
 *
 * @return string
 *   A string identifying a lock.
 */
function islandora_batch_get_lock_name($ingest_set) {
  if ($ingest_set === NULL) {
    return ISLANDORA_BATCH_LOCK_NAME;
  }
  else {
    return "islandora_batch_ingest_set_{$ingest_set}";
  }
}

/**
 * Figure out for how long a lock should be acquired.
 *
 * @param float $timeout
 *   The total amount of time for which we want to process a batch, in seconds.
 *   A negative value indicates that we are to be processing for an undefined
 *   amount of time.
 *
 * @return float
 *   An amount of time, in seconds.
 */
function islandora_batch_get_lock_timeout($timeout) {
  if ($timeout < 0) {
    $max_exec = intval(ini_get('max_execution_time'));
    if ($max_exec > 0) {
      // max_execution_time could be 0 if run from CLI (drush?)
      return min(3600.0, $max_exec);
    }
    else {
      // Lock for an hour by default.
      return 3600.0;
    }
  }
  else {
    return min(3600.0, $timeout);
  }
}

/**
 * Acquire lock for batch ingest.
 *
 * Different sets should be able to be run at the same time.
 * The general/set-independent batch should not be able to run at the same time
 * as any other batch.
 *
 * @param string|int|NULL $ingest_set
 *   A string or integer identifying an ingest set to process. NULL indicates
 *   that it is a general/set-independent batch, processing everything in the
 *   queue.
 * @param float $timeout
 *   The total amount of time for which we want to process a batch, in seconds.
 *   A negative value indicates that we are to be processing for an undefined
 *   amount of time. Defaults to -1.0.
 *
 * @return float|bool
 *   The amount of time for which a suitable lock was acquired, in seconds;
 *   otherwise, FALSE.
 */
function islandora_batch_get_lock($ingest_set, $timeout = -1.0) {
  $lock_timeout = islandora_batch_get_lock_timeout($timeout);

  $globally_locked = lock_acquire(ISLANDORA_BATCH_LOCK_NAME, $lock_timeout) ||
    ((($timeout >= 0) && !lock_wait(ISLANDORA_BATCH_LOCK_NAME, $timeout / 3) && lock_acquire(ISLANDORA_BATCH_LOCK_NAME, $lock_timeout))) ||
    (!lock_wait(ISLANDORA_BATCH_LOCK_NAME, $lock_timeout / 3) && lock_acquire(ISLANDORA_BATCH_LOCK_NAME, $lock_timeout));

  if ($globally_locked) {

    if ($ingest_set === NULL) {
      // General batch; ensure that no other batch is running when getting
      // lock.
      $q = db_select('semaphore', 's')
        ->fields('s', array())
        ->condition('name', db_like('islandora_batch_ingest_set_') . '%', 'LIKE')
        ->condition('expire', microtime(TRUE), '>')
        ->countQuery()
        ->execute();
      $num_sets_locked = intval($q->fetchField());
      $fully_locked = $num_sets_locked === 0;
      if (!$fully_locked) {
        // Appears to be sets in progress... Might as well release the lock.
        lock_release(ISLANDORA_BATCH_LOCK_NAME);
        return FALSE;
      }
      else {
        return $lock_timeout;
      }
    }
    else {
      // Set batch; ensure that general batch (and current set) is not running.
      $lock_name = islandora_batch_get_lock_name($ingest_set);
      $locked = lock_acquire($lock_name, $lock_timeout) ||
        ((($timeout >= 0) && !lock_wait($lock_name, $timeout / 3) && lock_acquire($lock_name, $lock_timeout))) ||
        (!lock_wait($lock_name, $lock_timeout / 3) && lock_acquire($lock_name, $lock_timeout));
      lock_release(ISLANDORA_BATCH_LOCK_NAME);
      if (!$locked) {
        return FALSE;
      }
      else {
        return $lock_timeout;
      }
    }
  }
}

/**
 * Batch operation to ingest from the queue.
 *
 * @param array $parameters
 *   An associative array of parameters to affect processing. Includes:
 *   - ingest_set: A string or integer used to limit items to ingest to the
 *     results of a single preprocessing run. This can instead be included as
 *     $context['results']['ingest_set']).
 *   - timeout: An amount of time in seconds. If provided, we will only ingest
 *     until this amount of time is reached (or we think we'll exceed it,
 *     based on a running average). A negative value (the default) indicates
 *     that we should try to run the batch until we exhaust the queue.
 * @param array $context
 *   The batch context.
 */
function islandora_batch_ingest_process(array $parameters, &$context) {
  module_load_include('inc', 'islandora_batch', 'includes/utilities');
  extract($parameters);

  if (!isset($ingest_set)) {
    if (isset($context['results']['ingest_set'])) {
      $ingest_set = $context['results']['ingest_set'];
    }
    else {
      $ingest_set = NULL;
    }
  }
  if (isset($ingest_set) && !isset($context['sandbox']['set_count'])) {
    // Make sure we have the set set in results, if we have one.
    $context['results']['ingest_set'] = $ingest_set;

    // Grab a count of the number of items in the set.
    module_load_include('inc', 'islandora_batch', 'includes/db');
    $count = islandora_batch_get_count_of_queued_set_objects($ingest_set);
    $context['sandbox']['set_count'] = intval($count);
  }

  if (!isset($timeout)) {
    // XXX: Magic value (negative), causes the batch to run until all objects
    // have been processed.
    $timeout = -1.0;
  }

  $state = ISLANDORA_BATCH_FINISHED_SET;

  if (isset($context['results']['timer'])) {
    // XXX: Hackishly set up our timer again...
    global $timers;
    $timers[ISLANDORA_BATCH_TIMER_NAME] = $context['results']['timer'];
  }

  timer_start(ISLANDORA_BATCH_TIMER_NAME);

  // Keep track of the states of each set.
  $set_states = array();

  $lock_timeout = islandora_batch_get_lock($ingest_set, $timeout);

  // Wait up to a third of our processing time to try to get the ingest lock.
  if ($lock_timeout !== FALSE) {
    timer_start(ISLANDORA_BATCH_LOCK_TIMER);

    // Try ingesting objects that are waiting for children, if all the children
    // are ready.
    // Select all children which are NOT fully ingested.
    $set_states[] = $state = islandora_batch_process_results(islandora_batch_get_parents($ingest_set), $timeout, $lock_timeout, $context);

    // If the last part finished completely... (due to timeout, if it didn't).
    if (in_array($state, array(ISLANDORA_BATCH_FINISHED_SET, ISLANDORA_BATCH_EMPTY_SET), TRUE)) {
      // ... Continue processing.
      $set_states[] = $state = islandora_batch_process_results(islandora_batch_get_ready($ingest_set), $timeout, $lock_timeout, $context);
    }

    if (in_array($state, array(ISLANDORA_BATCH_FINISHED_SET, ISLANDORA_BATCH_EMPTY_SET), TRUE)) {
      // Attempt to process any parents which might now be ready to go, if we
      // have any time left.
      $set_states[] = $state = islandora_batch_process_results(islandora_batch_get_parents($ingest_set), $timeout, $lock_timeout, $context);
    }
    timer_stop(ISLANDORA_BATCH_LOCK_TIMER);
    islandora_batch_timer_reset(ISLANDORA_BATCH_LOCK_TIMER);
  }

  // Attempting to process each set (parent/child/parent) returned each an
  // empty set, so we're done.
  $all_set_counts = array_count_values($set_states);
  $all_sets_finished = isset($all_set_counts[ISLANDORA_BATCH_EMPTY_SET]) &&
    $all_set_counts[ISLANDORA_BATCH_EMPTY_SET] === count($set_states);

  if (!$all_sets_finished && isset($parameters['timeout']) && !islandora_batch_time_check(ISLANDORA_BATCH_TIMER_NAME, $timeout * 1000 / 3, $timeout)) {
    // If we're not completely finished but we're ingesting for a limited
    // amount of time and we do not have enough time to try acquiring the
    // lock again, inform the batch process so it stops.
    $context['finished'] = 1;
  }
  elseif ($all_sets_finished) {
    // We finished the queue of objects.
    $context['finished'] = 1;
  }
  elseif (isset($context['sandbox']['set_count'])) {
    // We're not done, but we have a restricted set of things... Let's report
    // how far along we are.
    $context['sandbox']['current'] = $context['results']['count'];
    $context['finished'] = 1.0 * $context['sandbox']['current'] / $context['sandbox']['set_count'];
  }
  else {
    // Otherwise, make us get scheduled again.
    // XXX: Count of loops is just to offer /some/ indication of progress...
    // estimated times will be meaningless.
    $context['sandbox']['loops'] = isset($context['sandbox']['loops']) ?
      ($context['sandbox']['loops'] + 1) :
      1;
    $context['finished'] = 1.0 * $context['sandbox']['loops'] / ($context['sandbox']['loops'] + 1);
  }

  $context['results']['timer'] = timer_stop(ISLANDORA_BATCH_TIMER_NAME);
}

/**
 * Require a query to contain an ID in a set, if the set is provided.
 *
 * @param string $query
 *   A select query to which we may add a condition.
 * @param array $ingest_set
 *   An array of object which should exist in the database, to restrict queries.
 *   (Used to allow a set of preprocessed objects to be ingested immediatedly
 *   from the web front-end).
 */
function islandora_batch_restrict_to_set($query, $table_alias, $ingest_set) {
  if ($ingest_set) {
    $query->condition("$table_alias.sid", $ingest_set);
  }
}

/**
 * Get all parents which are ready to go (in the given set, if populated).
 */
function islandora_batch_get_parents($ingest_set) {
  // Select all children which are NOT fully ingested.
  $child_query = db_select('islandora_batch_queue', 'c')
    ->fields('c')
    ->where('c.parent = p.id')
    ->isNotNull('c.parent');
  $c_alias = $child_query->join('islandora_batch_state', 'cs',
    'c.id = cs.id');
  $child_query->condition('cs.state', ISLANDORA_BATCH_STATE__DONE, '<>');

  // Select all items which do not have children which are not fully
  // ingested, but are waiting for children.
  $parent_query = db_select('islandora_batch_queue', 'p')
    ->fields('p')
    ->fields('ps', array('state'));

  islandora_batch_restrict_to_set($parent_query, 'p', $ingest_set);

  $p_alias = $parent_query->join('islandora_batch_state', 'ps',
    'p.id = ps.id');
  $parent_query->condition('state', ISLANDORA_BATCH_STATE__PENDING_CHILDREN);

  $parent_query->notExists($child_query);

  $parent_query->range(0, 100);

  return $parent_query->execute();
}

/**
 * Get just the query to find all objects which are ready to be ingested.
 */
function islandora_batch_get_ready_query($ingest_set) {
  // Select other objects which are ready to be processed.
  $other_query = db_select('islandora_batch_queue', 'q')
    ->fields('q')
    ->fields('s', array('state'));

  islandora_batch_restrict_to_set($other_query, 'q', $ingest_set);

  $other_alias = $other_query->join('islandora_batch_state', 's',
    'q.id = s.id AND s.state = :s_state', array(':s_state' => ISLANDORA_BATCH_STATE__READY));

  return $other_query;
}

/**
 * Get all objects which are ready to be ingested.
 */
function islandora_batch_get_ready($ingest_set, $count = 100) {
  $q = islandora_batch_get_ready_query($ingest_set);

  if ($count > 0) {
    $q->range(0, $count);
  }

  return $q->execute();
}

/**
 * Check if we have enough time to perform "next_op", with a bit of padding.
 *
 * @param string $timer
 *   A string containing the name of a timer to check.
 * @param float $next_op
 *   A floating-point number which represents an amount of time which can pass
 *   before the given timer hits the value of the $timeout parameter, in
 *   milliseconds.
 * @param float $timeout
 *   A floating-point number representing the amount of time we do not with to
 *   pass on the timer, in seconds.
 */
function islandora_batch_time_check($timer, $next_op, $timeout) {
  return (timer_read($timer) + $next_op) / 1000 < $timeout + 5;
}

/**
 * A test implementation, to dump contents...
 */
function test_islandora_batch_process_results($results, $timeout, $lock_timeout, &$context) {
  dlm($results->getQueryString(), 'Query');
  foreach ($results as $result) {
    dlm($result, $result->id);
  }
  return TRUE;
}


/**
 * Process set of result from the islandora_batch_queue table.
 *
 * @param object $results
 *   The query results.
 * @param array $context
 *   The array of batch context.
 *
 * @return int
 *   A flag indicating the state.
 */
function islandora_batch_process_results($results, $timeout, $lock_timeout, &$context) {
  $average_time = -1;

  while ((
      $average_time < 0 || $timeout < 0 ||
      islandora_batch_time_check(ISLANDORA_BATCH_TIMER_NAME, $average_time, $timeout)
    ) &&
    ($object = $results->fetchAssoc()) &&
    islandora_batch_time_check(ISLANDORA_BATCH_LOCK_TIMER, $lock_timeout * 1000 / 3, $lock_timeout)
  ) {
    $start = timer_read(ISLANDORA_BATCH_TIMER_NAME);
    // Process a single object.
    $ingest_object = unserialize($object['data']);

    if ($object['state'] !== ISLANDORA_BATCH_STATE__DONE) {
      // Both a simple state and state with message return are accepted.
      $process_results = $ingest_object->batchProcess();
      $object['state'] = is_array($process_results) ? $process_results['state'] : $process_results;
    }

    if ($object['state'] === ISLANDORA_BATCH_STATE__DONE) {
      try {
        // XXX: Due to how the things currently work, the user name and
        // password hash gets serialized, so later changes to the password
        // can break things... Let's set the password again.
        $username = $ingest_object->repository->api->connection->username;
        $user = user_load_by_name($username);
        if ($ingest_object->repository->api->connection->password != $user->pass) {
          $ingest_object->repository->api->connection->password = $user->pass;
        }

        // Push to backend.
        $ingested_object = islandora_add_object($ingest_object);
        if ($ingested_object) {
          $object['data'] = serialize($ingested_object);
          $context['message'] = t('Ingested %pid.', array('%pid' => $ingested_object->id));
        }
        else {
          // Failed to ingest...  Flag an error.
          $object['state'] = ISLANDORA_BATCH_STATE__ERROR;
          $context['message'] = t('Unknown error: Failed to ingest %pid.', array('%pid' => $ingest_object->id));
        }
      }
      catch (Exception $e) {
        // Failed to ingest...  Flag an error.
        $object['state'] = ISLANDORA_BATCH_STATE__ERROR;
        $context['message'] = t('Exception occured: Failed to ingest %pid.', array('%pid' => $ingest_object->id));
      }
    }
    else {
      $context['message'] = t('%pid not ready for ingest.', array('%pid' => $ingest_object->id));
    }

    // Update the info in the database.
    module_load_include('inc', 'islandora_batch', 'includes/db');
    islandora_batch_merge_queued_object_data($object['id'], $object['data']);
    islandora_batch_merge_object_state($object['id'], $object['state']);
    islandora_batch_merge_object_message(
      $object['id'],
      isset($process_results['message']) && $process_results['message'] ?
        $process_results['message'] :
        // Just cleaning the HTML out of the context message.
        strip_tags($context['message'])
    );

    // Pass hook object and a pass/fail state for the object.
    module_invoke_all(ISLANDORA_BATCH_OBJECT_PROCESSED_HOOK, $ingest_object, ($object['state'] === ISLANDORA_BATCH_STATE__DONE ? 1 : 0));

    $end = timer_read(ISLANDORA_BATCH_TIMER_NAME);
    if (!isset($context['results']['count'])) {
      $context['results']['count'] = $context['results']['sum'] = 0;
    }

    $context['results']['count']++;
    $context['results']['sum'] += $end - $start;
    $average_time = islandora_batch_get_average($context);
  }

  if ($average_time < 0 && isset($object) && $object === FALSE) {
    // Didn't get to process any object, because there was nothing to
    // process... Possibly due to running out of total running time.
    return ISLANDORA_BATCH_EMPTY_SET;
  }
  elseif ($object === FALSE) {
    // $object should be "FALSE" if we finished this set of objects.
    return ISLANDORA_BATCH_FINISHED_SET;
  }
  elseif (!islandora_batch_time_check(ISLANDORA_BATCH_LOCK_TIMER, $lock_timeout * 1000 / 3, $lock_timeout)) {
    // We want to cause the batch engine to create the process again,
    // if there is less than a third of the time left on the lock.
    return ISLANDORA_BATCH_LOCK_TIMING;
  }
}

/**
 * Batch 'finished' callback.
 */
function islandora_batch_finished($success, $results, $operations) {
  module_load_include('inc', 'islandora_batch', 'includes/db');
  $successfully_processed_sets = array();
  $set = isset($results['ingest_set']) ? $results['ingest_set'] : NULL;
  $process_count = isset($results['count']) ? $results['count'] : 0;
  if (!$set) {
    $sets = (array) islandora_batch_get_sets(FALSE);
  }
  elseif ($set) {
    $sets = array($set);
  }

  $successfully_processed_sets = array_filter($sets, 'islandora_batch_check_if_set_is_fully_ingested');
  if (!empty($successfully_processed_sets)) {
    db_update('islandora_batch_set')
      ->fields(array(
        'completed' => time(),
      ))
      ->condition('id', $successfully_processed_sets)
      ->execute();
  }

  $lock_name = islandora_batch_get_lock_name($set);
  lock_release($lock_name);
  if (module_exists('views') && variable_get('islandora_batch_link_when_finished', TRUE)) {
    module_load_include('inc', 'islandora_batch', 'includes/utilities');
    if ($set) {
      if (islandora_batch_set_is_owner($set)) {
        drupal_set_message(filter_xss(t('Processing complete; review the !queue_link for some additional information.', array(
          '!queue_link' => l(t('queue'), "islandora_batch/reports/queue/{$set}"),
        ))));
      }
      elseif (user_access(ISLANDORA_BATCH_MANAGE_QUEUE_PERMISSION)) {
        drupal_set_message(filter_xss(t('Processing complete; review the !queue_link for some additional information.', array(
          '!queue_link' => l(t('queue'), "admin/reports/islandora_batch_queue/{$set}"),
        ))));
      }
      else {
        // Shouldn't really be able to get here... Just for completeness, I
        // guess?
        drupal_set_message(filter_xss(t('Processing complete; you may be able to see some additional information in the !queue_link.', array(
          '!queue_link' => l(t('queue'), 'islandora_batch/reports/queue'),
        ))));
      }
    }
    elseif (user_access(ISLANDORA_BATCH_MANAGE_QUEUE_PERMISSION)) {
      drupal_set_message(filter_xss(t('Processing complete; review !queue_link for some additional information.', array(
        '!queue_link' => l(t('queue'), 'admin/reports/islandora_batch_queue'),
      ))));
    }
    else {
      drupal_set_message(filter_xss(t('Processing complete; you may be able to see some additional information in the !queue_link.', array(
        '!queue_link' => l(t('queue'), 'islandora_batch/reports/queue'),
      ))));
    }
  }
  else {
    drupal_set_message(t('Processing complete!'));
  }
  module_invoke_all(ISLANDORA_BATCH_PROCESS_FINISHED_HOOK, $successfully_processed_sets);
}
