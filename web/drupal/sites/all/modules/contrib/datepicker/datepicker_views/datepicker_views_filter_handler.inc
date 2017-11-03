<?php

/**
 * @file
 * A standard Views filter for a single date field, using Date API form selectors and sql handling.
 */

class datepicker_views_filter_handler extends date_views_filter_handler {

  /**
   * Date selection options.
   */
  function widget_options() {
    $options = parent::widget_options();
    $options['datepicker'] = t('Inline datepicker');
    return $options;
  }

}