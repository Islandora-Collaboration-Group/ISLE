<?php
/**
 * @file
 * Defines the compound object ZIP importer.
 */

/**
 * ZIP file importer for compound objects.
 */
class IslandoraCompoundObjectZipBatchImporter extends ZipBatchImporter {
  protected $itemClass = 'IslandoraCompoundObjectZipBatchImportObject';

  /**
   * Get the form for this importer.
   *
   * @see IslandoraBatchImporter::getForm()
   */
  public static function getForm(array &$form_state) {
    module_load_include('inc', 'islandora', 'includes/utilities');

    $cm_options = array();
    $content_models = islandora_get_content_models();
    foreach ($content_models as $values) {
      $cm_options[$values['pid']] = array('title' => $values['label']);
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
          '#type' => 'textfield',
          '#title' => t('Object Namespace'),
          '#description' => t('The namespace in which the imported objects will be created.'),
          '#default_value' => islandora_get_namespace($form_state['storage']['parent_pid']),
        ),
        'immediate_ingest' => array(
          '#type' => 'checkbox',
          '#title' => t('Ingest immediately?'),
          '#default_value' => TRUE,
        ),
        'submit' => array(
          '#type' => 'submit',
          '#value' => t('Import'),
        ),
      ),
    );
  }

  /**
   * Get the info for the source parameter for the itemClass factory.
   *
   * @see IslandoraBatchImporterInterface::getBatchInfo()
   */
  public static function getBatchInfo(array &$form_state) {
    module_load_include('inc', 'islandora_compound_object', 'includes/manage.form');
    // Load the file.
    $file = file_load($form_state['values']['file']);

    // Grab the content models.
    // Cast to an array, in case there are none?
    $content_models = array_filter((array) $form_state['values']['content_model']);
    $parent_pid = $form_state['storage']['parent_pid'];
    $children = islandora_compound_object_get_parts($parent_pid, TRUE);
    $insert_seq = islandora_compound_object_get_insert_sequence_number($children);
    $info = array(
      'file' => $file,
      'pid_namespace' => $form_state['values']['namespace'],
      'content_model' => $content_models,
      'object_info' => static::getIngestInfo($file),
      'processed_objects' => array(),
      'sequence_number' => $insert_seq,
      'parent_pid' => $parent_pid,
      'immediate_ingest' => $form_state['values']['immediate_ingest'],
    );
    return $info;
  }

  /**
   * Helper function for getting info on the zip file.
   *
   * @param object $file
   *   The file object representing the ZIP to be processed.
   *
   * @return array
   *   The ingest info.
   */
  protected static function getIngestInfo($file) {
    $info = parent::getIngestInfo($file);
    ksort($info);
    return $info;
  }

  /**
   * Inherited.
   */
  public function ingestImmediately() {
    return $this->parameters['file']['immediate_ingest'];
  }
}

/**
 * Compound object ZIP import object.
 */
class IslandoraCompoundObjectZipBatchImportObject extends ZipBatchImportObject  {

  /**
   * Get an item from the source.
   *
   * @see IslandoraImportObjectInterface::getOne()
   */
  public static function getOne(&$info) {
    $info['sequence_number'] = $info['sequence_number'] + 1;
    $record = array(
      'pid_namespace' => $info['pid_namespace'],
      'file' => $info['file'],
      'content_model' => $info['content_model'],
      'sequence_number' => $info['sequence_number'],
      'parent_pid' => $info['parent_pid'],
    );
    $key = key($info['object_info']);
    $record['object_info'] = $info['processed_objects'][$key] = $info['object_info'][$key];
    unset($info['object_info'][$key]);
    return (empty($record) ? FALSE : new static($record));
  }

  /**
   * Overrides the relationships to add compound specific ones.
   *
   * @see IslandoraImportObjectInterface::modifyRelationships()
   */
  public function modifyRelationships(AbstractObject $object) {
    parent::modifyRelationships($object);
    $object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
    $rels_predicate = variable_get('islandora_compound_object_relationship', 'isConstituentOf');
    $parent_pid = $this->source['parent_pid'];
    $escaped_pid = str_replace(':', '_', $parent_pid);
    $object->relationships->add(FEDORA_RELS_EXT_URI, $rels_predicate, $parent_pid);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, "isSequenceNumberOf$escaped_pid", $this->source['sequence_number'], RELS_TYPE_PLAIN_LITERAL);
  }
}
