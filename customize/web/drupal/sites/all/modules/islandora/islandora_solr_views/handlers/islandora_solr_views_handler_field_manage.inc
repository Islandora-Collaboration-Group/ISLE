<?php

/**
 * @file
 * Views field handler for a manage link.
 */

// @codingStandardsIgnoreStart
// This Whole file is ignored due to classes and methods are not camelcase and it
// being called all over the place. TODO bring up to coding standards
class islandora_solr_views_handler_field_manage extends islandora_solr_views_handler_field {


  /**
   * Class init.
   *
   * @param type $view
   * @param type $options
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    // add additional field
    $this->additional_fields['PID'] = array('table' => 'islandora_solr', 'field' => 'PID');
  }

  /**
   * Define form element for 'link to object' option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // don't include link to object options
    unset($form['link_to_object']);
  }

  /**
   * Render whatever the data is as a link to the object.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {

    if (!empty($this->additional_fields['PID'])) {
      // Ensure user has access to edit this node.
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'islandora/object/' . $this->get_value($values, 'PID') . '/manage';
      $this->options['alter']['query'] = drupal_get_destination();

      $text = !empty($this->options['text']) ? $this->options['text'] : t('manage');

      return $text;
    }
  }

  /**
   *
   * @param type $values
   *
   * @return type string Rendered value
   */
  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
// @codingStandardsIgnoreEnd
