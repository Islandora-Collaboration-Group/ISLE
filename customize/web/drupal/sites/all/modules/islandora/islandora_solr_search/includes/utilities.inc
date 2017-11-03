<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Initialize a pager for theme('pager') without running an SQL query.
 *
 * The nuts and bolts of this function are borrowed from z3950.module
 * and also apachesolr.module.
 *
 * @see pager_query()
 *
 * @param int $total
 *   The total number of items found.
 * @param int $limit
 *   The number of items you will display per page.
 * @param int $element
 *   An optional integer to distinguish between multiple pagers on one page.
 *
 * @return int
 *   The current page for $element. 0 by default if $_GET['page'] is empty.
 */
function islandora_solr_pager_init($total, $limit = 10, $element = 0) {
  global $pager_page_array, $pager_total, $pager_total_items;
  $page = isset($_GET['page']) ? $_GET['page'] : '';

  // Convert comma-separated $page to an array, used by other functions.
  $pager_page_array = explode(',', $page);

  // We calculate the total of pages as ceil(items / limit).
  $pager_total_items[$element] = $total;
  $pager_total[$element] = ceil($pager_total_items[$element] / $limit);
  $pager_page_array[$element] = max(0, min((int) $pager_page_array[$element], ((int) $pager_total[$element]) - 1));

  // Return the current position.
  return $pager_page_array[$element];
}

/**
 * Restore slashes in a string.
 *
 * Can't have a slash in the URL, as it will break Drupal clean URLs.
 *
 * @param string $str
 *   An arbitrary string.
 *
 * @return string
 *   The string with any occurences of '~slsh~' and '~plus~' changed to '/' and
 *   '+' respectively... Effectively the opposite of
 *   islandora_solr_replace_slashes().
 *
 * @see islandora_solr_replace_slashes()
 */
function islandora_solr_restore_slashes($str) {
  $replacements = array("~slsh~", "~plus~");
  $values = array("/", "+");
  return str_replace($replacements, $values, $str);
}

/**
 * Replace slashes in a string.
 *
 * Can't have a slash in the URL, as it will break Drupal clean URLs.
 *
 * @param string $str
 *   An arbitrary string.
 *
 * @return string
 *   The string with any occurences of '/' and '+' changed to '~slsh~' and
 *   '~plus~' respectively... Effectively the opposite of
 *   islandora_solr_restore_slashes().
 *
 * @see islandora_solr_restore_slashes()
 */
function islandora_solr_replace_slashes($str) {
  $replacements = array("~slsh~", "~plus~");
  $values = array("/", "+");
  return str_replace($values, $replacements, $str);
}

/**
 * A better explode method allows quotes in the returned strings.
 *
 * Taken from php.net.
 *
 * @param string $delim
 *   The delimiter on which to break (',' by default).
 * @param string $str
 *   The string to break.
 * @param string $enclose
 *   An enclosing character.
 * @param bool $preserve
 *   ??? Something to do with $enclose.
 *
 * XXX: Is this function really necessary?
 *
 * @return array
 *   An array containing the items of $str, broken apart on $delim.
 */
function islandora_solr_csv_explode($delim = ',', $str = '', $enclose = '"', $preserve = FALSE) {
  $res_arr = array();
  $n = 0;
  $exp_enc_arr = explode($enclose, $str);
  foreach ($exp_enc_arr as $enc_item) {
    if ($n++ % 2) {
      array_push($res_arr, array_pop($res_arr) . ($preserve ? $enclose : '') . $enc_item . ($preserve ? $enclose : ''));
    }
    else {
      $exp_del_arr = explode($delim, $enc_item);
      array_push($res_arr, array_pop($res_arr) . array_shift($exp_del_arr));
      $res_arr = array_merge($res_arr, $exp_del_arr);
    }
  }
  return $res_arr;
}

/**
 * Escape a string for inclusion in a facet query term.
 *
 * Copied from Apache_Solr_Service.
 *
 * The default charset diverges from Apache_Solr_Service::escape():
 *
 * @param string $value
 *   A string to escape.
 *
 * @param array $pattern
 *   An regular expression pattern
 *
 * @return string
 *   The escaped string.
 */
function islandora_solr_facet_query_escape($value, $pattern = NULL) {
  if (!$pattern) {
    $pattern = ISLANDORA_SOLR_QUERY_FACET_LUCENE_ESCAPE_REGEX_DEFAULT;
  }
  $replace = '\\\$1';
  return preg_replace($pattern, $replace, $value);
}

/**
 * Escape a string for inclusion in a query field or value.
 *
 * Copied from Apache_Solr_Service.
 *
 * Divergences from Apache_Solr_Service::escape():
 * - removed '?':
 *   - '?' is useful.
 * - removed '*':
 *   - Allows wildcard searches.
 * - added space:
 *   - Needed for spaces in fields.
 *
 * @param string $value
 *   A string to escape.
 *
 * @return string
 *   The escaped string.
 */
function islandora_solr_lesser_escape($value) {
  $pattern = '/(\/|\+|-|&&|\|\||!|\(|\)|\{|}|\[|]|\^| |~|\?|:|\\\)/';
  $replace = '\\\$1';

  return preg_replace($pattern, $replace, $value);
}

/**
 * Escape a string for inclusion in a facet.
 *
 * Copied from Apache_Solr_Service.
 *
 * Divergences from Apache_Solr_Service::escape():
 * - removed '?':
 *   - '?' is useful.
 * - removed '*':
 *   - Allows wildcard searches.
 * - added space:
 *   - Needed for spaces in fields.
 *
 * @param string $value
 *   A string to escape.
 *
 * @return string
 *   The escaped string.
 */
function islandora_solr_facet_escape($value) {
  $pattern = '/(\"|\/|\+|-|&&|\|\||!|\(|\)|\{|}|\[|]|\^| |~|\?|:|\\\)/';
  $replace = '\\\$1';

  return preg_replace($pattern, $replace, $value);
}

/**
 * Escape characters in field names of facets.
 */
function islandora_solr_escape($facets) {
  $return_facets = array();
  foreach ($facets as $facet) {
    $tmp = drupal_substr($facet, 0, strpos($facet, ':"'));
    $tmp = islandora_solr_lesser_escape(trim($tmp));

    $return_facets[] = $tmp . drupal_substr($facet, strpos($facet, ':"'), drupal_strlen($facet));
  }
  return $return_facets;
}

/**
 * Turns term fields strings from the admin forms into arrays.
 *
 * Mostly used with variable_get().
 *
 * @deprecated
 *   No longer used, due to the use of a separate database table for
 *   configuration.
 *
 * @param string $raw_field_list
 *   A string containing the list of fields to parse, with individual items
 *   separated by either a comma or newline. Each field entry may be either a
 *   field name alone, or a field name, a tilde, and then a human-readable
 *   name, so for the field "field_name":
 *   - "field_name": The field itself will be used as the "human-readable".
 *   - "field_name ~ My Cool Field": "My Cool Field" will be used as as the
 *     human-readable version.
 *
 * @return array
 *   An associative array mapping the fields to our human-readable label.
 */
function islandora_solr_build_substitution_list($raw_field_list) {
  $facet_array = array();
  $facet_fields = '';
  $lines = preg_split('/[,|\n|\r]/', $raw_field_list);
  foreach ($lines as $line) {
    if ($line) {
      $line_array = explode('~', $line);
      $key = trim($line_array[0]);
      if (count($line_array) > 1) {
        $value = trim($line_array[1]);
      }
      else {
        $value = $key;
      }
      $facet_array[$key] = $value;
      if ($facet_fields) {
        $facet_fields .= ",";
      }
      $facet_fields .= $key;
    }
  }
  return $facet_array;
}

/**
 * Prepares the Solr docs of an object results array.
 *
 * Preparing the Solr doc array includes adding the label, value and class name
 * based on the Solr field name. When you created a new Solr display, it is
 * not required to use this helper function, but it can be helpful.
 *
 * @param array $object_results
 *   An array containing the object results. This includes the Solr doc array.
 *
 * @return array
 *   The original object results array, but with a modified Solr doc.
 */
function islandora_solr_prepare_solr_doc($object_results) {
  $fields_all = islandora_solr_get_fields('result_fields', FALSE);
  // Loop over object results.
  foreach ($object_results as $object_index => $object_result) {
    $doc = $object_result['solr_doc'];
    $rows = array();
    foreach ($doc as $field => $value) {
      if (isset($fields_all[$field])) {
        $label = $fields_all[$field];
      }
      else {
        $label = $field;
      }
      $rows[$field]['label'] = $label;
      $rows[$field]['class'] = drupal_strtolower(preg_replace('/[^A-Za-z0-9]/', '-', $field));
      $rows[$field]['value'] = $value;
    }
    // Replace Solr doc.
    $object_results[$object_index]['solr_doc'] = $rows;
  }
  return $object_results;
}

/**
 * Prepares Solr results before rendering.
 *
 * Prepares Solr results before rendering. Applies highlighting, implodes
 * arrays and links to objects to Solr result fields.
 *
 * @param array $solr_results
 *   Array containing the Solr results which are altered trough the query
 *   processor.
 *
 * @return array
 *   Returns the same array but with prepared Solr field values.
 */
function islandora_solr_prepare_solr_results($solr_results) {
  module_load_include('inc', 'islandora_solr', 'includes/field_truncation');

  $object_results = $solr_results['response']['objects'];
  $highlighting = isset($solr_results['highlighting']) ? $solr_results['highlighting'] : array();
  $fields_all = islandora_solr_get_fields('result_fields', FALSE);
  $link_to_object = islandora_solr_get_link_to_object_fields();
  $truncate_length = islandora_solr_get_truncate_length_fields();
  $link_to_search = islandora_solr_get_link_to_search_fields();
  $date_format = islandora_solr_get_date_format_fields();

  // Loop over object results.
  foreach ($object_results as $object_index => $object_result) {
    $solr_doc = $object_result['solr_doc'];
    $pid = $object_result['PID'];
    $rows = array();
    $options = array('html' => TRUE);
    if (isset($object_result['object_label'])) {
      $options['attributes']['title'] = $object_result['object_label'];
    }
    if (isset($object_result['object_url_params'])) {
      $options['query'] = $object_result['object_url_params'];
    }
    if (isset($object_result['object_url_fragment'])) {
      $options['fragment'] = $object_result['object_url_fragment'];
    }

    foreach ($solr_doc as $field => $original_value) {
      $value = (array) $original_value;

      // If value is truncated, use the truncated value separator.
      if (array_key_exists($field, $truncate_length)) {
        $separator = variable_get('islandora_solr_search_truncated_field_value_separator', "<br />");

        if (empty($separator)) {
          $separator = "<br />";
        }
      }
      else {
        $separator = variable_get('islandora_solr_search_field_value_separator', ', ');

        if (empty($separator)) {
          $separator = ', ';
        }
      }

      // Date formatting.
      if (isset($date_format[$field])) {
        $map_date_format = function ($value) use ($date_format, $field) {
          return format_date(strtotime($value), 'custom', $date_format[$field], 'UTC');
        };
        $value = array_map($map_date_format, $value);
      }
      // Only apply highlighting when the date isn't formatted.
      elseif (isset($highlighting[$pid][$field])) {
        $value = $highlighting[$pid][$field];
      }

      // Truncate value lengths before linking, avoids destroying link tags.
      if (array_key_exists($field, $truncate_length) && $truncate_length[$field]['maximum_length'] > 0 && (!isset($truncate_length[$field]['truncation_type']) || $truncate_length[$field]['truncation_type'] == 'separate_value_option')) {
        // Coder tends to not like inline anonymous functions.
        $truncate_func = function (&$val_val) use ($field, $truncate_length) {
          $tf =& $truncate_length[$field];
          $wordsafe = FALSE;
          $min_wordsafe_length = 1;

          if (isset($tf['wordsafe'])) {
            $wordsafe = $tf['wordsafe'];
            $min_wordsafe_length = $tf['wordsafe_length'];
          }

          $val_val = (string) new IslandoraSolrFieldTruncation(
            $val_val,
            $tf['maximum_length'],
            $wordsafe,
            $tf['add_ellipsis'],
            $min_wordsafe_length
          );

        };
        array_walk($value, $truncate_func);
      }
      // Truncate output based on the field rather than by value.
      if (array_key_exists($field, $truncate_length) && $truncate_length[$field]['maximum_length'] > 0 && isset($truncate_length[$field]['truncation_type']) && $truncate_length[$field]['truncation_type'] == 'whole_field_option') {
        // Handle linking and truncating together.
        $link_options = array(
          'link_to_object' => in_array($field, $link_to_object),
          'link_to_search' => in_array($field, $link_to_search),
          'field' => $field,
          'url' => $object_result['object_url'],
          'options' => $options,
        );
        $value = islandora_solr_truncate_field_display($value, $truncate_length[$field]['maximum_length'], $truncate_length[$field]['add_ellipsis'], $truncate_length[$field]['wordsafe'], $truncate_length[$field]['wordsafe_length'], $separator, $link_options);
      }
      else {
        // Add link to search.
        if (in_array($field, $link_to_search)) {
          $map_to_link = function ($original_value, $formatted_value) use ($field) {
            $solr_query = format_string('!field:"!value"', array(
              '!field' => $field,
              '!value' => islandora_solr_lesser_escape($original_value),
            ));
            return l($formatted_value, "islandora/search/$solr_query", array(
              'html' => TRUE,
            ));
          };
          $value = array_map($map_to_link, (array) $original_value, (array) $value);
        }
        elseif (in_array($field, $link_to_object)) {
          $value = l(implode($separator, $value), $object_result['object_url'], $options);
        }
      }
      // Implode.
      $value = is_array($value) ? implode(filter_xss($separator, islandora_solr_get_filter_tags()), $value) : $value;
      $solr_doc[$field] = $value;
    }
    // Replace Solr doc rows.
    $solr_results['response']['objects'][$object_index]['solr_doc'] = $solr_doc;
  }
  return $solr_results;
}

/**
 * Implements callback_islandora_basic_collection_query_backends().
 */
function islandora_solr_islandora_basic_collection_backend_callable($collection_object, $page, $limit) {
  module_load_include('inc', 'islandora_solr', 'includes/db');
  // XXX: We populate the global query class, to allow the sort and facet
  // blocks and the like to function.
  global $_islandora_solr_queryclass;
  $qp = new IslandoraSolrQueryProcessor();
  $_islandora_solr_queryclass = $qp;

  $qp->buildQuery(format_string('!member_field:("info:fedora/!pid" OR "!pid") OR !collection_member_field:("info:fedora/!pid" OR "!pid")', array(
    '!member_field' => variable_get('islandora_solr_member_of_field', 'RELS_EXT_isMemberOf_uri_ms'),
    '!pid' => $collection_object->id,
    '!collection_member_field' => variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms'),
  )), drupal_get_query_parameters());

  if (variable_get('islandora_solr_collection_result_limit_block_override', FALSE)) {
    if (isset($qp->internalSolrParams['limit'])) {
      $limit = $qp->internalSolrParams['limit'];
    }
    else {
      $limit = variable_get('islandora_solr_num_of_results', '20');
    }
  }

  $qp->solrStart = $page * $limit;
  $qp->solrLimit = $limit;

  if (variable_get('islandora_solr_collection_sort_block_override', FALSE) && isset($qp->internalSolrParams['sort'])) {
    $qp->solrParams['sort'] = $qp->internalSolrParams['sort'];
  }
  else {
    $qp->solrParams['sort'] = islandora_solr_get_collection_sort_string($collection_object->id, TRUE);
  }

  $qp->executeQuery();

  $map_to_pids = function ($result) {
    return $result['PID'];
  };
  return array(
    $qp->islandoraSolrResult['response']['numFound'],
    array_map($map_to_pids, $qp->islandoraSolrResult['response']['objects']),
  );
};

/**
 * Truncate and link the field display based on entire field result(s).
 *
 * @param array $display_values
 *   An array of the values that are to be processed for truncation.
 * @param int $max_length
 *   The maxium length of characters to display before truncating results.
 * @param bool $add_ellipsis
 *   Boolean to enable the display of an ellispsis.
 * @param bool $word_safe
 *   Boolean to enable word safe in truncate_utf8.
 * @param int $wordsafe_length
 *   Min wordsafe length.
 * @param string $separator
 *   A separator to use for display output.
 * @param array|NULL $link_options
 *   (Optional) An array of link options including:
 *   - 'link_to_object' : (bool) Include link to object.
 *   - 'link_to_search' : (bool) Include link to search results.
 *   - 'field' : (SOLR Result Field) Result field.
 *   - 'url' : (SOLR object results object_url) objects result url.
 *   - 'options' : link to object l() options param.
 *
 * @return string
 *   The updated display values.
 */
function islandora_solr_truncate_field_display($display_values, $max_length, $add_ellipsis, $word_safe, $wordsafe_length, $separator, $link_options = NULL) {
  module_load_include('inc', 'islandora_solr', 'includes/field_truncation');

  $updated_display_values = $display_values;
  $separator = filter_xss($separator, islandora_solr_get_filter_tags());

  if (count($updated_display_values) > 0) {
    $value_is_truncated = FALSE;
    $mod_path = drupal_get_path('module', 'islandora_solr');
    drupal_add_js("$mod_path/js/truncation-toggle.js");
    // Build two arrays for display, one filtered by size (max_values)
    // and one with all the data.
    $truncated_list = array();
    $character_output_count = 0;
    foreach ($updated_display_values as $index => $current_value) {
      if ($character_output_count + drupal_strlen($current_value) <= $max_length) {
        $truncated_list[] = $current_value;
        $character_output_count += drupal_strlen($current_value);
      }
      elseif ($character_output_count + drupal_strlen($current_value) > $max_length) {
        $value_is_truncated = TRUE;
        $truncation_length = $max_length - $character_output_count;
        // Force the display of the full ellipsis.
        if ($add_ellipsis) {
          $truncation_length = max($truncation_length, drupal_strlen(t('...')));
        }

        $truncated_list[] = (string) new IslandoraSolrFieldTruncation(
          $current_value,
          $truncation_length,
          $word_safe,
          $add_ellipsis,
          $wordsafe_length
        );

        break;
      }
    }

    $original_value = implode($separator, $updated_display_values);
    $truncated_value = implode($separator, $truncated_list);
    if ($link_options !== NULL) {
      if (isset($link_options['link_to_object']) && $link_options['link_to_object'] == TRUE) {
        $original_value = l($original_value, $link_options['url'], $link_options['options']);
        $truncated_value = l($truncated_value, $link_options['url'], $link_options['options']);
      }
      elseif (isset($link_options['link_to_search']) && $link_options['link_to_search'] == TRUE) {
        $solr_query = format_string('!field:"!value"', array(
          '!field' => $link_options['field'],
          '!value' => islandora_solr_lesser_escape($original_value),
          ));
        $original_value = l(
          $original_value,
          "islandora/search/$solr_query",
          array(
            'html' => TRUE,
          )
        );
        $truncated_value = l(
          $truncated_value,
          "islandora/search/$solr_query",
          array(
            'html' => TRUE,
          )
        );
      }
    }

    $updated_display_values
      = "<span class='toggle-wrapper'>" .
      t("<span>!value <br />!show_more</span>", array(
        '!value' => $truncated_value,
        '!show_more' => ($value_is_truncated ? "<a href='#' class='toggler'>Show more</a>" : ""),
      ));
    if ($value_is_truncated) {
      $updated_display_values .= t("<span>!original_value <br /><a href='#' class='toggler'>Show less</a></span>", array(
        '!original_value' => $original_value,
      ));
    }
    $updated_display_values .= '</span>';

  }
  else {
    $updated_display_values = '';
  }

  return $updated_display_values;
};

/**
 * Check the field type against the user-specified list of date field types.
 *
 * @param string $solr_field
 *   The Solr field name
 *
 * @return bool
 *   Whether the field matches the date field types.
 */
function islandora_solr_is_date_field($solr_field) {
  $date_types = array(
    'org.apache.solr.schema.DateField',
    'org.apache.solr.schema.TrieDateField',
  );
  return islandora_solr_is_typed_field($solr_field, $date_types);
}

/**
 * Check the field type against boolean-type fields.
 *
 * @param string $solr_field
 *   The Solr field name.
 *
 * @return bool
 *   Whether the field is a boolean type.
 */
function islandora_solr_is_boolean_field($solr_field) {
  $bool_types = array(
    'org.apache.solr.schema.BoolField',
  );
  return islandora_solr_is_typed_field($solr_field, $bool_types);
}

/**
 * Check the field type against an array of types.
 *
 * @param string $solr_field
 *   The Solr field name.
 * @param array $types
 *   A list of Solr field type classes to check against.
 *
 * @return bool
 *   Whether the field type is in the provided list.
 */
function islandora_solr_is_typed_field($solr_field, $types) {
  $luke_result = islandora_solr_get_luke(NULL, $solr_field);
  $type = (isset($luke_result['fields'][$solr_field]['type']) ? $luke_result['fields'][$solr_field]['type'] : FALSE);
  if ($type) {
    $class = islandora_solr_get_type_class($type);
    return in_array($class, $types);
  }
  return FALSE;
}

/**
 * Removes the base filters from the Solr params if they exist.
 *
 * @param array $params
 *   The Solr params array.
 *
 * @return array
 *   The $params array minus any base filters.
 */
function islandora_solr_remove_base_filters(array $params) {
  if (isset($params['fq'])) {
    $base_filters = preg_split("/\\r\\n|\\n|\\r/", variable_get('islandora_solr_base_filter', ''), -1, PREG_SPLIT_NO_EMPTY);
    $params['fq'] = array_diff($params['fq'], $base_filters);
  }
  return $params;
}

/**
 * Return an array of html tags to filter against when preparing solr display.
 *
 * @return array
 *   The array of html tags being filtered against.
 */
function islandora_solr_get_filter_tags() {
  $filter_tags = array(
    'a',
    'em',
    'strong',
    'cite',
    'blockquote',
    'code',
    'ul',
    'ol',
    'li',
    'dl',
    'dt',
    'dd',
    'br',
    'span',
    'div',
  );

  return $filter_tags;
}
