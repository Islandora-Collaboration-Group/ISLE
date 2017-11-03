<?php

/**
 * @file
 * This file includes helper functions for creating reports for islandora_webform.module
 *
 * @author Pat Dunlavey <patrick.dunlavey@commonmedia.com>
 */

// All functions within this file need the webform.submissions.inc.
module_load_include('inc', 'webform', 'includes/webform.submissions');

/**
 * Retrieve table of webform submissions for a given object.
 *
 * @param AbstractObject $object
 *   an islandora repository object
 *
 * @param int $pager_count
 *   the number of rows per page
 *
 * @return string
 *   themed table of submissions for this object
 *
 * @throws Exception
 */
function islandora_webform_results_object_submissions_page(AbstractObject $object, $pager_count = 10) {
  global $user;

  $query = db_select('webform_submissions', 's')->fields('s');
  $query->join('node', 'n', 'n.nid = s.nid');
  $query->addField('n', 'title', 'node_title');
  $query->join('webform_component', 'c', "c.nid = s.nid and c.form_key = 'islandora_object_pid'");
  $query->join('webform_submitted_data', 'd', "d.sid = s.sid and d.cid = c.cid");
  $query->condition('d.data', $object->id);

  if(module_exists('webform_workflow')) {
    $params = drupal_get_query_parameters();
    $wsid = !empty($params['wsid']) ? $params['wsid'] : NULL;
    if($wsid) {
      $query->join('webform_workflow_submissions', 'wws', 'wws.sid = s.sid');
      $query->condition('wws.wsid', $wsid);
    }
  }


  if ($pager_count) {
    // Extending the query instantiates a new query object.
    $query = $query->extend('PagerDefault');
    $query->limit($pager_count);
  }

  $submissions = $query->execute()->fetchAllAssoc('sid');


    if (isset($_GET['results']) && is_numeric($_GET['results'])) {
    $pager_count = $_GET['results'];
  }

  $header = array(
    array('data' => t('Webform'), 'field' => 'node_title'),
    array('data' => t('#'), 'field' => 'sid', 'sort' => 'desc'),
    array('data' => t('Submitted'), 'field' => 'submitted'),
    array('data' => t('User'), 'field' => 'name'),
    array('data' => t('IP Address'), 'field' => 'remote_addr'),
    array('data' => t('Operations'), 'colspan' => module_exists('print') ? 5 : 3),
  );

  $operation_column = end($header);
  $operation_total = $operation_column['colspan'];

  $rows = array();
  $count = count($submissions);
  foreach ($submissions as $sid => $submission) {
    $node = node_load($submission->nid);
    $row = array(
      l($node->title, 'node/' . $node->nid),
      $submission->is_draft ? t('@sid (draft)', array('@sid' => $sid)) : $sid,
      format_date($submission->submitted, 'short'),
    );
    if (webform_results_access($node, $user)) {
      $row[] = theme('username', array('account' => user_load($submission->uid)));
      $row[] = $submission->remote_addr;
    }
    $row[] = l(t('View'), "node/$node->nid/submission/$sid");
    $operation_count = 1;
    // No need to call this multiple times, just reference this in a variable.
    $destination = drupal_get_destination();
    if (webform_submission_access($node, $submission, 'edit', $user)) {
      $row[] = l(t('Edit'), "node/$node->nid/submission/$sid/edit", array('query' => $destination));
      $operation_count++;
    }
    if (webform_submission_access($node, $submission, 'delete', $user)) {
      $row[] = l(t('Delete'), "node/$node->nid/submission/$sid/delete", array('query' => $destination));
      $operation_count++;
    }
    if ($operation_count < $operation_total) {
      $row[count($row) - 1] = array('data' => $row[count($row) - 1], 'colspan' => $operation_total - $operation_count + 1);
    }
    $rows[] = $row;
  }

  // No harm in using whatever webform node came last as the $node here.
  $element['#node'] = !empty($node) ? $node : NULL;
  $element['#object'] = $object;
  $element['#submissions'] = $submissions;
  $element['#total_count'] = $count;
  $element['#pager_count'] = $pager_count;
  $element['#attached']['library'][] = array('webform', 'admin');

  $element['table']['#theme'] = 'table';
  $element['table']['#header'] = $header;
  $element['table']['#rows'] = $rows;
  $element['table']['#operation_total'] = $operation_total;

  // Let other modules modify this page, e.g. for ingestion.
  drupal_alter('iw_results_object_submissions_page_element', $element);

  return drupal_render($element);

}
