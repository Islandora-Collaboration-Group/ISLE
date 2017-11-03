<?php

/**
 * @file
 * Any hooks/functions used by this modules theme's.
 */

/**
 * Implements hook_preprocess_theme().
 */
function template_preprocess_islandora_internet_archive_bookreader(array &$variables) {
  module_load_include('inc', 'islandora_internet_archive_bookreader', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/authtokens');
  $object = $variables['object'];
  $dsid = $variables['datastream_id'];
  $library_path = libraries_get_path('bookreader');
  $colorbox_path = libraries_get_path('colorbox');
  $module_path = drupal_get_path('module', 'islandora_internet_archive_bookreader');
  $search_uri = module_exists('islandora_solr') ? url("internet_archive_bookreader_search/{$object->id}/TERM", array('absolute' => TRUE)) : NULL;
  $backup_uris = variable_get('islandora_internet_archive_bookreader_use_backup_dsid', FALSE);

  $pages = array();
  foreach ($variables['pages'] as $page_pid => $info) {
    $addendum = $backup_uris ? array('pid' => $page_pid) : array(
      'uri' => url("islandora/object/{$page_pid}/datastream/{$dsid}/view", array(
        'absolute' => TRUE,
        'query' => array(
          'token' => islandora_get_object_token($page_pid, $dsid, 2),
        ),
      )),
    );
    $pages[] = $info + $addendum;
  }

  global $base_url;
  $data_array = array(
    'islandoraInternetArchiveBookReader' => array(
      'book' => $object->id,
      'info' => theme('islandora_internet_archive_bookreader_book_info', array('object' => $object)),
      'label' => $object->label,
      'searchUri' => $search_uri,
      'textUri' => url('internet_archive_bookreader_text/PID', array('absolute' => TRUE)),
      'dimensionsUri' => url('internet_archive_bookreader_dimensions/PID', array('absolute' => TRUE)),
      'tokenUri' => url('internet_archive_bookreader_get_image_uri/PID', array('absolute' => TRUE)),
      'djatokaUri' => variable_get('islandora_paged_content_djatoka_url', 'http://localhost:8080/adore-djatoka'),
      'imagesFolderUri' => url("$base_url/$library_path/BookReader/images/", array('absolute' => TRUE, 'external' => TRUE)),
      'compression' => variable_get('islandora_internet_archive_bookreader_compression', '4'),
      'pageProgression' => $variables['page_progression'],
      'pages' => $pages,
      'pageCount' => count($pages),
      'overlayOpacity' => variable_get('islandora_internet_archive_bookreader_overlay_opacity', '0.5'),
      'mode' => (int) variable_get('islandora_internet_archive_bookreader_default_page_view', 1),
      'mobilize' => variable_get('islandora_internet_archive_bookreader_mobile_full_screen', FALSE),
      'content_type' => variable_get('islandora_internet_archive_bookreader_content_type', 'book'),
      'useBackupUri' => $backup_uris,
    ),
  );

  drupal_add_js($data_array, 'setting');
  drupal_add_css("$library_path/BookReader/BookReader.css", array(
    'group' => CSS_SYSTEM,
  ));
  drupal_add_css("$module_path/css/islandora_internet_archive_bookreader.css");
  // Included jQuery 1.4.2 and jQueryUI 1.8.5 are not being used in favor of
  // the Drupal system versions.
  drupal_add_library('system', 'effects');
  drupal_add_library('system', 'ui.slider');
  drupal_add_library('system', 'ui.draggable');

  // XXX: Extra slash is NOT an error or a mistake. We need to add this JS
  // both before and after our custom jQuery gets setup, as we need it in both
  // instances of jQuery.
  drupal_add_js("$library_path/BookReader//jquery.bt.min.js", array(
    'group' => JS_LIBRARY,
    'weight' => -6,
  ));
  drupal_add_js("$module_path/js/detectmobilebrowser.js",
    array(
      'group' => JS_LIBRARY,
      'weight' => -6,
    )
  );
  // Add the required jQuery version, specifying weight.
  drupal_add_js("$module_path/js/jquery_1_5.min.js", array(
    'group' => JS_LIBRARY,
    'weight' => -6,
  ));
  // We also need to be sure to use the correct jquery ui version.
  drupal_add_js("$module_path/js/jquery.ui-1.8.10/jquery-ui.min.js", array(
    'group' => JS_LIBRARY,
    'weight' => -6,
  ));
  drupal_add_js("$library_path/BookReader/dragscrollable.js", array(
    'group' => JS_LIBRARY,
    'weight' => -6,
  ));
  // Use a newer version of color box as the version bundled with BookReader
  // won't work with JQuery 1.4.4
  drupal_add_js("$colorbox_path/jquery.colorbox-min.js", array(
    'group' => JS_LIBRARY,
    'weight' => -6,
  ));
  drupal_add_js("$library_path/BookReader/jquery.bt.min.js", array(
    'group' => JS_LIBRARY,
    'weight' => -6,
  ));
  drupal_add_js("$library_path/BookReader/BookReader.js", array(
    'group' => JS_LIBRARY,
    'weight' => -6,
  ));
  drupal_add_js("$module_path/js/islandora_book_reader.js",
    array(
      'group' => JS_LIBRARY,
      'weight' => -4,
    )
  );
  drupal_add_js("$module_path/js/islandora_internet_archive_bookreader.js", array(
    'group' => JS_LIBRARY,
    'weight' => -3,
  ));
  // The Internet Archive Book Reader, hijacks the URL anchor '#' for jumping to
  // particular page # / view modes. Drupal fieldset javascript code
  // (collapse.js) expects the URL anchor '#' to be a valid HTML ID, and fails
  // to execute when it is not a valid HTML ID. To get around this we use our
  // own version of (collapse.js) that ignores this situation.
  drupal_add_js("$module_path/js/collapse.js", array(
    'group' => JS_THEME,
  ));
  // collapse.js requires form.js to be present.
  drupal_add_js('misc/form.js');
}

/**
 * Implements theme_hook().
 */
function theme_islandora_internet_archive_bookreader_book_info(array $variables) {
  module_load_include('inc', 'islandora', 'includes/metadata');

  $object = $variables['object'];

  // As IAV presents the metadata within a Colorbox window request for the
  // metadata in printable form.
  $metadata = islandora_retrieve_metadata_markup($object, TRUE);
  $viewer = variable_get('islandora_metadata_display', 'dublin_core');
  // Let's default back to what IAV had before for a default.
  if ($viewer != 'dublin_core' && $metadata) {
    $content = $metadata;
  }
  else {
    $fields = islandora_internet_archive_bookreader_info_fields($object);
    $convert_to_string = function($o) {
      return implode('<br/>', $o);
    };
    $fields = array_map($convert_to_string, $fields);
    $rows = array_map(NULL, array_keys($fields), array_values($fields));
    $content = theme('table', array(
      'caption' => '',
      'empty' => t('No Information specified.'),
      'attributes' => array(),
      'colgroups' => array(),
      'header' => array(t('Field'), t('Values')),
      'rows' => $rows,
      'sticky' => FALSE));
  }
  return $content;
}

/**
 * Retrieves fields to display in the BookReader's "info" modal dialog box.
 *
 * @param AbstractObject $object
 *   The object to fetch the information from.
 *
 * @return array
 *   Any associative array where the key is the label and the second the
 *   fields value(s).
 */
function islandora_internet_archive_bookreader_info_fields(AbstractObject $object) {
  if (!(isset($object['MODS']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['MODS']))) {
    return array();
  }
  $fields = array(
    'Title' => '//mods:title',
    'Subtitle' => '//mods:subTitle',
    'Date' => '//mods:dateCreated | //mods:dateIssued',
    'Name' => array('//mods:name', 'mods:namePart'),
    'Abstract' => '//mods:abstract',
    'Identifier' => '//mods:identifier',
    'Physical Description' => array('//mods:physicalDescription', 'mods:extent | mods:form | mods:note[not(@*)] | mods:note[@type="physical"] | mods:note[@type="details"] | mods:scale | mods:coordinates'),
    'Topic' => '/mods:mods/mods:subject/mods:topic',
    'Continent' => '//mods:continent',
    'Country' => '//mods:country',
    'Province' => '//mods:province',
    'County' => '//mods:county',
    'Region/Lot' => '//mods:region',
    'City' => '//mods:city',
    'City Section' => '//mods:citySection',
    'Use and Reproduction' => '//mods:accessCondition[@type="useAndReproduction"]',
    'Restrictions on Access' => '//mods:accessCondition[@type="restrictionOnAccess"]',
    'Physical Location' => '//mods:physicalLocation',
  );
  $xml = simplexml_load_string($object['MODS']->content);
  $xml->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
  $to_string = function($o) {
    return (string) $o;
  };
  // Query each context node with the given xpath.
  $query_each_context = function (array $contexts, $xpath) {
    $query = function(&$context, $key, $xpath) {
      $context->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
      $context = $context->xpath($xpath);
    };
    // Map values to each context node.
    array_walk($contexts, $query, $xpath);
    // Flatten to an array of values.
    return count($contexts) ? call_user_func_array('array_merge', $contexts) : array();
  };
  // Query the given xpath. If the xpath is any array the first value is the
  // xpath for the context node(s) and the second the path to the value fields.
  $query = function(SimpleXMLElement $xml, $xpath) use(&$query_each_context) {
    return is_string($xpath) ? $xml->xpath($xpath) : $query_each_context($xml->xpath($xpath[0]), $xpath[1]);
  };
  foreach ($fields as $label => $xpath) {
    $results = $query($xml, $xpath);
    // Remove empty values.
    $fields[$label] = array_filter(array_map($to_string, $results));
  }
  // Remove empty fields.
  return array_filter($fields);
}
