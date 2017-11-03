<?php

/**
 * @file
 * Menu callbacks.
 */

/**
 * Retrieve a citation for an object's MODS DS.
 *
 * $_GET['citation_style']
 *   The style to get the citation in.
 *
 * @param AbstractObject $object
 *   The object to get the citation for.
 */
function islandora_scholar_get_citation(AbstractObject $object) {
  drupal_json_output(
    array(
      'citation' => citeproc_bibliography_from_mods(
        citeproc_style($_GET['citation_style']),
        $object['MODS']->content
      ),
    )
  );
}
