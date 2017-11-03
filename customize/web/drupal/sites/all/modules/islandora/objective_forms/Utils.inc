<?php

/**
 * @file
 * Utility functions.
 */

module_load_include('inc', 'php_lib', 'Array');

/**
 * Recursively searches the Drupal form for the element with the given hash.
 *
 * @param array $form
 *   The Drupal Form.
 *
 * @param hash $hash
 *   The unique #hash property that identifies the FormElement.
 */
function &find_element(array &$form, $hash) {
  if (has_hash($form, $hash)) {
    return $form;
  }
  return find_child_element($form, $hash);
}

/**
 * Function find_child_elements.
 *
 * Recursively searches the Drupal Form elements children for the element that
 * matches the given hash.
 *
 * @param array $form
 *   A Drupal Form Element.
 * @param hash $hash
 *   The unique #hash property that identifies the FormElement.
 *
 * @return array
 *   The drupal form element whos #hash matches that of the given $hash value.
 */
function &find_child_element(array &$form, $hash) {
  $found = NULL;
  $children = element_children($form);
  foreach ($children as $key) {
    $child = &$form[$key];
    $found = has_hash($child, $hash) ? $child : find_child_element($child, $hash);
    if (isset($found)) {
      return $found;
    }
  }
  return $found;
}

/**
 * Functin has_hash.
 *
 * Checks to see if the given drupal form element has a #hash property and
 * checks to see if it matches the given hash.
 *
 * @param array $element
 *   A Drupal Form Element.
 * @param hash $hash
 *   The unique #hash property that identifies the FormElement.
 *
 * @return bool
 *   TRUE if the given element's hash matches, FALSE otherwise.
 */
function has_hash(array &$element, $hash) {
  if (isset($element['#hash']) && $element['#hash'] == $hash) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Gets the drupal form element name for the given element if possible.
 *
 * Only works on processed form elements.
 *
 * The name of the form element is based on the #parents property of your form
 * element. If it is array('foo', 'bar', 'baz') then 'foo][bar][baz' is the
 * elements name.
 *
 * @param array $element
 *   The Drupal form element.
 *
 * @return string
 *   The Drupal style name for the given form element if possible, FALSE
 *   otherwise.
 */
function form_element_name(array $element) {
  if (isset($element['#parents'])) {
    return implode('][', $element['#parents']);
  }
  return FALSE;
}

/**
 * Function form_element_is_child.
 *
 * Compares two Drupal form element names to determine if the second
 * argument is a child element of the first.
 *
 * @param string $parent
 *   A Drupal form element name for the assumed parent form element.
 *
 * @param string $child
 *   A Drupal form element name for the assumed child form element.
 *
 * @return bool
 *   TRUE if the given element
 */
function form_element_is_child($parent, $child) {
  return preg_match('/^' . preg_quote($parent) . '/', $child) != 0;
}
