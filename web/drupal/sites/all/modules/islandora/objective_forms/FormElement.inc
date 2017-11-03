<?php

/**
 * @file
 * Defines a FormElement class for encapsulating Drupal form elements. Each
 * FormElement class has a unique #hash property that can be used to identify
 * that FormElement.
 */

module_load_include('inc', 'php_lib', 'ReflectionHelpers');
module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');
module_load_include('inc', 'objective_forms', 'Utils');
module_load_include('inc', 'objective_forms', 'FormElementRegistry');

/**
 * Encapsulates drupal form elements.
 */
class FormElement implements ArrayAccess {

  /**
   * Holds references to protected variables. Allows for external access and
   * protected writes. parent -> Stores a reference to this FormElements Parent.
   * NULL if no parent. hash -> A identifier that is unique to this object.
   *
   * @var ReadOnlyProtected
   */
  protected $protected;

  /**
   * Child FormElements, only objects of type FormElement are stored in this
   * array.
   *
   * @var array
   */
  public $children;

  /**
   * Drupal form controls/properties. The values are mixed in this array.
   *
   * All control names are prefixed with '#' for example '#title'.
   *
   * @var array
   */
  public $controls;

  /**
   * Element Registry.
   *
   * @var FormElementRegistry
   */
  protected $registry;


  /**
   * Index of the current element, within its parent.
   *
   * @var string|int
   */
  protected $index;

  /**
   * Holds references to the original form default value for the form
   * element.
   *
   * @var ReadOnlyProtected
   */
  protected $originalDefaultValue;

  /**
   * Instantiate's a FormElement.
   *
   * @param array $form
   *   Drupal form definition.
   */
  public function __construct(FormElementRegistry $registry = NULL /* yuck fix this.. */, array $form = NULL, $index = NULL, $parents = array()) {
    $this->protected = new ReadOnlyProtectedMembers(array(
      'parent' => NULL,
      'hash' => NULL,
      'parentOffset' => $parents,
      'originalDefaultValue' => NULL,
    ));
    $this->registry = $registry;
    $this->controls = array();
    $this->children = array();
    $this->initialize($form);
    $this->hash = isset($this->controls['#hash']) ? $this->controls['#hash'] : spl_object_hash($this);
    // Deals with this case this class is used in a non user context ....
    if ($this->registry) {
      $this->registry->registerCreated($this);
    }
    $this->index = $index;
  }

  /**
   * Initialize this object from a drupal form definition.
   *
   * Takes the controls/properties/children from the drupal form definition, and
   * creates objects repersenting elements and properties where appropriate.
   *
   * @param array $form
   *   Drupal form definition.
   */
  protected function initialize(array &$form = NULL) {
    if (isset($form)) {
      $this->initializeControls($form);
      $this->initializeChildren($form);
    }
  }

  /**
   * Create controls/properties from a drupal form definition.
   *
   * @param array $form
   *   Drupal form definition.
   */
  protected function initializeControls(array &$form) {
    module_load_include('inc', 'objective_forms', 'FormProperty');
    $properties = element_properties($form);
    foreach ($properties as $key) {
      // Objectify the property where appropriate.
      $this->controls[$key] = FormProperty::Expand($key, $form[$key]);
    }
    // Store the form fields default_value for access when populating defaults.
    if (isset($form["#default_value"])) {
      $this->originalDefaultValue = $form["#default_value"];
    }
  }

  /**
   * Create child FormElements from a drupal form definition.
   *
   * @param array $form
   *   Drupal form definition.
   */
  protected function initializeChildren(array &$form) {
    $children = element_children($form);
    foreach ($children as $key) {
      $child = new FormElement($this->registry, $form[$key], $key);
      $this->adopt($child, $key);
    }
  }

  /**
   * Clone this FormElement.
   *
   * Performs a deep clone on controls and children. The cloned element won't
   * have a parent.
   */
  public function __clone() {
    $original_hash = $this->hash;
    $this->protected = clone $this->protected;
    // The parent was unaware of this cloned bastard child of science...
    $this->parent = NULL;
    // Maintain uniqueness.
    $this->hash = spl_object_hash($this);
    // Clone Controls.
    foreach ($this->controls as $key => $control) {
      if (is_array($control)) {
        $this->controls[$key] = array_copy_recursive($control);
      }
      elseif (is_object($control)) {
        $this->controls[$key] = clone $control;
      }
    }
    // Clone Children.
    $children = $this->children;
    $this->children = array();
    // Clone Children.
    foreach ($children as $key => $child) {
      $this->adopt(clone $child, $key);
    }
    // Deals with this case this class is used in a non user context ....
    if ($this->registry) {
      $this->registry->registerClone($original_hash, $this);
    }
    $this->index = NULL;
  }

  /**
   * Search for FormElement identified by it's unique #hash property.
   *
   * Searches this element and all of its children recursively.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return FormElement
   *   The FormElement if found NULL otherwise.
   */
  public function findElement($hash) {
    if ($this->hash == $hash) {
      return $this;
    }
    foreach ($this->children as $child) {
      $element = $child->findElement($hash);
      if ($element) {
        return $element;
      }
    }
    return NULL;
  }

  /**
   * Function flatten.
   *
   * Converts the tree like structure of a FormElement and its children to a
   * flat array.
   *
   * @return array
   *   An array containing this FormElement and all of its children, where its
   *   keys are the FormElements unique #hash properties. For example:
   *   array(#hash=>FormElement)
   */
  public function flatten() {
    $elements = array();
    $elements[$this->hash] = $this;
    foreach ($this->children as $child) {
      $elements = array_merge($elements, $child->flatten());
    }
    return $elements;
  }

  /**
   * Function eachChild.
   *
   * Takes a callback, and calls it repeatedly passing each child FormElement
   * to the callback as the first parameter. Any other parameters passed to this
   * function will be passed to the callback as additional parameters.
   *
   * Unlike eachDecendant() only the immediate children of this FormElement will
   * be used.
   *
   * If the callback returns FALSE, then processing stops and this function
   * exits.
   *
   * @param callback $function
   *   The function to be called repeatedly.
   *
   * @return bool
   *   TRUE if the function was called for all child FormElements, FALSE
   *   otherwise.
   */
  public function eachChild($function) {
    // Remove function name from the argument list.
    $param_arr = array_slice(func_get_args(), 1);
    foreach ($this->children as $child) {
      $args = array_merge(array($child), $param_arr);
      if (call_user_func_array($function, $args) === FALSE) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Function eachControl.
   *
   * Takes a callback, and calls it repeatedly passing each control/property to
   * the callback with the name of the control/property as the first parameter
   * and the value of the control/property as the second paramter. Any other
   * parameters passed to this function will be passed to the callback as
   * additional parameters.
   *
   * If the callback returns FALSE, then processing stops and this function
   * exits.
   *
   * @param callback $function
   *   The function to be called repeatedly.
   *
   * @return bool
   *   TRUE if the function was called for all controls/properties, FALSE
   *   otherwise.
   */
  public function eachControl($function) {
    // Remove function name from the argument list.
    $param_arr = array_slice(func_get_args(), 1);
    foreach ($this->controls as $key => &$control) {
      $args = array_merge(array($key, &$control), $param_arr);
      if (call_user_func_array($function, $args) === FALSE) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Function eachDecendant.
   *
   * Takes a callback, and calls it repeatedly passing each decendant
   * FormElement to the callback as the first parameter. Any other parameters
   * passed to this function will be passed to the callback as additional
   * parameters.
   *
   * Unlike eachChild() even children of children will be used.
   *
   * If the callback returns FALSE, then processing stops and this function
   * exits.
   *
   * @param callback $function
   *   The function to be called repeatedly.
   *
   * @return bool
   *   TRUE if the function was called for all decendants, FALSE otherwise.
   */
  public function eachDecendant($function) {
    $func_args = func_get_args();
    // Remove function name from the argument list.
    $param_arr = array_slice($func_args, 1);
    $func_args = func_get_args();
    foreach ($this->children as $child) {
      $args = array_merge(array($child), $param_arr);
      if (call_user_func_array($function, $args) === FALSE) {
        // End processing.
        return FALSE;
      }
      // Recurse...
      if (call_user_func_array(array($child, 'eachDecendant'), $func_args) === FALSE) {
        // End processing.
        return FALSE;
      }
    }
    // Continue processing.
    return TRUE;
  }

  /**
   * Function each.
   *
   * Takes a callback, and calls it repeatedly passing itself and each decendant
   * FormElement to the callback as the first parameter. Any other parameters
   * passed to this function will be passed to the callback as additional
   * parameters.
   *
   * Unlike eachDecendant() this object is also used.
   *
   * If the callback returns FALSE, then processing stops and this function
   * exits.
   *
   * @param callback $function
   *   The function to be called repeatedly.
   *
   * @return bool
   *   TRUE if the function was called for all decendants, FALSE otherwise.
   */
  public function each($function) {
    // Remove function name from the argument list.
    $param_arr = array_slice(func_get_args(), 1);
    $args = array_merge(array($this), $param_arr);
    if (call_user_func_array($function, $args) === TRUE) {
      return call_user_func_array(array($this, 'eachDecendant'), func_get_args());
    }
    return FALSE;
  }

  /**
   * Function __get.
   *
   * Gets a child FormElement, control/property, or ProtectedReadOnlyMember if
   * it exists.
   *
   * @param mixed $name
   *   The name of the child FormElement, control/property, or
   *   ProtectedReadOnlyMember.
   *
   * @return mixed
   *   The child FormElement, control/property, or ProtectedReadOnlyMember
   *   identified by $name if found, NULL otherwise.
   */
  public function __get($name) {
    if ($this->protected->has($name)) {
      return $this->protected->$name;
    }
    // Control.
    elseif ($this->offsetExists("#$name")) {
      return $this->offsetGet("#$name");
    }
    // Child.
    elseif ($this->offsetExists($name)) {
      return $this->offsetGet($name);
    }
    return NULL;
  }

  /**
   * Function __set.
   *
   * Adds/Sets the value for a child FormElement, control/property, or
   * ProtectedReadOnlyMember.
   *
   * Can't add to the ProtectedReadOnlyMembers.
   *
   * @param mixed $name
   *   The name of the child FormElement, control/property, or
   *   ProtectedReadOnlyMember.
   * @param mixed $value
   *   The the child FormElement, control/property, or ProtectedReadOnlyMember
   *   to set.
   */
  public function __set($name, $value) {
    if ($this->protected->has($name)) {
      $this->protected->$name = $value;
    }
    else {
      $name = is_or_descends_from($value, 'FormElement') ? $name : "#$name";
      $this->offsetSet($name, $value);
    }
  }

  /**
   * Function __isset.
   *
   * Checks if a given child FormElement or a control/property identified by
   * $name exists. Also checks for the existance of values stored in this
   * objects ReadOnlyMembers.
   *
   * @param mixed $name
   *   The name of the child FormElement, control/property, or
   *   ProtectedReadOnlyMember.
   *
   * @return bool
   *   TRUE if the child FormElement, control/property, or
   *   ProtectedReadOnlyMember is set.
   */
  public function __isset($name) {
    if ($this->protected->has($name)) {
      return isset($this->protected->$name);
    }
    return $this->offsetExists($name) || $this->offsetExists("#$name");
  }

  /**
   * Function __unset.
   *
   * Unsets a given child FormElement, control/property, or
   * ProtectedReadOnlyMember identified by $name.
   *
   * @param mixed $name
   *   The name of the child FormElement, control/property, or
   *   ProtectedReadOnlyMember.
   */
  public function __unset($name) {
    if ($this->protected->has($name)) {
      unset($this->protected->$name);
    }
    // Child.
    elseif ($this->offsetExists($name)) {
      $this->offsetUnset($name);
    }
    // Control.
    elseif ($this->offsetExists("#$name")) {
      $this->offsetUnset("#$name");
    }
  }

  /**
   * Function offsetExists.
   *
   * Checks if a given child FormElement or a control/property identified by
   * $offsets exists.
   *
   * @param mixed $offset
   *   The name of the child FormElement or control/property
   *
   * @return bool
   *   TRUE if the child FormElement or control/property exists.
   */
  public function offsetExists($offset) {
    $child_exists = isset($this->children[$offset]);
    $control_exists = isset($this->controls[$offset]);
    return $child_exists || $control_exists;
  }

  /**
   * Function offsetGet.
   *
   * Gets a the child FormElement or a control/property identified by $offset
   * if it exists.
   *
   * @param mixed $offset
   *   The name of the child FormElement or control/property.
   *
   * @return mixed
   *   If found, this function will return either a child FormElement or a
   *   FormControl.
   */
  public function offsetGet($offset) {
    if (array_key_exists($offset, $this->children)) {
      return $this->children[$offset];
    }
    if (array_key_exists($offset, $this->controls)) {
      return $this->controls[$offset];
    }
    return NULL;
  }

  /**
   * Function offsetSet.
   *
   * Adds/Sets a child FormElement or a control/property identified by $offset
   * if it exists.
   *
   * @param mixed $offset
   *   The name of the child FormElement or control/property.
   * @param mixed $value
   *   Either a child FormElement or some mixed value that repersents a
   *   control/property.
   */
  public function offsetSet($offset, $value) {
    if (is_or_descends_from($value, 'FormElement')) {
      $this->adopt($value, $offset);
    }
    elseif (element_property($offset)) {
      $this->controls[$offset] = $value;
    }
  }

  /**
   * Function offsetUnset.
   *
   * Unsets a given child FormElement or control/property identified by $offset.
   *
   * @param mixed $offset
   *   The name of the child FormElement or control/property.
   */
  public function offsetUnset($offset) {
    if (isset($this->children[$offset])) {
      unset($this->children[$offset]);
    }
    elseif (isset($this->controls[$offset])) {
      unset($this->controls[$offset]);
    }
  }

  /**
   * Function adopt.
   *
   * Adopt the child FormElement, this forcefully orphans the child from its
   * original parent.
   *
   * @param FormElement $child
   *   The FormElement to adopt.
   * @param mixed $key
   *   The index where the child FormElement is stored, if NULL the child is
   *   append onto the end of the array.
   */
  public function adopt(FormElement $child, $key = NULL) {
    $child->setParent($this);
    if (isset($key)) {
      if (isset($this->children[$key])) {
        $this->children[$key]->orphan();
      }
      $this->children[$key] = $child;
    }
    else {
      $this->children[] = $child;
    }
  }

  /**
   * Function setParent.
   *
   * Sets this FormElements parent to the one provided. Orphaning this
   * FormElement from its previous parent FormElement.
   *
   * @param FormElement $parent
   *   The new parent FormElement of this FormElement.
   */
  protected function setParent(FormElement $parent) {
    // There can only be one.
    $this->orphan();
    $this->parent = $parent;
  }

  /**
   * Function orphan.
   *
   * Orphans this FormElement, and removes the relationship its parent
   * FormElement has with it.
   *
   * @return bool
   *   TRUE if the element was orphaned FALSE otherwise.
   */
  public function orphan() {
    if (isset($this->parent)) {
      $was_orphaned = $this->parent->orphanChild($this);
      unset($this->parent);
      return $was_orphaned;
    }
    return FALSE;
  }

  /**
   * Function orphanChild.
   *
   * Removes the child element defined by $remove from this FormElement.
   *
   * @param FormElement $remove
   *   The child FormElement to remove.
   *
   * @return bool
   *   TRUE if the child was removed, FALSE otherwise.
   */
  protected function orphanChild(FormElement $remove) {
    foreach ($this->children as $index => $child) {
      if ($remove === $child) {
        unset($this->children[$index]);
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Funciton getIndex.
   *
   * Get's the index of this FormElement in its parent's children array if it
   * has a parent.
   *
   * @return mixed
   *   The index of this FormElement within its parents children array. NULL if
   *   has no parent.
   */
  public function getIndex() {
    if ($this->index === NULL && isset($this->parent)) {
      foreach ($this->parent->children as $index => $child) {
        if ($child === $this) {
          $this->index = $index;
          break;
        }
      }
    }
    return $this->index;
  }

  /**
   * Function getLocation.
   *
   * Gets the full path in the form definition to this element.
   *
   * @return string
   *   Describes the location of this FormElement within the Form.
   *   For example ['author']['name']
   */
  public function getLocation() {
    $index = $this->getIndex();
    return isset($index) ? "{$this->parent->getLocation()}[$index]" : '';
  }

  /**
   * Function toArray.
   *
   * Converts this FormElement into an array that can be used with the Drupal
   * Form API, as a form definition.
   *
   * @return array
   *   array of children
   */
  public function toArray() {
    $output = array('#hash' => $this->hash);
    $this->addControlsToArray($output);
    $this->addChildrenToArray($output);
    return $output;
  }

  /**
   * Function addControlsToArray.
   *
   * Converts this FormElements controls/properties to an array that can be
   * used by Drupal Form API.
   *
   * Adds the controls to the $output parameter.
   *
   * @param array &$output
   *   The Drupal Form API's repersentation of a form.
   */
  protected function addControlsToArray(array &$output) {
    foreach ($this->controls as $name => $value) {
      $has_property_interface = has_interface($value, 'FormPropertyInterface');
      $output[$name] = $has_property_interface ? $value->toDrupalForm() : $value;
    }
  }

  /**
   * Function addChildrenToArray.
   *
   * Converts this FormElements child FormElements to an array that can be used
   * by Drupal Form API.
   *
   * @param array &$output
   *   The Drupal Form API's repersentation of a form.
   */
  protected function addChildrenToArray(array &$output) {
    foreach ($this->children as $name => $child) {
      $output[$name] = $child->toArray();
    }
  }

  /**
   * Get array of parents, similar to Drupal's '#parents'.
   *
   * Used to reliably access values posted back from the user.
   *
   * @return array
   *   An array of indicies, including this element's, to use to access the
   *   value for this element in $form_state['values'].
   */
  public function getParentsArray() {
    if ($this->parent === NULL) {
      // Should only be the top-level "form" element.
      return $this->parentOffset;
    }
    elseif (isset($this['#parents'])) {
      // Potential optimization... Unlikely to hit?
      return $this['#parents'];
    }
    else {
      if (isset($this['#tree']) && $this['#tree']) {
        return array_merge($this->parent->getParentsArray(), array($this->getIndex()));
      }
      else {
        return array();
      }
    }
  }

  /**
   * Get the original default form value for the element.
   */
  public function getOriginalDefaultValue() {
    return $this->originalDefaultValue;
  }
}
