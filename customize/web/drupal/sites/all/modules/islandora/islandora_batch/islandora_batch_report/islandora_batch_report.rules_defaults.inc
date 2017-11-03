<?php

/**
 * @file
 * Default Rules implementations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function islandora_batch_report_default_rules_configuration() {
  $configs = array();
  $configs['islandora_batch_report_email_report'] = rules_import('
    { "islandora_batch_report_email_report" : {
        "LABEL" : "islandora_batch_report_email_report",
        "PLUGIN" : "reaction rule",
        "OWNER" : "rules",
        "REQUIRES" : [ "rules", "islandora_batch_report" ],
        "ON" : { "islandora_batch_report_prepare_daily_report" : [] },
        "DO" : [
          { "mail" : {
              "to" : "[site:mail]",
              "subject" : "Daily Islandora Batch Ingest Report - [report-date-timestamp:value:custom:m\/d\/Y]",
              "message" : "On [report-date-timestamp:value:custom:M jS], [successful-object-total:value] objects were ingested:\r\n[content-model-successful-ingest-breakdown:value]\r\n[successful-set-total:value] sets were successfully processed.\r\n[failed-set-total:value] sets failed processing.\r\n\r\nThere are still:\r\n[ready-to-process-sets:value] sets that remain to be processed.\r\n[ready-to-process-objects:value] objects that remain to be processed.",
              "language" : "en"
            }
          }
        ]
      }
    }
  ');
  return $configs;
}
