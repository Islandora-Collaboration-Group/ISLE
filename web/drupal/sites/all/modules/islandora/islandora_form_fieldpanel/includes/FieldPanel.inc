<?php

/**
 * @file
 * Allows forms to dynamically add new fieldpanes to a fieldpanel in a form.
 */

/**
 * A collection of static functions.
 *
 * Allows for theming and processing fieldpanesls.
 */
class FieldPanel {

  /**
   * Constants.
   */
  const ADDBUTTON = 'add-fieldpane';
  const MOVEFIELDSET = 'move-fieldpane';

  /**
   * Loads the required resources for displaying the FieldPane element.
   *
   * @static var boolean $load
   *   Keeps us from loading the same files multiple times, while not required
   *   it just saves some time.
   */
  public static function addRequiredResources(array &$form_state) {
    static $load = TRUE;
    if ($load) {
      drupal_add_js(ISLANDORA_FORM_FIELDPANEL_PATH_JS . 'fieldpanel.js');
      drupal_add_css(ISLANDORA_FORM_FIELDPANEL_PATH_CSS . 'fieldpanel.css');
      $load = FALSE;
    }
  }

  /**
   * FieldPanel's theme hook.
   *
   * @param array $element
   *   The element.
   * @param array $form_state
   *   The form state.
   * @param array $complete_form
   *   The completed form.
   */
  public static function process(array $element, array &$form_state, array $complete_form = NULL) {
    self::addRequiredResources($form_state);
    // Defaults to TRUE.
    $add = isset($element['#user_data']['add']) ? $element['#user_data']['add'] : TRUE;
    $children = element_children($element);
    if ($add && !empty($children)) {
      $add_label = isset($element['#user_data']['add_label']) ? $element['#user_data']['add_label'] : t('Add');
      $element[self::ADDBUTTON] = self::createAddButton($element, $complete_form, $add_label);
    }
    if (count($children) > 1) {
      $element[self::MOVEFIELDSET] = self::createMoveFieldset($element, $complete_form, t('Move'));
    }
    $element['#prefix'] = "<div class='clear-block islandora-form-fieldpanel-container' id='{$element['#hash']}'>";
    $element['#suffix'] = '</div>';
    return $element;
  }

  /**
   * Creates a button that allows fieldpanes to duplicate.
   *
   * @param array $element
   *   The element.
   * @param array $complete_form
   *   The completed form.
   * @param string $label
   *   The label.
   *
   * @return FormElement
   *   The processed form element.
   */
  private static function createAddButton(array &$element, array &$complete_form, $label) {
    $children = element_children($element);
    $child = $element[array_pop($children)];

    $add['#type'] = 'button';
    $add['#weight'] = 4;
    $add['#size'] = 30;
    $add['#id'] = $add['#name'] = $element['#hash'] . '-add';
    $add['#attributes'] = array('class' => array('fieldpanel-add'));
    $add['#value'] = $label;
    $add['#prefix'] = '<div class="ui-fieldpane-add-button">';
    $add['#suffix'] = '</div>';
    $add['#ajax'] = array(
      'params' => array(
        'target' => $element['#hash'],
        'render' => $element['#hash'],
        'action' => 'add',
        'child' => $child['#hash'],
      ),
      'callback' => 'xml_form_elements_ajax_callback',
      // The parents wrapper is set to the parents hash.
      'wrapper' => $element['#hash'],
      'method' => 'replaceWith',
      'effect' => 'fade',
    );
    $add['#limit_validation_errors'] = array();
    return $add;
  }

  /**
   * Creates a fieldset to move FieldPane elements.
   *
   * @param array $element
   *   The element.
   * @param array $complete_form
   *   The completed form.
   * @param string $label
   *   The label.
   *
   * @return FormElement
   *   The processed form element.
   */
  private static function createMoveFieldset(array &$element, array &$complete_form, $label) {
    $children = element_children($element);
    $options = array();
    $counter = 0;
    foreach ($children as $child) {
      if (is_numeric($child)) {
        $options[$child] = $counter;
        if ($counter == 0) {
          $options[$child] = 'Top';
        }
        $counter++;
      }
    }
    $child = $element[array_shift($children)];

    $move = array(
      '#type' => 'fieldset',
      '#title' => $label,
      '#id' => $element['#hash'] . '-swap-fieldset',
      '#attributes' => array('class' => array('fieldpanel-swap-fieldset')),
      '#name' => $element['#hash'] . '-swap-fieldset',
      '#description' => t('Move element to position. All elements at that position (and after) will be moved one step down'),
    );
    $move['move-element'] = array(
      '#type' => 'select',
      '#title' => t('Element Number'),
      '#attributes' => array('class' => array('fieldpanel-swap-fieldset-move-element')),
      '#options' => $options,
      '#default_value' => '0',
    );
    $move['move-position'] = array(
      '#type' => 'select',
      '#attributes' => array('class' => array('fieldpanel-swap-fieldset-move-position')),
      '#title' => t('Position Number'),
      '#options' => $options,
      '#default_value' => '0',
    );
    $move['move-op'] = array(
      '#type' => 'button',
      '#id' => $element['#hash'] . '-move',
      '#name' => $element['#hash'] . '-move',
      '#value' => $label,
      '#limit_validation_errors' => array($element['#parents']),
      '#ajax' => array(
        'params' => array(
          'target' => $element['#hash'],
          'render' => $element['#hash'],
          'action' => 'move',
          'child' => $child['#hash'],
        ),
        'callback' => 'xml_form_elements_ajax_callback',
        'wrapper' => $element['#hash'],
        'method' => 'replaceWith',
        'effect' => 'fade',
      ),
    );
    return $move;
  }

  /**
   * A function to filter children of a fieldpane.
   *
   * @param array $child
   *   The child element.
   */
  public static function filterChildren(array $child) {
    $ret = ($child['#type'] == 'fieldpane') ? TRUE : FALSE;
    return $ret;
  }

}

/**
 * Implements hook_preprocess_fieldpanel().
 */
function template_preprocess_fieldpanel(&$vars) {
  $tabs = $vars['element'];
  // Header Info.
  $vars['collapsible'] = $tabs['#collapsible'] == TRUE;
  $vars['collapsed'] = $tabs['#collapsed'] == TRUE;
  // Get Fieldpanel.
  $keys = element_children($tabs);
  $children = array();

  foreach ($keys as $key) {
    $children[$key] = &$tabs[$key];
  }
  $children = array_filter($children, array('FieldPanel', 'filterChildren'));
  // First element has different icons, and title.
  $first = array_shift($children);
  $vars['title'] = isset($first['#title']) ? $first['#title'] : 1;
  $vars['link'] = $first['#hash'];
  // Remaining fieldpane.
  $count = 2;
  foreach ($children as $child) {
    $title = isset($child['#title']) ? $child['#title'] : $count;
    $delete = isset($child[FieldPane::DELETEBUTTON]) ? $child[FieldPane::DELETEBUTTON]['#id'] : FALSE;
    $vars['fieldpane'][] = array($title, $child['#hash'], $delete);
    $count++;
  }
  // Content Info.
  if (isset($tabs['#children'])) {
    $vars['content'] = $tabs['#children'];
  }
  else {
    $parents = $tabs['#array_parents'];
    $name = array_pop($parents);
    $vars['content'] = '';
    drupal_set_message(t('Attempted to create tab %name without defining any child fieldpanes.', array('%name' => $name)), 'error');
  }
}
