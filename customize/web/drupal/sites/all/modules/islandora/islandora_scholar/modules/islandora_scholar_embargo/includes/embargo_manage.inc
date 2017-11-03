<?php
/**
 * @file
 * Implements function to display embargoed list.
 */

/**
 * Builds management form for embargoed entries.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   Returns the form.
 */
function islandora_scholar_embargo_manage_form(array $form, array &$form_state) {

  $table = islandora_scholar_embargo_build_table();

  $form['table'] = array(
    '#type' => 'markup',
    '#markup' => $table,
  );

  return $form;
}

/**
 * Builds table to pass to theming function.
 *
 * @return array
 *   Returns the table.
 */
function islandora_scholar_embargo_build_table() {
  module_load_include('inc', 'islandora_scholar_embargo', 'includes/embargo');
  $embargoed = islandora_scholar_embargo_get_all_embargoed();
  $header = array(
    t('Label'),
    t('PID'),
    t('Type'),
    t('Embargoed until'),
    t('Manage'),
  );
  $rows = array();
  foreach ($embargoed as $result) {
    $pid = $result['obj']['value'];
    $object_parts = explode('/', $pid);

    // When we explode if there's more than 1 element it constitutes that we
    // embargoed something other than the object itself.
    if (count($object_parts) == 1) {
      $type = t('Object');
    }
    else {
      list($pid, $type) = $object_parts;
    }
    $date = $result['date']['value'];
    if ($date != 'indefinite') {
      $date = new dateTime($date);
      $date = $date->format('M d, Y ');
    }
    else {
      $date = ucwords($date);
    }
    $temp_obj = islandora_object_load($pid);
    $rows[] = array(
      l($temp_obj->label, 'islandora/object/' . $pid),
      $pid,
      $type,
      $date,
      l(t('Manage'), 'islandora/object/' . $pid . '/manage/embargo'),
    );
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t("There are no embargoed entries")));
}
