<?php

/**
 * @file
 * Views base handler for field.
 */

// @codingStandardsIgnoreStart
// This Whole file is ignored due to classes and methods are not camelcase and it
// being called all over the place. TODO bring up to coding standards

class islandora_solr_views_handler_field extends views_handler_field {

  /**
   * Class init.
   *
   * @param type $view
   * @param type $options
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    // Don't add the additional fields to groupby.
    if (!empty($this->options['link_to_object'])) {
      $this->additional_fields['PID'] = array('table' => 'islandora_solr', 'field' => 'PID');
    }
  }

  /**
   * Get value.
   *
   * @param type $values
   * @param type $field
   *
   * @return type string
   */
  function get_value($values, $field = NULL) {
    $alias = isset($field) ? $this->aliases[$field] : $this->field_alias;
    if (isset($values->{$alias})) {
      if (is_array($values->{$alias})) {
        $values->{$alias} = array_filter($values->{$alias}, 'trim');
        return implode(", ", $values->{$alias});
      }
      else {
        return $values->{$alias};
      }
    }
  }

  /**
   * Define new option.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_object'] = TRUE;
    return $options;
  }

  /**
   * Define form element for 'link to object' option.
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_object'] = array(
      '#title' => t('Link field to object.'),
      '#description' => t("Enabling this will override any existing links in this field."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_object']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the object.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_object']) && !empty($this->additional_fields['PID'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = 'islandora/object/' . urlencode($this->get_value($values, 'PID'));
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
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

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
    $sort_field = (isset($this->definition['click sort field']) ? $this->definition['click sort field'] : $this->real_field);
    $this->query->add_sort($sort_field, $order);
  }

}
// @codingStandardsIgnoreEnd
