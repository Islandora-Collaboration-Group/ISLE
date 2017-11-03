<?php

/**
 * @file
 * Views base handler for sort.
 */

// @codingStandardsIgnoreStart
// This Whole file is ignored due to classes and methods are not camelcase and it
// being called all over the place. TODO bring up to coding standards
/**
 * Class for sorting for a field.
 */
class islandora_solr_views_handler_sort extends views_handler_sort {

  /**
   * Places the sort into the search parameters.
   */
  public function query() {
    $order = drupal_strtolower($this->options['order']);
    $this->query->add_sort($this->real_field, $order);
  }
}
// @codingStandardsIgnoreEnd