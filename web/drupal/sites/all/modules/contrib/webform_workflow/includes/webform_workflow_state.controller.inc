<?php
/**
 * @file
 * Entity controller for a webform workflow state.
 */

class WebformWorkflowStateController extends EntityAPIControllerExportable {

  /**
   * Overrides parent::create().
   */
  public function create(array $values = array()) {
    $default_values = array(
      'wsid' => NULL,
      'uid' => NULL,
      'label' => NULL,
      'color' => NULL,
      'permissions' => array(),
      'created' => NULL,
      'changed' => NULL,
    );
    // Overwrite default values with supplied values.
    $values = array_merge($default_values, $values);
    $values['permissions'] += $this->getPermissionsDefaults();
    return parent::create($values);
  }

  /**
   * Get the defaults for the $state->permissions array.
   *
   * @return array
   */
  protected function getPermissionsDefaults() {
    return array(
      'view' => array(),
      'edit' => array(),
      'from' => array(),
      'to' => array(),
    );
  }

  /**
   * Overrides parent::load().
   */
  public function load($ids = array(), $conditions = array()) {
    $states = parent::load($ids, $conditions);
    $permissions_defaults = $this->getPermissionsDefaults();
    foreach ($states as $state) {
      $state->permissions += $permissions_defaults;
    }
    return $states;
  }

  /**
   * Overrides parent::save().
   */
  public function save($state, DatabaseTransaction $transaction = NULL) {
    if (!isset($state->created)) {
      $state->created = REQUEST_TIME;
    }
    $state->changed = REQUEST_TIME;
    return parent::save($state, $transaction);
  }

}
