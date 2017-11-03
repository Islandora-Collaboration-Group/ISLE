<?php

/**
 * @file
 * The Element class.
 */

module_load_include('inc', 'xml_form_api', 'Utils');
module_load_include('inc', 'xml_form_api', 'XMLForm');
module_load_include('inc', 'objective_forms', 'Utils');
module_load_include('inc', 'objective_forms', 'FormPopulator');

/**
 * A collection of generic static functions that help in constructing elements.
 */
class Element {

  /**
   * Includes the given jQuery UI widgets.
   *
   * @param array $files
   *   A list of files to be included from the jquery theme, or a single file
   *   name.
   */
  public static function addUIWidgets($files = array()) {
    // TODO $files needs to be a string or multiple calls to drupal_add_library
    // need to be made.
    drupal_add_library('system', $files);
  }

  /**
   * Includes the given jQuery UI theme files.
   *
   * @param array $files
   *   A list of files to be included from the jquery theme, or a single file
   *   name.
   */
  public static function addUIThemeStyles($files = array()) {
    self::addFiles('drupal_add_css', XML_FORM_ELEMENTS_JQUERY_THEME_PATH, $files, array(
      'group' => CSS_THEME,
    ));
  }

  /**
   * Includes the given javascript files from this module.
   *
   * @param string[] $files
   *   The array of javascript files to add.
   */
  public static function addJS($files = array()) {
    self::addFiles('drupal_add_js', XML_FORM_ELEMENTS_JS_PATH, $files);
  }

  /**
   * Includes the given css files from this module.
   *
   * @param string[] $files
   *   The array of CSS files to add.
   */
  public static function addCSS($files = array()) {
    self::addFiles('drupal_add_css', XML_FORM_ELEMENTS_CSS_PATH, $files, array(
      'group' => CSS_THEME,
    ));
  }

  /**
   * Adds files using the given $method, $path and $files.
   *
   * @param string $function
   *   The function to call to add the given files.
   * @param string $path
   *   The path where the files are located.
   * @param array $files
   *   The file(s) to be added.
   * @param mixed $additional_argument
   *   Any potential additional arguments required by the $function.
   */
  protected static function addFiles($function, $path, $files = array(), $additional_argument = NULL) {
    // Convert string to array.
    $files = (array) $files;
    foreach ($files as $file) {
      $function($path . $file, $additional_argument);
    }
  }
}
