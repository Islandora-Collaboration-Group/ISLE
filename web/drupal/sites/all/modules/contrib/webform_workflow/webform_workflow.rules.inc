<?php
/**
 * @file
 * Rules integration for the Webform Workflow module.
 */

/**
 * Implements hook_rules_event_info().
 */
function webform_workflow_rules_event_info() {
  return array(
    'webform_workflow_transition' => array(
      'label' => t('After a webform submission changes state'),
      'help' => t('Act after a webform submission transitions to a new workflow state.'),
      'group' => t('Webform Workflow'),
      'module' => 'webform_workflow',
      'variables' => array(
        'webform' => array(
          'label' => t('Webform'),
          'type' => 'node',
        ),
        'submission' => array(
          'label' => t('Webform submission'),
          'type' => 'webform_workflow_submission',
        ),
        'previous' => array(
          'label' => t('Previous submission state'),
          'type' => 'webform_workflow_state',
          'bundle' => 'webform_workflow_state',
        ),
        'new' => array(
          'label' => t('New submission state'),
          'type' => 'webform_workflow_state',
          'bundle' => 'webform_workflow_state',
        ),
      ),
    ),
  );
}
