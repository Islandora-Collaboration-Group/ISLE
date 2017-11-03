<?php

/**
 * @file
 * Defines CollectionPolicy class to process the COLLECTION_POLICY datastream.
 * As defined here http://syn.lib.umanitoba.ca/collection_policy.xsd.
 */

/**
 * Collection Policy
 */
class CollectionPolicy {

  /**
   * The Collection Policy xml file.
   *
   * @var DOMDocument
   */
  protected $xml;

  /**
   * Gets an empty CollectionPolicy.
   *
   * @return CollectionPolicy
   *   An empty CollectionPolicy.
   */
  static public function emptyPolicy() {
    $xml = <<<EOT
<?xml version="1.0" encoding="UTF-8"?>
<collection_policy xmlns="http://www.islandora.ca" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" name="" xsi:schemaLocation="http://www.islandora.ca http://syn.lib.umanitoba.ca/collection_policy.xsd">
 <content_models/>
 <search_terms/>
 <staging_area/>
 <relationship>isMemberOfCollection</relationship>
</collection_policy>
EOT;
    return new CollectionPolicy($xml);
  }

  /**
   * Creates a new CollectionPolicy object.
   *
   * @param string $xml
   *   The COLLECTION_POLICY xml in string form.
   */
  public function __construct($xml) {
    $this->xml = new DOMDocument();
    $this->xml->preserveWhiteSpace = FALSE;
    $this->xml->formatOutput = TRUE;
    $this->xml->loadXML($xml);
    $path = drupal_get_path('module', 'islandora_basic_collection');
    if (!$this->xml->schemaValidate("$path/xml/collection_policy.xsd")) {
      throw new InvalidArgumentException('The given collection policy is not valid.');
    }
  }

  /**
   * Gets the name of the Collection Policy.
   *
   * @return string
   *   The name of this Collection Policy.
   */
  public function getName() {
    return $this->xml->getElementsByTagName('collection_policy')->item(0)->getAttribute('name');
  }

  /**
   * Gets the name of the relationship to use for members of this collection.
   *
   * @return string
   *   Gets the value of relationship if present, FALSE otherwise.
   */
  public function getRelationship() {
    return $this->xml->getElementsByTagName('relationship')->item(0)->nodeValue;
  }

  /**
   * Gets a list of content model objects supported by this collection policy.
   *
   * Also includes default values for creating child objects of supported
   * content models.
   *
   * @return array
   *   An array describing each content model allowed in this collection each
   *   populated by the key value pairs:
   *   - pid: The PID with which the content model object.
   *     - pid: The PID with which the content model object.
   *     - name: The default label to use for new child objects of this type.
   *     - namespace: The default namespace for new child objects of this type.
   */
  public function getContentModels() {
    $ret = array();
    $models = $this->xml->getElementsByTagName('content_models')->item(0)->getElementsByTagName('content_model');
    foreach ($models as $model) {
      $pid = $model->getAttribute('pid');
      $matches = array();
      // Clean up the namespace attribute as its wrong in most existing
      // COLLECTION_POLICY datastreams.
      preg_match('/^([^:]*)/', $model->getAttribute('namespace'), $matches);
      $ret[$pid] = array(
        'pid' => $pid,
        'name' => $model->getAttribute('name'),
        'namespace' => $matches[0],
      );
    }
    return $ret;
  }

  /**
   * Add an entry for the given content model to the collection policy.
   */
  public function addContentModel($pid, $name, $namespace) {
    $content_models_element = $this->xml->getElementsByTagName('content_models');
    $content_model_element = $content_models_element->item(0)->getElementsByTagName('content_model');
    $content_model_element = $this->xml->createElement('content_model');
    $content_model_element->setAttribute('name', $name);
    $content_model_element->setAttribute('dsid', variable_get('Islandora_Content_Model_DSID', ''));
    $content_model_element->setAttribute('namespace', $namespace);
    $content_model_element->setAttribute('pid', $pid);
    $content_models_element->item(0)->appendChild($content_model_element);
  }

  /**
   * Remove matching content model entries.
   *
   * @param string|array $candidates
   *   Either a single string containing a content model PID or an array of
   *   content model PIDs to remove.
   *
   * @return int
   *   The number of entries which have been removed.
   */
  public function removeContentModel($candidates) {
    $count = 0;
    foreach ((array) $candidates as $candidate) {
      if (is_string($candidate)) {
        $content_models_element = $this->xml->getElementsByTagName('content_models');
        $models = $content_models_element->item(0)->getElementsByTagName('content_model');
        $found = FALSE;
        for ($i = 0; $found === FALSE && $i < $models->length; $i++) {
          if ($models->item($i)->getAttribute('pid') == $candidate) {
            $found = $models->item($i);
          }
        }

        if ($found !== FALSE && $models->length > 0) {
          $content_models_element->item(0)->removeChild($found);
          $ret = TRUE;
        }
        $count++;
      }
    }

    return $count;
  }

  /**
   * Dump out the XML for this collection policy.
   */
  public function getXML() {
    return $this->xml->saveXML();
  }
}
