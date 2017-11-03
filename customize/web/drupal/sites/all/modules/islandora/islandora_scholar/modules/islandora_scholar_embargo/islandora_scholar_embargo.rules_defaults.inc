<?php
/**
 * @file
 * Holds default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function islandora_scholar_embargo_default_rules_configuration() {
  $datastreams_expired_rule_export = file_get_contents(drupal_get_path('module', 'islandora_scholar_embargo') . '/rules/notify.rule');
  $datastreams_expired_rule = rules_import($datastreams_expired_rule_export);
  $object_expired_rule = rules_reaction_rule();
  $object_expired_rule->event('islandora_scholar_object_embargo_expired')
    ->action(
      'islandora_scholar_embargo_lift_embargo'
    );
  return array(
    'islandora_scholar_embargo_datastreams_lift_expired_embargo' => $datastreams_expired_rule,
    'islandora_scholar_embargo_object_lift_expired_embargo' => $object_expired_rule,
  );
}
