<?php

/**
 * @file
 * Utility functions for the OAI tests.
 */

class IslandoraOAIWebTestCase extends IslandoraWebTestCase {

  /**
   * Accepts OAI requests with Xpaths and returns results from those queries.
   *
   * @param string $request
   *   The parameters of the GET request.
   * @param array|string $xpaths
   *   The Xpath query(ies) we'll be making to the XML we GET.
   * @param string $prefix
   *   The prefix Xpath queries use to represent the OAI-PMH 2.0 namespace.
   *
   * @return array
   *   An associative array of queries => results.
   */
  public function getXpathFromOAIRequest($request, $xpaths, $prefix = 'oai') {
    $path = $GLOBALS['base_url'] . '/oai2?' . $request;
    $xml = simplexml_load_string($this->drupalGet($path));
    if (!$xml) {
      $this->fail("Could not find XML at $path.");
      return array();
    }
    $xml->registerXPathNamespace($prefix, 'http://www.openarchives.org/OAI/2.0/');
    $results = array();
    foreach ((array) $xpaths as $xpath) {
      $results[$xpath] = $xml->xpath($xpath);
    }
    return $results;
  }

}
