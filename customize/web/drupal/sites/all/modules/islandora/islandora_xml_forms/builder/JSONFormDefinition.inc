<?php

/**
 * @file
 * Provides a class that represents a Form with JSON.
 */

module_load_include('inc', 'php_lib', 'String');
module_load_include('inc', 'php_lib', 'Array');
module_load_include('inc', 'php_lib', 'Misc');

/**
 * Given some JSON, it retrieves the form properties and a Drupal form.
 */
class JSONFormDefinition {

  /**
   * Instantiates the JSONForm definition.
   *
   * @param string $json
   *   The posted encoded JSON data.
   */
  public function __construct($json) {
    // In this case, $json equates to $_POST['data'].
    $this->json = json_decode($json);
  }

  /**
   * Gets both the form properties and the form.
   *
   * @return array
   *   An array where the first key is the form properties and the second is the
   *   form, e.g. array($properties, $form)
   */
  public function getPropertiesAndForm() {
    return array($this->getProperties(), $this->getForm());
  }

  /**
   * Gets the Form Properties.
   *
   * @return array
   *   The form properties.
   */
  public function getProperties() {
    $properties = $this->json->properties;
    $namespaces = object_as_array($properties->namespaces);
    return array(
      'root_name' => $properties->localName,
      'schema_uri' => $properties->schema,
      'namespaces' => $namespaces,
      'default_uri' => $properties->uri,
    );
  }

  /**
   * Gets the Drupal representation of this JSON form.
   *
   * @return array
   *   A Drupal form.
   */
  public function getForm() {
    $root = $this->json->elements[0];
    list($key, $value) = $this->getElement($root);
    return $value;
  }

  /**
   * Gets the array representation of an element from the JSON Form Definition.
   *
   * @param object $element
   *   A JSON representation of a Drupal Form Element.
   *
   * @return array
   *   A Drupal representation of the form element.
   */
  protected function getElement(stdClass $element) {
    $key = isset($element->key) ? $element->key : NULL;
    $properties = $this->getElementProperties($element);
    $children = $this->getElementChildren($element);
    return array($key, array_merge($properties, $children));
  }

  /**
   * Gets an element's properties from the JSON Form Definition as an array.
   *
   * @param object $element
   *   A JSON representation of a Drupal Form Element.
   *
   * @return array
   *   A Drupal representation of form element's properties.
   */
  protected function getElementProperties(stdClass $element) {
    $ignore = array(
      'key',
      'text',
      'root',
      'leaf',
      'id',
      'parentId',
      'icon',
      'cls',
      'qtip',
      'qtitle',
      'iconCls',
      'href',
      'hrefTarget',
      'elements',
      'index',
      'depth',
      'expanded',
      'expandable',
      'checked',
      'isLast',
      'isFirst',
      'allowDrop',
      'allowDrag',
      'loaded',
      'loading',
    );
    $controls = array();
    foreach ($element as $key => $value) {
      // Is it a control, or should the element be ignored?
      if (array_search($key, $ignore) === FALSE) {
        // Is the given property valid?
        if ($this->validateProperty($key, $value)) {
          $controls["#$key"] = (is_object($value) || is_array($value)) ?
              $this->transformElementProperties((array) $value) :
              cast_string_to_type($value);
        }
      }
    }
    return $controls;
  }

  /**
   * Converts explicit types to their native representation.
   *
   * @param array $property
   *   The array of properties to convert.
   *
   * @return array
   *   A converted array of native representations.
   */
  protected function transformElementProperties(array $property) {
    $array = array();
    foreach ($property as $key => $value) {
      $array[cast_string_to_type($key)] = (is_object($value) || is_array($value)) ?
          $this->transformElementProperties((array) $value) :
          cast_string_to_type($value);
    }
    return $array;
  }

  /**
   * Checks if the given property and its value are valid.
   *
   * This may be cast to the wayside at a later time.
   *
   * @param string $name
   *   The name of the element property.
   * @param mixed $value
   *   The value of the element property.
   *
   * @return bool
   *   TRUE if the property name and its value are valid.
   */
  protected function validateProperty($name, $value) {
    $controls = array(
      'type' => 'is_non_empty_string',
      'access' => 'is_bool',
      'after_build' => 'is_non_empty_array',
      'ahah' => 'is_non_empty_array',
      'attributes' => 'is_non_empty_array',
      'autocomplete_path' => 'is_non_empty_string',
      'button_type' => 'is_non_empty_string',
      'collapsed' => 'is_bool',
      'collapsible' => 'is_bool',
      'cols' => 'is_numeric',
      'default_value' => 'is_non_empty_string',
      'delta' => 'is_numeric',
      'description' => 'is_non_empty_string',
      'disabled' => 'is_bool',
      'element_validate' => 'is_non_empty_array',
      'executes_submit_callback' => 'is_bool',
      'field_prefix' => 'is_non_empty_string',
      'field_suffix' => 'is_non_empty_string',
      'maxlength' => 'is_numeric',
      'method' => 'is_non_empty_string',
      'multiple' => 'is_bool',
      'name' => 'is_non_empty_string',
      'options' => 'is_non_empty_array',
      'post_render' => 'is_non_empty_array',
      'prefix' => 'is_non_empty_string',
      'pre_render' => 'is_non_empty_array',
      'process' => 'is_non_empty_array',
      'required' => 'is_bool',
      'resizable' => 'is_bool',
      'return_value' => 'is_non_empty_string',
      'rows' => 'is_numeric',
      'size' => 'is_numeric',
      'src' => 'is_non_empty_string',
      'submit' => 'is_non_empty_array',
      'suffix' => 'is_non_empty_string',
      'theme' => 'is_non_empty_string',
      'title' => 'is_non_empty_string',
      'tree' => 'is_bool',
      'validate' => 'is_non_empty_array',
      'value' => 'is_non_empty_string',
      'weight' => 'is_numeric',
      'user_data' => 'is_non_empty_array',
      'actions' => 'is_non_empty_array',
    );
    $value = is_object($value) ? object_as_array($value) : $value;
    if (isset($controls[$name])) {
      return call_user_func($controls[$name], $value);
    }
    // Assume valid for non specified controls.
    return TRUE;
  }

  /**
   * Gets an array of the child elements of the given element.
   *
   * @param object $element
   *   A JSON representation of a Drupal Form Element.
   *
   * @return array
   *   A Drupal representation of form element's child form elements.
   */
  protected function getElementChildren(stdClass $element) {
    $children = array();
    if (isset($element->elements)) {
      foreach ($element->elements as $child) {
        list($key, $value) = $this->getElement($child);
        array_add($children, $key, $value);
      }
    }
    return $children;
  }

}
