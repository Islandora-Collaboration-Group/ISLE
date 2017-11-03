<?php

/**
 * @file
 * Defines the FormValueTracker class that is used by only the FormValues class.
 */

module_load_include('inc', 'objective_forms', 'FormElement');
module_load_include('inc', 'objective_forms', 'FormElementRegistry');

/**
 * This class utilizes scope and a reference pointer to track where
 * the current value for a given FormElement. Its used by the Form values
 * class where it is used to retrieve all the values of FormElements.
 */
class FormValueTracker {

  /**
   * Submitted values from the form. A reference to $form_state['values'].
   *
   * @var array
   */
  protected $values;

  /**
   * The form element registry used to get properties for given form elements.
   *
   * @var FormElementRegistry
   */
  protected $registry;

  /**
   * A reference to a position in $values.
   *
   * @var mixed
   */
  protected $current;

  /**
   * TRUE if we are tracking a location in the values array, FALSE if not.
   *
   * @var boolean
   */
  protected $track;

  /**
   * Creates a FormValues instance.
   *
   * @param array $values
   *   Array of values
   */
  public function __construct(array &$values, FormElementRegistry $registry) {
    $this->values = &$values;
    $this->current = &$this->values;
    // Default value is FALSE.
    $this->track = FALSE;
    $this->registry = $registry;
  }

  /**
   * Gets the value for a given FormElement.
   *
   * Tracks the current position in the $values array if applicable.
   *
   * @param array $element
   *   An element in the Drupal Form.
   *
   * @return mixed
   *   Submitted value for the given FormElement if found, NULL otherwise.
   */
  public function getValue(array &$element) {
    if (!isset($element['#hash'])) {
      return NULL;
    }

    $form_element = $this->registry->get($element['#hash']);
    $value = drupal_array_get_nested_value($this->current, $form_element->getParentsArray());
    return is_array($value) ? NULL : $value;
  }
}
