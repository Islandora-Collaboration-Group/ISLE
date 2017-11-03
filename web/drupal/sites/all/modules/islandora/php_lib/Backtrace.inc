<?php
// @codingStandardsIgnoreStart
/**
 * @file
 * Functions for determining call order and such...
 */

/**
 * This function is only intended for use within this file.
 *
 * Convenience function used to account for the current function in the call stack.
 * The caller assumes their function is index 0, so add one to account for this function.
 *
 * @param int $index
 *   Index to modify to account for the callee in the call stack.
 */
function _ignore_this_in_call_stack($index = NULL) {
  $base_index = 1;
  return (empty($index)) ? $base_index : $index + $base_index;
}

/**
 * Gets the call stack properties at the specifed index.
 *
 * @param int $index
 *
 * @return array
 *   Return the call stack properties for the given index if found. Otherwise NULL is returned.
 */
function get_call_stack_index($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  $call_stack = debug_backtrace();
  if (isset($call_stack[$index])) {
    return $call_stack[$index];
  }
  return NULL;
}

/**
 * Gets the given property of the caller at the specified index in the call stack.
 *
 * @param int $index
 *
 * @param string $property
 *   A call stack property. Expected to be one of the following.
 *      function    -> The current function name.
 *      line ->    The current line number.
 *      file -> The current file name.
 *      class    -> The current class name.
 *      object -> The current object.
 *      type -> The current call type.
 *      args -> The arguments to the current function.
 */
function get_caller_property($index, $property) {
  $index = _ignore_this_in_call_stack($index);
  $call_stack = get_call_stack_index($index);
  if (isset($call_stack) && isset($call_stack[$property])) {
    return $call_stack[$property];
  }
  return NULL;
}

/**
 * Gets the caller method at the given index.
 *
 * The caller of this function's index is assumed to be 0.
 *
 * @param int $index
 *
 * @return string
 */
function get_caller_function($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  return get_caller_property($index, 'function');
}

/**
 * Gets the line of the caller method at the given index.
 *
 * The caller of this function's index is assumed to be 0.
 *
 * @param int $index
 *
 * @return int
 */
function get_caller_line($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  return get_caller_property($index, 'line');
}

/**
 * Gets the name of file where the callee is located.
 *
 * @param int $index
 *
 * @return string
 */
function get_caller_file($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  return get_caller_property($index, 'file');
}

/**
 * Gets the class of the object used to make the method call at the specified index.
 *
 * @param int $index
 *
 * @return string
 */
function get_caller_class($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  return get_caller_property($index, 'class');
}

/**
 * Gets the object used to make the method call at the specified index.
 *
 * @param int $index
 *
 * @return object
 *   If defined the object used to make the call is returned otherwise NULL is returned.
 */
function get_caller_object($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  return get_caller_property($index, 'object');
}

/**
 * Gets the call type of the caller at the specified index.
 *
 * @param int $index
 *
 * @return string
 *   If a method call, "->" is returned. If a static method call, "::" is returned.
 *   If a function call, nothing is returned.
 */
function get_caller_type($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  return get_caller_property($index, 'type');
}

/**
 * Gets the function arguments for the function at the specified index in the call stack.
 *
 * @param int $index
 *
 * @return array
 *   If inside a function, this lists the functions arguments. If inside an included file,
 *   this lists the included file name(s).
 */
function get_caller_args($index = NULL) {
  $index = _ignore_this_in_call_stack($index);
  return get_caller_property($index, 'args');
}
// @codingStandardsIgnoreEnd
