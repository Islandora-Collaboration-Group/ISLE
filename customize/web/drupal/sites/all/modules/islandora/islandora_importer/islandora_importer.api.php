<?php

/**
 * @file
 * This file documents all available hook functions to manipulate data.
 */

/**
 * Hook to register importers.
 *
 * @return array
 *   An associative array mapping unique machine names to associative arrays
 *   containing the following keys:
 *   - title: A translated title,
 *   - class: A string containing the class name for your importer.
 *   Title is used as the label, and the class is one which extends
 *   IslandoraBatchImporter (defined in islandora_importer.inc; just
 *   implementing the abstract methods and using your custom "item class" should
 *   suffice). Please note that the class must be autoloaded.
 */
function hook_islandora_importer() {
  return array(
    'my_awesome_importer' => array(
      'title' => t('My Awesome Importer Module'),
      'class' => 'MAIM',
    ),
  );
}

/**
 * Alter hook to hide/update those available for user selection.
 *
 * @param array $options
 *   The array of options resulting from calling hook_islandora_importer().
 *
 * @see hook_islandora_importer()
 */
function hook_islandora_importer_alter(array &$options) {
  unset($options['my_replaced_importer']);
}
