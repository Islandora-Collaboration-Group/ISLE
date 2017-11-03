<?php

/**
 * @file
 * Defines a class that allows drupal forms to be built via objects instead of
 * arrays. Note that is still possible to build non functional drupal forms
 * using this class, the same rules apply that would normally apply to a Drupal
 * API
 * Form.
 */

module_load_include('inc', 'objective_forms', 'FormElement');
module_load_include('inc', 'objective_forms', 'FormStorage');
module_load_include('inc', 'objective_forms', 'FormElementRegistry');
module_load_include('inc', 'objective_forms', 'FormPopulator');

/**
 * A Container for all the FormElements that comprise the form.
 */
class Form implements ArrayAccess {

  /**
   * Stores persistent data.
   *
   * @var FormStorage
   */
  public $storage;

  /**
   * Registers every created/cloned FormElement.
   *
   * @var FormElementRegistry
   */
  public $registry;

  /**
   * The root FormElement.
   *
   * @var array
   */
  public $root;

  /**
   * Instantiate a Form.
   *
   * @param array $form
   *   The drupal form.
   * @param array $form_state
   *   The drupal form state.
   */
  public function __construct(array $form, array &$form_state, $parents = array()) {
    $this->storage = new FormStorage($form_state);
    $this->storage->elementRegistry = isset($this->storage->elementRegistry) ?
        $this->storage->elementRegistry :
        new FormElementRegistry();
    $this->registry = $this->storage->elementRegistry;
    $this->root = new FormElement($this->registry, $form, NULL, $parents);
  }

  /**
   * Function ajaxAlter.
   *
   * Apply ajax alters to the form.
   *
   * @var FormElementRegistry
   */
  public function ajaxAlter(array &$form, array &$form_state, array $triggering_element) {
    if (isset($form_state['triggering_element']['#ajax']['params'])) {
      if ($form['#hash'] == $form_state['triggering_element']['#ajax']['params']['target']) {
        $element = $this->findElement($form['#hash']);
        drupal_alter("form_element_{$element->type}_ajax", $element, $form, $form_state);
      }
      foreach (element_children($form) as $child) {
        $this->ajaxAlter($form[$child], $form_state, $triggering_element);
      }
    }
  }

  /**
   * Seaches the form for the given form element.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return FormElement
   *   The element
   */
  public function findElement($hash) {
    return $this->root->findElement($hash);
  }

  /**
   * Function hasElement.
   *
   * Checks to see if the FormElement identified by its unique $hash exists in
   * this form.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return bool
   *   TRUE if the FormElement exists within the form, FALSE otherwise.
   */
  public function hasElement($hash) {
    return $this->findElement($hash) != NULL;
  }

  /**
   * Duplicates a FormElement identified by its unique $hash.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return FormElement
   *   The cloned element if found, NULL is otherwise.
   */
  public function duplicate($hash) {
    $element = $this->registry->get($hash);
    if ($element) {
      return clone $element;
    }
    return NULL;
  }

  /**
   * Function duplicateOriginal.
   *
   * Duplicates a FormElement identified by its unique $hash from its original
   * definition.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return FormElement
   *   The cloned element if the original was found, NULL is otherwise.
   */
  public function duplicateOriginal($hash) {
    $original = $this->registry->getOriginal($hash);
    if ($original) {
      return clone $original;
    }
    return NULL;
  }

  /**
   * Remove the FormElement identified by its unique $hash from this form.
   *
   * @param hash $hash
   *   The unique #hash property that identifies the FormElement.
   *
   * @return FormElement
   *   The FormElement that was removed from this form if found, NULL otherwise.
   */
  public function remove($hash) {
    $element = $this->findElement($hash);
    if (isset($element)) {
      $element->orphan();
    }
    return $element;
  }

  /**
   * Validates the form.
   *
   * @param array $form
   *   The form
   * @param array $form_state
   *   The form state
   */
  public function validate(array &$form, array &$form_state) {
    // Implemented in child classes.
  }

  /**
   * Submits the form.
   *
   * @param array $form
   *   The form
   * @param array $form_state
   *   The form state
   */
  public function submit(array &$form, array &$form_state) {
    // Implemented in child classes.
  }

  /**
   * Outputs the form as an array, which can be used by the Drupal Form API.
   *
   * @return array
   *   returns the form
   */
  public function toArray(array &$form_state) {
    $form = $this->root->toArray();
    if (isset($form_state['values'])) {
      // @todo see if its nesscary to store this varible with the instance.
      module_load_include('inc', 'objective_forms', 'FormPopulator');
      $populator = new FormPopulator(new FormValues($form_state, $form, $this->registry), $form_state);
      $populator->populate($form);
      if (isset($form_state['triggering_element']) && array_key_exists('#ajax', $form_state['triggering_element'])) {
        $this->ajaxAlter($form, $form_state, $form_state['triggering_element']);
      }
    }
    return $form;
  }

  /**
   * Checks if a child element or property identified by $name exists.
   *
   * @param mixed $name
   *   The name
   *
   * @return bool
   *   TRUE if the child element or property exists FALSE otherwise.
   */
  public function __isset($name) {
    return isset($this->root->$name);
  }

  /**
   * Removes a child element or property identified by $name.
   *
   * @param mixed $name
   *   The name
   */
  public function __unset($name) {
    unset($this->root->$name);
  }

  /**
   * Gets a child element or property identified by $offset.
   *
   * @param mixed $name
   *   The name
   */
  public function __get($name) {
    return $this->root->$name;
  }

  /**
   * Add/Set a child element or property identified by $offset, with $value.
   *
   * @param mixed $name
   *   The name
   * @param mixed $value
   *   The value
   */
  public function __set($name, $value) {
    $this->root->$name = $value;
  }

  /**
   * Checks if a child element or property identified by $offset exists.
   *
   * @param mixed $offset
   *   The offset
   *
   * @return bool
   *   TRUE if the child element or property exists FALSE otherwise.
   */
  public function offsetExists($offset) {
    return $this->root->offsetExists($offset);
  }

  /**
   * Gets a child element or property identified by $offset.
   *
   * @param mixed $offset
   *   The offset
   *
   * @return mixed
   *   Gets a child element or property identified by $offset if it exists
   *   NULL otherwise.
   */
  public function offsetGet($offset) {
    return $this->root->offsetGet($offset);
  }

  /**
   * Add/Set a child element or property identified by $offset, with $value.
   *
   * @param mixed $offset
   *   The offset
   * @param mixed $value
   *   The value
   */
  public function offsetSet($offset, $value) {
    $this->root->offsetSet($offset, $value);
  }

  /**
   * Removes a child element or property identified by $offset.
   *
   * @param mixed $offset
   *   The offset
   */
  public function offsetUnset($offset) {
    $this->root->offsetUnset($offset);
  }

  /**
   * Creates a string repersentation of this object.
   *
   * @return string
   *   Returns the root
   */
  // @codingStandardsIgnoreStart
  // This function is invalid; only PHP magic methods should be prefixed with a
  // double underscore
  public function __toString() {
    return (string) $this->root;
  }
  // @codingStandardsIgnoreEnd
}
