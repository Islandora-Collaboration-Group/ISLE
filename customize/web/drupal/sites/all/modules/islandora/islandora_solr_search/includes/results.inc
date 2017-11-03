<?php

/**
 * @file
 * Contains methods to create rendered Solr displays from raw Solr results.
 * Depends on Apache_Solr_Php client.
 */

/**
 * Islandora Solr Results
 */
class IslandoraSolrResults {

  public $facetFieldArray = array();
  public $searchFieldArray = array();
  public $resultFieldArray = array();
  public $allSubsArray = array();
  public $islandoraSolrQueryProcessor;
  public $rangeFacets = array();
  public $dateFormatFacets = array();

  /**
   * Constructor.
   */
  public function __construct() {
    $this->prepFieldSubstitutions();
    $this->rangeFacets = islandora_solr_get_range_facets();
    $this->dateFormatFacets = islandora_solr_get_date_format_facets();
  }

  /**
   * Output the main body of the search results.
   *
   * @see islandora_solr()
   *
   * @param IslandoraSolrQueryProcessor $islandora_solr_query
   *   The IslandoraSolrQueryProcessor object which includes the current query
   *   settings and the raw Solr results.
   *
   * @return string
   *   Returns themed Solr results page, including wrapper and rendered search
   *   results.
   */
  public function displayResults($islandora_solr_query) {
    $this->islandoraSolrQueryProcessor = $islandora_solr_query;

    // Set variables to collect returned data.
    $results = NULL;
    $secondary_profiles = NULL;
    $elements = array();

    // Set breadcrumbs.
    $this->setBreadcrumbs($islandora_solr_query);

    // Raw solr results.
    $islandora_solr_result = $this->islandoraSolrQueryProcessor->islandoraSolrResult;

    // Solr results count.
    // Total Solr results.
    $elements['solr_total'] = (int) $islandora_solr_result['response']['numFound'];

    // Solr start.
    // To display: $islandora_solr_query->solrStart + ($total > 0 ? 1 : 0).
    $elements['solr_start'] = $islandora_solr_query->solrStart;

    // Solr results end.
    $end = min(($islandora_solr_query->solrLimit + $elements['solr_start']), $elements['solr_total']);
    $elements['solr_end'] = $end;

    // Pager.
    islandora_solr_pager_init($elements['solr_total'], $islandora_solr_query->solrLimit);
    $elements['solr_pager'] = theme('pager', array(
      'tags' => NULL,
      'element' => 0,
      'parameters' => NULL,
      'quantity' => 5,
    ));

    // Debug (will be removed).
    $elements['solr_debug'] = '';
    if (variable_get('islandora_solr_debug_mode', 0)) {
      $elements['solr_debug'] = $this->printDebugOutput($islandora_solr_result);
    }

    // Rendered secondary display profiles.
    $secondary_profiles = $this->addSecondaries($islandora_solr_query);

    // Rendered results.
    $results = $this->printResults($islandora_solr_result);

    return theme('islandora_solr_wrapper', array(
      'results' => $results,
      'secondary_profiles' => $secondary_profiles,
      'elements' => $elements,
    ));
  }

  /**
   * Renders the secondary display profile list.
   *
   * @param IslandoraSolrQueryProcessor $islandora_solr_query
   *   The IslandoraSolrQueryProcessor object which includes the current query
   *   settings and the raw Solr results.
   *
   * @return string
   *   Themed list of secondary displays
   *
   * @see IslandoraSolrResults::displayResults()
   */
  public function addSecondaries($islandora_solr_query) {
    $query_list = array();
    // Get secondary display profiles.
    $secondary_display_profiles = module_invoke_all('islandora_solr_secondary_display');

    // $_GET['q'] didn't seem to work here.
    $path = current_path();

    // Parameters set in URL.
    $params = $islandora_solr_query->internalSolrParams;

    // Get list of secondary displays.
    $secondary_array = variable_get('islandora_solr_secondary_display', array());
    foreach ($secondary_array as $name => $status) {
      if ($status === $name) {
        // Generate URL.
        $query_secondary = array_merge($params, array('solr_profile' => $name));

        // Set attributes variable for remove link.
        $attr = array();
        $attr['title'] = $secondary_display_profiles[$name]['description'];
        $attr['rel'] = 'nofollow';
        $attr['href'] = url($path, array('query' => $query_secondary));
        $logo = $secondary_display_profiles[$name]['logo'];

        // XXX: We are not using l() because of active classes:
        // @see http://drupal.org/node/41595
        // Create link.
        $query_list[] = '<a' . drupal_attributes($attr) . '>' . $logo . '</a>';
      }
    }

    return theme('item_list', array(
      'items' => $query_list,
      'title' => NULL,
      'type' => 'ul',
      'attributes' => array('id' => 'secondary-display-profiles'),
    ));
  }

  /**
   * Renders the primary or secondary display profile.
   *
   * Renders the primary or secondary display profile based on the raw Solr
   * results. This is the method most Islandora Solr display plugins would
   * override.
   *
   * @param array $solr_results
   *   The raw Solr results from
   *   IslandoraSolrQueryProcessor::$islandoraSolrResult.
   *
   * @return string
   *   Rendered Solr results
   *
   * @see IslandoraSolrResults::displayResults()
   */
  public function printResults($solr_results) {
    $solr_results = islandora_solr_prepare_solr_results($solr_results);
    $object_results = $solr_results['response']['objects'];
    $object_results = islandora_solr_prepare_solr_doc($object_results);

    $elements = array();
    $elements['solr_total'] = $solr_results['response']['numFound'];
    $elements['solr_start'] = $solr_results['response']['start'];

    // Return themed search results.
    return theme('islandora_solr', array('results' => $object_results, 'elements' => $elements));
  }


  /**
   * Displays elements of the current solr query.
   *
   * Displays current query and current filters. Includes a link to exclude the
   * query/filter.
   *
   * @param IslandoraSolrQueryProcessor $islandora_solr_query
   *   The IslandoraSolrQueryProcessor object which includes the current query
   *   settings and the raw Solr results.
   *
   * @return string
   *   Rendered lists of the currently active query and/or filters.
   */
  public function currentQuery($islandora_solr_query) {
    module_load_include('inc', 'islandora', 'includes/utilities');

    $output = '';
    $path = current_path();
    $format = variable_get('islandora_solr_facet_date_format', 'Y');

    // Get user provided filter parameters.
    $fq = isset($islandora_solr_query->internalSolrParams['f']) ? $islandora_solr_query->internalSolrParams['f'] : array();
    // Parameters set in URL.
    $params = $islandora_solr_query->internalSolrParams;
    // Get query values.
    if (!in_array($islandora_solr_query->solrQuery, $islandora_solr_query->differentKindsOfNothing)) {
      // Get query value.
      $query_value = stripslashes($islandora_solr_query->solrQuery);

      $query_list = array();
      if (variable_get('islandora_solr_human_friendly_query_block', TRUE)) {
        foreach ($this->searchFieldArray as $search_field => $search_field_label) {
          $query_value = str_replace($search_field . ':(', $search_field_label . ':(', $query_value);
        }
      }

      // Remove link keeps all parameters (query gets removed instead).
      $query_minus = $params;

      // Remove query from path.
      $path_minus = implode('/', explode('/', $path, -1));

      // Set attributes variable for remove link.
      $attributes = array(
        'minus' => array(
          'attr' => array(),
          'path' => $path_minus,
          'query' => $query_minus,
        ),
      );
      $attr_minus =& $attributes['minus']['attr'];
      $attr_minus['title'] = t('Remove') . ' ' . $query_value;
      $attr_minus['class'] = array('remove-query');
      $attr_minus['rel'] = 'nofollow';
      $attr_minus['href'] = url($path_minus, array('query' => $query_minus));

      $hooks = islandora_build_hook_list(ISLANDORA_SOLR_FACET_BUCKET_CLASSES_HOOK_BASE);
      drupal_alter($hooks, $attributes, $islandora_solr_query);

      // XXX: We are not using l() because of active classes:
      // @see http://drupal.org/node/41595
      // Create link.
      $query_list[] = '<a' . drupal_attributes($attributes['minus']['attr']) . '>(-)</a> ' . check_plain($query_value);

      // Add wrap and list.
      $output .= '<div class="islandora-solr-query-wrap">';
      $output .= theme('item_list', array(
        'items' => $query_list,
        'title' => t('Query'),
        'type' => 'ul',
        'attributes' => array('class' => 'islandora-solr-query-list query-list'),
      ));
      $output .= '</div>';

    }

    // Get filter values.
    if (!empty($fq)) {
      // Set list variables.
      $filter_list = array();
      foreach ($fq as $key => $filter) {
        // Check for exclude filter.
        if ($filter[0] == '-') {
          // Not equal sign.
          $symbol = '&ne;';
        }
        else {
          $symbol = '=';
        }
        $filter_string = $this->formatFilter($filter, $islandora_solr_query);
        // Pull out filter (for exclude link).
        $query_minus = array();
        $f_x['f'] = array_diff($params['f'], array($filter));
        $query_minus = array_merge($params, $f_x);
        // @todo Find a cleaner way to do this.
        // Resetting the filter keys' order.
        if ($query_minus['f']) {
          $query_minus['f'] = array_merge(array(), $query_minus['f']);
        }
        // Remove 'f' if empty.
        if (empty($query_minus['f'])) {
          unset($query_minus['f']);
        }
        // Set attributes variable for remove link.
        $attributes = array(
          'minus' => array(
            'attr' => array(),
            'path' => $path,
            'query' => $query_minus,
          ),
        );
        $attr_minus =& $attributes['minus']['attr'];
        $attr_minus['title'] = t('Remove') . ' ' . $filter;
        $attr_minus['class'] = array('remove-filter');
        $attr_minus['rel'] = 'nofollow';
        $attr_minus['href'] = url($path, array('query' => $query_minus));

        $hooks = islandora_build_hook_list(ISLANDORA_SOLR_FACET_BUCKET_CLASSES_HOOK_BASE);
        drupal_alter($hooks, $attributes, $islandora_solr_query);

        // XXX: We are not using l() because of active classes:
        // @see http://drupal.org/node/41595
        // Create link.
        $filter_list[] = '<a' . drupal_attributes($attributes['minus']['attr']) . '>(-)</a> ' . $symbol . ' ' . check_plain($filter_string);

      }

      // Return filter list.
      $output .= '<div class="islandora-solr-filter-wrap">';
      $output .= theme('item_list', array(
        'items' => $filter_list,
        'title' => t("Enabled Filters"),
        'type' => 'ul',
        'attributes' => array('class' => 'islandora-solr-filter-list filter-list'),
      ));
      $output .= '</div>';
    }
    return $output;
  }

  /**
   * Sets the Drupal breadcrumbs.
   *
   * @param IslandoraSolrQueryProcessor $islandora_solr_query
   *   The IslandoraSolrQueryProcessor object, which includes the current query
   *   settings and the raw Solr results.
   */
  public function setBreadcrumbs($islandora_solr_query) {
    $breadcrumb = $this->getBreadcrumbs($islandora_solr_query);
    drupal_set_breadcrumb($breadcrumb);
  }

  /**
   * Gets the Drupal breadcrumbs.
   *
   * Gets the Drupal breadcrumbs based on the current query and filters.
   * Provides links to exclude the query or filters.
   *
   * @param IslandoraSolrQueryProcessor $islandora_solr_query
   *   The IslandoraSolrQueryProcessor object which includes the current query
   *   settings and the raw Solr results.
   *
   * @return array
   *   An array of breadcrumbs.
   */
  public function getBreadcrumbs($islandora_solr_query) {
    // $_GET['q'] didn't seem to work here.
    $path = current_path();
    // Get date format.
    $format = variable_get('islandora_solr_facet_date_format', 'Y');

    $breadcrumb = array();
    // Get user provided filter parameters.
    if (isset($islandora_solr_query->internalSolrParams['f'])) {
      $fq = $islandora_solr_query->internalSolrParams['f'];
    }
    else {
      $fq = array();
    }

    // Parameters set in URL.
    $params = $islandora_solr_query->internalSolrParams;
    // Set filter key if there are no filters included.
    if (empty($params['f'])) {
      $params['f'] = array();
    }

    // Loop to create filter breadcrumbs if available.
    if (!empty($fq)) {
      $f['f'] = array();
      foreach ($fq as $key => $filter) {
        // Check for exclude filter.
        $exclude = FALSE;
        if ($filter[0] == '-') {
          $exclude = TRUE;
        }
        $filter_string = $this->formatFilter($filter, $islandora_solr_query);
        // Increment filter array with current filter (for breadcrumb link).
        $query = array();
        $query_diff = $params;
        if (isset($query_diff['f'])) {
          unset($query_diff['f']);
        }
        $f = array_merge_recursive($f, array('f' => array($filter)));
        $query = array_merge($query_diff, $f);

        // Pull out filter (for x link).
        $query_x = array();
        $f_x['f'] = array_diff($params['f'], array($filter));
        $query_x = array_merge($params, $f_x);
        // @todo Find a cleaner way to do this.
        // Resetting the filter key's order.
        if ($query_x['f']) {
          $query_x['f'] = array_merge(array(), $query_x['f']);
        }
        // Remove 'f' if empty.
        if (empty($query_x['f'])) {
          unset($query_x['f']);
        }

        // Set attributes variable.
        $attr = array();
        $attr['title'] = $filter;
        $attr['rel'] = 'nofollow';
        if ($exclude) {
          $attr['class'] = 'strikethrough';
        }
        $attr['href'] = url($path, array('query' => $query));

        // Set attributes variable for remove link.
        $attr_x = array();
        $attr_x['title'] = t('Remove') . ' ' . $filter;
        $attr_x['rel'] = 'nofollow';
        $attr_x['href'] = url($path, array('query' => $query_x));

        // XXX: We are not using l() because of active classes:
        // @see http://drupal.org/node/41595
        // Create link.
        $breadcrumb[] = '<a' . drupal_attributes($attr) . '>' . check_plain($filter_string) . '</a>'
              . '<span class="islandora-solr-breadcrumb-super"> <a' . drupal_attributes($attr_x) . '>(' . t('x') . ')</a></span>';

      }
      // At this point reverse the breadcrumbs array (only contains filters).
      $breadcrumb = array_reverse($breadcrumb);
    }

    // Create query breadcrumb.
    if (!in_array($islandora_solr_query->solrQuery, $islandora_solr_query->differentKindsOfNothing)) {
      // Get query value.
      $query_value = $islandora_solr_query->solrQuery;
      // Remove all filters for this breadcrumb.
      $query = $params;
      if (isset($query['f'])) {
        unset($query['f']);
      }
      // Remove button keeps all parameters (query gets removed instead).
      $query_x = array();
      $query_x = $params;
      if (empty($params['f'])) {
        unset($query_x['f']);
      }
      // Remove query from path.
      $path_x = implode('/', explode('/', $path, -1)) . '/ ';

      // Set attributes variable.
      $attr = array();
      $attr['title'] = $query_value;
      $attr['rel'] = 'nofollow';
      $attr['href'] = url($path, array('query' => $query));

      // Set attributes variable for remove link.
      $attr_x = array();
      $attr_x['title'] = t('Remove') . ' ' . $query_value;
      $attr_x['rel'] = 'nofollow';
      $attr_x['href'] = url($path_x, array('query' => $query_x));

      // Remove solr fields from breadcrumb value.
      $query_explode = preg_split(ISLANDORA_SOLR_QUERY_SPLIT_REGEX, $query_value);
      $query_implode = array();
      foreach ($query_explode as $value) {
        // Check for first colon to split the string.
        if (strpos($value, ':') != FALSE) {
          // Split the filter into field and value.
          $value_split = preg_split(ISLANDORA_SOLR_QUERY_FIELD_VALUE_SPLIT_REGEX, $value, 2);
          // Trim whitespace.
          $value_split[1] = trim($value_split[1]);
          // Trim brackets.
          $value = str_replace(array('(', ')'), '', $value_split[1]);
        }
        // No colon is found.
        else {
          $value = trim($value);
          // Strip brackets.
          $value = str_replace(array('(', ')'), '', $value);
        }
        $query_implode[] = $value;
      }
      $query_value = implode(" ", $query_implode);

      // XXX: We are not using l() because of active classes:
      // @see http://drupal.org/node/41595
      // Create link.
      $breadcrumb[] = '<a' . drupal_attributes($attr) . '>' . stripslashes(check_plain($query_value)) . '</a>'
            . '<span class="islandora-solr-breadcrumb-super"> <a' . drupal_attributes($attr_x) . '>(' . t('x') . ')</a></span>';
    }

    $breadcrumb[] = l(t('Home'), '<front>', array('attributes' => array('title' => t('Home'))));
    if (!empty($breadcrumb)) {
      $breadcrumb = array_reverse($breadcrumb);
    }
    $context = 'solr';
    drupal_alter('islandora_breadcrumbs', $breadcrumb, $context);
    return $breadcrumb;
  }

  /**
   * Formats the passed in filter into a human readable form.
   *
   * @param string $filter
   *   The passed in filter.
   * @param object $islandora_solr_query
   *   The current Solr Query
   *
   * @return string
   *   The formatted filter string for breadcrumbs and active query.
   */
  public function formatFilter($filter, $islandora_solr_query) {
    // @todo See how this interacts with multiple date filters.
    // Check if there are operators in the filter.
    $fq_split = preg_split('/ (OR|AND) /', $filter);
    if (count($fq_split) > 1) {
      $operator_split = preg_split(ISLANDORA_SOLR_QUERY_SPLIT_REGEX, $filter);
      $operator_split = array_diff($operator_split, $fq_split);
      $out_array = array();
      foreach ($fq_split as $fil) {
        $fil_split = preg_split(ISLANDORA_SOLR_QUERY_FIELD_VALUE_SPLIT_REGEX, $fil, 2);
        $out_str = str_replace(array('"', 'info:fedora/'), '', $fil_split[1]);
        $out_array[] = $out_str;
      }
      $filter_string = '';
      foreach ($out_array as $out) {
        $filter_string .= $out;
        if (count($operator_split)) {
          $filter_string .= ' ' . array_shift($operator_split) . ' ';
        }
      }
      $filter_string = trim($filter_string);
    }
    else {
      // Split the filter into field and value.
      $filter_split = preg_split(ISLANDORA_SOLR_QUERY_FIELD_VALUE_SPLIT_REGEX, $filter, 2);
      // Trim brackets.
      $filter_split[1] = trim($filter_split[1], "\"");
      $solr_field = ltrim($filter_split[0], '-');
      // If value is date.
      if (isset($islandora_solr_query->solrParams['facet.date']) && in_array($solr_field, $islandora_solr_query->solrParams['facet.date'])) {
        // Check date format setting.
        foreach ($this->rangeFacets as $value) {
          if ($value['solr_field'] == $solr_field && isset($value['solr_field_settings']['date_facet_format']) && !empty($value['solr_field_settings']['date_facet_format'])) {
            $format = $value['solr_field_settings']['date_facet_format'];
          }
        }
        // Split range filter string to return formatted date values.
        $filter_str = $filter_split[1];
        $filter_str = trim($filter_str, '[');
        $filter_str = trim($filter_str, ']');
        $filter_array = explode(' TO ', $filter_str);
        $filter_split[1] = format_date(strtotime(trim($filter_array[0])) + (60 * 60 * 24), 'custom', $format) . ' - ' . format_date(strtotime(trim($filter_array[1])) + (60 * 60 * 24), 'custom', $format);
      }
      elseif (isset($this->dateFormatFacets[$solr_field])) {
        $format = $this->dateFormatFacets[$solr_field]['solr_field_settings']['date_facet_format'];
        $filter_split[1] = format_date(strtotime(stripslashes($filter_split[1])), 'custom', $format);
      }
      $filter_string = $filter_split[1];
    }
    return stripslashes($filter_string);
  }

  /**
   * Displays facets based on a query response.
   *
   * Includes links to include or exclude a facet field in a search.
   *
   * @param IslandoraSolrQueryProcessor $islandora_solr_query
   *   The IslandoraSolrQueryProcessor object which includes the current query
   *   settings and the raw Solr results.
   *
   * @return string
   *   Rendered lists of facets including links to include or exclude a facet
   *   field.
   *
   * @see islandora_solr_islandora_solr_query_blocks()
   * @see islandora_solr_block_view()
   */
  public function displayFacets($islandora_solr_query) {
    IslandoraSolrFacets::init($islandora_solr_query);
    $output = '';
    $facet_order = $this->facetFieldArray;
    foreach ($facet_order as $facet_key => $facet_label) {
      $facet_obj = new IslandoraSolrFacets($facet_key);
      $output .= $facet_obj->getFacet();
    }

    // As we add additional facets, we're repeatedly URL-encoding old facet
    // strings. when we double-encode quotation marks they're incomprehensible
    // to Solr.
    $output = str_replace('%2B', '%252B', $output);
    return $output;
  }

  /**
   * Create a fieldset for debugging purposes.
   *
   * Creates a fieldset containing raw Solr results of the current page for
   * debugging purposes.
   *
   * @see IslandoraSolrResults::displayResults()
   *
   * @param array $islandora_solr_results
   *   The processed Solr results from
   *   IslandoraSolrQueryProcessor::islandoraSolrResult
   *
   * @return string
   *   Rendered fieldset containing raw Solr results data.
   */
  public function printDebugOutput($islandora_solr_results) {
    // Debug dump.
    $results = "<pre>Results: " . print_r($islandora_solr_results, TRUE) . "</pre>";
    $fieldset = array(
      '#title' => t("Islandora Processed Solr Results"),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#value' => $results,
      '#children' => '',
    );

    return theme('fieldset', array('element' => $fieldset));
  }

  /**
   * Reads configuration values and prepares field => label mappings.
   *
   * Reads configuration values and preps a number of key => value arrays for
   * output substitution. Replaces solr field labels with human readable labels
   * as set in the admin form.
   */
  public function prepFieldSubstitutions() {

    $this->facetFieldArray = islandora_solr_get_fields('facet_fields');

    $this->searchFieldArray = islandora_solr_get_fields('search_fields');

    $this->resultFieldArray = islandora_solr_get_fields('result_fields');

    $this->allSubsArray = array_merge($this->facetFieldArray, $this->searchFieldArray, $this->resultFieldArray);
  }
}

/**
 * Islandora Solr Facets
 */
class IslandoraSolrFacets {
  public static $islandoraSolrQuery;

  // XXX: Need to fix the property/member variable names...  Could have an
  // effect on other code, though, due to public visibility.
  // @codingStandardsIgnoreStart
  // Normal facet results.
  public static $facet_fields;
  // Date range facet results (Solr 1.4).
  public static $facet_dates;
  // Date or integer range facet results (Solr 3.1).
  public static $facet_ranges;

  public static $facet_fields_settings;
  public static $facet_fields_settings_simple;
  // Rename?
  public static $range_facets;
  public static $minimum_count = 1;
  public static $soft_limit;
  public static $exclude_range_values;
  public static $needed_solr_call;
  public static $range_slider_key = 0;
  public static $date_filter_key = 0;

  public $facet_field;
  public $settings;
  public $facet_type;
  public $results;
  public $title = NULL;
  public $content = NULL;


  // @codingStandardsIgnoreEnd

  /**
   * Constructor method.
   *
   * Stores the facet field name, settings and title in a parameter.
   *
   * @param string $facet_field
   *   The name of the solr field to build a facet for.
   */
  public function __construct($facet_field) {
    $this->facet_field = $facet_field;
    $this->settings = self::$facet_fields_settings[$facet_field];
    $this->title = self::$facet_fields_settings_simple[$facet_field];
  }

  /**
   * Static init.
   *
   * Populates static variables with Solr query results and user settings.
   *
   * @param object $islandora_solr_query
   *   Object containing the Solr query and results.
   */
  public static function init($islandora_solr_query) {
    self::$islandoraSolrQuery = $islandora_solr_query;
    self::$facet_fields = isset($islandora_solr_query->islandoraSolrResult) ? $islandora_solr_query->islandoraSolrResult['facet_counts']['facet_fields'] : array();
    self::$facet_dates = isset($islandora_solr_query->islandoraSolrResult) ? $islandora_solr_query->islandoraSolrResult['facet_counts']['facet_dates'] : array();
    // Not in place yet.
    // XXX: isset() checking, as older Solrs (before 3.1) won't return a value.
    self::$facet_ranges = isset($islandora_solr_query->islandoraSolrResult['facet_counts']['facet_ranges']) ?
      $islandora_solr_query->islandoraSolrResult['facet_counts']['facet_ranges'] :
      array();

    // Filtered, not simplified and fields as keys.
    self::$facet_fields_settings = islandora_solr_get_fields('facet_fields', TRUE, FALSE, TRUE);
    self::$facet_fields_settings_simple = _islandora_solr_simplify_fields(self::$facet_fields_settings);
    self::$range_facets = islandora_solr_get_range_facets();
    self::$soft_limit = variable_get('islandora_solr_facet_soft_limit', '10');
    self::$exclude_range_values = array(
      'gap',
      'start',
      'end',
      'other',
      'hardend',
      'include',
    );

    // Calculate variable date gap.
    // @XXX move elsewhere?
    self::variableDateGap();
  }

  /**
   * Prepare and render facet.
   *
   * Method called after a facet object is created. This will prepare the
   * results based on the type and user settings for this facet. It also does a
   * call to render the prepared data. This method also returns the rendered
   * endresult.
   *
   * @return string
   *   Returns the title and rendered facet.
   */
  public function getFacet() {
    $this->findFacetType();
    $this->getFacetResults();
    if (empty($this->results)) {
      return;
    }
    $this->processFacets();
    if (empty($this->content)) {
      return;
    }
    $elements = array(
      'title' => $this->title,
      'content' => $this->content,
      'pid' => $this->facet_field,
    );
    return theme('islandora_solr_facet_wrapper', $elements);
  }

  /**
   * Check facet type.
   *
   * Depending on facet type and Solr version, Solr will return different types
   * of facets in different arrays in the results object. Itereate where the
   * current facet object has returned.
   */
  public function findFacetType() {
    $facet_field = $this->facet_field;
    if (array_key_exists($facet_field, self::$facet_fields)) {
      $this->facet_type = 'facet_fields';
    }
    if (array_key_exists($facet_field, self::$facet_dates)) {
      $this->facet_type = 'facet_dates';
    }
    if (array_key_exists($facet_field, self::$facet_ranges)) {
      $this->facet_type = 'facet_ranges';
    }
  }

  /**
   * Gets facet results.
   *
   * Finds and stores facet results from the Solr query into a property.
   */
  public function getFacetResults() {
    $facet_field = $this->facet_field;
    $facet_type = $this->facet_type;
    if ($facet_type == 'facet_fields') {
      $this->results = self::$facet_fields[$facet_field];
    }
    if ($facet_type == 'facet_dates') {
      $this->results = self::$facet_dates[$facet_field];
    }
    if ($facet_type == 'facet_ranges') {
      $this->results = self::$facet_ranges[$facet_field];
    }
  }

  /**
   * Process facets.
   *
   * Calls a process method based on the facet type. This method exists so this
   * process of preparing and rendering arrays can be started from one simple
   * method call.
   */
  public function processFacets() {
    $facet_type = $this->facet_type;
    if ($facet_type == 'facet_fields') {
      $this->processFacetFields();
    }
    if ($facet_type == 'facet_dates') {
      $this->processFacetDates();
    }
    if ($facet_type == 'facet_ranges') {
      $this->processFacetRanges();
    }
  }

  /**
   * Process facet fields.
   *
   * Calls prepare method to prepare the array to be rendered. Then calls
   * the method to render the prepared results.
   */
  public function processFacetFields() {
    $prepared_facet_fields = $this->prepareFacetFields();
    $this->renderText($prepared_facet_fields);
  }

  /**
   * Process facet dates.
   *
   * Depending on the way the facets need to be rendered as set in the user
   * configuration, this method calls methods to prepare the results to be
   * rendered. Then calls the method to render the prepared results. Currently
   * there are 3 ways facet dates can be rendered: normal text, slider and/or
   * datepicker.
   *
   * @todo Maybe there should be some better pluggable way to prepare and
   *   render facets so that contrib modules can create their own widgets.
   */
  public function processFacetDates() {
    // @todo Make this check better. Could be pluggable in the future.
    if (isset($this->settings['solr_field_settings']['range_facet_slider_enabled']) &&
        $this->settings['solr_field_settings']['range_facet_slider_enabled'] == 1) {
      // Prepare and render facet dates as slider.
      $facet_dates_as_slider = $this->prepareFacetDatesSlider();
      if (!empty($facet_dates_as_slider)) {
        $this->renderSlider($facet_dates_as_slider);
      }
    }
    else {
      // Prepare and render facet dates as text.
      $facet_dates_as_text = $this->prepareFacetDates();
      $this->renderText($facet_dates_as_text);
    }
    // Date filter.
    if (isset($this->settings['solr_field_settings']['date_filter_datepicker_enabled']) &&
        $this->settings['solr_field_settings']['date_filter_datepicker_enabled'] == 1) {
      // Prepare and render facet dates as slider.
      $facet_dates_datefilter = $this->prepareFacetDatesFilter();
      if (!empty($facet_dates_datefilter)) {
        $this->renderFacetDatesFilter($facet_dates_datefilter);
      }
    }
  }

  /**
   * Process facet ranges.
   *
   * Not in place yet.
   */
  public function processFacetRanges() {

  }

  /**
   * Get date format.
   *
   * Finds the date formatting settings from user configuration.
   */
  public function getDateFormat() {
    if (isset($this->settings['solr_field_settings']['date_facet_format']) &&
        !empty($this->settings['solr_field_settings']['date_facet_format'])) {
      return $this->settings['solr_field_settings']['date_facet_format'];
    }
    else {
      return 'Y';
    }
  }

  /**
   * Prepare facet fields for text rendering.
   */
  public function prepareFacetFields() {
    $results = $this->results;
    $facet_field = $this->facet_field;
    $facet_results = array();
    module_load_include('inc', 'islandora_solr', 'includes/utilities');
    // It's possible that there could be a facet that's a date field
    // that's not a range.
    $date_format = isset($this->settings['solr_field_settings']['date_facet_format']) ? $this->settings['solr_field_settings']['date_facet_format'] : FALSE;
    foreach ($results as $bucket => $count) {
      $facet_results[] = array(
        'count' => $count,
        'filter' => islandora_solr_lesser_escape($facet_field) . ':"' .
        islandora_solr_facet_escape($bucket) . '"',
        'bucket' => $date_format ? format_date(strtotime($bucket), 'custom', $date_format) : $bucket,
      );
    }
    return $facet_results;
  }

  /**
   * Prepare facet dates for text rendering.
   */
  public function prepareFacetDates() {
    $facet_field = $this->facet_field;
    $results = $this->results;
    $format = $this->getDateFormat();
    $date_results = array();
    // Render date facet fields.
    foreach ($results as $bucket => $count) {
      // Don't include gap, end, etc that comes with range results.
      if (in_array($bucket, self::$exclude_range_values)) {
        continue;
      }
      $item = array();
      // Set count or documents.
      $item['count'] = $count;
      // Logic to get the next range key (next date).
      $field_keys = array_keys($results);
      $field_key = array_search($bucket, $field_keys);
      $field_key++;
      $bucket_next = (!in_array($field_keys[$field_key], self::$exclude_range_values)) ? $field_keys[$field_key] : $results['end'];
      // Set date range filter for facet URL.
      $item['filter'] = $facet_field . ':[' . $bucket . ' TO ' . $bucket_next . ']';
      // Set formatted value for facet link.
      $item['bucket'] = format_date(strtotime($bucket) + (60 * 60 * 24), 'custom', $format) . ' - ' . format_date(strtotime($bucket_next) + (60 * 60 * 24), 'custom', $format);
      $date_results[] = $item;
    }
    return $date_results;
  }

  /**
   * Render text facets.
   *
   * Based on a prepared array of results, this method will process and render
   * a facet as normal text. It includes bucket value, count, include link and
   * exclude link. If configured it also adds a 'read more' link to expose more
   * results.
   *
   * @param array $results
   *   An array with the prepared facet results.
   */
  public function renderText($results) {
    $facet_field = $this->facet_field;
    $islandora_solr_query = self::$islandoraSolrQuery;
    $soft_limit = self::$soft_limit;
    $buckets = array();
    $replace_bucket = (isset($this->settings['solr_field_settings']['pid_object_label']) && $this->settings['solr_field_settings']['pid_object_label'] ? TRUE : FALSE);
    $pid_mapper = function ($result) {
      $pid = str_replace('info:fedora/', '', $result['bucket']);
      return $pid;
    };
    $labels = array();
    if ($replace_bucket) {
      $valid_pids = array_filter(array_map($pid_mapper, $results), 'islandora_is_valid_pid');
      $mapping = array();
      foreach ($valid_pids as $valid_pid) {
        $mapping[$valid_pid] = "\"{$valid_pid}\"";
      }
      if (!empty($mapping)) {
        $qp = new IslandoraSolrQueryProcessor();
        $qp->buildQuery(format_string('PID:(!pids)', array(
          '!pids' => implode(' OR ', $mapping),
        )));
        $label_field = variable_get('islandora_solr_object_label_field', 'fgs_label_s');
        $qp->solrParams['facet'] = 'false';
        $qp->solrParams['fl'] = "PID, $label_field";
        $qp->solrLimit = count($mapping);
        $qp->executeQuery(FALSE, TRUE);
        $labels = array();
        if ($qp->islandoraSolrResult['response']['numFound'] > 0) {
          foreach ($qp->islandoraSolrResult['response']['objects'] as $doc) {
            $labels[$doc['PID']] = $doc['object_label'];
          }
        }
      }
    }
    foreach ($results as $key => $values) {
      $bucket = $values['bucket'];
      $filter = $values['filter'];
      $count = $values['count'];

      // Replace link bucket with object label based on facet field settings.
      if ($replace_bucket) {
        $label = NULL;
        $pid = $pid_mapper($values);
        if (isset($labels[$pid])) {
          $label = $labels[$pid];
        }
        // Fall back to islandora object if PID is not in solr.
        // eg: content models.
        else {
          if ($object = islandora_object_load($pid)) {
            $label = $object->label;
          }
        }
        $bucket = ($label ? $label : $bucket);
      }

      // Replace labels for boolean values if necessary.
      if ($bucket == 'true' && isset($this->settings['solr_field_settings']['boolean_facet_true_replacement']) && !empty($this->settings['solr_field_settings']['boolean_facet_true_replacement'])) {
        $bucket = $this->settings['solr_field_settings']['boolean_facet_true_replacement'];
      }
      if ($bucket == 'false' && isset($this->settings['solr_field_settings']['boolean_facet_false_replacement']) && !empty($this->settings['solr_field_settings']['boolean_facet_false_replacement'])) {
        $bucket = $this->settings['solr_field_settings']['boolean_facet_false_replacement'];
      }

      // Current URL query.
      $fq = isset($islandora_solr_query->solrParams['fq']) ? $islandora_solr_query->solrParams['fq'] : array();
      // 1: Check minimum count.
      // 2: Check if the filter isn't active.
      if ($count < self::$minimum_count || array_search($filter, $fq) !== FALSE) {
        continue;
      }
      // Current path including query, for example islandora/solr/query.
      // $_GET['q'] didn't seem to work here.
      $path = current_path();
      // Parameters set in URL.
      $params = $islandora_solr_query->internalSolrParams;
      // Set filter key if there are no filters included.
      if (!isset($params['f'])) {
        $params['f'] = array();
      }
      // Merge recursively to add new filter parameter.
      $query_plus = array_merge_recursive($params, array('f' => array($filter)));
      $query_minus = array_merge_recursive($params, array('f' => array('-' . $filter)));

      // Set basic attributes.
      $attributes = array(
        'link' => array(
          'path' => $path,
        ),
        'plus' => array(
          'path' => $path,
        ),
        'minus' => array(
          'path' => $path,
        ),
      );
      $attributes['link']['attr'] = $attributes['minus']['attr'] = $attributes['plus']['attr'] = array('rel' => 'nofollow');

      $attributes['link']['attr']['href'] = $attributes['plus']['attr']['href'] = url($path, array('query' => $query_plus));
      $attributes['link']['query'] = $attributes['plus']['query'] = $query_plus;
      $attributes['minus']['attr']['href'] = url($path, array('query' => $query_minus));
      $attributes['minus']['query'] = $query_minus;

      $attributes['plus']['attr']['class'] = array('plus');
      $attributes['minus']['attr']['class'] = array('minus');

      module_load_include('inc', 'islandora', 'includes/utilities');

      $hooks = islandora_build_hook_list(ISLANDORA_SOLR_FACET_BUCKET_CLASSES_HOOK_BASE);
      drupal_alter($hooks, $attributes, $islandora_solr_query);

      // XXX: We are not using l() because of active classes:
      // @see http://drupal.org/node/41595
      // Create link.
      $link['link'] = '<a' . drupal_attributes($attributes['link']['attr']) . '>' . $bucket . '</a>';
      $link['count'] = $count;
      $link['link_plus'] = '<a' . drupal_attributes($attributes['plus']['attr']) . '>+</a>';
      $link['link_minus'] = '<a' . drupal_attributes($attributes['minus']['attr']) . '>-</a>';
      $buckets[] = $link;
    }

    // Show more link.
    if (count($buckets) > $soft_limit) {
      $buckets_visible = array_slice($buckets, 0, $soft_limit);
      $buckets_hidden = array_slice($buckets, $soft_limit);
      $this->content .= theme('islandora_solr_facet', array(
        'buckets' => $buckets_visible,
        'hidden' => FALSE,
        'pid' => $facet_field,
      ));
      $this->content .= theme('islandora_solr_facet', array(
        'buckets' => $buckets_hidden,
        'hidden' => TRUE,
        'pid' => $facet_field,
      ));
      $this->content .= $this->showMore();
    }
    elseif (!empty($buckets)) {
      $this->content .= theme('islandora_solr_facet', array(
        'buckets' => $buckets,
        'hidden' => FALSE,
        'pid' => $facet_field,
      ));
    }
  }

  /**
   * Adds a 'show more' link.
   *
   * @return string
   *   Returns a rendered 'show more' link.
   */
  public function showMore() {
    drupal_add_js(drupal_get_path('module', 'islandora_solr') . '/js/islandora_solr_facets.js');
    $show_more = '<a href="#" class="soft-limit">' . t('Show more') . '</a>';
    return $show_more;
  }

  protected $sliderKey = NULL;

  /**
   * Prepare facet dates for slider.
   *
   * Preparing this array includes:
   * - Stripping dates highest and lowest end that have no results when
   *   variable range gap is used.
   * - Calculate the range gap based on the first two results.
   * - Based on the calculation, name the range and assign date formatting.
   * - Add color as set in user configuration.
   */
  public function prepareFacetDatesSlider() {
    $facet_field = $this->facet_field;
    $settings = $this->settings;
    $results = $this->results;
    $format = self::getDateFormat();
    $needed_solr_call = self::$needed_solr_call;
    if (!isset($this->sliderKey)) {
      $this->sliderKey = self::$range_slider_key++;
    }
    $range_slider_key = $this->sliderKey;

    $date_results = array();
    // Grab gap and end, and strip all non-buckets in results.
    $results_gap = $results['gap'];
    $results_end = $results['end'];
    foreach (self::$exclude_range_values as $exclude) {
      unset($results[$exclude]);
    }

    // Strip empty buckets top and bottom when no date range filters are set.
    if (!in_array($facet_field, $needed_solr_call)) {
      // Strip top.
      foreach ($results as $bucket => $count) {
        if ($count == 0) {
          unset($results[$bucket]);
        }
        else {
          break;
        }
      }
      // Reverse and strip other side.
      $results = array_reverse($results);
      $new_end = array();
      foreach ($results as $bucket => $count) {
        if ($count == 0) {
          unset($results[$bucket]);
          $new_end = array('bucket' => $bucket, 'count' => $count);
        }
        else {
          break;
        }
      }
      // Reverse to normal order.
      $results = array_reverse($results);

      // Add end date.
      if (isset($new_end['count']) AND $new_end['count'] == 0) {
        $end_bucket = $new_end['bucket'];
        $results[$end_bucket] = NULL;
      }
      else {
        $results[$results_end] = NULL;
      }
    }
    // Do not strip empty buckets left and right when filters are set.
    else {
      // Add end date.
      $results[$results_end] = NULL;
    }

    // If values are available.
    if (count($results) <= 1) {
      return array();
    }

    // Calculate gap.
    $calc_from = strtotime(key($results));
    next($results);
    $calc_to = strtotime(key($results));
    // Calculate difference between from and to date.
    $calc_diff = abs($calc_from - $calc_to);

    // Total difference in days.
    $calc_total_days = floor($calc_diff / 60 / 60 / 24);

    // @todo Could be done nicer?
    // @todo $date_format configurable.
    // Get gap based on total days diff.
    $gap = NULL;
    $date_format = 'Y';
    if ($calc_total_days < 7) {
      $gap = t('days');
      $date_format = 'M j, Y';
    }
    elseif ($calc_total_days >= 7 && $calc_total_days <= 28) {
      $gap = t('weeks');
      $date_format = 'M j, Y';
    }
    elseif ($calc_total_days >= 28 && $calc_total_days <= 32) {
      $gap = t('months');
      $date_format = 'M Y';
    }
    elseif ($calc_total_days >= 360 && $calc_total_days <= 370) {
      $gap = t('years');
      $date_format = 'Y';
    }
    elseif ($calc_total_days >= 720 && $calc_total_days <= 740) {
      $gap = t('2 years');
      $date_format = 'Y';
    }
    elseif ($calc_total_days >= 1800 && $calc_total_days <= 1850) {
      $gap = t('5 years');
      $date_format = 'Y';
    }
    elseif ($calc_total_days >= 3600 && $calc_total_days <= 3700) {
      $gap = t('decades');
      $date_format = 'Y';
    }
    elseif ($calc_total_days >= 36000 && $calc_total_days <= 37000) {
      $gap = t('centuries');
      $date_format = 'Y';
    }
    elseif ($calc_total_days >= 360000 && $calc_total_days <= 370000) {
      $gap = t('millennia');
      $date_format = 'Y';
    }

    // Create a nice array with our data.
    $data = array();
    foreach ($results as $bucket => $count) {
      $bucket_formatted = format_date(strtotime(trim($bucket)) + 1, 'custom', $date_format, 'UTC');

      $bucket_formatted = str_replace(' ', '&nbsp;', $bucket_formatted);
      $data[] = array(
        'date' => $bucket,
        'bucket' => $bucket_formatted,
        'count' => $count,
      );
    }

    // Add range slider color.
    if (isset($settings['solr_field_settings']['range_facet_slider_color']) &&
        !empty($settings['solr_field_settings']['range_facet_slider_color'])) {
      $slider_color = $settings['solr_field_settings']['range_facet_slider_color'];
    }
    else {
      $slider_color = '#edc240';
    }

    $elements = array(
      'data' => $data,
      'facet_field' => $facet_field,
      'slider_color' => $slider_color,
      'gap' => $gap,
      'date_format' => $date_format,
      'form_key' => $range_slider_key,
    );

    return $elements;
  }

  /**
   * Render slider.
   *
   * Based on the prepared results array, render the slider. The rendered
   * slider is a form. It passes the array and returns and renders the form.
   */
  public function renderSlider($facet_dates_as_slider) {
    $elements = $facet_dates_as_slider;

    // XXX: We do not want this to possibly grab from the cache when AJAXing,
    // since $elements changes... Makes a mess of rebuilding.
    $old_build_id = (
      // ... our form...
      isset($_POST['form_id']) && strpos($_POST['form_id'], 'islandora_solr_range_slider_form_') === 0 &&
      // ... AJAXing...
      isset($_POST['ajax_page_state']) &&
      // ... with the build ID.
      isset($_POST['form_build_id'])) ? $_POST['form_build_id'] : NULL;
    if (isset($old_build_id)) {
      $_POST['form_build_id'] = NULL;
    }
    $range_slider_form = drupal_get_form('islandora_solr_range_slider_form_' . $elements['form_key'], $elements);
    if (isset($old_build_id)) {
      // XXX: Restore the build ID to $_POST, just in case.
      $_POST['form_build_id'] = $old_build_id;
    }
    $this->content .= drupal_render($range_slider_form);
  }

  /**
   * Prepare facet dates filter.
   *
   * Prepares an array with settings to render a date/range filter.
   */
  public function prepareFacetDatesFilter() {
    $settings = $this->settings;
    // Datepicker range.
    if (isset($settings['solr_field_settings']['date_filter_datepicker_range']) && !empty($settings['solr_field_settings']['date_filter_datepicker_range'])) {
      $datepicker_range = $settings['solr_field_settings']['date_filter_datepicker_range'];
    }
    else {
      $datepicker_range = '-100:+3';
    }
    $elements = array(
      'facet_field' => $this->facet_field,
      'datepicker_range' => $datepicker_range,
      'form_key' => self::$date_filter_key,
    );
    self::$date_filter_key++;

    return $elements;
  }

  /**
   * Renders a facet dates filter.
   *
   * Based on the prepared array with elements, this method passes the array to
   * a form, returns it and then renders the form.
   */
  public function renderFacetDatesFilter($elements) {
    $date_filter_key = self::$date_filter_key;
    $date_filter_form = drupal_get_form('islandora_solr_date_filter_form_' . $elements['form_key'], $elements);
    if (!empty($this->content)) {
      $this->content .= drupal_render($date_filter_form);
    }
  }

  /**
   * Variable date gap.
   *
   * This method calculates the upper and lower ranges of a date range facet.
   * It also performs a second Solr query execution if necessary to recalculate
   * the date range facet with a new range gap.  This is a static function
   * because it calculates multiple date range facets, so in case multiple
   * facets need recalculation, it can do this in one Solr call, which saves on
   * performance. Statscomponent will be used in the future for range facets in
   * Solr 3.5 or higher.
   *
   * Tasks this method performs:
   * - 1: Gather info.
   * - 2: Calculate and render range gap.
   * - 3: Execute.
   * - 4: Prepare for return.
   * - 5: Update date facets.
   */
  public static function variableDateGap() {
    // 1: Gather info.
    // @todo Move this to separate functions.
    // Check settings.
    $facet_fields_settings = self::$facet_fields_settings;
    $variable_date_gap = array();
    foreach ($facet_fields_settings as $key => $settings) {
      if (isset($settings['solr_field_settings']['range_facet_variable_gap']) && $settings['solr_field_settings']['range_facet_variable_gap'] == 1) {
        $variable_date_gap[] = $settings['solr_field'];
      }
    }

    $islandora_solr_query = self::$islandoraSolrQuery;
    $fq = isset($islandora_solr_query->solrParams['fq']) ? $islandora_solr_query->solrParams['fq'] : array();
    $facet_dates = self::$facet_dates;
    // Populate with terms that needed a second solr call to update facets.
    $needs_solr_call = array();
    // Loop over all date facets.
    foreach ($facet_dates as $solr_field => $buckets) {
      $values = array();
      // Loop over all filters.
      foreach ($fq as $key => $filter) {
        // Check for enabled range filters.
        if (strpos($filter, $solr_field) === FALSE) {
          continue;
        }
        // Don't include excluded ranges for now, because it's complicated.
        // Maybe keep it like this for Solr 1.4. Solr 3.5 and up would
        // calculate this through the StatsComponent.
        if (substr($filter, 0, 1) == '-') {
          continue;
        }
        // @todo This could be done differently?
        // Check 'variable range gap' settings for this field.
        if (!in_array($solr_field, $variable_date_gap)) {
          continue;
        }
        // Split the filter into field and value.
        $filter_split = preg_split(ISLANDORA_SOLR_QUERY_FIELD_VALUE_SPLIT_REGEX, $filter, 2);
        // Trim brackets.
        $filter_split[1] = trim($filter_split[1], "\"");
        // Split range filter string to return formatted date values.
        $filter_str = $filter_split[1];
        $filter_str = trim($filter_str, '[');
        $filter_str = trim($filter_str, ']');
        $filter_array = explode(' TO ', $filter_str);
        // Collect values in array: timestamp => ISO8601.
        $from_str = trim($filter_array[0]);
        $to_str = trim($filter_array[1]);
        $field = trim($filter_split[0], ' -');
        // If a star is given, we need to perform an extra query to find out
        // what the minimum or maximum value is that 'star' would return.
        if (strpos($from_str, '*') !== FALSE) {
          $from_str = self::findMinMaxValue($field, 'asc');
        }
        if (strpos($to_str, '*') !== FALSE) {
          $to_str = self::findMinMaxValue($field, 'desc');
        }
        $values['from'][strtotime(strtolower($from_str))] = $from_str;
        $values['to'][strtotime(strtolower($to_str))] = $to_str;
      }

      // If the date facet field is found as at least one range filter,
      // calculate gap.
      if (empty($values)) {
        continue;
      }

      // From max value & to min value.
      $from_unix = max(array_keys($values['from']));
      $to_unix = min(array_keys($values['to']));

      // If the from date is bigger than the to date, abort.
      if ($from_unix >= $to_unix) {
        continue;
      }

      // Get ISO8601 values.
      $from = $values['from'][$from_unix];
      // If the hour is 00:00:00, subtract one second. If we always subtract
      // one second we keep eating away seconds after every filter.
      if (strpos($from, '00:00:00') !== FALSE) {
        $from .= '-1SECOND';
      }
      $to = $values['to'][$to_unix];

      // 2: Calculate and render range gap.
      // Calculate difference between from and to date.
      $diff = abs($from_unix - $to_unix);

      // Total difference in days.
      $total_days = floor($diff / 60 / 60 / 24);

      // @todo Make max buckets variable.
      // @todo Fine tune this: it's not very precise.
      // For maximum 15 buckets.
      if ($total_days <= 15) {
        $gap = '+1DAY';
      }
      elseif ($total_days <= 105) {
        $gap = '+7DAYS';
      }
      elseif ($total_days <= 450) {
        $gap = '+1MONTH';
      }
      elseif ($total_days <= 5475) {
        $gap = '+1YEAR';
      }
      elseif ($total_days <= 10950) {
        $gap = '+2YEARS';
      }
      elseif ($total_days <= 18250) {
        $gap = '+5YEARS';
      }
      elseif ($total_days <= 54750) {
        $gap = '+10YEARS';
      }
      elseif ($total_days <= 547500) {
        $gap = '+100YEARS';
      }
      elseif ($total_days <= 5475000) {
        $gap = '+1000YEARS';
      }

      // @todo Try to find a way to clone this object, because it's passed by
      // reference throughout the entire page call.
      // Update range facet values.
      $islandora_solr_query->solrParams["f.$solr_field.facet.date.start"] = $from;
      $islandora_solr_query->solrParams["f.$solr_field.facet.date.end"] = $to;
      $islandora_solr_query->solrParams["f.$solr_field.facet.date.gap"] = $gap;

      // Update variable.
      $needs_solr_call[] = $solr_field;
    }

    // 3: Execute.
    // If an extra solr call is necessary.
    if (!empty($needs_solr_call)) {
      // New query processor class.
      $range_query = new IslandoraSolrQueryProcessor();

      // Internal Solr query.
      $range_query->internalSolrQuery = $islandora_solr_query->internalSolrQuery;
      $range_query->solrLimit = 0;
      $range_query->solrStart = 0;
      $range_query->solrQuery = $islandora_solr_query->solrQuery;
      $range_query->solrParams = $islandora_solr_query->solrParams;
      // No need to include normal facets.
      unset($range_query->solrParams['facet.field']);

      // Excecute query.
      $range_query->executeQuery();

      // 4. Run query.
      $response_array = $range_query->islandoraSolrResult;

      // 5: Update date facets.
      self::$facet_dates = $response_array['facet_counts']['facet_dates'];
    }
    self::$needed_solr_call = $needs_solr_call;
  }

  /**
   * Finds the maximum or minimum value of a date field in a query.
   *
   * Called when one of the values in a date range filter equals to '*'.
   *
   * @param string $field
   *   Solr field to sort on.
   * @param string $order
   *   Sort order (asc or desc). Defaults to ascending.
   *
   * @return string
   *   Maximum or minimum value of a date field in the current query.
   */
  public static function findMinMaxValue($field, $order = 'asc') {
    $islandora_solr_query = self::$islandoraSolrQuery;

    // New query processor class.
    $min_max_query = new IslandoraSolrQueryProcessor();
    $min_max_query->internalSolrQuery = $islandora_solr_query->internalSolrQuery;
    $min_max_query->solrLimit = 1;
    $min_max_query->solrStart = 0;
    $min_max_query->solrQuery = $islandora_solr_query->solrQuery;
    $min_max_query->solrParams = $islandora_solr_query->solrParams;
    // No need to include normal facets.
    unset($min_max_query->solrParams['facet.field']);
    // Add the right sorting.
    $min_max_query->solrParams['sort'] = $field . ' ' . $order;
    $min_max_query->executeQuery();
    // Solr results.
    $results = $min_max_query->islandoraSolrResult;
    return $results['response']['objects'][0]['solr_doc'][$field];
  }
}
