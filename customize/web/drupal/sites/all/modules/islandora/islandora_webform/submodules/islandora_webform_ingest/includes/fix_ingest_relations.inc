<?php

/**
 * Retroactive IWI fix page.
 */
function islandora_ingest_webform_admin_settings() {
  $form = array(
    'retroactive_fix' => array(
      '#type' => 'fieldset',
      '#title' => 'Repair relations',
      '#weight' => 5,
      '#collapsible' => FALSE,
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Go'),
        '#submit' => array('iwi_retroactive_fix_collection_submit'),
        '#prefix' => '<p>' . t('Repair collection object predicates and add/replace islandora webform relation tags. You should only need to run this one time to fix missing collection memberships and erroneous relationship predicates that may have been caused by changing the relationship predicate name for a webform when using the RC1 version of Islandora Webform.') . '</p>',
      ),
    ),
  );

  return $form;
}

/**
 * Run retroactive relationship fixes.
 */
function iwi_retroactive_fix_collection_submit() {
  iwi_retroactive_fix_all_collection_predicates();
  iwi_retroactive_fix_all_iw_relation_tags();
}

/**
 * Fix collection relationships.
 *
 * Search for all objects that have rels-ext relations to collections, not tagged by RC1, and
 * re-set their relationship predicate to 'isMemberOfCollection'.
 */
function iwi_retroactive_fix_all_collection_predicates() {

  module_load_include('inc', 'islandora', 'includes/utilities');

  // Get list of available relationship predicates.
  $predicates = islandora_webform_ingest_rels_ext_predicates_options();
  unset($predicates['isMemberOfCollection']);

  $operations = array();
  array_push($operations, array(
          "iwi_fix_retroactive_collection_predicate",
          array($predicates),
        )
      );
  // Construct the batch array for processing.
  $batch = array(
    'operations' => $operations,
    'title' => t('Fix collection predicates'),
    'finished' => 'iwi_retroactive_fix_all_collection_predicates_finished',
    'file' => drupal_get_path('module', 'islandora_webform_ingest') . '/includes/fix_ingest_relations.inc',
  );

  batch_set($batch);
}

/**
 * Repair collection predicate for a single collection member.
 *
 * @param int $pid
 *   The pid of the object that is a member of a collection.
 * @param string $predicate
 *   The predicate that we want to change to isMemberOfCollection
 */
function iwi_fix_retroactive_collection_predicate($predicates, &$context) {
  $query = "";
  if (!isset($context['sandbox']['progress'])) {
    $query = iwi_get_all_objects_in_collections_query($predicates);
    $connection = islandora_get_tuque_connection(); 
    $child_objects_count = $connection->repository->ri->countQuery($query, 'sparql');
    $context['sandbox']['max'] = $child_objects_count;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['results_toprocess'] = array();
  }  
  if ($context['sandbox']['max'] == 0) {
    // Nothing to process.
    $context['finished'] = 1;
    return t('No relation to existing Collections to fix.');
  }
  if (empty($context['sandbox']['results_toprocess'])) {
    if (!isset($connection)) {
      $connection = islandora_get_tuque_connection(); 
      $query = iwi_get_all_objects_in_collections_query($predicates);
    }
    $offset = 100;
    $limited_query = $query . <<<EOQ
LIMIT $offset
EOQ;

   $context['sandbox']['results_toprocess'] = $connection->repository->ri->sparqlQuery($limited_query);
   if (empty($context['sandbox']['results_toprocess'])) {
    //Nothing more to fetch. 
    $context['finished'] = 1; 
    return;
   }
  }
  // Get one child object, removing the old relationship and
  // adding the isMemberOfCollection relationship.
  if (!empty($context['sandbox']['results_toprocess'])) {
    $target_object_array = array_pop($context['sandbox']['results_toprocess']);
    $context['message'] = t('Now processing object %pid', array('%pid' => $target_object_array['pid']['value']));
    $target_object = islandora_object_load($target_object_array['pid']['value']);
  
    if ($target_object !== FALSE) {

      $processedobject = FALSE;
      if (islandora_object_access(ISLANDORA_MANAGE_PROPERTIES, $target_object)) {
        $rels = $target_object->relationships;
        $current_rel = explode('#', $target_object_array['predicate']['uri']); //Had to split. No way making -> relationships understand the full uri.
        $rels->autoCommit = FALSE;
        if ($rels->remove(FEDORA_RELS_EXT_URI, $current_rel[1], $target_object_array['collection']['value'], RELS_TYPE_URI)) {
          $processedobject = TRUE;
        }
        // Check to see if there already is an isMemberOfCollection relation;
        // Add if not present.
        $memberofcollection_relationships = $rels->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
        if (empty($memberofcollection_relationships)) {
          $rels->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $target_object_array['collection']['value'], RELS_TYPE_URI);
          $processedobject = TRUE;
        }
        if ($processedobject) {
          $rels->commitRelationships();
          $context['results'][] = $target_object->id;
        }
      }
    }
    //Increment progress even if we could not process an object
    //Also set maximum progress to originally object count, in case someone is messing with our relations somewhere in paralell so we don't iterate forever.
    $context['sandbox']['progress'] = min( $context['sandbox']['max'], $context['sandbox']['progress'] + 1); 
  }
  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
}

/**
 * Finished batch callback for collection predicates repair.
 */
function iwi_retroactive_fix_all_collection_predicates_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), 'One collection predicate repaired.', '@count collection predicates repaired.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}

/**
 * Retroactively add/replace IW ingested object tracking tags.
 */
function iwi_retroactive_fix_all_iw_relation_tags() {
  // Get PIDs and webform nids from recorded ingestions.
  $query = db_select('islandora_webform_ingestions', 'i')
    ->distinct()
    ->fields('i', array('nid', 'pid'));
  $ingestions = $query->execute()->fetchAll();

  $operations = array();

  foreach ($ingestions as $ingestion) {
    array_push($operations, array(
        "iwi_retroactive_fix_single_iw_relation_tag",
        array($ingestion->pid, $ingestion->nid),
      )
    );
  }
  // Construct the batch array for processing.
  $batch = array(
    'operations' => $operations,
    'title' => t('Add/replace IW relation tags'),
    'finished' => 'iwi_retroactive_fix_all_iw_relation_tags_finished',
    'file' => drupal_get_path('module', 'islandora_webform_ingest') . '/includes/fix_ingest_relations.inc',
  );

  batch_set($batch);

}

/**
 * Add or replace a single child object's IW relation tags.
 *
 * @param string $pid
 *   PID for child object
 *
 * @param int $nid
 *   node id of the webform
 */
function iwi_retroactive_fix_single_iw_relation_tag($pid, $nid, &$context) {
  $object = islandora_object_load($pid);

  if (($object) && (islandora_object_access(ISLANDORA_MANAGE_PROPERTIES, $object))) {
    $rels = $object->relationships;
    $rels->autoCommit = FALSE;
    $object->relationships->remove(ISLANDORA_RELS_EXT_URI, ISLANDORA_WEBFORM_ISIW_PREDICATE);
    $object->relationships->remove(ISLANDORA_RELS_EXT_URI, ISLANDORA_WEBFORM_HASIW_PREDICATE);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, ISLANDORA_WEBFORM_ISIW_PREDICATE, "true", RELS_TYPE_PLAIN_LITERAL);
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, ISLANDORA_WEBFORM_HASIW_PREDICATE, "{$nid}", RELS_TYPE_PLAIN_LITERAL);
    $rels->commitRelationships();
    $context['results'][] = $object->id;
  }
}

/**
 * Finished batch callback for collection predicates repair.
 */
function iwi_retroactive_fix_all_iw_relation_tags_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), 'Relation tags repaired on one child object.', 'Relation tags repaired on @count child objects.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}


/**
 * Get all islandora objects related to a collection.
 *
 * @return string
 *   a Sparql Query
 */
function iwi_get_all_objects_in_collections_query($predicates) {

  module_load_include('inc', 'islandora', 'includes/utilities');
  //Avoid objects already tagged as created by RC1
  $content_model = 'islandora:collectionCModel';
  $query = <<<EOQ
    PREFIX fedora-rels-ext: <info:fedora/fedora-system:def/relations-external#>
    PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
    SELECT ?pid ?predicate ?collection ?nid
    FROM <#ri>
    WHERE {
      ?pid ?predicate ?collection.
      ?collection <fedora-model:hasModel> <info:fedora/$content_model>.
      ?pid <fedora-model:state> <fedora-model:Active> .
      optional {?pid islandora-rels-ext:hasIW ?nid .}
     FILTER(!sameTerm(?predicate, <fedora-rels-ext:isMemberOfCollection>))
     !predicatefilter
     FILTER (!bound(?nid))
    }
EOQ;
  $filter_map = function ($filter) {
    return "sameTerm(?predicate, <fedora-rels-ext:$filter>)";
  };
  $collection_predicate_filters = "";


  if (count($predicates)>0) {
  $collection_predicate_filters = implode(' || ', array_map($filter_map, array_keys($predicates)));
  }

  $query = format_string($query, array(
    '!predicatefilter' => !empty($collection_predicate_filters) ? "FILTER({$collection_predicate_filters})" : '',
  ));
  return $query;
}
