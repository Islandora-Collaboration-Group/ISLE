<?php

/**
 * @file
 * Defines the EndNoteXMLImporter.
 */

class EndNoteXMLImporter extends IslandoraBatchImporter {
  protected $itemClass = 'EndNoteXMLImportObject';

  /**
   * Get the number of items to import from $this->file.
   *
   * @see IslandoraBatchImporter::getNumber()
   */
  public function getNumber() {
    $endnote = new DOMDocument();
    $endnote->load($this->file->uri);
    $xpath = new DOMXPath($endnote);
    $results = $xpath->query('/xml/records/record');
    return $results ? $results->length : 0;
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
        '#title' => t('EndNote XML Batch Importer'),
        'file' => array(
          '#type' => 'managed_file',
          '#title' => t('File of EndNote XML records to import'),
          '#upload_validators' => array(
            'file_validate_extensions' => array('xml'),
          ),
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
    return !empty($form_state['values']['file']);
  }

  /**
   * Get the required info to pass into the file parameter of the constructor.
   *
   * @see IslandoraBatchImporter::getBatchInfo()
   */
  public static function getBatchInfo(array &$form_state) {
    $file = file_load($form_state['values']['file']);
    return $file;
  }
}

/**
 * EndNote XML import object.
 *
 * Actually does the Heavy-lifting during the import.
 * @author adam
 */
class EndNoteXMLImportObject extends IslandoraImportObject {
  protected $mods;

  /**
   * Get an item from the source.
   *
   * @see IslandoraImportObject::getOne()
   */
  public static function getOne(&$file) {
    $record = '';

    $endnote = new DOMDocument();
    $endnote->load($file->uri);
    module_load_include('inc', 'islandora_endnotexml', 'includes/utilities');
    $record = islandora_endnotexml_extract_first($endnote);
    if ($record) {
      $endnote->save($file->uri);
      file_save($file);
    }
    return (empty($record) ? FALSE : new static($record));
  }

  /**
   * Generates a MODS document repersenting the imported data.
   *
   * @see IslandoraImportObject::getMODS()
   */
  public function getMODS() {
    if ($this->mods === NULL) {
      module_load_include('inc', 'islandora_endnotexml', 'includes/utilities');
      $this->mods = islandora_endnotexml_transform_to_mods($this->source);
    }

    return $this->mods;
  }
}
