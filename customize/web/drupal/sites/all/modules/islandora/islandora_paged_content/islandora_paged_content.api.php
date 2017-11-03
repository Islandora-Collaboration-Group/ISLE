<?php

/**
 * @file
 * This file documents all available hook functions to manipulate data.
 */

/**
 * Identifies content models whose children are paged content.
 *
 * @note
 *   The children content model PIDs currently map to an empty array as there
 *   may be room for expansion in the future.
 * @return array
 *   An array mapping the parent paged content model to an array of keyed
 *   content model arrays containing:
 *   - parents: An associative array mapping applicable content models to
 *     predicates by which the current entry might be related.
 *     Not strictly needed if it belongs to a collection.
 *   - children: An array containing arrays of keyed content model children.
 */
function hook_islandora_paged_content_content_model_registry() {
  return array(
    'somecmodel' => array(
      'parents' => array(
        'islandora:bookCModel' => 'isMemberOfCollection',
      ),
      'children' => array(
        'somecmodel' => array(),
      ),
    ),
  );
}

/**
 * Allows one to alter the page management tabs.
 *
 * @param array $manage_tabs
 *   The renderable array of tabs to alter.
 * @param array $context
 *   An array containing the current object under the key 'object'.
 *
 * @see islandora_paged_content_manage_page_menu()
 */
function hook_islandora_paged_content_page_management_tabs_alter(&$manage_tabs, $context) {
  unset($manage_tabs['manage_page']['pdf']);
}

/**
 * Allows one to alter the paged content pages management tabs.
 *
 * @param array $manage_tabs
 *   The renderable array of tabs to alter.
 * @param array $context
 *   An array containing the current object under the key 'object'.
 *
 * @see islandora_paged_content_manage_pages_menu()
 */
function hook_islandora_paged_content_pages_management_tabs_alter(&$manage_tabs, $context) {
  unset($manage_tabs['manage_pages']['pdf']);
}
