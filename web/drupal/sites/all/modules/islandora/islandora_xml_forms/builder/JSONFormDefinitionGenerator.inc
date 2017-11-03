<?php

/**
 * @file
 * Provides a class that generates the required JSON for Form Builder GUI.
 */

module_load_include('inc', 'php_lib', 'String');

/**
 * Generates two JSON strings that can be used by the Form Builder GUI.
 */
class JSONFormDefinitionGenerator {

  /**
   * Creates an array that contains two JSON strings.
   *
   * These define the form properties and the form.
   *
   * @param array $properties
   *   The form properties.
   * @param array $form
   *   A Drupal form.
   *
   * @return array
   *   An array where the first value is a JSON string that describes the form
   *   properties and the second value is a JSON string that describes the
   *   Drupal form, e.g. array($properties, $form).
   */
  public static function Create(array &$properties, array &$form) {
    $properties = self::GetProperties($properties);
    $form = self::GetForm($form);
    return array($properties, $form);
  }

  /**
   * Gets the Form Properties as a JSON string.
   *
   * @param array $properties
   *   The Form Properties.
   *
   * @return string
   *   The JSON representation of the Form Properties.
   */
  public static function GetProperties(array $properties) {
    $namespaces = isset($properties['namespaces']) ? $properties['namespaces'] : array();
    $properties = array(
      'localName' => $properties['root_name'],
      'uri' => isset($properties['default_uri']) ? $properties['default_uri'] : NULL,
      'namespaces' => $namespaces,
      'schema' => isset($properties['schema_uri']) ? $properties['schema_uri'] : NULL,
    );
    return json_encode(array($properties));
  }

  /**
   * Gets the Drupal Form as a JSON string.
   *
   * @param array $form
   *   The Drupal Form.
   *
   * @return string
   *   The JSON representation of the Drupal Form.
   */
  public static function GetForm(array $form) {
    $elements = array(
      'key' => 'Root',
      'text' => 'Elements',
      'children' => array(),
    );
    $elements = self::GetElement($form);
    $elements['key'] = empty($elements['key']) ? 'Root' : $elements['key'];
    $elements['text'] = "{$elements['key']} ({$elements['type']})";
    return json_encode(array('expanded' => TRUE, 'children' => $elements));
  }

  /**
   * Gets a Drupal Form Element as a JSON string.
   *
   * @param array $element
   *   A Drupal Form Element.
   * @param string $identifier
   *   An optional identifier for the element.
   *
   * @return array
   *   An array that represents a ExtJS JSON object that represents the Drupal
   *   Form Element.
   */
  protected static function GetElement(array $element, $identifier = NULL) {
    $json = array();
    foreach (element_properties($element) as $key) {
      // Remove the #.
      $name = trim($key, '#');
      $json[$name] = $element[$key];
    }
    $json['children'] = array();
    foreach (element_children($element) as $key) {
      $json['children'][] = self::GetElement($element[$key], $key);
    }
    // These two are ExtJS specific.
    $json['key'] = $identifier;
    $json['text'] = "{$identifier} ({$element['#type']})";
    return $json;
  }

}
