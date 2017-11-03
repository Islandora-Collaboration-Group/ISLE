<?php

/**
 * @file
 * Holds helper functions for islandora_entities.
 */

/**
 * This function will add the entity content model to an object.
 *
 * Due to natural inheritance in the data model all entity type
 * objects will have the content model of entity as well as one
 * for their types.
 *
 * @param AbstractObject $fedora_object
 *   The object to add the relationship to.
 */
function islandora_entities_add_entity_content_model(AbstractObject $fedora_object) {
  $fedora_object->relationships->add(
      FEDORA_MODEL_URI, 'hasModel', 'islandora:entityCModel'
  );
}

/**
 * This function will run a query using Solr for the specified entity type.
 *
 * @param string $content_model
 *   The PID of the content model object representing they type of entity.
 * @param string $entities_query
 *   The value of the query to run for the Solr search.
 *
 * @return array
 *   The results of the query. Limited to PID, object label and EAC-CPF name.
 *   IslandoraSolrQueryProcessor->islandoraSolrResult
 */
function islandora_entities_solr_search_entities($content_model, $entities_query) {
  $content_model_field = variable_get(
      'islandora_solr_content_model_field', 'RELS_EXT_hasModel_uri_ms'
  );
  $entities_query_field = variable_get(
      'islandora_entities_query_field', 'eaccpf_name_complete_et'
  );
  $object_label_field = variable_get(
      'islandora_solr_object_label_field', 'fgs_label_s'
  );

  $query_processor = new IslandoraSolrQueryProcessor();
  $query_processor->solrQuery = format_string(
      '!entities_query_field:(!entities_query)', array(
        '!entities_query_field' => $entities_query_field,
        '!entities_query' => islandora_solr_lesser_escape($entities_query),
      )
  );
  $query_processor->solrStart = 0;
  $query_processor->solrLimit = 10;
  $query_processor->solrParams = array(
    'fl' => "$entities_query_field, $object_label_field, PID",
    'fq' => format_string(
        '!content_model_field:"info:fedora/!content_model"', array(
          '!content_model_field' => $content_model_field,
          '!content_model' => $content_model,
        )
    ),
  );

  $query_processor->executeQuery(FALSE);
  $solr_results = $query_processor->islandoraSolrResult;
  return $solr_results;
}

/**
 * This function will add an entity DS composite datastream to an object.
 *
 * This is a workaround of a known issue with temp files being lost in Tuque in
 * batches because of object destruction during the serialization process.
 *
 * @param AbstractObject $islandora_object
 *   The object to add the DS composite datastream to.
 */
function islandora_entities_ingest__entity_ds_composite_datastream(AbstractObject $islandora_object) {
  $module_path = drupal_get_path('module', 'islandora_entities');

  $ds_composite_datastream = $islandora_object->constructDatastream('DS-COMPOSITE-MODEL', 'X');
  $ds_composite_datastream->label = 'DS-COMPOSITE-MODEL';
  $ds_composite_datastream->mimetype = 'text/xml';
  $ds_composite_datastream->setContentFromFile("$module_path/xml/islandora_entity_ds_composite_model.xml", FALSE);
  $islandora_object->ingestDatastream($ds_composite_datastream);
}

/**
 * Creates themed list of scholar's citations.
 *
 * @param AbstractObject $object
 *   Object representing scholar
 *
 * @return html
 *   themed list of citations
 */
function islandora_entities_create_citation_tab(AbstractObject $object) {
  module_load_include('inc', 'islandora_entities', 'includes/utilities');
  if ($object['MADS']) {
    $mads = $object['MADS']->content;
    $simplexml = simplexml_load_string($mads);
    $identifiers = $simplexml->identifier;
    foreach ($identifiers as $identifier) {
      // PHP 5.3 and 5.4 can't handle empty((string) $identifier).
      $string_id = (string) $identifier;
      if ($identifier['type'] == 'u1' && !empty($string_id)) {
        return islandora_entities_get_related($string_id, (string) $simplexml->authority->titleInfo->title, 'citations');
      }
    }
  }
}

/**
 * Gets all citations related to scholars's unique identifier.
 *
 * @param string $identifier
 *   Scholar's unique ID
 *
 * @return array
 *   Array of links pointing to citations
 */
function islandora_entities_get_related($identifier, $title, $type) {
  $base_query = array(
    'citations' => variable_get(
      'islandora_entities_citation_base_query',
      '+RELS_EXT_hasModel_uri_mt:"ir:citationCModel"'),
    'theses' => variable_get(
      'islandora_entities_thesis_base_query',
      '+RELS_EXT_hasModel_uri_mt:"ir:thesisCModel"'),
  );
  $entities_linking_field = variable_get(
      'islandora_entities_linking_content_solr_field', 'mods_name_personal_displayForm_mt'
  );
  $query = "+{$entities_linking_field}:\"($identifier)\" AND {$base_query[$type]}";
  $params = array(
    'limit' => variable_get('islandora_entities_citation_number', '20'),
  );
  if (strlen(variable_get('islandora_entities_query_sort', '')) > 0) {
    $params['sort'] = variable_get('islandora_entities_query_sort', '');
  }

  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery($query, $params);
  $qp->solrParams['fl'] = "dc.title, PID";
  $qp->executeQuery(FALSE);
  try {
    $results = $qp->islandoraSolrResult['response']['objects'];
  }
  catch (Exception $e) {
    watchdog_exception('Islandora Entities', $e, 'Got an exception while searching entities for callback.', array(), WATCHDOG_ERROR);
    $results = array();
  }
  if ($results) {
    $links = array();
    foreach ($results as $choice) {
      if (isset($choice['solr_doc']['dc.title'])) {
        $links[] = l($choice['solr_doc']['dc.title'][0], 'islandora/object/' . $choice['PID']);
      }
    }
    $list_variables = array(
      'items' => $links,
      'title' => t('Recent @type for @scholar', array(
        '@type' => drupal_ucfirst($type),
        '@scholar' => $title,
      )),
      'type' => 'ul',
      'attributes' => array('class' => 'recent_citations'),
    );
    return theme_item_list($list_variables);
  }
  return NULL;
}

/**
 * Creates themed list of scholar's citations.
 *
 * @param AbstractObject $object
 *   Object representing scholar
 *
 * @return html
 *   themed list of citations
 */
function islandora_entities_create_theses_tab(AbstractObject $object) {
  module_load_include('inc', 'islandora_entities', 'includes/utilities');
  if ($object['MADS']) {
    $mads = $object['MADS']->content;
    $simplexml = simplexml_load_string($mads);
    $identifiers = $simplexml->identifier;
    foreach ($identifiers as $identifier) {
      // PHP 5.3 and 5.4 can't handle empty((string) $identifier).
      $string_id = (string) $identifier;
      if ($identifier['type'] == 'u1' && !empty($string_id)) {
        return islandora_entities_get_related($string_id, (string) $simplexml->authority->titleInfo->title, 'theses');
      }
    }
  }
}

/**
 * Gets all citations related to scholars's unique identifier.
 *
 * @param string $identifier
 *   Scholar's unique ID
 *
 * @return array
 *   Array of links pointing to citations
 */
function islandora_entities_get_related_pids($identifier, $title, $type) {
  $base_query = array(
    'citations' => variable_get(
      'islandora_entities_citation_base_query',
      '+RELS_EXT_hasModel_uri_mt:"ir:citationCModel"'),
    'theses' => variable_get(
      'islandora_entities_thesis_base_query',
      '+RELS_EXT_hasModel_uri_mt:"ir:thesisCModel"'),
  );
  $entities_linking_field = variable_get(
      'islandora_entities_linking_content_solr_field', 'mods_name_personal_displayForm_mt'
  );
  $author_field = variable_get(
      'islandora_entities_citation_author_solr_field', 'mods_name_personal_displayForm_mt'
  );
  $query = "+{$entities_linking_field}:\"($identifier)\" AND {$base_query[$type]}";

  $params = array(
    'limit' => variable_get('islandora_entities_citation_number', '20'),
  );
  if (strlen(variable_get('islandora_entities_query_sort', '')) > 0) {
    $params['sort'] = variable_get('islandora_entities_query_sort', '');
  }

  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery($query, $params);
  $qp->solrParams['fl'] = "dc.title, {$author_field}, PID";
  $qp->executeQuery(FALSE);
  try {
    $search_results = $qp->islandoraSolrResult['response']['objects'];
  }
  catch (Exception $e) {
    watchdog_exception('Islandora Entities', $e, 'Got an exception while searching entities for callback.', array(), WATCHDOG_ERROR);
    $search_results = array();
  }
  $results = array();
  if ($search_results) {
    foreach ($search_results as $choice) {
      $results[$choice['PID']] = array(
        'PID' => $choice['PID'],
        'title' => $choice['solr_doc']['dc.title'],
        'authors' => isset($choice['solr_doc'][$author_field]) ? $choice['solr_doc'][$author_field] : array(),
      );
    }
  }
  return $results;
}

/**
 * Adds entity autocomplete paths to a scholar citation form.
 *
 * @param array $form
 *   Drupal Form API array.
 */
function islandora_entities_add_autocompletes_to_citation_form(&$form) {
  // Skip the non-numeric keys.
  // Hack in the autocomplete paths.
  foreach ($form['authors'] as $key => $value) {
    if (is_int($key)) {
      $form['authors'][$key]['family']['#autocomplete_path'] = 'islandora/entities/autocomplete/last_name';
      $form['authors'][$key]['qualified_name']['#autocomplete_path'] = 'islandora/entities/autocomplete/disambiguated';
      $form['authors'][$key]['affiliation']['#autocomplete_path'] = 'islandora/entities/autocomplete/department';
    }
  }
}

/**
 * Adds entity autocomplete paths to a scholar thesis form.
 *
 * @param array $form
 *   Drupal Form API array.
 */
function islandora_entities_add_autocompletes_to_thesis_form(&$form) {
  // Skip the non-numeric keys.
  // Hack in the autocomplete paths.
  foreach ($form['advisor'] as $key => $value) {
    if (is_int($key)) {
      $form['advisor'][$key]['advisor']['#autocomplete_path'] = 'islandora/entities/autocomplete/disambiguated';
    }
  }

  foreach ($form['committee'] as $key => $value) {
    if (is_int($key)) {
      $form['committee'][$key]['commitee']['#autocomplete_path'] = 'islandora/entities/autocomplete/disambiguated';
    }
  }

  foreach ($form['granter'] as $key => $value) {
    if (is_int($key)) {
      $form['granter'][$key]['department']['#autocomplete_path'] = 'islandora/entities/autocomplete/department';
    }
  }
}
