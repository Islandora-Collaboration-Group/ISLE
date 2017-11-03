<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Determine if the given item exists in the queue, independent of state.
 *
 * @param string $id
 *   A string representing the ID of an item in the queue.
 *
 * @return bool
 *   TRUE if there's an entry for the given ID in the queue; otherwise, FALSE.
 */
function islandora_batch_item_exists($id) {
  $result = db_select('islandora_batch_queue', 'q')
    ->fields('q', array())
    ->condition('id', $id)
    ->countQuery()
    ->execute()
    ->fetchField();
  return $result > 0;
}

/**
 * Determine if an item was created by the given user.
 *
 * @param string $id
 *   A string representing the ID of an item in the queue.
 * @param object $user
 *   An object containing a loaded Drupal user. Defaults to the global $user,
 *   if not provided.
 *
 * @return bool
 *   TRUE if the item is in a set owned by the given user; otherwise, FALSE.
 */
function islandora_batch_item_is_owner($id, $user = NULL) {
  if ($user === NULL) {
    global $user;
  }

  $q = db_select('islandora_batch_queue', 'q');
  $q->join('islandora_batch_set', 's', 's.id = q.sid');

  $result = $q->fields('q', 'id')
    ->condition('q.id', $id)
    ->condition('s.uid', $user->uid)
    ->countQuery()
    ->execute()
    ->fetchField();
  return $result > 0;
}

/**
 * Determine if a set with the given ID exists.
 *
 * @param int $id
 *   An integer identifying a batch set.
 *
 * @return bool
 *   TRUE if the batch exists; otherwise, FALSE.
 */
function islandora_batch_set_exists($id) {
  $result = db_select('islandora_batch_set', 's')
    ->fields('s', array())
    ->condition('id', $id)
    ->countQuery()
    ->execute()
    ->fetchField();

  return $result > 0;
}

/**
 * Determine if the set exists and belongs to the given user.
 *
 * @param int $id
 *   An integer identifying a batch set.
 * @param object $user
 *   An object containing a loaded Drupal user. Defaults to the global $user,
 *   if not provided.
 *
 * @return bool
 *   TRUE if the batch exists was created by the given user; otherwise, FALSE.
 */
function islandora_batch_set_is_owner($id, $user = NULL) {
  if ($user === NULL) {
    global $user;
  }

  $result = db_select('islandora_batch_set', 's')
    ->fields('s', array())
    ->condition('id', $id)
    ->condition('uid', $user->uid)
    ->countQuery()
    ->execute()
    ->fetchField();

  return $result > 0;
}

/**
 * Gets the batch ID for an item based upon PID.
 *
 * @param string $pid
 *   The PID of item in the queue.
 *
 * @return int
 *   The batch ID associated to the item in the queue.
 */
function islandora_batch_get_bid($pid) {
  $bid = db_select('islandora_batch_queue', 'q')
    ->fields('q', array('bid'))
    ->condition('id', $pid)
    ->execute()
    ->fetchField();
  return intval($bid);
}

/**
 * Cleans up and flags files that have been processed as ready to delete.
 *
 * @param string $pid
 *   The PID of the item to be cleaned up.
 */
function islandora_batch_remove_file_usages($pid) {
  $bid = islandora_batch_get_bid($pid);
  $q = db_select('file_usage', 'u')
    ->fields('u', array('fid'))
    ->condition('id', $bid)
    ->condition('module', 'islandora_batch')
    ->condition('type', 'islandora_batch_object')
    ->execute();
  $files = file_load_multiple($q->fetchCol());
  foreach ($files as $file) {
    file_usage_delete($file, 'islandora_batch', 'islandora_batch_object', $bid);
    file_delete($file);
  }
}

/**
 * Resets a Drupal timer.
 *
 * @param string $name
 *   Name of the timer to reset.
 */
function islandora_batch_timer_reset($name) {
  global $timers;
  if (isset($timers[$name])) {
    unset($timers[$name]);
    $timers[$name]['time'] = 0;
  }
}

/**
 * Helper to test if a given encoding will work.
 *
 * @param string $type
 *   The "type" of target, we only care about "zip".
 * @param string $encoding
 *   The name of an encoding to test.
 *
 * @return NULL|string
 *   NULL if we have not found an issue; otherwise, a string trying to describe
 *   the issue.
 */
function islandora_batch_scan_check_encoding($type, $encoding) {
  if ($type == 'zip' && $encoding) {
    // Test parallel to drupal_convert_to_utf8().
    if (function_exists('iconv')) {
      $error_probe = function ($no = NULL, $str = NULL, $file = NULL, $line = NULL) {
        static $found = FALSE;
        $found = $found || $no !== NULL;
        return $found;
      };
      set_error_handler($error_probe);
      iconv_strlen('test', $encoding);
      restore_error_handler();
      if ($error_probe()) {
        // XXX: "Should be supported", since PHP and the "iconv" command may be
        // compiled against different libraries; however, this should be rare...
        // hopefully?
        return t('An invalid encoding has been specified for iconv; check "iconv -l" for a listing of those which should be supported.');
      }
    }
    elseif (function_exists('mb_convert_encoding')) {
      if (!(@mb_encoding_aliases($encoding) || in_array($encoding, mb_list_encodings()))) {
        return t('An invalid encoding has been specified for mb_convert_encoding().');
      }
    }
    elseif (function_exists('recode_string')) {
      $error_probe = function ($no = NULL, $str = NULL, $file = NULL, $line = NULL) {
        static $found = FALSE;
        $found = $found || $no !== NULL;
        return $found;
      };
      set_error_handler($error_probe);
      recode_string("$encoding..utf8", 'test');
      restore_error_handler();
      if ($error_probe()) {
        return t('An invalid encoding has been specified for recode_string().');
      }
    }
  }
}

/**
 * Helper to test scan target/target validity.
 *
 * @param string $type
 *   The "type" of target, either "zip" or "directory".
 * @param string $path
 *   The path to the target.
 *
 * @return array
 *   An array of arrays containing an error string and output label for
 *   use in drush_set_error(). Empty if we encounter no errors.
 */
function islandora_batch_scan_check_target($type, $path) {
  $errors = array();
  if ($type == 'zip') {
    if (DRUSH_VERSION >= 7) {
      if (!is_readable($path)) {
        $errors[] = array(
          'error' => 'SCAN TARGET DOES NOT EXIST',
          'label' => dt('"!target" does not exist as an absolute path or is not readable. ', array('!target' => $path)),
        );
      }
      else {
        if (!is_file($path)) {
          $errors[] = array(
            'error' => 'SCAN TARGET IS NOT A FILE',
            'label' => dt('"!target" is not a file. ', array('!target' => $path)),
          );
        }
      }
    }
    else {
      if (!is_readable($path)) {
        $errors[] = array(
          'error' => 'TARGET DOES NOT EXIST',
          'label' => dt('"!target" does not exist as an absolute path or is not readable. ', array('!target' => $path)),
        );
      }
      else {
        if (!is_file($path)) {
          $errors[] = array(
            'error' => 'TARGET IS NOT A FILE',
            'label' => dt('"!target" is not a file. ', array('!target' => $path)),
          );
        }
      }
    }
  }
  elseif ($type == 'directory') {
    if (DRUSH_VERSION >= 7) {
      if (!is_readable($path)) {
        $errors[] = array(
          'error' => 'SCAN TARGET DOES NOT EXIST',
          'label' => dt('"!target" does not exist as an absolute path or is not readable. ', array('!target' => $path)),
        );
      }
      else {
        if (!is_dir($path)) {
          $errors[] = array(
            'error' => 'SCAN TARGET IS NOT A DIRECTORY',
            'label' => dt('"!target" is not a directory. ', array('!target' => $path)),
          );
        }
      }
    }
    else {
      if (!is_readable($path)) {
        $errors[] = array(
          'error' => 'TARGET DOES NOT EXIST',
          'label' => dt('"!target" does not exist as an absolute path or is not readable. ', array('!target' => $path)),
        );
      }
      else {
        if (!is_dir($path)) {
          $errors[] = array(
            'error' => 'TARGET IS NOT A DIRECTORY',
            'label' => dt('"!target" is not a directory. ', array('!target' => $path)),
          );
        }
      }
    }
  }
  else {
    $errors[] = array(
      'error' => 'INVALID TYPE OPTION',
      'label' => dt('--type must be one of "directory" or "zip". '),
    );
  }
  return $errors;
}
