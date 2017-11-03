<?php
/**
 * @file
 * Page callbacks for the Webform Workflow module.
 */

/**
 * Page callback to display a list of webform submissions.
 *
 * This replaces the page at 'node/%webform_menu/webform-results', when workflow
 * is enabled for a webform. It allows the workflow state to be displayed.
 */
function webform_workflow_submissions_list($node) {
  if (!webform_workflow_is_enabled($node)) {
    module_load_include('inc', 'webform', 'includes/webform.report');
    return webform_results_submissions($node, FALSE, 50);
  }
  $view = views_get_view('webform_workflow_submissions');
  $view->override_path = current_path();
  $view->override_url = current_path();
  return $view->preview('embed', array($node->nid));
}

/**
 * Page callback to display a workflow transition log for a submission.
 */
function webform_workflow_log_page($node, $submission) {
  if (!webform_workflow_is_enabled($node)) {
    return MENU_NOT_FOUND;
  }
  drupal_set_title(t('Submission #@sid: workflow log', array(
    '@sid' => $submission->sid,
  )));
  $view = views_get_view('webform_workflow_log');
  $view->override_path = current_path();
  $view->override_url = current_path();
  return $view->preview('embed', array($node->nid, $submission->sid));
}
