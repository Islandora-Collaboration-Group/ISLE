<?php

/**
 * @file
 * Converts form properties into objects.
 */

/**
 * A collection of static functions to help with processing for properties.
 */
class FormProperty {

  /**
   * Asks all the installed modules if they define any FormProperty classes.
   *
   * @staticvar array $cache
   *   For each request $cache the results.
   *
   * @return array
   *   An array of information required to instantiate FormProperty classes
   *   where the names of the form properties are the keys in the array.
   */
  public static function GetRegisteredFormPropertiesTypes() {
    static $cache;
    if (empty($cache)) {
      $cache = array();
      foreach (module_implements('objectify_properties') as $module) {
        $properties = module_invoke($module, 'objectify_properties');
        if (isset($properties) && is_array($properties)) {
          $cache = array_merge_recursive($cache, $properties);
        }
      }
    }
    return $cache;
  }

  /**
   * Checks if a given form property has a registered class.
   *
   * @param string $name
   *   Name of the FormProperty we are looking for.
   *
   * @return bool
   *   TRUE if there is a registered class for the given form property.
   */
  public static function IsRegisteredFormProperty($name) {
    $properties = self::GetRegisteredFormPropertiesTypes();
    return isset($properties[$name]);
  }

  /**
   * Gets required information for loading and creating a given FormProperty.
   *
   * @param string $name
   *   Name of the FormProperty we are looking for.
   *
   * @return array
   *   The required information for loading and creating a given FormProperty if
   *   defined for the property $name.
   */
  public static function GetProperty($name) {
    if (self::IsRegisteredFormProperty($name)) {
      $properties = self::GetRegisteredFormPropertiesTypes();
      return $properties[$name];
    }
    return NULL;
  }

  /**
   * Loads the file where the FormProperty class exists.
   *
   * If the FormProperty type is not defined it does nothing.
   *
   * @param string $name
   *   Name of the FormProperty to create.
   */
  public static function LoadFile($name) {
    $property = self::GetProperty($name);
    if (isset($property)) {
      module_load_include($property['type'], $property['module'], $property['name']);
    }
  }

  /**
   * Creates a FormProperty object and initializes it with the provided $value.
   *
   * @param string $name
   *   Name of the FormProperty to create.
   * @param mixed $value
   *   Value to initialize the generated FormProperty object with.
   *
   * @return object
   *   The class that repersents the given form property.
   */
  public static function Create($name, $value) {
    $property = self::GetProperty($name);
    if (isset($property)) {
      $class = $property['class'];
      return new $class($value);
    }
    return NULL;
  }

  /**
   * Creates a FormProperty Object if one is defined by $name.
   *
   * @param string $name
   *   Name of the FormProperty to create.
   * @param mixed $value
   *   The value of the form property.
   *
   * @return mixed
   *   If a FormProperty class is defined for $name, then a FormProperty object
   *   is returned, otherwise the parameter $value is returned.
   */
  public static function Expand($name, $value) {
    if (self::IsRegisteredFormProperty($name)) {
      self::LoadFile($name);
      return self::Create($name, $value);
    }
    return $value;
  }
}
