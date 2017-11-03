<?php

/**
 * @file
 * This file holds supporting code for the scholar embargo module.
 */

/**
 * This builds form for embargoing an object.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $item
 *   The Object to embargo.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_scholar_embargo_form(array $form, array &$form_state, AbstractObject $item) {
  module_load_include('inc', 'islandora_scholar_embargo', 'includes/utilities');
  $pid = $item->id;
  $options = array(
    'object' => t('Object-level embargo'),
    'datastream' => t('Datastream'),
  );
  $embargo_results = islandora_scholar_embargo_get_embargoed($item);

  // Table seems somewhat of overkill for now, but as the embargo offering
  // grows more robust this sort of formatting for the UI will make sense.
  $rows = array();
  $current_embargoes = array();
  $date = '';
  foreach ($embargo_results as $result) {
    if ($result['obj']['value'] == $item->id) {
      // Object.
      $type = t('Object');
    }
    else {
      $type = str_replace("$pid/", '', $result['obj']['value']);
    }
    $date = $result['date']['value'];
    if ($date != 'indefinite') {
      $date = new dateTime($date);
      $date = $date->format('M d, Y ');
    }
    else {
      $date = ucwords($date);
    }
    $rows[] = array(
      $type,
      $date,
    );
    $current_embargoes[] = array(
      'type' => $type,
      'date' => $date,
    );
  }
  $form_state['islandora_embargo'] = $current_embargoes;
  $embargoed = TRUE;
  if (empty($embargo_results)) {
    $embargoed = FALSE;
  }

  // We're just using the PDF datastram at this time, but we'll leave this here
  // in case we decide to expand.
  foreach ($item as $datastream) {
    $options[$datastream->id] = $datastream->label;
  }

  /* XXX:  Chop 'em down to only those we want...
   * Another hook/configuration bit? */
  $used_options = array(
    'object' => '',
    'datastream' => '',
  );
  $options = array_intersect_key($options, $used_options);

  $form = array(
    '#tree' => TRUE,
    'pid' => array(
      '#type' => 'value',
      '#value' => $pid,
    ),
  );
  if (!$embargoed) {
    $form['dsid'] = array(
      '#type' => 'radios',
      '#title' => t('Item to be embargoed'),
      '#options' => $options,
      '#default_value' => 'object',
    );
    $ds_options = islandora_scholar_embargo_get_embargoable_datastreams($item);
    $form['ds_options'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Datastreams to Embargo'),
      '#options' => $ds_options,
      '#states' => array(
        'visible' => array(
          ':input[name="dsid"]' => array('value' => 'datastream'),
        ),
      ),
    );
    // Render a separate checkbox cause #states doesn't work nicely on
    // checkboxes values.
    $form['ds_options_other'] = array(
      '#type' => 'checkbox',
      '#title' => t('Other'),
      '#states' => array(
        'visible' => array(
          ':input[name="dsid"]' => array('value' => 'datastream'),
        ),
      ),
    );
    $form['ds_other'] = array(
      '#type' => 'textarea',
      '#description' => t('Enter a space- or comma-separated list of datastreams to embargo.'),
      '#states' => array(
        'visible' => array(
          ':input[name="dsid"]' => array('value' => 'datastream'),
          ':input[name="ds_options_other"]' => array('checked' => TRUE),
        ),
      ),
    );
    $date_state = array(
      ':input[name="update_date_options"]' => array('value' => 'change_embargo_date'),
    );

    islandora_scholar_embargo_append_xacml_recursion($form, $form_state, $item, FALSE);
  }
  else {
    $form['update_options'] = array(
      '#type' => 'radios',
      '#title' => t('Embargo Options'),
      '#options' => array(
        'update_date' => t('Update embargo'),
        'lift_embargo' => t('Lift the embargo'),
      ),
      '#required' => TRUE,
    );
    $date_state = array(
      ':input[name="update_options"]' => array('value' => 'update_date'),
      ':input[name="update_date_options"]' => array('value' => 'change_embargo_date'),
    );
    islandora_scholar_embargo_append_xacml_recursion($form, $form_state, $item, TRUE);
  }
  $form['update_date_options'] = array(
    '#type' => 'radios',
    '#title' => check_plain($embargoed ? t('Update Options') : t('Embargo Options')),
    '#options' => array(
      'change_embargo_date' => $embargoed ? t('Change the embargo date') : t('Set the embargo date'),
      'indefinite_embargo' => t('Embargo indefinitely (Indefinite embargoes must be lifted manually).'),
    ),
    '#default_value' => ($date === 'Indefinite') ? 'indefinite_embargo' : 'change_embargo_date',
    '#states' => array(
      'visible' => array(
        ':input[name="update_options"]' => array('value' => 'update_date'),
      ),
    ),
  );
  $form['embargo_date'] = array(
    '#type' => 'date',
    '#title' => t('Embargo date'),
    '#description' => t('A date until which this item should be embargoed.'),
    '#default_value' => ($embargoed && $date !== 'Indefinite') ? date_parse($date) : NULL,
    '#after_build' => array('_embargo_set_year_range'),
    '#states' => array(
      'visible' => $date_state,
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 102,
  );

  $params = array(
    'header' => array(
      t('Type'),
      t('Embargo Expiry'),
    ),
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('No items currently embargoed!'),
  );
  $form['embargo_table'] = array(
    '#markup' => theme('table', $params),
    '#weight' => 0,
  );
  return $form;
}

/**
 * Customizes date range for form element.
 */
function _embargo_set_year_range($form_element, $form_values) {
  $year = date("Y");
  $form_element['year']['#options'] = drupal_map_assoc(range($year, $year + 20));
  return $form_element;
}

/**
 * Facilitates adding without first lifting all associated embargoes...
 */
function islandora_scholar_embargo_lift_associated_except($pid, $dsid = NULL, $expiry = NULL) {

  $is = ISLANDORA_SCHOLAR_EMBARGO_RELS_URI;
  $eu = ISLANDORA_SCHOLAR_EMBARGO_EXPIRY_PRED;
  $query = <<<EOQ
PREFIX is:  <$is>
SELECT ?embargoed ?expiry
WHERE {{
    ?embargoed is:$eu ?expiry ;
               <fedora-model:hasModel> ?model .
      FILTER(sameTerm(?embargoed, <info:fedora/$pid>))
  }
  UNION {
    <info:fedora/$pid> <fedora-view:disseminates> ?embargoed ;
                       <fedora-model:hasModel> ?model .
    ?embargoed is:$eu ?expiry .
  }
}
EOQ;
  $item = islandora_object_load($pid);
  $results = $item->repository->ri->sparqlQuery($query);

  foreach ((array) $results as $result) {
    $e_pid = $result['embargoed']['value'];
    $e_dsid = NULL;
    $e_exp = $result['expiry']['value'];
    if (strpos($e_pid, '/') !== FALSE) {
      // Is DSID.
      list($e_pid, $e_dsid) = explode('/', $e_pid);
    }

    // XXX: Might get into trouble if we somehow have multiple embargo
    // statements for a single datastream, due to how we remove them from the
    // XACML.
    // Do not remove statements which match our parameters.
    if ($e_pid != $pid || $e_dsid != $dsid || $e_exp != $expiry) {
      islandora_scholar_embargo_lift_embargo($e_pid, $e_dsid);
    }
  }
}

/**
 * Implements hook_validate().
 */
function islandora_scholar_embargo_form_validate(array $form, array &$form_state) {
  $embargoed = !empty($form_state['islandora_embargo']);
  if (($embargoed && isset($form_state['values']['update_options']) && $form_state['values']['update_options'] !== 'lift_embargo') || (!$embargoed)) {
    if ($form_state['values']['update_date_options'] === 'change_embargo_date') {
      $date = $form_state['values']['embargo_date'];
      $parsed_date = format_string('@year-@month-@day', array(
        '@year' => $form_state['values']['embargo_date']['year'],
        '@month' => $form_state['values']['embargo_date']['month'],
        '@day' => $form_state['values']['embargo_date']['day'],
      ));
      $time_now = time();
      $parsed = strtotime($parsed_date);
      if ($embargoed) {
        // This will need to be updated in the future to handle multiple
        // embargoes on a single object.
        foreach ($form_state['islandora_embargo'] as $embargo_vals) {
          $embargo_date = $embargo_vals['date'];
          break;
        }
        if (strtotime($embargo_date) === $parsed) {
          form_set_error('embargo_date', t('The selected date matches the currently embargoed date!'));
        }
      }
      if ($parsed <= $time_now) {
        form_set_error('embargo_date', t('You cannot specify a date in the past!'));
      }
    }
    elseif ($form_state['values']['update_date_options'] === 'indefinite_embargo') {
      // This will need to be updated in the future to handle multiple embargoes
      // on a single object.
      $embargo_date = '';
      foreach ($form_state['islandora_embargo'] as $embargo_vals) {
        $embargo_date = $embargo_vals['date'];
        break;
      }
      if ($embargo_date === 'Indefinite') {
        form_set_error('update_date_options', t("The embargo's duration is already set indefinitely!"));
      }
    }
    // Make sure the user has selected at least one datastream to embargo if
    // they are embargoing datastreams.
    if (isset($form_state['values']['dsid']) && $form_state['values']['dsid'] == 'datastream') {
      $selected_options = array_filter($form_state['values']['ds_options']);
      if (empty($selected_options) && empty($form_state['values']['ds_other'])) {
        form_error($form['ds_options'], t('Please select at least one datastream to embargo!'));
      }
    }
  }
  if (isset($form_state['values']['recurse']) && $form_state['values']['recurse'] && $form_state['values']['recurse_options'] == 'default') {
    form_error($form['recurse_options'], t('Please select an option to apply recursively.'));
  }
}

/**
 * Implements hook_submit().
 */
function islandora_scholar_embargo_form_submit(array $form, array &$form_state) {
  $embargoed = !empty($form_state['islandora_embargo']);
  $pid = $form_state['values']['pid'];
  $object = islandora_object_load($pid);
  // We are lifting an embargo from an object that already has one.
  if (isset($form_state['values']['update_options']) && $form_state['values']['update_options'] === 'lift_embargo') {
    $dsids = array();
    // This will have to be re-defined if and when we decide to allow multiple
    // separate embargoing on an object likely by using this function:
    // islandora_scholar_embargo_lift_associated_except(). For now we can only
    // have one embargo.
    foreach ($form_state['islandora_embargo'] as $embargo_vals) {
      $type = $embargo_vals['type'];
      if ($type !== 'Object') {
        $dsids[] = $type;
      }
      else {
        $dsids = NULL;
      }
    }
    // Batch down and down and down.
    if (isset($form_state['values']['recurse']) && $form_state['values']['recurse']) {
      $option = $form_state['values']['recurse_options'];
      $query_array = $form_state['query_choices'][$option];
      $batch = array(
        'title' => t('Lifting Embargoes'),
        'progress_message' => t('Please wait if many objects are being updated this could take a few minutes.'),
        'operations' => array(
          array(
            'islandora_scholar_embargo_lift_batch_function',
            array($pid, $dsids, $query_array),
          ),
        ),
        'finished' => 'islandora_scholar_embargo_lift_batch_finished',
        'file' => drupal_get_path('module', 'islandora_scholar_embargo') . '/includes/batch.inc',
      );
      batch_set($batch);
    }
    islandora_scholar_embargo_lift_embargo($pid, $dsids);
    drupal_set_message(t('The embargo on @label (@pid) has been lifted!', array(
      '@label' => $object->label,
      '@pid' => $pid,
    )));
  }
  elseif (isset($form_state['values']['update_date_options'])) {
    // Embargoed indefinitely.
    if ($form_state['values']['update_date_options'] === 'indefinite_embargo') {
      $end = 'indefinite';
    }
    else {
      $time = $form_state['values']['embargo_date'];
      $date_string = $time['year'] . '-' . $time['month'] . '-' . $time['day'];
      $end = gmdate("Y-m-d\TH:i:s\Z", strtotime($date_string));
    }

    // Object is currently embargoed and we are updating things.
    if ($embargoed) {
      $dsids = array();
      foreach ($form_state['islandora_embargo'] as $embargo_vals) {
        $type = $embargo_vals['type'];
        if ($embargo_vals['type'] == 'Object') {
          $dsids = NULL;
        }
        else {
          $dsids[] = $type;
        }
      }
    }
    // New embargo entry.
    else {
      if ($form_state['values']['dsid'] === 'object') {
        $dsids = NULL;
      }
      else {
        $dsid_options = array_filter($form_state['values']['ds_options']);
        $other_options = array();
        if ($form_state['values']['ds_options_other']) {
          $other_options = drupal_map_assoc(array_values(array_filter(preg_split('/[,|\s]/', trim($form_state['values']['ds_other'])))));
        }
        $dsids = array_unique(array_merge($dsid_options, $other_options));
      }
    }
    // Currently we just overwrite what's already in place. In the future, for
    // multiple embargoes on a single object, this will have to be adjusted.

    // If we're recursing do those objects in a batch.
    if (isset($form_state['values']['recurse']) && $form_state['values']['recurse'] && $form_state['values']['recurse_options'] != 'newchildren') {
      $option = $form_state['values']['recurse_options'];
      $query_array = $form_state['query_choices'][$option];
      $batch = array(
        'title' => t('Updating Embargoes'),
        'progress_message' => t('Please wait if many objects are being updated this could take a few minutes.'),
        'operations' => array(
          array(
            'islandora_scholar_embargo_apply_batch_function',
            array($pid, $dsids, $end, $query_array),
          ),
        ),
        'finished' => 'islandora_scholar_embargo_apply_batch_finished',
        'file' => drupal_get_path('module', 'islandora_scholar_embargo') . '/includes/batch.inc',
      );
      batch_set($batch);
    }
    islandora_scholar_embargo_set_embargo($pid, $dsids, $end);
    islandora_scholar_embargo_set_display_message($pid, $dsids, $end);
  }
}

/**
 * Retrieve all embargoed items from an object.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 *
 * @return array
 *   An array containing the list of embargoed items on an object
 */
function islandora_scholar_embargo_get_embargoed($object) {
  $current_embargo = <<<EOQ
PREFIX is: <info:islandora/islandora-system:def/scholar#>
SELECT ?obj ?date
FROM <#ri>
WHERE {
  {
     ?obj is:embargo-until ?date .
     FILTER(REGEX(STR(?obj),'{$object->id}$'))
  }
  UNION {
    ?obj is:embargo-until ?date .
    FILTER(REGEX(STR(?obj),'{$object->id}/'))
  }
}
EOQ;
  $embargo_results = $object->repository->ri->sparqlQuery($current_embargo, 'unlimited', '0');
  return $embargo_results;
}

/**
 * Retrieve all embargoed items.
 *
 * @param bool $count
 *   Whether or not to just return the total count.
 *
 * @return array
 *   An array containing the list of embargoed items.
 */
function islandora_scholar_embargo_get_all_embargoed($count = FALSE) {
  $current_embargo = <<<EOQ
PREFIX is: <info:islandora/islandora-system:def/scholar#>
SELECT ?obj ?date
FROM <#ri>
WHERE {
  ?obj is:embargo-until ?date
}
EOQ;
  if ($count) {
    $current_embargo .= <<<EOQ
ORDER BY ?obj
EOQ;
  }
  $connection = islandora_get_tuque_connection();
  $ri = $connection->repository->ri;
  return $count ? $ri->countQuery($current_embargo, 'sparql') : $ri->sparqlQuery($current_embargo, 'unlimited', '0');
}

/**
 * Gets a subset of all embargoed items.
 *
 * @param array $slice_params
 *   An associative array containing:
 *   'offset_date': an ISO8601 datetime to filter items before
 *   'offset_pid': a PID to filter on when a date match is found.
 * @param int $limit
 *   The number of items to return in the subset.
 *
 * @return array
 *   A SPARQL query results array containing a subset of embargoed items,
 *   including 'obj' as the object ID, 'expiry' as the expiry date, and 'date'
 *   as the item's created or last modified date.
 */
function islandora_scholar_embargo_get_embargoed_items_subset($slice_params = array(), $limit = 10) {
  // Build the query.
  $current_embargo = <<<EOQ
PREFIX is: <info:islandora/islandora-system:def/scholar#>
PREFIX xs: <xml-schema:>
SELECT DISTINCT ?obj ?expiry ?date
FROM <#ri>
WHERE {
  ?obj is:embargo-until ?expiry ;
  { ?obj <fedora-model:createdDate> ?date } UNION
  { ?obj <fedora-view:lastModifiedDate> ?date ;
         <fedora-view:disseminationType> ?dsid
  }
EOQ;
  // Offset if asked.
  if ($slice_params) {
    $current_embargo .= "FILTER(?date > '{$slice_params['offset_date']}'^^xs:dateTime || (?date = '{$slice_params['offset_date']}'^^xs:dateTime && xs:string(?obj) > xs:string('info:fedora/{$slice_params['offset_pid']}')))";
  }
  $current_embargo .= <<<EOQ
}
ORDER BY ASC(?date) ASC(?obj)
EOQ;

  // Use the query.
  $connection = islandora_get_tuque_connection();
  return $connection->repository->ri->sparqlQuery($current_embargo, $limit);
}

/**
 * Displays a drupal_set_message when setting an embargo through the form.
 *
 * @param string $pid
 *   Pid of the object being embargoed;
 * @param string|NULL $dsid
 *   The DSID of the datastream we are embargoing, or NULL if it's the object.
 * @param string $end
 *   Either "indefinite" or a valid date string.
 */
function islandora_scholar_embargo_set_display_message($pid, $dsid, $end) {
  $object = islandora_object_load($pid);
  if (is_null($dsid)) {
    if ($end === 'indefinite') {
      drupal_set_message(t('A object-level embargo has been set on @label (@pid) indefinitely.',
        array(
          '@label' => $object->label,
          '@pid' => $pid,
        )
      ));
    }
    else {
      $date = new dateTime($end);
      $date = $date->format('M d, Y');
      drupal_set_message(t('A object-level embargo has been set on @label (@pid) until @duration.',
        array(
          '@label' => $object->label,
          '@pid' => $pid,
          '@duration' => $date,
        )
      ));
    }
  }
  else {
    if ($end === 'indefinite') {
      drupal_set_message(t('A @dsid datastream embargo has been set on @label (@pid) indefinitely.',
        array(
          '@dsid' => implode(', ', $dsid),
          '@label' => $object->label,
          '@pid' => $pid,
        )
      ));
    }
    else {
      $date = new dateTime($end);
      $date = $date->format('M d, Y');
      drupal_set_message(t('A @dsid datastream embargo has been set on @label (@pid) until @duration.',
        array(
          '@dsid' => implode(', ', $dsid),
          '@label' => $object->label,
          '@pid' => $pid,
          '@duration' => $date,
        )
      ));
    }
  }
}

/**
 * Adds the XACML recursion option to the embargo form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param AbstractObject $item
 *   An AbstractObject representing an object within Fedora to be embargoed.
 * @param bool $lift
 *   Bool denoting whether the action is to lift or to modify/apply the embargo.
 */
function islandora_scholar_embargo_append_xacml_recursion(&$form, &$form_state, AbstractObject $item, $lift = FALSE) {
  // XXX: Lifted from XACML, maybe refactor out at some point?
  $query_choices = array();
  foreach (islandora_build_hook_list('islandora_xacml_editor_child_query', $item->models) as $hook) {
    $temp = module_invoke_all($hook, $item);
    if (!empty($temp)) {
      $query_choices = array_merge_recursive($query_choices, $temp);
      // We are doing this to handle the "flat" use case where we are not
      // recursing more than one level. This is a unique case so this is the
      // easiest way to handle it.
      if (isset($query_choices['all_children'])) {
        $query_choices['flat_collection'] = $query_choices['all_children'];
        $query_choices['flat_collection']['description'] = t('All immediate children of the collection (shallow traversal)');
        $query_choices['flat_collection']['restricted_cmodels'] = array('islandora:collectionCModel');
      }
    }
  }
  if (!empty($query_choices)) {
    $update_options = array('default' => t('-- Select --'));
    foreach ($query_choices as $key => &$query) {
      // XXX: This should be changed if/when embargo supports inheritance on
      // ingest.
      if ($key == 'all_children') {
        $query['description'] = t('All children of this collection and collections within this collection.');
      }
      elseif ($key == 'book_pages') {
        $query['description'] = t('All pages of this book.');
      }
      elseif ($key == 'newspaper_pages') {
        $query['description'] = t('All pages of this issue.');
      }
      elseif ($key == 'newspaper_issues') {
        $query['description'] = t('All newspaper issues and their pages.');
      }
      $update_options[$key] = $query['description'];
      $form_state['query_choices'][$key] = $query;
    }
    $form['recurse'] = array(
      '#type' => 'checkbox',
      '#title' => t('!action the embargo recursively?', array('!action' => $lift ? t('Modify') : t('Apply'))),
      '#weight' => 100,
    );
    $form['recurse_options'] = array(
      '#type' => 'select',
      '#title' => t('What items would you like to !action?', array('!action' => $lift ? t('modify the embargo on') : t('apply this embargo to'))),
      '#description' => t('<strong>Note:</strong> This will only !action the embargo on all items with content models configured to accept them.', array('!action' => $lift ? t('modify') : t('apply'))),
      '#default_value' => $lift ? 'default' : 'newchildren',
      '#options' => $update_options,
      '#states' => array(
        'visible' => array(
          ':input[name="recurse"]' => array('checked' => TRUE),
        ),
      ),
      '#weight' => 101,
    );
  }
}
