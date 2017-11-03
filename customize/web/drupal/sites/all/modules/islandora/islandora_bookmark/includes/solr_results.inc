<?php

/**
 * @file
 * Contains methods to search solr and display results. Depends on
 * Apache_Solr_Php client.
 */

/**
 * Extention of IslandoraSolrResults to create an alternative display type.
 */
class IslandoraSolrResultsBookmark extends IslandoraSolrResults {
  /**
   * Constructor.
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Renders the Solr results as a table.
   *
   * @see displayResults()
   *
   * @param array $results
   *   The raw Solr results from IslandoraSolrQueryProcessor::solrResult
   *
   * @return array
   *   Rendered Solr results
   */
  public function printResults($results) {
    drupal_add_css(drupal_get_path('module', 'islandora_solr_config') . '/css/islandora_solr_config.theme.css');

    if (empty($results)) {
      return t('No results');
    }
    $results_form = drupal_get_form('islandora_bookmark_results_form', $this, $results['response']['objects']);
    return drupal_render($results_form);
  }

  /**
   * Form building method.
   *
   * @param array $form
   *   The Drupal form definition.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $object_results
   *   The object results from the Islandora Solr processor created in the
   *   printResults function.
   *
   * @return array
   *   An array containing form structure.
   */
  public function formGet($form, &$form_state, $object_results) {
    module_load_include('inc', 'islandora_bookmark', 'includes/api');
    $form_state['islandora_solr_query_processor'] = $this->islandoraSolrQueryProcessor;

    if (module_exists('citation_exporter')) {
      if (CitationExporter::ReadyToExport()) {
        $citation_export_head = array(
          '#type' => 'html_tag',
          '#tag' => 'meta',
          '#attributes' => array(
            'http-equiv' => 'refresh',
            'content' => '1.0;' . url(CITATION_EXPORTER_EXPORT_MENU, array('absolute' => TRUE)),
          ),
        );
        drupal_add_html_head($citation_export_head, 'citation_exporter_head');
      }
    }

    $header = $this->getTableHeader();
    $rows = array();
    foreach ($object_results as $object_result) {
      $rows[$object_result['PID']] = $this->getTableRow($object_result);
    }

    if (count($rows) > 0) {
      $form = array_merge($form, $this->getActions($form_state));
    }

    $form['islandora_bookmark_table'] = array(
      '#header' => $header,
      '#type' => 'tableselect',
      '#options' => $rows,
      '#attributes' => array('class' => array('islandora-bookmark-solr-results')),
      '#empty' => t('No results found.'),
    );
    return $form;
  }

  /**
   * Get the header row to use in the tableselect.
   *
   * @return array
   *   An associative array as used as headers by theme_tableselect()/
   */
  protected function getTableHeader() {
    return array(
      'markup' => t('Label'),
    );
  }

  /**
   * Build a row for inclusion in the tableselect.
   *
   * @param array $object_result
   *   A result from the query processor.
   *
   * @return array
   *   An associative array of cells, keyed according to the header provided in
   *   $this->getTableHeader().
   */
  protected function getTableRow($object_result) {
    $doc = $object_result['solr_doc'];
    $solr_field = variable_get('islandora_solr_object_label_field', 'fgs_label_s');
    $markup = '';
    if (isset($doc[$solr_field])) {
      $markup = $doc[$solr_field];
    }
    else {
      $markup = $object_result['PID'];
    }

    $object_url_info = array(
      'path' => $object_result['object_url'],
      'params' => $object_result['object_url_params'],
      'markup' => $markup,
    );

    return array(
      'markup' => islandora_bookmark_generate_markup($object_result['PID'], $object_url_info),
    );
  }

  /**
   * Get the "actions" section of the form.
   *
   * @param array $form_state
   *   The Drupal form state.
   *
   * @return array
   *   A form fragment containing a number of actions which might be performed
   *   on objects from the table select.
   *
   *   Submit buttons may have two additional properties:
   *   - action_validate: An array of validation methods to call, which should
   *     form_set_error() as necessary.
   *   - action_submit: An array of submission methods to call.
   *   as hinted, these are /methods/, which should exist on the given class
   *   (or one of its ancestors).
   */
  protected function getActions(&$form_state) {
    $groups = array();

    if (user_access('export islandora bookmarks')) {
      $export = islandora_bookmark_get_export_options();
      $export_options['default'] = t('- Export type -');
      $export_styles = array();
      foreach ($export as $module_name => $export_array) {
        // As export options can be user configurable and want to maintain
        // human readability through the UI need to account for the fact that
        // some options could have spaces in them. If spaces are encountered in
        // the $_POST Drupal will not pick them up in the form state values.
        // As such two formats can be used one which specifics a callable
        // and a human-readable pair and the old form.
        foreach ($export_array as $name => $export_func) {
          if (is_callable($export_func)) {
            $export_options[t("@module_name", array("@module_name" => $module_name))][$name] = $name;
            $form_state['islandora_bookmark_export'][$name] = $export_func;
          }
          else {
            $export_options[t("@module_name", array("@module_name" => $module_name))][$name] = $export_func['human_readable'];
            $form_state['islandora_bookmark_export'][$name] = $export_func['callable'];
          }
          $export_styles[$name] = islandora_bookmark_get_export_format_options($name);
        }
      }

      $group = array(
        '#type' => 'fieldset',
        '#title' => t('Export to file'),
        '#weight' => 0,
      );

      $group['export_styles'] = array(
        '#weight' => 4,
      );
      foreach ($export_styles as $key => $value) {
        if (!empty($export_styles[$key])) {
          $group['export_styles'][$key] = array(
            '#type' => 'select',
            '#title' => t('Style'),
            '#options' => $export_styles[$key],
            '#weight' => 4,
            '#states' => array(
              'visible' => array(
                ':input[name="export_options"]' => array('value' => $key),
              ),
            ),
          );
        }
      }

      $group['export_options'] = array(
        '#type' => 'select',
        '#options' => $export_options,
        '#weight' => 3,
        '#title' => t('Format'),
      );

      $group['export_selected_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Export selected objects'),
        '#name' => 'export_selected',
        '#weight' => 5,
        '#action_submit' => 'exportSelected',
        '#action_validate' => array(
          'hasExportTypeSelected',
          'hasObjectsSelected',
        ),
      );
      $group['export_all_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Export all results'),
        '#name' => 'export_all',
        '#weight' => 5,
        '#action_submit' => 'exportAll',
        '#action_validate' => array(
          'hasExportTypeSelected',
        ),
      );

      $groups['islandora_bookmark_export'] = array(
        'fieldset' => $group,
      );
    }

    $user_bookmarks = islandora_bookmark_get_user_owned_bookmarks();
    if (count($user_bookmarks) > 0) {
      $save_options = array();
      $save_options['default'] = t('- Select @type list -', array(
        '@type' => variable_get('islandora_bookmark_type', 'bookmark'),
      ));

      foreach ($user_bookmarks as $bookmark) {
        $save_options[$bookmark->getId()] = $bookmark->bookmarkName;
      }

      $group = array(
        '#type' => 'fieldset',
        '#title' => t('@type', array('@type' => ucwords(variable_get('islandora_bookmark_type', 'bookmark')))),
        '#weight' => 0,
      );
      $group['save_options'] = array(
        '#type' => 'select',
        '#options' => $save_options,
        '#weight' => 3,
      );
      $group['save_selected_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Bookmark selected objects'),
        '#weight' => 4,
        '#action_submit' => 'saveSelected',
        '#action_validate' => array(
          'hasListSelected',
          'hasObjectsSelected',
        ),
      );
      $group['save_all_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Bookmark all results'),
        '#weight' => 5,
        '#action_submit' => 'saveAll',
        '#action_validate' => array(
          'hasListSelected',
        ),
      );
      $group['go_to_bookmark'] = array(
        '#type' => 'submit',
        '#value' => t('View list'),
        '#name' => 'goto_bookmark',
        '#weight' => 6,
        '#action_submit' => 'goToBookmark',
        '#action_validate' => array(
          'hasListSelected',
        ),
      );

      $groups['islandora_bookmark_save_fieldset'] = array(
        'fieldset' => $group,
      );
    }

    return $groups;
  }

  /**
   * One of our '#action_validate' callbacks.
   *
   * Checks that the export action has an export type selected.
   */
  protected function hasExportTypeSelected($form, &$form_state) {
    $el = $form['islandora_bookmark_export']['fieldset']['export_options'];
    $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);
    if ($key == 'default') {
      form_set_error(implode('][', $el['#parents']), t('Please select an export type.'));
    }
  }

  /**
   * One of our '#action_validate' callbacks.
   *
   * Checks that the bookmark save/view action has a list selected.
   */
  protected function hasListSelected($form, &$form_state) {
    $el = $form['islandora_bookmark_save_fieldset']['fieldset']['save_options'];
    $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);
    if ($key == 'default') {
      form_set_error(implode('][', $el['#parents']), t('Please select a list.'));
    }
  }

  /**
   * One of our '#action_validate' callbacks.
   *
   * Checks that some objects are selected.
   */
  protected function hasObjectsSelected($form, &$form_state) {
    $pids = $this->getSelected($form_state);
    if (empty($pids)) {
      form_set_error('islandora_bookmark_table', t('You must select at least one object.'));
    }
  }

  /**
   * One of our '#action_submit' callbacks.
   *
   * Export all PIDs the search yielded.
   *
   * @param array $form
   *   The array representing a Drupal form.
   * @param array $form_state
   *   The array of form state.
   */
  protected function exportAll(&$form, &$form_state) {
    $pids = $this->getAll($form_state);
    $this->export($pids, $form_state);
  }

  /**
   * One of our '#action_submit' callbacks.
   *
   * Export the selected PIDs the search yielded.
   *
   * @param array $form
   *   The array representing a Drupal form.
   * @param array $form_state
   *   The array of form state.
   */
  protected function exportSelected(&$form, &$form_state) {
    $pids = $this->getSelected($form_state);
    $this->export($pids, $form_state);
  }

  /**
   * Actually handle the export of objects.
   *
   * @param array $pids
   *   Multiple PIDs, as strings.
   * @param array $form_state
   *   The array of form state.
   */
  protected function export($pids, &$form_state) {
    $el = $form_state['complete form']['islandora_bookmark_export']['fieldset']['export_options'];
    $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);

    $params = array(
      $pids,
    );
    if (array_key_exists($key, $form_state['values'])) {
      $params[] = $form_state['values'][$key];
    }
    // Preserve position.
    else {
      $params[] = NULL;
    }
    // Add the form state on the end for use in potential form alters. Doing it
    // here as opposed to earlier to preserve the listing of parameters that
    // existing implementations may expect.
    $params[] = $form_state;
    call_user_func_array($form_state['islandora_bookmark_export'][$key], $params);
  }

  /**
   * One of our '#action_submit' callbacks.
   *
   * Redirect to the given list.
   *
   * @param array $form
   *   The array representing a Drupal form.
   * @param array $form_state
   *   The array of form state.
   */
  protected function goToBookmark(&$form, &$form_state) {
    $el = $form_state['complete form']['islandora_bookmark_save_fieldset']['fieldset']['save_options'];
    $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);
    drupal_goto('islandora-bookmark/listid/' . $key);
  }

  /**
   * One of our '#action_submit' callbacks.
   *
   * Save all PIDs the search yielded to the selected list.
   *
   * @param array $form
   *   The array representing a Drupal form.
   * @param array $form_state
   *   The array of form state.
   */
  protected function saveAll(&$form, &$form_state) {
    $pids = $this->getAll($form_state);
    $this->save($pids, $form_state);
  }

  /**
   * One of our '#action_submit' callbacks.
   *
   * Save the selected PIDs the search yielded to the selected list.
   *
   * @param array $form
   *   The array representing a Drupal form.
   * @param array $form_state
   *   The array of form state.
   */
  protected function saveSelected(&$form, &$form_state) {
    $pids = $this->getSelected($form_state);
    $this->save($pids, $form_state);
  }

  /**
   * Actually handle saving PIDs to a list.
   *
   * @param array $pids
   *   An array of PIDs to save to the given list.
   * @param array $form_state
   *   The array of form state.
   */
  protected function save($pids, &$form_state) {
    $el = $form_state['complete form']['islandora_bookmark_save_fieldset']['fieldset']['save_options'];
    $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);

    // Instantiate a Bookmark so we can do some comparisons before we add
    // the selected.
    module_load_include('inc', 'islandora_bookmark', 'includes/api');
    $bookmark_object = islandora_bookmark_get_bookmark_by_number($key);

    $pids_to_add = array_diff($pids, $bookmark_object->getPids());
    $skipped_pids = array_intersect($pids, $bookmark_object->getPids());

    if (count($pids_to_add) > 0) {
      try {
        $bookmark_object->addPids($pids_to_add);

        drupal_set_message(format_plural(
          count($pids_to_add),
          'The selected object has been added to "@listname".',
          'The selected objects have been added to "@listname".',
          array(
            '@listname' => $bookmark_object->bookmarkName,
          )
        ));
      }
      catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
    }
    if (count($skipped_pids) > 0) {
      drupal_set_message(format_plural(
        count($skipped_pids),
        'The selected object (@skipped_pids) was not added as it already exists in the bookmark list "@listname".',
        'The selected objects (@skipped_pids) were not added as they already exist in the bookmark list "@listname".',
        array(
          '@skipped_pids' => implode(' ', $skipped_pids),
          '@listname' => $bookmark_object->bookmarkName,
        )
      ));
    }
  }

  /**
   * Go over any '#action_validate' functions on the clicked button.
   */
  public function formValidate(&$form, &$form_state) {
    $clicked_button = $form_state['clicked_button'];
    if (isset($clicked_button['#action_validate'])) {
      foreach ((array) $clicked_button['#action_validate'] as $validator) {
        call_user_func(array($this, $validator), $form, $form_state);
      }
    }
  }

  /**
   * Submit handler for the islandora_bookmark_results form.
   *
   * Go over any '#action_submit' functions on the clicked button.
   *
   * @param array $form
   *   The Drupal form definition.
   * @param array $form_state
   *   The Drupal form state.
   */
  public function formSubmit(&$form, &$form_state) {
    $queries = drupal_get_query_parameters();

    $form_state['redirect'] = array(request_path(), array('query' => $queries));

    $clicked_button = $form_state['clicked_button'];
    if (isset($clicked_button['#action_submit'])) {
      $params = array(&$form, &$form_state);
      foreach ((array) $clicked_button['#action_submit'] as $submit_function) {
        call_user_func_array(array($this, $submit_function), $params);
      }
    }
  }

  /**
   * Submission helper; gets the PIDs all results.
   */
  protected function getAll(&$form_state) {
    $pids = array();

    // Helper anonymous function... Just get the PID.
    $get_pid = function($result) {
      return $result['PID'];
    };

    $qp = $this->islandoraSolrQueryProcessor;
    $old_params = $qp->solrParams;
    $qp->solrParams['fl'] = 'PID';
    $qp->solrStart = 0;
    $qp->solrLimit = 10000;

    // Handle the first set separately, so we can get the total number for our
    // loop.
    $qp->executeQuery();
    $solr_results = $qp->islandoraSolrResult;
    $result_count = $solr_results['response']['numFound'];
    $object_results = $solr_results['response']['objects'];
    $pids = array_map($get_pid, $object_results);

    for ($i = $qp->solrLimit; $i < $result_count; $i += $qp->solrLimit) {
      $qp->solrStart = $i;
      $qp->executeQuery();
      $solr_results = $qp->islandoraSolrResult;
      $object_results = $solr_results['response']['objects'];

      $pids = array_merge(
        $pids,
        array_map($get_pid, $object_results)
      );
    }
    $qp->solrParams = $old_params;

    return $pids;
  }

  /**
   * Submission helper; gets the PIDs of the selected results.
   */
  protected function getSelected(&$form_state) {
    $selected = array_filter($form_state['values']['islandora_bookmark_table']);
    $functional = array_filter($selected, 'islandora_object_load');
    $broken = array_diff($selected, $functional);
    if (!empty($broken)) {
      watchdog('islandora_bookmark', 'Broken PIDs encountered: @pids', array(
        '@pids' => implode(', ', $broken),
      ), WATCHDOG_WARNING);
    }

    return $functional;
  }
}

/**
 * Form building function.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param IslandoraSolrResultsBookmark $results_class
 *   An object on which we can call buildForm() to get our form structure.
 * @param array $object_results
 *   The object results from the Islandora Solr processor created in the
 *   printResults function.
 *
 * @return array
 *   An array containing form structure.
 */
function islandora_bookmark_results_form($form, &$form_state, $results_class, $object_results) {
  return $results_class->formGet($form, $form_state, $object_results);
}

/**
 * Form validation handler.
 */
function islandora_bookmark_results_form_validate(array &$form, array &$form_state) {
  $form_state['build_info']['args'][0]->formValidate($form, $form_state);
}

/**
 * Submit handler for the islandora_bookmark_results form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_bookmark_results_form_submit(array &$form, array &$form_state) {
  $form_state['build_info']['args'][0]->formSubmit($form, $form_state);
}
