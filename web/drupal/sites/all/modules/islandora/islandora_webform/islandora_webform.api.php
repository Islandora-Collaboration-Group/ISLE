<?php

/**
 * Modify islandora webform object submissions page.
 *
 * This hook enables modules to modify the output of the
 * islandora_webform_results_object_submissions_page function, which generates
 * the content that appears on the the submissions tab for an islandora/fedora
 * object.
 * The $element variable is passed by reference and may be modified by this
 * hook.
 *
 * @param $element
 *   A renderable array for the page content. It includes...
 *    - #node: the webform object
 *    - #object: the fedora object
 *    - #submissions: an array of webform submission records
 *    - table: a renderable array of the submission results
 */
function hook_iw_results_object_submissions_page_element_alter(&$element) {
  $element['table']['#prefix'] = '<h2>Blah blah</h2>';
}

/**
 * Modify islandora inline webform ajax commands.
 *
 * This hook permits modules to modify the ajax commands that set behavior
 * when an inline webform link is clicked when webform_ajax is enabled.
 * The $commands array variable is passed by reference and may be modified by
 * this hook.
 *
 * @param &$commands
 *   An array of ajax commands. Provided commands have the following keys:
 *     'remove-container',
 *     'remove-wrapper-class'
 *     'add-wrapper-class'
 *     'jquery-selector'
 *   Look at the _iw_inline_webform() function for details.
 *
 * @param array $vars
 *   An array of useful data, indexed as follows:
 *   'webform' object
 *     The webform node
 *   'form' array
 *     The render array for the webform
 *   'html' string
 *     The rendered html of that webform
 *
 */
function hook_iw_inline_webform_ajax_commands_alter(&$commands, $vars) {

  // We're going to have the webform appear in a different place on the page.
  $jquery_selector = '#my_inline_webform_ajax_target_' . $vars['webform']->nid;
  $commands['jquery-selector'] = ajax_command_after($jquery_selector, $vars['html']);
}

/**
 * Modify islandora webform link markup.
 *
 * @param $webform_link
 *   Html for the webform link.
 *
 * @param $islandora_webform_record
 */
function hook_islandora_webform_theme_webform_link_alter(&$webform_link, $islandora_webform_record) {
  // Example: Display the help text above the link.
  if(!empty($islandora_webform_record->link_help)) {
    $webform_link = '<div class="webform-link-label">'. $islandora_webform_record->link_help . '</div><div class="webform-link">' . $webform_link . '</div>';
  }
}