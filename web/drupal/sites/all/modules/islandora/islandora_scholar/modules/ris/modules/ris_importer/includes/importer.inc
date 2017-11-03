<?php

/**
 * @file
 * Defines RISImporter.
 */

class RISImporter extends IslandoraBatchImporter {
  protected $itemClass = 'RISImportObject';

  /**
   * Get the number of items to import from $this->file.
   *
   * @see IslandoraBatchImporter::getNumber()
   */
  public function getNumber() {
    $count = 0;
    $file = fopen($this->file->uri, 'r');
    while (($line = fgets($file)) !== FALSE) {
      if (trim($line) == 'ER  -') {
        $count++;
      }
    }
    fclose($file);
    return $count;
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
        '#title' => t('RIS Batch Importer'),
        'file' => array(
          '#type' => 'managed_file',
          '#title' => t('File of RIS records to import (".ris" extension).'),
          '#upload_validators' => array(
            'file_validate_extensions' => array('ris'),
          ),
        ),
        'submit' => array(
          '#type' => 'submit',
          '#value' => t('Import'),
        ),
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
 * RIS import object.
 *
 * Actually does the heavy-lifting during the import.
 * @author adam
 */
class RISImportObject extends IslandoraImportObject {
  protected $mods;

  /**
   * Get an item from the source.
   *
   * @see IslandoraImportObject::getOne()
   */
  public static function getOne(&$file) {
    module_load_include('inc', 'islandora_ris', 'includes/utilities');
    $first = islandora_ris_extract_first($file);
    return $first ? new static($first) : FALSE;
  }

  /**
   * Generates a MODS document repersenting the imported data.
   *
   * @see IslandoraImportObject::getMODS()
   */
  public function getMODS() {
    if ($this->mods === NULL) {
      module_load_include('inc', 'islandora_ris', 'includes/utilities');
      $this->mods = islandora_ris_transform_to_mods($this->source);
    }

    return $this->mods;
  }
}
