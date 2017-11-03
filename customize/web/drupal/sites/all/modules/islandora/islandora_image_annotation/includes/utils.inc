<?php

/**
 * @file
 * Utility functions for islandora image annotation module.
 */

/**
 * Checks our admin table for an allowed datastream.
 *
 * @param string $pid
 *   The pid of an object.
 *
 * @return string
 *   The DSID of the annotation.
 */
function islandora_image_annotation_get_dsid_from_model($pid) {
  $dsid = 'OBJ';
  $object = islandora_object_load($pid);
  $models = $object->models;
  if (($key = array_search('fedora-system:FedoraObject-3.0', $models)) !== FALSE) {
    unset($models[$key]);
  }
  $mapping = variable_get('islandora_image_annotation_mappings', array());
  foreach ($models as $model) {
    if (isset($mapping[$model])) {
      $tmp_dsid = $mapping[$model]['DSID'];
      if (isset($object[$tmp_dsid])) {
        $dsid = $tmp_dsid;
      }
    }
  }
  return $dsid;
}

/**
 * Shared canvas setup utilitiy.
 *
 * Ajax callback for Shared Canvas, shared functionality in
 * islandora_image_annotation and islandora_critical_edition.
 * Supplies the basic data, including manifest URI. Returns json
 * to be echoed to the client.
 *
 * @param string $pid
 *   The Fedora object id to supply the data for.
 */
function islandora_image_annotation_canvas_init($pid) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $message = islandora_deprecated('7.x-1.4');
  watchdog('islandora_image_annotation', '@msg', array('@msg' => filter_xss($message)));
  module_load_include('inc', 'islandora_image_annotation', 'includes/build_streams');
  module_load_include('inc', 'islandora_image_annotation', 'includes/callbacks');
  module_load_include('module', 'taxonomy', 'module');
  global $base_path;

  $mappings = variable_get('islandora_image_annotation_mappings', array());
  $dropdown = variable_get('islandora_image_annotation_enforce_taxonomy', FALSE);

  $item = islandora_object_load($pid);
  $rels = $item->relationships->get();
  foreach ($rels as $relationship) {
    if ($relationship['predicate']['value'] == 'hasModel') {
      $cmodel = $relationship['object']['value'];
    }
  }
  $vid = isset($mappings[$cmodel]['TAX']) ? $mappings[$cmodel]['TAX'] : NULL;

  if ($dropdown) {
    $terms = taxonomy_get_tree($vid, 0, 1);
    $categories = array();
    foreach ($terms as $term) {
      $categories[] = $term->name;
    }
  }
  else {
    $categories = islandora_image_annotation_get_type_terms();
  }
  $results = array();
  $results['object_base'] = url("islandora/object/$pid");
  $results['no_edit'] = !user_access(ISLANDORA_IMAGE_ANNOTATION_CREATE);
  $results['categories'] = $categories;
  $results['use_dropdown'] = $dropdown;
  // Implemented in JavaScript in RDF, which breaks with URL-encoding.
  $results['manifest_url'] = rawurldecode(url(
    "islandora/anno/serve/$pid/Manifest/manifest.xml",
    array('absolute' => TRUE)));
  $results['islandora_post_url'] = url("islandora/anno/add_annotation/$pid");
  $results['get_annotation_list_url'] = url("islandora/anno/get_urns/$pid");
  $results['islandora_get_annotation'] = url("islandora/anno/get_annotation/");
  $results['islandora_delete_annotation'] = url("islandora/anno/delete_annotation/$pid/");
  $results['islandora_update_annotation'] = url("islandora/anno/update_annotation/");
  $results['islandora_base_url'] = $base_path;
  $results['islandora_anno_use_title_vocab'] = variable_get('islandora_image_annotation_use_title_vocabulary', FALSE);
  $results['islandora_anno_stroke_widths'] = variable_get('islandora_image_annotation_stroke_width', '.3 .6 .9 1.2 1.5');

  return $results;
}

/**
 * A library function for loading Javascript dependencies of shared Canvas.
 *
 * This can be used to support shared canvases in other modules.  They need
 * only call this function rather than include all the Javascript themselves.
 */
function islandora_image_annotation_include_common_js() {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $message = islandora_deprecated('7.x-1.4');
  watchdog('islandora_image_annotation', '@msg', array('@msg' => filter_xss($message)));
  $image_annotation_module_path = drupal_get_path('module', 'islandora_image_annotation');
  $allow_entity_linking = module_exists('islandora_entities') && variable_get('islandora_image_annotation_use_entity_tagging', FALSE);
  $can_choose = variable_get('islandora_image_annotation_choose_color', TRUE);
  drupal_add_js(array('islandora_image_annotation' => array('allow_entity_linking' => $allow_entity_linking)), 'setting');

  $images_path = url($image_annotation_module_path . '/shared_canvas/imgs', array('absolute' => TRUE));
  $entity_link_path = url('islandora/object/', array('absolute' => TRUE));
  drupal_add_js(array('islandora_image_annotation' => array('images_path' => $images_path)), 'setting');
  drupal_add_js(array('islandora_image_annotation' => array('entity_link_path' => $entity_link_path)), 'setting');
  drupal_add_js(array('islandora_image_annotation' => array('entities_search_callback' => url('islandora/entities/search'))), 'setting');
  drupal_add_js(array('islandora_image_annotation' => array('can_choose' => $can_choose)), 'setting');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/entity_search.js');

  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/dialog_annotate.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/jquery/jquery.watermark.min.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/ContextMenu/jquery.contextMenu.js');

  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/jquery/jquery.svg.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/jquery.rdfquery.rdfa.min-1.1.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/jquery.rdf.turtle.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/jquery.touchSwipe-1.2.4.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/raphael.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/scale.raphael.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/uuid.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/js/jquery/jquery.miniColors.js');

  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/sc_ui.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/sc_utils.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/sc_rdf.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/sc_rdfjson.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/sc_create.js');
  drupal_add_js($image_annotation_module_path . '/shared_canvas/stable/sc_pastebin.js');

}

/**
 * A library function for loading CSS dependencies of shared Canvas.
 *
 * This can be used to support shared canvases in other modules.  They need
 * only call this function rather than include all the CSS themselves.
 */
function islandora_image_annotation_include_common_css() {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $message = islandora_deprecated('7.x-1.4');
  watchdog('islandora_image_annotation', '@msg', array('@msg' => filter_xss($message)));
  $image_annotation_module_path = drupal_get_path('module', 'islandora_image_annotation');

  drupal_add_css($image_annotation_module_path . '/shared_canvas/js/ContextMenu/jquery.contextMenu.css');
  drupal_add_css($image_annotation_module_path . '/shared_canvas/css/jquery.miniColors.css');
  drupal_add_css($image_annotation_module_path . '/shared_canvas/css/sc.css');
}

/**
 * Retrieves content models from Fedora.
 *
 * @return array
 *   An array containing key value pairs of content model pids and titles.
 */
function islandora_image_annotation_get_content_models() {
  $query = <<<EOT
select ?object ?title from <#ri> where
  ?object <info:fedora/fedora-system:def/model#hasModel> <info:fedora/fedora-system:ContentModel-3.0> and
  ?object <fedora-model:label> ?title and
  (?object <fedora-view:disseminates> ?ds1 in <#ri> and ?ds1 <fedora-view:disseminationType> <info:fedora/*/DS-COMPOSITE-MODEL> in <#ri>)
  minus ?object <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
  minus ?object  <mulgara:is> <info:fedora/fedora-system:ContentModel-3.0>
  minus ?object  <mulgara:is> <info:fedora/fedora-system:ServiceDefinition-3.0>
  minus ?object  <mulgara:is> <info:fedora/fedora-system:ServiceDeployment-3.0>
  order by ?title
EOT;
  $results = islandora_get_tuque_connection()->repository->ri->itqlQuery($query, 'unlimited', '0');
  $ret = array();
  foreach ($results as $result) {
    $pid = $result['object']['value'];
    $ret[$pid] = $result['title']['value'];
  }
  return $ret;
}

/**
 * Retrieves datastream ids from the DS-COMPOSITE-MODEL.
 *
 * @param string $pid
 *   Pid of the object.
 *
 * @return array
 *   Array containing the DSIDs of datastreams.
 */
function islandora_image_annotation_get_dsids_from_dscomp($pid) {
  module_load_include('inc', 'islandora', 'includes/tuque');
  $fedora_object = islandora_object_load($pid);
  $datastream = $fedora_object['DS-COMPOSITE-MODEL'];
  $ds_comp_stream = $datastream->content;
  $xml = new SimpleXMLElement($ds_comp_stream);
  $output = array();
  foreach ($xml->dsTypeModel as $ds) {
    $mimes = array();
    foreach ($ds->form as $form) {
      $mimetype = (string) $form['MIME'];
      if (strstr($mimetype, 'image')) {
        $mimes[] = $mimetype;
      }
    }
    if (count($mimes) > 0) {
      $output[(string) $ds['ID']] = (string) $ds['ID'];
    }
  }
  return $output;
}

/**
 * Retrieves all taxonomies defined within Drupal.
 *
 * @return array
 *   A key value pair array containing the vid and taxonomy name.
 */
function islandora_image_annotation_get_taxonomies() {
  module_load_include('module', 'taxonomy', 'module');
  $taxonomies = taxonomy_get_vocabularies();
  $output = array(t('None') => 'none');
  if (!empty($taxonomies)) {
    foreach ($taxonomies as $tax) {
      $output[$tax->vid] = $tax->name;
    }
  }
  return $output;
}

/**
 * Get type terms via solr.
 *
 * @return array
 *   Keys of the terms returned from solr.
 */
function islandora_image_annotation_get_type_terms() {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $message = islandora_deprecated('7.x-1.4');
  watchdog('islandora_image_annotation', '@msg', array('@msg' => filter_xss($message)));
  $type_field = variable_get('islandora_image_annotation_solr_field_type', 'RELS_EXT_isAnnotationType_literal_s');
  $query = format_string('!field:("!content_model" OR "info:fedora/!content_model")', array(
    '!field' => variable_get('islandora_solr_content_model_field', 'RELS_EXT_hasModel_uri_ms'),
    '!content_model' => 'islandora:OACCModel',
  ));

  $params = array(
    'fl' => "$type_field, PID",
    'start' => '0',
    'rows' => '10000',
    'qt' => variable_get('islandora_solr_request_handler', 'standard'),
  );

  $results = islandora_image_annotation_query_solr($query, $params);
  $terms = array();
  if (isset($results['solr_doc'])) {
    foreach ($results['solr_doc'] as $doc) {
      if (array_key_exists($type_field, $doc)) {
        $terms[$doc[$type_field][0]] = 1;
      }
    }
  }

  return array_keys($terms);
}

/**
 * Get taxonomy categories.
 */
function islandora_image_annotation_get_categories(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $message = islandora_deprecated('7.x-1.4');
  watchdog('islandora_image_annotation', '@msg', array('@msg' => filter_xss($message)));
  module_load_include('module', 'taxonomy', 'module');
  if (variable_get('islandora_image_annotation_enforce_taxonomy', FALSE)) {
    $rels = $object->relationships->get();
    foreach ($rels as $relationship) {
      if ($relationship['predicate']['value'] == 'hasModel') {
        $content_model = $relationship['object']['value'];
      }
    }
    if ($content_model) {
      $mappings = variable_get('islandora_image_annotation_mappings', array());
      $vid = isset($mappings[$content_model]['TAX']) ? $mappings[$content_model]['TAX'] : NULL;
      $terms = taxonomy_get_tree($vid, 0, 1);
      $categories = array();
      foreach ($terms as $term) {
        $categories[] = $term->name;
      }
      return $categories;
    }
  }
  else {
    return islandora_image_annotation_get_type_terms();
  }
  return array();
}

/**
 * Helper function to query solr.
 *
 * @param string $query
 *   A query string.
 *
 * @param array $params
 *   An associate array containing solr params.
 */
function islandora_image_annotation_query_solr($query, $params) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $message = islandora_deprecated('7.x-1.4');
  watchdog('islandora_image_annotation', '@msg', array('@msg' => filter_xss($message)));
  $query_processor = new IslandoraSolrQueryProcessor();
  $query_processor->solrQuery = $query;
  foreach ($params as $field => $value) {
    if ($field == 'start') {
      $query_processor->solrStart = $value;
    }
    elseif ($field == 'rows') {
      $query_processor->solrLimit = $value;
    }
    else {
      $query_processor->solrParams[$field] = $value;
    }
  }
  $query_processor->executeQuery(FALSE);
  $results = $query_processor->islandoraSolrResult;
  return $results;
}

/**
 * Gets the type values as supplied by solr.
 *
 * @return array
 *   An array of strings each representing a unique type.
 */
function islandora_image_annotation_get_types() {
  if (!module_exists('islandora_solr')) {
    return array();
  }
  $solr_field = variable_get('islandora_image_annotation_solr_field_type', 'RELS_EXT_isAnnotationType_literal_s');
  $data = array(
    'q' => format_string('!field:("info:fedora/!content_model")', array(
      '!field' => variable_get('islandora_solr_content_model_field', 'RELS_EXT_hasModel_uri_ms'),
      '!content_model' => 'islandora:OACCModel',
    )),
    'fl' => $solr_field,
    'start' => '0',
    'rows' => '10000',
    'wt' => 'json',
    'qt' => variable_get('islandora_solr_request_handler', 'standard'),
    'sort' => "$solr_field asc",
  );
  $solr = variable_get('islandora_solr_url', 'localhost:8080/solr');
  $solr_url = url($solr . '/select', array('query' => $data));
  $result_request = drupal_http_request($solr_url);
  // Test whether the reponse status code is 200.
  if ($result_request->code == 200) {
    $results = json_decode($result_request->data, TRUE);
    $types = array();
    if (isset($results['response']['docs'])) {
      foreach ($results['response']['docs'] as $doc) {
        $types[] = $doc[$solr_field];
      }
    }
    $types = array_unique($types);
    sort($types, SORT_STRING);
    return $types;
  }
  else {
    return array();
  }
}

/**
 * Helper function that lists all of the comments annotations.
 *
 * @param AbstractObject $object
 *   The object in which the annotations are targeted at.
 *
 * @return array
 *   An associative array where each key is an annotation PID.
 *   - pid: The PID for a annotation.
 *   - type: The type of the annotation.
 */
function islandora_image_annotation_get_annotations(AbstractObject $object) {
  $query = <<<QUERY
    PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
    PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
    SELECT ?pid ?type from <#ri> WHERE {
      ?pid islandora-rels-ext:targets <info:fedora/{$object->id}> ;
        islandora-rels-ext:isAnnotationType ?type;
        fedora-model:state fedora-model:Active;
    }
    ORDER BY ASC(?type)
QUERY;
  $results = $object->repository->ri->sparqlQuery($query);
  $return = array();
  foreach ($results as $result) {
    $pid = $result['pid']['value'];
    $type = $result['type']['value'];
    $return[$pid] = array(
      'pid' => $pid,
      'type' => $type,
    );
  }
  return $return;
}

/**
 * Fetches the settings for all the widgets.
 *
 * @param AbstractObject $object
 *   The object to annotate.
 *
 * @return array
 *   The Drupal.settings for each Image Annotation Widget.
 */
function islandora_image_annotation_get_settings(AbstractObject $object) {
  $settings = &drupal_static(__FUNCTION__);
  if (!isset($settings[$object->id])) {
    $settings[$object->id] = array(
      'islandoraImageAnnotation' => array(
        'pid' => $object->id,
      ),
      'islandoraImageAnnotationDialog' => array(
        'pid' => $object->id,
        // Types is used in the autocomplete for the "Types" field.
        'types' => islandora_image_annotation_get_types(),
        'allowEntityLinking' => module_exists('islandora_entities') && variable_get('islandora_image_annotation_use_entity_tagging', FALSE),
        'useTitleVocabulary' => variable_get('islandora_image_annotation_use_title_vocabulary', FALSE),
        'canChooseColour' => !variable_get('islandora_image_annotation_choose_color', TRUE),
        'dialog' => array(
          'dialogClass' => 'islandora-image-annotation',
        ),
      ),
      'islandoraImageAnnotationList' => array(
        'pid' => $object->id,
        'editable' => user_access(ISLANDORA_IMAGE_ANNOTATION_CREATE),
        'annotations' => islandora_image_annotation_get_annotations($object),
      ),
    );
  }
  return $settings[$object->id];
}
