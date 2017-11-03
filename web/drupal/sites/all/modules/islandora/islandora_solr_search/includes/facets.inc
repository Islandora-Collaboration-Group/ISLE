<?php

/**
 * @file
 * Includes functions for advanced facet functionality.
 */

/**
 * Date filter form.
 */
function islandora_solr_date_filter_form($form, &$form_state, $elements) {
  global $_islandora_solr_queryclass;
  extract($elements);

  $form = array(
    '#tree' => TRUE,
    '#prefix' => '<div class="islandora-solr-date-filter">',
    '#suffix' => '</div>',
  );
  // Field.
  $form['date_filter_facet_field'] = array(
    '#type' => 'hidden',
    '#value' => $facet_field,
    '#name' => 'date_filter_facet_field_' . $form_key,
  );

  // Check if default value is possible.
  // Parameters set in URL.
  $params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();
  $filter_count = 0;
  if (isset($params['f'])) {
    $format = 'Y/m/d';
    foreach ($params['f'] as $key => $filter) {
      if (strpos($filter, $facet_field) === 0) {
        $filter_count++;
        // Split the filter into field and value.
        $filter_split = preg_split(ISLANDORA_SOLR_QUERY_FIELD_VALUE_SPLIT_REGEX, $filter, 2);
        // Trim brackets.
        $filter_split[1] = trim($filter_split[1], "\"");
        // Split range filter string to return formatted date values.
        $filter_str = $filter_split[1];

        $filter_str = trim($filter_str, '[');
        $filter_str = trim($filter_str, ']');
        $filter_array = explode(' TO ', $filter_str);

        // Get timestamps.
        $from_unix = strtotime(trim($filter_array[0]));
        $to_unix = strtotime(trim($filter_array[1]));

        // Only set default times if from date is lower than to date.
        if ($from_unix < $to_unix) {
          if ($from_unix !== FALSE) {
            $from_default = (strpos($filter_array[0], '*') !== FALSE) ? '*' : format_date($from_unix, 'custom', $format, 'UTC');
          }
          else {
            $from_default = NULL;
          }
          if ($to_unix !== FALSE) {
            $to_default = (strpos($filter_array[1], '*') !== FALSE) ? '*' : format_date($to_unix, 'custom', $format, 'UTC');
          }
          else {
            $to_default = NULL;
          }
        }
        else {
          $from_default = NULL;
          $to_default = NULL;
        }
      }
    }
  }
  if ($filter_count != 1) {
    $from_default = NULL;
    $to_default = NULL;
  }

  if ($from_default != NULL || $to_default != NULL) {
    $class = 'date-range-expanded';
    $value = t('Hide');
  }
  else {
    $class = 'date-range-collapsed';
    $value = t('Show');
  }

  $form['date_range_expand'] = array(
    '#markup' => t('Specify date range: <a href="#" class="toggle-date-range-filter @class">@value</a>', array('@class' => $class, '@value' => $value)),
    '#prefix' => '<span class="date-filter-toggle-text">',
    '#suffix' => '</span>',
  );
  $form['date_filter'] = array(
    '#prefix' => '<div class="date-range-filter-wrapper">',
    '#suffix' => '</div>',
  );
  $form['date_filter']['date_filter'] = array(
    '#markup' => '<div class="description">' . t('Format: @date', array('@date' => date("Y/m/d"))) . '</div>',
  );
  $form['date_filter']['date_filter_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#default_value' => ($from_default) ? $from_default : '',
    '#size' => 10,
    '#maxsize' => 10,
    '#attributes' => array('class' => array('islandora-solr-datepicker-' . $form_key)),
  );
  $form['date_filter']['date_filter_to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => ($to_default) ? $to_default : '',
    '#size' => 10,
    '#maxsize' => 10,
    '#attributes' => array('class' => array('islandora-solr-datepicker-' . $form_key)),
  );
  $form['date_filter']['date_filter_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#name' => 'date_filter_facet_field_' . $form_key,
    '#form_key' => $form_key,
  );
  $form['#submit'][] = 'islandora_solr_date_filter_form_submit';
  $form['#validate'][] = 'islandora_solr_date_filter_form_validate';

  $form['#attached'] = array(
    'library' => array(
      array('system', 'ui.datepicker'),
    ),
    'css' => array(
      'misc/ui/jquery.ui.datepicker.css',
    ),
    'js' => array(
      array(
        'type' => 'setting',
        'data' => array(
          'islandoraSolrDatepickerRange' => array(
            $facet_field => array(
              'datepickerRange' => trim($datepicker_range),
              'formKey' => $form_key,
            ),
          ),
        ),
      ),
    ),
  );

  return $form;
}

/**
 * Date filter form validate.
 */
function islandora_solr_date_filter_form_validate($form, &$form_state) {
  $from = explode('/', $form_state['values']['date_filter']['date_filter_from']);
  $to = explode('/', $form_state['values']['date_filter']['date_filter_to']);
  $form_key = $form_state['triggering_element']['#form_key'];
  // Default for month and day.
  $default = '01';

  // If the 'from' value is '*' just skip all checks.
  if (trim($from[0]) != '*') {
    // Apply some defaults.
    if (!isset($from[1])) {
      $from[1] = $default;
    }
    if (!isset($from[2])) {
      $from[2] = $default;
    }

    // Check from date.
    if (!checkdate(intval($from[1]), intval($from[2]), intval($from[0]))) {
      form_set_error($form_key . '][date_filter_from', t('<em>From</em> date is not formatted correctly.'));
    }
  }
  // If the 'to' value is '*' just skip all checks.
  if (trim($to[0]) != '*') {
    // Apply some defaults.
    if (!isset($to[1])) {
      $to[1] = $default;
    }
    if (!isset($to[2])) {
      $to[2] = $default;
    }
    // Check to date.
    if (!checkdate(intval($to[1]), intval($to[2]), intval($to[0]))) {
      form_set_error($form_key . '][date_filter_to', t('<em>To</em> date is not formatted correctly.'));
    }
  }
}

/**
 * Date filter form submit.
 */
function islandora_solr_date_filter_form_submit($form, &$form_state) {
  global $_islandora_solr_queryclass;

  $params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();
  $path = current_path();
  $facet_field = $form_state['values']['date_filter_facet_field'];
  $form_key = $form_state['triggering_element']['#form_key'];

  // Date.
  $from = explode('/', $form_state['values']['date_filter']['date_filter_from']);
  $to = explode('/', $form_state['values']['date_filter']['date_filter_to']);

  $build_date = function (array $date_values) {
    if (trim($date_values[0]) != '*') {
      // Apply some defaults.
      $default = '01';
      if (!isset($date_values[1])) {
        $date_values[1] = $default;
      }
      if (!isset($date_values[2])) {
        $date_values[2] = $default;
      }
      // Create date string.
      return format_string('!year-!month-!dayT00:00:00Z', array_combine(
        array('!year', '!month', '!day'),
        $date_values
      ));
    }
    else {
      return $date_values[0];
    }
  };

  $from_str = $build_date($from);
  $to_str = $build_date($to);

  // Create filter.
  $filter = "{$facet_field}:[{$from_str} TO {$to_str}]";

  // Set date filter key if there are no date filters included.
  if (isset($params['f'])) {
    foreach ($params['f'] as $key => $f) {
      if (strpos($f, $facet_field) !== FALSE) {
        array_splice($params['f'], $key);
      }
    }
    $params['f'][] = $filter;
    $query = $params;
  }
  else {
    $query = array_merge_recursive($params, array('f' => array($filter)));
  }

  $form_state['redirect'] = array($path, array('query' => $query));
}

/**
 * Range slider form.
 */
function islandora_solr_range_slider_form($form, &$form_state, $elements) {
  global $_islandora_solr_queryclass;
  extract($elements);
  $from_default = current($data);
  $to_default = end($data);
  if (!empty($gap)) {
    $gap = "({$gap})";
  }

  $form = array(
    '#tree' => TRUE,
    '#prefix' => '<div class="islandora-solr-range-slider">',
    '#suffix' => '</div>',
  );
  // Field.
  $form['range_slider_term'] = array(
    '#type' => 'hidden',
    '#value' => $facet_field,
    '#name' => 'range_slider_term_' . $form_key,
  );
  $slider_variables = array(
    'form_key' => $form_key,
    'gap' => $gap,
    'range_from' => format_date(strtotime(trim($from_default['date'])) + 1, 'custom', $date_format, 'UTC'),
    'range_to' => format_date(strtotime(trim($to_default['date'])), 'custom', $date_format, 'UTC'),
  );
  $form['markup'] = array(
    '#markup' => theme('islandora_solr_range_slider', $slider_variables),
  );
  // Hidden from.
  $form['range_slider_hidden_from'] = array(
    '#type' => 'hidden',
    '#default_value' => $from_default['date'],
    '#attributes' => array('class' => array('range-slider-hidden-from-' . $form_key)),
  );
  // Hidden to.
  $form['range_slider_hidden_to'] = array(
    '#type' => 'hidden',
    '#default_value' => $to_default['date'],
    '#attributes' => array('class' => array('range-slider-hidden-to-' . $form_key)),
  );
  $form['range_slider_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#name' => 'range_slider_submit_' . $form_key,
    '#form_key' => $form_key,
  );
  $form['#submit'][] = 'islandora_solr_range_slider_form_submit';

  // Include flot.
  // @TODO: use the libraries module for this!
  // @TODO: use the new version of flot. Didn't work out of the box, so needs
  // some extra attention.
  // @TODO: find a good way to call this for IE8 and lower
  // @code
  // <!--[if lte IE 8]>
  // $mod_path = drupal_get_path('module', 'islandora_solr')
  // drupal_add_js($mod_path . '/js/flot/excanvas.min.js')
  // <![endif]-->
  // @endcode

  $mod_path = drupal_get_path('module', 'islandora_solr');
  $form['#attached'] = array(
    'library' => array(
      array('system', 'ui.slider'),
    ),
    'css' => array(
      'misc/ui/jquery.ui.slider.css',
    ),
    'js' => array(
      array(
        'type' => 'setting',
        'data' => array(
          'islandoraSolrRangeSlider' => array(
            $facet_field => array(
              'facet_field' => $facet_field,
              'form_key' => $form_key,
              'data' => $data,
              'slider_color' => $slider_color,
            ),
          ),
        ),
      ),
      "$mod_path/js/flot/jquery.flot.min.js",
      "$mod_path/js/islandora_solr_facets.js",
    ),
  );

  return $form;
}

/**
 * Range slider form submit.
 */
function islandora_solr_range_slider_form_submit($form, &$form_state) {
  // Set variables.
  global $_islandora_solr_queryclass;
  $params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();
  $path = current_path();
  $form_key = $form_state['triggering_element']['#form_key'];

  $term = $form_state['values']['range_slider_term'];

  // Date.
  $from = $form_state['values']['range_slider_hidden_from'];
  $to = $form_state['values']['range_slider_hidden_to'];

  $filter = "{$term}:[{$from} TO {$to}]";

  // Set date filter key if there are no date filters included.
  if (isset($params['f'])) {
    foreach ($params['f'] as $key => $f) {
      if (strpos($f, $term) !== FALSE) {
        array_splice($params['f'], $key);
      }
    }
    $params['f'][] = $filter;
    $query = $params;
  }
  else {
    $query = array_merge_recursive($params, array('f' => array($filter)));
  }

  $form_state['redirect'] = array($path, array('query' => $query));
}
