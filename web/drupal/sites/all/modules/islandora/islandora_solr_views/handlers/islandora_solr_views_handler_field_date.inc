<?php

/**
 * @file
 * views handler for solr date field.
 *
 * Based on core views views_handler_field_date class.
 */

// @codingStandardsIgnoreStart
// This Whole file is ignored due to classes and methods are not camelcase and it
// being called all over the place. TODO bring up to coding standards
class islandora_solr_views_handler_field_date extends islandora_solr_views_handler_field {

  /**
   * Define new option.
   */
  function option_definition() {
    // Get parent options.
    $options = parent::option_definition();

    // Set defaults.
    $options['date_format'] = array('default' => 'small');
    $options['custom_date_format'] = array('default' => '');

    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {

    $date_formats = array();
    $date_types = system_get_date_types();
    foreach ($date_types as $key => $value) {
      $date_formats[$value['type']] = t('@title format: @date', array(
        '@title' => $value['title'],
        '@date' => format_date(REQUEST_TIME, $value['type']),
      ));
    }

    $form['date_format'] = array(
      '#type' => 'select',
      '#title' => t('Date format'),
      '#options' => $date_formats + array(
        'custom' => t('Custom'),
        'raw time ago' => t('Time ago'),
        'time ago' => t('Time ago (with "ago" appended)'),
        'raw time hence' => t('Time hence'),
        'time hence' => t('Time hence (with "hence" appended)'),
        'raw time span' => t('Time span (future dates have "-" prepended)'),
        'inverse time span' => t('Time span (past dates have "-" prepended)'),
        'time span' => t('Time span (with "ago/hence" appended)'),
      ),
      '#default_value' => isset($this->options['date_format']) ? $this->options['date_format'] : 'small',
    );
    $form['custom_date_format'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom date format'),
      '#description' => t('If "Custom" is selected, see <a href="http://us.php.net/manual/en/function.date.php" target="_blank">the PHP docs</a> for date formats. Otherwise, enter the number of different time units to display, which defaults to 2.'),
      '#default_value' => isset($this->options['custom_date_format']) ? $this->options['custom_date_format'] : '',
      '#dependency' => array(
        'edit-options-date-format' => array(
          'custom',
          'raw time ago',
          'time ago',
          'raw time hence',
          'time hence',
          'raw time span',
          'time span',
          'raw time span',
          'inverse time span',
          'time span')),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render field.
   */
  function render($values) {
    // Include rendering style of parent class.
    parent::render($values);
    $date_values = explode(', ', $this->get_value($values));
    $format = $this->options['date_format'];
    if (in_array($format, array(
      'custom',
      'raw time ago',
      'time ago',
      'raw time hence',
      'time hence',
      'raw time span',
      'time span',
      'raw time span',
      'inverse time span',
      'time span'))) {
      $custom_format = $this->options['custom_date_format'];
    }
    $dates = array();
    foreach ($date_values as $value) {
      $value = strtotime($value);
      if ($value) {
        // Will be positive for a datetime in the past (ago), and negative
        // for a datetime in the future (hence).
        $time_diff = REQUEST_TIME - $value;
        switch ($format) {
          case 'raw time ago':
            $dates[] = format_interval($time_diff, is_numeric($custom_format) ? $custom_format : 2);
            break;

          case 'time ago':
            $dates[] = t('%time ago', array('%time' => format_interval($time_diff, is_numeric($custom_format) ? $custom_format : 2)));
            break;

          case 'raw time hence':
            $dates[] = format_interval(-$time_diff, is_numeric($custom_format) ? $custom_format : 2);
            break;

          case 'time hence':
            $dates[] = t('%time hence', array('%time' => format_interval(-$time_diff, is_numeric($custom_format) ? $custom_format : 2)));
            break;

          case 'raw time span':
            $dates[] = ($time_diff < 0 ? '-' : '') . format_interval(abs($time_diff), is_numeric($custom_format) ? $custom_format : 2);
            break;

          case 'inverse time span':
            $dates[] = ($time_diff > 0 ? '-' : '') . format_interval(abs($time_diff), is_numeric($custom_format) ? $custom_format : 2);
            break;

          case 'time span':
            $substitutions = array(
              '%time' => format_interval(abs($time_diff), is_numeric($custom_format) ?
                $custom_format :
                2),
            );
            if ($time_diff < 0) {
              $dates[] = t('%time hence', $substitutions);
            }
            else {
              $dates[] = t('%time ago', $substitutions);

            }
            break;

          case 'custom':
            if ($custom_format == 'r') {
              $dates[] = format_date($value, $format, $custom_format, NULL, 'en');
            }
            $dates[] = format_date($value, $format, $custom_format);
            break;

          default:
            $dates[] = format_date($value, $format);
            break;
        }
      }
    }
    return implode(', ', $dates);
  }
}
// @codingStandardsIgnoreEnd
