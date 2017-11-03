<?php

/**
 * Get a list of datastream identifiers that can be ingested for this object.
 *
 * This function gets used in two contexts:
 * 1 - On the component mapping configuration form, where mimetype filtering is
 *     needed.
 * 2 - On the ingest submission page, where mimetype filtering is unnecessary.
 *
 * @param FedoraObject $object
 *   the object
 * @param array $mimetypes
 *   Used to filter datastreams by mimetype:
 *   If NULL, do not filter mimetype
 *   If string, filter mimetype by this string
 *   If array, filter mimetype by strings in the array
 *
 * @return array
 *   An array of the ingestible datastream objects
 */
function iwi_ingestible_datastreams(FedoraObject $object, $mimetypes = array('text/plain')) {

  if (!empty($object['DS-COMPOSITE-MODEL'])) {

    module_load_include('inc', 'xml_form_builder', 'includes/associations');
    module_load_include('inc', 'islandora_webform_ingest', '/includes/xml');


    $mimetypes = $mimetypes ? (array) $mimetypes : $mimetypes;
    $results = array();

    // Array for keeping track of which DSIDs we have already.
    $ingestible_dsids = array();

    // First, get available xml forms.
    // Xml forms can only accept text content.
    // If $mimetypes includes "text/plain", we can use xml forms.
    if (!$mimetypes || in_array('text/plain', $mimetypes)) {
      // Get the xml form associations and put them into the select list.
      $associations = xml_form_builder_get_associations(array(), array($object->id));
      if (count($associations) > 0) {
        foreach ($associations as $association) {
          $association['mime'] = 'text/xml';
          // Uniquely identify by dsid and form name.
          $results[$association['dsid'] . ' (' . $association['form_name'] . ')'] = $association;
          $ingestible_dsids[$association['dsid']] = $association['dsid'];
        }
      }
    }

    // Now get other datastreams and decide whether or not to add them also.
    // Run the xslt transform on the 'DS-COMPOSITE-MODEL' xml to get a list of
    // the datastreams that this content model uses.
    $info = array(
      'xsl' => drupal_get_path('module', 'islandora_webform_ingest') . '/includes/cmdatastreams.xsl',
      'input' => $object['DS-COMPOSITE-MODEL']->content,
    );
    $datastreams = (array) json_decode(islandora_webform_run_xsl_transform($info));

    foreach ($datastreams as $ds => $dsmime) {
      if (empty($ingestible_dsids[$ds])) {
        // Add ingestible datastreams from other mimetypes (e.g. text/plain)
        // If filtering by mimetype, only add datastreams that match a filter.
        if ($mimetypes) {
          foreach ($mimetypes as $type) {
            if (strpos($dsmime, $type) !== FALSE) {
              $results[$ds . ' (' . $dsmime . ')'] = array(
                'dsid' => $ds,
                'mime' => $dsmime,
              );
              $ingestible_dsids[$ds] = $ds;
              // Only map the datastream once!
              break;
            }
          }
        }
        // We aren't filtering mimetype, just add the datastream to the results.
        else {
          $results[$ds . ' (' . $dsmime . ')'] = array(
            'dsid' => $ds,
            'mime' => $dsmime,
          );
          $ingestible_dsids[$ds] = $ds;
        }
      }
    }

    // Support altering the ingestible datastreams list.
    drupal_alter('iwi_ingestible_datastreams', $results, $object, $mimetypes);

    return $results;
  }
}

/**
 * Find eligible datastream mimetypes for a given component type.
 *
 * @param array $component
 *   the webform component
 *
 * @return array
 *   an array of mimetypes that this component could be mapped to
 */
function iwi_datastream_mime_from_component($component) {

  // File components.
  $types = iwi_file_extensions_from_component($component);
  if (count($types)) {
    return iwi_get_mimetypes_from_extensions($types);
  }

  // For other component types, just use this hard-coded mapping.
  $map = array(
    'datepicker' => array('text/plain'),
    'date' => array('text/plain'),
    'email' => array('text/plain'),
    'hidden' => array('text/plain'),
    'number' => array('text/plain'),
    'select' => array('text/plain'),
    'textarea' => array('text/plain'),
    'html_textarea' => array('text/html'),
    'textfield' => array('text/plain'),
    'time' => array('text/plain'),
    'tag' => array('text/plain'),
  );
  return isset($map[$component['type']]) ? $map[$component['type']] : array('application/octet-stream');
}

/**
 * Utility function to get file extension filters defined for file component.
 *
 * @param array $component
 *   a webform component
 *
 * @return array
 *   an array of file extensions which may be empty
 */
function iwi_file_extensions_from_component($component) {
  $types = array();
  if (!empty($component['extra']['filtering']['addextensions'])) {
    $types = array_map('trim', explode(',', $component['extra']['filtering']['addextensions']));
  }
  if (!empty($component['extra']['filtering']['types'])) {
    $types = array_merge($types, $component['extra']['filtering']['types']);
  }

  return $types;
}

/**
 * Convert form array and into a list of ingestible fields and field types.
 *
 * Fieldsets are returned as arrays of fields recursively.
 *
 * @param array $input_form
 *   a drupal form
 *
 * @return array|bool
 *   List of ingestible fields. Fieldsets are returned as arrays of
 *   fields recursively
 */
function iwi_recursive_form_parsing(array $input_form) {

  if (!is_array($input_form)) {
    return FALSE;
  }
  $visible_children = array();

  foreach (element_children($input_form) as $key) {
    $child = $input_form[$key];

    // Skip un-accessible children.
    if (isset($child['#access']) && !$child['#access']) {
      continue;
    }

    // Skip value and hidden elements, since they are not rendered.
    if (isset($child['#type']) && in_array($child['#type'], array(
        'value',
        'hidden',
      ))
    ) {
      continue;
    }

    // If the child has children, recursively search in it.
    $child_children = element_children($child);

    if ((is_array($child)) && (!empty($child)) && count($child_children) > 0) {
      $temp = iwi_recursive_form_parsing($child);
      if ($temp) {
        $visible_children[$key] = $temp;
      }
      continue;
    }
    if (!empty($child['#actions']['update'])) {
      $visible_children[$key] = $child['#type'];
    }

  }
  if (!empty($visible_children)) {
    return $visible_children;
  }
  else {
    return FALSE;
  }
}

/**
 * Get an array of field paths for a given form.
 *
 * @param array $form
 *   form
 *
 * @return array
 *   a list of field paths
 */
function iwi_form_field_paths($form) {
  $data = iwi_recursive_form_parsing($form);
  $pathstack = array();
  $paths = array();
  $delimiter = ':';
  iwi_path_stack($pathstack, $paths, $data, $delimiter);

  return $paths;
}

/**
 * Recursive function builds field paths for nested fields from form data.
 *
 * @param array $pathstack
 *   An array passed by reference used to hold the working path stack
 * @param array $paths
 *   An array passed by reference that is populated with the paths
 * @param array $data
 *   The form data
 * @param string $delimiter
 *   What we're inserting in between field names in the path
 */
function iwi_path_stack(&$pathstack, &$paths, $data, $delimiter = ':') {
  foreach ($data as $rowname => $rowdata) {
    array_push($pathstack, $rowname);
    if (is_array($rowdata)) {
      iwi_path_stack($pathstack, $paths, $rowdata, $delimiter);
    }
    else {
      $mimes = iwi_datastream_mime_from_component(array('type' => $rowdata));
      if (count($mimes)) {
        $paths[] = array(
          'field path' => implode($delimiter, $pathstack),
          'mimes' => $mimes,
        );
      }
    }
    array_pop($pathstack);
  }
}

/**
 * Get the islandora_webform_ingestion records for a submission.
 *
 * @param int $sid
 *   The webform submission id
 *
 * @return mixed
 *   An array of one or more standard objects representing individual
 *   database rows or NULL for no result.
 */
function iwi_get_ingestion_info($sid) {
  return db_select('islandora_webform_ingestions', 'i')
    ->fields('i')
    ->condition('i.sid', $sid)
    ->execute()
    ->fetchAll();
}

/**
 * A webform component is being saved, save its islandora ingest mapping.
 *
 * @param array $form
 *   the component edit form
 * @param array $form_state
 *   the form_state
 *
 * @throws \Exception
 */
function iwi_save_component_mapping($form, $form_state) {

  $nid = $form['nid']['#value'];
  $cid = $form['cid']['#value'];
  $mode = !empty($form_state['values']['ingest']['mode']) ? $form_state['values']['ingest']['mode'] : '';
  $datastream_label = !empty($form_state['values']['ingest']['datastream']) ? $form_state['values']['ingest']['datastream'] : '';
  $datastream_parts = explode(' ', $datastream_label);
  $datastream_id = $datastream_parts[0];
  $field = !empty($form_state['values']['ingest']['fields_wrapper']['fields']) ? $form_state['values']['ingest']['fields_wrapper']['fields'] : '';
  $data = array(
    'mode' => $mode,
    'datastream' => $datastream_label,
    'datastream_id' => $datastream_id,
    'field' => $field,
  );
  if ($id = db_select('islandora_webform_ingest_map', 'm')
    ->fields('m', array('id'))
    ->condition('m.nid', $nid)
    ->condition('m.cid', $cid)
    ->execute()->fetchField()
  ) {
    // Empty mode means "do not ingest", so remove the mapping.
    if (empty($mode)) {
      db_delete('islandora_webform_ingest_map')
        ->condition('id', $id)
        ->execute();
    }
    else {
      db_update('islandora_webform_ingest_map')
        ->fields(array('data' => serialize($data)))
        ->condition('id', $id)
        ->execute();
    }
  }
  elseif (!empty($mode)) {
    db_insert('islandora_webform_ingest_map')
      ->fields(array(
          'nid' => $nid,
          'cid' => $cid,
          'data' => serialize($data),
        )
      )->execute();
  }

}

/**
 * Retrieve islandora ingest mapping for a given webform component.
 *
 * @param int $webform_nid
 *   The node id of the webform
 * @param int $cid
 *   The component id
 *
 * @return mixed
 *   the component mapping data as an array
 */
function iwi_get_saved_component_mapping($webform_nid, $cid) {
  $data = db_select('islandora_webform_ingest_map', 'm')
    ->fields('m', array('data'))
    ->condition('m.nid', $webform_nid)
    ->condition('m.cid', $cid)
    ->execute()->fetchField();
  return @unserialize($data);
}

/**
 * Retrieve all islandora ingest mappings for a given webform.
 *
 * @param int $webform_nid
 *   the node id of the webform
 *
 * @return mixed
 *   an array with one row per component,
 *   each row as an array of component mapping data
 */
function iwi_get_saved_component_mappings($webform_nid) {
  $results = db_select('islandora_webform_ingest_map', 'm')
    ->fields('m', array('cid', 'data'))
    ->condition('m.nid', $webform_nid)
    ->execute()->fetchAll();
  foreach ($results as &$result) {
    $result->data = @unserialize($result->data);
  }
  return $results;
}

/**
 * Retrieve the content_model filter for a webform.
 *
 * @param int $webform_nid
 *   the node id of the webform
 *
 * @return string
 *   the cmodel filter value, or NULL
 */
function iwi_webform_content_model($webform_nid) {
  return db_select('islandora_webform_webforms', 'iw')
    ->fields('iw', array('cmodel_filter'))
    ->condition('iw.entity_id', $webform_nid)
    ->execute()->fetchField();
}

/**
 * Retrieve the ingest settings for a webform.
 *
 * @param int $webform_nid
 *   the node id of the webform
 *
 * @return string
 *   the cmodel filter value, or NULL
 */
function iwi_webform_ingest_settings($webform_nid) {
  return db_select('islandora_webform_ingest_webforms', 'iw')
    ->fields('iw', array('ingest_enabled', 'cmodel', 'relation', 'namespace'))
    ->condition('iw.nid', $webform_nid)
    ->execute()->fetch();
}


/**
 * This function is the clone & replace workhorse.
 *
 * It uses the objective_form_storage to allow new elements to behave like
 * those created directly using XML values and elements.
 *
 * @param array $form
 *   the form we are modifying
 * @param array $form_state
 *   the form state
 * @param string $elementspath
 *   the full path(in case of multivalued, theorical) to the target element.
 * @param array $input_value
 *   the passed array of attributes for the target element
 * @param string $mode
 *   either "append" or "replace" (default)
 * @param int|str $offset
 *   Ignored if $mode != "append".
 *   If NULL (default) will append at the end.
 *   When == existing child element key will replace.
 *   Other values will append using the defined $offset as key.
 * @param string $delimiter
 *   the character used to delimit the $elementspath.
 */
function iwi_set_form_element_byhash(&$form, &$form_state, $elementspath, $input_value, $mode = 'replace', $offset = NULL, $delimiter = ':') {

  $parents = explode($delimiter, $elementspath);
  if (!is_array($input_value)) {
    drupal_set_message(t('The input_value passed to path %path in this form is not an array', array('%path' => $elementspath)), 'error');
    return;
  }
  else {
    // In case $input_value is empty Array give it something;
    $input_value += array('#attributes' => array(), '#default_value' => NULL);
  }
  // XML forms have two special form elements, whose children need to be cloned:
  // if #type in (tags,tabs) and $mode=='append'
  // So we are going to check for those types before deciding how
  // to handle appends.
  $special_xmlform_elements = array('tags' => 'tag', 'tabs' => 'tabpanel');
  // We are handling append different, because replace means just setting the
  // value to an existing element. But if the element was already cloned we
  // will have to switch to a replace functionality.
  // This means: if we got i.e a submited field like name[0]manePart - offset 1
  // and then later a name[0]role with the same offset we can't clone again.

  switch ($mode) {
    case 'append':
      $ref = &$form;
      $temp_path = array();
      $special_element_child_type = NULL;
      $target_element_hash = NULL;
      $child_element_hash = NULL;
      $level = 0;
      $special_element_ref = array();
      $previous_partial_path = array();
      $key_exists = NULL;
      foreach ($parents as $parent) {
        if (is_array($ref) && array_key_exists($parent, $ref)) {
          $key_exists = TRUE;
          $level++;
          $temp_path[] = $parent;
          if (count($special_element_ref) > 0 && $ref[$parent]['#type'] == $special_element_child_type) {
            // Means we already have found an special element container in a
            // previous iteration and we are standing at a child element;
            // Keep track of where we are, store tree descending path.
            $future_partial_path = array_slice($parents, $level);
            // Tree up path.
            $previous_partial_path = $temp_path;
            // Getting rid of the "numeric key" if it's there...
            array_pop($previous_partial_path);
            // Lets get the hash!
            $child_element_hash = $ref[$parent]['#hash'];
          }
          if (array_key_exists($ref[$parent]['#type'], $special_xmlform_elements)) {
            // Means we are at the root element of a special xml form, we want
            // the last one always, in the rare case we have multiple nested
            // ones.
            $special_element_ref = &$ref[$parent];
            $special_element_child_type = $special_xmlform_elements[$ref[$parent]['#type']];
            // Lets store this point so we know where to append our "cloned"
            // element.
            $target_element_hash = $ref[$parent]['#hash'];
          }
          $ref = &$ref[$parent];
        }
        else {
          // We passed an invalid path.
          $key_exists = FALSE;
          drupal_set_message(t('Attempted to resolve path %path in this form, but it does not exist.', array('%path' => $elementspath)), 'error');
        }
      }
      if (($key_exists && !empty($target_element_hash)) && (!empty($child_element_hash))) {
        // OK, we got something to clone, let's do it using the objective_
        // form_api:
        $element = $form_state['storage']['objective_form_storage']['root']->findElement($target_element_hash);
        if ($element) {
          switch ($special_element_child_type) {
            case "tabpanel":
              $tab = $element->findElement($child_element_hash);
              if ($tab) {
                $special_element_children = element_children($special_element_ref);
                if (!isset($special_element_children[$offset])) {
                  // If not already cloned for this offset clone it.
                  $new_tab = clone $tab;
                  $set_element_properties = function ($element) use ($special_xmlform_elements) {
                    $element->default_value = NULL;
                    foreach($element->children as $key => &$child) {
                      if($key !== 0 && in_array($child->controls['#type'], $special_xmlform_elements) ) {
                        $child->orphan();
                      }
                    }
                  };
                  // Set element properties on the descendants.
                  $new_tab->eachDecendant($set_element_properties);
                  $element->adopt($new_tab, $offset);
                  $new_tab_drupal = $new_tab->toArray();
                  // Set element properties into form only for $offset.
                  drupal_array_set_nested_value($new_tab_drupal, array_merge($future_partial_path, array('#attributes')), $input_value['#attributes']);
                  drupal_array_set_nested_value($new_tab_drupal, array_merge($future_partial_path, array('#default_value')), $input_value['#default_value']);
                  // Appending the drupal form array back to where it belongs.
                  if (isset($offset)) {
                    $special_element_ref[$offset] = $new_tab_drupal;
                  }
                  else {
                    // If no $offset given and 'appending' then
                    // just put at the end.
                    $special_element_ref[] = $new_tab_drupal;
                  }
                }
                else {
                  // Means we have already cloned the container, so just put
                  // the values there. We can't use the hash here; we don't
                  // know it.
                  // This gives us the real path to a previously cloned
                  // element.
                  $fullnewpath = array_merge($previous_partial_path, array($offset), $future_partial_path);
                  drupal_array_set_nested_value($form, array_merge($fullnewpath, array('#attributes')), $input_value['#attributes']);
                  drupal_array_set_nested_value($form, array_merge($fullnewpath, array('#default_value')), $input_value['#default_value']);
                }
              }
              break;

            case "tag":
              // Now appending at offset. This means the new value is
              // "themed" like a tag.
              $tag = $element->findElement($child_element_hash);
              if ($tag) {
                $special_element_children = element_children($special_element_ref);
                if (!isset($special_element_children[$offset])) {
                  $new_tag = clone $tag;
                  // Create new tag and have it stored in the state.
                  $new_tag->default_value = $input_value['#default_value'];
                  $element->adopt($new_tag, $offset);
                  $new_tag_drupal = $new_tag->toArray();
                  // Set element properties into form only for $offset.
                  $new_tag_drupal['#default_value'] = $input_value['#default_value'];
                  $new_tag_drupal['#attributes'] = $input_value['#attributes'];
                  // Update drupal form.
                  if (isset($offset)) {
                    $special_element_ref[$offset] = $new_tag_drupal;
                  }
                  else {
                    // If no $offset given and 'appending' then
                    // just put at the end.
                    $special_element_ref[] = $new_tag_drupal;
                  }
                }
                else {
                  $fullnewpath = array_merge($previous_partial_path, array($offset), $future_partial_path);
                  drupal_array_set_nested_value($form, array_merge($fullnewpath, array('#attributes')), $input_value['#attributes']);
                  drupal_array_set_nested_value($form, array_merge($fullnewpath, array('#default_value')), $input_value['#default_value']);
                }
              }
              break;

            default:
              break;

          }
        }
      }
      else {
        // We are going to text-append existing value.
        $key_exists = NULL;
        $target_element_hash = drupal_array_get_nested_value($form, array_merge($parents, array('#hash')), $key_exists);
        if ($key_exists) {
          drupal_array_set_nested_value($form, array_merge($parents, array('#attributes')), $input_value['#attributes']);
          drupal_array_set_nested_value($form, array_merge($parents, array('#default_value')), $input_value['#default_value']);
          $element = $form_state['storage']['objective_form_storage']['root']->findElement($target_element_hash);
          if (!empty($element->default_value)) {
            $input_value['#default_value'] = "; " . $input_value['#default_value'];
          }
          $element->default_value = $input_value['#default_value'];
        }
      }
      break;

    default:
      // We are going to replace existing value.
      // If we got only one, just put our values there.
      $key_exists = NULL;
      $target_element_hash = drupal_array_get_nested_value($form, array_merge($parents, array('#hash')), $key_exists);
      if ($key_exists) {
        drupal_array_set_nested_value($form, array_merge($parents, array('#attributes')), $input_value['#attributes']);
        drupal_array_set_nested_value($form, array_merge($parents, array('#default_value')), $input_value['#default_value']);
        $element = $form_state['storage']['objective_form_storage']['root']->findElement($target_element_hash);
        $element->default_value = $input_value['#default_value'];
      }
  }
}

/**
 * Write webform ingest configuration to database.
 *
 * @param array $form
 *   The webform config form
 * @param array $form_state
 *   The webform config form_state
 *
 * @throws \Exception
 */
function iwi_save_webform_configuration($form, &$form_state) {

  $nid = $form['nid']['#value'];
  $enabled = !empty($form_state['values']['islandora_ingest']['ingest_enabled']) ? $form_state['values']['islandora_ingest']['ingest_enabled'] : 0;
  $cmodel = !empty($form_state['values']['islandora_ingest']['cmodel']) ? $form_state['values']['islandora_ingest']['cmodel'] : '';
  // Makes no sense to store a relation if we are not creating new objects with this form anymore.
  $relation = !empty($form_state['values']['islandora_ingest']['relation']) && !empty($form_state['values']['islandora_ingest']['cmodel']) ? $form_state['values']['islandora_ingest']['relation'] : '';
  $namespace = !empty($form_state['values']['islandora_ingest']['namespace']) ? $form_state['values']['islandora_ingest']['namespace'] : islandora_get_namespace(variable_get('islandora_repository_pid', 'islandora') );
  if ($old_settings = db_select('islandora_webform_ingest_webforms', 'm')
    ->fields('m')
    ->condition('m.nid', $nid)
    ->execute()->fetch()
  ) {
    // If cmodel is empty we keep the old relation and namespace. Admin could have modified this first,
    // then changed cmodel to 'empty', and by doing this, made those fields invisible,
    // not being aware of the change anymore.
    if (empty($cmodel)) { 
      $relation = $old_settings->relation;
      $namespace = !empty($old_settings->{$namespace}) ? $old_settings->{$namespace} : 'islandora';
    }
    // If content model is changing, wipe out ingest mappings.
    if ($old_settings->cmodel != $cmodel) {
      db_delete('islandora_webform_ingest_map')->condition('nid', $nid)->execute();
      drupal_set_message(t('The destination content model was changed and therefore all existing component ingest mappings for this webform were deleted. You will have to edit each webform component that you wish to ingest.'), 'warning');
    }
    // If the relation is changing, show a new form and let the admin choose.
    // Additionally test if we are giving a new relation here && there is a cmodel in place.
    // Makes no sense to modify existing objects if the new form is converted from creation to update.
    if (($old_settings->relation != $relation) && !empty($relation) && !empty($cmodel)) {
      $form_state['stepdata']['form_build_id'] = $form_state['values']['form_build_id'];
      $form_state['stepdata']['old_relation'] = $old_settings->relation;
      $form_state['stepdata']['new_relation'] = $relation;
      $form_state['stepdata']['nid'] = $nid;
      $form_state['step'] = 'confirm';
      $form_state['rebuild'] = TRUE;
    }

    // Update the previous islandora webform ingest settings record.
    db_update('islandora_webform_ingest_webforms')
      ->fields(array(
        'ingest_enabled' => $enabled,
        'cmodel' => $cmodel,
        'relation' => $relation,
        'namespace' => $namespace,
      ))
      ->condition('id', $old_settings->id)
      ->execute();
    drupal_set_message(t('Your ingest settings where saved'), 'status');
    
    }

  // No previous islandora webform ingest settings record, so just create a new
  // one.
  else {
    db_insert('islandora_webform_ingest_webforms')
      ->fields(array(
        'nid' => $nid,
        'ingest_enabled' => $enabled,
        'cmodel' => $cmodel,
        'relation' => $relation,
        'namespace' => $namespace,
      ))
      ->execute();
  drupal_set_message(t('Your ingest settings where saved'), 'status');
  }
}

/**
 * Migrate objects from one predicate to other based admins choice
 *
 * @param array $form
 *   The webform confirm form
 * @param array $form_state
 *   The webform confirm form_state
 *
 * @throws \Exception
 */
function iwi_migrate_webform_objects($form, &$form_state) {

  module_load_include('inc', 'islandora_webform', 'includes/utilites');
  $old_relation = !empty($form_state['stepdata']['old_relation']) ? $form_state['stepdata']['old_relation'] : '';
  $new_relation = $form_state['stepdata']['new_relation'];
  $choice = $form_state['values']['islandora_confirm']['migrate_predicates'];
  $nid = $form_state['stepdata']['nid'];
  $subject_avoid_index = array();
  $old_relations_modified = array();
  $removedcount = 0;
  $addedcount = 0;
  $alreadyinplace = 0;
  switch ($choice) {
    case 'all':
      $child_objects_data = iwi_get_objects_with_relation(NULL, $nid);
      break;

    case 'oldpredicate':
      $child_objects_data = iwi_get_objects_with_relation($old_relation, $nid);
      break;

    case 'none':
    default:
      $child_objects_data = NULL;
      break;
  }

  
  // If the relation is changing, read the admin's choice on this.
  if ((!is_null($child_objects_data)) && (!empty($new_relation))){
    foreach ($child_objects_data as $key => $child_object_data) {
      // Our idea
      // First, if the $new_relation is already in place, mark this subjects and leave them alone
      // Second, even whem migrating all, we must give existing predicates having '$old_relation' priority
      // Third, if a subject has multiple relations and $old_relation was not found we will only replace the first one. It's a poor's man approach to
      // not knowing which one really was created by our webform, but removing everything is even worse
      // Fourth. Only add one relation. Our webforms generates only one, so why should we do more?
      if (!array_key_exists($child_object_data['object']['value'], $subject_avoid_index)) {
        if ($child_object_data['predicate']['uri'] == 'info:fedora/fedora-system:def/relations-external#'.$new_relation) {
          //We already have our new predicate. This subject gets marked 
          $subject_avoid_index[$child_object_data['object']['value']] = 1;
        }
        elseif (($child_object_data['predicate']['uri'] == 'info:fedora/fedora-system:def/relations-external#'.$old_relation) && !empty($old_relation)){
          //The old predicate is present. 
          $child_object = islandora_object_load($child_object_data['object']['value']);
          if ($child_object->relationships->remove(FEDORA_RELS_EXT_URI, $old_relation, $child_object_data['secondobject']['value'],RELS_TYPE_URI)) {
            $removedcount++;
            $old_relations_modified[$old_relation] = 1;
          }
          //Now add our new relation
          if ($new_relation) {
            $child_object->relationships->add(FEDORA_RELS_EXT_URI, $new_relation, $child_object_data['secondobject']['value'],RELS_TYPE_URI);
            $addedcount++;
          }
          //And dismiss this subject. We are done with him.
          $subject_avoid_index[$child_object_data['object']['value']] = 1;
        }
      }  
    }
    //Now iterate again to manage the rest, if any 
    foreach ($child_objects_data as $child_object_data) {
      if (!array_key_exists($child_object_data['object']['value'], $subject_avoid_index)) {
        $child_object = islandora_object_load($child_object_data['object']['value']);
        $current_rel = array();
        $current_rel = explode('#', $child_object_data['predicate']['uri']);
        if (isset($current_rel[1]) && !empty($current_rel[1])) {
          if ($child_object->relationships->remove(FEDORA_RELS_EXT_URI, $current_rel[1], $child_object_data['secondobject']['value'], RELS_TYPE_URI)) {
             $removedcount++;
             $old_relations_modified[$current_rel[1]] = 1;
          }
        }
        //Now add our new relation
        if ($new_relation) {
          $child_object->relationships->add(FEDORA_RELS_EXT_URI, $new_relation, $child_object_data['secondobject']['value'], RELS_TYPE_URI);
          $addedcount++;
        }
        //And dismiss this subject. We are done with him.
        $subject_avoid_index[$child_object_data['object']['value']] = 1;
      }
    }
  }
  drupal_set_message(format_string('@count objects had their relations updated from "@old" to "@new"', array(
      '@count' => $addedcount,
      '@old' => implode(",",array_keys($old_relations_modified)),
      '@new' => $new_relation,
    )));
}


/**
 * Utility function returns mime types given file extensions.
 *
 * @param string|array $extensions
 *   a single file extension as a string, or multiple file extensions
 *   as an array.
 *
 * @return string|array
 *   mime type(s), defaults to 'application/octet-stream'
 */
function iwi_get_mimetypes_from_extensions($extensions) {
  module_load_include('inc', 'content', 'includes/file.mimetypes');
  $mapping = file_mimetype_mapping();
  $mode = is_array($extensions) ? 'array' : 'string';
  $extensions = (array) $extensions;
  $mimetypes = array();
  foreach ($extensions as $extension) {
    // Islandora doesn't use the standard jpeg mime type.
    if ($extension == 'jpg') {
      $mimetypes[] = 'image/jpg';
    }
    elseif (isset($mapping['extensions'][$extension])) {
      $mimetypes[] = $mapping['mimetypes'][$mapping['extensions'][$extension]];
    }
  }
  if (!count($mimetypes)) {
    $mimetypes[] = 'application/octet-stream';
  }
  return $mode == 'array' ? $mimetypes : reset($mimetypes);
}

/**
 * Get data about objects associated with this pid through any rels-ext.
 *
 * @TODO: Add exclusive option - only return objects whose only relation
 * is to this object.
 *
 * @param int $pid
 *   The parent object's pid.
 *
 * @param int $page_number
 *   Used to calculate offset
 *
 * @param int $page_size
 *   If > 0, limit count of results to this number
 *
 * @return array
 *   An array containing pids of child objects
 */
function iwi_query_related_objects($pid, $page_number = 0, $page_size = 0) {
  $connection = islandora_get_tuque_connection();
  $query = <<<EOQ
SELECT ?object ?predicate
FROM <#ri>
WHERE {
  ?object <fedora-model:label> ?title ;
  ?predicate <info:fedora/{$pid}> .
  !optionals
  !optexclude
  !filters
}

EOQ;
  // Be consistent with REPO's XACML permissions.
  $query_optionals = (array) module_invoke('islandora_xacml_api', 'islandora_basic_collection_get_query_optionals', 'manage');
  $query_filters = (array) module_invoke('islandora_xacml_api', 'islandora_basic_collection_get_query_filters');
  // Exclude collection predicates and CMODEL predicates here.
  $collection_predicate_filters = " !sameTerm(?predicate, <fedora-rels-ext:isMemberOfCollection>) && !sameTerm(?predicate, <fedora-rels-ext:isMemberOf>) && !sameTerm(?predicate, <fedora-model:hasModel>)";
  $query_filters[] = $collection_predicate_filters;
  $filter_map = function ($filter) {
    return "FILTER($filter)";
  };
  // Basicly this query statement excludes every object that also has a
  // relationship to a collection. This avoids deleting object that would not
  // really became orphan if deleting it's parent.
  // @TODO we should really extend this to any relation that connects this
  // object to any other different than $PID.
  $query_unbound_optionals = "?object2 <fedora-model:label> ?title ; ?predicate <info:fedora/{$pid}>. ?object2 <fedora-rels-ext:isMemberOfCollection> ?collection";
  $query_filters[] = "!bound(?collection)";

  $query = format_string($query, array(
    '!optionals' => !empty($query_optionals) ? ('OPTIONAL {{' . implode('} UNION {', $query_optionals) . '}}') : '',
    '!filters' => implode(' ', array_map($filter_map, $query_filters)),
  ));
  $query = format_string($query, array(
    '!optexclude' => !empty($query_unbound_optionals) ? ('. OPTIONAL { ' . $query_unbound_optionals . '}') : '',
  ));

  // Include paging parameters if provided.
  if ($page_number > 0 && $page_size > 0) {
    // Add in the offset.
    $offset = $page_number * $page_size;
    $query .= " offset $offset";
  }
  if ($page_size > 0) {
    // Add in the limit.
    $query .= " limit $page_size";
  }
  error_log($query);
  $queryresults = $connection->repository->ri->sparqlQuery($query);

  return $queryresults;
}

/**
 * Get data about objects having this relation term and are related to a given webform .
 * This function can be used also with no relation or nid to fetch any combination needed
 * @param string $relation
 *   The relation term.
 * @param string $nid
 *   The webform's node id that originally was used to create the object.
 * @return array
 *   An array containing query results
 */
function iwi_get_objects_with_relation($relation = NULL, $nid = NULL) {
  $connection = islandora_get_tuque_connection();
  $query=iwi_get_objects_with_relation_query($relation,$nid);
  $queryresults = $connection->repository->ri->query($query,'sparql');
  return $queryresults;
}
/**
 * Get object count for objects having this relation term and are related to a given webform .
 * This function can be used also with no relation or nid to fetch any combination needed
 * @param string $relation
 *   The relation term.
 * @param string $nid
 *   The webform's node id that originally was used to create the object.
 * @return int
 *   Number of objects in query results
 */
function iwi_count_objects_with_relation($relation = NULL, $nid = NULL) {
  
  $connection = islandora_get_tuque_connection();
  $query=iwi_get_objects_with_relation_countquery($relation,$nid);
  $queryresults = $connection->repository->ri->countQuery($query,'sparql');
  return $queryresults;
}
/**
 * Get sparql query for fetching objects having this relation term and are related to a given webform .
 * This function can be used also with no relation or nid to fetch any combination needed
 * @param string $relation
 *   The relation term.
 * @param string $nid
 *   The webform's node id that originally was used to create the object.
 * @return array
 *   An array containing query results
 */
function iwi_get_objects_with_relation_query($relation = NULL, $nid = NULL) {
  $iw_is_predicate = ISLANDORA_WEBFORM_ISIW_PREDICATE;
  $iw_has_predicate = ISLANDORA_WEBFORM_HASIW_PREDICATE;
  $query = <<<EOQ
PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
PREFIX fedora-rels-ext: <info:fedora/fedora-system:def/relations-external#>
SELECT DISTINCT ?object ?predicate ?secondobject
FROM <#ri>
WHERE {
  ?object <fedora-model:label> ?title ;
  islandora-rels-ext:$iw_is_predicate "true";
  !iw_node_match
  ?predicate ?secondobject .
  ?secondobject <info:fedora/fedora-system:def/model#hasModel> <info:fedora/fedora-system:FedoraObject-3.0> .
  !filters
}
EOQ;

  $filter_map = function ($filter) {
    return "FILTER($filter)\n";
  };
  $query_filters = array();
  $query_extramatch = array();
  
  $iw_onlyuris_filters = "isIRI(?secondobject)"; //we want only relations that point to an object.
  
  $query_filters[] = $iw_onlyuris_filters;
  $query_filters[] ="regex(str(?predicate), 'info:fedora/fedora-system:def/relations-external#','i')";
  if (isset($relation) && !empty($relation)) {
  $iw_predicate_filters = "sameTerm(?predicate, <fedora-rels-ext:{$relation}>)"; //Only for a given predicate
  $query_filters[] = $iw_predicate_filters;
  }
  
  if (isset($nid) && !empty($nid)) {
  $iw_nodeid_match = "islandora-rels-ext:{$iw_has_predicate} \"{$nid}\""; //Only for a given Node id association
  $query_extramatch[] = $iw_nodeid_match;
  }
 
  $query = format_string($query, array(
    '!iw_node_match' => !empty($query_extramatch) ? implode(' ;', $query_extramatch)." ;" : '',
    '!filters' => implode(' ', array_map($filter_map, $query_filters)),
  ));  
 return $query;
  
}
/**
 * Get sparql query for counting objects having this relation term and are related to a given webform .
 * This function can be used also with no relation or nid to fetch any combination needed
 * @param string $relation
 *   The relation term.
 * @param string $nid
 *   The webform's node id that originally was used to create the object.
 * @return array
 *   An array containing query results
 */
function iwi_get_objects_with_relation_countquery($relation = NULL, $nid = NULL) {
  $iw_is_predicate = ISLANDORA_WEBFORM_ISIW_PREDICATE;
  $iw_has_predicate = ISLANDORA_WEBFORM_HASIW_PREDICATE;
  $query = <<<EOQ
PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
PREFIX fedora-rels-ext: <info:fedora/fedora-system:def/relations-external#>
SELECT DISTINCT ?object
FROM <#ri>
WHERE {
  ?object <fedora-model:label> ?title ;
  islandora-rels-ext:$iw_is_predicate "true";
  !iw_node_match
  ?predicate ?secondobject .
  ?secondobject <info:fedora/fedora-system:def/model#hasModel> <info:fedora/fedora-system:FedoraObject-3.0> .
  !filters
}
EOQ;

  $filter_map = function ($filter) {
    return "FILTER($filter)\n";
  };
  $query_filters = array();
  $query_extramatch = array();
  
  $iw_onlyuris_filters = "isIRI(?secondobject)"; //we want only relations that point to an object.
  
  $query_filters[] = $iw_onlyuris_filters;
  $query_filters[] ="regex(str(?predicate), 'info:fedora/fedora-system:def/relations-external#','i')";
  if (isset($relation) && !empty($relation)) {
  $iw_predicate_filters = "sameTerm(?predicate, <fedora-rels-ext:{$relation}>)"; //Only for a given predicate
  $query_filters[] = $iw_predicate_filters;
  }
  
  if (isset($nid) && !empty($nid)) {
  $iw_nodeid_match = "islandora-rels-ext:{$iw_has_predicate} \"{$nid}\""; //Only for a given Node id association
  $query_extramatch[] = $iw_nodeid_match;
  }
 
  $query = format_string($query, array(
    '!iw_node_match' => !empty($query_extramatch) ? implode(' ;', $query_extramatch)." ;" : '',
    '!filters' => implode(' ', array_map($filter_map, $query_filters)),
  ));  
 return $query;
  
}