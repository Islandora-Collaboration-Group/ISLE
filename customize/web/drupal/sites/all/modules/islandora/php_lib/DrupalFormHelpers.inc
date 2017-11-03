<?php
// @codingStandardsIgnoreStart
/**
 * @file
 */
module_load_include('inc', 'php_lib', 'Array');

/**
 *
 * @param array $element
 * @param array $form
 */
function &get_form_element_parent(array $element, array &$form) {
  if (!empty($element['#array_parents'])) {
    array_pop($element['#array_parents']);
    $parent = &array_traverse_path($form, $element['#array_parents']);
    return $parent;
  }
  return NULL;
}

/**
 * Takes the value of the given property from the current element and propogates it to its child elements
 * if they have not already set that property.
 *
 * @param array $element
 * @param string $property
 */
function propagate_form_element_property(array &$element, $property) {
  $element[$property] = get_form_element_property($element, $property);
  foreach (element_children($element) as $key) {
    $child = &$element[$key];
    if (!isset($child[$property])) {

      // Don't squash an existing property value.
      $child[$property] = $element[$property];
    }
    propagate_form_element_property($child, $property);
  }
}

/**
 * Get the value of the given property if set, otherwise the default value for that property will be returned.
 *
 * @param array $element
 * @param string $property
 *
 * @return mixed
 */
function get_form_element_property(array &$element, $property) {
  $default_value = NULL;
  if ((!empty($element['#type'])) && ($info = _element_info($element['#type']))) {
    $default_value = $info[$property];
  }
  return isset($element[$property]) ? $element[$property] : $default_value;
}

/**
 *
 * @param array $element
 * @param string $type
 * @return array
 */
function get_element_children(array &$element, $type) {
  $children = element_children($element);
  foreach ($children as $key => $child) {
    if (empty($element[$child]['#type']) || $element[$child]['#type'] != $type) {
      unset($children[$key]);
    }
  }
  return $children;
}
// @codingStandardsIgnoreEnd
