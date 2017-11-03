<?php

/**
 * @file
 * Defines a class used to keep track of all the FormElements created/cloned
 * during the Form rendering/submission process.
 */

module_load_include('inc', 'objective_forms', 'FormElement');
module_load_include('inc', 'objective_forms', 'FormStorage');

/**
 * A class that holds a flat array of all FormElements created/cloned during
 * the Form rendering/submission process. Also the history of cloned form
 * elements are tracked so that the original FormElement can be retrieved.
 */
class FormElementRegistry {

  /**
   * A flat array of all elements, where the key is the FormElement's hash.
   *
   * @var array
   */
  protected $elements;

  /**
   * The cloned history of a FormElement. Where the key is the FormElement's
   * hash and its value is an array. The array values are the hashes of the
   * FormElements that were cloned in order to clone this element.
   * They are stored in the sequential order in which they were cloned.
   *
   * @var array
   */
  protected $cloned;

  /**
   * Instantiate the FormElementRegistry.
   */
  public function __construct() {
    $this->elements = array();
    $this->cloned = array();
  }

  /**
   * Registers the newly created FormElement.
   *
   * @param FormElement $created
   *   The FormElement to register.
   */
  public function registerCreated(FormElement $created) {
    $this->elements[$created->hash] = $created;
  }

  /**
   * Registers the newly cloned FormElement and logs the clone's ancestory.
   *
   * @param hash $original
   *   The hash of the FormElement the clone was created from.
   * @param FormElement $clone
   *   The newly cloned FormElement.
   */
  public function registerClone($original, FormElement $clone) {
    // Clone's also count as created elements.
    $this->registerCreated($clone);
    $this->setAncestory($clone->hash, $original);
  }

  /**
   * Function registerDestroyed.
   *
   * Should this ever get called? Since this class holds references to every
   * object created.
   *
   * @param string $hash
   *   The hash
   */
  public function registerDestroyed($hash) {
    // @todo Implement...
  }

  /**
   * Checks to see if the element identified by hash exists in the registry.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return bool
   *   TRUE if the element is registered FALSE otherwise.
   */
  public function isRegistered($hash) {
    return isset($this->elements[$hash]);
  }

  /**
   * Get a registered FormElement identified by its hash.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return FormElement
   *   The form element if found otherwise NULL;
   */
  public function get($hash) {
    if ($this->isRegistered($hash)) {
      return $this->elements[$hash];
    }
    return NULL;
  }

  /**
   * Function isClone.
   *
   * Checks to see if the form element identified by its hash is a clone of
   * another form element.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return bool
   *   TRUE if the FormElement identified by its hash was cloned from another
   *   FormElement.
   */
  public function isClone($hash) {
    return isset($this->cloned[$hash]);
  }

  /**
   * Function hasAncestor.
   *
   * Checks if a FormElement has a ancestor it was cloned from at the given
   * depth.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @param int $depth
   *   The depth to look into the FormElements cloned ancestory.
   *
   * @return bool
   *   TRUE if this FormElement has an Ancestor at the given depth.
   */
  public function hasAncestor($hash, $depth) {
    if ($this->isClone($hash)) {
      return isset($this->cloned[$hash][$depth]);
    }
    return FALSE;
  }

  /**
   * Get's the ancestory of cloned FormElements for a particular FormElement.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return array
   *   The FormElements ancestory.
   */
  public function getAncestory($hash) {
    if ($this->isClone($hash)) {
      return $this->cloned[$hash];
    }
    return array();
  }

  /**
   * Function setAncestory.
   *
   * Sets the history for a given element and the hash of the element in which
   * it was cloned from.
   *
   * @param hash $clone
   *   The unique #hash property that identifies the cloned FormElement.
   * @param hash $original
   *   The unique #hash property that identifies the FormElement the clone was
   *   created from.
   */
  public function setAncestory($clone, $original) {
    $ancestory = $this->getAncestory($original);
    array_unshift($ancestory, $original);
    $this->cloned[$clone] = $ancestory;
  }

  /**
   * Function getAncestor.
   *
   * Gets the ancestor this FormElement was cloned from at the given depth in
   * its cloned.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the cloned FormElement.
   * @param int $depth
   *   The position in the clone history to look for the ancestor.
   *
   * @return FormElement
   *   The an form element this the form element was cloned from if found,
   *   otherwise NULL.
   */
  public function getAncestor($hash, $depth) {
    if ($this->hasAncestor($hash, $depth)) {
      $ancestor_hash = $this->cloned[$hash][$depth];
      return $this->get($ancestor_hash);
    }
    return NULL;
  }

  /**
   * Gets the original ancestor this form element was cloned from.
   *
   * If the element doesn't have a clone history, return the given form element.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the cloned FormElement.
   *
   * @return FormElement
   *   Either the Original FormElement (the oldest ancestor) or the FormElement
   *   itself if it wasn't a clone.
   */
  public function getOriginal($hash) {
    if ($this->isClone($hash)) {
      $length = count($this->cloned[$hash]);
      $original_hash = $this->cloned[$hash][$length - 1];
      return $this->get($original_hash);
    }
    return $this->get($hash);
  }

  /**
   * Function clearAncestory.
   *
   * Clears the ancestory of the given FormElement, or if no FormElement is
   * provided the ancestory of all FormElements will be clear.
   *
   * @param hash $hash
   *   The unique #hash property of the form element whose history will be
   *   cleared. If NULL all history will be removed.
   */
  public function clearAncestory($hash = NULL) {
    if (isset($hash)) {
      unset($this->cloned[$hash]);
    }
    else {
      $this->cloned = array();
    }
  }

}
