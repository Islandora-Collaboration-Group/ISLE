<?php

/**
 * @file
 * Theme hooks.
 */

require_once dirname(__FILE__) . '/../includes/utilities.inc';

/**
 * Theme the page navigator.
 */
function theme_islandora_paged_content_page_navigator(array $variables) {
  $object = $variables['object'];
  $results = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'isPageOf');
  $result = reset($results);
  $parent = $result ? islandora_object_load($result['object']['value']) : FALSE;
  $pages = $parent ? islandora_paged_content_get_pages($parent) : array();
  $pages = array_keys($pages);
  if (empty($pages)) {
    $pages[] = $object->id;
  }
  $index = array_search($object->id, $pages);
  $previous_page = isset($pages[$index - 1]) ? $pages[$index - 1] : NULL;
  $next_page = isset($pages[$index + 1]) ? $pages[$index + 1] : NULL;
  $links = array();
  if ($previous_page) {
    $links[] = array(
      'title' => t('Prev'),
      'href' => url("islandora/object/{$previous_page}", array('absolute' => TRUE)),
    );
  }
  $links[] = array(
    'title' => '<strong>' . t('Page') . '</strong>',
    'html' => TRUE,
  );
  if ($next_page) {
    $links[] = array(
      'title' => t('Next'),
      'href' => url("islandora/object/{$next_page}", array('absolute' => TRUE)),
    );
  }
  if ($parent) {
    $links[] = array(
      'title' => t('All Pages'),
      'href' => url("islandora/object/{$parent->id}", array('absolute' => TRUE)),
    );
  }
  return theme('links', array(
    'links' => $links,
    'attributes' => array(
      'class' => array(
        'links',
        'inline',
      ),
    ),
  ));
}

/**
 * Implements template_preprocess_theme().
 */
function template_preprocess_swaptable(array &$variables) {
  global $pager_page_array, $pager_total, $pager_total_items, $pager_limits;
  element_set_attributes($variables['element'], array('id', 'name'));
  $table = &$variables['element'];
  $variables['pager'] = swaptable_pagers($table['#pager'], count($table['#rows']));
  $variables['table']['left'] = swaptable_table('left', $table, $variables['pager']['left']);
  $variables['table']['right'] = swaptable_table('right', $table, $variables['pager']['right']);
  $base = $table['#id'];
  $settings['swapTable'][$base] = array(
    'name' => $table['#name'],
    'order' => $table['order']['#value'],
    'modified' => $table['modified']['#value'],
  );
  drupal_add_js($settings, 'setting');
}

/**
 * Builds the descriptions of both pagers for use in the swaptable.
 *
 * @param array $pager
 *   The general description of the pagers.
 * @param int $total
 *   The total number of items to page through.
 *
 * @return array
 *   The description of both pagers for use in the swaptable.
 */
function swaptable_pagers(array $pager, $total) {
  extract($pager);
  return array(
    'left' => swaptable_pager($element['left'], $tags, $quantity, $limit, $total),
    'right' => swaptable_pager($element['right'], $tags, $quantity, $limit, $total),
  );
}

/**
 * Builds a description of a pager for use in the swaptable.
 *
 * @param int $element
 *   The unique id for the pager.
 * @param array $tags
 *   The tags definition to use when rendering the pager.
 * @param int $quantity
 *   The number of pages to display as links.
 * @param int $limit
 *   The number of items to show on a page.
 * @param int $total
 *   The total number of items to page through.
 *
 * @return array
 *   The pager definition for use in the swaptable.
 */
function swaptable_pager($element, $tags, $quantity, $limit, $total) {
  $page = pager_default_initialize($total, $limit, $element);
  return array(
    'element' => $element,
    'tags' => $tags,
    'quantity' => $quantity,
    'limit' => $limit,
    'page' => $page,
  );
}

/**
 * Prepares the rows to be rendered in a table.
 *
 * @param string $position
 *   The position of the table either 'left' or 'right'.
 * @param array $items
 *   The description of each row of the table where the keys are the unique
 *   identifers for each row.
 * @param array $ordering
 *   The ordering in which to display the given rows.
 * @param array $modified
 *   The rows which have been modified by the user directly.
 * @param array $pager
 *   The description of the pager to attach to this table.
 *
 * @return array
 *   The rows definition for the table.
 */
function swaptable_table_rows($position, array $items, array $ordering, array $modified, array $pager) {
  $rows = array();
  $offset = $pager['page'] * $pager['limit'];
  $ordering = array_slice($ordering, $offset, $pager['limit']);
  $original_ordering = element_children($items);
  foreach ($ordering as $i => $item) {
    $row = $items[$item];
    $data = array();
    $data[] = array(
      'data' => $offset + $i + 1,
      'class' => array('ordering'),
    );
    $data[] = array(
      'data' => array_search($item, $original_ordering) + 1,
      'class' => array('original-ordering', 'swaptable-hide'),
    );
    foreach (element_children($row) as $j) {
      $cell = $row[$j];
      $data[] = array(
        'data' => empty($cell['#printed']) ? drupal_render($cell) : $cell['#children'],
        'class' => array($j),
      );
    }
    $rows[] = array(
      'id' => str_replace(':', '-', "{$position}-{$item}"),
      'class' => in_array($item, $modified) ? array('ui-modified') : array(),
      'data' => $data,
    );
  }
  return $rows;
}

/**
 * Renders a single swap table where $position, denotes 'left' or 'right'.
 *
 * @param string $position
 *   The position of the table either 'left' or 'right'.
 * @param array $table
 *   The description of the table.
 * @param array $pager
 *   The description of the pager to attach to this table.
 *
 * @return string
 *   The rendered table.
 */
function swaptable_table($position, array $table, array $pager) {
  $table['#attributes']['id'] = "{$table['#id']}-{$position}";
  $table['#attributes']['name'] = $position;
  $table['#attributes']['valign'] = 'top';
  $header = array(
    array(
      'data' => t('Page #'),
      'class' => array('ordering'),
    ),
    array(
      'data' => t('*Original Page #'),
      'class' => array('original-ordering', 'swaptable-hide'),
    ),
  );
  return array(
    'header' => array_merge($header, $table['#header']),
    'rows' => swaptable_table_rows($position, $table['#rows'], $table['order']['#value'], $table['modified']['#value'], $pager),
    'attributes' => $table['#attributes'],
  );
}
