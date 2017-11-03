<?php

/**
 * @file
 * Holds block definitions for the compound solution pack.
 */

/**
 * Constructs the JAIL display block for a compound.
 *
 * @return array
 *   Renderable array for the block.
 */
function islandora_compound_object_jail_display_block() {
  $block = array();
  $object = menu_get_object('islandora_object', 2);
  if ($object) {
    $compound_info = islandora_compound_object_retrieve_compound_info($object, TRUE);
    if (!empty($compound_info)) {
      $module_path = drupal_get_path('module', 'islandora_compound_object');
      $jail_path = libraries_get_path('JAIL');
      $block['#attached']['js'] = array(
        "$jail_path/lib/jquery.js" => array(
          'group' => JS_LIBRARY,
        ),
        "$jail_path/dist/jail.min.js" => array(
          'group' => JS_LIBRARY,
        ),
        "$module_path/js/compound_jail.js" => array(
          'group' => JS_LIBRARY,
        ),
      );
      $block['#attached']['js'][] = array(
        'data' => array(
          'islandora_compound_object' => array(
            'image_path' => url(drupal_get_path('module', 'islandora') . '/images/folder.png'),
          ),
        ),
        'type' => 'setting',
      );
      $block['#attached']['css'] = array(
        "$module_path/css/islandora_compound_object.jail_block.css",
      );
      $block['part_title'] = array(
        '#type' => 'item',
        '#markup' => t('Part of: @label (@count @obj)', array(
          '@label' => $compound_info['parent_label'],
          '@count' => $compound_info['child_count'],
          '@obj' => format_plural($compound_info['child_count'], 'object', 'objects'),
        )),
      );
      if ($compound_info['parent_url']) {
        $block['manage_link'] = array(
          '#type' => 'item',
          '#markup' => l(t('manage parent'), $compound_info['parent_url']),
        );
      }

      $query_params = drupal_get_query_parameters();
      foreach ($compound_info['siblings_detailed'] as $sibling) {
        $path = 'islandora/object/' . $sibling['pid'] . '/datastream/TN/view';
        $class = array(
          'islandora-compound-object-jail',
        );
        if ($sibling['pid'] == $compound_info['current_pid']) {
          $class[] = 'islandora-compound-object-jail-active';
        }
        $img = array(
          'image' => array(
            '#theme' => 'image',
            '#path' => "$module_path/images/loader.png",
            '#attributes' => array(
              'class' => $class,
              'data-src' => url($path),
            ),
            '#href' => "islandora/object/{$sibling['pid']}",
          ),
          'noscript' => array(
            '#theme' => 'image',
            '#path' => $path,
            '#prefix' => '<noscript>',
            '#suffix' => '</noscript>',
          ),
        );
        $block[$sibling['pid']] = array(
          '#type' => 'container',
          '#attributes' => array(),
          'link' => array(
            '#type' => 'link',
            '#title' => t("@title", array("@title" => $sibling['title'])),
            '#href' => "islandora/object/{$sibling['pid']}",
            '#options' => array('query' => $query_params),
          ),
          'content' => array(
            '#theme' => 'link',
            '#text' => drupal_render($img),
            '#path' => "islandora/object/{$sibling['pid']}",
            '#options' => array(
              'attributes' => array(),
              'html' => TRUE,
              'query' => $query_params,
            ),
          ),
        );
      }
    }
  }
  return $block;
}

/**
 * Constructs the default navigation block for a compound.
 *
 * @param AbstractObject $object
 *   If supplied the object to create the block for.
 *
 * @return array
 *   Renderable array for the block.
 */
function islandora_compound_object_navigation_block(AbstractObject $object = NULL) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  drupal_add_css(drupal_get_path('module', 'islandora_compound_object') . '/css/islandora_compound_object.block.css');
  $object = $object ? $object : menu_get_object('islandora_object', 2);
  if ($object) {
    $output = '';
    if ($object) {
      $compound_info = islandora_compound_object_retrieve_compound_info($object, TRUE);
      // This object is part of a compound object.
      if (!empty($compound_info)) {
        $prev_next = theme('islandora_compound_prev_next', $compound_info);
        $output .= $prev_next;
      }

      if (!empty($output)) {
        return array('#markup' => $output);
      }
    }
  }
}
