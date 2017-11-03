<?php
/**
 * Creates a HTML stream from MODS for simple text cmodel.
 *
 * This function can be modified to display other fields in concert with any
 * changes that you make to islandora_example_simple_text_retrieve_mods_data.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 * @param bool $force
 *   Whether derivative generation is being forced or not.
 *
 * @return array
 *   An array describing the outcome of the html creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_example_simple_text_create_html(AbstractObject $object, $force = FALSE) {
  $return = '';
  $ingest = FALSE;
  // This is just an example of HTML creation from MODs
  // This can be extended to extract more complex elements from MODS.
  if (!isset($object['HTML']) || (isset($object['HTML']) && $force === TRUE)) {
    $html_data_array = islandora_example_simple_text_retrieve_mods_data($object);
    $html_string = '<div>';
    if (count($html_data_array) > 0) {
      if (count($html_data_array['title']) > 0) {
        $html_string .= "<h1>{$html_data_array['title']}</h1>";
      }
      if (count($html_data_array['subtitle']) > 0) {
        $html_string .= "<h2>{$html_data_array['subtitle']}</h2>";
      }
      if (isset($html_data_array['note'])) {
        if (count($html_data_array['note']) > 0) {
          foreach ($html_data_array['note'] as $note) {
            $html_string .= "<p>{$note}</p>";
          }
        }
      }

    }
    $html_string .= '</div>';

    if (!isset($object['HMTL'])) {
      $ingest = TRUE;
      $html_datastream = $object->constructDatastream('HTML', 'M');
      $html_datastream->label = 'Derived HTML from MODS';
      $html_datastream->mimetype = 'text/html';
    }
    else {
      $html_datastream = $object['HTML'];
    }
    $html_datastream->setContentFromString($html_string);
    if ($ingest) {
      $object->ingestDatastream($html_datastream);
    }
    $return = array(
      'success' => TRUE,
      'messages' => array(
        array(
          'message' => t('The HTML datastream was generated and added successfully for @pid!'),
          'message_sub' => array('@pid' => $object->id),
          'type' => 'dsm',
        ),
      ),
    );

  }
  return $return;
}

/**
 * Retrieve MODS fields from an object.
 *
 * This function should be modified to retrieve the fields that you want.
 *
 * @param AbstractObject $object
 *   An object that we want to extract the MODS data from
 *
 * @return array
 *   MODS data for the object
 */
function islandora_example_simple_text_retrieve_mods_data(AbstractObject $object) {
  $data = array();
  if (!empty($object['MODS'])) {
    $mods_string = $object['MODS']->content;
    $mods_doc = new DOMDocument();
    $mods_doc->loadXML($mods_string);
    $mods_xpath = new DOMXPath($mods_doc);
    $mods_xpath->registerNamespace('mods', 'http://www.loc.gov/mods/v3');
    $title_xpath = ISLANDORA_EXAMPLE_SIMPLE_TEXT_MODS_RELATED_XPATH . '/mods:titleInfo/mods:title';
    $subtitle_xpath = ISLANDORA_EXAMPLE_SIMPLE_TEXT_MODS_RELATED_XPATH . '/mods:titleInfo/mods:subTitle';
    $note_xpath = ISLANDORA_EXAMPLE_SIMPLE_TEXT_MODS_RELATED_XPATH . '/mods:note';

    $mods_title_xpath = $mods_xpath->query($title_xpath);
    if ($mods_title_xpath->length) {
      $mods_title = $mods_title_xpath->item(0);
      $data['title'] = htmlspecialchars_decode($mods_title->nodeValue);
    }
    $mods_subtitle_xpath = $mods_xpath->query($subtitle_xpath);
    if ($mods_subtitle_xpath->length) {
      $mods_subtitle = $mods_subtitle_xpath->item(0);
      $data['subtitle'] = htmlspecialchars_decode($mods_subtitle->nodeValue);
    }
    $mods_note_xpath = $mods_xpath->query($note_xpath);
    if ($mods_note_xpath->length) {
      foreach ($mods_note_xpath as $item) {
        $data['note'][] = htmlspecialchars_decode($item->nodeValue);
      }
    }
  }
  return $data;
}

/**
 * Modifies the MODS stream for simple text cmodel.
 *
 * It reads the parent objects PID and adds the correct xlink property
 * to mods:relatedItem
 * This functionaliy was added as a derivative, instead of a object ingested
 * hook to avoid that additional relations trigger, after object creation, a
 * modification of this value
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 * @param bool $force
 *   Whether derivative generation is being forced or not.
 *
 * @return array
 *   An array describing the outcome of the mods modification.
 *
 * @see hook_islandora_derivative()
 */
function islandora_example_simple_text_addparent_xlink(AbstractObject $object, $force = FALSE) {
  module_load_include('inc', 'islandora_example_simple_text', 'includes/utilities');
  $return = '';
  $mods_was_modified = FALSE;
  // Let's check if this object has MODS or not.
  // Also allow if the object is new, means not HTML stream yet.
  if ((isset($object['MODS']) && $force === TRUE) || !isset($object['HTML'])) {
    // First we get the parent objects for the current one
    // parentObjects is returned "filtered" by array_filter function to remove
    // nulls, but keys are not reordered/reset, so we have to "array_shift"
    // it to get the first one.
    $parent_objects = islandora_example_simple_text_get_parents_from_rels_ext($object);
    if (count($parent_objects) > 0) {
      // We will get only the first object here. If there are multiple then we
      // don't know how to define which one is the correct anyway, so we get
      // always the first one added.
      // We could in theory discard parent collections, but
      // comments/contribution could also be done to Collections.
      $parent_object = array_shift($parent_objects);
      $url_enconded_pid = urlencode($parent_object->id);
      $xlink = $GLOBALS['base_url'] . "/islandora/object/{$url_enconded_pid}";
      // Now add this to the current object's MODS.
      $mods_doc = new DOMDocument();
      $mods_doc->loadXML($object['MODS']->content);
      $mods_xpath = new DOMXPath($mods_doc);
      $mods_xpath->registerNamespace('mods', 'http://www.loc.gov/mods/v3');
      $relateditem_xpath = ISLANDORA_EXAMPLE_SIMPLE_TEXT_MODS_RELATED_XPATH;
      $mods_relateditem_xpath = $mods_xpath->query($relateditem_xpath);
      if ($mods_relateditem_xpath->length) {
        $mods_relateditem_xpath->item(0)
          ->setAttribute('xlink', htmlspecialchars($xlink));
        $mods_was_modified = TRUE;
      }
      if ($mods_was_modified) {

        $object['MODS']->content = $mods_doc->saveXML();
      }
    }
    else {
      // Nothing to do, no relations here.
      $return = array();
    }
  }
  else {
    $return = array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('Object @pid has no MODS stream to work on!'),
          'message_sub' => array('@pid' => $object->id),
          'type' => 'dsm',
        ),
      ),
    );
  }
  return $return;
}
