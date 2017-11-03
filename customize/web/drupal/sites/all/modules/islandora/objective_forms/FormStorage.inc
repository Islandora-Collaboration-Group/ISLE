<?php

/**
 * @file
 * Defines a class that is used to persist data for Drupal Forms across
 * requests.
 */

/**
 * Stores data in the $form_state['storage'] for use in building/rendering the
 * Form.
 */
class FormStorage {
  /**
   * The root element where we store all the required info.
   */
  const STORAGE_ROOT = 'objective_form_storage';

  /**
   * A reference to $form_state['storage'][STORAGE_ROOT], this is where all
   * persistant data is kept.
   *
   * @var array
   */
  protected $storage;

  /**
   * Creates the FormStorage Singleton.
   *
   * @param array $form_state
   *   The Drupal Form State.
   */
  public function __construct(array &$form_state) {
    $this->initializeFormState($form_state);
    $this->storage = &$form_state['storage'][self::STORAGE_ROOT];
  }

  /**
   * Creates the storage slot in the Drupal form state.
   *
   * @param array $form_state
   *   The Drupal Form State.
   */
  protected function initializeFormState(array &$form_state) {
    if (empty($form_state['storage'])) {
      $form_state['storage'] = array();
    }
    if (empty($form_state['storage'][self::STORAGE_ROOT])) {
      $form_state['storage'][self::STORAGE_ROOT] = array();
    }
  }

  /**
   * Checks storage for the existance of a variable.
   *
   * @param string $name
   *   The stored variables name.
   *
   * @return bool
   *   TRUE if the variable exists in storage, FALSE otherwise.
   */
  public function __isset($name) {
    return isset($this->storage[$name]);
  }

  /**
   * Removes a variable from storage.
   *
   * @param string $name
   *   The stored variables name.
   */
  public function __unset($name) {
    unset($this->storage[$name]);
  }

  /**
   * Get a value from storage.
   *
   * @param mixed $name
   *   The stored variables name.
   *
   * @return mixed
   *   The stored variables value.
   */
  public function __get($name) {
    if (isset($this->storage[$name])) {
      return $this->storage[$name];
    }
    return NULL;
  }

  /**
   * Store a value.
   *
   * @param mixed $name
   *   The stored variables name.
   * @param mixed $value
   *   The stored variables value.
   */
  public function __set($name, $value) {
    $this->storage[$name] = $value;
  }

}
