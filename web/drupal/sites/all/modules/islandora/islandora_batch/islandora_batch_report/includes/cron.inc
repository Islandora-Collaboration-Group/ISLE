<?php

/**
 * @file
 * Cron functions.
 */

/**
 * Email daily report to the list of email address set in the admin form.
 */
function islandora_batch_report_prepare_daily_email_report() {
  module_load_include('inc', 'islandora_batch_report', 'includes/db');
  module_load_include('inc', 'islandora_batch', 'includes/db');
  $timestamp = time();

  // XXX: An issue with rules module where it will not output 0 value because
  // the logic checks using empty() on the value it means: 0, "0", 0.00 will not
  // output. A patch was put in to fix the issue by switching the !empty to
  // isset https://www.drupal.org/node/2087109 if this gets merged in there is
  // no need to concatenate " " on each variable.  For output spacing
  // consistency always concatenate " " at the start.

  rules_invoke_event('islandora_batch_report_prepare_daily_report',
    $timestamp,
    " " . islandora_batch_report_get_daily_object_count(1, $timestamp),
    " " . islandora_batch_report_get_daily_object_count(0, $timestamp),
    " " . islandora_batch_report_get_daily_set_count(1, $timestamp),
    " " . islandora_batch_report_get_daily_set_count(0, $timestamp),
    islandora_batch_report_get_daily_object_count_by_content_model(1, $timestamp),
    islandora_batch_report_get_daily_object_count_by_content_model(0, $timestamp),
    " " . islandora_batch_get_count_of_queued_sets_ready_to_process(),
    " " . islandora_batch_get_count_of_queued_objects_ready_to_process()
  );
}
