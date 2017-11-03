<?php
// @codingStandardsIgnoreStart
/**
 * @file
 * A collection of functions that help with reflection like properties.
 */

/**
 * Checks to see if the given variable has a class.
 *
 * @param mixed $var
 *
 * @return boolean
 */
function has_class($var) {
  return isset($var) && is_object($var);
}

/**
 * Check to is if the object matches the given type.
 *
 * @param object $obj
 * @param string $type
 *
 * @return boolean
 */
function is_type($obj, $type) {
  return has_class($obj) && get_class($obj) == $type;
}

/**
 * Checks if the object is a decendant of the provided type.
 *
 * @param object $child
 * @param string $type
 *
 * @return boolean
 */
function is_descendant($obj, $type) {
  $class = has_class($obj) ? get_class($obj) : NULL;
  if ($class) {
    $reflection = new ReflectionClass($class);
    return $reflection->isSubclassOf($type);
  }
  return FALSE;
}

/**
 * Checks to see if the object matches the given type, or if the object descends from the given type.
 *
 * @param object $obj
 * @param string $type
 *
 * @return boolean
 */
function is_or_descends_from($obj, $type) {
  return is_type($obj, $type) || is_descendant($obj, $type);
}

/**
 *
 * @param mixed $obj
 * @param string $interface
 * @return boolean
 */
function has_interface($obj, $interface) {
  if (has_class($obj)) {
    $implements = class_implements(get_class($obj));
    return isset($implements[$interface]);
  }
  return FALSE;
}
// @codingStandardsIgnoreEnd
