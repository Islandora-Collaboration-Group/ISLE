<?php

/**
 * @file
 * Definition of a BookmarkDefaultSessionList object.
 */

/**
 * Class that defines a BookmarkDefaultSessionList object. A
 * BookmarkDefaultSessionList object only difference is that it cannot be
 * deleted.
 */
class BookmarkDefaultSessionList extends BookmarkSession {
  /**
   * Constructor for the BookmarkDefaultDatabaseList object.
   */
  public function __construct($list_id) {
    parent::__construct($list_id);
    $this->isDeletable = FALSE;
  }

  /**
   * Overwrite to do nothing...
   */
  public function delete() {

  }
}
