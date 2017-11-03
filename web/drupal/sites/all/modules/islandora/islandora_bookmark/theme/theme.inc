<?php

/**
 * @file
 * Theme and preprocess functions.
 */

/**
 * Implements hook_preprocess_theme().
 */
function template_preprocess_islandora_bookmark_list_columns(array &$variables) {
  $variables['column_count'] = 3;
  $list_links = theme(
    'islandora_bookmark_list_links',
    array('bookmark_id' => $variables['bookmark_id'], 'current' => 'manage')
  );
  $variables['list_links'] = drupal_render($list_links);
  drupal_add_css(drupal_get_path('module', 'islandora_bookmark') . '/css/list-columns.css');

  module_load_include('inc', 'islandora_bookmark', 'includes/api');
  $pids = islandora_bookmark_list_pids_query($variables['bookmark_id']);
  $pid_count = 0;
  foreach ($pids as $pid) {
    // Drop the PIDs into baskets corresponding to columns.
    $column = $pid_count % $variables['column_count'] + 1;
    $pid_count++;
    $variables['objects'][$column][$pid] = array();
    // Build markup for objects.
    $object_markup = &$variables['objects'][$column][$pid];
    $object = islandora_object_load($pid);
    $object_markup['image'] = islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN']) ?
      theme(
        'image',
        array('path' => url("islandora/object/$pid/datastream/TN/view"))) :
      '';

    $display_label = isset($object->label) ? $object->label : $pid;
    $object_markup['label'] = l(
      t("<strong>@display_label</strong>", array('@display_label' => $display_label)),
      "islandora/object/$pid",
      array('html' => TRUE)
    );

    if (islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['DC'])) {
      $dc_dom = new DOMDocument();
      $dc_dom->loadXML($object['DC']->content);
      $dc_xpath = new DOMXpath($dc_dom);
      $dc_elements = $dc_xpath->query("//*[local-name()='description']");
      if ($dc_elements->item(0)) {
        $object_markup['description'] = "<div>{$dc_elements->item(0)->nodeValue}</div>";
      }
    }
  }
}

/**
 * Implements theme_hook().
 */
function theme_islandora_bookmark_list_links(array &$variables) {
  $links = array('#weight' => -1);
  $module_path = drupal_get_path('module', 'islandora_bookmark');
  if ($variables['current'] != '') {
    $links['manage'] = array(
      '#markup' => l(
        t('Admin'),
        "islandora-bookmark/listid/{$variables['bookmark_id']}/manage",
        array('attributes' => array('class' => array('list-links')))
      ),
    );
  }
  if ($variables['current'] != 'islandora_bookmark_list_columns') {
    $links['islandora_bookmark_list_columns'] = array(
      '#markup' => l(
        t('View'),
        "islandora-bookmark/listid/{$variables['bookmark_id']}/view",
        array('attributes' => array('class' => array('list-links')))
      ),
    );
  }
  // This is the RSS icon link.
  $links['bookmark_rss'] = array(
    '#markup' => l(theme('image', array(
      'path' => $module_path . '/images/rss.png',
      'title' => t('Present Bookmark list in RSS form'),
      'attributes' => array(),
        )), "islandora-bookmark/listid/{$variables['bookmark_id']}/rss", array('html' => TRUE)),
    '#prefix' => '<div id="islandora-bookmark-rss-format">',
    '#suffix' => '</div>',
  );
  drupal_add_css($module_path . '/css/list-links.css');
  return $links;
}

/**
 * Prepares variables for islandora_bookmark_list_info templates.
 *
 * Default template: islandora-bookmark-list-info.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - bookmark: A Bookmark object for which to render info.
 *   We populate:
 *   - user: The owner's name.
 *   - name: The name of the list.
 *   - description: The "description" value associated with the list.
 *   - link: An absolute URL to the current path.
 */
function template_preprocess_islandora_bookmark_list_info(array &$variables) {
  $variables['name'] = $variables['bookmark']->bookmarkName;
  $owner = user_load($variables['bookmark']->bookmarkOwner);
  $variables['user'] = $owner->name;
  $variables['description'] = $variables['bookmark']->bookmarkDescription;
  $variables['link'] = url(current_path(), array('absolute' => TRUE));
}

/**
 * Wrapper to display a view of a bookmark.
 *
 * @param string $list_id
 *   A string representing a bookmark object.
 *
 * @return string
 *   The HTML to be rendered for the list columns.
 */
function islandora_bookmark_view_bookmark($list_id) {
  return theme('islandora_bookmark_list_columns', array('bookmark_id' => $list_id));
}
