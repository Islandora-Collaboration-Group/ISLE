<?php

/**
 * @file
 * Implementation of Rules hooks, giving default configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function islandora_newspaper_batch_default_rules_configuration() {
  $configs = array();
  $parse_exported = '{ "islandora_newspaper_batch_newspaper_issue_ingested" : {
    "LABEL" : "Newspaper Issue Ingested",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "islandora" ],
    "ON" : { "islandora_object_ingested" : [] },
    "IF" : [
      { "list_contains" : {
          "list" : [ "object:models" ],
          "item" : "islandora:newspaperIssueCModel"
        }
      },
      { "islandora_object_has_relationship" : {
          "subject" : [ "object" ],
          "pred_uri" : "http:\/\/islandora.ca\/ontology\/relsext#",
          "pred" : "newspaper-batched",
          "object" : "true",
          "type" : "1"
        }
      },
      { "islandora_object_has_relationship" : {
          "subject" : [ "object" ],
          "pred_uri" : "http:\/\/islandora.ca\/ontology\/relsext#",
          "pred" : "email-admin-when-ingested",
          "object" : "true",
          "type" : "1"
        }
      }
    ],
    "DO" : [
      { "mail" : {
          "to" : "[site:mail]",
          "subject" : "[[site:name]] \u0022[object:label]\u0022 has been ingested",
          "message" : "[object:label] has been ingested as [object:id].",
          "language" : [ "" ]
        }
      },
      { "islandora_object_remove_relationship" : {
          "subject" : [ "object" ],
          "pred_uri" : "http:\/\/islandora.ca\/ontology\/relsext#",
          "pred" : "email-admin-when-ingested",
          "object" : "true",
          "type" : "1"
        }
      }
    ]
  }
}';
  $parsed_parse = rules_import($parse_exported);
  $configs[$parsed_parse->name] = $parsed_parse;

  $complete_exported = '{ "islandora_newspaper_batch_newspaper_issue_batch_complete" : {
    "LABEL" : "Newspaper Issue Batch Complete",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "10",
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "islandora" ],
    "ON" : { "islandora_object_ingested" : [] },
    "IF" : [
      { "list_contains" : {
          "list" : [ "object:models" ],
          "item" : "islandora:newspaperIssueCModel"
        }
      },
      { "islandora_object_has_relationship" : {
          "subject" : [ "object" ],
          "pred_uri" : "http:\/\/islandora.ca\/ontology\/relsext#",
          "pred" : "newspaper-batched",
          "object" : "true",
          "type" : "1"
        }
      }
    ],
    "DO" : [
      { "islandora_object_remove_relationship" : {
          "subject" : [ "object" ],
          "pred_uri" : "http:\/\/islandora.ca\/ontology\/relsext#",
          "pred" : "newspaper-batched",
          "object" : "true",
          "type" : "1"
        }
      }
    ]
  }
}';
  $parsed_complete = rules_import($complete_exported);
  $configs[$parsed_complete->name] = $parsed_complete;
  return $configs;
}
