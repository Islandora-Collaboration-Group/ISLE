<?php

/**
 * @file
 * Defines a Class for associating DOMNodes with the 'self' FormElement context.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * Registers the DOMNodes that defined the 'self' context of FormElements. A
 * FormElement can have at most one DOMNode representing its 'self' context, but
 * a DOMNode can represent the 'self' context for multiple FormElements.
 */
class NodeRegistry implements Serializable {

  /**
   * A map of FormElements to the DOMNodes that represents its 'self' context.
   *
   * Where the keys of the array are the unique #hash property of a FormElement,
   * and the value is the DOMNode representing the 'self' context of the
   * FormElement.
   *
   * When this object is serialized the registered DOMNodes are converted to
   * XPaths (since we can't serialize resources). Upon un-serializing this
   * object, NodeRegistry::restored() must be called to convert the XPaths back
   * to DOMNodes.
   *
   * Registered DOMNodes should not be orphan. This leads to wrong XPaths that
   * can't be converted back to DOMNodes.
   *
   * @var array
   */
  protected $registered;

  /**
   * TRUE if this object needs to be restored. FALSE otherwise.
   *
   * To restore this object call the NodeRegistry::restored() function.
   *
   * @var bool
   */
  protected $restoredRequired;

  /**
   * Creates an instance of the NodeRegistry.
   */
  public function __construct() {
    $this->registered = array();
    $this->restoredRequired = FALSE;
  }

  /**
   * Serializes this object.
   *
   * The registered DOMNodes are converted into XPaths, since DOMNodes cannot be
   * serialized.
   *
   * @return string
   *   String representing the serialized object.
   */
  public function serialize() {
    $data = array();
    foreach ($this->registered as $key => $node) {
      $data[$key] = get_dom_node_xpath($node);
    }
    return serialize($data);
  }

  /**
   * Un-serializes this object.
   *
   * Note that for this object to be fully functional NodeRegistry::restore()
   * must also be called after un-serialization.
   *
   * @param string $serialized
   *   The serialized string.
   */
  public function unserialize($serialized) {
    $this->registered = unserialize($serialized);
    $this->restoredRequired = TRUE;
  }

  /**
   * Restores the registry.
   *
   * Queries the registered XPaths and replaces them with the actual DOMNodes
   * they represent.
   *
   * @param DOMDocument $document
   *   The DOMDocument to restore the registry for.
   *
   * @throws Exception
   *   When unable to restore DOMNodes from their stored path.
   */
  public function restore(DOMDocument $document) {
    if ($this->restoredRequired) {
      $xpath = new DOMXPath($document);
      foreach ($this->registered as $key => $path) {
        $result = $xpath->query($path);
        if ($result->length == 1) {
          $this->registered[$key] = $result->item(0);
        }
        else {
          throw new Exception('Fatal error: failed to restore DOMNode from stored path. Key: ' . $key . ' Path: ' . $path);
        }
      }
      $this->restoredRequired = FALSE;
    }
  }

  /**
   * Registers a 'self' context relationship between a FormElement and DOMNode.
   *
   * This context is identified by its unique #hash property.
   *
   * @param string $hash
   *   The #hash property of the FormElement to create the registry entry for.
   * @param DOMNode $node
   *   The DOMNode to register with the FormElement.
   *
   * @throws Exception
   *   If NodeRegistry::restoreRequired is flagged as true and this is called.
   */
  public function register($hash, DOMNode $node) {
    if ($this->restoredRequired) {
      throw new Exception("DOMNodes cannot be registered until this object has been restored. Make sure 'NodeRegistry::restore()' has been called after this object has been unserialized.");
    }
    $this->registered[$hash] = $node;
  }

  /**
   * Unregisters a relationship between a FormElement and DOMNode.
   *
   * @param string $hash
   *   The #hash property of the FormElement to remove from the registry.
   *
   * @throws Exception
   *   If NodeRegistry::restoreRequired is flagged as true and this is called.
   */
  public function unregister($hash) {
    if ($this->restoredRequired) {
      throw new Exception("DOMNodes cannot be unregistered until this object has been restored. Make sure 'NodeRegistry::restore()' has been called after this object has been unserialized.");
    }
    unset($this->registered[$hash]);
  }

  /**
   * Checks if a the given FormElement is registered.
   *
   * @param string $hash
   *   The #hash property of the FormElement to confirm registry for.
   *
   * @return bool
   *   TRUE if the element is registered, FALSE otherwise.
   */
  public function isRegistered($hash) {
    return isset($this->registered[$hash]);
  }

  /**
   * Get the registered DOMNode associated with the given FormElement.
   *
   * @param string $hash
   *   The #hash property of the FormElement to get.
   *
   * @throws Exception
   *   If NodeRegistry::restoreRequired is flagged as true and this is called.
   *
   * @return DOMNode
   *   The DOMNode registered to the FormElement with the given #hash property.
   */
  public function get($hash) {
    if ($this->restoredRequired) {
      throw new Exception("DOMNodes cannot be retrieved until this object has been restored. Make sure 'NodeRegistry::restore()' has been called after this object has been unserialized.");
    }
    return $this->isRegistered($hash) ? $this->registered[$hash] : NULL;
  }

  /**
   * Gets all the registered DOMNodes.
   *
   * @throws Exception
   *   If NodeRegistry::restoreRequired is flagged as true and this is called.
   *
   * @return array
   *   An array containing all registered DOMNodes where the array keys are the
   *   FormElement's hashes and the values are the registered DOMNodes.
   */
  public function getRegistered() {
    if ($this->restoredRequired) {
      throw new Exception("DOMNodes cannot be retrieved until this object has been restored. Make sure 'NodeRegistry::restore()' has been called after this object has been unserialized.");
    }
    return $this->registered;
  }

}
