<?php
/**
 * @file
 * Theme functions for the islandora_solr_metadata module.
 */

/**
 * Implements theme_element().
 */
function theme_islandora_solr_metadata_management_table(array $variables) {
  $table = $variables['table'];
  $rows = array();
  $weight_class = 'islandora-solr-metadata-weight';
  foreach (element_children($table['rows']) as $id) {
    $row = &$table['rows'][$id];
    $row['weight']['#attributes'] = array('class' => array($weight_class));
    $rows[] = array(
      'data' => array(
        drupal_render($row['remove_field']),
        drupal_render($row['solr_field_markup']),
        drupal_render($row['label']),
        drupal_render($row['operations']),
        drupal_render($row['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $table_id = 'entry-order';
  $output = theme('table', array(
    'header' => $table['#header'],
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
    'empty' => t('There are no entries available.')));
  $output .= drupal_render_children($table);
  drupal_add_tabledrag($table_id, 'order', 'sibling', $weight_class);
  return $output;
}

/**
 * Prepares variables for islandora_solr_metadata_display templates.
 *
 * Default template: islandora-solr-metadata-display.tpl.php
 *
 * @param array $variables
 *   An associative array containing:
 *   - islandora_object: The AbstractObject for which we are generating a
 *     metadata display.
 *   - print: A boolean indicating to disable some functionality, to facilitate
 *     printing. When TRUE, avoids adding the "collapsible" and "collapsed"
 *     classes to fieldsets.
 */
function template_preprocess_islandora_solr_metadata_display(array &$variables) {
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  module_load_include('inc', 'islandora', 'includes/utilities');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');

  $object = $variables['islandora_object'];
  $db_fields = array();
  $solr_fields =& $variables['solr_fields'];
  $associations = $variables['associations'];

  foreach ($associations as $configuration_id) {
    $field = islandora_solr_metadata_get_fields($configuration_id['configuration_id']);
    $db_fields = array_merge($db_fields, $field);
  }
  foreach ($db_fields as $solr_field => $value) {
    if (isset($solr_fields[$solr_field])) {
      continue;
    }
    // Make an array for use later on.
    $solr_fields[$solr_field] = $value + array(
      'value' => array(),
    );
  }
  $separator = variable_get('islandora_solr_metadata_field_value_separator', "\n");

  if (empty($separator)) {
    $separator = "\n";
  }
  $variables['separator'] = $separator;
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
}

/**
 * Implements template_process_HOOK() for islandora_solr_metadata_description.
 */
function template_process_islandora_solr_metadata_display(array &$variables) {
  $variables['found'] = islandora_solr_metadata_query_fields($variables['islandora_object'], $variables['solr_fields']);
  $variables['not_found_message'] = t('Document not present in Solr while generating the metadata display for @id. Try !reloading the page, or contact an administrator if this issue persists.', array(
    '@id' => $variables['islandora_object']->id,
    '!reloading' => '<a href="" onclick="window.location.reload(true)">' . t('reloading') . '</a>',
  ));
}

/**
 * Prepares variables for islandora_solr_metadata_description templates.
 *
 * Default template: islandora-solr-metadata-description.tpl.php
 *
 * @param array $variables
 *   An associative array containing:
 *   - islandora_object: The AbstractObject for which we are generating a
 *     metadata display.
 */
function template_preprocess_islandora_solr_metadata_description(array &$variables) {
  $object = $variables['islandora_object'];
  $solr_fields =& $variables['solr_fields'];
  $associations = $variables['associations'];

  $descriptions = array();
  module_load_include('inc', 'islandora_solr_metadata', 'includes/db');
  foreach ($associations as $configuration_id) {
    $description = islandora_solr_metadata_retrieve_description($configuration_id['configuration_id']);
    if ($description['description_field'] !== NULL) {
      $descriptions[] = $description;
    }
  }
  foreach ($descriptions as $description) {
    $desc_field = $description['description_field'];
    $solr_fields[$desc_field] = array(
      'display_label' => $description['description_label'],
      'value' => array(),
      'description_data' => $description['description_data'],
    );
  }
  $separator = variable_get('islandora_solr_metadata_field_value_separator', "\n");

  if (empty($separator)) {
    $separator = "\n";
  }
  $variables['separator'] = $separator;
}

/**
 * Implements template_process_HOOK() for islandora_solr_metadata_description.
 */
function template_process_islandora_solr_metadata_description(array &$variables) {
  $variables['found'] = islandora_solr_metadata_query_fields($variables['islandora_object'], $variables['solr_fields']);

  // XXX: Seems a little odd, yes... Was reusing the "description" value,
  // though.
  $variables['description'] = $variables['solr_fields'];
  $separator = variable_get('islandora_solr_metadata_field_value_separator', "\n");

  if (empty($separator)) {
    $separator = "\n";
  }
  // Truncate the description value(s).
  foreach ($variables['description'] as &$field) {
    if (isset($field['description_data']['truncation'])) {
      if ($field['description_data']['truncation']['max_length'] > 0) {
        module_load_include('inc', 'islandora_solr', 'includes/utilities');
        $field['value'] = array_filter($field['value']);
        if ($field['description_data']['truncation']['truncation_type'] == 'whole_field_option') {
          $updated_value[] = islandora_solr_truncate_field_display(
            $field['value'],
            $field['description_data']['truncation']['max_length'],
            $field['description_data']['truncation']['ellipsis'],
            $field['description_data']['truncation']['word_safe'],
            $field['description_data']['truncation']['min_wordsafe_length'],
            $separator
          );
          $field['value'] = $updated_value;
        }
        else {
          foreach ($field['value'] as $index => $value) {
            $field['value'][$index] = islandora_solr_truncate_field_display(
              (array) $value,
              $field['description_data']['truncation']['max_length'],
              $field['description_data']['truncation']['ellipsis'],
              $field['description_data']['truncation']['word_safe'],
              $field['description_data']['truncation']['min_wordsafe_length'],
              " "
            );
          }
        }
      }
    }
  }
}

/**
 * Perform the query to Solr to get our metadata values.
 *
 * @param AbstractObject $object
 *   The object for which we are going to query Solr... Really only need the
 *   PID...
 * @param array $solr_fields
 *   An associative array mapping Solr fields to associative arrays, of which
 *   we use:
 *   - hyperlink: An optional boolean indicating if the given value should be
 *     rendered a link.
 *   - weight: An optional weight we will use to sort the values.
 *   - formatter: An optional callable to which we will call with each value
 *     from the given field, to return alternate text or markup as desired.
 *
 * @return bool
 *   TRUE if we have found a document for the object in question; otherwise,
 *   FALSE.
 */
function islandora_solr_metadata_query_fields($object, &$solr_fields) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  if (!empty($solr_fields)) {
    global $user;

    $query_processor = new IslandoraSolrQueryProcessor();
    $query_processor->solrQuery = 'PID:"' . $object->id . '"';
    $query_processor->solrParams['fl'] = '*';
    $query_processor->executeQuery(FALSE);
    if ($query_processor->islandoraSolrResult['response']['numFound'] > 0) {
      $solr_results_doc = $query_processor->islandoraSolrResult['response']['objects']['0']['solr_doc'];

      // Ensure we only provide results for what is specified.
      $constrained_results = array_intersect_key($solr_results_doc, $solr_fields);

      foreach ($constrained_results as $solr_field => $value) {
        // XXX. Need to pass $solr_fields by reference here even though it's not
        // modified to get around a bug in PHP 5.3.3 when used on CentOS 6.6,
        // otherwise the calling function doesn't maintain a reference to the
        // original value and it becomes a copy.
        $get_display_value = function ($original_value) use ($solr_field, &$solr_fields) {
          $value = $original_value;
          $field_config = $solr_fields[$solr_field] + array(
            'hyperlink' => 0,
            'formatter' => NULL,
            'uri_replacement' => '',
          );
          // Replace URI or PID with a configured field.
          if (!empty($field_config['uri_replacement'])) {
            module_load_include('inc', 'islandora', 'includes/utilities');
            $pid = str_replace('info:fedora/', '', $value);
            if (islandora_is_valid_pid($pid)) {
              $relation_qp = new IslandoraSolrQueryProcessor();
              $relation_qp->solrQuery = 'PID:"' . $pid . '"';
              $relation_qp->solrParams['fl'] = "PID,{$field_config['uri_replacement']}";
              $relation_qp->solrLimit = 1;
              $relation_qp->executeQuery(FALSE);
              if ($relation_qp->islandoraSolrResult['response']['numFound'] > 0) {
                $relation_doc = $relation_qp->islandoraSolrResult['response']['objects'][0]['solr_doc'];
                if (isset($relation_doc[$field_config['uri_replacement']])) {
                  $value = $relation_doc[$field_config['uri_replacement']];
                }
              }
            }
          }
          if (($truncation = drupal_array_get_nested_value($field_config, array('truncation'))) && $truncation['max_length'] > 0 && (!isset($truncation['truncation_type']) || $truncation['truncation_type'] == 'separate_value_option')) {
            $value = truncate_utf8($value, $truncation['max_length'], $truncation['word_safe'], $truncation['ellipsis'], $truncation['min_wordsafe_length']);
            if ($value !== $original_value) {
              $mod_path = drupal_get_path('module', 'islandora_solr');
              drupal_add_js("$mod_path/js/truncation-toggle.js");
              // XXX: implode()'ing to avoid newlines which get converted to
              // <br/>'s in the later check_markup.
              return implode('', array(
                '<span class="toggle-wrapper">',
                t("<span>!value <a href='#' class='toggler'>Show more</a></span>", array(
                  '!value' => $value,
                )),
                t("<span>!original_value <a href='#' class='toggler'>Show less</a></span>", array(
                  '!original_value' => $original_value,
                )),
                '</span>',
              ));
            }
          }

          if (is_callable($field_config['formatter'])) {
            $value = call_user_func($field_config['formatter'], $value);
          }
          if ($field_config['hyperlink'] == 1 && trim($value)) {
            if (variable_get('islandora_solr_metadata_omit_empty_values', FALSE) && !trim($value)) {
              // Avoid generating markup (throwing off the "empty" check).
              // Will get stripped out later.
              return;
            }
            $solr_query = format_string('!field:!value', array(
              '!field' => $solr_field,
              '!value' => islandora_solr_facet_escape($original_value),
            ));
            return l($value, 'islandora/search', array(
              "query" => array(
                "type" => "dismax",
                "f" => array(
                  $solr_query,
                ),
              ),
            ));
          }
          return $value;
        };

        // Want the values in array form even if it's a single valued field.
        if (!is_array($value)) {
          $value = (array) $value;
        }

        $solr_fields[$solr_field]['value'] = array_map($get_display_value, $value);
      }

      if (variable_get('islandora_solr_metadata_omit_empty_values', FALSE)) {
        $filter_function = function ($field_info) {
          // Remove all empty field values to determine if field is empty.
          $filtered_values = array_filter($field_info['value']);
          return !empty($filtered_values);
        };
        $filtered = array_filter($solr_fields, $filter_function);
        $solr_fields = $filtered;
      }

      if (variable_get('islandora_solr_metadata_dedup_values', FALSE)) {
        foreach ($solr_fields as &$field) {
          $field['value'] = array_unique($field['value']);
        }
        unset($field);
      }

      // After values are omitted/dedupped filter down the displayed results.
      foreach ($solr_fields as &$field) {
        // Check if the field is configured to filter based on whole field.
        if (($truncation = drupal_array_get_nested_value($field, array('truncation'))) && $truncation['max_length'] > 0 && isset($truncation['truncation_type']) && $truncation['truncation_type'] == 'whole_field_option') {
          // Keep the updated value stored in an array.
          $updated_value[] = islandora_solr_truncate_field_display($field['value'], $truncation['max_length'], $truncation['ellipsis'], $truncation['word_safe'], $truncation['min_wordsafe_length'], variable_get('islandora_solr_metadata_field_value_separator', "\n"));
          // Replace value with updated field display.
          $field['value'] = $updated_value;
        }

        // Apply rules configurations for this field as configured
        // on the metadata configuration page. Unset the field from
        // the $solr_fields array to prevent it from being shown.
        $permission = drupal_array_get_nested_value($field, array('permissions'));
        if ($permission['enable_permissions'] && !array_intersect(array_keys($user->roles), $permission['permissions'])) {
          // Permissions are being enforced and none of our roles are allowed...
          unset($solr_fields[$field['solr_field']]);
        }
      }

      uasort($solr_fields, 'drupal_sort_weight');
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}
