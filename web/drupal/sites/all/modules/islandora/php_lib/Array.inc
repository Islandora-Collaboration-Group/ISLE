<?php
// @codingStandardsIgnoreStart
/**
 * @file
 */

/**
 * Recusively copy's an array cloning any references. Perserves Keys, Objects shallow copy unless they implement __clone.
 */
function array_copy_recursive(array &$array) {
  $output = array();
  foreach ($array as $key => $value) {
    if (is_array($value)) {
      $output[$key] = array_copy_recursive($value);
    }
    if (is_object($value)) {
      $output[$key] = clone $value;
    }
    else {

      // Default type copyed by default.
      $output[$key] = $value;
    }
  }
  return $output;
}

/**
 *
 * @param array $array
 * @param string or int or object... $key
 * @param string or int or object... $value
 */
function array_add(array &$array, $key, $value) {
  if (isset($key)) {
    $array[$key] = $value;
  }
  else {
    $array[] = $value;
  }
}

/**
 * Given an array of index's this functions attempts a depth first traversal of the given array
 * using the provided indices.
 *
 * @param array $form
 * @param array $indices
 *
 * @return mixed
 *   A reference to the value at the final index if found, FALSE otherwise.
 */
function &array_traverse_path(array &$array, array $indices) {
  $point = &$array;
  foreach ($indices as $index) {
    if (empty($point[$index])) {
      $result = FALSE;
      return $result;
    }
    $point = &$point[$index];
  }
  return $point;
}

/**
 *
 * @param array $array
 * @param string $type
 * @return array
 */
function array_filter_type(array &$array, $type) {
  $filtered = array();
  foreach ($array as $item) {
    if ($item instanceof $type) {
      $filtered[] = $item;
    }
  }
  return $filtered;
}

/**
 *
 * @param array $array
 */
function is_non_empty_array($array) {
  return is_array($array) && count($array) > 0;
}

/**
 *
 * @param string or int or object... $object
 * @return type
 */
function object_as_array($object) {
  $array = array();
  if (is_object($object) || is_array($object)) {
    foreach ($object as $key => $value) {
      $array[$key] = is_object($value) ? object_as_array($value) : $value;
    }
    return $array;
  }
  return $object;
}

/**
 * Returns the first element without modifying the array.
 *
 * @param array $array
 */
function array_peek(array $array) {
  reset($array);
  list($key, $value) = each($array);
  reset($array);
  return array($key, $value);
}
// @codingStandardsIgnoreEnd
