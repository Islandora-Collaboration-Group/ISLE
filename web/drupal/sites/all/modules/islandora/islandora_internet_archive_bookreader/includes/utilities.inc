<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Remap the more general structure of the from islandora_paged_content.
 *
 * IA Bookreader requires a different structure than that which
 * islandora_paged_content provides.
 */
function islandora_internet_archive_bookreader_remap_highlighting_for_bookreader_results($original_results) {
  $results = array(
    'matches' => array(),
  );

  foreach ($original_results as $pid => $info) {
    foreach ($info['snippets'] as $snippet => $bounding_boxes) {
      $match = array(
        'text' => preg_replace('/[^a-zA-Z0-9-\n {}]/', '', $snippet),
        'par' => array(
          array(
            'page' => intval($info['page']['sequence_number']),
            'boxes' => array(),
          ),
        ),
      );

      foreach ($bounding_boxes as $bounding_box) {
        $match['par'][0]['boxes'][] = array(
          'l' => $bounding_box['left'],
          't' => $bounding_box['top'],
          'r' => $bounding_box['right'],
          'b' => $bounding_box['bottom'],
          'page' => $info['page']['sequence_number'],
        );
      }

      $results['matches'][] = $match;
    }
  }

  return $results;
}
