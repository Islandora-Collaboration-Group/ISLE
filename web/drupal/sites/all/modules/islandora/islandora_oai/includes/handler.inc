<?php

/**
 * @file
 * Stores all the default handler functionality for OAI responses.
 */

/**
 * Responds to the ListSets request.
 *
 * @param array $params
 *   An array containing:
 *   -token (object): A resumption token object specifying information about
 *   a previous request, or a new empty one.
 *   -max_records (int): The maximum number of records to return per request.
 *
 * @return array
 *   An array containing:
 *   -total_records (int): The total amount of records for the query.
 *   -records (array): An array containing:
 *     -pid (string): The pid of the result
 *     -label (string): The label of the result.
 *
 * @see hook_islandora_oai_identify_request_handler()
 */
function islandora_oai_retrieve_sets($params) {
  switch (variable_get('islandora_oai_query_backend', 'sparql')) {
    case 'sparql':
      $connection = islandora_get_tuque_connection();
      $records = array();
      $filters = array('sameTerm(?model, <info:fedora/islandora:collectionCModel>)');
      $sparql_query = islandora_oai_construct_sparql_query_for_sets($filters);
      $total_records = $connection->repository->ri->countQuery($sparql_query, 'sparql');
      $sparql_query .= <<<EOQ

LIMIT {$params['max_records']}
OFFSET {$params['token']->deliveredrecords}
EOQ;
      $query_results = $connection->repository->ri->sparqlQuery($sparql_query);
      foreach ($query_results as $result) {
        $object = islandora_object_load($result['object']['value']);
        $description = FALSE;
        if (isset($object['DC'])) {
          $description = islandora_oai_get_set_dc_portion($object['DC']);
        }
        $pid = $result['object']['value'];
        $records[$pid] = array(
          'pid' => $pid,
          'label' => $result['title']['value'],
        );
        if ($description) {
          $records[$pid]['description'] = $description;
        }
      }
      break;

    case 'solr':
      $qp = new IslandoraSolrQueryProcessor();
      $label_field = variable_get('islandora_solr_object_label_field', 'fgs_label_s');
      $solr_params = array(
        'limit' => $params['max_records'],
        'sort' => $label_field,
      );
      $qp->buildQuery(format_string('!field:"info:fedora/islandora:collectionCModel"', array(
        '!field' => variable_get('islandora_solr_content_model_field', 'RELS_EXT_hasModel_uri_ms'),
      )), $solr_params);
      $qp->solrStart = $params['token']->deliveredrecords;
      $qp->executeQuery(FALSE);
      $total_records = $qp->islandoraSolrResult['response']['numFound'];
      $records = array();
      $datastreams_field = variable_get('islandora_solr_datastream_id_field', 'fedora_datastreams_ms');
      foreach ($qp->islandoraSolrResult['response']['objects'] as $object) {
        $records[$object['PID']] = array(
          'pid' => $object['PID'],
          'label' => $object['solr_doc'][$label_field],
        );
        if (isset($object['solr_doc'][$datastreams_field]) && in_array('DC', $object['solr_doc'][$datastreams_field])) {
          $loaded_object = islandora_object_load($object['PID']);
          $records[$object['PID']]['description'] = islandora_oai_get_set_dc_portion($loaded_object['DC']);
        }
      }
      break;
  }
  return array(
    'total_records' => $total_records,
    'records' => $records,
  );
}

/**
 * Gets the DC portion of a set.
 *
 * @param AbstractDatastream $dc
 *   The DC XML datastream.
 *
 * @return string
 *   The XML to add to the set info.
 */
function islandora_oai_get_set_dc_portion(AbstractDatastream $dc) {
  $dom = new DOMDocument();
  $dom->loadXML($dc->content);
  return $dom->saveXML($dom->documentElement);
}

/**
 * Constructs the base object response XML depending on configuration options.
 *
 * @param array $params
 *   An array containing:
 *   -metadata_prefix (string): The metadata prefix of the request being
 *   executed.
 *   -pid (string): The pid of the record that we are building for.
 *
 * @return null|string
 *   XML string output of the record depending on configuration, NULL if an
 *   error was encountered.
 *
 * @see hook_islandora_oai_identify_request_handler()
 */
function islandora_oai_object_response_xml($params) {
  $object = islandora_object_load($params['pid']);
  $param = variable_get("islandora_oai_metadata_datastream_id_{$params['metadata_prefix']}", 'DC');
  if (isset($object[$param])) {
    $content = $object[$param]->content;
  }
  else {
    $content = NULL;
  }

  $oai_output = NULL;
  if (!empty($content)) {
    $self_transform_file = variable_get("islandora_oai_self_transform_file_{$params['metadata_prefix']}", 'default');
    if ($self_transform_file != 'default') {
      $self_xslt = new XSLTProcessor();
      $self_xsl = new DOMDocument();
      $self_xsl->load($self_transform_file);
      $self_input = new DOMDocument();
      $self_input->loadXML($content);
      $self_xslt->importStylesheet($self_xsl);
      $self_transform_params = module_invoke_all('islandora_oai_self_transform_params', $object, $params['metadata_prefix']);
      foreach ($self_transform_params as $namespace_uri => $transform_params) {
        $self_xslt->setParameter($namespace_uri, $transform_params);
      }

      $content = $self_xslt->transformToXML($self_input);
    }

    $xslt = new XSLTProcessor();
    $xsl = new DOMDocument();
    $file = variable_get("islandora_oai_transform_file_{$params['metadata_prefix']}", 'default');
    if ($file != 'default') {
      $xsl->load($file);
      $input = new DOMDocument();

      $did_load_ok = $input->loadXML($content);

      if ($did_load_ok) {
        $xslt->importStylesheet($xsl);
        $oai_output = $xslt->transformToXml($input);
      }
    }
    else {
      // Need to potentially remove the header information from the XML as
      // the response is embedded within XML.
      $document_dom = new DOMDocument();
      $document_dom->loadXML($content);
      $oai_output = $document_dom->saveXML($document_dom->documentElement);
    }
  }
  // Jam the dc.identifier.thumbnail into the OAI_DC output.
  if (isset($object['TN']) && variable_get('islandora_oai_append_dc_thumbnail', FALSE) && $params['metadata_prefix'] == 'oai_dc' && !empty($oai_output) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN'])) {
    $thumb_url = url("islandora/object/{$object->id}/datastream/TN/view/{$object->label}.jpg", array(
      'absolute' => TRUE,
      'language' => NULL,
    ));
    $dom = new DOMDocument();
    $dom->loadXML($oai_output);
    $dom->documentElement->appendChild($dom->createElementNS('http://purl.org/dc/elements/1.1/', 'dc:identifier.thumbnail', $thumb_url));
    $oai_output = $dom->saveXML($dom->documentElement);
  }

  if (variable_get("islandora_oai_include_object_links_for_{$params['metadata_prefix']}", FALSE) && !empty($oai_output)) {
    // See if we need to XPath to a specific spot to add it.
    $dom = new DOMDocument();
    $dom->loadXML($oai_output);
    $results = db_query('SELECT * FROM {islandora_oai_metadata_formats} WHERE metadata_prefix = :metadata_prefix', array(':metadata_prefix' => $params['metadata_prefix']));
    $record = $results->fetchObject();

    $object_url = url("islandora/object/{$object->id}", array(
      'absolute' => TRUE,
      'language' => NULL,
    ));
    $xpath = variable_get("islandora_oai_object_links_for_{$params['metadata_prefix']}_xpath", '');
    $field = variable_get("islandora_oai_object_links_for_{$params['metadata_prefix']}_field", '');
    $record_namespace = variable_get("islandora_oai_object_links_for_{$params['metadata_prefix']}_record_namespace", FALSE);

    if ($xpath) {
      $dom_xpath = new DOMXPath($dom);
      $dom_xpath->registerNamespace($record->record_prefix, $record->record_namespace);
      $dom_xpath->registerNamespace($record->metadata_prefix, $record->metadata_namespace);
      $elements = $dom_xpath->query($xpath);
      if ($elements->length == 0) {
        watchdog('islandora_oai', 'Unable to append object URL as could not find the element to XPath to for @pid. Error information: Metadata Prefix: @prefix, XPath: @xpath', array(
          '@pid' => $object->id,
          '@prefix' => $params['metadata_prefix'],
          '@xpath' => $xpath,
        ));
      }
      else {
        // If there's more than one element we'll just append to the first.
        foreach ($elements as $element) {
          if ($record_namespace) {
            $link_element = $dom->createElementNS($record->record_namespace, "{$record->record_prefix}:{$field}", $object_url);
          }
          else {
            $link_element = $dom->createElement($field, $object_url);
          }
          $element->appendChild($link_element);
          $oai_output = $dom->saveXML($dom->documentElement);
          break;
        }
      }
    }
    else {
      if ($record_namespace) {
        $link_element = $dom->createElementNS($record->record_namespace, "{$record->record_prefix}:{$field}", $object_url);
      }
      else {
        $link_element = $dom->createElement("$field", $object_url);
      }
      $dom->documentElement->appendChild($link_element);
      $oai_output = $dom->saveXML($dom->documentElement);
    }
  }

  // Transformation fails.
  if (empty($oai_output) && isset($object[$param])) {
    watchdog('islandora_oai', 'The pid @pidname failed to transform on the @datastream datastream with the file @file.',
      array(
        '@pidname' => $params['pid'],
        '@file' => $file,
        '@datastream' => $param,
      ), WATCHDOG_ERROR);
  }
  // Datastream doesn't exist.
  elseif (!isset($object[$param])) {
    watchdog('islandora_oai', "The pid @pidname failed to transform on the @datastream datastream as the datastream doesn't exist.",
      array(
        '@pidname' => $params['pid'],
        '@datastream' => $param,
      ), WATCHDOG_ERROR);
  }
  return $oai_output;
}

/**
 * Handles the GetRecord request.
 *
 * @param array $params
 *   An array containing:
 *   -pid (string): The pid of the record being requested.
 *
 * @return array|bool
 *   FALSE if the record does not exist, otherwise an array containing:
 *   -pid (string): The pid of the record.
 *   -date (string): The date timestamp of the record.
 *   -solr_doc (array): The Solr doc for the record.
 *
 * @see hook_islandora_oai_identify_request_handler()
 */
function islandora_oai_retrieve_record($params) {
  $pid = $params['pid'];
  $record = FALSE;
  $results = islandora_oai_query_solr(0, 'PID', $pid);
  if ($results['numFound'] > 0) {
    $solr_object = $results['objects'][0];
    $record = islandora_oai_construct_record_from_solr_response($solr_object);
  }
  return $record;
}

/**
 * Gets the setSpec membership information for a record.
 *
 * @param array $params
 *   An array containing:
 *   -record: An array containing:
 *     -pid: A string representation of the PID of the record.
 *     -date: A string representation of the date timestamp of the record.
 *     -solr_doc: An array containing the Solr doc for the record.
 *
 * @return array
 *   An array containing all set pids that this record belongs to.
 *
 * @see hook_islandora_oai_identify_request_handler()
 */
function islandora_oai_get_membership($params) {
  $sets = array();
  foreach (islandora_oai_get_membership_array() as $collection_field) {
    if (isset($params['record']['solr_doc'][$collection_field])) {
      // Need to cast to array such that we can check for multiple collections.
      foreach ((array) $params['record']['solr_doc'][$collection_field] as $set) {
        $sets[] = $set;
      }
    }
  }
  return $sets;
}

/**
 * Responds to the ListRecords or ListIdentifier request.
 *
 * @param array $params
 *   An array containing:
 *   -set: String of the PID of the set being searched for, FALSE if no set
 *   argument.
 *   -from: String of the from time starting point, FALSE if no from argument.
 *   -until: String of the until ending point, FALSE if no until argument.
 *   -token: Object representing a token.
 *
 * @return array
 *   An array describing the record retrieved from Solr.
 */
function islandora_oai_retrieve_records_or_identifiers($params) {
  $records = array();
  $set = $params['set'];
  $from = $params['from'];
  $until = $params['until'];
  $token = $params['token'];
  $field = 'PID';
  $query = NULL;
  $date_query = NULL;

  if ($from || $until) {
    $field = variable_get('islandora_oai_date_field', 'fgs_lastModifiedDate_dt');

    if ($from) {
      $solr_from = islandora_oai_datetime_to_solr_time($from);
    }
    else {
      $solr_from = '*';
    }

    if ($until) {
      $solr_until = islandora_oai_datetime_to_solr_time($until, TRUE);
    }
    else {
      $solr_until = '*';
    }
    $date_query = "$field:[$solr_from TO $solr_until]";
  }

  if (!isset($set)) {
    $set = NULL;
  }
  $results = islandora_oai_query_solr($token->deliveredrecords, $field, $query, $set, $date_query);
  foreach ($results['objects'] as $solr_object) {
    $records[] = islandora_oai_construct_record_from_solr_response($solr_object);
  }
  return array(
    'total_records' => $results['numFound'],
    'records' => $records,
  );
}

/**
 * Helper function to mold the output from the queryProcessor to form.
 *
 * @param array $solr_object
 *   An array describing a result obtained from a Solr query.
 *
 * @return array
 *   An array describing the record retrieved from Solr.
 */
function islandora_oai_construct_record_from_solr_response($solr_object) {
  return array(
    'pid' => $solr_object['PID'],
    'date' => $solr_object['solr_doc'][variable_get('islandora_oai_date_field', 'fgs_lastModifiedDate_dt')],
    'solr_doc' => $solr_object['solr_doc'],
  );
}

/**
 * Converts a time string into a form Solr can parse.
 *
 * @param string $datetime
 *   Time string to be parsed.
 * @param bool $until
 *   Whether an until time is specified.
 *
 * @return string
 *   A string representing the time in the given granularity.
 */
function islandora_oai_datetime_to_solr_time($datetime, $until = FALSE) {
  module_load_include('inc', 'islandora_oai', 'includes/request');
  $granularity = islandora_oai_get_date_granularity($datetime);
  $to_return = $datetime;

  switch ($granularity) {
    case 'YYYY-MM-DD':
      $to_return .= 'T00:00:00Z';
      if ($until) {
        $to_return .= '+1DAY-1MILLISECOND';
      }
      else {
        $to_return .= '/DAY';
      }
      break;

    case 'YYYY-MM-DDThh:mm:ssZ':
      if ($until) {
        $to_return .= '+1SECOND-1MILLISECOND';
      }
      else {
        $to_return .= '/SECOND';
      }
      break;

    default:
      // XXX: Maintain original behaviour of returning NULL... Should perhaps
      // throw an exception? Dunno.
      return;
  }

  return $to_return;
}

/**
 * Function that performs the queries to Solr.
 *
 * @param int $start
 *   The starting point for Solr queries, mainly used for resumption tokens.
 * @param string $field
 *   Field which we are using to query Solr.
 * @param string $query
 *   Query string to search for.
 * @param bool $set
 *   Whether we are recursively querying through a set.
 * @param bool $date_query
 *   Whether this is a date query or not.
 *
 * @return array
 *   The results generated from the crafted Solr query.
 */
function islandora_oai_query_solr($start = 0, $field = 'PID', $query = NULL, $set = NULL, $date_query = NULL) {
  $query_processor = new IslandoraSolrQueryProcessor();

  // Build the query string.
  if (empty($query)) {
    $query = '*:*';
  }
  else {
    module_load_include('inc', 'islandora_solr', 'includes/utilities');
    $query = $field . ':' . islandora_solr_lesser_escape($query);
  }
  $query_processor->buildQuery($query);

  // Build the fields to return.
  $fl_fields = islandora_oai_get_membership_array();
  $fl_fields[] = 'PID';
  $fl_fields[] = variable_get('islandora_oai_date_field', 'fgs_lastModifiedDate_dt');
  $query_processor->solrParams['fl'] = implode(',', $fl_fields);

  // Build the filter array.
  if ($date_query) {
    $query_processor->solrParams['fq'][] = $date_query;
  }
  $exclude_content_models = islandora_oai_get_restricted_models();
  $has_model = variable_get('islandora_oai_content_model_field', 'RELS_EXT_hasModel_uri_ms');
  foreach ($exclude_content_models as $content_model) {
    $content_model = trim($content_model);
    if ($content_model) {
      $query_processor->solrParams['fq'][] = '(-' . $has_model . ':("' . $content_model . '" OR "info:fedora/' . $content_model . '"))';
    }
  }

  // Recurse through sets if necessary.
  if ($set) {
    // OAI uses : to separate multiple setSpecs so we had to replace it
    // from the pids earlier.
    $sets = explode(':', $set);
    // Replace the first occurrence of _ with a : so we are back to a
    // valid pid.
    $repair_sets = function(&$set) {
      $set = preg_replace('/_/', ':', $set, 1);
      $set = trim($set);
    };
    array_walk($sets, $repair_sets);
    $descendants = array();

    switch (variable_get('islandora_oai_query_backend', 'sparql')) {
      case 'sparql':
        // Get all descendant children (collection and objs) and add to the sets
        // array if not already existing.
        // Cannot constrain on collections only as name conventions may change
        // across implementations.
        foreach ($sets as $value) {
          $query_results = islandora_oai_query_ri_for_child_sets($value);
          foreach ($query_results as $result) {
            $walk_set = $result['child']['uri'];
            $walk_set = str_replace('info:fedora/', '', $walk_set);
            if (!in_array($walk_set, $descendants)) {
              $descendants[] = $walk_set;
            }
          }
          $descendants[] = $value;
        }
        $query_processor->solrParams['fq'][] = islandora_oai_build_membership_fq_statement($descendants);
        break;

      case 'solr':
        // Easier if we have ancestors.
        $ancestors_field = variable_get('islandora_oai_solr_object_ancestors_field', '');
        if (!empty($ancestors_field)) {
          foreach ($sets as $value) {
            $query_processor->solrParams['fq'][] = format_string('!ancestors_field:("!pid" OR "info:fedora/!pid")', array(
              '!ancestors_field' => $ancestors_field,
              '!pid' => $value,
            ));
          }
        }
        // Otherwise we have to recurse manually.
        else {
          foreach ($sets as $value) {
            $query_results = islandora_oai_query_solr_for_child_sets($value);
            foreach ($query_results as $result) {
              if (!in_array($result, $descendants)) {
                $descendants[] = $result;
              }
            }
            $descendants[] = $value;
          }
          $query_processor->solrParams['fq'][] = islandora_oai_build_membership_fq_statement($descendants);
        }
        break;
    }
  }
  $query_processor->solrStart = $start;
  $query_processor->solrLimit = variable_get('islandora_oai_max_size', '20');
  try {
    // XXX: Use POST as the filters for membership to a collection can be very
    // large.
    $query_processor->executeQuery(FALSE, TRUE);
    $solr_results = $query_processor->islandoraSolrResult['response'];
  }
  catch (Exception $e) {
    drupal_set_message(t('error searching @message', array('@message' => $e->getMessage())), 'error');
  }
  return $solr_results;
}

/**
 * Helper function to build an 'fq' statement for child PIDs.
 *
 * @param array $children
 *   The array of child PIDs.
 *
 * @return string
 *   The 'fq' statement.
 */
function islandora_oai_build_membership_fq_statement(array $children) {
  $set_fq = array();
  // We are using OR here to account for fields in Solr that may index
  // just the PID or the entire URI. In the future if performance becomes
  // an issue with huge Solr queries we should re-visit this.
  foreach ($children as $walk) {
    foreach (islandora_oai_get_membership_array() as $collection_field) {
      $set_fq[] = format_string('!field:("!walk" OR "info:fedora/!walk")', array(
        '!field' => $collection_field,
        '!walk' => $walk,
      ));
    }
  }
  return implode(' OR ', $set_fq);
}

/**
 * Helper function to query recursively for child sets from Solr.
 *
 * @param string $parent
 *   The set to get children in.
 * @param array $sets
 *   Used internally to maintain the list of sets found as it recurses.
 *
 * @return array
 *   A list of set PIDs.
 */
function islandora_oai_query_solr_for_child_sets($parent, &$sets = array()) {
  global $user;
  $user_name = $user->uid === 0 ? 'anonymous' : $user->name;

  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery('*:*');
  $qp->solrParams['fl'] = 'PID';
  // Make sure we get everything but still filter on XACML permissions.
  $qp->solrParams['fq'] = array();
  module_invoke('islandora_xacml_api', 'islandora_solr_query', $qp);
  // Set is part of the current collection.
  $qp->solrParams['fq'][] = islandora_oai_build_membership_fq_statement(array($parent));
  // Set is also a collection itself.
  $qp->solrParams['fq'][] = format_string('!field:"info:fedora/islandora:collectionCModel"', array(
    '!field' => variable_get('islandora_solr_content_model_field', 'RELS_EXT_hasModel_uri_ms'),
  ));

  // XXX: Use POST as the filters for membership to a collection can be very
  // large.
  $qp->executeQuery(FALSE, TRUE);
  $get_pid = function($object) {
    return $object['PID'];
  };
  $pids = array_map($get_pid, $qp->islandoraSolrResult['response']['objects']);
  $diff = array_diff($pids, $sets);
  $sets = array_merge($sets, $pids);
  // Recurse through the diff.
  foreach ($diff as $child_collection) {
    islandora_oai_query_solr_for_child_sets($child_collection, $sets);
  }
  return $sets;
}

/**
 * Helper function to query for child sets from the RI.
 *
 * @param string $set
 *   The PID of the set to query in.
 *
 * @return array
 *   The results of the RI query from Tuque.
 */
function islandora_oai_query_ri_for_child_sets($set) {
  global $user;
  $user_name = $user->uid === 0 ? 'anonymous' : $user->name;

  $connection = islandora_get_tuque_connection();
  // Query sets from RI (SPARQL).
  if (!variable_get('islandora_risearch_use_itql_when_necessary', TRUE)) {
    $xacml_filter = module_invoke('islandora_xacml_api', 'islandora_basic_collection_get_query_filters');
    $query = <<<EOQ
SELECT ?child
FROM <#ri>
WHERE {
  ?child <fedora-model:state> <fedora-model:Active> .
  {
    ?child <fedora-model:hasModel> <info:fedora/islandora:collectionCModel>
  } UNION {
    ?child <fedora-model:hasModel> [<fedora-model:hasModel> <info:fedora/islandora:collectionCModel>]
  } .
  {
    ?child <fedora-rels-ext:isMemberOfCollection>+ <info:fedora/$set>
  } UNION {
    ?child <fedora-rels-ext:isMemberOf>+ <info:fedora/$set>
  }
  OPTIONAL {
    ?child <http://islandora.ca/ontology/relsext#isViewableByRole> ?role .
  }
  OPTIONAL {
    ?child <http://islandora.ca/ontology/relsext#isViewableByUser> ?user .
  }
  FILTER($xacml_filter)
}
EOQ;
    $query_results = $connection->repository->ri->sparqlQuery($query);
  }
  // Query sets from RI (iTQL).
  else {
    $itql_query = '
    select $child from <#ri>
    where
    (
      (
        (
        $child <fedora-model:hasModel> $model
        and $model <mulgara:is> <info:fedora/islandora:collectionCModel>
        and $model <fedora-model:hasModel> <info:fedora/fedora-system:ContentModel-3.0>
        )
      or
        (
        $child <fedora-model:hasModel> $model
        and $model <fedora-model:hasModel> <info:fedora/islandora:collectionCModel>
        and $child <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
        )
      minus $child <http://islandora.ca/ontology/relsext#isViewableByRole> $role
      minus $child <http://islandora.ca/ontology/relsext#isViewableByUser> $user
      )
    or
      (
        (
        $child <fedora-model:hasModel> $model
        and $model <mulgara:is> <info:fedora/islandora:collectionCModel>
        and $model <fedora-model:hasModel> <info:fedora/fedora-system:ContentModel-3.0>
        )
      or
        (
        $child <fedora-model:hasModel> $model
        and $model <fedora-model:hasModel> <info:fedora/islandora:collectionCModel>
        and $child <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
        )
    and
        (';
    foreach ($user->roles as $role) {
      $itql_query .= '$child <http://islandora.ca/ontology/relsext#isViewableByRole> ' . "'$role' or ";
    }
    $itql_query .= '$child <http://islandora.ca/ontology/relsext#isViewableByUser> ' . "'$user_name'" . ')';
    $itql_query .= ')
    )
  and
    (
    walk
      (
        $parent <fedora-rels-ext:isMemberOfCollection><info:fedora/' . $set . '>
         and $child <fedora-rels-ext:isMemberOfCollection> $parent
      )
    or
    walk
      (
        $parent <fedora-rels-ext:isMemberOf><info:fedora/' . $set . '>
        and $child <fedora-rels-ext:isMemberOf> $parent
      )
  )';
    $query_results = $connection->repository->ri->itqlQuery($itql_query);
  }
  return $query_results;
}

/**
 * Returns the collection fields defined in the admin page in array format.
 *
 * @return array
 *   An array containing the fields.
 */
function islandora_oai_get_membership_array() {
  // Store in a static variable, to avoid re-parsing within the same request.
  $mini_cache = &drupal_static(__FUNCTION__, array());

  if (empty($mini_cache)) {
    foreach (preg_split('/(\\r?\\n|\\r)+/', variable_get('islandora_oai_collection_field', "RELS_EXT_isMemberOfCollection_uri_ms\nRELS_EXT_isMemberOf_uri_ms")) as $spec) {
      $mini_cache[] = trim($spec);
    }
    $mini_cache = array_filter($mini_cache);
  }
  return $mini_cache;
}

/**
 * Get the restricted/excluded models.
 *
 * @return array
 *   An array of strings representing the models of which objects should not be
 *   returned from OAI requests.
 */
function islandora_oai_get_restricted_models() {
  $exclude_content_models = variable_get('islandora_oai_exclude_content_models', '');
  $cmodels = explode("\n", $exclude_content_models);
  $cmodels = array_map('trim', $cmodels);
  return array_filter($cmodels);
}

/**
 * Helper function to query for sets.
 *
 * @param array $filters
 *   An array of content models to be used as filters.
 * @param array $required
 *   An array of arrays containing:
 *   -vars: An array containing strings of the variables that are referenced
 *   in the required statements.
 *   -statements: An array containing string statements pertaining to the
 *   SPARQL query.
 *
 * @return string
 *   The SPARQL query to be executed.
 */
function islandora_oai_construct_sparql_query_for_sets($filters, $required = array()) {
  module_load_include('inc', 'islandora', 'includes/tuque');
  $sparql_query = <<<EOQ
  SELECT DISTINCT ?object ?title ?created !vars
  FROM <#ri>
  WHERE {
    ?object <fedora-model:hasModel> ?model ;
            <fedora-model:label> ?title ;
            <fedora-model:createdDate> ?created ;
            <fedora-model:state> <fedora-model:Active> .
    !required
    !optionals
    !filters
  }
  ORDER BY ?title
EOQ;
  $optionals = (array) module_invoke('islandora_xacml_api', 'islandora_basic_collection_get_query_optionals', 'view');
  $filter_modules = array(
    'islandora_xacml_api',
    'islandora',
  );
  $excluded_models = islandora_oai_get_restricted_models();
  if (!empty($excluded_models)) {
    $map_excludes = function ($exclude) {
      return "sameTerm(?model, <info:fedora/$exclude>)";
    };
    $filters[] = format_string('!(!exclude_conjunction)', array(
      '!exclude_conjunction' => implode(' || ', array_map($map_excludes, $excluded_models)),
    ));
  }

  foreach ($filter_modules as $module) {
    $filters = array_merge_recursive($filters, (array) module_invoke($module, 'islandora_basic_collection_get_query_filters', 'view'));
  }

  // If the user is choosing to restrict things within the Islandora namespace
  // the filter needs to be modified as the islandora namespace is always
  // allowed by default.
  if (variable_get('islandora_namespace_restriction_enforced', FALSE) && variable_get('islandora_oai_exclude_islandora_namespace', FALSE)) {
    module_load_include('inc', 'islandora', 'includes/utilities');
    // This is somewhat of duplicated code but needs to be done so we remove
    // only the "islandora" namespace.
    $namespace_array = islandora_get_allowed_namespaces();
    $key = array_keys($namespace_array, 'islandora');
    $namespace_array = array_diff_key($namespace_array, array_flip($key));
    $namespace_sparql = implode('|', $namespace_array);
    $filters['islandora_namespace_restrictions'] = format_string('regex(str(?object), "info:fedora/(!namespaces):")', array('!namespaces' => $namespace_sparql));
  }
  $filter_map = function ($filter) {
    return "FILTER($filter)";
  };
  $sparql_query = format_string($sparql_query, array(
    '!vars' => !empty($required['vars']) ? implode(' ', $required['vars']) : '',
    '!required' => !empty($required['statements']) ? implode('. ', $required['statements']) : '',
    '!optionals' => !empty($optionals) ? ('OPTIONAL {{' . implode('} UNION {', $optionals) . '}}') : '',
    '!filters' => !empty($filters) ? implode(' ', array_map($filter_map, $filters)) : '',
  ));
  return $sparql_query;
}
