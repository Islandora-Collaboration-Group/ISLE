<?php

/**
 * @file
 * A basic batch ingest implementation (preprocessor and object).
 *
 * Works much the same as the zip importer in the islandora_importer module,
 * or the old D6 zip batch ingester.
 */

/**
 * A class which is capable of getting the listing of a directory or zip file.
 *
 * Used as a basis for the book batch. Really, just adds a couple methods to
 * help out.
 */
class IslandoraScanBatch extends IslandoraBatchPreprocessor {

  // Change to FALSE if one wants to take control over hierarchical structures.
  // @todo Make zip scan respect this.
  public $recursiveScan = TRUE;

  /**
   * Get a listing of "file-object"-like entries.
   *
   * @return array
   *   An associative array of stdClass objects representing files. Array keys
   *   are URIs relative to the "target", and the objects properties include:
   *   - uri: A string containing the complete URI of the resource.
   *   - filename: The filename.
   *   - name: The filename without its extension.
   */
  protected function scan() {
    $method = "scan" . ucfirst($this->getTargetType());
    return $this->$method($this->getTarget());
  }

  /**
   * Get the target resource.
   */
  protected function getTarget() {
    return $this->parameters['target'];
  }

  /**
   * Get the type of the target resource.
   *
   * Prefixed with "scan_" to determine which method gets called to generate
   * the list of resource.
   */
  protected function getTargetType() {
    return $this->parameters['type'];
  }

  /**
   * Allow the pattern to be set differently.
   */
  protected static function getPattern() {
    return '/.*/';
  }

  /**
   * Scan the directory with file_scan_directory().
   */
  protected function scanDirectory($target) {
    $files = array();
    $target_path = drupal_realpath($target);
    $target_length = strlen($target_path) + 1;
    $directory_contents = file_scan_directory(
      $target_path,
      $this->getPattern(),
      array('recurse' => $this->recursiveScan)
    );
    foreach ($directory_contents as $uri => $value) {
      $files[substr($uri, $target_length)] = $value;
    }
    return $files;
  }

  /**
   * Generate output analogous to file_scan_directory().
   */
  protected function scanZip($target) {
    $zip = new ZipArchive();

    $zip_file = drupal_realpath($target);
    if (($error = $zip->open($zip_file)) !== TRUE) {
      return FALSE;
    }

    $count = $zip->numFiles;

    $info = array();
    for ($i = 0; $i < $count; $i++) {
      $raw_name = $zip->getNameIndex($i, ZipArchive::FL_UNCHANGED);
      if (isset($this->parameters['zip_encoding']) && trim($this->parameters['zip_encoding']) !== '') {
        $name = drupal_convert_to_utf8($raw_name, $this->parameters['zip_encoding']);
      }
      else {
        $name = $zip->getNameIndex($i);
      }
      $path_info = pathinfo($name);

      // Avoid creating entries for directories and some weird Mac stuff...
      // XXX: Should be made to take the pattern from
      // $this::getPattern(), and to filter the results based on it.
      if (isset($path_info['extension']) && strpos($path_info['dirname'], '__MACOSX') === FALSE && $path_info['extension'] != 'DS_Store') {
        $file_entry = new stdClass();
        $file_entry->uri = "zip://$zip_file#$raw_name";
        $file_entry->filename = $path_info['basename'];
        $file_entry->name = $path_info['filename'];
        $info[$name] = $file_entry;
      }
    }

    $zip->close();

    return $info;
  }

  /**
   * Group file entries logically, to pass off to the import object.
   *
   * Default implementation groups on files' basename.
   *
   * @param array $files
   *   An array, as returned by file_scan_directory().
   */
  protected function groupFiles($files) {
    $grouped = array();

    // TODO: Group according to basename, to and hand-off each group to object
    // wrappers.
    foreach ($files as $uri => &$file_object) {
      $file_object->ext = pathinfo($file_object->filename, PATHINFO_EXTENSION);
      $grouped[$file_object->name][$file_object->ext] = $file_object;
    }

    return $grouped;
  }

  /**
   * Get the name of the class to instantiate for the batch operations.
   */
  protected static function getObjectClass() {
    return "IslandoraScanBatchObject";
  }

  /**
   * Perform preprocessing of the scanned resources.
   */
  public function preprocess() {
    $files = $this->scan();

    if ($files === FALSE) {
      // Failed to scan.
      // TODO: Pop an error.
    }
    elseif (empty($files)) {
      // No contents.
      // TODO: Pop a warning.
    }

    $added = array();

    $grouped = $this->groupFiles($files);

    $object_class = static::getObjectClass();
    foreach ($grouped as $base_name => $object_info) {
      $ingest_object = new $object_class($this->connection, $base_name, $object_info, $this->parameters);

      // XXX: Might be better to have this actually handled as another
      // "preprocessor", so arbitrary "tree" structures might be built?
      $added = array_merge($added, $this->preprocessChildren($ingest_object));
    }

    return $added;
  }

  /**
   * Recursively attempt to preprocess children.
   */
  protected function preprocessChildren(IslandoraBatchObject $object, $parent = NULL) {
    $to_return = array();

    // XXX: Squash exceptions and log 'em.
    try {
      $object->addRelationships();
      $this->addToDatabase($object, $object->getResources(), $parent);
      $to_return[] = $object;
      $children = $object->getChildren($this->connection);
      $child_count = count($children);
      if ($child_count > 0) {
        if (isset($this->parameters['namespace'])) {
          $namespace = $this->parameters['namespace'];
        }
        else {
          $namespace = $this->determineNamespace($object);
        }
        $children_ids = (array) $this->connection->repository->getNextIdentifier($namespace, NULL, $child_count);
        foreach ($children as $child) {
          $pid = array_shift($children_ids);
          if ($pid !== NULL) {
            $child->id = $pid;
            $this->preprocessChildren($child, $object->id);
          }
          else {
            throw new Exception(t('No PID was available to allocate for object.'));
          }
        }
      }
    }
    catch (Exception $e) {
      watchdog_exception('islandora_batch', $e);
    }

    return $to_return;
  }
}

class IslandoraScanBatchObject extends IslandoraBatchObject {
  protected $baseName;
  protected $objectInfo;
  // Keys in objectInfo that are not to be datastreams.
  public $objectInfoDatastreamExclusions = array('xml', 'mrc');
  protected $preprocessorParameters;
  // Path to MODS->DC XSLT.
  public $modsToDcTransform;

  /**
   * Constructor for the IslandoraScanBatchObject.
   */
  public function __construct(IslandoraTuque $connection, $base_name, $object_info, $preprocessor_parameters) {
    parent::__construct(NULL, $connection->repository);

    $this->modsToDcTransform = drupal_get_path('module', 'islandora_batch') . '/transforms/mods_to_dc.xsl';
    $this->baseName = $base_name;
    $this->objectInfo = $object_info;
    $this->resources = array();
    $this->preprocessorParameters = $preprocessor_parameters;
  }

  /**
   * Static Function to get mime.
   */
  protected static function getMime($file_object) {
    static $mime_detect = NULL;
    if ($mime_detect === NULL) {
      $mime_detect = new MimeDetect();
    }

    return $mime_detect->getMimetype($file_object->filename);
  }

  /**
   * Function batch_process.
   */
  public function batchProcess() {
    // Use object_info to create some datastreams.
    $this->label = $this->getTitle();
    $this->getMods();
    $this->getDc();

    if (!isset($this['OBJ'])) {
      $other = array_diff_key($this->objectInfo, array_flip($this->objectInfoDatastreamExclusions));
      foreach ($other as $obj) {
        list($dsid, $mimetype) = static::determineDSIDAndMimetype($obj->filename);
        $obj_datastream = $this->constructDatastream($dsid);
        $obj_datastream->mimetype = $mimetype;
        $obj_datastream->label = "$dsid Datastream";
        $obj_datastream->setContentFromFile($obj->uri, FALSE);

        $this->ingestDatastream($obj_datastream);
      }
    }

    return ISLANDORA_BATCH_STATE__DONE;
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
   * - Either boolean FALSE or the datastream ID, as mapped in the
   * DS-COMPOSITE-MODELs of the selected object.
   * - A string containing the detected mimetype of the file.
   */
  protected function determineDSIDAndMimetype($name) {
    // Something like this would be nice to have... Need a way to indicate
    // primary assets, though... Think about basic image, where the content
    // model can contain "OBJ" image/jpeg, "TN" image/jpeg and so on...
    static $mime_detect = NULL;
    static $dsids = NULL;

    if ($mime_detect === NULL || $dsids === NULL) {
      $mime_detect = new MimeDetect();
      module_load_include('inc', 'islandora', 'includes/utilities');
      $models = (array) $this->preprocessorParameters['content_models'];
      $dsids = islandora_get_datastreams_requirements_from_models($models);
    }

    $mimetype = $mime_detect->getMimetype($name);
    $dsid = FALSE;
    // Determine which stream this should be... Uses the first matching, as
    // received from the DC-COMPOSITE.
    foreach ($dsids as $ds => $info) {
      if (in_array($mimetype, $info['mime'])) {
        $dsid = $ds;
        break;
      }
    }
    return array($dsid, $mimetype);
  }

  /**
   * Get resources for the current object.
   */
  public function getResources() {
    return $this->resources;
  }

  /**
   * Get the title for this object.
   *
   * Gets the label not empty; otherwise, grabs a title from the MODS.
   */
  protected function getTitle() {
    if (!isset($this->label) || empty($this->label)) {
      // Acquire a title.
      $mods = $this->getMods();
      if ($mods) {
        $mods_doc = new DOMDocument();
        $mods_doc->loadXML($mods);
        $mods_xpath = new DOMXPath($mods_doc);
        $mods_xpath->registerNamespace('m', 'http://www.loc.gov/mods/v3');

        $this->label = $mods_xpath->evaluate('string(//m:mods/m:titleInfo/m:title/text())');
      }
    }

    return $this->label;
  }

  /**
   * Run an XSLT, and return the results.
   *
   * @param array $info
   *   An associative array of parameters, containing:
   *   - input: The input XML in a string.
   *   - xsl: The path to an XSLT file.
   *   - php_functions: Either a string containing one or an array containing
   *     any number of functions to register with the XSLT processor.
   *
   * @return string
   *   The transformed XML, as a string.
   */
  protected static function runXslTransform($info) {
    $xsl = new DOMDocument();
    $xsl->load($info['xsl']);

    $input = new DOMDocument();
    $input->loadXML($info['input']);

    $processor = new XSLTProcessor();
    $processor->importStylesheet($xsl);

    if (isset($info['php_functions'])) {
      $processor->registerPHPFunctions($info['php_functions']);
    }

    // XXX: Suppressing warnings regarding unregistered prefixes.
    return $processor->transformToXML($input);
  }

  /**
   * Function to get the mods.
   */
  protected function getMods() {
    if (!isset($this['MODS'])) {
      $dir = drupal_get_path('module', 'islandora_batch');
      // TODO: Create the MODS datastream on this object.
      $mods_datastream = $this->constructDatastream('MODS', 'M');
      $mods_datastream->mimetype = 'application/xml';
      $mods_datastream->label = 'MODS Record';

      // If we have an XML stream which is:
      if (isset($this->objectInfo['xml'])) {
        $xml = file_get_contents($this->objectInfo['xml']->uri);

        if ($xml) {
          $s_xml = new SimpleXMLElement($xml);
          // MODS, set.
          if ($s_xml->getName() == 'mods') {
            // XXX: Should probably create a resource entry.
            $mods_datastream->setContentFromFile($this->objectInfo['xml']->uri, FALSE);
          }
          // MARCXML, transform to MODS and set.
          elseif (in_array($s_xml->getName(), array('collection', 'record'))) {
            $mods_datastream->content = static::runXslTransform(array(
              'input' => $xml,
              'xsl' => $dir . '/transforms/MARC21slim2MODS3-4.xsl',
            ));
          }
          // DC, transform to MODS and set.
          elseif ($s_xml->getName() == 'dc') {
            $mods_datastream->content = static::runXslTransform(array(
              'input' => $xml,
              'xsl' => $dir . '/transforms/simpleDC2MODS.xsl',
            ));
          }
          // Unrecognized format...
          else {
            // Do nothing?
          }
        }
      }

      // We use some Java code from LoC to transform binary MARC to MODS.
      if (!isset($mods_datastream->content) && isset($this->objectInfo['mrc'])) {
        // Generate some MODS from binary MARC.
        // Get the java executable.
        $java = variable_get('islandora_batch_java', 'java');
        $temp_file_marc = tempnam('/tmp', 'binmrc');
        $temp_file_mods = tempnam('/tmp', 'mods');

        // Copy the file, so we're sure it's accessible on the filesystem.
        copy($this->objectInfo['mrc']->uri, $temp_file_marc);
        $absolute = realpath($dir);

        // The command to run.
        $command = "$java -cp $absolute/transforms/marcxml/marcxml.jar:$absolute/transforms/marcxml/marc4j.jar gov.loc.marcxml.MARC2MODS $temp_file_marc $temp_file_mods";

        // Run the command.
        $proc = popen($command, 'r');
        pclose($proc);

        // Load the output into a DOMDocument.
        $doc = new DOMDocument();
        $doc->load($temp_file_mods);

        // Drop the temp.
        unlink($temp_file_marc);
        unlink($temp_file_mods);

        $mods_docs = $doc->getElementsByTagName('mods');
        if ($mods_docs->length > 0) {
          // Grab the first.
          $mods = new DOMDocument();
          $mods_node = $mods_docs->item(0);
          $mods_node = $mods->importNode($mods_node, TRUE);
          $mods->appendChild($mods_node);

          // TODO: Transform dates to Solr format...  Or should this be left to
          // a core hook?
          // And put the content into the datastream.
          $mods_datastream->content = $mods->saveXML();
        }
      }

      if (!isset($mods_datastream->content)) {
        // If we do not yet have any XML, create a dumb MODS with just the
        // title, and set it...
        $title = $this->baseName;

        $mods_datastream->content = <<<EOXML
<mods xmlns:mods="http://www.loc.gov/mods/v3" xmlns="http://www.loc.gov/mods/v3">
  <titleInfo>
    <title>$title</title>
  </titleInfo>
</mods>
EOXML;
      }

      $this->ingestDatastream($mods_datastream);
    }

    return $this['MODS']->content;
  }
  /**
   * Function to get dc.
   */
  protected function getDc() {
    if (!isset($this['DC'])) {
      $ingest_dc = FALSE;
      // TODO: Create the DC datastream on this object.
      $dc_datastream = $this->constructDatastream('DC', 'M');
      $dc_datastream->mimetype = 'application/xml';
      $dc_datastream->label = 'DC Record';

      if (isset($this->objectInfo['xml'])) {
        $xml = file_get_contents($this->objectInfo['xml']->uri);

        if ($xml) {
          $s_xml = new SimpleXMLElement($xml);
          if ($s_xml->getName() == 'dc') {
            // XXX: Should probably create a resource entry.
            $dc_datastream->setContentFromFile($this->objectInfo['xml']->uri, FALSE);
            $ingest_dc = TRUE;
          }
        }
      }

      // Get the DC by transforming from MODS.
      if (!isset($dc_datastream->content)) {
        // XXX: Might want to make this use a file, instead of staging the DC
        // in the database table (inside the object we serialize).
        $mods_content = $this->getMods();
        if ($mods_content) {
          $new_dc = static::runXslTransform(
            array(
              'xsl' => $this->modsToDcTransform,
              'input' => $mods_content,
            )
          );
        }
        if (isset($new_dc)) {
          $ingest_dc = TRUE;
          $dc_datastream->content = $new_dc;
        }
      }
      if ($ingest_dc) {
        $this->ingestDatastream($dc_datastream);
      }
    }

    return isset($this['DC']) ? $this['DC']->content : FALSE;
  }

  /**
   * Add collection and content model relationships.
   */
  public function addRelationships() {
    $collection = $this->preprocessorParameters['parent'];
    $predicate = $this->preprocessorParameters['parent_relationship_pred'];
    $uri = $this->preprocessorParameters['parent_relationship_uri'];

    $this->relationships->add($uri, $predicate, $collection);
    $this->addContentModelRelationships();
    $this->inheritXacmlPolicies();
  }

  /**
   * Add inheritXacmlFrom relationship.
   */
  protected function inheritXacmlPolicies() {
    if (module_exists('islandora_xacml_editor')) {
      $collection = $this->preprocessorParameters['parent'];
      $parent = islandora_object_load($collection);
      if ($parent) {
        islandora_xacml_editor_apply_parent_policy($this, $parent);
      }
    }
  }

  /**
   * Add the content model relationship(s).
   */
  protected function addContentModelRelationships() {
    $this->models = $this->preprocessorParameters['content_models'];
  }

}
