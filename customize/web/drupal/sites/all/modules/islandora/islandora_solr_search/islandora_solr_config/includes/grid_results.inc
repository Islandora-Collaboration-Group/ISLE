<?php

/**
 * @file
 * Class override for grid display
 */

/**
 * Extension of IslandoraSolrResults to create an alternative display type.
 */
class IslandoraSolrResultsGrid extends IslandoraSolrResults {

  /**
   * Renders the Solr results as a responsive grid view.
   *
   * Markup and styling is based on the Islandora collection grid view. Some
   * styling is inherited from it.
   *
   * @see IslandoraSolrResults::displayResults()
   *
   * @param array $solr_results
   *   The processed Solr results from
   *   IslandoraSolrQueryProcessor::islandoraSolrResult.
   *
   * @return string
   *   Rendered Solr results.
   */
  public function printResults($solr_results) {
    $mod_path = drupal_get_path('module', 'islandora_solr_config');
    drupal_add_css("$mod_path/css/islandora_solr_config.theme.css");
    $object_results = $solr_results['response']['objects'];

    $elements = array();
    $elements['solr_total'] = $solr_results['response']['numFound'];
    $elements['solr_start'] = $solr_results['response']['start'];

    return theme('islandora_solr_grid', array(
      'results' => $object_results,
      'elements' => $elements,
    ));
  }
}
