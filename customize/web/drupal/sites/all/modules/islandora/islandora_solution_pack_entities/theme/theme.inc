<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_person(array &$variables) {
  module_load_include('inc', 'islandora_entities', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/metadata');
  module_load_include('inc', 'islandora_entities', 'includes/entities_rss');
  module_load_include('inc', 'islandora', 'includes/utilities');
  drupal_add_css(drupal_get_path('module', 'islandora_entities') . '/css/islandora-person.css');

  $biography = "";
  $object = $variables['object'];
  $object_pid = $object->id;

  if ($object['MADS']) {
    $mads = $object['MADS']->content;
    $simplexml = simplexml_load_string($mads);
    $notes = $simplexml->note;
    foreach ($notes as $note) {
      if ($note['type'] == 'history') {
        $biography = (string) $note;
        $variables['biography'] = str_replace("\n", "<br />", $biography);
      }
    }
    $variables['title'] = (string) $simplexml->authority->titleInfo->title;
    $activities = $simplexml->fieldOfActivity;
    foreach ($activities as $activity) {
      if ((string) $activity) {
        $fields_of_activity[] = (string) $activity;
      }
    }
    if (isset($fields_of_activity)) {
      $list_variables = array(
        'items' => $fields_of_activity,
        'title' => t('Research Interests'),
        'type' => 'ul',
        'attributes' => array('class' => 'research_interests'),
      );
      $variables['activities'] = theme_item_list($list_variables);
    }
    $depts = $simplexml->affiliation->organization;
    // It is possible there is no affiliation.
    if ($depts) {
      $colleages = array();
      foreach ($depts as $dept) {
        if ((string) $dept) {
          $members = islandora_entities_get_dept_members(
            (string) $dept,
            $object->id
          );
          if ($members) {
            $colleages[(string) $dept] = $members;
          }
        }
      }
      if (count($colleages) > 0) {
        foreach ($colleages as $dept => $members) {
          $links = array();
          foreach ($members as $pid => $scholar) {
            if (islandora_namespace_accessible($pid)) {
              $links[] = l($scholar, "islandora/object/$pid/view");
            }
          }
          $list_variables = array(
            'items' => $links,
            'title' => t('Other Scholars in @dept', array('@dept' => $dept)),
            'type' => 'ul',
            'attributes' => array('class' => 'fellow_scholars'),
          );
          $variables['fellow_scholars'][] = theme_item_list($list_variables);
        }
      }
    }
    $identifiers = $simplexml->identifier;
    foreach ($identifiers as $identifier) {
      if ($identifier['type'] == 'u1' && (string) $identifier) {
        $themed_links = islandora_entities_get_related((string) $identifier, $variables['title'], 'citations');
      }
    }
    $name = $variables['title'];
    // Add the RSS image, with menu callback.
    $form = array();
    $form['bookmark_rss'] = array(
      '#weight' => -1,
      '#markup' => l(theme('image', array(
          'path' => drupal_get_path('module', 'islandora_entities') . '/images/rss.png',
          'title' => t('Present Entity data as list in an RSS form'),
          'attributes' => array(),
          )
        ), "islandora/entities/$object_pid/rss", array('html' => TRUE)),
      '#prefix' => '<div><div id="islandora-entities-rss-format" style="float:left;">',
      '#suffix' => '</div><div class="islandora-entities-rss-format-name" style="float:left;">Subscribe to ' . $name . ' RSS feed</div></div>',
    );
    $variables['rss_feed'] = drupal_render($form);
    if (!empty($themed_links)) {
      $variables['recent_citations'] = $themed_links;
    }
  }
  if ($object['TN']) {
    $variables['tn'] = url("islandora/object/{$object->id}/datastream/TN/view");
  }
  $variables['metadata'] = islandora_retrieve_metadata_markup($object, TRUE);
}

/**
 * Gets all members of department.
 *
 * @param string $dept
 *   Department being queried
 * @param string $pid
 *   Object pid, results will be excluded
 *
 * @return array
 *   Department members
 */
function islandora_entities_get_dept_members($dept, $pid) {
  module_load_include('inc', 'islandora_solr', 'includes/query_processor');
  $query = "MADS_organization_ms:\"$dept\" AND -PID:\"$pid\"";
  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery($query);
  $qp->solrParams['fl'] = "MADS_title_ms, PID";
  $qp->executeQuery(FALSE);
  try {
    $results = $qp->islandoraSolrResult['response']['objects'];
  }
  catch (Exception $e) {
    watchdog_exception('Islandora Entities', $e, 'Got an exception while searching entities for callback.', array(), WATCHDOG_ERROR);
    $results = array();
  }
  $colleages = array();
  if ($results) {
    foreach ($results as $choice) {
      if (isset($choice['solr_doc']['MADS_title_ms'])) {
        foreach ($choice['solr_doc']['MADS_title_ms'] as $candidate) {
          $colleages[$choice['PID']] = $candidate;
        }
      }
    }
  }
  return $colleages;
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_islandora_dept(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/metadata');
  $module_path = drupal_get_path('module', 'islandora');
  drupal_add_css("$module_path/css/islandora.objects.css");
  $object = $variables['object'];
  $variables['metadata'] = islandora_retrieve_metadata_markup($object, TRUE);
  if ($object['TN']) {
    $variables['tn'] = url("islandora/object/{$object->id}/datastream/TN/view");
  }
}
