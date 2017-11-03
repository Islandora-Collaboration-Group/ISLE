<?php

/**
 * @file
 * Builds placeholder replacement tokens for webform_workflow-related data.
 */

/**
 * Implements hook_token_info().
 */
function webform_workflow_token_info() {
  $info = array();
  $info['types']['webform-workflow-transition'] = array(
    'name' => t('Workflow state transition'),
    'description' => t('A transition between two states.'),
    'needs-data' => 'webform-workflow-transition',
  );
  $info['tokens']['webform-workflow-transition']['previous-state'] = array(
    'name' => t('Previous state'),
    'description' => t('The previous workflow state.'),
    'type' => 'webform_workflow_state',
  );
  $info['tokens']['webform-workflow-transition']['new-state'] = array(
    'name' => t('New state'),
    'description' => t('The new workflow state.'),
    'type' => 'webform_workflow_state',
  );
  $info['tokens']['webform-workflow-transition']['timestamp'] = array(
    'name' => t('Timestamp'),
    'description' => t('The date and time of the state change.'),
    'type' => 'date',
  );
  $info['tokens']['webform-workflow-transition']['user'] = array(
    'name' => t('User'),
    'description' => t('The user who made the state change.'),
    'type' => 'user',
  );
  $info['tokens']['webform-workflow-transition']['message'] = array(
    'name' => t('Log message'),
    'description' => t('The log message entered for the transition.'),
  );
  $info['tokens']['submission']['state'] = array(
    'name' => t('Workflow state'),
    'description' => t('The current workflow state for this submission.'),
    'type' => 'webform_workflow_state',
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function webform_workflow_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);
  if ($type == 'submission' && !empty($data['webform-submission'])) {
    $submission = $data['webform-submission'];
    $state = webform_workflow_state_load_by_submission($submission);
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'state':
          $replacement = $state->label;
          $replacements[$original] = $sanitize ? check_plain($replacement) : $replacement;
          break;

      }
    }

    if ($state_tokens = token_find_with_prefix($tokens, 'state')) {
      $replacements += token_generate('webform_workflow_state', $state_tokens, array(
        'webform_workflow_state' => $state,
        'sanitize' => $sanitize,
      ));
    }
  }
  if ($type == 'webform-workflow-transition' && !empty($data['webform-workflow-transition'])) {
    $transition = $data['webform-workflow-transition'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'previous-state':
          $replacement = $transition['previous_state']->label;
          $replacements[$original] = $sanitize ? check_plain($replacement) : $replacement;
          break;

        case 'new-state':
          $replacement = $transition['new_state']->label;
          $replacements[$original] = $sanitize ? check_plain($replacement) : $replacement;
          break;

        case 'message':
          if (!empty($transition['message'])) {
            $replacement = $transition['message'];
            $replacements[$original] = $sanitize ? check_plain($replacement) : $replacement;
          }
          break;

        case 'user':
          if (!empty($transition['account'])) {
            $replacement = format_username($transition['account']);
            $replacements[$original] = $sanitize ? check_plain($replacement) : $replacement;
          }
          break;

        case 'timestamp':
          $replacements[$original] = format_date($transition['timestamp']);
          break;

      }
    }
    if ($new_state_tokens = token_find_with_prefix($tokens, 'new-state')) {
      $replacements += token_generate('webform_workflow_state', $new_state_tokens, array(
        'webform_workflow_state' => $transition['new_state'],
        'sanitize' => $sanitize,
      ));
    }
    if ($previous_state_tokens = token_find_with_prefix($tokens, 'previous-state')) {
      $replacements += token_generate('webform_workflow_state', $previous_state_tokens, array(
        'webform_workflow_state' => $transition['previous_state'],
        'sanitize' => $sanitize,
      ));
    }
    if ($timestamp_tokens = token_find_with_prefix($tokens, 'timestamp')) {
      $replacements += token_generate('date', $timestamp_tokens, array(
        'date' => $transition['timestamp'],
        'sanitize' => $sanitize,
      ));
    }
    if ($user_tokens = token_find_with_prefix($tokens, 'user')) {
      $replacements += token_generate('user', $user_tokens, array(
        'account' => $transition['account'],
        'sanitize' => $sanitize,
      ));
    }
  }
  return $replacements;
}
