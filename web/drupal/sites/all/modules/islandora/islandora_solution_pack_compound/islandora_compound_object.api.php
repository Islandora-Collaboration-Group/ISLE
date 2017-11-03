<?php

/**
 * @file
 * API documentation.
 */

/**
 * Notify other modules about the addition of objects to a compound.
 *
 * @param array $objects
 *   An array of AbstractObjects added to the parents, to produce compounds.
 * @param array $parent_pids
 *   An array of PIDs representing the parents to which each object was added,
 *   to make each member of $objects a member of multiple compounds.
 */
function hook_islandora_compound_object_children_added_to_parent($objects, $parent_pids) {

}

/**
 * Notify other modules about the removal of objects from a compound.
 *
 * @param array $object
 *   An array of AbstractObjects removed from the parents.
 * @param array $parent_pids
 *   An array of PIDs representing the parents to which each object was added,
 *   to make each member of $objects a member of multiple compounds.
 */
function hook_islandora_compound_object_children_removed_from_parent($object, $parent_pids) {

}

/**
 * Registry menu paths for the management controls for compound to appear on.
 */
function hook_islandora_compound_object_management_control_paths() {
  return array(
    'islandora/object/%/your/menu/path/here',
  );
}

/**
 * Describe query "backends" which might be selected between.
 *
 * We have a number of different mechanisms which might be used to generate
 * lists of PIDs belonging to given compound objects, so let us roll an
 * interface so we can talk to them in a consistent manner.
 *
 * @return array
 *   Should return an associative array mapping unique (module-prefixed,
 *   preferably) keys to associative arrays containing:
 *   - title: A human-readable title for the backend.
 *   - description: A description of the backend.
 *   - callable: A PHP callable to call for this backend, implementing
 *     callback_islandora_compound_object_query_backends().
 *   - file: An optional file to load before attempting to call the callable.
 */
function hook_islandora_compound_object_query_backends() {
  $a_callable = function ($pid, $ret_title) {
    // Do something to get the total number of objects
  };
  return array(
    'awesome_backend' => array(
      'title' => t('Awesome Backend'),
      'callable' => $a_callable,
    ),
  );
}

/**
 * Return an array of pids that are part of a compound object.
 *
 * Callback for hook_islandora_compound_object_query_backends().
 *
 * @param String $pid
 *   A compound object for which to obtain members.
 *
 * @return array
 *   An array of arrays sorted in the order the compound objects should be
 *   displayed with the PID of the compound object member as the array key for
 *   each entry and the array for each compound member containing:
 *   - pid: The PID of the object.
 *   - title: The title of the object.
 *   - seq: The sequence number of the object.
 */
function callback_islandora_compound_object_get_parts($pid) {
  return array(
    'islandora:123' => array(
        'pid' => 'islandora:123',
        'title' => 'objects title',
        'seq' => 60,
    ),
  );
}
