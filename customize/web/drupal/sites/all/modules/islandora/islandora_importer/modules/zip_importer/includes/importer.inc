<?php

/**
 * @file
 * Defines the ZipBatchImporter.
 */

/**
 * Zip file importer.
 * @author adam
 */
class ZipBatchImporter extends IslandoraBatchImporter {

  protected $itemClass = 'ZipBatchImportObject';

  /**
   * Get the number of items to import from $this->file.
   *
   * @see IslandoraBatchImporter::getNumber()
   */
  public function getNumber() {
    return count($this->file['object_info']);
  }

  /**
   * Get the form for this importer.
   *
   * @see IslandoraBatchImporter::getForm()
   */
  public static function getForm(array &$form_state) {
    // Get the list of valid content models.
    $parent = islandora_object_load($form_state['storage']['parent_pid']);

    $cm_options = array();
    $ns_options = array(
      NULL => t('Default namespace'),
    );
    if (isset($parent['COLLECTION_POLICY'])) {
      $policy_stream = $parent['COLLECTION_POLICY'];
      $policy = new CollectionPolicy($policy_stream->content);

      $cm_options = array();
      $ns_options = array(NULL => t('Default namespace'));
      foreach ($policy->getContentModels() as $pid => $info) {
        $cm_options[$pid] = array('title' => $info['name']);

        $cm_namespace = $info['namespace'];
        if (!empty($cm_namespace)) {
          $cm_namespace = $info['namespace'];
          $cm_namespace = explode(':', $info['namespace']);
          $namespace = trim($cm_namespace[0]);
          $ns_options[$namespace] = $namespace;
        }
      }
    }

    return array(
      'fs' => array(
        '#type' => 'fieldset',
        '#title' => t('Zip Batch Importer'),
        '#description' => t('Select the file containing the assets and metadata to import. Assets and metadata will be matched together based on the portion of the filename without the extension - e.g. my_file.xml and my_file.pdf would be combined into a single object.'),
        'file' => array(
          '#type' => 'managed_file',
          '#title' => t('Zip file containing files to import'),
          '#upload_validators' => array(
            'file_validate_extensions' => array('zip'),
          ),
        ),
        'fs' => array(
          '#type' => 'fieldset',
          '#title' => t('Content Model'),
          '#description' => t('The content model(s) to assign to the imported objects.'),
          'content_model' => array(
            '#type' => 'tableselect',
            '#header' => array('title' => t('Name')),
            '#multiple' => TRUE,
            '#options' => $cm_options,
          ),
        ),
        'namespace' => array(
          '#type' => 'select',
          '#title' => t('Object Namespace'),
          '#description' => t('The namespace in which the imported objects will be created.'),
          '#options' => $ns_options,
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
    if (empty($form_state['values']['file'])) {
      form_set_error('file', t('Need a Zip file!'));
    }

    $content_models = array_filter($form_state['values']['content_model']);
    if (empty($content_models)) {
      form_set_error('content_model', t('Need to select at least one content model.'));
    }

    return !empty($form_state['values']['file']) && !empty($content_models);
  }

  /**
   * Get the info for the source parameter for the itemClass factory.
   *
   * @see IslandoraBatchImporterInterface::getBatchInfo()
   */
  public static function getBatchInfo(array &$form_state) {
    // Load the file.
    $file = file_load($form_state['values']['file']);

    // Grab the content models.
    // Cast to an array, in case there are none?
    $content_models = array_filter((array) $form_state['values']['content_model']);

    $info = array(
      'file' => $file,
      'pid_namespace' => $form_state['values']['namespace'],
      'content_model' => $content_models,
      'object_info' => static::getIngestInfo($file),
      'processed_objects' => array(),
    );
    return $info;
  }

  /**
   * Helper function for getting info on the zip file..
   *
   * @return array
   *   The ingest info.
   */
  protected static function getIngestInfo($file) {
    $zip = new ZipArchive();
    if (($error = $zip->open(drupal_realpath($file->uri))) !== TRUE) {
      drupal_set_message(t('Error opening the provided Zip file.  Code: %code', array(
            '%code' => $error,
          )));
      return;
    }

    $count = $zip->numFiles;

    $info = array();
    for ($i = 0; $i < $count; $i++) {
      $name = $zip->getNameIndex($i);
      $path_info = pathinfo($name);

      $object_key = $path_info['dirname'] . '-_-' . $path_info['filename'];
      if (strpos($path_info['dirname'], '__MACOSX') === FALSE
        && isset($path_info['extension'])
        && $path_info['extension'] != 'DS_Store') {
        $datastream_key = strtolower($path_info['extension']);
        $info[$object_key][$datastream_key] = $name;
      }
    }

    $zip->close();

    return $info;
  }
}

/**
 * ZIP import object.
 *
 * Actually does the heavy-lifting during the import.
 * @author adam
 */
class ZipBatchImportObject extends IslandoraImportObject {

  protected $mods;
  protected static $MARC2MODS = '';
  protected static $DC2MODS = '';
  protected static $DWC2DC = '';
  protected static $MADS2DC = '';
  protected static $transformPath = '';

  /**
   * Function to initialize the static paths for various xsl.
   */
  public static function initializePaths() {

    if (self::$transformPath == '') {

      self::$transformPath = drupal_get_path('module', 'zip_importer') . '/xsl';

      if (self::$MARC2MODS == '') {
        self::$MARC2MODS = self::$transformPath . "/MARC21slim2MODS3-4.xsl";
      }

      if (self::$DC2MODS == '') {
        self::$DC2MODS = self::$transformPath . "/simpleDC2MODS.xsl";
      }

      if (self::$DWC2DC == '') {
        self::$DWC2DC = self::$transformPath . "/dwc_to_dc.xsl";
      }

      if (self::$MADS2DC == '') {
        self::$MADS2DC = self::$transformPath . "/mads_to_dc.xsl";
      }
    }
  }

  /**
   * An AbstractObject for use in calling hooks... Should *NOT* be ingested.
   *
   * @var
   */
  protected $skeleton;

  /**
   * Constructor.
   */
  protected function __construct($source) {
    parent::__construct($source);
    $this->pidNamespace = $this->source['pid_namespace'];
    $this->contentModel = (array) $this->source['content_model'];
    self::initializePaths();
    $this->skeleton = islandora_get_tuque_connection()->repository->constructObject('temp:pid');
    $this->skeleton->models = $this->contentModel;
  }

  /**
   * Get an item from the source.
   *
   * @see IslandoraImportObjectInterface::getOne()
   */
  public static function getOne(&$info) {
    $record = array(
      'pid_namespace' => $info['pid_namespace'],
      'file' => $info['file'],
      'content_model' => $info['content_model'],
    );
    $key = key($info['object_info']);
    $record['object_info'] = $info['processed_objects'][$key] = $info['object_info'][$key];
    unset($info['object_info'][$key]);
    return (empty($record) ? FALSE : new static($record));
  }

  /**
   * Determine the datastream ID for the given file.
   *
   * Gets info from the DS-COMPOSITE-MODELs for the selected content models
   * and attempts to match based on detected MIME-Types.
   *
   * @param string $name
   *   A filename for which to determine a mimetype.
   *
   * @return array
   *   An array containing two values:
   *   - Either boolean FALSE or the datastream ID, as mapped in the
   *     DS-COMPOSITE-MODELs of the selected object.
   *   - A string containing the detected mimetype of the file.
   */
  protected function determineDSIDAndMimetype($name) {
    module_load_include('inc', 'islandora', 'includes/utilities');
    // Something like this would be nice to have...  Need a way to indicate
    // primary assets, though...  Think about basic image, where the content
    // model can contain "OBJ" image/jpeg, "TN" image/jpeg and so on...
    static $mime_detect = NULL;
    static $dsids = NULL;
    $models = (array) $this->source['content_model'];

    if ($mime_detect === NULL || $dsids === NULL) {
      $mime_detect = new MimeDetect();
      $dsids = islandora_get_datastreams_requirements_from_models($models);
    }

    $mimetype = $mime_detect->getMimetype($name);
    $dsid = array();
    // Determine which stream this should be, Gather together all
    // non optional datastream ids.
    foreach ($dsids as $ds => $info) {
      if (in_array($mimetype, $info['mime'])) {
        // Remove optional DS by default.
        if (!$info['optional']) {
          $dsid[$ds] = $info;
        }
      }
    }

    // Fire off to the derivative hook to get the primary DSID's.
    $hook_output = islandora_invoke_hook_list(ISLANDORA_DERIVATIVE_CREATION_HOOK, $models, array($this->skeleton));
    $source_dsid = FALSE;
    foreach ($dsid as $arr_key => $key_val) {
      foreach ($hook_output as $hook_key => $hook_value) {
        if (isset($hook_output[$hook_key]['source_dsid']) && $hook_output[$hook_key]['source_dsid'] == $dsid[$arr_key]['id']) {
          // We have a match on source object, this is it.
          $source_dsid = $hook_output[$hook_key]['source_dsid'];
        }
      }
    }

    // Didn't match a primary datastream fallback for supplemental by matching
    // the first available.
    if (!$source_dsid) {
      foreach ($dsids as $datastream => $ds_info) {
        if (in_array($mimetype, $ds_info['mime'])) {
          $source_dsid = $ds_info['id'];
          break;
        }
      }
    }
    return array($source_dsid, $mimetype);
  }

  /**
   * Acquire base datastreams structure.
   *
   * @see IslandoraImportObject::getDatastreams()
   */
  protected function getDatastreams(&$errors = NULL, &$files = NULL) {
    module_load_include('inc', 'islandora', 'includes/utilities');
    $to_return = parent::getDatastreams($errors, $files);

    foreach ($this->source['object_info'] as $datastream => $name) {
      $do_determine_dsid_and_mimetype = TRUE;
      if ($datastream == 'xml') {

        if ($this->isDWC($this->getXML())) {
          // For Darwin Core file, datastream to DWC and skips the detection.
          $do_determine_dsid_and_mimetype = FALSE;
          $dsid = 'DWC';
          $mimetype = 'application/xml';
        }

        elseif ($this->isMADS($this->getXML())) {
          // For MADS file, datastream to MADS and skips the detection.
          $do_determine_dsid_and_mimetype = FALSE;
          $dsid = 'MADS';
          $mimetype = 'application/xml';
        }

        else {
          // XML streams are handled via the parent implementation,
          // (via get_{mods,dc}()) so let's go to the next item.
          continue;
        }
      }

      if ($do_determine_dsid_and_mimetype) {
        // Determine which stream this should be...  Uses the first matching, as
        // as received from the DC-COMPOSITE.
        list($dsid, $mimetype) = $this->determineDSIDAndMimetype($name);
      }

      if (!$dsid) {
        $errors[] = array(
          t('The detected mimetype of %filename (@mime) is not supported by any of the selected content models.'),
          array(
            '@mime' => $mimetype,
            '%filename' => $name,
          ),
          WATCHDOG_ERROR,
        );
        continue;
      }

      $zip = new ZipArchive();
      $zip->open(drupal_realpath($this->source['file']->uri));
      if ($stream = $zip->getStream($name)) {
        $file = file_save_data($stream, "public://" . basename($name));
        fclose($stream);
        $file->filename = $name;
        $file->status &= ~FILE_STATUS_PERMANENT;
        $file = file_save($file);
        $files[] = $file;

        $to_return += array(
          $dsid => array(
            'dsid' => $dsid,
            'label' => "$dsid datastream",
            'mimetype' => $mimetype,
            'datastream_file' => file_create_url($file->uri),
            'filename' => $name,
            'control_group' => 'M',
            'file' => $file,
          ),
        );
      }
      $zip->close();
    }

    // Correct the labels of the datastreams.
    foreach ($to_return as &$datastream) {
      $datastream['label'] = $this->getLabel($datastream['dsid'], $datastream);
    }

    return $to_return;
  }

  /**
   * Generates a MODS document repersenting the imported data.
   *
   * @see IslandoraImportObject::getMODS()
   */
  public function getMODS() {
    if ($this->mods === NULL) {
      $xml = $this->getXML();
      if ($this->isMODS($xml)) {
        $this->mods = $xml;
      }
      elseif ($this->isMARC($xml)) {
        // If it's a MARC record we need to potentially clean up any collection
        // wrappers that come out of the XSL output before setting the contents.
        // As documented, Islandora does not support multiple records within
        // a single MARC collection wrapper.
        $transformed_marc = static::runXSLTransform(array('input' => $xml, 'xsl' => self::$MARC2MODS));
        $this->mods = $this->cleanupMARC($transformed_marc);
      }
      elseif ($this->isDC($xml)) {
        $this->mods = static::runXSLTransform(array('input' => $xml, 'xsl' => self::$DC2MODS));
      }
      elseif ($this->isDWC($xml)) {
        $this->mods = static::runXSLTransform(array('input' => $this->getDC(), 'xsl' => self::$DC2MODS));
      }
      elseif ($this->isMADS($xml)) {
        $this->mods = static::runXSLTransform(array('input' => $this->getDC(), 'xsl' => self::$DC2MODS));
      }
      if (empty($this->mods)) {
        // If we do not yet have any XML, create a dumb MODS with just the
        // title, and set it.
        $title = htmlspecialchars(pathinfo(reset($this->source['object_info']), PATHINFO_FILENAME));
        $this->mods = <<<EOXML
<mods xmlns="http://www.loc.gov/mods/v3">
  <titleInfo>
    <title>$title</title>
  </titleInfo>
</mods>
EOXML;
      }
    }
    return $this->mods;
  }

  /**
   * Implements the PHP magic method __wakeup.
   */
  public function __wakeup() {
    static::initializePaths();
  }

  /**
   * Generate DC to describe the imported data.
   *
   * @see ZipBatchImportObject::getDC()
   *
   * @return string
   *   A string containing DC XML.
   */
  public function getDC() {
    if ($this->dc === NULL) {
      $xml = $this->getXML();
      if ($this->isDC($xml)) {
        $this->dc = $xml;
      }
      elseif ($this->isDWC($xml)) {
        $this->dc = static::runXSLTransform(array('input' => $xml, 'xsl' => self::$DWC2DC));
      }
      elseif ($this->isMADS($xml)) {
        $this->dc = static::runXSLTransform(array('input' => $xml, 'xsl' => self::$MADS2DC));
      }
      // Otherwise, call the parent implementation (transform from MODS).
      if (empty($this->dc)) {
        parent::getDC();
      }
    }
    return $this->dc;
  }

  /**
   * Gets the label to be used for the given Datastream.
   *
   * @param string $dsid
   *   The datastream identifier.
   * @param array $datastream
   *   The datatream as defined by self::getDatastreams().
   *
   * @return string
   *   The label to use for the datastream.
   */
  protected function getLabel($dsid, $datastream) {
    $use_filename = variable_get('zip_importer_use_filenames_as_labels', FALSE);
    if (isset($this->source['object_info']['xml'])) {
      $xml_filename = $this->source['object_info']['xml'];
      $xml = $this->getXML();
    }
    else {
      return "$dsid datastream";
    }
    switch ($dsid) {
      case 'MODS':
        // Check if a file was associated with MODS.
        // If it was generated some other way we ignore it.
        $has_filename = $this->isMODS($xml);
        return $use_filename && $has_filename ? $xml_filename : 'MODS Record';

      case 'DC':
        // Check if a file was associated with DC.
        // If it was generated some other way we ignore it.
        $has_filename = $this->isDC($xml);
        return $use_filename && $has_filename ? $xml_filename : 'DC Record';

      case "DWC":
        // Check if a file was associated with DarwinCore.
        // If it was generated some other way we ignore it.
        $has_filename = $this->isDWC($xml);
        return $use_filename && $has_filename ? $xml_filename : 'Darwin Core Record';

      case "MADS":
        // Check if a file was associated with MADS.
        // If it was generated some other way we ignore it.
        $has_filename = $this->isMADS($xml);
        return $use_filename && $has_filename ? $xml_filename : 'MADS Record';

      default:
        if ($use_filename && isset($datastream['filename'])) {
          return $datastream['filename'];
        }
    }
    return isset($datastream['label']) ? $datastream['label'] : "$dsid datastream";
  }

  /**
   * Gets the XML file content associated with the source if found.
   */
  protected function getXML() {
    if (isset($this->source['object_info']['xml'])) {
      $zip = new ZipArchive();
      $zip->open(drupal_realpath($this->source['file']->uri));
      $xml = $zip->getFromName($this->source['object_info']['xml']);
      $zip->close();
      return $xml;
    }
    return NULL;
  }

  /**
   * Checks if the given file content is actually a MODS document.
   */
  protected function isMODS($xml) {
    $root_elements = array('mods', 'modsCollection');
    return in_array($this->getLocalNameOfRootElement($xml), $root_elements);
  }

  /**
   * Checks if the given file content is actually a MARC document.
   */
  protected function isMARC($xml) {
    // We are checking collection here to handle the use case where a collection
    // wrapper element is present around a singular record. This does not
    // support multiple MARC records within a collection. This is a case that
    // Islandora itself does not support.
    $root_element = $this->getLocalNameOfRootElement($xml);
    return $root_element == 'record' || $root_element == 'collection';
  }

  /**
   * Checks if the given file content is actually a DC document.
   */
  protected function isDC($xml) {
    return $this->getLocalNameOfRootElement($xml) == 'dc';
  }

  /**
   * Checks if the given file content is actually a Darwin Core document.
   */
  protected function isDWC($xml) {
    $rootdoc = $this->getLocalNameOfRootElement($xml);
    return (($rootdoc == 'DarwinRecordSet') || ($rootdoc == 'SimpleDarwinRecordSet') || ($rootdoc == 'SimpleDarwinRecord'));
  }

  /**
   * Checks if the given file content is actually a MADS document.
   */
  protected function isMADS($xml) {
    $rootdoc = $this->getLocalNameOfRootElement($xml);
    return $rootdoc == 'mads';
  }

  /**
   * Gets the local name of the root element of the given xml document.
   *
   * @param string $xml
   *   An xml document.
   *
   * @return string
   *   The local name of the root element, if found. Otherwise NULL.
   */
  protected function getLocalNameOfRootElement($xml) {
    try {
      $xml = new SimpleXMLElement($xml);
      return $xml->getName();
    }
    catch (Exception $e) {
      return NULL;
    }
  }

  /**
   * Strips off the collection wrapper from MARC if it exists.
   *
   * @param string $mods_xml
   *   The generated MODS XML from the LOC XSLT.
   *
   * @return string
   *   The cleaned up MODS without the MARC collection wrapper, if it exists.
   */
  protected function cleanupMARC($mods_xml) {
    $dom = new DOMDocument();
    $dom->loadXML($mods_xml);
    if ($dom->documentElement->tagName == 'modsCollection') {
      $new_dom = new DOMDocument('1.0', 'UTF-8');
      $new_dom->formatOutput = TRUE;
      $new_node = $new_dom->importNode($dom->getElementsByTagName('mods')->item(0), TRUE);
      $new_dom->appendChild($new_node);
      return $new_dom->saveXML();
    }
    else {
      return $mods_xml;
    }
  }
}
