<?php

/**
 * @file
 * Document interfaces/hooks exposed by this module.
 */

/**
 * Allow other modules to alter variables being output into citations.
 *
 * @param array $output
 *   The associative array of variables being passed to citeproc.
 * @param SimpleXMLElement $mods
 *   The MODS from which the original output was produced.
 */
function hook_convert_mods_to_citeproc_jsons_alter(array &$output, SimpleXMLElement $mods) {
  if (!empty($output['abstract'])) {
    $texts = array();

    // CSL can't build up a "structured" abstract on its own, as cannot accept
    // multiple abstracts with labels... So something like this has been
    // desirable elsewhere.
    foreach ($mods->xpath('//mods:mods[1]/mods:abstract') as $abstract) {
      $text = isset($abstract['displayLabel']) ?
        trim($abstract['displayLabel']) . ': ':
        '';

      $text .= (string) $abstract;
      $texts[] = $text;
    }

    $output['abstract'] = implode(' ', $texts);
  }
}
