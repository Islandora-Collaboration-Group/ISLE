<?php

/**
 * @file
 * This file documents all available hook functions to manipulate data.
 */

/**
 * Get the list of users to notify about embargo events.
 *
 * @param AbstractObject $object
 *   The object to display
 *
 * @return array
 *   An array whose values are Drupal user objects.
 */
function hook_islandora_scholar_embargo_users_to_notify(AbstractObject $object) {
  // Example: notify some IR administrators if it's a citation.
  if (in_array('ir:citationCModel', $object->models)) {
    $admin_users = array(
      'ir_administrative_user',
      'some_other_ir-ish_person',
    );
    return array_map('user_load_by_name', $admin_users);
  }
}

/**
 * Alter the list of users to notify about embargo events.
 *
 * @param array $users
 *   The list of user objects to notify, passed in by reference.
 * @param AbstractObject $object
 *   The object to display.
 */
function hook_islandora_scholar_embargo_users_to_notify_alter(array &$users, AbstractObject $object) {
  // Example: don't notify the anonymous user if the object is a citation.
  if (in_array('ir:citationCModel', $object->models)) {
    $filter_anonymous = function($user) {
      return $user->name !== 'anonymous';
    };
    $users = array_filter($users, $filter_anonymous);
  }
}
