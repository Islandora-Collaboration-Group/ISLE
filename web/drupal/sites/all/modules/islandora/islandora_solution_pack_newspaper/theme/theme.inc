<?php

/**
 * @file
 * Theme file for Newspaper solution pack. Includes preprocess functions.
 */

require_once dirname(__FILE__) . '/../includes/utilities.inc';

/**
 * Prepares variables for islandora_newspaper templates.
 *
 * Default template: islandora-newspaper.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_islandora_newspaper(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/metadata');
  drupal_add_js('misc/collapse.js');
  $object = $variables['object'];
  $issues = islandora_newspaper_get_issues($object);
  $grouped_issues = islandora_newspaper_group_issues($issues);
  $output = array(
    'controls' => array(
      '#theme' => 'links',
      '#attributes' => array(
        'class' => array('links', 'inline'),
      ),
      '#links' => array(
        array(
          'title' => t('Expand all months'),
          'href' => "javascript://void(0)",
          'html' => TRUE,
          'external' => TRUE,
          'attributes' => array(
            'onclick' => "Drupal.toggleFieldset(jQuery('fieldset.month.collapsed'));",
          ),
        ),
        array(
          'title' => t('Collapse all months'),
          'href' => "javascript://void(0)",
          'html' => TRUE,
          'external' => TRUE,
          'attributes' => array(
            'onclick' => "Drupal.toggleFieldset(jQuery('fieldset.month:not(.collapsed)'));",
          ),
        ),
      ),
    ),
    'tabs' => array(
      '#type' => 'vertical_tabs',
    ),
  );
  $tabs = &$output['tabs'];
  foreach ($grouped_issues as $year => $months) {
    $tabs[$year] = array(
      '#title' => $year,
      '#type' => 'fieldset',
    );
    foreach ($months as $month => $days) {
      $month_name = t("@date", array(
        "@date" => date("F", mktime(0, 0, 0, $month, 1, 2000)),
      ));
      $tabs[$year][$month] = array(
        '#title' => $month_name,
        '#type' => 'fieldset',
        '#attributes' => array(
          'class' => array('collapsible', 'collapsed', 'month'),
        ),
      );
      foreach ($days as $day => $issues) {
        foreach ($issues as $issue) {
          $tabs[$year][$month][$day][] = array(
            '#theme' => 'link',
            '#prefix' => '<div>',
            '#suffix' => '</div>',
            '#text' => t("@month @day, @year", array(
                '@year' => $year,
                '@month' => $month_name,
                '@day' => $day,
                )),
            '#path' => "islandora/object/{$issue['pid']}",
            '#options' => array(
              'attributes' => array(),
              'html' => FALSE,
            ),
          );
        }
      }
      ksort($tabs[$year][$month]);
    }
    ksort($tabs[$year]);
  }
  ksort($tabs);

  $variables['islandora_content_render_array'] = $output;
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);
}

/**
 * Process variables for islandora_newspaper_issue templates.
 *
 * Default template: islandora-newspaper-issue.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 *   - islandora_content_render_array: A renderable array of content to jamb
 *     in.
 */
function template_process_islandora_newspaper(array &$variables) {
  $variables['islandora_content'] = drupal_render($variables['islandora_content_render_array']);
}

/**
 * Prepares variables for islandora_newspaper_issue templates.
 *
 * Default template: islandora-newspaper-issue.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_islandora_newspaper_issue(array &$variables) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/metadata');
  $object = $variables['object'];
  $pages = islandora_paged_content_get_pages($object);
  $variables['pages'] = array_keys($pages);

  // Get viewer id.
  $variables['viewer_id'] = islandora_get_viewer_id('islandora_newspaper_issue_viewers');

  // Get viewer content.
  $variables['viewer_params'] = array(
    'object' => $object,
    'pages' => $pages,
    'page_progression' => islandora_paged_content_get_page_progression($object),
  );

  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);
}

/**
 * Process variables for islandora_newspaper_issue templates.
 *
 * Default template: islandora-newspaper-issue.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 *   - viewer_id: The "ID" of the viewer to use.
 *   - viewer_params: An (associative) array of parameters to pass to the
 *     viewer.
 */
function template_process_islandora_newspaper_issue(array &$variables) {
  $variables['viewer'] = NULL;
  if ($variables['viewer_id'] && $variables['viewer_id'] != 'none') {
    $viewer_callback = islandora_get_viewer_callback($variables['viewer_id']);
    if (function_exists($viewer_callback)) {
      $viewer = $viewer_callback($variables['viewer_params'], $variables['object']);
      if ($viewer) {
        $variables['viewer'] = $viewer;
      }
    }
  }
  else {
    drupal_add_js('misc/form.js');
    drupal_add_js('misc/collapse.js');
  }
}

/**
 * Theme the issue navigator.
 */
function theme_islandora_newspaper_issue_navigator(array $variables) {
  $object = $variables['object'];
  $newspaper = islandora_newspaper_get_newspaper($object);
  $newspaper = $newspaper ? islandora_object_load($newspaper) : FALSE;
  $issues = $newspaper ? islandora_newspaper_get_issues($newspaper) : array();
  $issues = array_keys($issues);
  if (empty($issues)) {
    $issues[] = $object->id;
  }
  $index = array_search($object->id, $issues);
  $previous_issue = isset($issues[$index - 1]) ? $issues[$index - 1] : NULL;
  $next_issue = isset($issues[$index + 1]) ? $issues[$index + 1] : NULL;
  $links = array();
  if (isset($issues[$index - 1])) {
    $previous_issue = $issues[$index - 1];
    $links[] = array(
      'title' => t('Prev'),
      'href' => url("islandora/object/{$previous_issue}", array('absolute' => TRUE)),
    );
  }
  if (count($issues) > 1) {
    $links[] = array(
      'title' => '<strong>' . t('Issue') . '</strong>',
      'html' => TRUE,
    );
  }
  if (isset($issues[$index + 1])) {
    $next_issue = $issues[$index + 1];
    $links[] = array(
      'title' => t('Next'),
      'href' => url("islandora/object/{$next_issue}", array('absolute' => TRUE)),
    );
  }
  if ($newspaper) {
    $links[] = array(
      'title' => t('All Issues'),
      'href' => url("islandora/object/{$newspaper->id}", array('absolute' => TRUE)),
    );
  }
  $attributes = array('class' => array('links', 'inline'));
  return theme('links', array('links' => $links, 'attributes' => $attributes));
}

/**
 * Theme the page selector.
 */
function theme_islandora_newspaper_page_select(array $variables) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $path = drupal_get_path('module', 'islandora_newspaper');
  drupal_add_js($path . '/js/islandora_newspaper.js');
  $object = $variables['object'];
  $results = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'isPageOf');
  $result = reset($results);
  $parent = $result ? islandora_object_load($result['object']['value']) : FALSE;
  $pages = $parent ? islandora_paged_content_get_pages($parent) : FALSE;
  if (!$pages) {
    return;
  }

  $variables = array(
    '#options' => array(),
    '#attributes' => array('class' => array('page-select')),
    '#value' => $object->id,
  );
  foreach ($pages as $pid => $page) {
    $variables['#options'][$pid] = $page['page'];
  }
  $prefix = '<strong>' . t('Image') . ':</strong> ';
  return $prefix . t('!page_selector of @total', array(
    '!page_selector' => theme('select', array('element' => $variables)),
    '@total' => count($pages),
  ));
}

/**
 * Theme a newspaper pages controls.
 */
function template_preprocess_islandora_newspaper_page_controls(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  global $base_url;
  $view_prefix = '<strong>' . t('View:') . ' </strong>';
  $download_prefix = '<strong>' . t('Download:') . ' </strong>';
  $object = $variables['object'];
  $issue = islandora_newspaper_get_issue($object);
  $issue = $issue ? islandora_object_load($issue) : FALSE;
  $controls = array(
    'page_select' => theme('islandora_newspaper_page_select', array('object' => $object)),
    'page_pager' => theme('islandora_paged_content_page_navigator', array('object' => $object)),
    'issue_pager' => ($issue ?
      theme('islandora_newspaper_issue_navigator', array('object' => $issue)) :
      ''),
  );

  // Text view.
  if (isset($object['OCR']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['OCR'])) {
    $url = islandora_datastream_get_url($object['OCR'], 'view');
    $attributes = array('attributes' => array('title' => t('View text')));
    $controls['text_view'] = $view_prefix . l(t('Text'), $url, $attributes);
  }
  // PDF download.
  if (isset($object['PDF']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PDF'])) {
    $size = islandora_datastream_get_human_readable_size($object['PDF']);
    $text = t('PDF (@size)', array('@size' => $size));
    $url = islandora_datastream_get_url($object['PDF'], 'download');
    $attributes = array('attributes' => array('title' => t('Download PDF')));
    $controls['pdf_download'] = $download_prefix . l($text, $url, $attributes);
  }
  if (isset($object['JP2']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JP2'])) {
    // JP2 download.
    $size = islandora_datastream_get_human_readable_size($object['JP2']);
    $text = t('JP2 (@size)', array('@size' => $size));
    $url = islandora_datastream_get_url($object['JP2'], 'download');
    $attributes = array('attributes' => array('title' => t('Download JP2')));
    $controls['jp2_download'] = $download_prefix . l($text, $url, $attributes);

    $controls['clip'] = theme(
      'islandora_openseadragon_clipper',
      array('pid' => $object->id)
    );
  }
  // TIFF download.
  if (isset($object['OBJ']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['OBJ'])) {
    $size = islandora_datastream_get_human_readable_size($object['OBJ']);
    $text = t('TIFF (@size)', array('@size' => $size));
    $url = islandora_datastream_get_url($object['OBJ'], 'download');
    $attributes = array('attributes' => array('title' => t('Download TIFF')));
    $controls['tiff_download'] = $download_prefix . l($text, $url, $attributes);
  }
  $variables['controls'] = $controls;
}

/**
 * Prepares variables for islandora_newspaper_page templates.
 *
 * Default template: islandora-newspaper-page.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 */
function template_preprocess_islandora_newspaper_page(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/metadata');
  $object = $variables['object'];

  $params = array();
  if (isset($object['JP2']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JP2'])) {
    // Get token to allow access to XACML protected datastreams if necessary.
    // Always use token authentication in case there is a global policy.
    module_load_include('inc', 'islandora', 'includes/authtokens');
    $token = islandora_get_object_token($object->id, 'JP2', 2);
    $jp2_url = url("islandora/object/{$object->id}/datastream/JP2/view",
      array(
        'absolute' => TRUE,
        'query' => array('token' => $token),
      ));

    // Display newspaper.
    $params['jp2_url'] = $jp2_url;
  }
  $variables['viewer_id'] = islandora_get_viewer_id('islandora_newspaper_page_viewers');
  $variables['viewer_params'] = $params;
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);
}

/**
 * Process variables for islandora_newspaper_page templates.
 *
 * Default template: islandora-newspaper-page.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - object: An AbstractObject for which to generate the display.
 *   - viewer_id: The "ID" of the viewer to use.
 *   - viewer_params: An (associative) array of parameters to pass to the
 *     viewer.
 */
function template_process_islandora_newspaper_page(array &$variables) {
  $object = $variables['object'];
  if ($variables['viewer_id'] && $variables['viewer_id'] != 'none') {
    $viewer_callback = islandora_get_viewer_callback($variables['viewer_id']);
    if (function_exists($viewer_callback)) {
      $viewer = $viewer_callback($variables['viewer_params'], $variables['object']);
      if ($viewer) {
        $variables['content'] = $viewer;
      }
    }
  }
  // Fallback to image.
  elseif (!isset($variables['content']) && isset($object['JPG']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JPG'])) {
    $params = array(
      'title' => $object->label,
      'path' => url("islandora/object/{$object->id}/datastream/JPG/view"),
    );
    $variables['content'] = theme('image', $params);
  }
  else {
    $variables['content'] = NULL;
  }
}
