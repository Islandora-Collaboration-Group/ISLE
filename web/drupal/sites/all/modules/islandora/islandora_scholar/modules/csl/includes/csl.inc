<?php

/**
 * @file
 * Implements a class for interacting with the stored CSL files.
 */

class CSL {
  /**
   * The name of the table where the CSL styles are stored.
   */
  const TABLE = 'csl';

  /**
   * Checks to see if the given CSL style is a valid definition.
   *
   * @param string/DOMDocument $style_in
   *   A CSL 1.0 style file to check to see if it is valid.
   *
   * @return bool
   *   TRUE if the given style is valid, FALSE otherwise.
   */
  public static function Valid($style_in) {
    $schema_filename = drupal_get_path('module', 'csl') . '/schema/csl.rng';
    $style = NULL;
    if ($style_in instanceof DOMDocument) {
      $style = $style_in;
    }
    else {
      $style = new DOMDocument();
      $style->loadXML($style_in);
    }
    // We don't want to show any of this errors to the end users, but we do
    // want to log them, so we prevent Drupal from catching errors.
    libxml_use_internal_errors(TRUE);
    if (!($valid = $style->relaxNGValidate($schema_filename))) {
      self::loglibxmlErrors();
    }
    // Allow Drupal to catch errors.
    libxml_use_internal_errors(TRUE);
    return $valid;
  }

  /**
   * Log any xml errors to the watchdog.
   */
  protected static function loglibxmlErrors() {
    $errors = libxml_get_errors();
    foreach ($errors as $error) {
      $levels = array(
        LIBXML_ERR_WARNING => WATCHDOG_WARNING,
        LIBXML_ERR_ERROR => WATCHDOG_ERROR,
        LIBXML_ERR_FATAL => WATCHDOG_ERROR,
      );
      $severity = $levels[$error->level];
      watchdog('csl', 'code: %code message: %message file: %file line: %line column: %column', array(
        '%code' => $error->code,
        '%message' => $error->message,
        '%file' => $error->file,
        '%line' => $error->line,
        '%column' => $error->column,
      ), $severity);
    }
    libxml_clear_errors();
  }

  /**
   * Checks to see if the given CSL style exists in the database.
   *
   * @param string $style_name
   *   The name of the CSL style.
   *
   * @return bool
   *   TRUE if the given style exists, FALSE otherwise.
   */
  public static function Exists($style_name) {
    $results = db_select(self::TABLE, 't')
      ->fields('t')
      ->condition('name', $style_name)
      ->execute();
    $count = $results->rowCount();
    return $count == 1;
  }

  /**
   * Gets a list of all the availible styles.
   *
   * @return array
   *   An array of defined style's names, where both the key and the value are
   *   the style's name. EX. array('name' => 'name')
   */
  public static function GetNames() {
    $names = array();
    $results = db_select(self::TABLE, 't')
      ->fields('t', array('name'))
      ->execute();
    foreach ($results as $data) {
      $names[$data->name] = $data->name;
    }
    return $names;
  }

  /**
   * Sets the given style name to default if it exists.
   *
   * @param string $style_name
   *   The name of the style to set to the default.
   */
  public static function SetDefaultName($style_name) {
    if (self::Exists($style_name)) {
      variable_set('csl_default_style', $style_name);
    }
  }

  /**
   * Gets the default style name.
   *
   * If it has not been previously set it will be the first  name from
   * self::GetNames().
   *
   * @return string
   *   The default style name
   */
  public static function GetDefaultName() {
    $names = self::GetNames();
    $default = variable_get('csl_default_style', array_shift($names));
    return $default;
  }

  /**
   * Gets the CSL style definition identified by name.
   *
   * @param string $style_name
   *   The name of the CSL style.
   * @param bool $as_string
   *   Returns the CSL as a string (default is a DOMDocument)
   *
   * @return mixed
   *   The CSL style definition if found, FALSE otherwise.
   */
  public static function Get($style_name, $as_string = FALSE) {
    if (self::Exists($style_name)) {
      $results = db_select(self::TABLE, 't')
        ->fields('t', array('style'))
        ->condition('name', $style_name)
        ->execute();

      foreach ($results as $data) {
        $xml = trim($data->style);
        if (!empty($xml)) {
          $definition = NULL;
          if (!$as_string) {
            $definition = new DOMDocument();
            $definition->loadXML($xml);
          }
          else {
            $definition = $xml;
          }
          return $definition;
        }
      }
    }
    return FALSE;
  }

  /**
   * Gets the default style.
   *
   * @param bool $as_string
   *   Return a string if TRUE, a DOMDocument if FALSE.
   *
   * @return mixed
   *   Returns the default style as a string or DOMDocument depending on the
   *   $as_string argument.
   */
  public static function GetDefault($as_string = FALSE) {
    $style_name = self::GetDefaultName();
    return self::Get($style_name, $as_string);
  }

  /**
   * Creates a CSL style with the given style name and definition.
   *
   * If the CSL style already exists it will fail.
   *
   * @param string $style_name
   *   The name of the CSL style.
   * @param mixed $definition
   *   A CSL style definition. (DOMDocument or string)
   *
   * @return bool
   *   TRUE if successful, otherwise FALSE.
   */
  public static function Create($style_name, $definition) {
    if (!self::Exists($style_name)) {
      if (!self::Valid($definition)) {
        drupal_set_message(t("The @style is not valid CSL 1.0.1 file. Problems may occur.", array(
          '@style' => $style_name,
        )), 'warning');
      }
      $object = new stdClass();
      $object->name = $style_name;
      if ($definition instanceof DOMDocument) {
        $object->style = $definition->saveXML();
      }
      else {
        // Assume a string.
        $object->style = trim($definition);
      }
      return drupal_write_record(self::TABLE, $object) !== FALSE;
    }
    return FALSE;
  }

  /**
   * Deletes the style with the given name.
   *
   * @param string $style_name
   *   The name of the CSL style definition.
   *
   * @return bool
   *   TRUE if successful FALSE otherwise.
   */
  public static function Delete($style_name) {
    if (self::Exists($style_name)) {
      db_delete(self::TABLE)
        ->condition('name', $style_name)
        ->execute();
      return TRUE;
    }
    return FALSE;
  }

}
