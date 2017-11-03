<?php

/**
 * @file
 * Class for form tabs.
 */

module_load_include('inc', 'xml_form_elements', 'includes/TabPanel');

class Tabs {

  /**
   * Loads the required resources for displaying the Tabs element.
   *
   * @param array $form_state
   *   The Drupal form state.
   *
   * @staticvar bool $load
   *   Keeps us from loading the same files multiple times. While not required
   *   it just saves some time.
   */
  public static function addRequiredResources(array &$form_state) {
    static $load = TRUE;
    if ($load) {
      form_load_include($form_state, 'inc', 'xml_form_elements', 'includes/Element');
      Element::addUIWidgets('ui.tabs');
      Element::addUIThemeStyles(array(
        'ui.core.css',
        'ui.tabs.css',
        'ui.theme.css',
      ));
      Element::addJS('tabs.js');
      Element::addCSS('tabs.css');
      $load = FALSE;
    }
  }

  /**
   * Processes the element.
   *
   * @param array $element
   *   The tabs element.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $complete_form
   *   The completed form.
   *
   * @return array
   *   The processed tabs element.
   */
  public static function process(array $element, array &$form_state, array $complete_form = NULL) {
    self::addRequiredResources($form_state);
    $element['#prefix'] = "<div class='clear-block' id='{$element['#hash']}'>";
    $element['#suffix'] = '</div>';
    return $element;
  }

  /**
   * Checks if a child element has the 'tabpanel' #type.
   *
   * @param array $child
   *   The child to determine.
   *
   * @return bool
   *   TRUE if it is a tabpanel, FALSE otherwise.
   */
  public static function FilterChildren(array $child) {
    $ret = ($child['#type'] == 'tabpanel') ? TRUE : FALSE;
    return $ret;
  }

}

/**
 * Pre-process tabs theme hook.
 *
 * @param array $vars
 *   Theme variables.
 */
function template_preprocess_tabs(&$vars) {
  $tabs = $vars['element'];
  // Header Info.
  $vars['collapsible'] = $tabs['#collapsible'] == TRUE;
  $vars['collapsed'] = $tabs['#collapsed'] == TRUE;
  // Get Tabpanels.
  $keys = element_children($tabs);
  $children = array();

  foreach ($keys as $key) {
    $children[$key] = &$tabs[$key];
  }
  $children = array_filter($children, array('Tabs', 'FilterChildren'));
  // First element has different icons, and title.
  $first = array_shift($children);
  $vars['title'] = isset($first['#title']) ? $first['#title'] : 1;
  $vars['link'] = $first['#hash'];
  // Remaining tabpanels.
  $count = 2;
  foreach ($children as $child) {
    $title = isset($child['#title']) ? $child['#title'] : $count;
    $delete = isset($child[TabPanel::DELETE_BUTTON]) ? $child[TabPanel::DELETE_BUTTON]['#id'] : FALSE;
    $vars['tabpanels'][] = array($title, $child['#hash'], $delete);
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
    drupal_set_message(t('Attempted to create tab %name without defining any child tabpanels.', array('%name' => $name)), 'error');
  }
}
