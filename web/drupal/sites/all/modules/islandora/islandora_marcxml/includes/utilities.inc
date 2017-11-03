<?php

/**
 * @file
 * Helper functions used by this module.
 */

/**
 * Run an XSLT, and return the results.
 *
 * @param array $info
 *   An associative array of parameters, containing:
 *   - input: The input XML in a string.
 *   - xsl: The path to an XSLT file.
 *
 * @return string
 *   The transformed XML, as a string.
 */
function islandora_marcxml_run_xsl_transform($info) {
  $xsl = new DOMDocument();
  $xsl->load($info['xsl']);
  $input = new DOMDocument();
  $input->loadXML($info['input']);
  $processor = new XSLTProcessor();
  $processor->importStylesheet($xsl);
  // XXX: Suppressing warnings regarding unregistered prefixes.
  return @$processor->transformToXML($input);
}

/**
 * Grabs MODS off the given object, and returns it transformed to MARCXML.
 *
 * @param AbstractObject $object
 *   A Fedora object containing a MODS datastream.
 *
 * @return string
 *   The transformed XML, as a string.
 */
function islandora_marcxml_transform_mods_to_marcxml(AbstractObject $object) {
  if (!isset($object['MODS'])) {
    drupal_not_found();
  }
  $mods_str = trim($object['MODS']->content);
  return islandora_marcxml_run_xsl_transform(array(
    'input' => $mods_str,
    'xsl' => drupal_get_path('module', 'islandora_marcxml') . '/xsl/MODS2MARC21slim.xsl',
  ));
}

/**
 * Produce HTML representing MARCXML content.
 *
 * First, transforms MODS to MARCXML, and then MARCXML to HTML.
 *
 * @param AbstractObject $object
 *   A Fedora object containing a MODS datastream.
 *
 * @return string
 *   The transformed HTML, as a string.
 */
function islandora_marcxml_transform_marc_to_html(AbstractObject $object) {
  return islandora_marcxml_run_xsl_transform(array(
    'input' => islandora_marcxml_transform_mods_to_marcxml($object),
    'xsl' => drupal_get_path('module', 'islandora_marcxml') . '/xsl/MARC21slim2HTML.xsl',
  ));
}

/**
 * Populates the MODS datastream with the transformed MARCXML.
 *
 * @param AbstractObject $object
 *   A Fedora object.
 * @param object $file
 *   A Drupal file object containing the MARCXML.
 *
 * @return bool
 *   TRUE if the MODS datastream was created FALSE if it was updated.
 */
function islandora_marcxml_populate_mods(AbstractObject $object, $file) {
  $created = FALSE;
  $transform_args = array(
    'xsl' => drupal_get_path('module', 'islandora_marcxml') . '/xsl/MARC21slim2MODS3-5.xsl',
    'input' => file_get_contents($file->uri),
  );
  $transformed_xml = trim(islandora_marcxml_run_xsl_transform($transform_args));
  if (empty($object['MODS'])) {
    $ds = $object->constructDatastream('MODS', 'M');
    $object->ingestDatastream($ds);
    $created = TRUE;
  }
  else {
    $ds = $object['MODS'];
  }
  $ds->content = $transformed_xml;
  return $created;
}

/**
 * Renders the MARCXML tab.
 *
 * @pararm AbstractObject $object
 *   The object the MARCXML belongs to.
 *
 * @return array
 *   A renderable array representing the tab.
 */
function islandora_marcxml_view(AbstractObject $object) {
  return array(
    'link' => array(
      '#markup' => l(t('Download MARCXML'), "islandora/object/{$object->id}/download_mods_as_marcxml"),
    ),
    'marc' => array(
      '#markup' => islandora_marcxml_transform_marc_to_html($object),
    ),
  );
}
