<?php

/**
 * @file
 * Views handler for a dismax query filter
 */

// @codingStandardsIgnoreStart
// This Whole file is ignored due to classes and methods are not camelcase and it
// being called all over the place. TODO bring up to coding standards
/**
 * Query Parser filter field.
 */
class islandora_solr_views_handler_filter_query_dismax extends islandora_solr_views_handler_filter_query_lucene {

  // @TODO add requestHandler option (dropdown).
  function query() {
    parent::query();
    $this->query->add_parameter('defType', 'dismax');
  }
}
// @codingStandardsIgnoreEnd
