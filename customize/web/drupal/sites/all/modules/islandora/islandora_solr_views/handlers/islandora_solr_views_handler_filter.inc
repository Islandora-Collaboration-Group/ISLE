<?php

/**
 * @file
 * Views filter handler base.
 */

// @codingStandardsIgnoreStart
// This Whole file is ignored due to classes and methods are not camelcase and it
// being called all over the place. TODO bring up to coding standards
class islandora_solr_views_handler_filter extends views_handler_filter {
  function query() {
    if (!empty($this->value)) {
      $value = $this->value;

      module_load_include('inc', 'islandora_solr', 'includes/utilities');
      $value = islandora_solr_lesser_escape($value);

      $fieldname = $this->field;
      $exclude = isset($this->operator) && $this->operator === '!=';
      if (is_array($value)) {
        $values = array_filter($value);
        // Ensure that some values have been selected.
        if (!empty($values)) {
          $this->query->add_filter($fieldname, '(' . implode('OR', $values) . ')', $this->options['group'], $exclude);
        }
        return;
      }
      $this->query->add_filter($fieldname, $value, $this->options['group'], $exclude);
    }
  }

  /**
   * Provide a simple textfield for equality.
   */
  function value_form(&$form, &$form_state) {
    $which = 'all';
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($this->definition['title']),
      '#default_value' => $this->value,
    );
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }
    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(1)) ? 'value' : 'none';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
      }
    }
    if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
      $form_state['input'][$identifier] = $this->value;
    }

    if ($which == 'all') {
      $form['value'] += array(
        '#dependency' => array($source => $this->operator_values(1)),
      );
    }
  }

  /**
   * Provide default options for exposed filters.
   */
  function expose_options() {
    $this->options['expose']['identifier'] = drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '_', $this->options['id']));
  }

  function operators() {
    $operators = array(
      '=' => array(
        'title' => t('Is equal to'),
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'short' => t('!='),
        'values' => 1,
      ),
    );
    return $operators;
  }

  /**
   * Build strings from the operators() for 'select' options
   */
  function operator_options($which = 'title') {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      $options[$id] = $info[$which];
    }

    return $options;
  }

  function operator_values($values = 1) {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      if (isset($info['values']) && $info['values'] == $values) {
        $options[] = $id;
      }
    }

    return $options;
  }
}
// @codingStandardsIgnoreEnd
