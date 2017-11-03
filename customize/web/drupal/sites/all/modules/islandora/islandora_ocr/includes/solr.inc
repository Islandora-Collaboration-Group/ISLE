<?php

/**
 * @file
 * Functions for performing/parsing highlighed solr searches with HOCR data.
 */

/**
 * Executes the given query, provides some defaults for highlighting.
 *
 * @param string $query
 *   The SOLR query.
 * @param int $offset
 *   The number of results to return.
 * @param int $limit
 *   The number of results to return.
 * @param array $params
 *   SOLR query parameters.
 *
 * @return object
 *   The Solr results; deserialized JSON.
 */
function islandora_ocr_highlighted_solr_search($query, $offset = 0, $limit = -1, array &$params = array()) {
  if (!module_exists('islandora_solr')) {
    return array();
  }
  $limit = $limit != -1 ? $limit : variable_get('islandora_ocr_solr_hocr_result_count', 32);
  // Remove any fields that have no values specified so the defaults can be
  // applied.
  $params = array_filter($params);

  // Our settings... We add in the original, items at the end.
  $params = array(
    'hl' => 'true',
    'qt' => (variable_get('islandora_solr_request_handler', 'standard') ? variable_get('islandora_solr_request_handler', 'standard') : ''),
    'hl.fl' => variable_get('islandora_ocr_solr_hocr_highlighting_field', 'text_nodes_HOCR_hlt'),
    'hl.useFastVectorHighlighter' => variable_get('islandora_ocr_solr_hocr_highlighting_use_fast', TRUE) ? 'true' : 'false',
    // XXX: Could end up with a large fragment size with fielded
    // searches... Don't really see it as a major issue, though.
    // XXX: We add a bit to the fragsize, so we should get any characters
    // surrounding the fragment--on either side--to be able to account for the
    // fragment Solr selects vs what the first token may start with or the last
    // may end with in the HOCR.
    "hl.fragsize" => max((int) (drupal_strlen($query) * 1.10), 18),
    "hl.snippets" => "8",
  ) + $params;

  if (variable_get('islandora_solr_use_ui_qf', FALSE) || !islandora_solr_check_dismax()) {
    // Put our "qf" in if we are configured to or have none from the
    // request handler.
    $params['qf'] = variable_get('islandora_solr_query_fields', 'dc.title^5 dc.subject^2 dc.description^2 dc.creator^2 dc.contributor^1 dc.type');
  }

  $url = parse_url(variable_get('islandora_solr_url', 'localhost:8080/solr'));
  $solr = new Apache_Solr_Service($url['host'], $url['port'], $url['path'] . '/');
  $solr->setCreateDocuments(0);
  try {
    $results = $solr->search($query, $offset, $limit, $params);
    return json_decode($results->getRawResponse(), TRUE);
  }
  catch (Exception $e) {
    watchdog_exception('islandora_ocr', $e, 'Got an exception while searching.', array(), WATCHDOG_ERROR);
  }
}

/**
 * Maps any highlighted snippets in the solr search results to coordinate data.
 *
 * @param object $solr_results
 *   The Solr results; deserialized JSON.
 * @param bool $ignore_duplicate_snippets_results
 *   If TRUE ignore the exact words that are matched by multiple snippets.
 *
 * @return array
 *   An associative array containing all the relevent bounding information for
 *   highlighted search results:
 */
function islandora_ocr_map_highlighted_solr_results_to_bounding_boxes($solr_results, $ignore_duplicate_snippets_results = TRUE) {
  $results = array();
  $hocr_search_params = array(
    'solr' => $solr_results['responseHeader']['params'],
  );
  foreach ($solr_results['highlighting'] as $pid => $highlighted_fields) {
    $object = islandora_object_load($pid);
    if ($object) {
      if (isset($object['HOCR'])) {
        $hocr = HOCR::fromDatastream($object['HOCR']);
        $highlighted_words = islandora_ocr_map_highlighted_fields_to_words($hocr, $hocr_search_params, $highlighted_fields);
        $snippets = islandora_ocr_map_highlighted_words_to_snippets($highlighted_words, $ignore_duplicate_snippets_results);
        $results[$pid] = array(
          'page' => $hocr->getPageDimensions(),
          'snippets' => $snippets,
        );
      }
      else {
        $results[$pid] = array();
        foreach ($highlighted_fields as $highlighted_field) {

          foreach ($highlighted_field as $snippet) {
            $results[$pid]['snippets'][$snippet] = array();
          }
        }
      }
    }
  }
  return $results;
}

/**
 * Searches for every highlighted word and records what snippets they belong to.
 *
 * @param HOCR $hocr
 *   Used to search the HOCR for the given highlighted fields.
 * @param array $search_params
 *   Parameters to pass onto the HOCR search.
 * @param array $highlighted_fields
 *   The highlighted fields of a solr result document.
 *
 * @return array
 *   An associative array containing all the highlighted words.
 *   - id: The ID of the word.
 *     - id: The ID of the word.
 *     - class: The class of the word.
 *     - bbox: The bounding box of the word.
 *     - snippets: The snippets this word belongs to.
 */
function islandora_ocr_map_highlighted_fields_to_words(HOCR $hocr, array $search_params, array $highlighted_fields) {
  // Search for all the words in the snippet and combined them such that
  // for all words we record all the snippets that they belong to.
  $snippets = islandora_ocr_map_solr_highlighted_fields_to_snippets($highlighted_fields);
  $highlighted_matches = array();
  foreach ($snippets as $snippet) {
    $matches = $hocr->search($snippet, $search_params);
    foreach ($matches as $match) {
      $id = $match['id'];
      if (empty($highlighted_matches[$id])) {
        $highlighted_matches[$id] = $match;
      }
      $highlighted_matches[$id]['snippets'][] = $snippet;
    }
  }
  return $highlighted_matches;
}

/**
 * Maps the given solr document results highlighted fields to snippets.
 *
 * @param array $highlighted_fields
 *   The highlighted fields.
 *
 * @return array
 *   The snippets.
 */
function islandora_ocr_map_solr_highlighted_fields_to_snippets(array $highlighted_fields) {
  $snippets = array();
  foreach ($highlighted_fields as $highlighted_field) {
    foreach ($highlighted_field as $snippet) {
      $snippets[] = $snippet;
    }
  }
  return $snippets;
}

/**
 * Maps the given highlighted words bounding boxes to snippets they belong to.
 *
 * @see islandora_ocr_map_highlighted_fields_to_words()
 *
 * @param array $highlighted_words
 *   The highlighted words.
 *
 * @param bool $ignore_duplicate_snippets_results
 *   If TRUE only the first matching snippet is populated in the results for a
 *   given word.
 *
 * @return array
 *   An associative array where the keys are the solr snippets, and the values
 *   are array's of bounding boxes for each of the highlighted words in the
 *   solr snippet.
 */
function islandora_ocr_map_highlighted_words_to_snippets(array $highlighted_words, $ignore_duplicate_snippets_results) {
  $snippets = array();
  if ($ignore_duplicate_snippets_results) {
    foreach ($highlighted_words as $word) {
      $snippet = $word['snippets'][0];
      $snippets[$snippet][] = $word['bbox'];
    }
  }
  else {
    foreach ($highlighted_words as $word) {
      foreach ($word['snippets'] as $snippet) {
        $snippets[$snippet][] = $word['bbox'];
      }
    }
  }
  return $snippets;
}
