<?php

/**
 * @file
 * Houses helper functions used in the islandora_bookmark module
 */

/**
 * Retrieves lists that the specified user has access to.
 *
 * @global stdClass $user
 *
 * @param string|NULL $type
 *   Optional type of bookmark to retrieve. Defaults to all bookmarks.
 *
 * @return array
 *   An array of PidList objects.
 */
function islandora_bookmark_get_user_owned_bookmarks($type = NULL) {
  global $user;
  $list_query = db_select('islandora_bookmark_list_names', 'i')
    ->fields('i', array('listid'))
    ->condition('listowner', $user->uid)
    ->orderBy('listid')
    ->distinct();
  if ($type) {
    $list_query = $list_query->condition('type', $type);
  }
  $listids = (array) $list_query->execute()
    ->fetchAll(PDO::FETCH_COLUMN);

  $bookmark_list = array();
  foreach ($listids as $list_id) {
    $templist = BookmarkDatabase::getList($list_id);

    $bookmark_list[] = $templist;
  }

  // User is anon, use session as well.
  if ($user->uid == 0) {
    if (!isset($_SESSION) || empty($_SESSION['islandora_bookmark']) && variable_get('islandora_bookmark_create_user_default_lists', FALSE)) {
      BookmarkDefaultSessionList::createNewList(t('My Default List'), 'bookmark_default', $user);
    }

    if (!empty($_SESSION['islandora_bookmark'])) {
      foreach (array_keys($_SESSION['islandora_bookmark']) as $id) {
        $bookmark_list[] = BookmarkSession::getList($id);
      }
    }
  }
  return $bookmark_list;
}

/**
 * Retrieves all Bookmarks the current user is shared on.
 *
 * @global stdClass $user
 * @return array
 *   An array of Bookmarks.
 */
function islandora_bookmark_get_user_shared_bookmarks() {
  global $user;

  $owner_ids = array();
  $shared_ids = array();
  $bookmark_list = array();

  $owner_query = db_query('SELECT listid FROM {islandora_bookmark_list_names} WHERE listowner = :uid ORDER BY listid', array(':uid' => $user->uid));

  foreach ($owner_query as $record) {
    $owner_ids[] = $record->listid;
  }
  $shared_query = db_query('SELECT listid FROM {islandora_bookmark_list_users} WHERE uid = :uid OR uid = 0 ORDER BY listid', array(':uid' => $user->uid));

  foreach ($shared_query as $record) {
    $shared_ids[] = $record->listid;
  }
  $shared_ids = array_diff($shared_ids, $owner_ids);

  foreach ($shared_ids as $id) {
    $templist = Bookmark::getList($id);
    $bookmark_list[] = $templist;
  }
  return array_filter($bookmark_list);
}

/**
 * Retrieves a Bookmark given a specified ID.
 *
 * @global stdClass $user
 *
 * @param int $listid
 *   List ID to be searched for.
 *
 * @return bool|Bookmark
 *   PidList object or FALSE if the list does not exist.
 */
function islandora_bookmark_get_bookmark_by_number($listid) {
  $list = Bookmark::getList($listid);
  if ($list === NULL) {
    return FALSE;
  }
  return $list;
}

/**
 * Retrieves all Bookmarks visible to the current user.
 *
 * @param string $pid
 *   The object's PID.
 * @param int $uid
 *   Drupal user id.
 *
 * @return array
 *   An array containing listids.
 */
function islandora_bookmark_get_bookmarks_visible_to_user($pid, $uid) {
  $lists = array();
  // SESSION FIRST.
  if ($uid == 0) {
    if (!isset($_SESSION) || empty($_SESSION['islandora_bookmark']) && variable_get('islandora_bookmark_create_user_default_lists', FALSE)) {
      BookmarkDefaultSessionList::createNewList(t('My Default List'), 'bookmark_default', NULL);
    }
    if (isset($_SESSION['islandora_bookmark'])) {
      foreach ($_SESSION['islandora_bookmark'] as $value) {
        $templist = $value;
        if (in_array($pid, $templist->getPids())) {
          $lists[$templist->getId()] = $templist->getId();
        }
      }
    }
  }
  else {
    // DB.
    $q = <<<EOQ
SELECT n.listid
FROM {islandora_bookmark_list_pids} as p INNER JOIN
  {islandora_bookmark_list_names} as n ON p.listid = n.listid LEFT JOIN
  {islandora_bookmark_list_users} as u ON u.listid = p.listid
WHERE p.pidid = :pid AND :uid IN (u.uid, n.listowner)
EOQ;
    $list_result = db_query($q, array(':uid' => $uid, ':pid' => $pid))
      ->fetchAll(PDO::FETCH_COLUMN);
    $lists += drupal_map_assoc($list_result);
  }
  return $lists;
}

/**
 * Retrieves all the pids from the bookmark list id.
 *
 * @param string $value
 *   The listid.
 *
 * @return array
 *   An array containing listpids.
 */
function islandora_bookmark_list_pids_query($value) {
  $list = Bookmark::getList($value);
  return $list->getPids();
}

/**
 * Retrieves all the uids from the bookmark listid, including owner.
 *
 * @param string $value
 *   The listid.
 *
 * @return array
 *   An array containing user ids.
 */
function islandora_bookmark_list_users_query($value) {
  $list = Bookmark::getList($value);
  $users = $list->getUsers();
  $users[] = $list->bookmarkOwner;
  return $users;
}

/**
 * Determines whether the user has a BookmarkDefaultDatabaseList or not.
 *
 * @param object $user
 *   Optional user object if we are checking for a specific user.
 *
 * @return int|bool
 *   The ID of the list if they do have one, FALSE otherwise.
 */
function islandora_bookmark_get_default_list($user = NULL) {
  if ($user === NULL) {
    global $user;
  }
  elseif (!is_object($user) || !isset($user->uid)) {
    return FALSE;
  }
  $result = db_query('SELECT listid FROM {islandora_bookmark_list_names} WHERE listowner = :owner AND type = :type',
    array(
      ':owner' => $user->uid,
      ':type' => 'bookmark_default',
    )
  );
  $total_results = $result->fetchField();
  if (empty($total_results)) {
    return FALSE;
  }
  else {
    return $total_results;
  }
}
