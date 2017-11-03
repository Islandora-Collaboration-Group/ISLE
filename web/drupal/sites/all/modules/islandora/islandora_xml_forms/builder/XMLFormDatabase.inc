<?php

/**
 * @file
 * Provides a wrapper class around the database table were forms are stored.
 */

class XMLFormDatabase {

  /**
   * Constants
   */
  const TABLE = 'xml_forms';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const table = self::TABLE;
  // @codingStandardsIgnoreEnd

  /**
   * Checks to see if the given form exists in the database.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   *
   * @return bool
   *   TRUE if the given form exists, FALSE otherwise.
   */
  public static function Exists($form_name) {
    // @TODO: integrate 'fancy' 'dynamic' self::table
    $count = db_select('xml_forms', 'xf')
      ->condition('xf.name', $form_name)
      ->fields('xf', array('name'))
      ->execute()
      ->rowCount();

    return $count == 1;
  }

  /**
   * Checks to see if the given form exists in the database and is valid.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   *
   * @return bool
   *   TRUE if the given form exists, FALSE otherwise.
   */
  public static function Valid($form_name) {
    if (self::Exists($form_name)) {
      return self::Get($form_name) !== FALSE;
    }
    return FALSE;
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
    if (self::Exists($form_name)) {
      $query = db_query('SELECT form FROM {xml_forms} WHERE name = :name', array(':name' => $form_name));
      $xml = $query->fetchField();
      if (trim($xml) !== '') {
        $definition = new DOMDocument();
        $definition->loadXML($xml);
        $definition = self::Upgrade($form_name, $definition);
        return $definition;
      }
    }
    return FALSE;
  }

  /**
   * Upgrade to the latest version and save it back to the database.
   *
   * @param string $form_name
   *   The name of the form to update.
   * @param DOMDocument $definition
   *   The form definition to update.
   *
   * @return XMLFormDefinition
   *   The form definition, upgraded to the latest version.
   */
  protected static function Upgrade($form_name, DOMDocument $definition) {
    module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
    $version = XMLFormDefinition::getVersion($definition);
    if (!$version->isLatestVersion()) {
      $definition = XMLFormDefinition::upgradeToLatestVersion($definition);
      // Updates to latest.
      self::Update($form_name, $definition);
    }
    return $definition;
  }

  /**
   * Gets a list of all defined form names.
   *
   * @return array
   *   An array of defined form names, where both the key and the value are the
   *   form's name; e.g. array('name' => 'name').
   */
  public static function GetNames() {
    $names = array();
    $result = db_query('SELECT name FROM {xml_forms}')->fetchCol();
    foreach ($result as $data) {
      $names[] = array('name' => $data, 'indb' => TRUE);
    }
    return $names;
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
    foreach ($form_names as $key => $form_name) {
      if (self::Valid($form_name)) {
        $valid_names[$key] = $form_name;
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
   *   An XML Form Definition.
   *
   * @return bool
   *   TRUE if successful, otherwise FALSE.
   */
  public static function Create($form_name, DOMDocument $definition = NULL) {
    if (!self::Exists($form_name)) {
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      $definition = isset($definition) ? $definition : xml_form_api_get_empty_form_definition();
      $definition = XMLFormDefinition::upgradeToLatestVersion($definition);
      $object = new stdClass();
      $object->name = $form_name;
      $object->form = $definition->saveXML();
      return drupal_write_record(self::TABLE, $object) !== FALSE;
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
   *   TRUE if successful; FALSE otherwise.
   */
  public static function Copy($form_name_src, $form_name_dest) {
    if (self::Exists($form_name_src)) {
      $definition = self::Get($form_name_src);
      return self::Create($form_name_dest, $definition);
    }
    return FALSE;
  }

  /**
   * Updates the form with the given form name; the previous definition is lost.
   *
   * If the form does not exist, this function will fail.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   * @param DOMDocument $definition
   *   A XML Form Definition.
   *
   * @returns bool
   *   TRUE if successful; FALSE otherwise.
   */
  public static function Update($form_name, DOMDocument $definition) {
    if (self::Exists($form_name)) {
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      $version = XMLFormDefinition::getVersion($definition);
      if (!$version->isLatestVersion()) {
        $definition = XMLFormDefinition::upgradeToLatestVersion($definition);
      }
      $num = db_update(self::TABLE)
        ->fields(array('form' => $definition->saveXML()))
        ->condition('name', $form_name)
        ->execute();
      if ($num) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    return FALSE;
  }

  /**
   * Deletes the form with the given form name.
   *
   * @param string $form_name
   *   The name of the XML Form Definition.
   *
   * @return bool
   *   TRUE if successful; FALSE otherwise.
   */
  public static function Delete($form_name) {
    if (self::Exists($form_name)) {
      $num = db_delete(self::TABLE)
        ->condition('name', $form_name)
        ->execute();
      if ($num) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    return FALSE;
  }

}
