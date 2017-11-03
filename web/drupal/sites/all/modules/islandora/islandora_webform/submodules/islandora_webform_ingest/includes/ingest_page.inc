<?php


/**
 * Callback to build islandora webform ingest submission page.
 *
 * @param array $form
 *   form
 * @param array $form_state
 *   form_state
 * @param int $sid
 *   the submission ID
 *
 * @return array
 *   A renderable Drupal form array
 *
 * @throws Exception
 */
function iwi_preview_ingest_submission_form($form, &$form_state, $sid) {
  module_load_include('inc', 'webform', 'includes/webform.submissions');
  module_load_include('inc', 'islandora_webform', 'includes/utilities');
  module_load_include('inc', 'islandora_webform_ingest', 'includes/utilities');
  drupal_add_css(drupal_get_path('module', 'islandora_webform_ingest') . '/css/islandora_webform_ingest.css');
  form_load_include($form_state, 'inc', 'islandora_webform_ingest', '/includes/ingest_page');
  form_load_include($form_state, 'inc', 'islandora', '/includes/ingest.form');
  form_load_include($form_state, 'inc', 'islandora', '/includes/utilities');
  // Get the submission data.
  $submissions = webform_get_submissions(array('sid' => $sid));
  $submission = array_shift($submissions);
  // Get the webform ingest configuration.
  $iwi_settings = iwi_webform_ingest_settings($submission->nid);
  // Get the submission component mappings.
  $mappings = iwi_get_saved_component_mappings($submission->nid);
  if (!count($mappings)) {
    return $form;
  }


  // If cmodel is defined in the webform ingest settings, we want to create
  // a new object related to the original object.
  if ($iwi_settings->cmodel) {
    if ($iwi_settings->namespace && $iwi_settings->relation) {
      // Setting $object to NULL causes a new object of this cmodel
      // to be created from scratch.
      $object = NULL;
      $ingest_content_model = islandora_object_load($iwi_settings->cmodel);
    }
    else {
      // We shouldn't ever see this condition, but just in case...
      $url = 'node/' . $submission->nid . '/webform/islandora';
      drupal_set_message(t('Invalid webform ingestion settings for creating a new object. Please check your settings at !link', array('!link' => l($url, $url))), 'error');
      return $form;
    }
  }
  // If cmodel is not defined in the webform ingest settings, we are modifying
  // the original object.
  else {
    $object = $submission->islandora_object;
    $ingest_content_model = islandora_object_load(iwi_webform_content_model($submission->nid));
  }


  $webform = node_load($submission->nid);

  // Get the content model datastreams.
  // It's ok to use NULL mimetype filter because our mappings below
  // control which datastreams we will actually show.
  $datastreams = iwi_ingestible_datastreams($ingest_content_model, NULL);

  // Get row label for submission summary.
  if ($object) {
    $submission_object_text = l($object->label, 'islandora/object/' . $object->id);
  }
  else {
    $submission_object_text = t('New @cmodel object related by @relation to "@object"',
      array(
        '@cmodel' => $ingest_content_model->label,
        '@relation' => $iwi_settings->relation,
        '@object' => $submission->islandora_object->label,
      ));
  }

  $submission_info = array(
    $submission_object_text,
    l($webform->title, 'node/' . $webform->nid),
    l($submission->is_draft ? t('@sid (draft)', array('@sid' => $sid)) : $sid, "node/$webform->nid/submission/$sid"),
    format_date($submission->submitted, 'short'),
    theme('username', array('account' => $submission)),
  );
  $submission_header = array(
    t('Object'),
    t('Webform'),
    t('Submission'),
    t('Submitted on'),
    t('Submitted by'),
  );
  // Put the submission info into non-renderable storage. We can't render it
  // inside this form since it may itself contain a form. The calling function
  // should pull it out and render it separately.
  $form['#submission_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission info'),
    '#attributes' => array (
      'class' => array(
        'collapsible',
        'collapsed',
      )
    ),
    '#attached' => array(
      'library' => array(
        0 => array('system', 'drupal.collapse'),
      ),
    ),
    '#weight' => -1,
    'description' => array(
      '#markup' => theme('table', array(
        'rows' => array($submission_info),
        'header' => $submission_header,
      )),
    ),
    'submission' => webform_submission_render($webform, $submission, NULL, NULL),
  );
  $islandora_webform_values = array();
  $used_associations = array();

  // Loop through the component mappings, adding submitted values to
  // appropriate datastream sub-forms
  foreach ($mappings as $mapping) {
    // Check if we have submitted data for this component, if not, skip it
    // entirely.
    if (isset($submission->data[$mapping->cid][0])) {
      $submitted_value = $submission->data[$mapping->cid][0];
      $dsid = $mapping->data['datastream_id'];
      $dsindex = $mapping->data['datastream'];
      $association = $datastreams[$dsindex];
      //Only store datastream associations for fields we actually have
      $used_associations[$dsindex]=$association;
      // Store all our submited values.
      $islandora_webform_values[$dsid]['association'] = $association;
      $islandora_webform_values[$dsid]['data_values'][] = array(
        'path' => $mapping->data['field'],
        'input_value' => array(
          '#default_value' => $submitted_value,
          '#attributes' => array(
            'class' => array(
              'submitted-value',
            ),
          ),
        ),
        'mode' => $mapping->data['mode'],
      );
      $islandora_webform_values[$dsid]['component'] = $webform->webform['components'][$mapping->cid];
    }
  }
  //set our mapped dsid and associations
  $form_state['datastreams'] =  $used_associations;
  // We need to preserve the original $form_state to pass it to
  // xml_form_builder_get_form.
  // On the first build pass.
  $form_state_original = $form_state;

  
  foreach ($islandora_webform_values as $dsid => $dsid_values) {
    // If datastream has an edit form, add that to the form.
    if (isset($dsid_values['association']['form_name'])) {
      $new_form_name = $dsid_values['association']['form_name'];
      $xml = NULL;
      if (isset($object[$dsid])) {
        $xml = $object[$dsid]->content;
      }
      $form2 = NULL;
      // Deal with ajax. We are going to simulate a multistep form using only
      // one dsid at a time. Dirty!
      if (isset($form_state['rebuild']) && $form_state['rebuild'] == TRUE) {
        $form_state_new['values'] = $form_state['values'][$dsid];
        $form_state_new['storage']['objective_form_storage'] = $form_state['storage']['islandora_webform'][$dsid];
        $form_state_new['triggering_element'] = $form_state['triggering_element'];
        $form2 = xml_form_builder_get_form(array(), $form_state_new, $new_form_name, $xml);
      }
      // End deal with ajax.
      else {
        $form_state_new = $form_state_original;
        $form2 = xml_form_builder_get_form($form, $form_state_new, $new_form_name, $xml);
      }

      // Only do this once. If ajax is called, then the form is rebuilt and
      // we end re-adding new elements. So check if form was rebuild. If yes,
      // don't do anything.
      if (!isset($form_state['rebuild']) || $form_state['rebuild'] == FALSE) {
        // Loop through the submitted values, setting them into $form2.
        foreach ($dsid_values['data_values'] as $input) {
          iwi_set_form_element_byhash($form2, $form_state_new, $input['path'], $input['input_value'], $input['mode']);
        }

        // Permit other modules to modify the form.
        drupal_alter('iwi_set_form', $form2, $form_state_new, $submission);
      }

      // Build our own local and multiple objective_form_storage for every dsid.
      $form_state['storage']['islandora_webform'][$dsid] = $form_state_new['storage']['objective_form_storage'];

      $new_form = array(
        '#title' => isset($dsid_values['association']['form_name']) ? $dsid_values['association']['form_name'] : $dsid,
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#collapsible' => FALSE,
      );
      foreach (element_children($form2) as $childkey) {
        $new_form[$childkey] = $form2[$childkey];
      }
      // Recover the #hash.
      $new_form['#hash'] = $form2['#hash'];

      $form[$dsid] = $new_form;
    }
    else {
      // There is no edit form, display using webform component theming.
      $new_form = array(
        '#title' => $dsid,
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#collapsible' => FALSE,
      );

      // Display the component using webform defaults.
      $data = $submission->data[$dsid_values['component']['cid']];
      $new_form['component'] = webform_component_invoke($dsid_values['component']['type'], 'display', $dsid_values['component'], $data);

      if (!empty($new_form['component']['#value']->fid)) {
        // Include the file id in a hidden field.
        $new_form['fid'] = array(
          '#type' => 'hidden',
          '#value' => $new_form['component']['#value']->fid,
        );
      }

      $form[$dsid] = $new_form;
    }
  }

  // Add the submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ingest this Submission'),
    '#name' => 'submit',
    '#submit' => array('iwi_ingest_page_form_submit'),
    '#weight' => 100,
  );

  // Store some stuff for submit callback.
  $form['#submission'] = $submission;
  $form['#iwi_settings'] = $iwi_settings;
  $form['#ingest_cmodel'] = $ingest_content_model;

  return $form;

}

/**
 * Form-submit callback for islandora webform ingest submission page.
 *
 * @param array $form
 *   the form
 * @param array $form_state
 *   the form_state
 */
function iwi_ingest_page_form_submit($form, &$form_state) {
  if (!empty($form_state['datastreams']) && !empty($form['#submission']->islandora_object)) {
    $new_object = ($form['#iwi_settings']->cmodel) ? TRUE : FALSE;
    // Keep track of datastream count to avoid ingesting an empty object
    // to the repo.
    $datastream_count = 0;
    if ($new_object) {
      $new_pid = $form['#submission']->islandora_object->repository->getNextIdentifier($form['#iwi_settings']->namespace, variable_get('islandora_basic_collection_generate_uuid', FALSE));
      $relationships = array('relationship' => $form['#iwi_settings']->relation, 'pid' => $form['#submission']->islandora_object->id);
      $label = "New Object";
      $object = islandora_prepare_new_object($new_pid, $label, array(), array($form['#iwi_settings']->cmodel), array($relationships));
      //Now add our control predicates, so we can keep track of this object
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, ISLANDORA_WEBFORM_ISIW_PREDICATE, "true", RELS_TYPE_PLAIN_LITERAL);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, ISLANDORA_WEBFORM_HASIW_PREDICATE, "{$form['#submission']->nid}", RELS_TYPE_PLAIN_LITERAL);
      // Don't add the Object to the REPO yet, let's do this at the end, if
      // $datastream_count > 0
    }
    else {
      // Reload object to avoid MD5 checksum problems.
      $object = islandora_object_load($form['#submission']->islandora_object->id);
    }

    foreach ($form_state['datastreams'] as $association) {
      $dsid = $association['dsid'];

      // Get values for non-editable components.
      if(empty($form_state['values'][$dsid]) && !empty($form[$dsid]['component']['#value'])) {
        $form_state['values'][$dsid] = $form[$dsid]['component']['#value'];
        if(is_object($form_state['values'][$dsid])) {
          $form_state['values'][$dsid] = (array) $form_state['values'][$dsid];
        }
      }

      if (isset($form_state['values'][$dsid]) && isset($form[$dsid])) {

        // Importing via xml form?
        if (isset($association['form_name'])) {

          // Pull out the sub form from the aggregate form.
          $sub_form = $form[$dsid];

          // Reload the original sub_form $form_state
          $new_form_state['storage']['objective_form_storage'] = $form_state['storage']['islandora_webform'][$dsid];


          // Load the input/values to our new form state.
          $new_form_state['input'] = $form_state['input'][$dsid];
          $new_form_state['values'] = $form_state['values'][$dsid];
          // Create XMLForm object from new form state.
          $xml_form = new XMLForm($new_form_state);

          // Process the form.
          $document = $xml_form->submit($sub_form, $new_form_state);

          // Get updated object label.
          $label = NULL;
          if (is_array($association['title_field'])) {
            $title_field = drupal_array_get_nested_value($sub_form, $association['title_field']);
            $label = $title_field['#value'];
          }

          // Update the islandora object with this datastream.
          // xml_form_builder_update_object returns
          // (array($association['dsid'] => $created, 'DC' => $created_dc);)
          $created = xml_form_builder_update_object($object, $association, $document, $label);
          if ($created[$dsid]) {
            $datastream_count++;
          }
          if ($created['DC']) {
            $datastream_count++;
          }
        }
        // Done handling form-based datastream.

        // Handle other datastream mimetypes.
        else {
          $value = $form_state['values'][$dsid];
          // Rendered webform components put the value inside an array with the
          // structure `array('component' => [value]);`
          if(is_array($value)) {
            $value = reset($value);
          }
          $value_type = gettype($value);
          // Handle text datastreams.
          $text_value_types = array('integer', 'double', 'string');
          if ($association['mime'] == 'text/plain' && in_array($value_type, $text_value_types)) {
            // Get the datastream object that we'll be ingesting into.
            // Create it if not preexisting.
            if (empty($object[$dsid])) {
              $datastream = $object->constructDatastream($dsid, 'M');
              $datastream->mimeType = $association['mime'];
              $object->ingestDatastream($datastream);
            }
            else {
              $datastream = $object[$dsid];
            }
            $datastream->setContentFromString($value);
            $datastream_count++;
          }
          // Handle file ingestion.
          elseif (!empty($form_state['values'][$dsid]['fid'])) {
            // Handle file.
            if ($file = file_load($form_state['values'][$dsid]['fid'])) {
              // Get permissible extensions for this datastream.
              module_load_include('inc', 'islandora', 'includes/mimetype.utils');
              $extensions = islandora_get_extensions_for_datastream($object, $dsid);
              $ext = pathinfo($file->filename, PATHINFO_EXTENSION);
              // If extension is valid for this datastream, go ahead and ingest.
              if (in_array($ext, $extensions)) {
                if (empty($object[$dsid])) {
                  $datastream = $object->constructDatastream($dsid, 'M');
                  $object->ingestDatastream($datastream);
                }
                else {
                  $datastream = $object[$dsid];
                }
                $datastream->label = $file->filename;
                $datastream->mimetype = $file->filemime;
                $file_path = drupal_realpath($file->uri);
                $datastream->setContentFromFile($file_path);
                // Assuming for now everything wen't fine.
                $datastream_count++;
              }
              else {
                drupal_set_message(t('%type is not a supported file type for ingest to %dsid', array(
                  '%type' => $ext,
                  '%dsid' => $dsid,
                ), 'error'));
              }
            }
            else {
              drupal_set_message(t('File could not be found', array(), 'error'));
            }
          }
          // Respond to invalid types.
          else {
            drupal_set_message(t('%mime is not a supported mime type for ingest', array('%mime' => $association['mime'])), 'error');
          }
        }
      }
    }

    $url = 'islandora/object/' . $object->id;
    $link = l($url, $url);
    if (($new_object) && ($datastream_count > 0)) {
      // Finally, add the Object to the repo.
      islandora_add_object($object);

      // Report new object created.
      drupal_set_message(t('New @cmodel object with PID @pid was ingested: !link', array(
        '@cmodel' => $form['#ingest_cmodel']->label,
        '@pid' => $object->id,
        '!link' => $link,
      )));
    }
    else {
      // Report original object modified.
      drupal_set_message(t('@cmodel object with PID @pid was updated: !link', array(
        '@cmodel' => $form['#ingest_cmodel']->label,
        '@pid' => $object->id,
        '!link' => $link,
      )));
    }

    // Save a record of this ingestion.
    iwi_record_ingested($form['#submission'], $object);

    // Let other modules respond to the ingestion event.
    module_invoke_all('islandora_webform_submission_ingested', $form['#submission'], $object);
  }
}

/**
 * Save a record of an islandora webform ingestion.
 *
 * @param object $submission
 *   A webform submission object
 * @param mixed $object
 *   The repository object
 *
 * @throws Exception
 */
function iwi_record_ingested($submission, $object) {
  if (in_array(get_class($object), array('IslandoraNewFedoraObject', 'IslandoraFedoraObject'))) {
    $fields = array(
      'nid' => $submission->nid,
      'sid' => $submission->sid,
      'pid' => $object->id,
      'date' => REQUEST_TIME,
      'data' => serialize($submission->data),
    );

    db_insert('islandora_webform_ingestions')->fields($fields)->execute();

  }
  else {
    watchdog('islandora_webform_ingest', 'Could not record object ingestion because second argument was not correct class', array(), WATCHDOG_ERROR);
  }
}