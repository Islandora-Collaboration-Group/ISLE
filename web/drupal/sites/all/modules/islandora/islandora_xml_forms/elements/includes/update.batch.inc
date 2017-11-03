<?php
/**
 * @file
 * Batch to update all forms.
 */

/**
 * Batch operation to rename all datepicker elements to the new value.
 *
 * @param array $context
 *   An array containing the context of the batch operation.
 */
function xml_form_elements_rename_datepicker_batch_operation(&$context) {
  $sandbox = &$context['sandbox'];

  if (!isset($sandbox['forms'])) {
    $sandbox['forms'] = db_select('xml_forms', 'x')
      ->fields('x')
      ->execute()
      ->fetchAllAssoc('id', PDO::FETCH_ASSOC);
    if (empty($sandbox['forms'])) {
      return;
    }
  }
  // Process a form at a time.
  $form = array_shift($sandbox['forms']);
  $context['message'] = t('Parsing @form_name for datepicker elements.', array(
    '@form_name' => $form['name'],
  ));
  $dom = new DOMDocument();
  $dom->loadXML($form['form']);

  $xpath = new DOMXPath($dom);
  $results = $xpath->query("//element/properties/type[text() = 'datepicker']");
  if ($results->length > 0) {
    foreach ($results as $element) {
      $element->nodeValue = XML_FORM_ELEMENTS_DATEPICKER_THEME;
    }
    db_update('xml_forms')
      ->fields(array(
        'form' => $dom->saveXML(),
      ))
      ->condition('id', $form['id'])
      ->execute();
    drupal_set_message(t('Updated @count elements in @form_name (@form_id) successfully.', array(
      '@count' => $results->length,
      '@form_name' => $form['name'],
      '@form_id' => $form['id'],
    )));
  }
  else {
    drupal_set_message(t('Skipped @form_name (@form_id) as no occurrences were found.', array(
      '@form_name' => $form['name'],
      '@form_id' => $form['id'],
    )));
  }
  $context['finished'] = empty($sandbox['forms']) ? 1 : 0;
}
