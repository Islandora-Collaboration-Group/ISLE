<?php
/**
 * @file
 * Database interaction helpers.
 */

/**
 * Gets the database id related to a PID.
 *
 * Sets one if it doesn't exist.
 */
function islandora_usage_stats_get_pid_id($pid) {
  $id = islandora_usage_stats_pid_id($pid);
  // If no matches, create one.
  if (!$id) {
    return islandora_usage_stats_pid_to_db($pid);
  }
  return $id;
}

/**
 * Gets the database id related to a PID.
 */
function islandora_usage_stats_pid_id($pid) {
  $result = db_select('islandora_usage_stats_objects')
    ->fields('islandora_usage_stats_objects', array('id'))
    ->condition('pid', $pid)
    ->execute();
  $record = $result->fetchAssoc();
  return isset($record['id']) ? $record['id'] : FALSE;
}

/**
 * Gets the database id related to a PID.
 *
 * @throws Exception if the $pid cannot be loaded.
 */
function islandora_usage_stats_pid_to_db($pid) {
  $object = islandora_object_load($pid);
  if ($object) {
    return db_insert('islandora_usage_stats_objects')
      ->fields(array('pid' => $pid, 'label' => $object->label))
      ->execute();
  }
  else {
    throw new Exception("Could not load '{$pid}' as Islandora object");
  }
}

/**
 * Get the ds_dsid for a given pid/dsid combo.
 *
 * Creates it if it doesn't exist.
 */
function islandora_usage_stats_get_ds_dsid($pid, $dsid) {
  $pid_id = islandora_usage_stats_get_pid_id($pid);
  $result = db_select('islandora_usage_stats_datastreams')
    ->fields('islandora_usage_stats_datastreams', array('id'))
    ->condition('pid_id', $pid_id)
    ->condition('dsid', $dsid)
    ->execute();
  $record = $result->fetchAssoc();
  $ds_dsid = isset($record['id']) ? $record['id'] : FALSE;
  if (!$ds_dsid) {
    // This record does not exist yet, we must add it.
    $object = islandora_object_load($pid);
    $ds_dsid = db_insert('islandora_usage_stats_datastreams')
      ->fields(
        array(
          'pid_id' => $pid_id,
          'dsid' => $dsid,
          'ds_label' => $object[$dsid]->label,
        )
      )
      ->execute();
  }

  return $ds_dsid;
}

/**
 * Get a number of the most recently viewed objects.
 *
 * @param int $count
 *   The number of objects to get.
 * @param array $exclude
 *   A list of PIDs to exclude.
 *
 * @return array
 *   A list of results containing an object with attributes of pid and views.
 */
function islandora_usage_stats_get_most_viewed($count, $exclude = NULL) {
  $query = db_select('islandora_usage_stats_object_access_log', 'log');
  $query->join('islandora_usage_stats_objects', 'objects', 'log.pid_id = objects.id');
  $query->addExpression('COUNT(log.pid_id)', 'views');
  $query->fields('objects', array('pid'))
    ->groupBy('log.pid_id')
    ->orderBy('views', 'DESC')
    ->range(0, $count);
  // XXX: This could potentially cause slow down on large data sets.
  if ($exclude) {
    $query->condition('pid', $exclude, 'NOT IN');
  }
  $results = $query->execute();
  $records = $results->fetchAllAssoc('pid');
  return $records;
}

/**
 * Get a number of the most viewed objects.
 *
 * @param int $count
 *   The number of objects to get.
 * @param array $exclude
 *   A list of PIDs to exclude.
 *
 * @return array
 *   A list of PIDs.
 */
function islandora_usage_stats_get_recently_viewed($count, $exclude = NULL) {
  $query = db_select('islandora_usage_stats_object_access_log', 'log');
  $query->join('islandora_usage_stats_objects', 'objects', 'log.pid_id = objects.id');
  $query->fields('objects', array('pid'))
    ->orderBy('log.time', 'DESC')
    ->range(0, $count);
  // XXX: This could potentially cause slow down on large data sets.
  if ($exclude) {
    $query->condition('pid', $exclude, 'NOT IN');
  }
  $results = $query->execute();
  $records = $results->fetchAllAssoc('pid');
  return array_keys($records);
}

/**
 * Update an object's label if necessary.
 *
 * @param string $pid
 *   The PID of the object.
 * @param string $label
 *   The label to set.
 */
function islandora_usage_stats_update_object_label($pid, $label) {
  db_update('islandora_usage_stats_objects')
    ->fields(array('label' => $label))
    ->condition('pid', $pid, '=')
    ->condition('label', $label, '<>')
    ->execute();
}

/**
 * Select top downloads, ordered by most downloaded.
 *
 * @param int $count
 *   The number of max results to return.
 *
 * @return array
 *   A list of PIDS.
 */
function islandora_usage_stats_get_top_downloads($count) {
  $query = db_select('islandora_usage_stats_object_ds_access_log', 'log');
  $query->join('islandora_usage_stats_datastreams', 'datastreams', 'log.ds_id = datastreams.id');
  $query->join('islandora_usage_stats_objects', 'objects', 'datastreams.pid_id = objects.id');
  $results = $query->fields('objects', array('pid'))
    ->orderBy('log.time', 'DESC')
    ->range(0, $count)
    ->execute();
  $records = $results->fetchAllAssoc('pid');
  return array_keys($records);
}

/**
 * Get similar search terms for a given term.
 *
 * @param string $term
 *   The Current search term.
 *
 * @return array
 *   The search term as an associated array.
 */
function islandora_usage_stats_get_search_term($term) {
  $result = db_select('islandora_usage_stats_top_searches', 'ts')
    ->fields('ts')
    ->condition('term', db_like($term) . '%', 'LIKE')
    ->execute()
    ->fetchAssoc();
  return $result;
}

/**
 * Increase search term usage by row id.
 *
 * @param int $id
 *   The row id in db to increment.
 *
 * @return array
 *   Increment results array.
 */
function islandora_usage_stats_increment_search_term($id) {
  $result = db_update('islandora_usage_stats_top_searches')
    ->expression('search_total', 'search_total + :search_total', array(
      ':search_total' => 1,
      )
    )
    ->condition('id', $id)
    ->execute();
  return $result;
}

/**
 * Set a new search term to track.
 *
 * @param string $term
 *   The search term to track.
 *
 * @return array
 *   Results of the insertion.
 */
function islandora_usage_stats_set_search_term($term) {
  $result = db_insert('islandora_usage_stats_top_searches')->fields(
    array(
      'term' => $term,
      'search_total' => 1,
    )
  )->execute();
  return $result;
}

/**
 * Get top searches used on this site.
 *
 * @param int $count
 *   Max number of results to return
 *
 * @return array
 *   Results array of top search terms.
 */
function islandora_usage_stats_get_top_searched($count) {
  $query = db_select('islandora_usage_stats_top_searches', 'ts');
  $results = $query->orderBy('search_total', 'DESC')
    ->fields('ts', array('term', 'search_total'))
    ->range(0, $count)
    ->execute();
  $records = $results->fetchAll();
  return $records;
}

/**
 * Clear information from tables.
 *
 * @param string $table
 *   The base table to remove information from info from other tables may be
 *   removed as well.
 * @param string $pid
 *   The PID to remove information about.
 */
function islandora_usage_stats_clear_tracking($table, $pid = NULL) {
  $results = array();
  if ($pid == NULL) {
    $results[] = db_truncate($table)->execute();
    // If the object_access_log we nuke collection and content model as well.
    if ($table == 'islandora_usage_stats_object_access_log') {
      db_truncate('islandora_usage_stats_collection_access_log')->execute();
      db_truncate('islandora_usage_stats_content_model_access_log')->execute();
    }
  }
  else {
    if ($table == 'islandora_usage_stats_objects') {
      $results[] = db_delete($table)
        ->condition('pid', $pid)
        ->execute();
    }
    else {
      $pid_id = islandora_usage_stats_pid_id($pid);
      // If the object_access_log we nuke collection and content model as well.
      if ($table == 'islandora_usage_stats_object_access_log') {
        $in_query = db_select('islandora_usage_stats_object_access_log')
          ->fields('islandora_usage_stats_object_access_log', array('id'))
          ->condition('pid_id', $pid_id);
        $results[] = db_delete('islandora_usage_stats_collection_access_log')
          ->condition('object_access_id', $in_query, 'IN')
          ->execute();
        $results[] = db_delete('islandora_usage_stats_content_model_access_log')
          ->condition('object_access_id', $in_query, 'IN')
          ->execute();
        $pid_id_field = 'pid_id';
      }
      elseif ($table == 'islandora_usage_stats_collection_access_log') {
        $pid_id_field = 'collection';
      }
      elseif ($table == 'islandora_usage_stats_content_model_access_log') {
        $pid_id_field = 'content_model';
      }
      if ($table == 'islandora_usage_stats_object_ds_access_log') {
        $in_query = db_select('islandora_usage_stats_datastreams')
          ->fields('islandora_usage_stats_datastreams', array('id'))
          ->condition('pid_id', $pid_id);
        $results[] = db_delete('islandora_usage_stats_object_ds_access_log')
          ->condition('ds_id', $in_query, 'IN')
          ->execute();
      }
      if (isset($pid_id_field)) {
        $results[] = db_delete($table)
          ->condition($pid_id_field, $pid_id)
          ->execute();
      }
    }
  }
  return $results;
}

/**
 * Write an object view to the DB.
 *
 * @global $user
 *
 * @param int $pid_id
 *   DB ID for a PID.
 * @param int $request_time
 *   Time to log, defaults to REQUEST_TIME.
 * @param string $ip_address
 *   IP to log to. Defaults to whatever ip_address() returns.
 * @param int $uid
 *   User ID to log to, defaults to global user's.
 *
 * @return int
 *   The DB ID for the log entry.
 */
function islandora_usage_stats_object_view_to_db($pid_id, $request_time = NULL, $ip_address = NULL, $uid = NULL) {
  global $user;
  return db_insert('islandora_usage_stats_object_access_log')->fields(array(
    'pid_id' => $pid_id,
    'time' => ($request_time !== NULL) ? $request_time : REQUEST_TIME,
    'ip' => ($ip_address !== NULL) ? $ip_address : ip_address(),
    'uid' => ($uid !== NULL) ? $uid : $user->uid,
  ))->execute();
}

/**
 * Write a content model view to the DB.
 */
function islandora_usage_stats_content_model_view_to_db($object_access_id, $model_id) {
  db_insert('islandora_usage_stats_content_model_access_log')->fields(array(
    'object_access_id' => $object_access_id,
    'content_model' => $model_id,
  ))->execute();
}

/**
 * Write a collection view to the DB.
 */
function islandora_usage_stats_collection_view_to_db($object_access_id, $collection_id) {
  db_insert('islandora_usage_stats_collection_access_log')->fields(array(
    'object_access_id' => $object_access_id,
    'collection' => $collection_id,
  ))->execute();
}

/**
 * Write a datastream download to the DB.
 *
 * @global $user
 *
 * @param int $ds_id
 *   DB ID for a datastream.
 * @param int $request_time
 *   Time to log, defaults to REQUEST_TIME.
 * @param string $ip_address
 *   IP to log to. Defaults to whatever ip_address() returns.
 * @param int $uid
 *   User ID to log to, defaults to global user's.
 *
 * @return int
 *   The DB ID for the log entry.
 */
function islandora_usage_stats_datastream_download_to_db($ds_id, $request_time = NULL, $ip_address = NULL, $uid = NULL) {
  global $user;
  db_insert('islandora_usage_stats_object_ds_access_log')->fields(array(
    'ds_id' => $ds_id,
    'time' => ($request_time !== NULL) ? $request_time : REQUEST_TIME,
    'ip' => ($ip_address !== NULL) ? $ip_address : ip_address(),
    'uid' => ($uid !== NULL) ? $uid : $user->uid,
  ))->execute();
}

/**
 * Helper function to retrieve the amount of views for an object.
 *
 * @param string $pid
 *   The PID of the object to get view count stats for.
 *
 * @return string
 *   The number of views the object has had.
 */
function islandora_usage_stats_get_individual_object_view_count($pid) {
  $query = db_select('islandora_usage_stats_objects', 'objs');
  $query->condition('pid', $pid, '=');
  $query->join('islandora_usage_stats_object_access_log', 'logs', 'logs.pid_id = objs.id');
  return $query->countQuery()
    ->execute()
    ->fetchField();
}

/**
 * Helper function to retrieve the amount of downloads of datastreams.
 *
 * @param string $pid
 *   The PID of the object to get datastream download stats for.
 * @param NULL|string $dsid
 *   The string of the DSID to get stats for, NULL to return all stats.
 *
 * @return array
 *   An array keyed by DSID and the number of views it has had.
 */
function islandora_usage_stats_get_datastream_downloads_count($pid, $dsid = NULL) {
  $query = db_select('islandora_usage_stats_object_ds_access_log', 'ds_log');
  $query->join('islandora_usage_stats_datastreams', 'dses', 'dses.id = ds_log.ds_id');
  $query->join('islandora_usage_stats_objects', 'objs', 'objs.id = dses.pid_id');
  $query->addExpression('COUNT(dses.dsid)');
  if ($dsid) {
    $query->condition('dses.dsid', $dsid);
  }
  return $query->condition('objs.pid', $pid)
    ->fields('dses', array('dsid'))
    ->groupBy('dsid')
    ->execute()
    ->fetchAllKeyed();
}
