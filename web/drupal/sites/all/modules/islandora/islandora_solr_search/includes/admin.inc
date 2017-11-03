<?php

/**
 * @file
 * Contains all administration related functionality.
 */

/**
 * Islandora Solr index admin form.
 */
function islandora_solr_admin_index_settings($form, &$form_state) {
  // Add admin form CSS.
  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'islandora_solr') . '/css/islandora_solr.admin.css',
    ),
  );

  $solr_url = !empty($form_state['values']['islandora_solr_url']) ?
    $form_state['values']['islandora_solr_url'] :
    variable_get('islandora_solr_url', 'localhost:8080/solr');

  // Solr connect triggering handler is dismax or not set on page load.
  if ((isset($form_state['triggering_element']) &&
      (($form_state['triggering_element']['#name'] == 'islandora_solr_url') ||
      ($form_state['triggering_element']['#name'] == 'islandora_solr_request_handler'))) ||
      !isset($form_state['triggering_element'])) {

    // Check for the PHP Solr lib class.
    if (!class_exists('Apache_Solr_Service')) {
      $message = t('This module requires the <a href="!url">Apache Solr PHP Client</a>. Please install the client in the root directory of this module before continuing.', array('!url' => 'http://code.google.com/p/solr-php-client'));
      drupal_set_message(check_plain($message));
      return;
    }

    // Get request handler.
    $handler = !empty($form_state['values']['islandora_solr_request_handler']) ? $form_state['values']['islandora_solr_request_handler'] : variable_get('islandora_solr_request_handler', FALSE);

    if (strpos($solr_url, 'https://') !== FALSE && strpos($solr_url, 'https://') == 0) {
      $confirmation_message = format_string('<img src="@image_url"/>!message', array(
        '@image_url' => file_create_url('misc/watchdog-error.png'),
        '!message' => t('Islandora does not support SSL connections to Solr.'),
      ));
      $solr_avail = FALSE;
    }
    elseif (preg_match('/^([^\/]+):/', $solr_url) === 0) {
      form_set_error('islandora_solr_url', t('Port number required in Solr URL.'));
      $solr_avail = FALSE;
    }
    else {
      // Check if Solr is available.
      $solr_avail = islandora_solr_ping($solr_url);

      $dismax_allowed = FALSE;
      // If solr is available, get the request handlers.
      if ($solr_avail) {
        // Find request handlers (~500ms).
        $handlers = _islandora_solr_get_handlers($solr_url);
      }
      // Get confirmation message.
      if ($solr_avail) {
        $confirmation_message = format_string('<img src="@image_url"/>!message', array(
          '@image_url' => file_create_url('misc/watchdog-ok.png'),
          '!message' => t('Successfully connected to Solr server at !link. <sub>(!ms ms)</sub>', array(
            '!link' => l($solr_url, islandora_solr_check_http($solr_url), array(
              'attributes' => array(
                'target' => '_blank',
              ),
            )),
            '!ms' => number_format($solr_avail, 2),
          )),
        ));
      }
      else {
        $confirmation_message = format_string('<img src="@image_url"/>!message', array(
          '@image_url' => file_create_url('misc/watchdog-error.png'),
          '!message' => t('Unable to connect to Solr server at !link.', array(
            '!link' => l($solr_url, islandora_solr_check_http($solr_url), array(
              'attributes' => array(
                'target' => '_blank',
              ),
            )),
          )),
        ));
      }
    }
  }
  // AJAX wrapper for URL checking.
  $form['solr_ajax_wrapper'] = array(
    '#prefix' => '<div id="solr-url">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  // Solr URL.
  $form['solr_ajax_wrapper']['islandora_solr_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr URL'),
    '#size' => 80,
    '#weight' => -1,
    '#description' => t('The URL of the Solr installation. Defaults to localhost:8080/solr.  Port number (e.g. ":8080") is required.'),
    '#default_value' => $solr_url,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_islandora_solr_update_solr_url',
      'wrapper' => 'solr-url',
      'effect' => 'fade',
      'event' => 'blur',
      'progress' => array('type' => 'throbber'),
    ),
  );

  // Hidden submit button.
  $form['solr_ajax_wrapper']['refresh_page'] = array(
    '#type' => 'submit',
    '#value' => t('Test connection'),
    '#attributes' => array('class' => array('refresh-button')),
    '#submit' => array('_islandora_solr_admin_refresh'),
  );
  // Confirmation message.
  $form['solr_ajax_wrapper']['infobox'] = array(
    '#type' => 'item',
    '#markup' => isset($confirmation_message) ? $confirmation_message : $form_state['complete form']['solr_ajax_wrapper']['infobox']['#markup'],
  );

  // Don't show form item if no request handlers are found.
  if (!empty($handlers)) {
    $form['solr_ajax_wrapper']['islandora_solr_request_handler'] = array(
      '#type' => 'select',
      '#title' => t('Request handler'),
      '#options' => $handlers,
      '#description' => t('Request handlers, as defined by <a href="!url">solrconfig.xml</a>.', array('!url' => 'http://wiki.apache.org/solr/SolrConfigXml')),
      '#default_value' => $handler,
      '#ajax' => array(
        'callback' => '_islandora_solr_update_solr_url',
        'wrapper' => 'solr-url',
        'effect' => 'fade',
        'event' => 'change',
        'progress' => array('type' => 'throbber'),
      ),
    );
  }
  $form['solr_ajax_wrapper']['islandora_solr_available'] = array(
    '#type' => 'hidden',
    '#value' => $solr_avail ? 1 : 0,
  );

  // Solr force delete from index during object purge.
  $form['solr_ajax_wrapper']['islandora_solr_force_update_index_after_object_purge'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force update of Solr index after an object is deleted'),
    '#weight' => 5,
    '#description' => t('If checked, deleting objects will also force their removal from the Solr index. <br/><strong>Note:</strong> When active, UI consistency will be increased on any pages using Solr queries for display. This setting is not appropriate for every installation (e.g., on sites with a large volume of Solr commits that hit execution limits, or where the Solr index is not directly writable from Drupal).'),
    '#default_value' => variable_get('islandora_solr_force_update_index_after_object_purge', 0),
  );

  // Actions.
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Solr configuration'),
    '#weight' => 0,
    '#submit' => array('_islandora_solr_admin_index_settings_submit'),
    '#validate' => array('_islandora_solr_admin_index_settings_validate'),
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('_islandora_solr_admin_index_settings_submit'),
  );
  return $form;
}

/**
 * Islandora Solr index admin validate callback.
 */
function _islandora_solr_admin_index_settings_validate($form, &$form_state) {
  if ($form_state['values']['islandora_solr_available'] == 0) {
    form_set_error('islandora_solr_url', t('Please enter a valid Solr URL.'));
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Islandora Solr index admin submit callback.
 */
function _islandora_solr_admin_index_settings_submit($form, &$form_state) {
  // If this function is called not using submit or reset buttons.
  if ($form_state['triggering_element']['#id'] != 'edit-submit' && $form_state['triggering_element']['#id'] != 'edit-reset') {
    $form_state['rebuild'] = TRUE;
    return;
  }
  // Get operation.
  $op = isset($form_state['triggering_element']['#id']) ? $form_state['triggering_element']['#id'] : '';

  switch ($op) {
    case 'edit-submit':
      variable_set('islandora_solr_url', $form_state['values']['islandora_solr_url']);
      variable_set('islandora_solr_request_handler', $form_state['values']['islandora_solr_request_handler']);
      variable_set('islandora_solr_force_update_index_after_object_purge', $form_state['values']['islandora_solr_force_update_index_after_object_purge']);
      drupal_set_message(t('The Solr configuration options have been saved.'));
      break;

    case 'edit-reset':
      variable_del('islandora_solr_url');
      variable_del('islandora_solr_request_handler');
      variable_del('islandora_solr_force_update_index_after_object_purge');
      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }

  // Force renewal of the cached value, as the request handler might have
  // changed.
  islandora_solr_check_dismax(TRUE);
}

/**
 * Function to return administration setting form.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The form array.
 */
function islandora_solr_admin_settings($form, &$form_state) {
  // Add admin form css.
  drupal_set_title(t('Solr settings'));

  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'islandora_solr') . '/css/islandora_solr.admin.css',
    ),
    'library' => array(
      array('system', 'ui.dialog'),
    ),
    'js' => array(
      drupal_get_path('module', 'islandora_solr') . '/js/islandora_solr.admin.js',
    ),
  );
  $form['islandora_solr_tabs'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 5,
  );
  // Display profiles.
  $form['display_profiles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display profiles'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Primary displays.
  $form['display_profiles']['islandora_solr_primary_display_table'] = array(
    '#type' => 'item',
    '#title' => t('Primary display profiles'),
    '#description' => t('Preferred normal display profile for search results. These may be provided by third-party modules.'),
     // This attribute is important to return the submitted values in a deeper
     // nested arrays in.
    '#tree' => TRUE,
    '#theme' => 'islandora_solr_admin_primary_display',
  );

  // Get the table settings.
  $primary_display_array = variable_get('islandora_solr_primary_display_table', array());
  // Get all defined primary displays.
  $profiles = module_invoke_all("islandora_solr_primary_display");

  // If any primary display profiles are found.
  if (!empty($profiles)) {
    $profiles_sorted = array();
    // If the table settings are set, we change the order of the table rows.
    if (!empty($primary_display_array)) {
      // Set weight variable.
      $weight = $primary_display_array['weight'];
      // First sort by value and then sort equal values by key:
      // @see http://stackoverflow.com/a/6611077/477949
      array_multisort(array_values($weight), SORT_ASC, array_keys($weight), SORT_ASC, $weight);
      // Add all previously existing profiles with a weight...
      foreach (array_intersect_key($weight, $profiles) as $key => $value) {
        $profiles_sorted[$key] = $profiles[$key];
      }
      // Account for new profiles.
      foreach (array_diff_key($profiles, $profiles_sorted) as $key => $value) {
        $profiles_sorted[$key] = $value;
        // Add weight for new profile (heaviest +1).
        $primary_display_array['weight'][$key] = end($weight) + 1;
      }
    }
    // Or else use the default.
    else {
      // Only apply when there's no sort variable available.
      // Sort by key.
      ksort($profiles);
      $profiles_sorted = $profiles;
    }

    // Table loop.
    foreach ($profiles_sorted as $machine_name => $profile) {
      // Incremetally add every display profile to the options array.
      $options[$machine_name] = '';

      // Human name.
      $form['display_profiles']['islandora_solr_primary_display_table']['name'][$machine_name] = array(
        '#type' => 'item',
        '#markup' => $profile['name'],
      );
      // Machine name.
      $form['display_profiles']['islandora_solr_primary_display_table']['machine_name'][$machine_name] = array(
        '#type' => 'item',
        '#markup' => $machine_name,
      );
      // Weight.
      $form['display_profiles']['islandora_solr_primary_display_table']['weight'][$machine_name] = array(
        '#type' => 'weight',
        '#default_value' => (isset($primary_display_array['weight'][$machine_name])) ? $primary_display_array['weight'][$machine_name] : 0,
        '#attributes' => array('class' => array('solr-weight')),
      );
      // Configuration URL.
      $form['display_profiles']['islandora_solr_primary_display_table']['configuration'][$machine_name] = array(
        '#type' => 'item',
        '#markup' => (isset($profile['configuration']) && $profile['configuration'] != '') ? l(t('configure'), $profile['configuration']) : '',
      );
    }
    // Default display.
    $form['display_profiles']['islandora_solr_primary_display_table']['default'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => variable_get('islandora_solr_primary_display', 'default'),
    );
    // Enabled display.
    $form['display_profiles']['islandora_solr_primary_display_table']['enabled'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => (!empty($primary_display_array)) ? $primary_display_array['enabled'] : array(),
    );
  }

  // Secondary profiles.
  $profiles = module_invoke_all("islandora_solr_secondary_display");
  ksort($profiles);
  foreach ($profiles as $machine_name => $profile) {
    $islandora_solr_secondary_display_options[$machine_name] = $profile['name'];
  }
  if (!empty($islandora_solr_secondary_display_options)) {
    $form['display_profiles']['islandora_solr_secondary_display'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Secondary display profiles'),
      '#options' => $islandora_solr_secondary_display_options,
      '#default_value' => variable_get('islandora_solr_secondary_display', array()),
      '#description' => "Enabled secondary output/download types for search results.",
    );
  }

  // Default display settings.
  $form['islandora_solr_tabs']['default_display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default display settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Display fields.
  $terms = array(
    '#type' => 'item',
    '#title' => t('Display fields'),
    '#description' => t('Set labels for Solr fields to be included in the search results.'),
    '#tree' => TRUE,
    '#prefix' => '<div id="islandora-solr-result-fields-wrapper">',
    '#suffix' => '</div>',
    '#field_type' => 'result_fields',
    '#theme' => 'islandora_solr_admin_fields',
  );

  // Create terms/fields.
  islandora_solr_admin_settings_fields($form_state, $terms, 'result_fields');

  // Result fields.
  $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_result_fields'] = $terms;

  // Other results settings.
  $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_limit_result_fields'] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit results to fields listed above?'),
    '#return_value' => 1,
    '#default_value' => variable_get('islandora_solr_limit_result_fields', 0),
    '#description' => t('If checked, results displayed will be limited to the fields specified above. The order of the display fields is only enforced when this is enabled.<br /><strong>Note:</strong> some display profiles may not honour this value.'),
  );
  $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_num_of_results'] = array(
    '#type' => 'textfield',
    '#title' => t('Results per page'),
    '#size' => 5,
    '#description' => t('Default number of results to show per page.'),
    '#default_value' => variable_get('islandora_solr_num_of_results', '20'),
  );
  $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_num_of_results_advanced'] = array(
    '#type' => 'textfield',
    '#title' => t('Advanced results per page'),
    '#size' => 5,
    '#description' => t('Comma seperated list of integers to increase or decrease results per page.'),
    '#default_value' => implode(',', variable_get('islandora_solr_num_of_results_advanced', array())),
  );
  $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_search_navigation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable search navigation block'),
    '#description' => t('Add navigation params to object links.'),
    '#default_value' => variable_get('islandora_solr_search_navigation', FALSE),
  );
  $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_search_field_value_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Field value separator'),
    '#description' => t('Characters to separate values in multivalued fields. If left empty it will default to @value.', array('@value' => '", "')),
    '#default_value' => variable_get('islandora_solr_search_field_value_separator', ', '),
  );
  $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_search_truncated_field_value_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Truncated field value separator'),
    '#description' => t('Characters to separate truncated values in multivalued fields. If left empty it will default to line break.'),
    '#default_value' => variable_get('islandora_solr_search_truncated_field_value_separator', "<br />"),
  );
  // Sort settings.
  $form['islandora_solr_tabs']['sort'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sort settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Sort terms.
  $sort_terms = array(
    '#type' => 'item',
    '#title' => t('Sort fields'),
    '#description' => t('Indicates what fields should appear in the <strong>Islandora sort block</strong>. To sort on relevancy, use the \'score\' field.<br /><strong>Note:</strong> not all fields are sortable. For more information, check the <a href="!url">Solr documentation</a>.', array('!url' => 'http://wiki.apache.org/solr/CommonQueryParameters#sort')),
    '#tree' => TRUE,
    '#prefix' => '<div id="islandora-solr-sort-fields-wrapper">',
    '#suffix' => '</div>',
    '#field_type' => 'sort_fields',
    '#theme' => 'islandora_solr_admin_fields',
  );

  // Create terms/fields.
  islandora_solr_admin_settings_fields($form_state, $sort_terms, 'sort_fields');

  // Sort fields.
  $form['islandora_solr_tabs']['sort']['islandora_solr_sort_fields'] = $sort_terms;

  // Facet settings.
  $form['islandora_solr_tabs']['facet_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facet settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Facet terms.
  $facet_terms = array(
    '#type' => 'item',
    '#title' => t('Facet fields'),
    '#description' => t('Indicate what fields should appear as <strong>facets</strong>.<br /><strong>Note:</strong> it is recommended to use non-tokenized Solr fields (full literal strings).'),
    '#tree' => TRUE,
    '#prefix' => '<div id="islandora-solr-facet-fields-wrapper">',
    '#suffix' => '</div>',
    '#field_type' => 'facet_fields',
    '#theme' => 'islandora_solr_admin_fields',
  );

  // Create terms/fields.
  islandora_solr_admin_settings_fields($form_state, $facet_terms, 'facet_fields');

  // Facet fields.
  $form['islandora_solr_tabs']['facet_settings']['islandora_solr_facet_fields'] = $facet_terms;

  $form['islandora_solr_tabs']['facet_settings']['islandora_solr_facet_min_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum limit'),
    '#size' => 5,
    '#description' => t('The minimum number of results required to display a facet'),
    '#default_value' => variable_get('islandora_solr_facet_min_limit', '2'),
  );
  $form['islandora_solr_tabs']['facet_settings']['islandora_solr_facet_soft_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Soft limit'),
    '#size' => 5,
    '#description' => t('The number of results which should be displayed initially. If there are more, then the a "Show more" button will allow the rest up to the value below to be displayed. Use 0 to disable.'),
    '#default_value' => variable_get('islandora_solr_facet_soft_limit', '10'),
  );
  $form['islandora_solr_tabs']['facet_settings']['islandora_solr_facet_max_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum limit'),
    '#size' => 5,
    '#description' => t('The maximum number of terms that should be returned to the user. For example, if there are 100 possible subjects in a faceted result you may wish to only return the top 10.'),
    '#default_value' => variable_get('islandora_solr_facet_max_limit', '20'),
  );

  // Advanced search block.
  $form['islandora_solr_tabs']['advanced_search_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced search block'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $search_terms = array(
    '#type' => 'item',
    '#title' => t('Search terms'),
    '#description' => t(
      'Indicates what fields should appear in the dropdown menu of terms for
      the <strong>Advanced Search Block</strong>.<br /><strong>Note:</strong>
      it is recommended to use tokenized fields and non-tokenized string fields
      will not match correctly.'
    ),
    '#tree' => TRUE,
    '#prefix' => '<div id="islandora-solr-search-fields-wrapper">',
    '#suffix' => '</div>',
    '#field_type' => 'search_fields',
    '#theme' => 'islandora_solr_admin_fields',
  );

  // Create terms/fields.
  islandora_solr_admin_settings_fields($form_state, $search_terms, 'search_fields');

  // Search fields.
  $form['islandora_solr_tabs']['advanced_search_block']['islandora_solr_search_fields'] = $search_terms;

  $form['islandora_solr_tabs']['advanced_search_block']['islandora_solr_search_boolean'] = array(
    '#type' => 'radios',
    '#title' => t('Default boolean operator'),
    '#default_value' => variable_get('islandora_solr_search_boolean', 'user'),
    '#options' => array(
      'user' => t('User-configurable'),
      'AND' => t('AND'),
      'OR' => t('OR'),
    ),
    '#description' => t('Select a default boolean operator for the search query. Selecting "User-configurable" exposes a dropdown menu which gives the user a choice between AND, OR and NOT.'),
  );
  $form['islandora_solr_tabs']['advanced_search_block']['islandora_solr_allow_preserve_filters'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Preservation of Filters'),
    '#default_value' => variable_get('islandora_solr_allow_preserve_filters', FALSE),
    '#description' => t('Allow users to preserve filters when changing their search.'),
  );
  $form['islandora_solr_tabs']['advanced_search_block']['islandora_solr_human_friendly_query_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Human-friendly Current Query'),
    '#default_value' => variable_get('islandora_solr_human_friendly_query_block', TRUE),
    '#description' => t('Use labels instead of raw field names when displaying an advanced search in the Current Query block.'),
  );

  $form['islandora_solr_tabs']['advanced_search_block']['islandora_solr_advanced_search_block_lucene_syntax_escape'] = array(
    '#title' => t('Escape Lucene special characters'),
    '#description' => t('Allow the use of lucene syntax string escaping on search terms'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('islandora_solr_advanced_search_block_lucene_syntax_escape', FALSE),
  );

  $form['islandora_solr_tabs']['advanced_search_block']['islandora_solr_advanced_search_block_lucene_regex_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default regular expression evaluated on search term'),
    '#default_value' => variable_get('islandora_solr_advanced_search_block_lucene_regex_default', ISLANDORA_SOLR_QUERY_FACET_LUCENE_ESCAPE_REGEX_DEFAULT),
    '#description' => t(
      "The default regular expression, used to escape characters when found in search terms. Defaults to @regex",
      array(
        '@regex' => ISLANDORA_SOLR_QUERY_FACET_LUCENE_ESCAPE_REGEX_DEFAULT,
      )
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_solr_advanced_search_block_lucene_syntax_escape"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Query defaults.
  $form['islandora_solr_tabs']['query_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Query defaults'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['islandora_solr_tabs']['query_defaults']['islandora_solr_namespace_restriction'] = array(
    '#type' => 'textarea',
    '#title' => t('Limit results to specific namespaces'),
    '#default_value' => variable_get('islandora_solr_namespace_restriction', ''),
    '#description' => t("Enter a space- or comma-separated list of namespaces (for example, 'demo, default') to restrict results to PIDs within those namespaces."),
  );
  $form['islandora_solr_tabs']['query_defaults']['islandora_solr_base_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr default query'),
    '#size' => 30,
    '#description' => t('A default query used to browse Solr results when no explicit user query is set.
      Setting a useful default query allows the use of Solr to browse without having to enter a query.
      This may be used in conjunction with a background filter below.<br />
      Consider using <strong>fgs_createdDate_dt:[* TO NOW]</strong> or <strong>*:*</strong><br />'),
    '#default_value' => variable_get('islandora_solr_base_query', '*:*'),
  );
  $form['islandora_solr_tabs']['query_defaults']['islandora_solr_base_advanced'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use default query for empty advanced searches too'),
    '#description' => t('When selected, an empty advanced search will perform the same as an empty simple search. If not selected, empty advanced searches will search *:*'),
    '#default_value' => variable_get('islandora_solr_base_advanced', FALSE),
  );
  $form['islandora_solr_tabs']['query_defaults']['islandora_solr_base_sort'] = array(
    '#type' => 'textfield',
    '#title' => t('Sort field for default query'),
    '#size' => 30,
    '#description' => t('Indicates which field should define the sort order for the default query.<br />
    For example: <strong>fgs_createdDate_dt desc</strong>.<br /><strong>Note:</strong> only single-valued fields are sortable.
    For more information, see "sort" as discussed in the "Common query parameters" section of the 
    <a href="!url">Solr documentation</a>.', array('!url' => 'http://archive.apache.org/dist/lucene/solr/ref-guide/')),
    '#default_value' => variable_get('islandora_solr_base_sort', ''),
  );
  $form['islandora_solr_tabs']['query_defaults']['islandora_solr_base_filter'] = array(
    '#type' => 'textarea',
    '#title' => t('Solr base filter'),
    '#description' => t('Lists base filters that are appended to all user queries. This may be used to filter results and facilitate null-query browsing. Enter one filter per line. <br />
      These filters will be applied to all queries in addition to any user-selected facet filters'),
    '#default_value' => variable_get('islandora_solr_base_filter', ''),
    '#wysiwyg' => FALSE,
  );
  $form['islandora_solr_tabs']['query_defaults']['islandora_solr_query_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Query fields'),
    '#default_value' => variable_get('islandora_solr_query_fields', 'dc.title^5 dc.subject^2 dc.description^2 dc.creator^2 dc.contributor^1 dc.type'),
    '#description' => t('Query fields to use for DisMax (simple) searches. See the QueryFields section in the <a href="@url" target="_blank" title="Solr documentation">Solr documentation</a> for examples.', array(
      '@url' => 'http://archive.apache.org/dist/lucene/solr/ref-guide/',
    )),
  );
  $form['islandora_solr_tabs']['query_defaults']['islandora_solr_use_ui_qf'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefer defined query fields?'),
    '#default_value' => variable_get('islandora_solr_use_ui_qf', FALSE),
    '#description' => t('Use the above "@qf" by default; otherwise, they will only be used as a fallback in the case there are none defined in the selected request handler.', array(
      '@qf' => t('Query fields'),
    )),
  );

  // Required Solr fields.
  $form['islandora_solr_tabs']['required_solr_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Required Solr fields'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Content Model Solr field.
  $form['islandora_solr_tabs']['required_solr_fields']['islandora_solr_content_model_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Content model Solr field'),
    '#size' => 30,
    '#description' => t('Solr field containing the content model URIs. This should be a multivalued string field'),
    '#default_value' => variable_get('islandora_solr_content_model_field', 'RELS_EXT_hasModel_uri_ms'),
    '#required' => TRUE,
  );
  // Present datastreams Solr field.
  $form['islandora_solr_tabs']['required_solr_fields']['islandora_solr_datastream_id_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Datastream ID Solr field'),
    '#size' => 30,
    '#description' => t('Solr field containing the populated datastream IDs.
      This should be a multivalued string field. If this field is not populated,
      the DSID of TN will be assumed valid for thumbnails.'),
    '#default_value' => variable_get('islandora_solr_datastream_id_field', 'fedora_datastreams_ms'),
    '#required' => TRUE,
  );
  // Label Solr field.
  $form['islandora_solr_tabs']['required_solr_fields']['islandora_solr_object_label_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Object label Solr field'),
    '#size' => 30,
    '#description' => t("The Solr field containing an object's label. This should be a single valued string field."),
    '#default_value' => variable_get('islandora_solr_object_label_field', 'fgs_label_s'),
    '#required' => TRUE,
  );
  // The isMemberOf Solr field.
  $form['islandora_solr_tabs']['required_solr_fields']['islandora_solr_member_of_field'] = array(
    '#type' => 'textfield',
    '#title' => t('The isMemberOf Solr field'),
    '#size' => 30,
    '#description' => t("The Solr field containing an object's isMemberOf relationship"),
    '#default_value' => variable_get('islandora_solr_member_of_field', 'RELS_EXT_isMemberOf_uri_ms'),
    '#required' => TRUE,
  );
  // The isMemberOfCollection Solr field.
  $form['islandora_solr_tabs']['required_solr_fields']['islandora_solr_member_of_collection_field'] = array(
    '#type' => 'textfield',
    '#title' => t('The isMemberOfCollection Solr field'),
    '#size' => 30,
    '#description' => t("The Solr field containing an object's isMemberOfCollection relationship"),
    '#default_value' => variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms'),
    '#required' => TRUE,
  );
  // Miscellaneous.
  $form['islandora_solr_tabs']['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Debug mode.
  $form['islandora_solr_tabs']['other']['islandora_solr_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode?'),
    '#return_value' => 1,
    '#default_value' => variable_get('islandora_solr_debug_mode', 0),
    '#description' => t('Dumps Solr queries to the screen for testing. Warning: if you have the Drupal Apache Solr module enabled alongside this one, the debug function will not work.'),
    '#weight' => 6,
  );
  // Luke timeout.
  $form['islandora_solr_tabs']['other']['islandora_solr_luke_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Luke timeout'),
    '#size' => 10,
    '#description' => t("Number of seconds to set timeout for retrieving fields from Solr's Luke interface. Increase this if autocomplete fields containing Solr field names time out."),
    '#default_value' => variable_get('islandora_solr_luke_timeout', '45'),
    '#required' => TRUE,
  );

  // The content of the popup dialog.
  $form['islandora_solr_admin_dialog'] = array(
    '#theme_wrappers' => array('container'),
    '#id' => 'islandora-solr-admin-dialog',
    '#weight' => 50,
  );
  $form['islandora_solr_admin_dialog']['title'] = array(
    '#markup' => '<h2 id="islandora-solr-admin-dialog-title"></h2>',
  );
  $form['islandora_solr_admin_dialog']['body'] = array(
    '#theme_wrappers' => array('container'),
    '#id' => 'islandora-solr-admin-dialog-body',
    '#markup' => t('Default dialog text'),
  );

  // Actions.
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Solr configuration'),
    '#weight' => 0,
    '#submit' => array('_islandora_solr_admin_settings_submit'),
    '#validate' => array('_islandora_solr_admin_settings_validate'),
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('_islandora_solr_admin_settings_submit'),
  );

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('Error: the settings have not been saved.'), 'error');
  }
  return $form;
}

/**
 * Confirmation for reset solr settings.
 *
 * @param array $form
 *   Array containing all form elements.
 * @param array $form_state
 *   The form_state array of the form. Includes the values.
 *
 * @return array
 *   Rendered confirmation form.
 */
function islandora_solr_admin_settings_default_confirm_form(array $form, array &$form_state) {
  $form = confirm_form($form,
    t('Confirm settings reset to default'),
    'admin/islandora/search/islandora_solr/settings/',
    t('Confirm reset settings to default, this cannot be undone.'),
    t('Continue'),
    t('Cancel')
  );
  return $form;
}

/**
 * If user confirms reset solr settings delete settings.
 *
 * Iterate through an array of Solr settings and remove the present values,
 * redirect user back to settings page.
 *
 * @param array $form
 *   Array containing all form elements.
 * @param array $form_state
 *   The form_state array of the form. Includes the values.
 */
function islandora_solr_admin_settings_default_confirm_form_submit(array $form, array &$form_state) {
  module_load_include('install', 'islandora_solr');
  $vars = islandora_solr_search_settings_variables();
  array_walk($vars, 'variable_del');
  db_delete('islandora_solr_fields')->execute();
  drupal_set_message(t('The configuration options have been reset to their default values.'));
  $form_state['redirect'] = 'admin/islandora/search/islandora_solr/settings/';
}

/**
 * Generates fields for the admin fields table.
 *
 * First collects values to populate the table, then render the fields.
 *
 * @param array $form_state
 *   The form_state array of the form. Includes submitted values.
 * @param array $terms
 *   Terms array passed by reference. A base form element for the table is
 *   already created and new form elements are added in this function.
 * @param string $field_type
 *   An identifier for the field table which is being created.
 *
 * @link http://drupal.stackexchange.com/questions/14855/how-do-i-dynamically-fill-a-textfield-with-ajax/16576#16576 Stackexchange Post @endlink
 */
function islandora_solr_admin_settings_fields(&$form_state, &$terms, $field_type) {

  // Set variables.
  $field_type_class = str_replace('_', '-', $field_type);

  // Ways to populate the fields table:
  // 1. Saved values from the DB
  // 2. Triggered element
  // 2.a. Add field
  // 2.b. Remove field
  // 2.c. Configure field:
  // - Prepare array to populate dialog.
  // 2.d. Dialog submit:
  // - Process values from modal dialog: add variables to form_state array and
  //   update the table info.
  // 1. Saved values from the DB.
  if ($form_state['rebuild'] == FALSE) {

    // Get values from DB unfiltered, not simplified and incremental keys.
    $records = islandora_solr_get_fields($field_type, FALSE, FALSE);

    // Set fields to populate the admin table.
    $fields = array();
    foreach ($records as $key => $record) {
      $fields[$key] = array(
        'solr_field' => $record['solr_field'],
        'field_type' => $record['field_type'],
        'weight' => $record['weight'],
      );
    }
    // Move solr_field_settings into the form state.
    foreach ($records as $key => $value) {
      $solr_field = $value['solr_field'];
      $solr_field_settings = $value['solr_field_settings'];
      $form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field] = $solr_field_settings;
    }
  }

  // 2. Triggered element (Add item or remove).
  if (isset($form_state['triggering_element']) &&
      $form_state['triggering_element']['#field_type'] == $field_type) {
    // Unset terms from input array.
    $form_state['values']['islandora_solr_' . $field_type]['terms'] = isset($form_state['input']['islandora_solr_' . $field_type]['terms']) ? $form_state['input']['islandora_solr_' . $field_type]['terms'] : array();
    unset($form_state['input']['islandora_solr_' . $field_type]);

    // 2.a add field.
    if ($form_state['triggering_element']['#name'] == $field_type . '_add_more') {
      // Get new solr field value.
      $add_solr_field = $form_state['values']['islandora_solr_' . $field_type]['add_solr_field'];
      // Check if the value is empty.
      $solr_field_error = NULL;
      if (empty($add_solr_field)) {
        $solr_field_error = t('Field cannot be empty.');
      }
      // Check if value is already added.
      $check_terms = $form_state['values']['islandora_solr_' . $field_type]['terms'];

      foreach ($check_terms as $key => $term) {
        if (isset($term['solr_field']) && $term['solr_field'] == $add_solr_field) {
          $solr_field_error = t('Solr term can only be added once.');
        }
      }
      // Add new field to values.
      if ($solr_field_error == NULL) {
        $form_state['values']['islandora_solr_' . $field_type]['terms'][] = array('solr_field' => $add_solr_field);
      }
    }
    // 2.b. remove field.
    if ($form_state['triggering_element']['#value'] == t('Remove')) {
      // Triggered field #.
      $field = $form_state['triggering_element']['#field'];
      $solr_field = $form_state['values']['islandora_solr_' . $field_type]['terms'][$field]['solr_field'];
      // Remove field.
      array_splice($form_state['values']['islandora_solr_' . $field_type]['terms'], $field, 1);
      // Remove field values.
      unset($form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field]);
    }
    // 2.c. configure field.
    if ($form_state['triggering_element']['#value'] == t('Configure')) {
      // Triggered field #.
      $field = $form_state['triggering_element']['#field'];
      // Get field + dialog values and merge.
      $values = $form_state['values']['islandora_solr_' . $field_type]['terms'][$field];
      $solr_field = $form_state['values']['islandora_solr_' . $field_type]['terms'][$field]['solr_field'];
      $solr_field_settings = (array) drupal_array_get_nested_value($form_state, array(
        'solr_field_settings',
        'islandora_solr_' . $field_type,
        $solr_field,
      ));
      $values = array_merge_recursive($values, $solr_field_settings);
      // Prepare dialog values.
      $dialog_id = 'edit-islandora-solr-' . $field_type_class . '-terms-' . $field . '-dialog-button';
      $form_state['dialog'] = array(
        'solr_field' => $solr_field,
        'field_type' => $field_type,
        'dialog_id' => $dialog_id,
        'values' => $values,
      );
    }
    // 2.d. Dialog submit.
    if ($form_state['triggering_element']['#value'] == t('Dialog')) {
      // Triggered field #.
      $field = $form_state['triggering_element']['#field'];
      // Get $solr_field.
      $solr_field = $form_state['values']['islandora_solr_' . $field_type]['terms'][$field]['solr_field'];
      // Get dialog values from recent dialog submit.
      $dialog_values = $form_state['input']['_dialog_values'] = json_decode($form_state['input']['_dialog_values'], TRUE);
      // Save the dialog values in the form state.
      $form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field] = _islandora_solr_handle_solr_field_settings($dialog_values, $field_type);
    }
  }

  // Prepare fields array.
  if (isset($form_state['values']['islandora_solr_' . $field_type]['terms'])) {
    $fields = $form_state['values']['islandora_solr_' . $field_type]['terms'];
  }

  // If no fields available set string.
  if (empty($fields)) {
    $terms['no_fields'] = array(
      '#markup' => t('No fields defined'),
    );
  }
  // Else render fields.
  else {
    $term = array();
    $count = count($fields);
    $fields = array_values($fields);
    foreach ($fields as $key => $value) {
      $solr_field = $value['solr_field'];

      $term['draggable_handler'] = array(
        '#type' => 'item',
        '#markup' => '',
      );
      $term['solr_field_render'] = array(
        '#type' => 'item',
        '#markup' => isset($value['solr_field']) ? $value['solr_field'] : '',
      );
      $term['solr_field'] = array(
        '#type' => 'hidden',
        '#value' => isset($value['solr_field']) ? $value['solr_field'] : '',
      );
      $solr_field_settings = NULL;
      if (isset($form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field]['label'])) {
        $solr_field_settings = function_exists('i18n_string') ?
          i18n_string("islandora_solr:field_settings:$field_type:label:$solr_field", $form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field]['label']) :
          $form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field]['label'];
      }
      $term['solr_field_settings'] = array(
        '#markup' => $solr_field_settings ? t('Label: @label', array('@label' => $solr_field_settings)) : '',
      );
      $term['weight'] = array(
        '#type' => 'weight',
        '#default_value' => isset($value['weight']) ? $value['weight'] : $key,
        '#delta' => $count >= 10 ? $count : 10,
        '#attributes' => array('class' => array('solr-weight-' . $field_type_class)),
      );
      $term['configure'] = array(
        '#type' => 'link',
        '#title' => t('configure'),
        '#href' => '#',
        '#options' => array('attributes' => array('class' => 'islandora-solr-configure-link')),
      );
      $term['configure_button'] = array(
        '#type' => 'submit',
        '#value' => t('Configure'),
        '#field' => $key,
        '#field_type' => $field_type,
        '#attributes' => array('class' => array('islandora-solr-configure-submit')),
        '#submit' => array('_islandora_solr_update_fields_submit'),
        '#name' => $field_type . '_configure_' . $key,
        '#id' => 'edit-islandora-solr-' . $field_type_class . '-terms-' . $key . '-configure-button',
        '#ajax' => array(
          'callback' => '_islandora_solr_admin_settings_field_configure',
          'wrapper' => 'islandora-solr-' . $field_type_class . '-wrapper',
          'effect' => 'none',
          'event' => 'click',
          'progress' => array('message' => '', 'type' => 'throbber'),
        ),
      );
      $term['remove'] = array(
        '#type' => 'link',
        '#title' => t('remove'),
        '#href' => '#',
        '#options' => array('attributes' => array('class' => 'islandora-solr-remove-link')),
      );
      $term['remove_button'] = array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#field' => $key,
        '#field_type' => $field_type,
        '#attributes' => array('class' => array('islandora-solr-remove-submit')),
        '#submit' => array('_islandora_solr_update_fields_submit'),
        '#name' => $field_type . '_remove_' . $key,
        '#id' => 'edit-islandora-solr-' . $field_type_class . '-terms-' . $key . '-remove-button',
        '#ajax' => array(
          'callback' => '_islandora_solr_update_fields',
          'wrapper' => 'islandora-solr-' . $field_type_class . '-wrapper',
          'effect' => 'none',
          'event' => 'click',
          'progress' => array('message' => '', 'type' => 'throbber'),
        ),
      );
      $term['dialog_button'] = array(
        '#type' => 'submit',
        '#value' => t('Dialog'),
        '#field' => $key,
        '#field_type' => $field_type,
        '#attributes' => array('class' => array('islandora-solr-dialog-submit')),
        '#submit' => array('_islandora_solr_update_fields_submit'),
        '#name' => $field_type . '_dialog_' . $key,
        '#id' => 'edit-islandora-solr-' . $field_type_class . '-terms-' . $key . '-dialog-button',
        '#ajax' => array(
          'callback' => '_islandora_solr_update_fields',
          'wrapper' => 'islandora-solr-' . $field_type_class . '-wrapper',
          'effect' => 'none',
          'event' => 'click',
          'progress' => array('message' => '', 'type' => 'throbber'),
        ),
      );
      $terms['terms'][] = $term;
    }
  }
  $terms['add_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Add another item'),
    '#size' => 45,
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#default_value' => '',
  );
  if (isset($solr_field_error) && !empty($solr_field_error)) {
    $terms['add_solr_field']['#title'] .= '<br /><span class="solr-admin-error">' . $solr_field_error . '</span>';
    $terms['add_solr_field']['#attributes'] = array('class' => array('error'));
  }

  $terms['add_more'] = array(
    '#type' => 'button',
    '#value' => t('Add'),
    '#attributes' => array('class' => array('islandora-solr-add-more-submit')),
    '#submit' => array('_islandora_solr_update_fields_submit'),
    '#name' => $field_type . '_add_more',
    '#field_type' => $field_type,
    '#id' => 'edit-islandora-solr-' . $field_type_class . '-add-more',
    '#ajax' => array(
      'callback' => '_islandora_solr_update_fields',
      'wrapper' => 'islandora-solr-' . $field_type_class . '-wrapper',
      'event' => 'click',
      'effect' => 'none',
    ),
  );
}

/**
 * Display admin form for breadcrumb field choice.
 *
 * @param array $form
 *   Array containing all form elements.
 * @param array $form_state
 *   The form_state array of the form. Includes the values.
 *
 * @return array
 *   The Drupal form object.
 */
function islandora_solr_admin_breadcrumbs_settings(array $form, array &$form_state) {
  $form = array();
  $form['islandora_solr_breadcrumbs_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumbs'),
  );
  $form['islandora_solr_breadcrumbs_admin']['admin'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Enable Islandora Solr for Breadcrumbs'), 'admin/islandora/configure'),
  );
  $form['islandora_solr_breadcrumbs_admin']['islandora_solr_breadcrumbs_parent_fields'] = array(
    '#type' => 'textarea',
    '#title' => t('Solr Parent Fields'),
    '#description' => t('A list of Solr fields containing the PIDs of parent objects,
    one per line. Will search top to bottom and stop on the first hit.'),
    '#default_value' => variable_get('islandora_solr_breadcrumbs_parent_fields', "RELS_EXT_isMemberOfCollection_uri_ms\r\nRELS_EXT_isMemberOf_uri_ms"),
  );
  $form['islandora_solr_breadcrumbs_admin']['islandora_solr_breadcrumbs_add_collection_query'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append query breadcrumbs to collection breadcrumbs'),
    '#description' => t('Appends any additional available breadcrumbs, such as facet breadcrumbs, to the standard collection hierarchy breadcrumbs, if using the Solr collection query backend.'),
    '#default_value' => variable_get('islandora_solr_breadcrumbs_add_collection_query', FALSE),
  );
  return system_settings_form($form);
}

/**
 * Theme function to create an admin table for result fields.
 *
 * @param array $variables
 *   Array containing form elements to be themed.
 *
 * @return string
 *   Rendered table.
 */
function theme_islandora_solr_admin_fields($variables) {
  $form = $variables['form'];

  $field_type = $form['#field_type'];

  // Render class name from field type.
  $field_type_class = str_replace('_', '-', $field_type);

  $rows = array();

  // If no_fields is set, we don't render all the fields.
  if (isset($form['no_fields'])) {
    $row = array();
    $row[] = array('data' => drupal_render($form['no_fields']), 'colspan' => 10);

    // Add to rows.
    $rows[] = array('data' => $row);
  }
  else {
    foreach ($form['terms'] as $key => $element) {
      if (is_array($element) && element_child($key)) {
        $row = array();
        $row[] = array('data' => drupal_render($form['terms'][$key]['draggable_handler']), 'class' => 'islandora-solr-multiple-drag');
        $row[] = array('data' => drupal_render($form['terms'][$key]['solr_field_render']), 'class' => 'islandora-solr-solr-field-render');
        $row[] = array('data' => drupal_render($form['terms'][$key]['solr_field_settings']), 'class' => 'islandora-solr-solr-field-settings');
        $row[] = array('data' => drupal_render($form['terms'][$key]['weight']));
        $row[] = array('data' => drupal_render($form['terms'][$key]['configure']), 'class' => 'islandora-solr-operations');
        $row[] = array('data' => drupal_render($form['terms'][$key]['configure_button']), 'class' => 'islandora-solr-operations islandora-solr-operations-hidden');
        $row[] = array('data' => drupal_render($form['terms'][$key]['remove']), 'class' => 'islandora-solr-operations');
        $row[] = array('data' => drupal_render($form['terms'][$key]['remove_button']), 'class' => 'islandora-solr-operations islandora-solr-operations-hidden');
        $row[] = array('data' => drupal_render($form['terms'][$key]['dialog_button']), 'class' => 'islandora-solr-operations islandora-solr-operations-hidden');

        // Add to rows.
        $rows[] = array('data' => $row, 'class' => array('draggable'));
      }
    }
  }

  // Create header array.
  $header = array();
  $header[] = array(
    'data' => t('Solr field'),
    'colspan' => 2,
    'class' => 'islandora-solr-solr-field-header',
  );
  $header[] = array('data' => t('Settings'));
  $header[] = array('data' => t('Weight'), 'class' => 'islandora-solr-solr-field-weight');
  $header[] = array('data' => t('Operations'), 'colspan' => 4);

  // Controls table.
  $rows_controls = array();
  // Add another item form elements.
  $rows_controls[] = array(
    'data' => array(
      array(
        'data' => drupal_render($form['add_solr_field']) . drupal_render($form['add_more']),
        'colspan' => 10,
      ),
    ),
    'no_striping' => 1,
    'class' => array('add-item-controls'),
  );

  // Render form.
  $output = '';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'islandora-solr-' . $field_type_class,
      'class' => array('islandora-solr-fields-table'),
    ),
  ));
  $output .= theme('table', array(
    'header' => array(),
    'rows' => $rows_controls,
    'attributes' => array('class' => array('islandora-solr-table-controls')),
  ));
  $output .= drupal_render_children($form);
  // Add tabledrag features.
  drupal_add_tabledrag('islandora-solr-' . $field_type_class, 'order', 'sibling', 'solr-weight-' . $field_type_class);

  return $output;
}

/**
 * Theme function for the admin primary display table.
 *
 * @param array $variables
 *   An associative array, containing:
 *   - "render element": An array containing the form to render.
 *
 * @return string
 *   Rendered form element.
 *
 * @see islandora_solr_admin_settings()
 */
function theme_islandora_solr_admin_primary_display($variables) {
  $form = $variables['form'];

  $default = variable_get('islandora_solr_primary_display', 'default');
  $rows = array();

  foreach ($form['name'] as $key => $element) {
    // Do not take form control structures.
    if (is_array($element) && element_child($key)) {
      // Disable checkbox for the default display, because it cannot be
      // disabled.
      if ($key == $default) {
        $form['enabled'][$key]['#attributes']['disabled'] = 'disabled';
        $form['enabled'][$key]['#checked'] = TRUE;
      }
      $row = array();
      $row[] = array('data' => drupal_render($form['default'][$key]));
      $row[] = array('data' => drupal_render($form['enabled'][$key]));
      $row[] = array('data' => drupal_render($form['name'][$key]), 'title' => $key);
      $row[] = array('data' => drupal_render($form['machine_name'][$key]));
      $row[] = array('data' => drupal_render($form['weight'][$key]));
      $row[] = array('data' => drupal_render($form['configuration'][$key]));

      // Add to rows.
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }

  // Individual table headers:
  // - default
  // - enabled
  // - name
  // - Machine readable name
  // - weight
  // - configuration.
  $header = array();
  $header[] = array('data' => t('Default'));
  $header[] = array('data' => t('Enabled'));
  $header[] = array('data' => t('Name'));
  $header[] = array('data' => t('Machine-readable name'));
  $header[] = array('data' => t('Weight'));
  $header[] = array('data' => t('Configuration'));

  // Render form.
  $output = '';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'solr-primary-display-table'),
  ));
  $output .= drupal_render_children($form);

  // Add tabledrag features.
  drupal_add_tabledrag('solr-primary-display-table', 'order', 'sibling', 'solr-weight');

  return $output;
}

/**
 * Form validator.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form state.
 */
function _islandora_solr_admin_settings_validate($form, &$form_state) {
  $limits = explode(',', $form_state['values']['islandora_solr_num_of_results_advanced']);
  $form_state['values']['islandora_solr_num_of_results_advanced'] = array_filter(array_map('trim', $limits), 'is_numeric');
  $form_state['values']['islandora_solr_namespace_restriction'] = preg_replace('/:$/', '', $form_state['values']['islandora_solr_namespace_restriction']);
}

/**
 * Solr settings from submit.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form state.
 */
function _islandora_solr_admin_settings_submit($form, &$form_state) {

  // If this function is called not using submit or reset buttons.
  if ($form_state['clicked_button']['#id'] != 'edit-submit' && $form_state['clicked_button']['#id'] != 'edit-reset') {
    $form_state['rebuild'] = TRUE;
    return;
  }

  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  // @todo Remove once we assigned the values manually.
  // Exclude unnecessary elements.
  unset($form_state['values']['submit'],
      $form_state['values']['reset'],
      $form_state['values']['form_id'],
      $form_state['values']['op'],
      $form_state['values']['form_token'],
      $form_state['values']['form_build_id'],
      $form_state['values']['refresh_page']);

  // If the operation is 'Reset'.
  switch ($op) {
    // XXX: Using the translated string seems like a Bad Idea™.
    case t('Save Solr configuration'):
      // Save primary display table.
      $old_default = variable_get('islandora_solr_primary_display', 'default');
      $new_default = $form_state['values']['islandora_solr_primary_display_table']['default'];
      // Make default enabled.
      $form_state['values']['islandora_solr_primary_display_table']['enabled'][$new_default] = $new_default;
      $form_state['values']['islandora_solr_primary_display_table']['enabled'][$old_default] = $old_default;
      variable_set('islandora_solr_primary_display', $new_default);

      // Prepare all field variables to be saved to the DB.
      $insert_values = array();
      $field_types = array(
        'result_fields',
        'sort_fields',
        'facet_fields',
        'search_fields',
      );
      foreach ($field_types as $field_type) {
        if (isset($form_state['values']['islandora_solr_' . $field_type]['terms'])) {
          $result_fields = $form_state['values']['islandora_solr_' . $field_type]['terms'];
          foreach ($result_fields as $key => $value) {
            $solr_field = $value['solr_field'];
            $solr_field_settings = array();
            if (isset($form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field])) {
              $solr_field_settings = $form_state['solr_field_settings']['islandora_solr_' . $field_type][$solr_field];
              // Handle linking to objects to not break existing features while
              // adding new functionality.
              if ($field_type == 'result_fields') {
                if (isset($solr_field_settings['link_rendering'])) {
                  $link_choice = $solr_field_settings['link_rendering'];
                  $solr_field_settings['link_to_object'] = FALSE;
                  $solr_field_settings['link_to_search'] = FALSE;

                  if ($link_choice === 'object') {
                    $solr_field_settings['link_to_object'] = TRUE;
                  }
                  elseif ($link_choice === 'search') {
                    $solr_field_settings['link_to_search'] = TRUE;
                  }
                  unset($solr_field_settings['link_rendering']);
                }
              }
            }
            if (module_exists('i18n_string')) {
              $to_i18nize = array(
                'label',
                'boolean_facet_true_replacement',
                'boolean_facet_false_replacement',
              );
              foreach ($to_i18nize as $string_type) {
                if (isset($solr_field_settings[$string_type])) {
                  i18n_string_update("islandora_solr:field_settings:$field_type:$string_type:$solr_field", $solr_field_settings[$string_type]);
                }
                else {
                  i18n_string_remove("islandora_solr:field_settings:$field_type:$string_type:$solr_field");
                }
              }
            }
            $insert_values[] = array(
              'solr_field' => $solr_field,
              'field_type' => $field_type,
              'weight' => $value['weight'],
              'solr_field_settings' => serialize($solr_field_settings),
            );
          }
        }
      }

      // @todo Merge delete and save/insert into one query.
      // Clear the db fields table...
      db_delete('islandora_solr_fields')->execute();
      // And then populate them.
      $insert = db_insert('islandora_solr_fields')->fields(array(
        'solr_field',
        'field_type',
        'weight',
        'solr_field_settings',
      ));
      foreach ($insert_values as $record) {
        $insert->values($record);
      }
      $insert->execute();

      // Loop over all submitted values.
      unset($form_state['values']['islandora_solr_result_fields']);
      unset($form_state['values']['islandora_solr_sort_fields']);
      unset($form_state['values']['islandora_solr_facet_fields']);
      unset($form_state['values']['islandora_solr_search_fields']);
      foreach ($form_state['values'] as $key => $value) {
        // Prepare variables in array.
        if (is_array($value) && isset($form_state['values']['array_filter'])) {
          $value = array_keys(array_filter($value));
        }
        // Set variables.
        variable_set($key, $value);
      }

      drupal_set_message(t('The Solr configuration options have been saved.'));
      break;

    case t('Reset to defaults'):
      $form_state['redirect'] = 'admin/islandora/search/islandora_solr/settings/confirmation';
  }

  // Clear caches.
  // @todo Is this still necessary?
  cache_clear_all();
  drupal_theme_rebuild();
}

/**
 * Get available handlers.
 *
 * @param string $solr_url
 *   URL which points to Solr.
 *
 * @return array
 *   An associative array mapping the names of all request handlers found in
 *   the solrconfig.xml of the Solr instance to themselves.
 */
function _islandora_solr_get_handlers($solr_url) {
  $xml = islandora_solr_get_solrconfig_xml($solr_url);
  $handlers = array(
    FALSE => t('Let Solr decide'),
  );
  if ($xml) {
    $xpath = '//requestHandler[@class="solr.SearchHandler" and not(starts-with(@name, "/")) and not(@name="dismax") and not(@name="partitioned")]';
    foreach ($xml->xpath($xpath) as $handler) {
      $handler_name = (string) $handler['name'];
      $handlers[$handler_name] = $handler_name;
    }
  }
  else {
    drupal_set_message(t('Error retrieving @file from Solr.', array('@file' => 'solrconfig.xml')), 'warning');
  }
  return $handlers;
}

/**
 * AJAX callback from the admin form.
 *
 * Checks URL validity and refreshes request handler dropdown list.
 *
 * @param array $form
 *   An array containing the form structure.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   The element to replace.
 *
 * @see islandora_solr_admin_settings()
 */
function _islandora_solr_update_solr_url($form, $form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['rebuild'] = TRUE;

  return $form['solr_ajax_wrapper'];
}

/**
 * Submit button callback to update the Solr URL only.
 *
 * @param array $form
 *   An array containing the form structure.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @see islandora_solr_admin_settings()
 */
function _islandora_solr_admin_refresh($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['rebuild'] = TRUE;
}

/**
 * Checks for dismax.
 *
 * @param string $solr_url
 *   URL which point to Solr.
 * @param string $selected_handler
 *   Handler to check if dismax is allowed on it.
 *
 * @return bool
 *   TRUE if dismax is allowed, FALSE if not.
 *
 * @see islandora_solr_admin_settings()
 */
function _islandora_solr_check_dismax($solr_url, $selected_handler) {
  $xml = islandora_solr_get_solrconfig_xml($solr_url);

  $handlers = array();
  if ($xml) {
    if ($selected_handler) {
      $handlers = $xml->xpath("//requestHandler[@class='solr.SearchHandler' and @name='$selected_handler']");
    }
    else {
      $potential_handlers = $xml->xpath("//requestHandler[@class='solr.SearchHandler' and @default='true']");
      if ($potential_handlers) {
        // We don't have a handler name, but we do have a the default handler.
        $handlers = $potential_handlers;
      }
      else {
        $potential_handlers = $xml->xpath("//requestHandler[@class='solr.SearchHandler' and @name='/select']");
        if ($potential_handlers) {
          // We don't have a name or a declared, so look at the implied default.
          // Solr 3.6 seems to be able to store settings here which get applied
          // to all request handlers...
          // XXX: Might have to make this always get checked for completeness,
          // if it can really provide general defaults.
          $handlers = $potential_handlers;
        }
      }
    }
  }
  else {
    drupal_set_message(t('Error retrieving @file from Solr.', array('@file' => 'solrconfig.xml')), 'warning');
  }

  // An empty array evaluates to false... Let's coerce into a boolean.
  return TRUE == array_filter($handlers, function ($handler) {
    // XPath return an array of elements, and empty array evaluates to FALSE...
    // Coerce into a boolean, just because.
    return TRUE == $handler->xpath('lst/str[@name="qf"]');
  });
}

/**
 * Admin autocomplete callback which returns solr fields from Luke.
 *
 * @param string $string
 *   String filled out in the autocomplete textfield.
 *
 * @return json
 *   A json array containing the Solr luke values that contain the given string.
 */
function _islandora_solr_autocomplete_luke($string = '') {

  $luke = islandora_solr_get_luke();
  $result = array();
  foreach ($luke['fields'] as $term => $value) {
    if (stripos($term, $string) !== FALSE) {
      // Search case insensitive, but keep the case on replace.
      $term_str = preg_replace("/$string/i", "<strong>\$0</strong>", $term);

      // Add strong elements to highlight the found string.
      $result[$term] = $term_str . '<strong style="position: absolute; right: 5px;">(' . $value['type'] . ')</strong>';
    }
  }
  // Sort alphabetically.
  ksort($result);

  drupal_json_output($result);
  exit();
}

/**
 * Submit callback function for fields.
 *
 * Used by 'Add field', 'Configure', 'Remove' and 'Dialog'.
 *
 * @param array $form
 *   An associative array containing the form definition.
 * @param array $form_state
 *   An associative array containing the form state.
 */
function _islandora_solr_update_fields_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback function for fields.
 *
 * Used by 'Add field', 'Remove' and 'Dialog' links/buttons.
 *
 * @param array $form
 *   An associative array containing the form definition.
 * @param array $form_state
 *   An associative array containing the form state.
 *
 * @return array
 *   The part of the form array that needs asynchronous updating.
 */
function _islandora_solr_update_fields($form, &$form_state) {

  // Check what section the call is from.
  switch ($form_state['triggering_element']['#field_type']) {
    case 'result_fields':
      return $form['islandora_solr_tabs']['default_display_settings']['islandora_solr_result_fields'];

    case 'sort_fields':
      return $form['islandora_solr_tabs']['sort']['islandora_solr_sort_fields'];

    case 'facet_fields':
      return $form['islandora_solr_tabs']['facet_settings']['islandora_solr_facet_fields'];

    case 'search_fields':
      return $form['islandora_solr_tabs']['advanced_search_block']['islandora_solr_search_fields'];
  }
}

/**
 * AJAX callback function for 'Configure' link/button.
 *
 * Passes values from the field, loads the right form and populates and
 * triggers the modal dialog.
 *
 * @param array $form
 *   An associative array containing the form definition.
 * @param array $form_state
 *   An associative array containing the form state.
 *
 * @return array
 *   An array of AJAX commands.
 */
function _islandora_solr_admin_settings_field_configure($form, &$form_state) {
  $variables = $form_state['dialog'];
  $field_type = $form_state['dialog']['field_type'];
  $solr_field = $form_state['dialog']['solr_field'];
  $form_function = _islandora_solr_admin_get_form_function($field_type);

  $commands = array();

  // Open dialog.
  $commands[] = ajax_command_invoke('#islandora-solr-admin-dialog', 'dialog', array('open'));
  // Dialog title.
  $title = t('Configure field:') . ' ' . $solr_field;
  $commands[] = ajax_command_invoke('#islandora-solr-admin-dialog-title', 'html', array($title));
  // Dialog body.
  $form = drupal_get_form($form_function, $variables);
  $body = drupal_render($form);
  $commands[] = ajax_command_invoke('#islandora-solr-admin-dialog-body', 'html', array($body));
  // Attach behaviors (re-attaches javascript).
  $commands[] = ajax_command_invoke(NULL, 'islandoraSolrAttachBehaviors', array());

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * AJAX callback function for the modal dialog submit button.
 *
 * Closes the modal dialog and passes the values to the main form.
 *
 * @param array $form
 *   An associative array containing the form definition.
 * @param array $form_state
 *   An associative array containing the form state.
 *
 * @return array
 *   An array of AJAX commands.
 */
function _islandora_solr_admin_settings_dialog($form, &$form_state) {

  $dialog_id = $form_state['dialog']['dialog_id'];

  $commands = array();
  // Close dialog.
  $commands[] = ajax_command_invoke('#islandora-solr-admin-dialog', 'dialog', array('close'));

  // Call function which attaches the dialog form values to
  // Drupal.ajax.{dialog-button-id}.options.data.
  // After triggering the dialog ajax callback, the dialog values will be added
  // to the $form_state of the main form.
  $data = array(
    'id' => $dialog_id,
    'values' => $form_state['input'],
  );
  $commands[] = ajax_command_invoke(NULL, 'islandoraSolrDialogValues', array($data));
  // Trigger dialog button.
  $commands[] = ajax_command_invoke('#' . $dialog_id, 'trigger', array('click'));

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Select the fields from the raw input values the modal form.
 *
 * AJAX call fun.
 *
 * @todo This should probably be removed and values we don't need maybe should
 *   be filtered out before sending it back to the main form.
 */
function _islandora_solr_handle_solr_field_settings($solr_field_settings = NULL, $field_type = NULL) {
  $to_return = FALSE;

  if (!empty($solr_field_settings) && !empty($field_type)) {
    switch ($field_type) {
      case 'result_fields':
        $to_return = array(
          'label' => trim($solr_field_settings['label']),
          'link_rendering' => $solr_field_settings['link_rendering'],
          'snippet' => isset($solr_field_settings['snippet']) ? $solr_field_settings['snippet'] : NULL,
          'date_format' => isset($solr_field_settings['date_format']) ? trim($solr_field_settings['date_format']) : '',
          'truncation_type' => isset($solr_field_settings['truncation_type']) ? trim($solr_field_settings['truncation_type']) : 'separate_value_option',
          'maximum_length' => isset($solr_field_settings['maximum_length']) ? trim($solr_field_settings['maximum_length']) : '',
          'add_ellipsis' => isset($solr_field_settings['add_ellipsis']) ? $solr_field_settings['add_ellipsis'] : FALSE,
          'wordsafe' => isset($solr_field_settings['wordsafe']) ? $solr_field_settings['wordsafe'] : FALSE,
          'wordsafe_length' => isset($solr_field_settings['wordsafe_length']) ? $solr_field_settings['wordsafe_length'] : 1,
          'permissions' => empty($solr_field_settings['enable_permissions']) ? FALSE : $solr_field_settings['permissions'],
        );
        break;

      case 'sort_fields':
        $to_return = array(
          'label' => trim($solr_field_settings['label']),
          'permissions' => empty($solr_field_settings['enable_permissions']) ? FALSE : $solr_field_settings['permissions'],
        );
        break;

      case 'facet_fields':
        $fields = drupal_map_assoc(array(
          'label',
          'range_facet_select',
          'range_facet_variable_gap',
          'range_facet_start',
          'range_facet_end',
          'range_facet_gap',
          'date_facet_format',
          'range_facet_slider_enabled',
          'range_facet_slider_color',
          'date_filter_datepicker_enabled',
          'date_filter_datepicker_range',
          'pid_object_label',
          'boolean_facet_true_replacement',
          'boolean_facet_false_replacement',
          'sort_by',
        ));
        $relevant_values = array_intersect_key($solr_field_settings, $fields);
        $relevant_values['label'] = isset($relevant_values['label']) ?
          trim($relevant_values['label']) :
          '';
        $relevant_values['permissions'] = empty($solr_field_settings['enable_permissions']) ? FALSE : $solr_field_settings['permissions'];
        $to_return = $relevant_values;
        break;

      case 'search_fields':
        $to_return = array(
          'label' => trim($solr_field_settings['label']),
          'permissions' => empty($solr_field_settings['enable_permissions']) ? FALSE : $solr_field_settings['permissions'],
        );
        break;
    }
  }

  return $to_return;
}

/**
 * Function which returns the form function name associated with a field type.
 *
 * @param string $field_type
 *   Field type to get the form function name for.
 *
 * @return bool|string
 *   A form function name associated with the field type, or FALSE if field
 *   type is not given or does not exist.
 */
function _islandora_solr_admin_get_form_function($field_type = NULL) {
  if (!empty($field_type)) {
    switch ($field_type) {
      case 'result_fields':
        $form_callback = 'islandora_solr_admin_settings_result_fields';
        break;

      case 'sort_fields':
        $form_callback = 'islandora_solr_admin_settings_sort_fields';
        break;

      case 'facet_fields':
        $form_callback = 'islandora_solr_admin_settings_facet_fields';
        break;

      case 'search_fields':
        $form_callback = 'islandora_solr_admin_settings_search_fields';
        break;

      default:
        return FALSE;
    }
    return $form_callback;
  }
  return FALSE;
}

/**
 * @defgroup dialog-forms
 * @{
 * Form functions for the modal dialogs.
 */

/**
 * Genereate the element definition for the permission fieldset.
 *
 * @param bool|array $permissions
 *   The selected permissions (from the $form_state) either an array containing
 *   key values, or FALSE if it is not enabled.
 * @param array $permissions_default
 *   The permissions to select by default.
 * @param array $permissions_disable
 *   Some permissions which we will not allow to be changed (set as disabled in
 *   the form).
 * @param bool $default_value
 *   Whether the checkbox is to be checked by default.
 *
 * @return array
 *   An associative array containing the definition for the permissions
 *   fieldset.
 */
function islandora_solr_get_admin_permissions_fieldset($permissions, $permissions_default, $permissions_disable, $default_value) {
  if (is_array($permissions)) {
    $default_enable = !empty($permissions) ? TRUE : $default_value;
  }
  else {
    $default_enable = $permissions;
  }

  $permissions_fieldset = array(
    '#type' => 'fieldset',
    '#title' => t('Permissions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'enable_permissions' => array(
      '#type' => 'checkbox',
      '#title' => t('Enable permissions'),
      '#default_value' => $default_enable,
    ),
    'permissions' => array(
      '#type' => 'checkboxes',
      '#title' => t('Permissions'),
      '#options' => user_roles(),
      '#default_value' => !empty($permissions) ? $permissions : $permissions_default,
      '#description' => t('Select which roles can access this field.<br /><strong>Note:</strong> checkboxes may be grayed out for roles which do not have permission to search the Solr index.'),
      '#states' => array(
        'visible' => array(
          ':input[name="enable_permissions"]' => array('checked' => TRUE),
        ),
      ),
    ),
  );
  foreach ($permissions_disable as $rid) {
    $permissions_fieldset['permissions'][$rid] = array(
      '#disabled' => TRUE,
    );
  }

  return $permissions_fieldset;
}

/**
 * Form for result field settings.
 *
 * @params array $variables
 *   This parameter contains default values to be passed from the main form.
 */
function islandora_solr_admin_settings_result_fields($form, &$form_state, $variables) {
  form_load_include($form_state, 'inc', 'islandora_solr', 'includes/admin');

  $form_state['dialog'] = $variables;

  $solr_field = $variables['solr_field'];
  $values = $variables['values'];

  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('scroll')),
    '#id' => 'islandora-solr-admin-dialog-form',
  );
  $form['options']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($values['label']) ? $values['label'] : '',
    '#description' => t('A human-readable name.'),
  );
  $link_options = array(
    'none' => t('None'),
    'object' => t("Link this field to the object's page."),
    'search' => t("Link the value to a Solr search result. (NOTE: Will likely break with very large values.)"),
  );
  $default_link = 'none';

  if (isset($values['link_rendering'])) {
    $default_link = $values['link_rendering'];
  }
  elseif (isset($values['link_to_object']) && $values['link_to_object'] != FALSE) {
    $default_link = 'object';
  }
  elseif (isset($values['link_to_search']) && $values['link_to_search'] != FALSE) {
    $default_link = 'search';
  }
  $form['options']['link_rendering'] = array(
    '#type' => 'radios',
    '#title' => t('Linking'),
    '#options' => $link_options,
    '#default_value' => $default_link,
  );
  $form['options']['snippet'] = array(
    '#type' => 'checkbox',
    '#title' => t('Highlight'),
    '#default_value' => isset($values['snippet']) ? $values['snippet'] : FALSE,
    '#description' => t('If a match is found on this field, the search term will be highlighted.<br /><strong>Note:</strong> Only text that has been both indexed and stored may be highlighted. While highlighting on non-tokenized fields is possible, the best results are achieved using tokenized fields. This checkbox may be grayed out if the Solr field cannot be highlighted.'),
  );
  $highlighting_allowed = islandora_solr_check_highlighting_allowed($solr_field);
  if ($highlighting_allowed == FALSE) {
    $form['options']['snippet']['#default_value'] = 0;
    $form['options']['snippet']['#disabled'] = TRUE;
  }

  if (islandora_solr_is_date_field($solr_field)) {
    $form['options']['date_format'] = array(
      '#type' => 'textfield',
      '#title' => t('Date format'),
      '#default_value' => isset($values['date_format']) ? $values['date_format'] : '',
      '#description' => t('The format of the date, as it will be displayed in the search results. Use <a href="!url" target="_blank">PHP date()</a> formatting. Works best when the date format matches the granularity of the source data. Otherwise it is possible that there will be duplicates displayed.', array('!url' => 'http://php.net/manual/function.date.php')),
    );
  }

  $form['options']['max_length_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Maximum Length'),
    '#description' => t('<strong>Note:</strong> Truncation can lead to unexpected results when used in secondary display profiles such as CSV and RSS.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'truncation_type' => array(
      '#type' => 'radios',
      '#title' => t('Truncation Type'),
      '#options' => array('separate_value_option' => t('Limit length of each separate value'), 'whole_field_option' => t('Limit Length of the whole field')),
      '#default_value' => isset($values['truncation_type']) ? $values['truncation_type'] : 'separate_value_option',
    ),
    'maximum_length' => array(
      '#type' => 'textfield',
      '#title' => t('Maximum Length'),
      '#default_value' => isset($values['maximum_length']) ? $values['maximum_length'] : '0',
      '#element_validate' => array('element_validate_integer'),
      '#description' => t('Maximum field length to render for display. A setting of 0 (default) renders the entire value.<br /> When truncating based on the whole field the max length may be exceeded by the length of ellispse string.'),
    ),
    'add_ellipsis' => array(
      '#type' => 'checkbox',
      '#title' => t('Add Ellipsis'),
      '#description' => t('Add ... to the end of the truncated string.'),
      '#default_value' => isset($values['add_ellipsis']) ? $values['add_ellipsis'] : FALSE,
      '#states' => array(
        'invisible' => array(
          ':input[name="maximum_length"]' => array('value' => '0'),
        ),
      ),
    ),
    'wordsafe' => array(
      '#type' => 'checkbox',
      '#title' => t('Wordsafe'),
      '#description' => t('If selected attempt to truncate on a word boundary. See <a href="!url" target="_blank".>documentation</a> for more information.', array('!url' => 'https://api.drupal.org/api/drupal/includes!unicode.inc/function/truncate_utf8/7')),
      '#default_value' => isset($values['wordsafe']) ? $values['wordsafe'] : FALSE,
      '#states' => array(
        'invisible' => array(
          ':input[name="maximum_length"]' => array('value' => '0'),
        ),
      ),
    ),
    'wordsafe_length' => array(
      '#type' => 'textfield',
      '#title' => t('Minimum Wordsafe Length'),
      '#description' => t('The minimum acceptable length for truncation.'),
      '#states' => array(
        'invisible' => array(
          array(':input[name="maximum_length"]' => array('value' => '0')),
          array(':input[name="wordsafe"]' => array('checked' => FALSE)),
        ),
      ),
      '#default_value' => isset($values['wordsafe_length']) ? $values['wordsafe_length'] : 1,
    ),
  );

  islandora_solr_append_permissions_and_actions($values, $form);

  return $form;
}

/**
 * Utility function to append permissions and actions to the modal.
 *
 * @param array $values
 *   An array of values.
 * @param array $form
 *   An array representing the Drupal form, passed by reference.
 * @param bool $default_value
 *   Whether the default enabled checkbox is to be TRUE or FALSE. *
 */
function islandora_solr_append_permissions_and_actions($values, &$form, $default_value = TRUE) {
  $permissions = isset($values['permissions']) ? $values['permissions'] : array();
  $permissions_disable = _islandora_solr_permissions_disable();
  $permissions_default = _islandora_solr_permissions_default();
  $form['options']['permissions_fieldset'] = islandora_solr_get_admin_permissions_fieldset($permissions, $permissions_default, $permissions_disable, $default_value);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-buttons')),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#weight' => 5,
    '#field' => 'dialog_submit',
    '#field_type' => 'result_fields',
    '#name' => 'result-fields-dialog-submit',
    '#submit' => array('_islandora_solr_update_fields_submit'),
    '#ajax' => array(
      'callback' => '_islandora_solr_admin_settings_dialog',
      'wrapper' => 'islandora-solr-admin-dialog-form',
      'effect' => 'none',
      'event' => 'click',
      'progress' => array('message' => '', 'type' => 'throbber'),
    ),
  );
}

/**
 * Form for sort field settings.
 *
 * @params array $variables
 *   This parameter contains default values to be passed from the main form.
 */
function islandora_solr_admin_settings_search_or_sort_fields($form, &$form_state, $variables) {
  form_load_include($form_state, 'inc', 'islandora_solr', 'includes/admin');

  $form_state['dialog'] = $variables;

  $values = $variables['values'];
  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('scroll')),
    '#id' => 'islandora-solr-admin-dialog-form',
  );
  $form['options']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($values['label']) ? $values['label'] : '',
    '#description' => t('A human-readable name.'),
  );
  islandora_solr_append_permissions_and_actions($values, $form);
  return $form;
}

/**
 * Form for facet field settings.
 *
 * @params array $variables
 *   This parameter contains default values to be passed from the main form.
 */
function islandora_solr_admin_settings_facet_fields($form, &$form_state, $variables) {
  form_load_include($form_state, 'inc', 'islandora_solr', 'includes/admin');

  $form_state['dialog'] = $variables;

  $solr_field = $variables['solr_field'];
  $values = $variables['values'];

  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('scroll')),
    '#id' => 'islandora-solr-admin-dialog-form',
  );
  $form['options']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($values['label']) ? $values['label'] : '',
    '#description' => t('A human-readable name.'),
  );
  $form['options']['sort_by'] = array(
    '#type' => 'radios',
    '#title' => t('Sort by'),
    '#default_value' => isset($values['sort_by']) ? $values['sort_by'] : 'count',
    '#options' => array(
      'count' => t('Count of facet (numerically)'),
      'index' => t('Text labels (alphabetically)'),
    ),
    '#description' => t('Facets can be sorted by text label or the count of the facet. If you sort by text labels AND replace PID with object label your sort order is not guaranteed.'),
  );

  if (islandora_solr_is_date_field($solr_field)) {
    // Add in defaults, to avoid isset() tests.
    $values += array(
      'range_facet_select' => 0,
      'range_facet_variable_gap' => 0,
      'range_facet_start' => 'NOW/YEAR-20YEARS',
      'range_facet_end' => 'NOW',
      'range_facet_gap' => '+1YEAR',
      'date_facet_format' => 'Y',
      'range_facet_slider_enabled' => 0,
      'range_facet_slider_color' => '#edc240',
      'date_filter_datepicker_enabled' => 0,
      'date_filter_datepicker_range' => '-100:+3',
    );
    $form['options']['date_facet_format'] = array(
      '#type' => 'textfield',
      '#title' => t('Date format'),
      '#default_value' => $values['date_facet_format'],
      '#description' => t('The format of the date, as it will be displayed in the facet block. Use <a href="!url">PHP date()</a> formatting. Works best when the date format matches the granularity of the source data. Otherwise it is possible that there will be duplicates displayed.', array('!url' => 'http://php.net/manual/function.date.php')),
    );

    $form['options']['range_facet'] = array(
      '#type' => 'fieldset',
      '#id' => 'range-facet-wrapper',
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
    );

    // @todo Grey out if LUKE says it's not possible to use as a range field?
    //   Add AJAX callback to show more options?
    $form['options']['range_facet']['range_facet_select'] = array(
      '#type' => 'checkbox',
      '#title' => t('Range facet'),
      '#default_value' => $values['range_facet_select'],
      '#description' => t('Toggles whether this facet field should be configured as a Solr range facet.'),
    );

    // @todo Check for non-ajax values.
    $form['options']['range_facet']['wrapper'] = array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          ':input[name="range_facet_select"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['options']['range_facet']['wrapper']['range_facet_variable_gap'] = array(
      '#type' => 'checkbox',
      '#title' => t('Variable range gap'),
      '#return_value' => 1,
      '#default_value' => $values['range_facet_variable_gap'],
      '#description' => t('When checked, the following date range settings will be used by default, but if a date range is filtered down, a new range gap will be calculated and applied. When left unchecked, the following settings provide fixed range gaps.'),
    );
    $form['options']['range_facet']['wrapper']['range_facet_start'] = array(
      '#type' => 'textfield',
      '#title' => t('Start'),
      '#default_value' => $values['range_facet_start'],
      '#description' => t('The lower bound of the first date range for all date faceting on this field. This should be a single date expression which may use the <a href="!url">DateMathParser</a> syntax.', array('!url' => 'http://lucene.apache.org/solr/api/org/apache/solr/util/DateMathParser.html')),
    );
    $form['options']['range_facet']['wrapper']['range_facet_end'] = array(
      '#type' => 'textfield',
      '#title' => t('End'),
      '#default_value' => $values['range_facet_end'],
      '#description' => t('The minimum upper bound of the last date range for all Date Faceting on this field. This should be a single date expression which may use the <a href="!url">DateMathParser</a> syntax.', array('!url' => 'http://lucene.apache.org/solr/api/org/apache/solr/util/DateMathParser.html')),
    );
    $form['options']['range_facet']['wrapper']['range_facet_gap'] = array(
      '#type' => 'textfield',
      '#title' => t('Gap'),
      '#default_value' => $values['range_facet_gap'],
      '#description' => t('The size of each date range, expressed as an interval to be added to the lower bound using the <a href="!url">DateMathParser</a> syntax.', array('!url' => 'http://lucene.apache.org/solr/api/org/apache/solr/util/DateMathParser.html')),
    );
    // Range slider.
    $form['options']['range_facet']['wrapper']['range_slider'] = array(
      '#type' => 'fieldset',
      '#title' => t('Range slider'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['options']['range_facet']['wrapper']['range_slider']['range_facet_slider_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable range slider'),
      '#return_value' => 1,
      '#default_value' => $values['range_facet_slider_enabled'],
      '#description' => t('When checked, the normal range facet will be replaced by a range slider widget.'),
    );
    $form['options']['range_facet']['wrapper']['range_slider']['range_facet_slider_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Slider color'),
      '#description' => t('The range slider\'s color, formatted as a hex value. Defaults to <span style="color: #edc240">#edc240</span>'),
      '#default_value' => $values['range_facet_slider_color'],
      '#states' => array(
        'visible' => array(
          ':input[name="range_facet_slider_enabled"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Date filter.
    $form['options']['range_facet']['wrapper']['date_filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date range filter'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['options']['range_facet']['wrapper']['date_filter']['date_filter_datepicker_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable date range filter'),
      '#return_value' => 1,
      '#default_value' => $values['date_filter_datepicker_enabled'],
      '#description' => t('When checked, a date range filter will become available underneath the date range facet. The date range filter includes <em>from date</em> and a <em>to date</em> text fields. It also comes with a calendar popup widget.'),
    );
    $form['options']['range_facet']['wrapper']['date_filter']['date_filter_datepicker_range'] = array(
      '#type' => 'textfield',
      '#title' => t('Years back and forward'),
      '#default_value' => $values['date_filter_datepicker_range'],
      '#size' => 10,
      '#maxsize' => 10,
      '#description' => t('The range of years displayed in the year drop-down menu. These are either relative to today\'s year ("-nn:+nn"), to the currently selected year ("c-nn:c+nn"), an absolute ("nnnn:nnnn"), or combinations of these formats ("nnnn:-nn"). For more info, check the jQuery UI <a href="!url" target="_blank">datepicker documentation</a>.', array('!url' => 'http://api.jqueryui.com/datepicker/#option-yearRange')),
      '#states' => array(
        'visible' => array(
          ':input[name="date_filter_datepicker_enabled"]' => array('checked' => TRUE),
        ),
      ),
    );
  }
  elseif (islandora_solr_is_boolean_field($solr_field)) {
    // Defaults.
    $values += array(
      'boolean_facet_true_replacement' => '',
      'boolean_facet_false_replacement' => '',
    );
    $form['options']['boolean_facet'] = array(
      '#type' => 'fieldset',
      '#title' => t('Boolean Facet Options'),
      '#collapsible' => FALSE,
      'blurb' => array(
        '#markup' => t('Enter replacement values for the labels of TRUE and FALSE results for boolean facets. Leaving a field empty will cause that result to show up as "true" or "false", respectively, which may be less than clear to the end user.'),
      ),
      'boolean_facet_true_replacement' => array(
        '#type' => 'textfield',
        '#title' => t('Replacement for TRUE values'),
        '#default_value' => $values['boolean_facet_true_replacement'],
      ),
      'boolean_facet_false_replacement' => array(
        '#type' => 'textfield',
        '#title' => t('Replacement for FALSE values'),
        '#default_value' => $values['boolean_facet_false_replacement'],
      ),
    );
  }

  // Permissions.
  islandora_solr_append_permissions_and_actions($values, $form);

  $form['options']['pid_object_label'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace PID with Object Label'),
    '#return_value' => 1,
    '#default_value' => (isset($values['pid_object_label']) ? $values['pid_object_label'] : 0),
    '#description' => t("Replace a PID (islandora:foo) or a URI (info:fedora/islandora:foo) with that object's label. Will only work with non-tokenized Solr fields (full literal strings)."),
  );

  return $form;
}

/**
 * Returns an array of role IDs to disable checkboxes.
 */
function _islandora_solr_permissions_disable() {
  $user_roles = array_keys(user_roles());
  $permissions_solr = array_keys(user_roles(FALSE, 'search islandora solr'));
  $permissions_disable = array_diff($user_roles, $permissions_solr);
  // If authenticated users have permission exclude all authenticated users
  // fields from the disable list.
  if (in_array('2', $permissions_solr)) {
    foreach ($permissions_disable as $key => $rid) {
      if ($rid != '1') {
        unset($permissions_disable[$key]);
      }
    }
  }
  return $permissions_disable;
}

/**
 * Returns an array of role id's to set default values for checkboxes.
 */
function _islandora_solr_permissions_default() {
  $user_roles = array_keys(user_roles());
  $permissions_solr = array_keys(user_roles(FALSE, 'search islandora solr'));
  $permissions_default = array_intersect($user_roles, $permissions_solr);
  // If authenticated users have permission include all authenticated users
  // fields to the default list.
  if (in_array('2', $permissions_solr)) {
    foreach ($user_roles as $rid) {
      if ($rid != '1' && !in_array($rid, $permissions_default)) {
        $permissions_default[] = $rid;
      }
    }
  }
  return $permissions_default;
}

/**
 * Get the Solr config from the given installation.
 *
 * Seem to get the Solr config a couple places... We cache statically, just
 * because.
 *
 * @param string $solr_url
 *   The Solr URL from which to try to grab the solrconfig.xml.
 *
 * @return SimpleXMLElement
 *   A SimpleXMLElement representing the contents of the solrconfig.xml, or
 *   NULL if it could not be retrieved.
 */
function islandora_solr_get_solrconfig_xml($solr_url) {
  $xml = &drupal_static(__FUNCTION__, array());

  if (!array_key_exists($solr_url, $xml) || $xml[$solr_url] === NULL) {
    $url = $solr_url . '/admin/file/?file=solrconfig.xml';

    // XXX: The Solr URL should be made to contain the URI scheme in the actual
    // configuration.
    if (strpos($solr_url, 'http') !== 0) {
      $url = "http://$url";
    }

    $req = drupal_http_request($url);
    if ($req->code / 100 === 2) {
      $xml[$solr_url] = simplexml_load_string($req->data);
      islandora_solr_resolve_xincludes($xml[$solr_url], (strpos($solr_url, 'http') === 0 ? '' : 'http://') . $solr_url);
    }
    else {
      $xml[$solr_url] = NULL;
    }
  }

  return $xml[$solr_url];
}

/**
 * Resolve XIncludes which include files local to the Solr config directory.
 *
 * Relies upon some internal libxml behaviour... When one imports a
 * SimpleXMLElement into DOM and acts upon it, the original SimpleXMLElement is
 * also changed.
 *
 * @param SimpleXMLElement $doc_element
 *   An SimpleXMLElement under which to resolve XInclude statements. NOTE: This
 *   object will be modified in place.
 * @param string $solr_url
 *   A string representing the path to Solr.
 */
function islandora_solr_resolve_xincludes(SimpleXMLElement $doc_element, $solr_url) {
  $dom_element = dom_import_simplexml($doc_element);
  $doc = $dom_element->ownerDocument;
  $xpath = new DOMXPath($doc);
  $xpath->registerNamespace('xi', 'http://www.w3.org/2001/XInclude');
  $xincludes = $xpath->query('//xi:*[@href and not(contains(@href, "/"))]');
  foreach ($xincludes as $element) {
    $element->setAttribute('href', "$solr_url/admin/file?file=" . $element->getAttribute('href'));
  }

  // XXX: Suppress errors, as trying to access a file which doesn't exist
  // will throw an HTTP 400 instead of a 404... Solr is silly.
  @$doc->xinclude();
}

/**
 * @} End of "defgroup dialog-forms".
 */
