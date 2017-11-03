<?php

/**
 * @file
 * Provides a wrapper class around getting forms from the database or hooks.
 */

module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');

class XMLFormRepository extends XMLFormDatabase {

  /**
   * Returns forms defined by hooks in modules.
   *
   * @return array[]
   *   An array where the keys are the form names, paired with values which are
   *   also arrays, in the format "array('form_file' => 'path/to/the/form')".
   */
  protected static function getFormsFromHook() {
    $hooks = module_invoke_all('xml_form_builder_forms');
    // @todo Remove (deprecated) invokation if
    // "islandora_xml_form_builder_forms".
    $hooks += module_invoke_all('islandora_xml_form_builder_forms');
    return $hooks;
  }

  /**
   * Checks to see if the given form exists.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   *
   * @return bool
   *   TRUE if the given form exists, FALSE otherwise.
   */
  public static function Exists($form_name) {
    $in_database = parent::Exists($form_name);

    if ($in_database) {
      return TRUE;
    }

    $forms = self::getFormsFromHook();
    if (isset($forms[$form_name])) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Checks to see if the given form exists and is a valid definition.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   *
   * @return bool
   *   TRUE if the given form exists, FALSE otherwise.
   */
  public static function Valid($form_name) {
    $in_database = parent::Exists($form_name);

    if ($in_database) {
      return parent::Valid($form_name);
    }

    return self::Get($form_name) !== FALSE;
  }

  /**
   * Gets the XML Form Definition identified by name.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   *
   * @return DOMDocument
   *   The XML Form Definition if found, FALSE otherwise.
   */
  public static function Get($form_name) {
    $in_database = parent::Exists($form_name);
    if ($in_database) {
      return parent::Get($form_name);
    }

    $hooks = self::getFormsFromHook();
    if (!isset($hooks[$form_name])) {
      return FALSE;
    }

    $path = $hooks[$form_name]['form_file'];
    if (!file_exists($path)) {
      return FALSE;
    }

    module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
    $definition = new DOMDocument();
    $definition->load($path);
    $version = XMLFormDefinition::getVersion($definition);
    if (!$version->isLatestVersion()) {
      $definition = XMLFormDefinition::upgradeToLatestVersion($definition);
    }
    return $definition;
  }

  /**
   * Gets a list of all defined form names.
   *
   * @return array
   *   An array of defined form names, where both the key and the value are the
   *   form's name, e.g. array('name' => 'name').
   */
  public static function GetNames() {
    $hook = self::getFormsFromHook();
    $hook_names = array();
    foreach ($hook as $key => $array) {
      $hook_names[] = array('name' => $key, 'indb' => FALSE);
    }
    usort($hook_names, array('XMLFormRepository', 'ComparisonFunction'));

    $db_names = parent::GetNames();
    usort($hook_names, array('XMLFormRepository', 'ComparisonFunction'));

    $names = array_merge($hook_names, $db_names);

    return $names;
  }

  /**
   * Compares the strings inside the 'name' key for two arrays.
   *
   * @param array $a
   *   The first array to use in the comparison; must contain a 'name' key.
   * @param array $b
   *   The second array to use in the comparison; must contain a 'name' key.
   *
   * @return int
   *   The string comparison as a strnatcasecmp() integer.
   */
  public static function ComparisonFunction($a, $b) {
    return strnatcasecmp($a['name'], $b['name']);
  }

  /**
   * Gets a list of all defined form names that have valid definitions.
   *
   * @return array
   *   An array of defined form names, where both the key and the value are the
   *   form's name, e.g. array('name' => 'name').
   */
  public static function GetValidNames() {
    $form_names = self::GetNames();
    $valid_names = array();
    foreach ($form_names as $form_name) {
      if (self::Valid($form_name['name'])) {
        $valid_names[] = $form_name;
      }
    }
    return $valid_names;
  }

  /**
   * Creates a form with the given form name and definition.
   *
   * If the form already exists it will fail.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   * @param DOMDocument $definition
   *   A XML Form Definition.
   *
   * @return bool
   *   TRUE if successful, otherwise FALSE.
   */
  public static function Create($form_name, DOMDocument $definition = NULL) {
    if (!self::Exists($form_name)) {
      return parent::Create($form_name, $definition);
    }
    return FALSE;
  }

  /**
   * Copies an existing form.
   *
   * @param string $form_name_src
   *   The name of the source form to copy from.
   * @param string $form_name_dest
   *   The name of the destination form which gets copied to.
   *
   * @return bool
   *   TRUE if successful FALSE otherwise.
   */
  public static function Copy($form_name_src, $form_name_dest) {
    if (self::Exists($form_name_src)) {
      $definition = self::Get($form_name_src);
      return self::Create($form_name_dest, $definition);
    }
    return FALSE;
  }
}
