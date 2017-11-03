<?php

/**
 * @file
 * Views base handler for collection counting.
 */

class IslandoraSolrViewsHandlerCollectionCount extends views_handler_field {
  /**
   * Inherits.
   *
   * @param object $values
   *   An object containing the values of the query thus far.
   *
   * @return string
   *   The rendered markup string.
   */
  public function render($values) {
    $pid = $values->PID;
    $object = islandora_object_load($pid);

    if (!in_array('islandora:collectionCModel', $object->models)) {
      return NULL;
    }

    $qp = new IslandoraSolrQueryProcessor();
    $qp->buildQuery("*:*");
    $qp->solrParams['fl'] = 'PID';
    $member_of = variable_get('islandora_solr_member_of_field', 'RELS_EXT_isMemberOf_uri_ms');
    $member_of_collection = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');
    $filters = array(
      format_string('!member_of_collection:"!pid" OR !member_of:"!pid"', array(
        '!member_of_collection' => $member_of_collection,
        '!pid' => "info:fedora/$pid",
        '!member_of' => $member_of,
      )),
    );
    $qp->solrParams['fq'] = $filters;
    $qp->solrLimit = 0;
    $qp->executeQuery(FALSE);
    return $qp->islandoraSolrResult['response']['numFound'];
  }
}
