<?php
/**
 * @file
 * Forms for the Webform Workflow module.
 */

/**
 * Configuration form.
 */
function webform_workflow_config_form($form, &$form_state, $node) {
  form_load_include($form_state, 'inc', 'webform_workflow', 'includes/webform_workflow.forms');

  $form_state['node'] = $node;

  $form['enable_workflow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable workflow'),
    '#description' => t('Enable workflow for this form.'),
    '#default_value' => webform_workflow_is_enabled($node),
  );

  $form['states'] = array(
    '#type' => 'container',
  );
  field_attach_form('node', $node, $form['states'], $form_state, NULL, array('field_name' => 'webform_workflow_states'));

  // Ensure that the Metatags and Redirect module's form elements are not
  // accidentally included.
  unset($form['states']['#metatags'], $form['states']['redirect']);

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('State settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !isset($node->webform_workflow),
  );

  $form['settings']['require_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require a log message'),
    '#description' => t('Set whether users must enter a log message whenever they change the state of a submission.'),
    '#default_value' => isset($node->webform_workflow) && !empty($node->webform_workflow->data['require_log']),
  );

  $form['settings']['os_view_state'] = array(
    '#type' => 'checkbox',
    '#title' => t("Always allow the submitter to view the submission's state"),
    '#default_value' => isset($node->webform_workflow) && !empty($node->webform_workflow->data['os_view_state']),
  );

  $form['settings']['new_gets_first_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set new submissions to the first state'),
    '#description' => t('Enable this to automatically assign the first workflow state to new submissions.'),
    '#default_value' => isset($node->webform_workflow) && !empty($node->webform_workflow->data['new_gets_first_state']),
  );
  $form['settings']['new_triggers_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify users when setting the state of new submissions'),
    '#description' => t('This is not recommended, because the <a href="@settings">Webform E-mails</a> settings already allow you to send messages for new submissions.', array(
      '@settings' => url('node/' . $node->nid . '/webform/emails'),
    )),
    '#states' => array(
      'visible' => array(
        ':input[name="new_gets_first_state"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => isset($node->webform_workflow) && !empty($node->webform_workflow->data['new_triggers_notification']),
  );

  $form['settings']['updated_gets_first_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set submissions to the first state after editing'),
    '#description' => t('Enable this to move submissions to the first state after editing.'),
    '#default_value' => isset($node->webform_workflow) && !empty($node->webform_workflow->data['updated_gets_first_state']),
  );

  $emails = isset($node->webform_workflow) ? $node->webform_workflow->data['emails'] : webform_workflow_get_default_email();

  $form['emails'] = array(
    '#type' => 'fieldset',
    '#title' => t('State transition notification e-mails'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['emails']['subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Message Subject',
    '#default_value' => $emails['subject'],
    '#size' => 80,
  );
  $form['emails']['body'] = array(
    '#type' => 'textarea',
    '#title' => 'Message Body',
    '#default_value' => $emails['body'],
    '#rows' => 5,
    '#cols' => 80,
    '#description' => theme('webform_token_help', array(
      'groups' => array(
        'node',
        'submission',
        'webform_workflow_state',
        'webform-workflow-transition',
      ),
    )),
  );

  $form['buttons'] = array('#type' => 'actions');
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler for the workflow fieldset.
 *
 * Ensures valid inputs otherwise sets form errors.
 */
function webform_workflow_config_form_validate($form, &$form_state) {
  $node = $form_state['node'];
  if ($form_state['values']['enable_workflow']) {
    field_attach_form_validate('node', $node, $form['states'], $form_state);
  }
}

/**
 * Form submit handler for the workflow configuration form.
 */
function webform_workflow_config_form_submit($form, &$form_state) {
  $node = $form_state['node'];

  $fields = array(
    'workflow' => $form_state['values']['enable_workflow'] ? 1 : 0,
    'data' => serialize(array(
      'emails' => $form_state['values']['emails'],
      'require_log' => $form_state['values']['require_log'],
      'os_view_state' => $form_state['values']['os_view_state'],
      'new_gets_first_state' => $form_state['values']['new_gets_first_state'],
      'updated_gets_first_state' => $form_state['values']['updated_gets_first_state'],
    )),
  );

  // Update the workflow configuration.
  db_merge('webform_workflow')
    ->key(array('nid' => $node->nid))
    ->fields($fields)
    ->execute();

  // Update the workflow states (entity reference field).
  field_attach_submit('node', $node, $form['states'], $form_state);
  // This avoids node_save() for performance reasons.
  field_attach_update('node', $node);
  entity_get_controller('node')->resetCache(array($node->nid));
}

/**
 * Form for changing the state of an existing submission.
 */
function webform_workflow_submission_state_form($form, &$form_state, $submission) {
  form_load_include($form_state, 'inc', 'webform_workflow', 'includes/webform_workflow.forms');

  $node = node_load($submission->nid);
  $state = webform_workflow_state_load_by_submission($submission);

  $form['#attributes']['class'] = array('webform-workflow-submission-state-form');

  $form['workflow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow'),
  );

  $form['workflow']['current_state'] = array(
    '#type' => 'item',
    '#title' => t('Current state'),
    '#markup' => theme('webform_workflow_state', array('state' => $state)),
  );

  if (!webform_workflow_state_check_access('from', $state, $node, $submission)) {
    return $form;
  }

  $new_state_options = array();
  $seen_current = FALSE;
  $available_states = webform_workflow_get_available_states($node);
  $group = ($state->wsid) && count($available_states) > 1;
  $new_state_colors = array();
  foreach ($available_states as $new_state) {
    if ($new_state->wsid == $state->wsid) {
      $seen_current = TRUE;
      continue;
    }
    if (!webform_workflow_state_check_access('to', $new_state, $node, $submission)) {
      continue;
    }
    // Group the state options into 'Earlier' and 'Later'. This assumes that
    // the Entity API has provided the available states correctly sorted (this
    // does appear to work with Inline Entity Form).
    if ($group) {
      $key = $seen_current ? t('Later states') : t('Earlier states');
      $new_state_options[$key][$new_state->wsid] = $new_state->label;
    }
    else {
      $new_state_options[$new_state->wsid] = $new_state->label;
    }
    if ($new_state->color) {
      $new_state_colors['state-' . $new_state->wsid] = $new_state->color;
    }
  }

  if (!$new_state_options) {
    return $form;
  }

  $log = db_select('webform_workflow_transition', 'wwl')
    ->fields('wwl')
    ->condition('sid', $submission->sid)
    ->orderBy('id', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetch();
  if ($log) {
    $log_old_state = $log->old_state_wsid ? webform_workflow_state_load($log->old_state_wsid) : NULL;
    $log_new_state = $log->new_state_wsid ? webform_workflow_state_load($log->new_state_wsid) : NULL;
    $log_string = t('@time: %old &rarr; %new', array(
      '@time' => format_date($log->timestamp, 'short'),
      '%old' => $log_old_state ? $log_old_state->label : t('[None]'),
      '%new' => $log_new_state ? $log_new_state->label : t('[None]'),
    ));
    if ($log->uid) {
      $log_string .= ' ' . t('(changed by !user)', array(
        '!user' => theme('username', array('account' => user_load($log->uid))),
      ));
    }
    if ($log->message) {
      $log_string .= '<blockquote class="webform-workflow-log-message">'
        . check_plain($log->message) . '</blockquote>';
    }
    $form['workflow']['log'] = array(
      '#type' => 'item',
      '#title' => t('Last state change'),
      '#markup' => $log_string,
      '#weight' => 50,
    );
  }

  $form_state['submission'] = $submission;

  $form['workflow']['new_state'] = array(
    '#title' => t('New state'),
    '#type' => 'select',
    '#options' => $new_state_options,
    '#required' => TRUE,
    '#title_display' => 'before',
    '#weight' => 20,
  );

  if ($new_state_colors) {
    $form['workflow']['new_state']['#attributes']['class'] = array('webform-workflow-state-select');
    $form['workflow']['new_state']['#attached']['js'] = array(
      drupal_get_path('module', 'webform_workflow') . '/includes/webform_workflow.js',
      array(
        'data' => array('webformWorkflow' => array('stateColors' => $new_state_colors)),
        'type' => 'setting',
      ),
    );
  }

  $new_state_trigger = array(
    ':input[name="new_state"]' => array('!value' => ''),
  );
  $form['workflow']['message'] = array(
    '#type' => 'textfield',
    '#title' => t('Log message'),
    '#description' => t('Describe why you are making this change.'),
    '#states' => array('visible' => $new_state_trigger),
    '#required' => !empty($node->webform_workflow->data['require_log']),
    '#weight' => 30,
  );
  $form['workflow']['buttons'] = array(
    '#type' => 'actions',
    '#states' => array('visible' => $new_state_trigger),
    '#weight' => 40,
  );
  $form['workflow']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change state'),
  );
  $form['workflow']['buttons']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => current_path(),
  );
  return $form;
}

/**
 * Submit callback for changing the state of an existing submission.
 */
function webform_workflow_submission_state_form_submit($form, &$form_state) {
  $submission = $form_state['submission'];
  $new_state = webform_workflow_state_load($form_state['values']['new_state']);
  $message = $form_state['values']['message'];
  webform_workflow_transition($submission, $new_state, $message, $GLOBALS['user'], TRUE, TRUE);
}
