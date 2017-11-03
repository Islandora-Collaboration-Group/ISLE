<?php

/**
 * @file
 * Definition of a Bookmark object.
 */

/**
 * Class that defines a Bookmark object. A Bookmark object needs only to be
 * constructed such that it contains a listname and listid for uniqueness.
 */
class BookmarkDatabase extends Bookmark {
  /**
   * Mapping for use in $this->getMapped().
   *
   * @var array
   */
  protected $magicMap = array(
    'bookmarkName' => 'listname',
    'bookmarkOwner' => 'listowner',
    'type' => 'type',
    'bookmarkDescription' => 'descrip',
  );

  /**
   * Constructor for the BookmarkDatabase object.
   */
  public function __construct($list_id) {
    parent::__construct($list_id, NULL, NULL);
    $this->isPersistent = TRUE;

    foreach (array_keys($this->magicMap) as $for_magic) {
      unset($this->$for_magic);
    }
  }

  /**
   * Helper function to deal with magic property.
   *
   * The properties as stored in the database are different. Let's map 'em
   * here.
   *
   * @param string $property
   *   A property on this object which we want to map to a column in the
   *   "islandora_bookmark_list_names" table.
   *
   * @return string
   *   A string representing the column name.
   *
   * @throws Exception
   *   If we do not have a mapping for the given exception, throw an exception.
   */
  protected function getMapped($property) {
    if (isset($this->magicMap[$property])) {
      return $this->magicMap[$property];
    }
    else {
      trigger_error(format_string('Unknown property "@property".', array(
        '@property' => $property,
      )));
      return FALSE;
    }
  }

  /**
   * Magic property... Map to our column name, and set it.
   */
  public function __set($property, $value) {
    $mapped = $this->getMapped($property);
    if ($mapped) {
      db_update('islandora_bookmark_list_names')
        ->fields(array(
          $mapped => $value,
        ))
        ->condition('listid', $this->bookmarkId)
        ->execute();
      return $value;
    }
  }

  /**
   * Magic property... Map to our column name, and get it.
   */
  public function __get($property) {
    $mapped = $this->getMapped($property);
    if ($mapped) {
      return db_select('islandora_bookmark_list_names', 'n')
        ->fields('n', array($this->getMapped($property)))
        ->condition('listid', $this->bookmarkId)
        ->execute()
        ->fetchField();
    }
  }

  /**
   * Refer to parent docs.
   *
   * Branch to different classes based on type. Return NULL if a mapping for
   * the given type is not available.
   */
  public static function getList($id, $force_session_attempt = FALSE) {
    if ($force_session_attempt) {
      // Call the parent, which should call this with FALSE if not a session...
      return parent::getList($id, $force_session_attempt);
    }

    $types = module_invoke_all('islandora_bookmark_database_types');
    $type = db_select('islandora_bookmark_list_names', 'n')
      ->fields('n', array('type'))
      ->condition('listid', $id)
      ->execute()
      ->fetchField();

    if (isset($types[$type])) {
      $class = $types[$type];
      return new $class($id);
    }
    else {
      return NULL;
    }
  }

  /**
   * Convenience; get the base query for PIDs.
   *
   * @return SelectQuery
   *   A select query for the PIDs table.
   */
  protected function getPidQuery() {
    return db_select('islandora_bookmark_list_pids', 'p')
      ->fields('p', array('pidid'))
      ->condition('listid', $this->bookmarkId)
      ->distinct();
  }

  /**
   * Inherits.
   */
  public function getPids($offset = 0, $limit = -1) {
    $s = $this->getPidQuery();

    if ($limit > 0) {
      $s->range($offset, $limit);
    }

    return $s->execute()->fetchAll(PDO::FETCH_COLUMN);
  }

  /**
   * Inherits.
   */
  public function getPidCount() {
    $s = $this->getPidQuery()->countQuery();

    return intval($s->execute()->fetchField());
  }

  /**
   * Build a new list.
   *
   * @param string $name
   *   The name for the list.
   * @param string $type
   *   A string indicating the type of list. Default is 'bookmark'.
   * @param object $owner
   *   A user object from which we can grab the uid. If not provided, we'll
   *   grab the global "$user" variable.
   * @param string $desc
   *   An optional description of the bookmark list.
   *
   * @return BookmarkDatabase
   *   A new Bookmark object.
   */
  public static function createNewList($name, $type = 'bookmark', $owner = NULL, $desc = '') {
    if ($owner === NULL) {
      global $user;
      $owner = $user;
    }
    drupal_alter(ISLANDORA_BOOKMARK_DATABASE_LIST_CREATION_HOOK, $name, $type, $owner, $desc);

    $fields = array(
      'listname' => $name,
      'listowner' => $owner->uid,
      'type' => $type,
      'descrip' => $desc,
    );
    $list_id = db_insert('islandora_bookmark_list_names')
      ->fields($fields)
      ->execute();
    return static::getList($list_id, FALSE);
  }

  /**
   * Inherits.
   */
  public function delete() {
    if ($this->managementAccess()) {
      $this->removeUsers($this->getUsers());
      $this->removePids($this->getPids());
      db_delete('islandora_bookmark_list_names')
        ->condition('listid', $this->bookmarkId)
        ->execute();

      // XXX: Only to deal with old lists, in which the owner was included in
      // "users" table.
      $this->removeUser($this->bookmarkOwner);
    }
    else {
      throw new Exception(t("You do not have access to remove the list @bookmarkname", array("@bookmarkname" => $this->bookmarkName)));
    }
  }

  /**
   * Inherits.
   */
  public function removeUser($user_delete) {
    db_delete('islandora_bookmark_list_users')
      ->condition('uid', $user_delete)
      ->condition('listid', $this->bookmarkId)
      ->execute();
  }

  /**
   * Inherits.
   */
  public function addUser($user_add) {
    $fields = array(
      'uid' => $user_add,
      'listid' => $this->bookmarkId,
    );
    db_insert('islandora_bookmark_list_users')->fields($fields)->execute();
  }

  /**
   * Inherits.
   */
  public function removePid($pid) {
    db_delete('islandora_bookmark_list_pids')
      ->condition('pidid', $pid)
      ->condition('listid', $this->bookmarkId)
      ->execute();
  }

  /**
   * Inherits.
   */
  public function addPid($pid) {
    $fields = array(
      'listid' => $this->bookmarkId,
      'pidid' => $pid,
    );
    db_insert('islandora_bookmark_list_pids')->fields($fields)->execute();
  }

  /**
   * Inherits.
   */
  public function getUsers() {
    return db_select('islandora_bookmark_list_users', 'u')
      ->fields('u', array('uid'))
      ->condition('listid', $this->bookmarkId)
      ->condition('uid', $this->bookmarkOwner, '!=')
      ->execute()
      ->fetchAll(PDO::FETCH_COLUMN);
  }
}
