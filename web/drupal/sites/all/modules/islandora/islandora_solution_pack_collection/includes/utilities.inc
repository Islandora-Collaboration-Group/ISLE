<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Converts given model descriptions into Drupal form select element #options.
 *
 * @see islandora_get_content_models()
 *
 * @param array $content_models
 *   A list of content models as returned by islandora_get_content_models().
 *
 * @return array
 *   An associative array of content models for use in Drupal form select
 *   element options:
 *   - pid: The PID of the content model
 *     - label: The label to display to the user.
 */
function islandora_basic_collection_get_content_models_as_form_options(array $content_models) {
  $map_content_models_to_options = function($o) {
    $label = isset($o['label']) ? $o['label'] : $o['name'];
    return format_string('!pid ~ !label', array(
      '!pid' => $o['pid'],
      '!label' => $label,
    ));
  };
  return array_map($map_content_models_to_options, $content_models);
}

/**
 * Lists collections for use in a Drupal form select element #options.
 *
 * Ignores the given collection.
 *
 * @see islandora_get_collections()
 *
 * @param AbstractObject $object
 *   A collection object to ignore.
 *
 * @return array
 *   An associative array of collections for use in Drupal form select
 *   element options:
 *   - pid: The PID of the collection object.
 *     - label: The label to display to the user.
 */
function islandora_basic_collection_get_other_collections_as_form_options(AbstractObject $object) {
  $map_options = function($o) {
    return filter_xss($o['label']);
  };
  $options = array_map($map_options, islandora_basic_collection_get_collections());
  // Ignore given collection.
  unset($options[$object->id]);
  return $options;
}

/**
 * Gets a choose namespace element for use in the collection management forms.
 *
 * @param string $default_value
 *   The default value to use for the namespace field.
 *
 * @return array
 *   A Drupal form element for selecting the a namespace.
 */
function islandora_basic_collection_get_namespace_form_element($default_value) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $restrict_namespace = variable_get('islandora_namespace_restriction_enforced', FALSE);
  return array(
    '#type' => $restrict_namespace ? 'select' : 'textfield',
    '#title' => 'Children namespace',
    '#description' => t('Namespace for objects with selected content models.'),
    '#size' => 15,
    '#options' => $restrict_namespace ? drupal_map_assoc(islandora_get_allowed_namespaces()) : NULL,
    '#default_value' => $default_value,
  );
}

/**
 * A Drupal form select table populated with the given collection's children.
 *
 * @param AbstractObject $object
 *   The collection object.
 * @param array $pager_options
 *   An associative array containing options to pass onto the pager.
 *   - element: The numeric ID of the pager
 *   - fragment: The url fragment to append onto pager links, includes '#'.
 *
 * @return array
 *   A Drupal select table form element.
 */
function islandora_basic_collection_get_children_select_table_form_element(AbstractObject $object, array $pager_options) {
  // Assumes all results are returned although the function description
  // states otherwise.
  $limit = variable_get('islandora_basic_collection_admin_page_size', 10);
  $page = pager_find_page($pager_options['element']);
  list($count, $results) = islandora_basic_collection_get_member_objects($object, $page, $limit, 'manage');
  $page = pager_default_initialize($count, $limit, $pager_options['element']);
  $rows = array();
  foreach ($results as $result) {
    $pid = $result['object']['value'];
    $label = empty($result['title']['value']) ? t('Untitled') : filter_xss($result['title']['value']);
    $label = $label . " (" . $pid . ")";
    $owner = empty($result['owner']['value']) ? t('Unowned') : filter_xss($result['owner']['value']);
    $date_modified = empty($result['date_modified']['value']) ? t('Unknown') : filter_xss($result['date_modified']['value']);
    $rows[$pid] = array(
      'owner' => array(
        'data' => array(
          '#markup' => '<div>' . $owner . '</div>',
        ),
      ),
      'date_modified' => array(
        'data' => array(
          '#markup' => '<div>' . $date_modified . '</div>',
        ),
      ),
      'label' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $label,
          '#href' => "islandora/object/{$pid}")));
  }
  $pager = theme('pager', array('quantity' => $limit, 'element' => $pager_options['element']));
  $pager = islandora_basic_collection_append_fragment_to_pager_url($pager, $pager_options['fragment']);
  return array(
    '#type' => 'tableselect',
    '#header' => array(
      'label' => array('data' => t('Label')),
      'owner' => array('data' => t('Owner')),
      'date_modified' => array('data' => t('Date Modified'))),
    '#options' => $rows,
    '#empty' => t('Collection is empty.'),
    '#prefix' => $pager,
    '#suffix' => $pager,
  );
}

/**
 * Gets a list of all parent pids.
 *
 * @param AbstractObject $object
 *   The object.
 *
 * @return array
 *   The list of parent PIDs.
 */
function islandora_basic_collection_get_parent_pids(AbstractObject $object) {
  $results = array_merge(
      $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection'),
      $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOf'));
  $map_results = function($o) {
    return $o['object']['value'];
  };
  $collections = array_map($map_results, $results);
  return array_unique(array_filter($collections));
}

/**
 * Gets a list of all parent pids excluding the given parent.
 *
 * @param AbstractObject $object
 *   The object.
 * @param AbstractObject $parent
 *   The parent of the object.
 *
 * @return array
 *   The list of parent PIDs.
 */
function islandora_basic_collection_get_other_parents(AbstractObject $object, AbstractObject $parent) {
  $parents = islandora_basic_collection_get_parent_pids($object);
  unset($parents[array_search($parent->id, $parents)]);
  return $parents;
}

/**
 * Adds the given object to the given collection.
 *
 * @param AbstractObject $new_member
 *   The object to add.
 * @param AbstractObject $collection
 *   The collection object to add to.
 */
function islandora_basic_collection_add_to_collection(AbstractObject $new_member, AbstractObject $collection) {
  $results = $new_member->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection->id);
  if (empty($results)) {
    $new_member->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection->id);
  }
}

/**
 * Removes the given object from the given collection.
 *
 * @param AbstractObject $member
 *   The object to remove.
 * @param AbstractObject $collection
 *   The collection object to remove from.
 */
function islandora_basic_collection_remove_from_collection(AbstractObject $member, AbstractObject $collection) {
  $member->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection->id);
  $member->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOf', $collection->id);
}

/**
 * Searches through available collection objects.
 *
 * @param string $search_value
 *   Returns a json array of matching collections.
 */
function islandora_basic_collection_get_collections_filtered($search_value) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $tuque = islandora_get_tuque_connection();
  $sparql_query = <<<EOQ
SELECT ?pid ?label
WHERE {
  ?pid <fedora-model:label> ?label ;
       <fedora-model:hasModel> <info:fedora/islandora:collectionCModel> .
  FILTER(regex(?label, '$search_value', 'i') || regex(str(?pid), '$search_value', 'i'))
}
EOQ;
  $results = $tuque->repository->ri->sparqlQuery($sparql_query);
  $return = array();
  foreach ($results as $objects) {
    $pid = $objects['pid']['value'];
    if (islandora_namespace_accessible($pid)) {
      $return[$pid] = t('@label (@pid)', array(
        '@label' => $objects['label']['value'],
        '@pid' => $pid,
      ));
    }
  }
  drupal_json_output($return);
}

/**
 * Implements callback_islandora_basic_collection_query_backends().
 */
function islandora_basic_collection_display_query_sparql($collection_object, $page, $limit) {
  list($total, $results) = islandora_basic_collection_get_member_objects($collection_object, $page, $limit);
  $map_to_pids = function ($result) {
    return $result['object']['value'];
  };
  return array($total, array_map($map_to_pids, $results));
}

/**
 * Helper function to add fragment identifiers to pager URLs.
 *
 * Theme pager doesn't support url fragments in D7 so we insert manually.
 *
 * @param string $pager
 *   The pager markup to be rendered.
 * @param string $fragment
 *   The fragment identifier to be appended.
 *
 * @return string
 *   The pager markup to be rendered with fragment.
 */
function islandora_basic_collection_append_fragment_to_pager_url($pager, $fragment) {
  $pattern = '/href="([^"]+)"/';
  $replace = format_string('href="\1!fragment"', array('!fragment' => $fragment));
  $pager = preg_replace($pattern, $replace, $pager);
  return $pager;
}
