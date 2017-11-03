<?php
/**
 * @file
 * Breadcrumb functions.
 */

/**
 * Implements callback_islandora_breadcrumbs_backends.
 */
function islandora_solr_islandora_breadcrumbs_backends_callable($object) {
  $breadcrumbs = islandora_solr_get_breadcrumbs_recursive($object->id);
  // Pop off the current object.
  array_pop($breadcrumbs);
  $query_backend = variable_get('islandora_basic_collection_display_backend', 'islandora_basic_collection_legacy_sparql');
  if (variable_get('islandora_solr_breadcrumbs_add_collection_query', FALSE) && $query_backend == 'islandora_solr_query_backend') {
    global $_islandora_solr_queryclass;
    if (isset($_islandora_solr_queryclass)) {
      $results = new IslandoraSolrResults();
      $qp_breadcrumbs = $results->getBreadcrumbs($_islandora_solr_queryclass);
      // Slice out the first two breadcrumbs, as they are 'Home' and the
      // breadcrumb query predicate.
      return array_merge($breadcrumbs, array_slice($qp_breadcrumbs, 2));
    }
  }
  return $breadcrumbs;
}

/**
 * Builds an array of drupal links for use in breadcrumbs.
 *
 * @todo Make fully recursive...
 *
 * @param string $pid
 *   The object id whose parent will be fetched for the next link.
 * @param array $context
 *   An associative array of context for internal use when recursing. Currently
 *   only used to track a single value:
 *   - level: The number of child-parent relationships to follow. Defaults to
 *     10.
 *
 * @return array
 *   An array of links representing the breadcrumb trail, "root" first.
 */
function islandora_solr_get_breadcrumbs_recursive($pid, array &$context = NULL) {
  if ($context === NULL) {
    $context['level'] = 10;
  }
  if (!isset($context['root'])) {
    $root = variable_get('islandora_repository_pid', 'islandora:root');
    $context['root'] = $root;
  }
  if ($pid == $context['root']) {
    $title = 'Islandora Repository';
    $mlid = db_select('menu_links', 'ml')
      ->condition('ml.link_path', 'islandora')
      ->fields('ml', array('mlid'))
      ->execute()
      ->fetchField();
    if ($mlid) {
      $link = menu_link_load($mlid);
      $title = (isset($link['title']) ? $link['title'] : $title);
    }
    return array(
      l(t('Home'), '<front>'),
      l($title, 'islandora'),
    );
  }
  else {
    $parents = islandora_solr_get_breadcrumb_parent($pid);
    if ($parents) {
      $this_label = $parents['label'];
      $parent = $parents['PID'];
    }
    if (empty($this_label)) {
      $this_label = t('-');
    }
    if (isset($parent)) {
      // We found a parent.
      $context['level']--;
      return array_merge(islandora_solr_get_breadcrumbs_recursive($parent, $context), array(
        l($this_label, "islandora/object/$pid"),
      ));
    }
    else {
      // Add an non-link, as we don't know how to get back to the root, and
      // render the last two links and break (on the next pass).
      return array_merge(
        islandora_solr_get_breadcrumbs_recursive($context['root'], $context),
        array('...')
      );
    }
  }
}

/**
 * Get the parent object using the configured Solr fields.
 *
 * @param string $pid
 *   The pid of the object.
 *
 * @return array
 *   Array with the parent pid and current object label.
 */
function islandora_solr_get_breadcrumb_parent($pid) {
  module_load_include('inc', 'islandora_solr', 'includes/query_processor');
  $solr_build = new IslandoraSolrQueryProcessor();
  $solr_query = "PID:(" . islandora_solr_lesser_escape($pid) . " OR " .
    islandora_solr_lesser_escape("info:fedora/" . $pid) . ")";
  $parent_fields = preg_split("/\\r\\n|\\n|\\r/", variable_get('islandora_solr_breadcrumbs_parents', "RELS_EXT_isMemberOfCollection_uri_ms\r\nRELS_EXT_isMemberOf_uri_ms"), -1, PREG_SPLIT_NO_EMPTY);
  $solr_params = array(
    'fl' => implode(",", $parent_fields) . ',fgs_label_s,PID',
    'hl' => 'false',
    'facet' => 'false',
  );
  $solr_build->buildQuery($solr_query, $solr_params);
  $solr_build->solrParams = array_replace_recursive($solr_build->solrParams, $solr_params);
  $solr_build->solrParams = islandora_solr_remove_base_filters($solr_build->solrParams);
  $solr_build->solrParams = islandora_solr_clean_compound_filters($solr_build->solrParams);

  try {
    $solr_build->executeQuery(FALSE);
    $results = (array) $solr_build->islandoraSolrResult['response']['objects'];
  }
  catch (Exception $e) {
    $results = array();
    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error', FALSE);
  }

  $find_solr_value = function($o, $field) {
    if (isset($o[$field])) {
      return $o[$field];
    }
    elseif (isset($o['solr_doc'][$field])) {
      return $o['solr_doc'][$field];
    }
    else {
      return NULL;
    }
  };

  if ($results) {
    foreach ($parent_fields as $field) {
      $field_value = $find_solr_value($results[0], $field);
      if ($field_value) {
        if (is_array($field_value)) {
          $parent_pid = reset($field_value);
        }
        else {
          $parent_pid = $field_value;
        }
        // Strip prefix if there is one.
        $parent_pid = str_replace("info:fedora/", "", $parent_pid);
        return array(
          'PID' => $parent_pid,
          'label' => $find_solr_value($results[0], 'fgs_label_s'),
        );
      }
    }
  }
  return array();
}

/**
 * To trace breadcrumbs isConstituent through Solr we need to remove filters.
 *
 * @param array $params
 *   The Solr params array.
 *
 * @return array
 *   The $params array minus any base filters.
 */
function islandora_solr_clean_compound_filters(array $params) {
  if (module_exists('islandora_compound_object') && is_array($params) && isset($params['fq'])) {
    $compound_filters = array(variable_get('islandora_compound_object_solr_fq', '-RELS_EXT_isConstituentOf_uri_mt:[* TO *]'));
    $params['fq'] = array_diff($params['fq'], $compound_filters);
  }
  return $params;
}
