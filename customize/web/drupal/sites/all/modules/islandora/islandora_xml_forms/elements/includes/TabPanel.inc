<?php

/**
 * @file
 * Allows forms to dynamically add new tabs to a tabpanel in a form.
 */

module_load_include('inc', 'php_lib', 'DrupalFormHelpers');

/**
 * Static functions that allow for theming and processing tabpanels.
 */
class TabPanel {

  /**
   * Constants
   */
  const ADD_BUTTON = 'add-tabpanel';
  const DELETE_BUTTON = 'delete-tabpanel';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const AddButton = self::ADD_BUTTON;
  const DeleteButton = self::DELETE_BUTTON;
  // @codingStandardsIgnoreEnd

  /**
   * TabPanel's theme hook.
   *
   * @param array $element
   *   The theme element.
   *
   * @return string
   *   A div string to pass back to theme.
   */
  public static function Theme($element) {
    $children = isset($element['#children']) ? $element['#children'] : '';
    $description = isset($element['#description']) ? "<div class='description'>{$element['#description']}</div>" : '';
    return "<div id='{$element['#hash']}'>{$description}{$children}</div>";
  }

  /**
   * The default #process function for tabpanel's.
   *
   * Adds elements that allow for adding/remove form elements.
   *
   * @param array $element
   *   The theme element.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $complete_form
   *   An optional complete form.
   *
   * @return array
   *   The processed element.
   */
  public static function Process(array $element, array &$form_state, array $complete_form = NULL) {
    // Defaults to TRUE.
    $add = isset($element['#user_data']['add']) ? $element['#user_data']['add'] : TRUE;
    if ($add) {
      $add_label = isset($element['#user_data']['add_label']) ? $element['#user_data']['add_label'] : t('Add');
      $element[self::ADD_BUTTON] = self::createAddButton($element, $complete_form, $add_label);
    }
    // Defaults to TRUE.
    $delete = isset($element['#user_data']['delete']) ? $element['#user_data']['delete'] : TRUE;
    if ($delete) {
      $delete_label = isset($element['#user_data']['delete_label']) ? $element['#user_data']['delete_label'] : t('Delete');
      $element[self::DELETE_BUTTON] = self::createRemoveButton($element, $complete_form, $delete_label);
    }
    return $element;
  }

  /**
   * Creates a button that allows tabpanels to duplicate.
   *
   * @param array $element
   *   The tabpanel form element.
   * @param array $complete_form
   *   The completed form.
   * @param string $label
   *   The label for the button.
   *
   * @return FormElement
   *   The "add" button.
   */
  protected static function createAddButton(array &$element, array &$complete_form, $label) {
    $tabs = get_form_element_parent($element, $complete_form);
    $add['#type'] = 'button';
    $add['#weight'] = 4;
    $add['#size'] = 30;
    $add['#id'] = $add['#name'] = $element['#hash'] . '-add';
    $add['#value'] = $label;
    $add['#prefix'] = '<div class="ui-tabpanel-add-button">';
    $add['#suffix'] = '</div>';
    $add['#limit_validation_errors'][] = $element['#parents'];
    $add['#ajax'] = array(
      'params' => array(
        'target' => $tabs['#hash'],
        'render' => $tabs['#hash'],
        'action' => 'add',
        'child' => $element['#hash'],
      ),
      'callback' => 'xml_form_elements_ajax_callback',
      // The parents wrapper is set to the parents hash.
      'wrapper' => $tabs['#hash'],
      'method' => 'replaceWith',
      'effect' => 'fade',
    );
    return $add;
  }

  /**
   * Creates a remove button that allows the user to remove this tabpanel.
   *
   * @param array $element
   *   The tabpanel form element.
   * @param array $complete_form
   *   The completed form.
   * @param string $label
   *   The label to use for the button.
   *
   * @return FormElement
   *   The "remove" button.
   */
  protected static function createRemoveButton(array &$element, array & $complete_form, $label) {
    $tabs = get_form_element_parent($element, $complete_form);
    $delete['#type'] = 'button';
    $delete['#weight'] = 4;
    $delete['#size'] = 30;
    $delete['#id'] = $delete['#name'] = $element['#hash'] . '-remove';
    $delete['#value'] = $label;
    $delete['#attributes'] = array(
      // Triggered via javascript.
      'style' => 'display:none',
    );
    $delete['#prefix'] = '<div class="ui-tabpanel-delete-button">';
    $delete['#suffix'] = '</div>';
    $delete['#limit_validation_errors'] = array();
    $delete['#ajax'] = array(
      'callback' => 'xml_form_elements_ajax_callback',
      'params' => array(
        'target' => $tabs['#hash'],
        'render' => $tabs['#hash'],
        'action' => 'delete',
        'child' => $element['#hash'],
      ),
      // The parent's wrapper is set to the parent's hash.
      'wrapper' => $tabs['#hash'],
      'method' => 'replaceWith',
      'effect' => 'fade',
    );
    return $delete;
  }

}

/**
 * Theme hook for a tabpanel.
 */
function theme_tabpanel($vars) {
  return TabPanel::Theme($vars['element']);
}
