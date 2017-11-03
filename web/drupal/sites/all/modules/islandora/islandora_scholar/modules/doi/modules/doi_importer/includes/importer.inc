<?php

/**
 * @file
 * Defines a class for importing DOI citations.
 */

class DOIImporter extends IslandoraBatchImporter {

  protected $itemClass = 'DOIImportObject';

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
  public static function getForm(array & $form_state) {
    $openurl_pid = variable_get('islandora_doi_openurl_pid', 'user@example.com');
    if ($openurl_pid == 'user@example.com') {
      drupal_set_message(t('OpenURL PID email address has not been configured, please enter a valid email address in the <a href="@doi-admin-page">Scholar DOI configuration form</a>.', array('@doi-admin-page' => '/admin/islandora/solution_pack_config/scholar/islandora_doi')), 'error');
    }
    else {
      return array(
        'fs' => array(
          '#type' => 'fieldset',
          '#title' => t('DOI Batch Journal Importer'),
          '#description' => t('Provide either a list of Digital Object Identifiers in a file, or enter DOIs in the textarea below. In either case, the DOIs in the list should be separated by either whitespace or commas (or some combination thereof). NOTE: Presently, only journal (articles) are supported.'),
          'file' => array(
            '#type' => 'managed_file',
            '#title' => t('File of DOIs'),
            '#description' => t('A file listing Digital Object Identifiers (with ".txt" extension).'),
            '#upload_validators' => array(
              'file_validate_extensions' => array('txt'),
            ),
          ),
          'textarea' => array(
            '#type' => 'textarea',
            '#title' => t('DOIs'),
            '#description' => t('Provide a list Digital Object Identifiers (DOI) - eg. 10.1016/j.jorganchem.2011.11.018, 10.1016/j.tiv.2011.10.017, 10.1111/j.1540-4560.2012.01733.x'),
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
      $file_dois = trim(file_get_contents(drupal_realpath($file->uri)));
      if ($text) {
        $text .= ' ' . $file_dois;
      }
      else {
        $text = $file_dois;
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
 * DOI import object.
 *
 * Actually does the heavy-lifting during the import.
 * @author adam
 */
class DOIImportObject extends IslandoraImportObject {

  /**
   * Generated MODS Document.
   *
   * @var SimpleXMLElement
   */
  protected $mods;

  /**
   * Get an item from the source.
   *
   * @see IslandoraImportObject::getOne()
   */
  public static function getOne(&$ids) {
    $id = array_shift($ids);

    module_load_include('inc', 'islandora_doi', 'includes/utilities');
    $response = islandora_doi_perform_request($id);
    $headers = array_change_key_case($response->headers);
    if ($response->data &&
      strpos($headers['content-type'], 'text/html') === FALSE &&
      strpos($response->data, "Malformed DOI") === FALSE) {

      $crossref_xml = simplexml_load_string($response->data);
      // Totally don't support books so let's not attempt to make records that
      // are actually books!
      if ($crossref_xml->doi_record->crossref->error) {
        return FALSE;
      }
      elseif (!$crossref_xml->doi_record->crossref->journal) {
        drupal_set_message(t("Only the 'Journal' type is currently supported for DOI importing."), 'error');
        return FALSE;
      }
      else {
        return new self($response->data);
      }
    }
    else {
      return FALSE;
    }
  }

  /**
   * Generates a MODS document representing the imported data.
   *
   * @see IslandoraImportObject::getMODS()
   */
  public function getMODS() {
    if ($this->mods === NULL) {
      // @todo Implement book support.
      module_load_include('inc', 'islandora_doi', 'includes/utilities');
      $doi_xml = new DOMDocument('1.0');
      $doi_xml->loadXML($this->source);
      $this->mods = islandora_doi_transform_to_mods($doi_xml)->saveXML();
    }

    return $this->mods;
  }
}
