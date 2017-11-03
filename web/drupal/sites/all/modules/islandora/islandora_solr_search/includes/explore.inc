<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Generates the links for the Islandora explore block.
 *
 * @return output
 *   A rendered list containing explore links to islandora solr search and
 * object counts.
 */
function islandora_solr_explore_generate_links() {
  $output = '';
  $links = array();
  $expore_config = variable_get('islandora_solr_explore_config', array());
  if (!empty($expore_config)) {
    $qp = new IslandoraSolrQueryProcessor();
    $qp->buildQuery('*:*');
    $qp->solrParams['fl'] = '*';
    $qp->solrParams['facet.limit'] = 100000;
    $qp->solrParams['facet.mincount'] = variable_get('islandora_solr_facet_min_limit', '2');
    $qp->solrParams['facet.field'] = array();
    $qp->solrParams['fl'] = 'PID';
    foreach ($expore_config as $index => $option) {
      $qp->solrParams['facet.query'][] = $option['filter'];
    }
    $qp->executeQuery(FALSE);
    foreach ($expore_config as $index => $option) {
      $url = htmlspecialchars(urlencode($option['filter']));
      $links[] = l(
        htmlentities($option['label']),
        "islandora/search/$url",
        array()
      ) . '&nbsp;<span>(' . htmlentities($qp->islandoraSolrResult['facet_counts']['facet_queries'][$option['filter']]) . ')</span>';
    }
  }

  if (!empty($links)) {
    $output = theme('item_list', array(
    'items' => $links,
    'title' => NULL,
    'type' => 'ul',
    'attributes' => array('class' => 'islandora-solr-explore'),
    ));
  }

  return $output;
}

/**
 * Tests for a valid facet query by seeing if the query returns any data.
 */
function islandora_solr_explore_test_facet_query($query) {
  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery('*:*');
  $qp->solrParams['fq'] = $query;
  $qp->solrParams['fl'] = '*';
  $qp->solrParams['facet.limit'] = 1;
  $qp->solrParams['facet.mincount'] = variable_get('islandora_solr_facet_min_limit', '2');
  $qp->solrParams['facet.field'] = array();
  $qp->solrParams['fl'] = 'PID';
  $qp->executeQuery(FALSE);
}
