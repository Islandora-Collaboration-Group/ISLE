<?php

/**
 * @file
 * Implementation of Rules hooks, giving default configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function islandora_book_batch_default_rules_configuration() {
  $configs = array();

  $rule = rules_reaction_rule();

  $rel_params = array(
    // Select the event variable for our event (islandora_book_batch_ingested)
    'subject:select' => 'book-object',
    // Define for ISLANDORA_RELS_EXT_URI isn't available if Tuque isn't loaded.
    'pred_uri' => 'http://islandora.ca/ontology/relsext#',
    'pred' => 'email-admin-when-ingested',
    'object' => 'true',
    'type' => 1,
  );

  $rule->label = 'E-mail admin';
  $rule->active = TRUE;
  $rule
    ->event('islandora_book_batch_ingested')
    ->condition('islandora_object_has_relationship', $rel_params)
    ->action('mail', array(
      'to' => '[site:mail]',
      'subject' => '[[site:name]] "[book-object:label]" has been ingested',
      'message' => '[book-object:label] has been ingested as [book-object:id].',
    ))
    ->action('islandora_object_remove_relationship', $rel_params);

  $configs['islandora_book_batch_notify_admin'] = $rule;

  return $configs;
}
