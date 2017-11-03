<?php

/**
 * @file
 * The admin form.
 */

/**
 * Admin form for the batch.
 */
function islandora_batch_admin_form($form, $form_state) {
  $form['islandora_batch_java'] = array(
    '#type' => 'textfield',
    '#title' => t('Java Executable'),
    '#description' => t('The Java executable to use to run MARC to MODS transformation.'),
    '#default_value' => variable_get('islandora_batch_java', 'java'),
  );
  $form['islandora_batch_auto_remove_set'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-remove batch set after batch successfully processes?'),
    '#description' => t('When a batch set successfully completes, it will be removed from the batch queue.'),
    '#default_value' => variable_get('islandora_batch_auto_remove_set', FALSE),
  );
  $form['islandora_batch_link_when_finished'] = array(
    '#type' => 'checkbox',
    '#access' => module_exists('views'),
    '#title' => t('Link to batch queue after batch successfully processes?'),
    '#description' => t('When a batch set successfully completes, a link to review the queue for the given set will be created. This is dependent on the "views" module being enabled.'),
    '#default_value' => variable_get('islandora_batch_link_when_finished', TRUE),
    '#states' => array(
      'invisible' => array(
        ':input[name="islandora_batch_auto_remove_set"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Batch configuration'),
    '#submit' => array('islandora_batch_admin_form_submit'),
  );

  return $form;
}

/**
 * Submit handler for the Admin form.
 */
function islandora_batch_admin_form_submit($form, $form_state) {
  variable_set('islandora_batch_java', $form_state['values']['islandora_batch_java']);
  variable_set('islandora_batch_auto_remove_set', $form_state['values']['islandora_batch_auto_remove_set']);
  // If auto-remove is enabled, force link-when-finished to be disabled.
  if ($form_state['values']['islandora_batch_auto_remove_set']) {
    variable_set('islandora_batch_link_when_finished', FALSE);
  }
  else {
    variable_set('islandora_batch_link_when_finished', $form_state['values']['islandora_batch_link_when_finished']);
  }

  drupal_set_message(t('The Batch configuration options have been saved.'));
}
