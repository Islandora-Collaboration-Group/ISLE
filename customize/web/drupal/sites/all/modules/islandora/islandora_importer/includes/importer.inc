<?php

/**
 * @file
 * Importer interface for the heavy-lifting.
 */

interface IslandoraBatchImporterInterface {
  /**
   * Determine if we're ready to run our batch process...
   *
   * ... probably checking values.  Might almost be thought of as validation?
   *
   * @param array $form_state
   *   An array containing the form state.
   *
   * @return bool
   *   A boolean indicating whether or not we have all the info to start the
   *   batch is available.
   */
  public static function readyForBatch(array &$form_state);

  /**
   * Get the info for the source parameter for the itemClass factory.
   *
   * This is also used to populate the file parameter to the Importer's
   * constructor.
   *
   * @param array $form_state
   *   An array containing the form state (very likely resulting from the
   *   submission of the form returned from self::getForm
   *
   * @return mixed
   *   Whatever (serializable) data will be passed to self::$itemClass->getOne()
   */
  public static function getBatchInfo(array &$form_state);
}

/**
 * Batch importer interface/abstract class.
 *
 * Implement the abstract functions (and those in the Import Object/item class)
 * and it should be able to work.
 *
 * @author adam@discoverygarden.ca
 */
abstract class IslandoraBatchImporter extends IslandoraBatchPreprocessor implements IslandoraBatchImporterInterface {
  /**
   * A string containing the name of the class to actually do the heavy
   * lifting.
   * @var string
   */
  protected $itemClass = "IslandoraImportObject";

  /**
   * May be a  bit of a misnomer...  We don't really make any assumptions
   *   about what this holds...  We just pass it as a parameter to
   *   abstract methods...  Irrespective of whether the given implementation
   *   actually uses them.  Probably has to be serializable to work properly
   *   with the batch stuff.
   * @var mixed
   */
  protected $file;

  /**
   * The Fedora collection PID into which items should be imported as members.
   * @var string
   */
  protected $parentPid;

  /**
   * Store the batch context, to avoid passing it to individual methods.
   * @var array
   */
  protected $context;

  /**
   * Constructor.
   *
   * Just store the passed in parameters in members.
   *
   * @param IslandoraTuque $connection
   *   A Tuque connection, so we can setup objects.
   * @param mixed $file
   *   May be a  bit of a misnomer...  We don't really make any assumptions
   *   about what this holds...  We just pass it along to the item class.
   * @param string $parent_pid
   *   The PID if the parent to which the ingested objects will belong.
   */
  public function __construct(IslandoraTuque $connection, $file, $parent_pid) {
    $this->file = $file;
    $this->parentPid = $parent_pid;

    $parameters = array(
      'file' => $file,
      'parent_pid' => $parent_pid,
    );

    parent::__construct($connection, $parameters);
  }

  /**
   * Get the form for this importer.
   *
   * Could have a multiple-page form given readyForBatch and the form state.
   */
  public static function getForm(array &$form_state) {
    return array(
      'nothing' => array(
        '#type' => 'item',
        '#value' => t('Warning: this importer does not define a form'),
      ),
    );
  }

  /**
   * Get the number of items to import from $this->file.
   *
   * @return int
   *   The number of items left to process.
   */
  public abstract function getNumber();

  /**
   * Get an identifier.
   *
   * @return string
   *   An identifier for the repo.
   */
  protected function getIdentifier(IslandoraTuque $tuque, $namespace) {
    if (empty($this->context['results']['pid_cache'])) {
      // Get enough PIDs for half of the remaining items.
      // (plus one, so we'll always get at least one).
      $this->context['results']['pid_cache'] = (array) $tuque->repository->getNextIdentifier(
        $namespace,
        NULL,
        intval((($this->context['sandbox']['max'] - $this->context['sandbox']['progress']) / 2) + 1)
      );
    }

    return array_shift($this->context['results']['pid_cache']);
  }

  /**
   * Try to get the namespace from the collection policy.
   *
   * @param IslandoraImportObject $item
   *   So we can get the item's namepace, if we don't get one from a collection
   *   policy.
   *
   * @return string
   *   The namespace with which the object should be ingested.
   */
  protected function getNamespace($item) {
    // Get namespace based on collection, then from item.
    $collection_policies =& drupal_static(__METHOD__, array());
    $namespace = $item->pidNamespace;
    if (module_exists('islandora_basic_collection') && !isset($collection_policies[$this->parentPid])) {
      $parent = islandora_object_load($this->parentPid);
      if (isset($parent['COLLECTION_POLICY'])) {
        $policy_stream = $parent['COLLECTION_POLICY'];
        $collection_policies[$this->parentPid] = new CollectionPolicy($policy_stream->content);
      }
    }
    if (isset($collection_policies[$this->parentPid])) {
      $info = $collection_policies[$this->parentPid]->getContentModels();
      $applicable = array_intersect_key($info, drupal_map_assoc($item->getContentModels()));
      if ($applicable) {
        $first_applicable = reset($applicable);
        $namespace = $first_applicable['namespace'];
      }
    }

    return $namespace;
  }

  /**
   * Inherited.
   */
  public function preprocess() {
    $preprocessed = array();
    $item_class = $this->itemClass;

    $total = $this->getNumber();
    for ($i = 0; $i < $total; $i++) {
      $item = $item_class::getOne($this->file);
      if ($item) {
        $this->parameters['namespace'] = $this->getNamespace($item);
        $wrapper = $item->getWrapperClass();
        $preprocessed[] = $object = new $wrapper($this->connection, $item, $this->parameters);
        $object->addRelationships();
        $this->addToDatabase($object, $object->getResources());
      }
    }

    return $preprocessed;
  }

  /**
   * Indicate if this batch should be ingested immediately after preprocessing.
   *
   * @return bool
   *   Defaults to TRUE to maintain legacy functionality, but individual
   *   importers may expose as an option to delay the ingest. This is desirable
   *   to have ingests run in times when the server is idle.
   */
  public function ingestImmediately() {
    return TRUE;
  }
}

interface IslandoraImportObjectInterface {
  /**
   * Get an item from the source.
   *
   * Extract one item from the source (modifying the source such that in no
   * longer contains the one being extracted).  Return an instantiation of
   * this class representing the one extracted from the source.
   *
   * Return NULL if there's none to get.
   */
  public static function getOne(&$source);
}

/**
 * A base object (MODS + DC) which can write itself out to Fedora.
 *
 * Might be a good idea to make use of static caching (using static variable)
 * within getter methods.
 *
 * @author adam@discoverygarden.ca
 */
abstract class IslandoraImportObject implements IslandoraImportObjectInterface {
  public $pidNamespace = 'ir';
  public $pidCacheSize = 2;

  /**
   * Allow subclasses to use different content models.
   * @var string
   */
  protected $contentModel = 'ir:citationCModel';

  /**
   * Used to store the source data in the default implementation.
   * @var mixed
   */
  protected $source;

  /**
   * Used for lazy-loading storage.
   * @var string|null
   */
  protected $dc = NULL;
  protected $title = NULL;

  /**
   * Private constructor.
   *
   * Instantiations should be obtained via the "getOne" static method, which
   * should know how to extract and feed what is needed to the constructor.
   *
   * Default implementation just saves the source.
   */
  protected function __construct($source) {
    $this->source = $source;
  }

  /**
   * Get the source text.
   */
  public function getSource() {
    return $this->source;
  }

  /**
   * Get the title.
   *
   * By default, should just get the first titleInfo/title out of the generated
   * MODS.
   *
   * @return string
   *   A string containing the title (eventually used as a label).
   */
  public function getTitle() {
    if ($this->title === NULL) {
      $mods = $this->getMODS();
      if ($mods) {
        $mods_doc = new DOMDocument();
        $mods_doc->loadXML($mods);
        $mods_xpath = new DOMXPath($mods_doc);
        $mods_xpath->registerNamespace('m', 'http://www.loc.gov/mods/v3');

        $this->title = $mods_xpath->evaluate('string(//m:mods/m:titleInfo/m:title/text())');
      }
    }

    return $this->title;
  }

  /**
   * Create some MODS representing us.
   *
   * Don't really care how it is obtained, as long as it's a string containing
   * MODS XML.  Statically caching your result may be a good idea, to avoid
   * regenerating it.
   *
   * @return string
   *   A string containing MODS XML.
   */
  public abstract function getMODS();

  /**
   * Create some DC representing us.
   *
   * Default implementation transforms MODS.
   *
   * @return string
   *   A string containing DC XML.
   */
  public function getDC() {
    if ($this->dc === NULL) {
      $new_dc = static::runXSLTransform(array(
                  'xsl' => drupal_get_path('module', 'islandora_importer') . '/xsl/mods_to_dc.xsl',
                  'input' => $this->getMODS(),
                ));
      if ($new_dc) {
        $this->dc = $new_dc;
      }
    }

    return $this->dc;
  }

  /**
   * Acquire base datastreams structure.
   *
   * Default implementation gets MODS and DC as inline-XML datastreams.
   *
   * @param array $errors
   *   A reference to an array of arrays describing errors which may occur,
   *   used to generate a watchdog entries and a simple HTML log which can be
   *   downloaded after the batch process completes.  Each contained array can
   *   contain four elements:
   *   - A translated string
   * @param array $files
   *   A reference to an array of Drupal file objects, which will be deleted
   *   after the ingest process completes (so temporary files can be dealt
   *   with). (Just seems to need ->uri?)
   *
   * @return array
   *   The array of datastreams as used by
   *   islandora_prepare_new_object().
   */
  protected function getDatastreams(&$errors = NULL, &$files = NULL) {
    $datastreams = array();

    // Ingest the MODS.
    $obtained_mods = $this->getMODS();
    if ($obtained_mods) {
      $name = file_create_filename('MODS record.xml', 'public://');
      $files[] = $file = file_save_data($obtained_mods, $name);
      $datastreams[] = array(
        'dsid' => 'MODS',
        'label' => 'MODS Record',
        'mimetype' => 'application/xml',
        'control_group' => 'M',
        'datastream_file' => file_create_url($file->uri),
        'file' => $file,
      );
    }
    else {
      $errors[] = array(
        t('Failed to produce MODS record for @pid.'),
      );
    }

    // Ingest the DC second.
    $obtained_dc = $this->getDC();
    if ($obtained_dc) {
      $name = file_create_filename('DC record.xml', 'public://');
      $files[] = $file = file_save_data($obtained_dc, $name);
      $datastreams[] = array(
        'dsid' => 'DC',
        'label' => 'DC Record',
        'mimetype' => 'application/xml',
        'control_group' => 'X',
        'datastream_file' => file_create_url($file->uri),
        'file' => $file,
      );
    }
    else {
      $errors[] = array(
        t('Failed to produce DC record for @pid.'),
      );
    }

    return $datastreams;
  }

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
  protected static function runXSLTransform($info) {
    $xsl = new DOMDocument();
    $xsl->load($info['xsl']);

    $input = new DOMDocument();
    $input->loadXML($info['input']);

    $processor = new XSLTProcessor();
    $processor->importStylesheet($xsl);

    return $processor->transformToXML($input);
  }

  /**
   * Accessor for content models.
   */
  public function getContentModels() {
    return (array) $this->contentModel;
  }

  /**
   * Expose results of getDatastreams without breaking interface.
   */
  final public function getDatastreamInfo() {
    $errors = array();
    $files = array();
    $datastreams = $this->getDatastreams($errors, $files);
    return array(
      $datastreams,
      $errors,
      $files,
    );
  }

  /**
   * Add relationships to the given object being created.
   */
  public function modifyRelationships(AbstractObject $object) {
    $object->models = $this->getContentModels();
  }

  /**
   * Get the name of the class which wraps this.
   *
   * Should be a subclass of IslandoraImporterBatchObject.
   *
   * @return string
   *   The name of class.
   */
  public function getWrapperClass() {
    return 'IslandoraImporterBatchObject';
  }
}

class IslandoraImporterBatchObject extends IslandoraBatchObject {
  protected $importerObjectInstance = NULL;
  protected $preprocessorParameters = array();

  /**
   * Constructor.
   */
  public function __construct(IslandoraTuque $connection, IslandoraImportObject $object, $preprocessor_parameters = array()) {
    parent::__construct(NULL, $connection->repository);
    $this->importerObjectInstance = $object;
    $this->preprocessorParameters = $preprocessor_parameters;
  }

  /**
   * Inherited.
   */
  public function batchProcess() {
    $this->label = $this->importerObjectInstance->getTitle();
    list($datastreams, $errors, $files) = $this->importerObjectInstance->getDatastreamInfo();
    foreach ($files as $file) {
      $file->status &= ~FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'islandora_batch', 'islandora_batch_object', $this->getBatchId());
    }
    foreach ($datastreams as $ds) {
      $ds_uri = isset($ds['file']) ?
        $ds['file']->uri :
        $ds['datastream_file'];

      $datastream = $this->constructDatastream($ds['dsid'], $ds['control_group']);
      $datastream->label = $ds['label'];
      $datastream->mimetype = $ds['mimetype'];
      $datastream->setContentFromFile($ds_uri);
      $this->ingestDatastream($datastream);
    }
    return ISLANDORA_BATCH_STATE__DONE;
  }

  /**
   * Inherited.
   */
  public function getResources() {
    return array();
  }

  /**
   * Inherited.
   */
  public function addRelationships() {
    $this->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $this->preprocessorParameters['parent_pid']);
    $this->importerObjectInstance->modifyRelationships($this);

    if (module_exists('islandora_xacml_editor')) {
      $parent = islandora_object_load($this->preprocessorParameters['parent_pid']);
      if ($parent) {
        islandora_xacml_editor_apply_parent_policy($this, $parent);
      }
    }
  }
}
