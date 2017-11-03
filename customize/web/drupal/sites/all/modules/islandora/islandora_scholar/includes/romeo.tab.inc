<?php

/**
 * @file
 * Romeo tab for the islandora_scholar module.
 */

/**
 * Page callback, to render the RoMEO tab.
 *
 * Grabs the ISSN from the object, and fires it at the configured URL.
 */
function islandora_scholar_romeo_view($object) {
  return RomeoView::getRomeoMarkup(RomeoView::getISSN($object));
}

/**
 * Class to bundle up the functions pertaining to rendering RoMEO results.
 */
class RomeoView {
  /**
   * Get an ISSN from the object's MODS.
   *
   * @param AbstractObject $object
   *   A AbstractObject whose MODS we are to check.
   *
   * @return string|FALSE
   *   Returns either the ISSN in a string, or boolean FALSE if no ISSN could
   *   be found.
   */
  public static function getISSN($object) {
    $cache_id = "islandora_scholar_issn_cache_{$object->id}";
    // Try to get the issn of the object.
    if ($issn = cache_get($cache_id)) {
      return $issn->data;
    }
    else {
      $issn = FALSE;

      // @todo Try to get the ISSN from Solr?
      if (!$issn) {

      }

      // Try to get the ISSN from the object in Fedora Proper.
      if (!$issn) {
        // Try to get the ISSN from a MODS stream.
        if (!$issn && isset($object['MODS'])) {
          $mods_doc = new DOMDocument();
          $mods_doc->loadXML($object['MODS']->content);
          $mods_xpath = new DOMXPath($mods_doc);
          $mods_xpath->registerNamespace('m', 'http://www.loc.gov/mods/v3');

          $issn_results = $mods_xpath->query('//m:identifier[@type="issn" and normalize-space(text())]');
          if ($issn_results->length > 0) {
            $issn = $issn_results->item(0)->textContent;
          }
        }
      }

      // Cache even if not successful...  It's still meaningful that we're not
      // able to find one.
      $cache_time = variable_get('islandora_scholar_issn_cache_time', '2');
      cache_set($cache_id, $issn, 'cache', time() + ($cache_time * 86400));

      return $issn;
    }
  }

  /**
   * Fetch markup for the given ISSN.
   *
   * @param string $issn
   *   The ISSN to lookup.
   *
   * @return string
   *   Either a string containing the RoMEO markup, or an empty string in the
   *   case we could not fetch it (in which case we will also set a Drupal
   *   message).
   */
  public static function getRomeoMarkup($issn) {
    $cache_id = "islandora_scholar_romeo_cache_$issn";
    if ($romeo_cache = cache_get($cache_id)) {
      return $romeo_cache->data;
    }
    else {
      $params = array(
        'issn' => $issn,
      );
      if ($key = variable_get('islandora_scholar_romeo_key', '')) {
        $params['ak'] = $key;
      }

      $ch = curl_init(url(variable_get('islandora_scholar_romeo_url', 'http://www.sherpa.ac.uk/romeo/api29.php'), array(
        'query' => $params,
      )));

      // @todo Use Drupal's HTTP request function.
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
      $romeo_content = trim(curl_exec($ch));
      curl_close($ch);

      try {
        $processor = new XsltProcessor();
      }
      catch (Exception $e) {
        drupal_set_message(t('@error', array('@error' => $e->getMessage())), 'error');
        return;
      }

      $xsl = new DOMDocument();
      $xsl->load(drupal_get_path('module', 'islandora_scholar') . '/xsl/romeo.xsl');
      $input = new DOMDocument();
      if ($romeo_content) {
        $input->loadXML($romeo_content);
        $xsl = $processor->importStylesheet($xsl);
        $xml_output = $processor->transformToDoc($input);
        $romeo_markup = $xml_output->saveXML();
        $cache_time = variable_get('islandora_scholar_romeo_cache_time', '2');
        cache_set($cache_id, $romeo_markup, 'cache', time() + ($cache_time * 86400));
        return $romeo_markup;
      }
      else {
        drupal_set_message(t('Could not retrieve RoMEO information for ISSN: @issn!', array('@issn' => $issn)), 'warning');
        return '';
      }
    }
  }
}
