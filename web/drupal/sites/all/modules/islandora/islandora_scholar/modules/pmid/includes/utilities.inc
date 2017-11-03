<?php

/**
 * @file
 * Utility code.
 */

/**
 * Perform a request for the given PMID.
 *
 * @param string|int $id
 *   A PMID.
 *
 * @return object
 *   An object as provided by drupal_http_request().
 */
function islandora_pmid_perform_request($id) {
  $pubmed_url = 'http://www.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi';
  $url = url($pubmed_url, array(
    'query' => array(
      'db' => 'pubmed',
      'retmode' => 'xml',
      'id' => $id,
    ),
  ));

  return drupal_http_request($url);
}

/**
 * Get the PMID from the given PubMed Document.
 */
function islandora_pmid_get_pmid(SimpleXMLElement $xml) {
  $pmid = $xml->xpath('//PubmedArticle/MedlineCitation/PMID');
  return (string) array_shift($pmid);
}

/**
 * Normalize some fields.
 *
 * @param DOMDocument $doc
 *   The transformed MODS document to alter.
 */
function islandora_pmid_adjust_output(DOMDocument $doc) {
  $xpath = new DOMXPath($doc);
  $xpath->registerNamespace('mods', 'http://www.loc.gov/mods/v3');

  // Correct dateIssued/dateCaptured
  $results = $xpath->query('//mods:dateIssued | //mods:dateCaptured');
  foreach ($results as $date) {
    islandora_pmid_normalize_date($date, $xpath);
  }

  // Add the record creation date.
  $results = $xpath->query('//mods:recordInfo/mods:recordCreationDate');
  foreach ($results as $date) {
    $date->nodeValue = gmdate("Y-m-d\TH:i:s\Z");
  }
}

/**
 * Normalizes dates used in PubMed, only supports PubDate and ArticleDate.
 *
 * Torn from CoH.
 * Note: Can't normalize <MedlineDate> values.
 *
 * @param DOMElement $date
 *   The element containing the PubMed date.
 * @param DOMXPath $xpath
 *   The xpath used to fetch the date.
 */
function islandora_pmid_normalize_date(DOMElement $date, DOMXPath $xpath) {
  $months = array(
    'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct',
    'Nov', 'Dec');
  $iso = $xpath->evaluate('count(mods:MedlineDate) = 0', $date);
  if ($iso) {
    $date->setAttribute('encoding', 'iso8601');
    $y = $xpath->evaluate('normalize-space
(mods:Year/text())', $date);
    $m = $xpath->evaluate('normalize-space(mods:Month/text())', $date);
    $d = $xpath->evaluate('normalize-space(mods:Day/text())', $date);
  }
  else {
    $string = $xpath->evaluate('normalize-space(mods:MedlineDate/text())', $date);
    if (preg_match('/^[0-9]{4}-[0-9]{2}-[0-9]{2}$/', $string)) {
      list($y, $m, $d) = explode('-', $string);
    }
    elseif (preg_match('/^[0-9]{4}-[0-9]{2}$/', $string)) {
      list($y, $m) = explode('-', $string);
    }
    elseif (preg_match('/^[0-9]{4}$/', $string)) {
      $y = $string;
    }
    elseif (preg_match('/^[0-9]{4}-[0-9]{4}$/', $string)) {
      $y = substr($string, 0, 4);
    }
    elseif (preg_match('/^[0-9]{4} [A-Za-z]{3}-[A-Za-z]{3}$/', $string)) {
      $y = substr($string, 0, 4);
      $m = substr($string, 5, 3);
    }
    elseif (preg_match('/^[0-9]{4} [A-Za-z]{3}$/', $string)) {
      $y = substr($string, 0, 4);
      $m = substr($string, 5, 3);
    }
    elseif (preg_match('/^[0-9]{4}-[A-Za-z]{3}$/', $string)) {
      $y = substr($string, 0, 4);
      $m = substr($string, 5, 3);
    }
  }
  if (isset($m)) {
    $key = array_search(strtolower($m), array_map('strtolower', $months));
    if ($key !== FALSE) {
      $m = $key + 1;
    }
  }
  $values = array(&$y, &$m, &$d);
  foreach ($values as &$val) {
    // Pad mth and day with 0.
    $val = (strlen($val) < 2 && $val) ? str_pad($val, 2, "0", STR_PAD_LEFT) : $val;
  }
  if ($y) {
    $string = $y;
    if ($m) {
      $string .= '-' . $m;
      if ($d) {
        $string .= '-' . $d;
      }
    }
  }
  $date->nodeValue = $string;
}

/**
 * Generates a MODS document repersenting the imported data.
 *
 * @param SimpleXMLElement $pm_xml
 *   The XML scraped resulting from the PMID query.
 *
 * @return DOMDocument
 *   The transformed (and "adjusted") MODS document.
 */
function islandora_pmid_transform_to_mods(SimpleXMLElement $pm_xml) {
  $xsl = new DOMDocument();
  $xsl->load(drupal_get_path('module', 'islandora_pmid') . '/xsl/pubmed_to_mods.xslt');

  $xslt = new XSLTProcessor();
  $xslt->importStylesheet($xsl);

  $pm_xml = $xslt->transformToDoc($pm_xml);

  islandora_pmid_adjust_output($pm_xml);

  return $pm_xml;
}

/**
 * Covenience function to get MODS for the given PMID in one shot.
 *
 * @param string|int $id
 *   A PMID.
 *
 * @return DOMDocument|bool
 *   A DOMDocument containing the transformed MODS, or boolean FALSE on
 *   failure.
 */
function islandora_pmid_get_mods($id) {
  $response = islandora_pmid_perform_request($id);

  if ($response->data) {
    $pubmed_xml = simplexml_load_string($response->data);
    if ($id == islandora_pmid_get_pmid($pubmed_xml)) {
      return islandora_pmid_transform_to_mods($pubmed_xml);
    }
    else {
      return FALSE;
    }
  }
  return FALSE;
}
