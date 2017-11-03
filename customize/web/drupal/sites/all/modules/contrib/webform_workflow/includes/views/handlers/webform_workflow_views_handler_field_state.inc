<?php

/**
 * @file
 * Views field handler to display a webform submission's workflow state.
 */

class webform_workflow_views_handler_field_state extends views_handler_field {

  /**
   * Overrides parent::init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['color'] = array(
      'table' => 'webform_workflow_state',
      'field' => 'color',
    );
  }

  /**
   * Overrides parent::get_value().
   */
  public function render($values) {
    $label = parent::get_value($values);
    if (!$label) {
      return;
    }
    return theme('webform_workflow_state_color', array(
      'content' => $label,
      'color' => parent::get_value($values, 'color'),
      'classes_array' => array('webform-workflow-state'),
    ));
  }

}
