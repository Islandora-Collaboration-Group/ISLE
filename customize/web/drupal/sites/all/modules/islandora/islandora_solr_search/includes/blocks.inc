<?php

/**
 * @file
 * Contains all blocks related functionality.
 */

/**
 * Implements hook_islandora_solr_query_blocks().
 */
function islandora_solr_islandora_solr_query_blocks() {
  // Note: As in the examples below, it is valid to specify
  // *either* a class and method *or* a form.  The latter implies no
  // class needs to be instantiated.
  return array(
    'advanced' => array(
      'name' => t('Islandora advanced search'),
      'module' => 'islandora_solr',
      'file' => 'includes/blocks.inc',
      'class' => NULL,
      'function' => NULL,
      'form' => 'islandora_solr_advanced_search_form',
    ),
    'simple' => array(
      'name' => t('Islandora simple search'),
      'module' => 'islandora_solr',
      'file' => 'includes/blocks.inc',
      'class' => NULL,
      'function' => NULL,
      'form' => 'islandora_solr_simple_search_form',
    ),
    'basic_facets' => array(
      'name' => t('Islandora facets'),
      'module' => 'islandora_solr',
      'file' => 'includes/results.inc',
      'class' => 'IslandoraSolrResults',
      'function' => 'displayFacets',
      'form' => NULL,
    ),
    'current_query' => array(
      'name' => t('Islandora query'),
      'module' => 'islandora_solr',
      'file' => 'includes/results.inc',
      'class' => 'IslandoraSolrResults',
      'function' => 'currentQuery',
      'form' => NULL,
    ),
    'display_switch' => array(
      'name' => t('Islandora displays'),
      'module' => 'islandora_solr',
      'file' => 'includes/blocks.inc',
      'class' => NULL,
      'function' => 'islandora_solr_display',
      'form' => NULL,
    ),
    'sort' => array(
      'name' => t('Islandora sort'),
      'module' => 'islandora_solr',
      'file' => 'includes/blocks.inc',
      'class' => NULL,
      'function' => 'islandora_solr_sort',
      'form' => NULL,
    ),
    'explore' => array(
      'name' => t('Islandora explore'),
      'module' => 'islandora_solr',
      'file' => 'includes/explore.inc',
      'class' => NULL,
      'function' => 'islandora_solr_explore_generate_links',
      'form' => NULL,
    ),
    'search_navigation' => array(
      'name' => t('Islandora search navigation'),
      'module' => 'islandora_solr',
      'file' => 'includes/blocks.inc',
      'class' => NULL,
      'function' => 'islandora_solr_search_navigation',
      'form' => NULL,
    ),
    'result_limit' => array(
      'name' => t('Islandora search result limit'),
      'module' => 'islandora_solr',
      'file' => 'includes/blocks.inc',
      'class' => NULL,
      'function' => 'islandora_solr_search_results_limit',
      'form' => NULL,
    ),
  );
}


/**
 * Implements hook_block_info().
 */
function islandora_solr_block_info() {
  $blocks = array();

  $solr_blocks = module_invoke_all("islandora_solr_query_blocks");
  foreach ($solr_blocks as $name => $block) {
    $blocks[$name] = array(
      'info' => $block['name'],
      'cache' => DRUPAL_NO_CACHE,
    );
  }
  return $blocks;
}

/**
 * Implements hook_block_configure().
 */
function islandora_solr_block_configure($delta = '') {
  if ($delta == 'simple') {
    $form = array();
    $form['islandora_solr_simple_search_label_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Simple Search Label'),
      '#size' => 30,
      '#default_value' => variable_get('islandora_solr_simple_search_label_title', 'Search Term'),
      '#description' => t('Customize label text associated with search input form. Accessibility guidelines recommend whenever possible to explicitly associate label text with form inputs.'),
      '#required' => TRUE,
    );
    $form['islandora_solr_simple_search_label_visibility'] = array(
      '#type' => 'checkbox',
      '#title' => t('Simple Search Label Visibility'),
      '#default_value' => variable_get('islandora_solr_simple_search_label_visibility', TRUE),
      '#description' => t('Display the label alongside the simple search input. If unchecked, label will be visibly hidden but will be kept available for screen-readers.'),
    );
    return $form;
  }
}

/**
 * Implements hook_block_save().
 */
function islandora_solr_block_save($delta = '', $edit = array()) {
  if ($delta == 'simple') {
    variable_set('islandora_solr_simple_search_label_title', $edit['islandora_solr_simple_search_label_title']);
    variable_set('islandora_solr_simple_search_label_visibility', $edit['islandora_solr_simple_search_label_visibility']);
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Currently checks for delta of explore and modifies the form to add custom
 * configuration options to setup facet display filters to be rendered.
 */
function islandora_solr_form_BLOCK_ADMIN_CONFIGURE_alter(&$form, &$form_state) {
  if ($form['module']['#value'] == 'islandora_solr' && $form['delta']['#value'] == 'explore') {
    // Get the variables for the form display facets.
    $expore_config = (isset($form_state['islandora_solr_facet_filters']) ? $form_state['islandora_solr_facet_filters'] : variable_get('islandora_solr_explore_config', array()));

    // Check if remove was clicked and removed the label and filter from the
    // values and the table.
    if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == t('Remove selected')) {
      foreach ($form_state['values']['table'] as $key => $row) {
        if (!empty($row)) {
          // Get selected row index.
          $row_index = str_replace("facet-row-", "", $key);
          // Unset the index to keep the other keys the same.
          unset($expore_config[$row_index]);
        }
      }
      $form_state['islandora_solr_facet_filters'] = $expore_config;
    }

    // Check if weights are being updated.
    if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == t('Update weights') && !empty($expore_config)) {
      // Note: select_weight is only in the $form_state['input'] and doesn't
      // exist in $form_state['values'].
      $selected_weights = $form_state['input']['select_weight'];
      foreach ($selected_weights as $index => $weight) {
        $expore_config[$index]['weight'] = $weight;
      }
      // Sort config array by weight and update drupal variable.
      uasort($expore_config, 'drupal_sort_weight');
      $form_state['islandora_solr_facet_filters'] = $expore_config;
    }

    // Check if new display facet is being added to the table.
    if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#id'] == 'facet-filter-add-more') {
      $duplicate_label = FALSE;
      $duplicate_filter = FALSE;
      $duplicate = FALSE;
      $facet_label = $form_state['values']['label'];
      $facet_filter = $form_state['values']['filter'];
      $facet_weight = $form_state['values']['facet_weight'];

      if (empty($facet_filter)) {
        form_set_error('filter', t('Facet Filter is required to add a display facet.'));
      }

      if (empty($facet_label)) {
        form_set_error('label', t('Display Label is required to add a display facet.'));
      }

      if (!empty($facet_label) && !empty($expore_config)) {
        foreach ($expore_config as $row) {
          if ($row['label'] == $facet_label) {
            // Label exists return form error.
            $duplicate_label = TRUE;
            break;
          }
          elseif ($row['filter'] == $facet_filter) {
            // Facet filter exists return form error.
            $duplicate_filter = TRUE;
            break;
          }
        }
        if ($duplicate_label) {
          form_set_error('label', t('Display Label must be unique.'));
          $duplicate = TRUE;
        }
        elseif ($duplicate_filter) {
          form_set_error('filter', t('Facet Filter must be unique.'));
          $duplicate = TRUE;
        }
      }
      if (!empty($facet_label) && !empty($facet_filter) && !$duplicate) {
        // Before it's added try to run the facet query and see if it's valid.
        $facet_is_valid = TRUE;

        if (!empty($facet_filter)) {
          module_load_include('inc', 'islandora_solr', 'includes/explore');

          // Store current messages.
          $old_msg = drupal_get_messages();

          // Clear current error messages.
          drupal_get_messages('error', TRUE);

          // Run a facet query on the supplied filter.
          islandora_solr_explore_test_facet_query($facet_filter);

          // Clear error messages run facet query and check if there are any
          // error messages.  If there are error messages assume that the query
          // failed and set facet to invalid.
          $error_msgs = drupal_get_messages('error');
          if (isset($error_msgs['error'])) {
            $facet_is_valid = FALSE;
          }
          // Restore the original messages.
          $_SESSION['messages'] = $old_msg;
          if (!$facet_is_valid) {
            form_set_error('filter', t('Invalid Facet Query Filter.'));
          }
        }

        if ($facet_is_valid) {
          // Put values into config array.
          $expore_config[$facet_filter] = array(
            'label' => $facet_label,
            'filter' => $facet_filter,
            'weight' => $facet_weight,
          );

          // Sort config array by weight and update drupal variable.
          uasort($expore_config, 'drupal_sort_weight');

          // Reset input values on successful add.
          $form_state['input']['label'] = '';
          $form_state['input']['filter'] = '';
          $form_state['input']['facet_weight'] = 0;
          $form_state['islandora_solr_facet_filters'] = $expore_config;
        }
      }
    }

    // Set table row data to be rendered.
    $rows = array();
    if (!empty($expore_config)) {
      foreach ($expore_config as $row_index => $row) {
        $rows["facet-row-$row_index"] = array(
          'display_label' => htmlentities($row['label']),
          'facet_filter' => htmlentities($row['filter']),
          'facet_weight' => array(
            'data' => array(
              '#type' => 'select',
              '#title' => t('Weight'),
              '#options' => drupal_map_assoc(range(-50, 50)),
              '#value' => (isset($row['weight']) ? $row['weight'] : 0),
              '#title_display' => 'invisible',
              '#attributes' => array(
                'name' => "select_weight[{$row['filter']}]",
              ),
            ),
          ),
        );
      }
    }

    $form['facet'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="islandora-solr-facet-filter-wrapper">',
      '#suffix' => '</div>',
      '#title' => t('Setup Display Facets'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['facet']['table'] = array(
      '#type' => 'tableselect',
      '#header' => array(
        'display_label' => t('Display Label'),
        'facet_filter' => t('Facet Query Filter'),
        'facet_weight' => t('Weight'),
      ),
      '#options' => $rows,
      '#empty' => t('No display facets'),
      '#attributes' => array('class' => array('main-facet-table')),
    );
    $form['facet']['actions'] = array(
      '#type' => 'actions',
      '#weight' => 5,
    );
    $form['facet']['actions']['remove'] = array(
      '#type' => 'button',
      '#value' => t('Remove selected'),
      '#weight' => 0,
      '#ajax' => array(
        'callback' => '_islandora_solr_update_filter_table',
        'wrapper' => 'islandora-solr-facet-filter-wrapper',
      ),
    );
    $form['facet']['actions']['update_weight'] = array(
      '#type' => 'button',
      '#value' => t('Update weights'),
      '#weight' => 1,
      '#ajax' => array(
        'callback' => '_islandora_solr_update_filter_table',
        'wrapper' => 'islandora-solr-facet-filter-wrapper',
      ),
    );
    $form['facet']['fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add New Display Facet Details'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#weight' => 13,
    );
    $form['facet']['fieldset']['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Display Label'),
      '#size' => 60,
      '#description' => t('The text displayed for the generated link.'),
      '#attributes' => array('class' => array('new-facet-label')),
    );
    $form['facet']['fieldset']['filter'] = array(
      '#type' => 'textarea',
      '#title' => t('Facet Query Filter'),
      '#size' => 60,
      '#description' => t("Write a custom facet query, for information on available tags see:") . l(t('Solr Luke Schema.'), "{$GLOBALS['base_url']}:8080/solr/admin/luke"),
      '#attributes' => array('class' => array('new-facet-filter')),
    );
    $form['facet']['fieldset']['facet_weight'] = array(
      '#type' => 'select',
      '#title' => t('Facet weight'),
      '#description' => t('Display weight'),
      '#options' => drupal_map_assoc(range(-50, 50)),
      '#default_value' => 0,
      '#attributes' => array('class' => array('new-facet-weight')),
    );
    $form['facet']['fieldset']['add'] = array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#attributes' => array('class' => array('islandora-solr-add-more-submit')),
      '#id' => 'facet-filter-add-more',
      '#ajax' => array(
        'callback' => '_islandora_solr_update_filter_table',
        'wrapper' => 'islandora-solr-facet-filter-wrapper',
      ),
    );
    // Process submit and variable set the new config on submit.
    $form['#submit'][] = 'islandora_solr_update_filters_on_submit';
  }
}

/**
 * On submit of the explore block config save the facet filters in form state.
 */
function islandora_solr_update_filters_on_submit($form, &$form_state) {
  if (isset($form_state['islandora_solr_facet_filters'])) {
    variable_set('islandora_solr_explore_config', $form_state['islandora_solr_facet_filters']);
  }
}

/**
 * Ajax Callback to reload the facet form when adding rows to the table.
 */
function _islandora_solr_update_filter_table($form, &$form_state) {
  return $form['facet'];
}

/**
 * Implements hook_block_view().
 *
 * @global IslandoraSolrQueryProcessor $_islandora_solr_queryclass
 *   The IslandoraSolrQueryProcessor object which includes the current query
 *   settings and the raw Solr results.
 */
function islandora_solr_block_view($block_name = '') {
  global $_islandora_solr_queryclass;
  $block = array();

  $solr_blocks = module_invoke_all("islandora_solr_query_blocks");

  // If the block that is called is defined, and we have permission to view it.
  if (!empty($solr_blocks[$block_name]) && user_access('search islandora solr')) {
    // Set block title.
    $block['subject'] = $solr_blocks[$block_name]['name'];

    // Include the file from which the block originates.
    if (isset($solr_blocks[$block_name]['file'])) {
      require_once drupal_get_path('module', $solr_blocks[$block_name]['module']) . '/' . $solr_blocks[$block_name]['file'];
    }

    // If a class is present, instantiate it and proceed.
    // The gloval variable $_islandora_solr_queryclass (the
    // IslandoraSolrQueryProcessor, containing the Solr search result), is fed
    // as an argument.
    if (!empty($solr_blocks[$block_name]['class']) && !empty($solr_blocks[$block_name]['function'])) {
      // Check if we're on a search results page.
      if (islandora_solr_results_page($_islandora_solr_queryclass)) {
        $display_class = new $solr_blocks[$block_name]['class']();
        $block_function = $solr_blocks[$block_name]['function'];
        if (method_exists($display_class, $block_function)) {

          $block['content'] = $display_class->$block_function($_islandora_solr_queryclass);
        }
      }
    }
    elseif (!empty($solr_blocks[$block_name]['form'])) {
      $block['content'] = drupal_get_form($solr_blocks[$block_name]['form']);
    }
    // Else if only a function is given (no class, no form).
    elseif (!empty($solr_blocks[$block_name]['function'])) {
      $function = $solr_blocks[$block_name]['function'];
      $block['content'] = $function();
    }
  }
  return $block;
}

/**
 * Islandora Solr simple search form.
 *
 * @param array $form
 *   An associative array containing form structure.
 * @param array $form_state
 *   An associative array containing form state.
 *
 * @return array
 *   An associative array containing the fully built form structure.
 */
function islandora_solr_simple_search_form($form, &$form_state) {

  $form['simple'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'container-inline',
      ),
    ),
  );

  if (variable_get('islandora_solr_simple_search_label_visibility', TRUE) == FALSE) {
    $title_visible = 'invisible';
  }
  else {
    $title_visible = 'before';
  }

  $title_label = variable_get('islandora_solr_simple_search_label_title', 'Search Term');

  $form['simple']["islandora_simple_search_query"] = array(
    '#size' => '15',
    '#type' => 'textfield',
    '#title' => check_plain($title_label),
    '#title_display' => $title_visible,
    // @todo Should this be the searched value?
    '#default_value' => '',
  );
  $form['simple']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('search'),
  );
  return $form;
}

/**
 * Islandora Solr simple search form submit callback.
 *
 * @param array $form
 *   An associative array containing form structure.
 * @param array $form_state
 *   An associative array containing form state.
 */
function islandora_solr_simple_search_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  $search_string = islandora_solr_replace_slashes($form_state['values']['islandora_simple_search_query']);

  $query = array('type' => 'dismax');

  $form_state['redirect'] = array(
    ISLANDORA_SOLR_SEARCH_PATH . "/$search_string",
    array(
      'query' => $query,
    ),
  );
}

/**
 * Islandora Solr advanced search block form.
 *
 * Check form states:
 * 1: Form update using AJAX.
 * 2: Populate with current query on search results page.
 * 3: Anywhere else: empty form.
 *
 * @link http://drupal.stackexchange.com/questions/14855/how-do-i-dynamically-fill-a-textfield-with-ajax/16576#16576 Some example AJAX. @endlink
 *
 * @global IslandoraSolrQueryProcessor $_islandora_solr_queryclass
 *   The IslandoraSolrQueryProcessor object which includes the current query
 *   settings and the raw Solr results.
 *
 * @param array $form
 *   An associative array containing form structure.
 * @param array $form_state
 *   An associative array containing form state.
 *
 * @return array
 *   An associative array containing the fully built form structure.
 */
function islandora_solr_advanced_search_form($form, &$form_state) {
  global $_islandora_solr_queryclass;

  // 1: Form update using AJAX.
  if (isset($form_state['triggering_element'])) {

    unset($form_state['input']['terms']);
    // Check for add.
    if ($form_state['triggering_element']['#value'] == '+') {
      $form_state['values']['terms'][] = array();
    }
    // Check for remove.
    elseif ($form_state['triggering_element']['#value'] == '-') {
      $field = $form_state['triggering_element']['#field'];
      array_splice($form_state['values']['terms'], $field, 1);
    }

    $values = $form_state['values'];
  }
  // 2: Populate with current query on search results page.
  elseif (islandora_solr_results_page($_islandora_solr_queryclass) == TRUE && !isset($_islandora_solr_queryclass->internalSolrParams['type'])) {

    // Get current query.
    $query = $_islandora_solr_queryclass->solrQuery;

    $values['terms'] = array();

    $query_explode = preg_split(ISLANDORA_SOLR_QUERY_SPLIT_REGEX, $query);

    // Break up the solr query to populate the advanced search form.
    $i = 0;
    foreach ($query_explode as $key => $value) {
      $term = array();

      // Check for first colon to split the string.
      if (strpos($value, ':') != FALSE) {
        // Split the filter into field and value.
        $value_split = preg_split(ISLANDORA_SOLR_QUERY_FIELD_VALUE_SPLIT_REGEX, $value, 2);

        $values['terms'][$i]['field'] = stripslashes($value_split[0]);

        // Second part of the split is the query value (or first part of it).
        $value_split[1] = str_replace(array('(', ')'), '', $value_split[1]);
        $values['terms'][$i]['search'] = stripslashes($value_split[1]);
      }
      // If the string does not include a colon or AND/OR/NOT, then it is a
      // part of the query value.
      elseif (!preg_match('/(AND|OR|NOT)/', $value, $matches)) {
        // Trim brackets.
        $value = str_replace(array('(', ')'), '', $value);

        if (isset($values['terms'][$i]['search'])) {
          $values['terms'][$i]['search'] = $values['terms'][$i]['search'];
          // Append to search string.
          $values['terms'][$i]['search'] .= ' ' . stripslashes($value);
        }
        else {
          // Search field is not set, so create new search value.
          $values['terms'][$i]['search'] = stripslashes($value);
        }
      }
      // If it matches AND/OR/NOT, then we have the boolean operator.
      else {
        $values['terms'][$i]['boolean'] = $value;

        // XXX: Something about only incrementing here seems... Wrong?
        $i++;
      }
    }
  }
  // 3: Anywhere else: empty form.
  else {
    // Need at least one term to draw the search box.
    $values = array(
      'terms' => array(''),
    );
  }

  $terms = array(
    '#type' => 'markup',
    '#prefix' => '<div id="islandora-solr-advanced-terms">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  foreach ($values['terms'] as $i => $value) {
    $term = array(
      '#tree' => TRUE,
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
    $term['field'] = array(
      '#title' => t('Field'),
      '#type' => 'select',
      '#default_value' => isset($value['field']) ? $value['field'] : 'dc.title',
      '#options' => islandora_solr_get_fields('search_fields'),
    );
    $term['search'] = array(
      '#title' => t('Search terms'),
      '#type' => 'textfield',
      '#size' => 20,
      '#default_value' => (isset($value['search']) ?
        $value['search'] :
        ''),
    );
    // Used for when the user presses enter on the search field.
    $term['hidden_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#attributes' => array('style' => 'visibility:hidden;position:fixed;top:-1000px;right:-1000px;'),
    );
    $term['add'] = array(
      '#type' => 'button',
      '#value' => '+',
      '#attributes' => array('title' => t('Add field')),
      '#name' => 'add-field-' . $i,
      '#ajax' => array(
        'callback' => '_islandora_solr_advanced_search_terms',
        'wrapper' => 'islandora-solr-advanced-terms',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type' => 'none'),
      ),
    );
    if (count($values['terms']) > 1) {
      $term['remove'] = array(
        '#type' => 'button',
        '#field' => $i,
        '#value' => '-',
        '#attributes' => array('title' => t('Remove field')),
        '#name' => 'remove-field-' . $i,
        '#ajax' => array(
          'callback' => '_islandora_solr_advanced_search_terms',
          'wrapper' => 'islandora-solr-advanced-terms',
          'method' => 'replace',
          'effect' => 'fade',
          'progress' => array('type' => 'none'),
        ),
      );
      if ((variable_get('islandora_solr_search_boolean', 'user') == 'user') && ((count($values['terms']) - 1) != $i)) {
        $term['boolean'] = array(
          '#type' => 'select',
          '#prefix' => '<div>',
          '#suffix' => '</div>',
          '#default_value' => isset($value['boolean']) ? $value['boolean'] : 'AND',
          '#options' => array(
            'AND' => 'AND',
            'OR' => 'OR',
            'NOT' => 'NOT',
          ),
        );
      }
    }
    $terms[] = $term;
  }

  // Add terms.
  $form['terms'] = $terms;
  // Add controls.
  $form['controls'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="islandora-solr-advanced-controls">',
    '#suffix' => '</div>',
  );
  // Filter preservation toggle.
  if (variable_get('islandora_solr_allow_preserve_filters', FALSE)) {
    $form['controls']['islandora_solr_allow_preserve_filters'] = array(
      '#type' => 'checkbox',
      '#title' => t('Preserve Filters'),
      '#default_value' => FALSE,
    );
  }
  $form['controls']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  return $form;
}

/**
 * AJAX callback for advanced search form.
 *
 * @param array $form
 *   An associative array containing form structure.
 * @param array $form_state
 *   An associative array containing form state.
 *
 * @return array
 *   An associative array containing the fully built form element.
 *
 * @see islandora_solr_advanced_search_form()
 */
function _islandora_solr_advanced_search_terms($form, &$form_state) {
  return $form['terms'];
}


/**
 * Islandora Solr advanced search form submit callback.
 *
 * @param array $form
 *   An associative array containing form structure.
 * @param array $form_state
 *   An associative array containing form state.
 *
 * @see islandora_solr_advanced_search_form()
 */
function islandora_solr_advanced_search_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');

  // Collect query values.
  $query_array = array();

  // Get Lucene syntax escaping configuration, prior to the following
  // foreach loop.
  $lucene_syntax_escape = variable_get('islandora_solr_advanced_search_block_lucene_syntax_escape', FALSE);
  $lucene_regex = variable_get(
    'islandora_solr_advanced_search_block_lucene_regex_default',
    ISLANDORA_SOLR_QUERY_FACET_LUCENE_ESCAPE_REGEX_DEFAULT
  );

  foreach ($form_state['values']['terms'] as $term) {
    $field = islandora_solr_lesser_escape($term['field']);
    $search = trim($term['search']);

    $boolean = (isset($term['boolean'])) ? $term['boolean'] : variable_get('islandora_solr_search_boolean', 'user');

    // Add query.
    if (!empty($search)) {
      $search = $lucene_syntax_escape ?
        islandora_solr_facet_query_escape($search, $lucene_regex) :
        islandora_solr_lesser_escape($search);

      $query_array[] = array(
        'search' => "$field:($search)",
        'boolean' => $boolean,
      );
    }
  }

  // Create query.
  $query = '';
  $i = 0;
  foreach ($query_array as $term) {
    $query .= $term['search'];
    if (count($query_array) - 1 != $i) {
      $query .= ' ' . $term['boolean'] . ' ';
    }
    $i++;
  }

  // Check if query is empty.
  if (empty($query)) {
    if (variable_get('islandora_solr_base_advanced', FALSE)) {
      $query = variable_get('islandora_solr_base_query', '*:*');
    }
    else {
      $query = '*:*';
    }
  }

  // Handle filters.
  $filter = '';
  if (isset($form_state['values']['islandora_solr_allow_preserve_filters']) && $form_state['values']['islandora_solr_allow_preserve_filters']) {
    $filter = isset($_GET['f']) ? $_GET['f'] : '';
  }

  // Use work around for some special URL characters.
  $query = islandora_solr_replace_slashes($query);

  // Navigate to results page.
  $form_state['redirect'] = array(
    ISLANDORA_SOLR_SEARCH_PATH . "/$query",
    $filter ? array('query' => array('f' => $filter)) : array(),
  );
}

/**
 * Provides list to change primary displays.
 *
 * Only populated with displays that are enabled through the admin form.
 *
 * @global IslandoraSolrQueryProcessor $_islandora_solr_queryclass
 *   The IslandoraSolrQueryProcessor object which includes the current query
 *   settings and the raw Solr results.
 *
 * @return string
 *   A rendered list containing enabled primary display links.
 */
function islandora_solr_display() {
  global $_islandora_solr_queryclass;

  $output = '';

  // Check if the page is dealing with search results.
  if (islandora_solr_results_page($_islandora_solr_queryclass)) {

    // Parameters set in URL.
    $params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();
    $path = current_path();

    $profiles = module_invoke_all("islandora_solr_primary_display");
    // Get the table settings.
    $primary_display_array = variable_get('islandora_solr_primary_display_table', array());

    if (!empty($primary_display_array)) {

      $weight = $primary_display_array['weight'];
      $enabled = $primary_display_array['enabled'];
      $default = $primary_display_array['default'];

      // Sort by weight.
      array_multisort(array_values($weight), SORT_ASC, array_keys($weight), SORT_ASC, $weight);

      $list_items = array();

      foreach ($weight as $key => $value) {
        if ($enabled[$key] !== 0) {
          if (isset($params['display'])) {
            $current_display = $params['display'];
          }
          else {
            $current_display = $default;
          }

          $display_name = $profiles[$key]['name'];

          $display_description = isset($profiles[$key]['description']) ? $profiles[$key]['description'] : NULL;

          $query = array_merge($params, array('display' => $key));

          $attr = array();
          $attr['class'] = 'display-' . drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $key));
          if ($key == $current_display) {
            $attr['class'] .= ' active';
          }
          $attr['rel'] = 'nofollow';
          $attr['title'] = $display_description;
          $attr['href'] = url($path, array('query' => $query));

          // XXX: We're not using l() because of
          // @link http://drupal.org/node/41595 active classes. @endlink
          $item = '<a' . drupal_attributes($attr) . '>' . check_plain($display_name) . '</a>';

          // Add link to list.
          $list_items[] = $item;
        }
      }
      // Theme item list (if more than one displays are available).
      if (count($list_items) > 1) {
        $output = theme('item_list', array(
          'items' => $list_items,
          'title' => NULL,
          'type' => 'ul',
          'attributes' => array('class' => 'islandora-solr-display'),
        ));
      }
    }
  }
  return $output;
}


/**
 * Provides a list to change the sorting on a current search query.
 *
 * @global IslandoraSolrQueryProcessor $_islandora_solr_queryclass
 *   The IslandoraSolrQueryProcessor object which includes the current query
 *   settings and the raw Solr results.
 *
 * @return string
 *   A rendered list containing Solr sort links.
 */
function islandora_solr_sort() {
  module_load_include('inc', 'islandora', 'includes/utilities');
  global $_islandora_solr_queryclass;

  $output = '';

  // Check if the page is dealing with search results.
  if (islandora_solr_results_page($_islandora_solr_queryclass)) {

    // Parameters set in URL.
    $params = isset($_islandora_solr_queryclass->internalSolrParams) ? $_islandora_solr_queryclass->internalSolrParams : array();

    $path = current_path();

    $sort_terms = islandora_solr_get_fields('sort_fields');

    // Get current sort (check in the actual parameters used for the query,
    // because modules might have altered it).
    if (isset($_islandora_solr_queryclass->solrParams['sort'])) {
      $sort_explode = preg_split(
        ISLANDORA_SOLR_QUERY_SPLIT_REGEX,
        $_islandora_solr_queryclass->solrParams['sort']
      );
      $current_sort_term = stripslashes($sort_explode[0]);
      $current_sort_order = isset($sort_explode[1]) ? $sort_explode[1] : NULL;
    }
    else {
      // If no parameters are given, internally solr defaults to 'score desc'.
      // See Common Query Paremeters section:
      // http://archive.apache.org/dist/lucene/solr/ref-guide/
      $current_sort_term = 'score';
      $current_sort_order = 'desc';
    }

    $list_items = array();

    foreach ($sort_terms as $term => $label) {
      $indicator = '';

      // Create query and indicator arrow.
      if ($term == $current_sort_term) {
        if (isset($current_sort_order) && $current_sort_order == 'asc') {
          $order = 'desc';
          $indicator = theme('tablesort_indicator', array('style' => 'desc'));
        }
        else {
          $order = 'asc';
          $indicator = theme('tablesort_indicator', array('style' => 'asc'));
        }
      }
      // The term is score or a date, we want descending order by default.
      elseif ($term == 'score' || stripos($term, 'date')) {
        $order = 'desc';
      }
      else {
        $order = 'asc';
      }
      $sort_param = $term . ' ' . $order;

      $query = array_merge($params, array('sort' => $sort_param));

      $attributes = array(
        'sort' => array(
          'attr' => array(
            'class' => array(),
          ),
          'query' => $query,
          'path' => $path,
        ),
      );
      $attr =& $attributes['sort']['attr'];
      if ($term == $current_sort_term) {
        $attr['class'] = array('active');
      }
      $attr['rel'] = 'nofollow';
      $attr['title'] = $label;
      $attr['href'] = url($path, array('query' => $query));

      $hooks = islandora_build_hook_list(ISLANDORA_SOLR_FACET_BUCKET_CLASSES_HOOK_BASE);
      drupal_alter($hooks, $attributes, $_islandora_solr_queryclass);

      // XXX: We're not using l() because of
      // @link http://drupal.org/node/41595 active classes. @endlink
      $item = '<a' . drupal_attributes($attr) . '>' . $label . ' ' . $indicator . '</a>';

      // Add link to list.
      $list_items[] = $item;
    }

    // Theme item list.
    if (!empty($list_items)) {
      $output = theme('item_list', array(
        'items' => $list_items,
        'title' => NULL,
        'type' => 'ul',
        'attributes' => array('class' => 'islandora-solr-sort'),
      ));
    }
  }

  return $output;
}

/**
 * Search results navigation block.
 *
 * Pull params out of the URL to determine search id
 * and object offset.
 *
 * We use offset because the pid of a search result can be altered.
 * eg: Book pages
 *
 * @return string
 *   The rendered html for the block content.
 */
function islandora_solr_search_navigation() {
  $return = NULL;
  $params = drupal_get_query_parameters();
  if (variable_get('islandora_solr_search_navigation', FALSE) &&
    isset($params['solr_nav']['id']) &&
    isset($params['solr_nav']['page']) &&
    isset($params['solr_nav']['offset']) &&
    isset($_SESSION['islandora_solr_search_nav_params'][$params['solr_nav']['id']])) {
    $nav_params = $_SESSION['islandora_solr_search_nav_params'][$params['solr_nav']['id']];

    $prev_solr_object = NULL;
    $prev_offset = NULL;
    $prev_page = NULL;
    $next_solr_object = NULL;
    $next_offset = NULL;
    $next_page = NULL;
    $offset = $params['solr_nav']['offset'];
    $page = $params['solr_nav']['page'];
    $query_processor = new IslandoraSolrQueryProcessor();
    $query_processor->solrQuery = $nav_params['query'];
    $query_processor->internalSolrQuery = $nav_params['query_internal'];
    $query_processor->solrLimit = $nav_params['limit'];
    $query_processor->internalSolrParams = $nav_params['params_internal'];
    $query_processor->solrParams = $nav_params['params'];
    $limit = $nav_params['limit'];
    $query_processor->solrStart = $page * $limit;

    // Get solr objects for current page.
    $query_processor->executeQuery();

    $objects = array();
    if (!empty($query_processor->islandoraSolrResult)) {
      $objects = $query_processor->islandoraSolrResult['response']['objects'];
    }

    // Try get previous object on the current page.
    if ($offset != 0) {
      if (isset($objects[$offset - 1])) {
        $prev_solr_object = $objects[$offset - 1];
        $prev_offset = $offset - 1;
        $prev_page = $page;
      }
    }

    // If previous object can't be found on current page,
    // go to the previous page and select the last object.
    if (!$prev_solr_object && $page != 0) {
      $query_processor->solrStart = ($page - 1) * $limit;
      $query_processor->executeQuery();
      if (!empty($query_processor->islandoraSolrResult) && !empty($query_processor->islandoraSolrResult['response']['objects'])) {
        $prev_objects = $query_processor->islandoraSolrResult['response']['objects'];
        end($prev_objects);
        $prev_offset = key($prev_objects);
        $prev_page = $page - 1;
        $prev_solr_object = $prev_objects[$prev_offset];
      }
    }

    // Try get next object on the current page.
    if (isset($objects[$offset + 1])) {
      $next_solr_object = $objects[$offset + 1];
      $next_offset = $offset + 1;
      $next_page = $page;
    }

    // If next object can't be found on current page,
    // go to the next page and select the first object.
    if (!$next_solr_object) {
      $limit = $query_processor->solrLimit;
      $query_processor->solrStart = ($page + 1) * $limit;
      $query_processor->executeQuery();
      if (!empty($query_processor->islandoraSolrResult) && !empty($query_processor->islandoraSolrResult['response']['objects'])) {
        $next_objects = $query_processor->islandoraSolrResult['response']['objects'];
        $next_offset = 0;
        $next_page = $page + 1;
        $next_solr_object = $next_objects[$next_offset];
      }
    }

    $return_link = NULL;
    $prev_link = NULL;
    $next_link = NULL;
    // Get URL details from the prev and next objects
    // to populate navigation links.
    if (isset($nav_params['path'])) {
      $return_query = array_merge($query_processor->internalSolrParams, array('page' => $page));
      $return_link = url($nav_params['path'], array('query' => $return_query, 'absolute' => TRUE));
    }
    if ($prev_solr_object) {
      $prev_params = $prev_solr_object['object_url_params'];
      $prev_params['solr_nav']['offset'] = $prev_offset;
      $prev_params['solr_nav']['page'] = $prev_page;
      $prev_params['solr_nav']['id'] = $params['solr_nav']['id'];
      $prev_link = url($prev_solr_object['object_url'], array(
        'query' => $prev_params,
        'fragment' => (isset($prev_solr_object['object_url_fragment']) ? $prev_solr_object['object_url_fragment'] : ''),
        'absolute' => TRUE,
      ));

    }

    if ($next_solr_object) {
      $next_params = $next_solr_object['object_url_params'];
      $next_params['solr_nav']['offset'] = $next_offset;
      $next_params['solr_nav']['page'] = $next_page;
      $next_params['solr_nav']['id'] = $params['solr_nav']['id'];
      $next_link = url($next_solr_object['object_url'], array(
        'query' => $next_params,
        'fragment' => (isset($next_solr_object['object_url_fragment']) ? $next_solr_object['object_url_fragment'] : ''),
        'absolute' => TRUE,
      ));
    }
    $return = theme('islandora_solr_search_navigation_block', array(
      'return_link' => $return_link,
      'return_text' => t('Return to search'),
      'prev_link' => $prev_link,
      'prev_text' => t('Previous Object'),
      'next_link' => $next_link,
      'next_text' => t('Next Object'),
    ));
  }
  return $return;
}

/**
 * Search results page size block.
 *
 * Use the limit URL param to increase or decrease the result page size.
 *
 * @return string
 *   The rendered html for the block content.
 */
function islandora_solr_search_results_limit() {
  global $_islandora_solr_queryclass;
  // Check if the page is dealing with search results.
  if (islandora_solr_results_page($_islandora_solr_queryclass)) {
    $default_limit = variable_get('islandora_solr_num_of_results', '20');
    $limits = variable_get('islandora_solr_num_of_results_advanced', array());
    $params = drupal_get_query_parameters();
    $path = current_path();
    if (isset($params['limit']) && is_numeric($params['limit'])) {
      $default_limit = $params['limit'];
    }
    $limit_list = array();
    foreach ($limits as $limit) {
      if ($limit == $default_limit) {
        $limit_list[] = $limit;
      }
      else {
        $attr = array();
        $attr['rel'] = 'nofollow';
        $query = $params;
        $query['limit'] = $limit;
        unset($query['page']);
        $attr['href'] = url($path, array('query' => $query));
        // XXX: We're not using l() because of
        // @link http://drupal.org/node/41595 active classes. @endlink
        $limit_list[] = '<a' . drupal_attributes($attr) . '>' . $limit . '</a>';
      }
    }

    if (!empty($limit_list)) {
      return theme('item_list', array(
        'items' => $limit_list,
        'title' => NULL,
        'type' => 'ul',
        'attributes' => array('id' => 'islandora_solr_results_limit'),
      ));
    }
  }
}
