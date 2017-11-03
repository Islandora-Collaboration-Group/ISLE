<?php

/**
 * @file
 * Holds derivative functionality for compound solution pack.
 */

/**
 * Derivative creation function for compound parent object TN.
 *
 * Wraps the TN update/creation function and
 * adds expected logging.
 *
 * @param AbstractObject $object
 *   The compound object.
 * @param bool $force
 *   Boolean for forcing this derivative creation.
 *
 * @return array
 *   Success message.
 */
function islandora_compound_object_create_thumbnail(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['TN'])) {
    $parts = islandora_compound_object_get_parts($object->id);
    if (!empty($parts)) {
      $child = islandora_object_load($parts[0]);
      try {
        islandora_compound_object_update_parent_thumbnail($object, $child);
      }
      catch (Exception $e) {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('Failed to update @dsid on @parent.'),
              'message_sub' => array(
                '@dsid' => 'TN',
                '@parent' => $object->id,
              ),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }
      return array(
        'success' => TRUE,
        'messages' => array(
          array(
            'message' => t('Updated @dsid on @parent.'),
            'message_sub' => array(
              '@dsid' => 'TN',
              '@parent' => $object->id,
            ),
            'type' => 'dsm',
            'severity' => 'status',
          ),
        ),
      );
    }
  }
}

/**
 * Update parent from child object.
 *
 * Given a child object, check to see if it is part of
 * a compound and update the parent's TN datastream.
 * Child objects of objects whose content model is NOT
 * islandora:compoundCModel will be ignored by this functionality.
 * Only islandora:compoundCModel parents will run a derivative to
 * check their children for TN sync. We add this check here to ensure
 * that the inverse is also true and the non-compound child don't try
 * to change their parents.
 *
 * @param AbstractObject $child
 *   The potential child object.
 * @param bool $force
 *   Boolean for forcing this derivative creation.
 *
 * @return array|null
 *   Success message.
 */
function islandora_compound_object_create_thumbnail_on_parent(AbstractObject $child, $force = TRUE) {
  // Test if child has a parent.
  $compound_info = islandora_compound_object_retrieve_compound_info($child);
  if (empty($compound_info)) {
    return NULL;
  }
  if ($force || !isset($compound_info['TN'])) {
    // Test if child is the first child of the compound.
    if (empty($compound_info['previous_pid'])) {

      $parent = islandora_object_load($compound_info['parent_pid']);
      // Ignore objects which aren't explicitly compound objects.
      if (!in_array('islandora:compoundCModel', $parent->models)) {
        return NULL;
      }

      // Child doesn't have a thumbnail then we can't set the parents TN either.
      if (empty($child['TN'])) {
        // Parents with first children with no TN shouldn't have one either.
        if (isset($parent['TN'])) {
          $parent->purgeDatastream('TN');
        }
        return NULL;
      }
      try {
        islandora_compound_object_update_parent_thumbnail($parent, $child);
      }
      catch (Exception $e) {
        return array(
          'success' => FALSE,
          'messages' => array(
            array(
              'message' => t('Failed to update @dsid on @parent.'),
              'message_sub' => array(
                '@dsid' => 'TN',
                '@parent' => $parent->id,
              ),
              'type' => 'watchdog',
              'severity' => WATCHDOG_ERROR,
            ),
          ),
        );
      }

      return array(
        'success' => TRUE,
        'messages' => array(
          array(
            'message' => t('Updated @dsid on @parent.'),
            'message_sub' => array(
              '@dsid' => 'TN',
              '@parent' => $parent->id,
            ),
            'type' => 'dsm',
            'severity' => 'status',
          ),
        ),
      );
    }
  }

}
