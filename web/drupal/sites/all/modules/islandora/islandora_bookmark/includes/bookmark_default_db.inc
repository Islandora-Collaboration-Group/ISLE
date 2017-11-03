<?php

/**
 * @file
 * Definition of a BookmarkDefaultDatabaseList object.
 */

/**
 * Class that defines a BookmarkDefaultDatabaseList object. A
 * BookmarkDefaultDatabaseList object only difference is that it cannot be
 * deleted.
 */
class BookmarkDefaultDatabaseList extends BookmarkDatabase {
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
