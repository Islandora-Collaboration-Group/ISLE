<?php

/**
 * @file
 * Defines an Interface for converting FormElement properties dynamically into
 * Object and back into Scalar Values.
 */

/**
 * An interface for FormProperties that want to be treated as objects.
 */
interface FormPropertyInterface {

  /**
   * Function __constructor.
   *
   * Creates the FormProperty object from its scalar or array Drupal Form
   * Property value.
   *
   * @param mixed $value
   *   The value
   */
  public function __construct($value);

  /**
   * Converts this object to a Drupal Form Property value.
   *
   * @return mixed
   *   Returns a scalar or array repersentation of this class, which can be used
   *   within a Drupal Form.
   */
  public function toDrupalForm();
}
