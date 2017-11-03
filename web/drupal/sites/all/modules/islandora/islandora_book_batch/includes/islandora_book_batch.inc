<?php

/**
 * @file
 * Implementation of classes from the batch ingest framework for books.
 */

/**
 * Batch preprocessor.
 */
class IslandoraBookBatch extends IslandoraScanBatch {
  /**
   * Static function to get object class.
   */
  protected static function getObjectClass() {
    return 'IslandoraBookBookBatchObject';
  }

  /**
   * Parses the file listing into the structure we use.
   *
   * @overrides IslandoraScanBatch::groupFiles()
   */
  protected function groupFiles($listing) {
    $files = array();
    foreach ($listing as $relative_uri => $file_info) {
      $dir_name = dirname($relative_uri);
      $components = explode(DIRECTORY_SEPARATOR, $dir_name);
      if (isset($components[1]) && !is_numeric($components[1])) {
        // Check that the page numbers are numeric.
        throw new Exception('Bad Batch. Page numbers must be numeric, ensure the page directories only contain numeric values. Refer to the module read me https://github.com/Islandora/islandora_book_batch/blob/7.x/README.md');
      }
      $this->reGroup($files, $components, $relative_uri, $file_info);
    }

    // We are not interested in files at the top level:
    unset($files['.']);

    if ($this->parameters['wait_for_metadata']) {
      $metadata_entries = array('MODS', 'DC', '--METADATA--');
      $no_metadata = array();
      foreach ($files as $key => $value) {
        if (count(array_intersect(array_keys($value['.']), $metadata_entries)) === 0) {
          $no_metadata[$key] = $key;
        }
      }
      $files = array_diff_key($files, $no_metadata);
    }

    if ($this->getTargetType() == 'directory' && $this->parameters['directory_dedup']) {
      // Grab all entries starting with the target from the resources table.
      $target = drupal_realpath($this->getTarget());
      $target_length = strlen($target);

      $q = db_select('islandora_batch_resources', 'r')
        ->fields('r')
        ->condition('type', 'book_directory')
        ->condition('resource', $target . '%', 'LIKE');

      // Remove entries from the $files array which already exist in the
      // resources table.
      foreach ($q->execute() as $result) {
        // Grab the relative path of the book resource entry.
        $relative_path = substr($result->resource, $target_length + 1);
        // Unset the entry from the array, if it exists.
        if (isset($files[$relative_path])) {
          unset($files[$relative_path]);
        }
      }
    }
    return $files;
  }

  /**
   * Recursively regroup/parse the listing.
   */
  protected function reGroup(&$files, $components, $relative_uri, $file_info) {
    $first_component = array_shift($components);
    $count_components = count($components);

    if ($first_component === NULL || $first_component == '.') {
      $files['.'][$file_info->name] = $file_info;
    }
    else {
      if (!isset($files[$first_component])) {
        $files[$first_component] = array(
          '.' => array(),
        );
      }
      $this->reGroup($files[$first_component], $components, $relative_uri, $file_info);
    }
  }

  /**
   * Function preprocess.
   */
  public function preprocess() {
    $added = parent::preprocess();

    $object_type = static::getObjectClass();

    foreach ($added as $object) {
      if ($object instanceof $object_type) {
        $children = db_select('islandora_batch_queue', 'q')
          ->fields('q', array('id'))
          ->condition('parent', $object->id);

        // Set pages ready to go.
        db_update('islandora_batch_state')
          ->fields(array(
            'state' => ISLANDORA_BATCH_STATE__READY,
          ))
          ->condition('id', $children, 'IN')
          ->execute();
        // Set book ready to go (after children, that is).
        db_update('islandora_batch_state')
          ->fields(array(
            'state' => ISLANDORA_BATCH_STATE__PENDING_CHILDREN,
          ))
          ->condition('id', $object->id)
          ->execute();
      }
    }

    return $added;
  }
}

/**
 * Each directory is an object. Each contained file is a datastream.
 *
 * Files are used "DSID.ext", and the extension is used to figure out the
 * mimetype for the datastream.
 * @todo Should be moved into islandora_batch.
 */
class IslandoraFlatBatchObject extends IslandoraScanBatchObject {
  /**
   * Function batchProcess.
   */
  public function batchProcess() {
    // Use object_info to create some datastreams.
    $this->label = $this->getTitle();
    $this->getMods();
    $this->getDc();

    // Add other assets...
    // XXX: This should be made into a constant or sommat?
    // Skip MODS and DC (and --METADATA--), 'cause they get handled in other
    // functions.
    $skip = drupal_map_assoc(array('MODS', 'DC', '--METADATA--'));

    foreach (array_diff_key($this->objectInfo['.'], $skip) as $dsid => $obj) {
      $obj_datastream = $this->constructDatastream($dsid);
      // Figure out the mimetype.
      $obj_datastream->mimetype = static::getMime($obj);
      $obj_datastream->label = "$dsid Datastream";
      $obj_datastream->setContentFromFile($obj->uri, FALSE);

      $this->ingestDatastream($obj_datastream);
    }

    return ISLANDORA_BATCH_STATE__DONE;
  }

  /**
   * Function getMods.
   */
  protected function getMods() {
    if (!isset($this['MODS'])) {
      $mods_datastream = $this->constructDatastream('MODS', 'X');
      $mods_datastream->mimetype = 'application/xml';
      $mods_datastream->label = 'MODS Record';

      $dir = drupal_get_path('module', 'islandora_batch');

      if (isset($this->objectInfo['.']['MODS'])) {
        $mods_datastream->setContentFromFile($this->objectInfo['.']['MODS']->uri, FALSE);
      }
      elseif (isset($this->objectInfo['.']['--METADATA--'])) {
        // Detect type of contents, and transform.
        $this->getModsFromGeneralSource($mods_datastream);
      }
      elseif (isset($this->objectInfo['.']['DC'])) {
        $mods_datastream->content = static::runXslTransform(array(
          'input' => file_get_contents($this->objectInfo['.']['DC']->uri),
          'xsl' => $dir . '/transforms/simpleDC2MODS.xsl',
        ));
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
   * Transform from --METADATA--.{mrc,xml} to MODS.
   *
   * @param AbstractDatastream $datastream
   *   The datastream
   */
  protected function getModsFromGeneralSource(AbstractDatastream $datastream) {
    $meta = $this->objectInfo['.']['--METADATA--'];
    $ext = strtolower(pathinfo($meta->filename, PATHINFO_EXTENSION));
    $dir = drupal_get_path('module', 'islandora_batch');
    if ($ext == 'xml') {
      $xml = file_get_contents($meta->uri);

      if ($xml) {
        $s_xml = new SimpleXMLElement($xml);
        // MODS, set.
        if ($s_xml->getName() == 'mods') {
          // XXX: Should probably create a resource entry.
          $datastream->setContentFromFile($meta->uri, FALSE);
        }
        // MARCXML, transform to MODS and set.
        elseif (in_array($s_xml->getName(), array('record', 'collection'))) {
          $datastream->content = static::runXslTransform(array(
            'input' => $xml,
            'xsl' => $dir . '/transforms/MARC21slim2MODS3-4.xsl',
          ));
        }
        // DC, transform to MODS and set.
        elseif ($s_xml->getName() == 'dc') {
          $datastream->content = static::runXslTransform(array(
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
    elseif ($ext == 'mrc') {
      // Generate some MODS from binary MARC.
      // Get the java executable.
      $java = variable_get('islandora_batch_java', 'java');

      $temp_file_marc = tempnam('/tmp', 'binmrc');
      $temp_file_mods = tempnam('/tmp', 'mods');

      // Copy the file, so we're sure it's accessible on the filesystem.
      copy($meta->uri, $temp_file_marc);
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

      // The transformation produces a collection of mods docs (even if there's
      // only one)...  Let's grab the first one.
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
        $datastream->content = $mods->saveXML();
      }
    }
  }
  /**
   * Function batchProcess.
   */
  protected function getDc() {
    if (!isset($this['DC'])) {
      // Create the DC datastream on this object.
      $dc_datastream = $this->constructDatastream('DC', 'X');
      $dc_datastream->mimetype = 'application/xml';
      $dc_datastream->label = 'DC Record';

      if (isset($this->objectInfo['.']['DC'])) {
        $dc_datastream->setContentFromFile($this->objectInfo['.']['DC']->uri, FALSE);
      }
      elseif (isset($this->objectInfo['.']['--METADATA--'])) {
        // If --METADATA-- contains DC directly, use it.
        $this->getDcFromGeneralSource($dc_datastream);
      }

      // Get the DC by transforming from MODS.
      if (!isset($dc_datastream->content)) {
        // XXX: Might want to make this use a file, instead of staging the DC
        // in the database table (inside the object we serialize).
        $new_dc = static::runXslTransform(array(
          'xsl' => drupal_get_path('module', 'islandora_batch') . '/transforms/mods_to_dc.xsl',
          'input' => $this->getMods(),
        ));
        if ($new_dc) {
          $dc_datastream->content = $new_dc;
        }
      }

      if (isset($dc_datastream->content)) {
        $this->ingestDatastream($dc_datastream);
      }
    }

    return $this['DC']->content;
  }

  /**
   * If --METADATA--.xml happens to be DC, grab it...
   *
   * ... otherwise, we'll let the MODS implementation transform it.
   *
   * @param AbstractDatastream $datastream
   *   The datastream to which we should add the metadata.
   */
  protected function getDcFromGeneralSource(AbstractDatastream $datastream) {
    $meta = $this->objectInfo['.']['--METADATA--'];

    if ('xml' == pathinfo($meta->filename, PATHINFO_EXTENSION)) {
      $xml = file_get_contents($meta->uri);

      if ($xml) {
        $s_xml = new SimpleXMLElement($xml);
        if ($s_xml->getName() == 'dc') {
          // XXX: Should probably create a resource entry.
          $datastream->setContentFromFile($meta->uri, FALSE);
        }
      }
    }
  }
}

/**
 * Class IslandoraBookBatchObject.
 */
class IslandoraBookBookBatchObject extends IslandoraFlatBatchObject {
  /**
   * Function class constructor.
   */
  public function __construct(IslandoraTuque $connection, $base_name, $object_info, $pp) {
    parent::__construct($connection, $base_name, $object_info, $pp);

    if ($pp['directory_dedup'] && $pp['type'] == 'directory') {
      $this->resources['book_directory'] = array(
        drupal_realpath($pp['target'] . '/' . $this->baseName),
      );
    }
  }
  /**
   * Function initialBatchState.
   */
  public function initialBatchState() {
    // Not ready until we've added the pages.
    return ISLANDORA_BATCH_STATE__NOT_READY;
  }

  /**
   * Function getChildren.
   */
  public function getChildren(IslandoraTuque $connection) {
    // Create page objects, return in an array.
    $children = array();

    foreach ($this->objectInfo as $sequence => $info) {
      if ($sequence != '.') {
        $children[] = new IslandoraBookPageBatchObject($connection, $this->id, $sequence, $info, $this->preprocessorParameters);
      }
    }

    return $children;
  }

  /**
   * Function addRelationships.
   */
  public function addRelationships() {
    parent::addRelationships();
    module_load_include('inc', 'islandora_paged_content', 'includes/derivatives');
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

    if (isset($this->preprocessorParameters['create_pdfs']) && $this->preprocessorParameters['create_pdfs']) {
      islandora_paged_content_set_pdf_flag($this);
    }
    if (isset($this->preprocessorParameters['aggregate_ocr']) && $this->preprocessorParameters['aggregate_ocr']) {
      islandora_paged_content_set_ocr_flag($this);
    }
    if (isset($this->preprocessorParameters['page_progression']) && $this->preprocessorParameters['page_progression']) {
      islandora_paged_content_set_page_progression($this, $this->preprocessorParameters['page_progression']);
    }
    // Identify the source of this object.
    $this->relationships->add(ISLANDORA_RELS_EXT_URI, 'book-batched', 'true', RELS_TYPE_PLAIN_LITERAL);
    if (isset($this->preprocessorParameters['email_admin']) && $this->preprocessorParameters['email_admin']) {
      $this->relationships->add(ISLANDORA_RELS_EXT_URI, 'email-admin-when-ingested', 'true', RELS_TYPE_PLAIN_LITERAL);
    }
  }
}

/**
 * Class IslandoraBookPageBatchObject.
 */
class IslandoraBookPageBatchObject extends IslandoraFlatBatchObject {
  protected $parentId;
  protected $sequenceNumber;

  /**
   * Function construct.
   */
  public function __construct(IslandoraTuque $connection, $parent_id, $sequence, $object_info, $pp) {

    $label = "$parent_id-$sequence";
    // Change the label of the page to the sequence number if variable is set.
    if (variable_get('islandora_paged_content_page_label', FALSE)) {
      $label = $sequence;
    }

    parent::__construct($connection, $label, $object_info, $pp);

    $this->parentId = $parent_id;
    $this->sequenceNumber = $sequence;
  }

  /**
   * Function getChildren.
   */
  public function getChildren(IslandoraTuque $connection) {
    // Pages should not have children.
    return array();
  }

  /**
   * Function initialBatchState.
   */
  public function initialBatchState() {
    // Not ready.
    return ISLANDORA_BATCH_STATE__NOT_READY;
  }

  /**
   * Function addRelationships.
   */
  public function addRelationships() {
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    // Add relationship to collection.
    $rels_ext = $this->relationships;
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageOf', $this->parentId);
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $this->sequenceNumber, TRUE);
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $this->sequenceNumber, TRUE);
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
    islandora_paged_content_set_relationship($rels_ext, FEDORA_RELS_EXT_URI, 'isMemberOf', $this->parentId);
    // Add content model relationship.
    $this->models = 'islandora:pageCModel';
    if (isset($this->preprocessorParameters['generate_ocr'])) {
      module_load_include('inc', 'islandora_ocr', 'includes/derivatives');
      islandora_ocr_set_generate_ocr_rels_ext_statement($this, $this->preprocessorParameters['generate_ocr']);
    }
  }
}
