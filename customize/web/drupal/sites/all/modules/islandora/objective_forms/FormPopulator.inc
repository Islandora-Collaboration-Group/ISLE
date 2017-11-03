<?php

/**
 * @file
 * Defines a class that can be used to dynamically populate the default values
 * of a Drupal Form, from the submitted POST data.
 */

module_load_include('inc', 'objective_forms', 'FormValues');

/**
 * Used to populate a Drupal Form with values submitted as POST data.
 */
class FormPopulator {

  /**
   * The submitted POST data.
   *
   * @var FormValues
   */
  protected $values;

  /**
   * Instantiates a FormPopulator.
   *
   * @param FormValues $values
   *   The submitted POST data.
   */
  public function __construct(FormValues $values) {
    $this->values = $values;
  }

  /**
   * Function populate.
   *
   * Populates a Drupal Form's elements #default_value properties with POST
   * data.
   *
   * @param array $form
   *   The Drupal Form to populate
   *
   * @return array
   *   The populated Drupal Form.
   */
  public function populate(array &$form) {
    $children = element_children($form);
    foreach ($children as $key) {
      $child = &$form[$key];
      $default_value = isset($child['#hash']) ? $this->values->getValue($child['#hash']) : NULL;
      // We must check that it is set so that newly created elements aren't
      // overwritten.
      if (isset($default_value)) {
        $child['#default_value'] = $default_value;
      }
      // Recurse...
      $this->populate($child);
    }
    return $form;
  }

}
