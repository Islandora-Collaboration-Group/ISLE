<?php

/**
 * @file
 * Defines the PMImporter.
 */

class PMImporter extends IslandoraBatchImporter {
  protected $itemClass = 'PMIDImportObject';

  /**
   * Get the number of items to import from $this->file.
   *
   * @see IslandoraBatchImporter::getNumber()
   */
  public function getNumber() {
    // "file" is perhaps a misnomer.
    return count($this->file);
  }

  /**
   * Get the form for this importer.
   *
   * @see IslandoraBatchImporter::getForm()
   */
  public static function getForm(array &$form_state) {
    return array(
      'fs' => array(
        '#type' => 'fieldset',
        '#title' => t('Pubmed Batch Importer'),
        '#description' => t("Give either a list of Pubmed IDs in a file, or enter the list in the textarea. In either case, the items in the list should be separated by either whitespace or commas (or some combination thereof)."),
        'file' => array(
          '#type' => 'managed_file',
          '#title' => t('File of Pubmed IDs'),
          '#description' => t('A file listing Pubmed IDs (".txt" extension).'),
          '#upload_validators' => array(
            'file_validate_extensions' => array('txt'),
          ),
        ),
        'textarea' => array(
          '#type' => 'textarea',
          '#title' => t('Pubmed IDs'),
          '#description' => t('A list Pubmed IDs.'),
        ),
        'ids' => array(
          '#type' => 'value',
          '#value' => array(),
        ),
        'submit' => array(
          '#type' => 'submit',
          '#value' => t('Import'),
        ),
      ),
      '#attributes' => array(
        'enctype' => 'multipart/form-data',
      ),
    );
  }

  /**
   * Determine if we're ready to run our batch process.
   *
   * @see IslandoraBatchImporter::readyForBatch()
   */
  public static function readyForBatch(array &$form_state) {
    $text = trim($form_state['values']['textarea']);
    $file = file_load($form_state['values']['file']);
    if ($file) {
      $file_pmids = trim(file_get_contents($file->uri));
      if ($text) {
        $text .= ' ' . $file_pmids;
      }
      else {
        $text = $file_pmids;
      }
      file_delete($file);
      unset($form_state['values']['file']);
    }

    if ($text) {
      $form_state['values']['ids'] = array_unique(preg_split("/[\s,]+/", $text));
    }

    return !empty($form_state['values']['ids']);
  }

  /**
   * Get the required info to pass into the file parameter of the constructor.
   *
   * @see IslandoraBatchImporter::getBatchInfo()
   */
  public static function getBatchInfo(array &$form_state) {
    $ids = $form_state['values']['ids'];
    return $ids;
  }
}

/**
 * Pubmed ID import object.
 *
 * Actually does the heavy-lifting during the import.
 * @author adam
 */
class PMIDImportObject extends IslandoraImportObject {
  protected $mods;

  /**
   * Get an item from the source.
   *
   * @see IslandoraImportObject::getOne()
   */
  public static function getOne(&$ids) {
    $id = array_shift($ids);

    module_load_include('inc', 'islandora_pmid', 'includes/utilities');
    $response = islandora_pmid_perform_request($id);

    if ($response->data) {
      $pubmed_xml = simplexml_load_string($response->data);
      if ($id == self::getPMID($pubmed_xml)) {
        return new self($response->data);
      }
      else {
        return FALSE;
      }
    }
  }

  /**
   * Get the PMID from the given PubMed Document.
   */
  protected static function getPMID(SimpleXMLElement $xml) {
    module_load_include('inc', 'islandora_pmid', 'includes/utilities');
    return islandora_pmid_get_pmid($xml);
  }

  /**
   * Normalize some fields.
   *
   * This /would/ call islandora_pmid_adjust_output(), except for the
   * overridable "normalizeDate()" call.
   */
  protected static function correctTransform(DOMDocument $doc) {
    $xpath = new DOMXPath($doc);
    $xpath->registerNamespace('mods', 'http://www.loc.gov/mods/v3');

    // Correct dateIssued/dateCaptured
    $results = $xpath->query('//mods:dateIssued | //mods:dateCaptured');
    foreach ($results as $date) {
      self::normalizeDate($date, $xpath);
    }

    // Add the record creation date.
    $results = $xpath->query('//mods:recordInfo/mods:recordCreationDate');
    foreach ($results as $date) {
      $date->nodeValue = gmdate("Y-m-d\TH:i:s\Z");
    }

    return $xpath->document;
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
  protected static function normalizeDate(DOMElement $date, DOMXPath $xpath) {
    module_load_include('inc', 'islandora_pmid', 'includes/utilities');
    islandora_pmid_normalize_date($date, $xpath);
  }

  /**
   * Generates a MODS document representing the imported data.
   *
   * This /would/ call islandora_pmid_transform_to_mods(), except for the
   * overridable "correctTransform()" call.
   *
   * @see IslandoraImportObject::getMODS()
   */
  public function getMODS() {
    if ($this->mods === NULL) {
      $xsl = new DOMDocument();
      $xsl->load(drupal_get_path('module', 'islandora_pmid') . '/xsl/pubmed_to_mods.xslt');
      $pm_xml = new DOMDocument();
      $pm_xml->loadXML($this->source);

      $xslt = new XSLTProcessor();
      $xslt->importStylesheet($xsl);

      $pm_xml = $xslt->transformToDoc($pm_xml->documentElement);

      $pm_xml = self::correctTransform($pm_xml);

      $this->mods = $pm_xml->saveXML();
    }

    return $this->mods;
  }
}
