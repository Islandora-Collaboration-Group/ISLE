<?php

/**
 * @file
 * Rules hook implementations.
 */

/**
 * Implements hook_rules_event_info().
 */
function islandora_book_batch_rules_event_info() {
  $events = array();

  $events['islandora_book_batch_ingested'] = array(
    'group' => t('Islandora Book'),
    'label' => t('Book ingested'),
    'variables' => array(
      'book_object' => array(
        'type' => 'islandora_object',
        'label' => t('The ingested book'),
        'description' => t('A Tuque object for the ingested book, as an entity.'),
      ),
    ),
  );

  return $events;
}
