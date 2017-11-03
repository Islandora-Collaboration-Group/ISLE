<?php

/**
 * @file
 * Defines callbacks and resources needed for the 'datepicker' form element.
 */

class Datepicker {

  /**
   * Loads the required resources for displaying the Datepicker element.
   *
   * @staticvar bool $load
   *   Keeps us from loading the same files multiple times; while not required,
   *   it just saves some time.
   */
  public static function addRequiredResources(&$form_state) {
    static $load = TRUE;
    if ($load) {
      form_load_include($form_state, 'inc', 'xml_form_elements', 'includes/Element');
      // @todo: add Element::addRequiredResources().
      Element::addUIWidgets('ui.datepicker');
      Element::addUIThemeStyles(array(
        'ui.core.css',
        'ui.datepicker.css',
        'ui.theme.css',
      ));
      Element::addJS('datepicker.js');
      $load = FALSE;
    }
  }

  /**
   * The '#process' callback for the datepicker form element.
   *
   * @param array $element
   *   The datepicker form element.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $complete_form
   *   The complete Drupal form definition.
   *
   * @return array
   *   The datepicker form element.
   */
  public static function process(array $element, array &$form_state, array $complete_form = NULL) {
    self::addRequiredResources($form_state);
    return $element;
  }

}

/**
 * Implements theme_hook().
 *
 * @param array $variables
 *   An array of theming variables.
 */
function theme_datepicker_xml_form_elements($variables) {
  $element = $variables['element'];
  $array_switch = is_array($element['#value']) && isset($element['#value']['value']);
  $element_val = $array_switch ? $element['#value']['value'] : $element['#value'];
  $size = empty($element['#size']) ? '' : ' size="' . $element['#size'] . '"';
  $maxlength = empty($element['#maxlength']) ? '' : ' maxlength="' . $element['#maxlength'] . '"';
  $class = array('form-text', 'datepicker');
  $output = '';
  _form_set_class($element, $class);
  if (isset($element['#field_prefix'])) {
    $output .= '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ';
  }
  $output .= '<input type="text"' . $maxlength . ' name="' . $element['#name'] . '" id="' . $element['#id'] . '"' . $size . ' value="' . check_plain($element_val) . '"' . drupal_attributes($element['#attributes']) . ' />';
  if (isset($element['#field_suffix'])) {
    $output .= ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>';
  }
  return $output;
}
