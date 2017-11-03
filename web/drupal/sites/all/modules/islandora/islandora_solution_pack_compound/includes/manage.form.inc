<?php

/**
 * @file
 * includes/manage.form.inc
 *
 * Compound object management callback and form.
 */

/**
 * Menu callback for manage form.
 */
function islandora_compound_object_manage(FedoraObject $object) {
  return drupal_get_form('islandora_compound_object_manage_form', $object);
}

/**
 * Compound management form.
 */
function islandora_compound_object_manage_form($form, &$form_state, $object) {
  $form = array();
  $form_state['object'] = $object;
  $rels_predicate = variable_get('islandora_compound_object_relationship', 'isConstituentOf');

  // Add child objects.
  if ((variable_get('islandora_compound_object_compound_children', TRUE) && in_array(ISLANDORA_COMPOUND_OBJECT_CMODEL, $object->models)) || !variable_get('islandora_compound_object_compound_children', TRUE)) {
    $form['add_children'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add Child Objects'),
      '#description' => t('Add child objects as part of this compound object'),
    );
    $form['add_children']['child'] = array(
      '#type' => 'textfield',
      '#title' => t('Child Object Pid/Label'),
      '#autocomplete_path' => 'islandora_compound_object/autocomplete/child',
    );

    // Remove children.
    $children = islandora_compound_object_get_parts($object->id, TRUE);
    if (!empty($children)) {
      $form['children'] = array(
        '#type' => 'fieldset',
        '#title' => t('Remove Child Objects'),
        '#description' => t('Remove child objects of as part of this compound object'),
        '#collapsed' => TRUE,
        '#collapsible' => TRUE,
      );

      $header = array('title' => t('Title'), 'pid' => t('Object ID'));
      $form['children']['remove_children'] = array(
        '#type' => 'tableselect',
        '#title' => t('Children'),
        '#header' => $header,
        '#options' => $children,
      );
      $form['reorder_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('Reorder'),
        '#collapsed' => TRUE,
        '#collapsible' => TRUE,
      );
      $form['reorder_fieldset']['table'] = array(
        '#theme' => 'islandora_compound_object_draggable_table',
      ) + islandora_compound_object_get_tabledrag_element($object);
    }
  }
  // Add parents.
  $form['add_to_parent'] = array(
    '#type' => 'fieldset',
    '#title' => t('Associate with Parent Object'),
    '#description' => t('Add this object to a parent object'),
  );
  $form['add_to_parent']['parent'] = array(
    '#type' => 'textfield',
    '#title' => t('Parent Object Pid/Label'),
    '#autocomplete_path' => 'islandora_compound_object/autocomplete/parent',
  );

  // Remove parents.
  $parent_part_of = $object->relationships->get('info:fedora/fedora-system:def/relations-external#', $rels_predicate);
  if (!empty($parent_part_of)) {
    $form['parents'] = array(
      '#type' => 'fieldset',
      '#title' => t('Unlink From Parent'),
      '#description' => t('Remove the relationship between this object and parent objects.'),
    );

    $parents = array();
    foreach ($parent_part_of as $parent) {
      // Shouldn't be too much of a hit but would be good to avoid the
      // islandora_object_loads.
      $pid = $parent['object']['value'];
      $parent_object = islandora_object_load($pid);
      $parents[$pid] = array('title' => $parent_object->label, 'pid' => $pid);
    }

    $form['parents']['unlink_parents'] = array(
      '#type' => 'tableselect',
      '#title' => t('Parents'),
      '#header' => array('title' => t('Title'), 'pid' => t('Object ID')),
      '#options' => $parents,
    );
  }

  $form['object'] = array(
    '#type' => 'value',
    '#value' => $object,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Compound object management validation.
 */
function islandora_compound_object_manage_form_validate($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $object = $form_state['values']['object'];
  $rels_predicate = variable_get('islandora_compound_object_relationship', 'isConstituentOf');

  // Child.
  if (!empty($form_state['values']['child'])) {
    if (variable_get('islandora_compound_object_compound_children', TRUE)) {
      $compound = FALSE;
      foreach ($object->models as $value) {
        if ($value == ISLANDORA_COMPOUND_OBJECT_CMODEL) {
          $compound = TRUE;
          break;
        }
      }
      if (!$compound) {
        form_set_error('child', t('This object is not a compound object'));
      }
    }

    // Do not allow child of self.
    if ($form_state['values']['child'] == $object->id) {
      form_set_error('child', t('An object may not be a child of itself.'));
    }

    // Do not allow repeated child and check if valid child pid.
    $child_object = islandora_object_load($form_state['values']['child']);
    if ($child_object) {
      $child_part_of = $child_object->relationships->get(FEDORA_RELS_EXT_URI, $rels_predicate);
      foreach ($child_part_of as $part) {
        if ($part['object']['value'] == $object->id) {
          form_set_error('child', t('The object is already a parent of the child.'));
        }
      }
    }
    else {
      form_set_error('child', t('Invalid object supplied.'));
    }
  }

  // Parent.
  if (!empty($form_state['values']['parent'])) {
    // Check if valid parent pid.
    $parent_object = islandora_object_load($form_state['values']['parent']);
    if (!$parent_object) {
      form_set_error('parent', t('Invalid object supplied.'));
    }
    else {
      if (variable_get('islandora_compound_object_compound_children', TRUE)) {
        if (!in_array(ISLANDORA_COMPOUND_OBJECT_CMODEL, $parent_object->models)) {
          form_set_error('parent', t('The parent object (@pid) is not a compound object!', array('@pid' => $form_state['values']['parent'])));
        }
      }
      // Do not allow parent of self.
      if ($form_state['values']['parent'] == $object->id) {
        form_set_error('parent', t('An object may not be the parent of itself.'));
      }
      // Do not allow repeated parent.
      $parent_part_of = $object->relationships->get(FEDORA_RELS_EXT_URI, $rels_predicate);
      foreach ($parent_part_of as $part) {
        if ($part['object']['value'] == $form_state['values']['parent']) {
          form_set_error('parent', t('The object is already a child of the parent.'));
        }
      }
    }
  }

  if (!empty($form_state['values']['parent']) && !empty($form_state['values']['child']) && $form_state['values']['parent'] == $form_state['values']['child']) {
    form_set_error('child', t('An object may not be the parent and child of the same object.'));
    form_set_error('parent');
  }
}

/**
 * Compound object management submit.
 */
function islandora_compound_object_manage_form_submit($form, &$form_state) {
  $object = $form_state['values']['object'];
  $create_thumbs = variable_get('islandora_compound_object_thumbnail_child', TRUE);
  $rels_predicate = variable_get('islandora_compound_object_relationship', 'isConstituentOf');
  // Relationship from child to this object.
  if (!empty($form_state['values']['child'])) {
    $child_object = islandora_object_load($form_state['values']['child']);
    islandora_compound_object_add_parent($child_object, $object->id);
    if ($create_thumbs) {
      islandora_compound_object_update_parent_thumbnail($object);
    }
  }

  // Add relationship to parent.
  if (!empty($form_state['values']['parent'])) {
    islandora_compound_object_add_parent($object, $form_state['values']['parent']);
    if ($create_thumbs) {
      islandora_compound_object_update_parent_thumbnail(islandora_object_load($form_state['values']['parent']));
    }
  }

  // Remove children.
  // @todo Batch.
  if (!empty($form_state['values']['remove_children'])) {
    islandora_compound_object_remove_parent(array_map('islandora_object_load', array_filter($form_state['values']['remove_children'])), $object->id);
    if ($create_thumbs) {
      islandora_compound_object_update_parent_thumbnail($object);
    }
  }

  // Unlink parents.
  if (!empty($form_state['values']['unlink_parents'])) {
    foreach (array_filter($form_state['values']['unlink_parents']) as $parent) {
      islandora_compound_object_remove_parent($object, $parent);
      if ($create_thumbs) {
        $parent_object = islandora_object_load($parent);
        islandora_compound_object_update_parent_thumbnail($parent_object);
      }
    }
  }

  // @TODO: Actually confirm.
  drupal_set_message(t('Compound relationships modified.'));
}

/**
 * Returns the table to be used for tabledragging.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array
 *   An array defining the markup for the table.
 */
function islandora_compound_object_get_tabledrag_element(AbstractObject $object) {
  $children = islandora_compound_object_get_parts($object->id, TRUE);
  $seq = 0;
  $delta = count($children);
  $map = function($child) use($delta, &$seq) {
    $seq++;
    return array(
      'label' => array('#markup' => check_plain($child['title'])),
      'pid' => array('#markup' => check_plain($child['pid'])),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $seq,
        '#delta' => $delta,
        '#title-display' => 'invisible',
      ));
  };
  $rows = array_map($map, $children);

  return array(
    '#tree' => TRUE,
    'table' => array(
      '#type' => 'markup',
      '#header' => array(t('Title'), t('Object ID'), t('Weight')),
      'rows' => $rows,
    ),
    'actions' => array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save Changes'),
        '#submit' => array('islandora_compound_object_sequence_submit'),
      ),
    ),
  );
}

/**
 * Submit handler for the re-ordering table.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_compound_object_sequence_submit($form, &$form_state) {
  $object = $form_state['object'];
  $compounds = &$form_state['values']['table']['table']['rows'];
  if ($compounds) {
    uasort($compounds, 'drupal_sort_weight');
    islandora_compound_object_sequence_batch($object->id, $compounds);
  }
}

/**
 * Defines and constructs the batch used to re-order compound objects.
 *
 * @param string $pid
 *   The pid of the parent object.
 * @param array $compounds
 *   An array of compound objects we are iterating over.
 */
function islandora_compound_object_sequence_batch($pid, array $compounds) {
  $object = islandora_object_load($pid);
  $context['results'] = array(
    'success' => array(),
    'failure' => array(),
  );
  $context['sandbox']['progress'] = 0;
  $context['sandbox']['total'] = count($compounds);
  $operations = islandora_compound_object_build_sequence_operations($pid, $compounds);
  $batch = array(
    'operations' => $operations,
    'title' => t("Sequencing @label's compound objects ...", array('@label' => $object->label)),
    'init_message' => t("Preparing to sequence @label's child objects ...", array('@label' => $object->label)),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_compound_object') . '/includes/manage.form.inc',
  );
  batch_set($batch);
}

/**
 * To change the thumbnail after update sequence.
 *
 * @param string $parent
 *   The parent pid.
 */
function islandora_compound_object_change_thumbnail($parent) {
  $create_thumbs = variable_get('islandora_compound_object_thumbnail_child', TRUE);
  if ($create_thumbs) {
    islandora_compound_object_update_parent_thumbnail(islandora_object_load($parent));
  }
}

/**
 * Builds up a sequence of operations for sequence numbering.
 *
 * @param string $parent_pid
 *   The pid of the parent object.
 * @param array $compounds
 *   An array of compound objects.
 *
 * @return array
 *   The built up operations array.
 */
function islandora_compound_object_build_sequence_operations($parent_pid, $compounds) {
  $operations = array();
  $start_seq = 1;
  foreach ($compounds as $child_pid => $weight) {
    $operations[] = array(
      'islandora_compound_object_update_sequence', array(
        $parent_pid,
        $child_pid,
        $start_seq,
      ),
    );
    $start_seq++;
  }
  $operations[] = array(
    'islandora_compound_object_change_thumbnail', array(
      $parent_pid,
    ),
  );
  return $operations;
}

/**
 * Updates the relationships to reflect the object's order to the parent.
 *
 * @param string $parent_pid
 *   The pid of the parent object.
 * @param array $child_pid
 *   The pid of the child object;
 * @param array $context
 *   Context used in the batch.
 */
function islandora_compound_object_update_sequence($parent_pid, $child_pid, $start_seq, &$context) {
  $escaped_pid = str_replace(':', '_', $parent_pid);
  $child_object = islandora_object_load($child_pid);
  $child_object->relationships->remove(ISLANDORA_RELS_EXT_URI, "isSequenceNumberOf$escaped_pid");
  $child_object->relationships->add(ISLANDORA_RELS_EXT_URI, "isSequenceNumberOf$escaped_pid", $start_seq, RELS_TYPE_PLAIN_LITERAL);
  $context['message'] = t('Inserting page "@label" (@pid) at position "@pos"', array(
    '@label' => $child_object->label,
    '@pid' => $child_object->id,
    '@pos' => $start_seq,
    )
  );
}

/**
 * Retrieves the insertion sequence number point.
 *
 * @param array $children
 *   All the child objects of a parent we need to iterate over.
 *
 * @return int
 *   The point at which we are to insert the compound object.
 */
function islandora_compound_object_get_insert_sequence_number($children) {
  $insert_seq = 0;
  foreach ($children as $child) {
    if (!empty($child['seq']) && $child['seq'] > $insert_seq) {
      $insert_seq = $child['seq'];
    }
  }
  // Want to insert one past this point.
  $insert_seq++;
  return $insert_seq;
}

/**
 * Add the given objects to the given parents.
 *
 * @param array|AbstractObject $objects
 *   An array of AbstractObject to add to a parent, to produce a compound.
 * @param array|string $parent_pids
 *   An array of PIDs representing the parents to which an object should be
 *   added, to make each member of $objects a member of multiple compounds.
 */
function islandora_compound_object_add_parent($objects, $parent_pids) {
  $objects = $objects instanceof AbstractObject ? array($objects) : $objects;
  $parent_pids = (array) $parent_pids;
  $rels_predicate = variable_get('islandora_compound_object_relationship', 'isConstituentOf');

  foreach ($parent_pids as $parent_pid) {
    $escaped_pid = str_replace(':', '_', $parent_pid);

    foreach ($objects as $object) {
      $children = islandora_compound_object_get_parts($parent_pid, TRUE);
      $insert_seq = islandora_compound_object_get_insert_sequence_number($children);

      $object->relationships->autoCommit = FALSE;
      $object->relationships->add(FEDORA_RELS_EXT_URI, $rels_predicate, $parent_pid);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, "isSequenceNumberOf$escaped_pid", $insert_seq, RELS_TYPE_PLAIN_LITERAL);
      $object->relationships->commitRelationships();
    }
  }

  module_invoke_all('islandora_compound_object_children_added_to_parent', $objects, $parent_pids);
}

/**
 * Remove the given objects from the given parents.
 *
 * @param array|string $objects
 *   An array of objects to remove from each of the PIDs listed in
 *   $parent_pids.
 * @param array|string $parent_pids
 *   An array of PIDs from which the members of $objects will be removed.
 */
function islandora_compound_object_remove_parent($objects, $parent_pids) {
  $objects = $objects instanceof AbstractObject ? array($objects) : $objects;
  $parent_pids = (array) $parent_pids;
  $rels_predicate = variable_get('islandora_compound_object_relationship', 'isConstituentOf');

  foreach ($parent_pids as $parent_pid) {
    $escaped_pid = str_replace(':', '_', $parent_pid);

    foreach ($objects as $object) {
      $object->relationships->remove(FEDORA_RELS_EXT_URI, $rels_predicate, $parent_pid);
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, "isSequenceNumberOf$escaped_pid");
    }
  }

  module_invoke_all('islandora_compound_object_children_removed_from_parent', $objects, $parent_pids);
}
