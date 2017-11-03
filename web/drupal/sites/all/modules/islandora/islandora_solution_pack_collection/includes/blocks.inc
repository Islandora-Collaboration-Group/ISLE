<?php
/**
 * @file
 * Provides block functionality for the basic collection module.
 */

/**
 * Constructs the contents of the object count block.
 *
 * @return string
 *   The markup to be rendered inside the block.
 */
function islandora_basic_collection_object_count_listing_content() {
  $block = array();
  $id = drupal_html_id('islandora_basic_collection_object_count_block_count');
  $block['counts'] = array(
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'islandora_basic_collection') . '/js/collection-block-count.js',
        array(
          'type' => 'setting',
          'data' => array(
            'islandora_basic_collection' => array(
              'count_block' => array(
                'id' => $id,
                'callback' => url('islandora/collection/count_ajax'),
              ),
            ),
          ),
        ),
      ),
    ),
    '#prefix' => "<span id='$id'>",
    '#suffix' => '</span>',
    '#markup' => variable_get('islandora_basic_collection_object_count_listing_placeholder', 'Counting items in collections...'),
  );
  return $block;
}

/**
 * Build an ID for caching.
 *
 * Grabbed the internals of _block_get_cache_id(): Since we're not setting
 * the general block cache to be used, we cannot depend call it.
 *
 * @return string
 *   The ID to use for the object count block.
 */
function islandora_basic_collection_get_object_count_block_cache_id() {
  $block = block_load('islandora_basic_collection', 'collection_object_count');

  $cid_parts[] = $block->module;
  $cid_parts[] = $block->delta;
  $cid_parts = array_merge($cid_parts, drupal_render_cid_parts($block->cache));

  return implode(':', $cid_parts);
}

/**
 * Clear the cache of content for the object count block.
 */
function islandora_basic_collection_clear_object_count_cache() {
  $block = block_load('islandora_basic_collection', 'collection_object_count');

  $cid_parts[] = $block->module;
  $cid_parts[] = $block->delta;

  cache_clear_all(implode(':', $cid_parts), 'cache', TRUE);
}

/**
 * AJAX callback to get info about the count of objects and collections.
 *
 * Outputs a JSON string to the buffer.
 */
function islandora_basic_collection_object_count_callback() {
  $cid = islandora_basic_collection_get_object_count_block_cache_id();

  if ($value = cache_get($cid, 'cache')) {
    $substitutions = $value->data;
  }
  else {
    $tuque = islandora_get_tuque_connection();
    $objects_query_array = islandora_basic_collection_get_query_info(array(
      'object' => islandora_object_load(variable_get('islandora_repository_pid', 'islandora:root')),
      'collection_listing' => TRUE,
      'all_objects' => TRUE,
    ));
    $collection_query_array = islandora_basic_collection_get_query_info(array(
      'object' => islandora_object_load(variable_get('islandora_repository_pid', 'islandora:root')),
      'collection_listing' => TRUE,
      'all_objects' => FALSE,
    ));
    $collection_objects = $tuque->repository->ri->sparqlQuery($collection_query_array['query'], $collection_query_array['type']);
    $total_object_count = $tuque->repository->ri->countQuery($objects_query_array['query'], $objects_query_array['type']);

    $collections = array();
    foreach ($collection_objects as $collection) {
      $collections[$collection['object']['value']] = $collection['object']['value'];
    }
    $models_to_exclude = variable_get('islandora_basic_collection_object_count_listing_content_models_to_restrict', FALSE);
    if ($models_to_exclude) {
      $collections = islandora_basic_collection_filter_collection_by_cmodel($collections, array_filter($models_to_exclude));
    }
    $total_collection_count = count($collections);

    $substitutions = array(
      '!objects' => $total_object_count,
      '!collections' => $total_collection_count,
    );
    cache_set($cid, $substitutions, 'cache');
  }

  $title_phrase = variable_get('islandora_basic_collection_object_count_listing_phrase', '!objects Items in !collections Collections');
  $text = format_string($title_phrase, $substitutions);
  drupal_json_output($text);
}

/**
 * Constructs the content of the collection listing block.
 *
 * @return string
 *   The markup to be rendered in the block.
 */
function islandora_basic_collection_collection_listing_content() {
  $block = array();
  $tuque = islandora_get_tuque_connection();
  $collection_query_array = islandora_basic_collection_get_query_info(array(
    'object' => islandora_object_load('islandora:root'),
    'collection_listing' => TRUE,
    'all_objects' => FALSE,
  ));
  $collections_to_display = variable_get('islandora_basic_collection_listing_block_links_to_render', 10);
  $collection_objects = $tuque->repository->ri->sparqlQuery($collection_query_array['query'], $collection_query_array['type']);
  $collections = array();
  $stored_collections = array();
  foreach ($collection_objects as $collection) {
    $pid = $collection['object']['value'];
    $collections[$pid] = $pid;
    $stored_collections[$pid] = $collection['title']['value'];
  }
  $models_to_exclude = variable_get('islandora_basic_collection_listing_block_content_models_to_restrict', FALSE);
  if ($models_to_exclude) {
    $collections = islandora_basic_collection_filter_collection_by_cmodel($collections, array_filter($models_to_exclude));
  }

  $formatted_objects = array();
  foreach ($collections as $pid) {
    $formatted_objects[$pid] = l($stored_collections[$pid], "islandora/object/$pid");
  }
  $block['collections'] = array(
    '#theme' => 'item_list',
    '#items' => array_slice($formatted_objects, 0, $collections_to_display),
    '#type' => 'ul',
  );
  $root_pid = variable_get('islandora_repository_pid', 'islandora:root');
  $block['browse_repository'] = array(
    '#type' => 'item',
    '#markup' => l(t('Browse repository'), "islandora/object/$root_pid"),
    '#attributes' => array(
      'class' => array('islandora-basic-collection-read-more'),
    ),
  );
  return drupal_render($block);
}


/**
 * Filters out collections that only contain included content models.
 *
 * @param array $collections
 *   An array of PIDs representing collection objects.
 * @param array $models
 *   An array of content models to be included.
 *
 * @return array
 *   An array of collections to be used.
 */
function islandora_basic_collection_filter_collection_by_cmodel($collections, $models) {
  $collection_filter = function ($collection) use ($models) {
    $tuque = islandora_get_tuque_connection();
    $models_query = <<<EOQ
    SELECT DISTINCT ?model from <#ri> where {
?object <fedora-model:hasModel> ?model ;
        <fedora-rels-ext:isMemberOfCollection> <info:fedora/{$collection}>
}
EOQ;
    $models_results = $tuque->repository->ri->sparqlQuery($models_query);
    $collection_models = array();
    foreach ($models_results as $result) {
      $collection_models[$result['model']['value']] = $result['model']['value'];
    }
    return array_intersect($collection_models, $models);
  };
  $filtered_collections = array_filter($collections, $collection_filter);
  return $filtered_collections;
}
