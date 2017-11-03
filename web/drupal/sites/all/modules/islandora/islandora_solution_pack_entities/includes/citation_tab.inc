<?php

/**
 * @file
 * Creates citation tab.
 */

/**
 * Builds citation chooser.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object with related citations.
 * @param string $type
 *   citation or thesis
 *
 * @return array
 *   The Drupal form.
 */
function islandora_entities_citation_form($form, $form_state, $object, $type = 'theses') {
  module_load_include('inc', 'islandora_entities', 'includes/utilities');
  if (module_exists('citation_exporter')) {
    if (CitationExporter::ReadyToExport()) {
      $citation_export_head = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
          'http-equiv' => 'refresh',
          'content' => '1.0;' . url(CITATION_EXPORTER_EXPORT_MENU, array('absolute' => TRUE)),
        ),
      );
      drupal_add_html_head($citation_export_head, 'citation_exporter_head');
    }
  }

  if ($object['MADS']) {
    $mads = $object['MADS']->content;
    $simplexml = simplexml_load_string($mads);
    $identifiers = $simplexml->identifier;
    foreach ($identifiers as $identifier) {
      // PHP 5.3 and 5.4 can't handle empty((string) $identifier).
      $string_id = (string) $identifier;
      if ($identifier['type'] == 'u1' && !empty($string_id)) {
        $results = islandora_entities_get_related_pids($string_id, (string) $simplexml->authority->titleInfo->title, $type);
      }
    }
    $lines = array();
    $has_entries = empty($results) ? FALSE : TRUE;
    if ($has_entries) {
      foreach ($results as $pid => $data) {
        $object = islandora_object_load($pid);
        $label = l($object->label, 'islandora/object/' . $object->id);
        $output = array();
        foreach ($data['authors'] as $author) {
          $author = preg_replace('/\((.*?)\)/', '', $author);

          if (trim($author)) {
            $output[] = $author;
          }
        }
        $authors = implode('; ', $output);
        $lines[$object->id] = array('title' => $label, 'authors' => $authors);
      }
    }
  }
  $header = array(
    'title' => t('Title'),
    'authors' => t('Authors'),
  );
  $form['#tree'] = TRUE;
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );
  $form['citations'] = array(
    '#type' => 'tableselect',
    '#title' => t('Citations'),
    '#header' => $header,
    '#options' => $lines,
    '#attributes' => '',
    '#empty' => t("There are no @type.", array('@type' => $type)),
  );
  if ($has_entries) {
    $form['exporter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Detailed List'),
      '#prefix' => '<div id="islandora-bookmark-detailed-list">',
      '#suffix' => '</div>',
    );
    islandora_entities_add_export_features($form['exporter'], $form_state, $type);
  }
  return $form;
}

/**
 * Adds export selectors.
 *
 * @param array $container
 *   Form element receiving new elements
 * @param array $form_state
 *   Drupal Form State
 * @param string $type
 *   citation or thesis
 */
function islandora_entities_add_export_features(&$container, &$form_state, $type) {

  $groups = array();
  $export_styles = array();
  $export = islandora_bookmark_get_export_options();
  $export_options['default'] = t('- Export type -');

  foreach ($export as $module_name => $export_array) {
    foreach ($export_array as $name => $export_func) {
      $export_options[t("@module_name", array("@module_name" => $module_name))][$name] = $name;
      $form_state['islandora_bookmark_export'][$name] = $export_func;
      $export_styles[$name] = islandora_bookmark_get_export_format_options($name);
    }
  }

  $style_elements = array();
  foreach ($export_styles as $key => $value) {
    if (!empty($value)) {
      $style_elements[$key] = array(
        '#type' => 'select',
        '#title' => t('Style'),
        '#options' => $value,
        '#weight' => 0,
        '#states' => array(
          'visible' => array(
            ':input[name="exporter[export_options]"]' => array('value' => $key),
          ),
        ),
      );
    }
  }

  $groups += array(
    'export_title' => array(
      '#markup' => '<h3>' . t('Export') . '</h3>',
      '#weight' => 3,
    ),
    'export_options' => array(
      '#type' => 'select',
      '#title' => t('Format'),
      '#options' => $export_options,
      '#weight' => 4,
    ),
    'format_opts' => array(
      '#weight' => 5,
      'export_styles' => $style_elements,
      'export_selected' => array(
        '#type' => 'submit',
        '#value' => t('Export selected @type', array('@type' => $type)),
        '#weight' => 5,
        '#action_validate' => array(
          'formHasExportTypeSelected',
          'formHasObjectsSelected',
        ),
      ),
    ),
  );
  if (!empty($groups)) {
    $container += $groups;
  }
}

/**
 * Form validator.
 *
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_entities_citation_form_validate($form, $form_state) {
  $pids = array_filter(array_values($form_state['values']['citations']));
  if (empty($pids)) {
    form_set_error('citations', t("Please select one or more objects for export."));
  }

}
/**
 * Submit handler.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_entities_citation_form_submit($form, $form_state) {
  $el = $form_state['complete form']['exporter']['export_options'];
  $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);
  $pids = array_filter(array_values($form_state['values']['citations']));
  $params = array($pids);
  if (isset($form_state['values']['exporter']['format_opts']['export_styles'])) {
    if (array_key_exists($key, $form_state['values']['exporter']['format_opts']['export_styles'])) {
      $params[] = $form_state['values']['exporter']['format_opts']['export_styles'][$key];
    }
  }
  call_user_func_array($form_state['islandora_bookmark_export'][$key], $params);
}
