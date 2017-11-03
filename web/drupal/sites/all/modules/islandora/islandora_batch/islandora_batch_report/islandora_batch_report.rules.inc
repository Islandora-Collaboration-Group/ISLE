<?php

/**
 * @file
 * Rule hook implementations.
 */

/**
 * Implements hook_rules_event_info().
 */
function islandora_batch_report_rules_event_info() {
  return array(
    'islandora_batch_report_prepare_daily_report' => array(
      'group' => t('Islandora Batch Report'),
      'label' => t('Prepare Daily Report'),
      'variables' => array(
        'report_date_timestamp' => array(
          'type' => 'date',
          'label' => t('The unix timestamp of the date that the report data relates to.'),
        ),
        'successful_object_total' => array(
          'type' => 'text',
          'label' => t('The count of successfully ingested batch objects.'),
        ),
        'failed_object_total' => array(
          'type' => 'text',
          'label' => t('The count of failed to ingest batch objects.'),
        ),
        'successful_set_total' => array(
          'type' => 'text',
          'label' => t('The count of successfully ingested batch sets.'),
        ),
        'failed_set_total' => array(
          'type' => 'text',
          'label' => t('The count of failed to ingest batch sets.'),
        ),
        'content_model_successful_ingest_breakdown' => array(
          'type' => 'text',
          'label' => t('A breakdown by content model of the totals of successfully ingested batch objects.'),
        ),
        'content_model_failed_ingest_breakdown' => array(
          'type' => 'text',
          'label' => t('A breakdown by content model of the totals of failed to ingest batch objects.'),
        ),
        'ready_to_process_sets' => array(
          'type' => 'text',
          'label' => t('The count of batch sets that remain and are ready to be processed.'),
        ),
        'ready_to_process_objects' => array(
          'type' => 'text',
          'label' => t('The count of remaining batch objects that are ready to be processed. Note: Includes objects waiting for parents.'),
        ),
      ),
    ),
  );
}
