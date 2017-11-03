<?php

/**
 * @file
 * Handles OAI requests and responds as appropriate.
 */

/**
 * Parses the URL and calls the appropriate function to handle the request.
 */
function islandora_oai_parse_request() {
  global $base_url;

  // Instantiate a new XML Writer object we will pass around.
  $writer = new XMLWriter();
  $writer->openURI('php://output');
  $writer->startDocument('1.0', 'utf-8');
  $writer->startElement('OAI-PMH');
  $writer->writeAttribute('xmlns', 'http://www.openarchives.org/OAI/2.0/');
  $writer->writeAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
  $writer->writeAttribute('xsi:schemaLocation', 'http://www.openarchives.org/OAI/2.0/ http://www.openarchives.org/OAI/2.0/OAI-PMH.xsd');
  $response_date = gmstrftime('%Y-%m-%dT%H:%M:%SZ');
  $writer->writeElement('responseDate', $response_date);

  // Arguments are all GET arguments except for Drupal's q.
  if (isset($_GET) && count($_GET) != 1) {
    $args = $_GET;
    $verb = $_GET['verb'];
    unset($args['q']);
    unset($args['verb']);
  }
  // Allow post requests.
  elseif (isset($_POST) && count($_POST) != 0) {
    $args = $_POST;
    $verb = $args['verb'];
    unset($args['verb']);
  }
  // Error if no GET arguments.
  else {
    islandora_oai_add_error('badRequestMethod', $_SERVER['REQUEST_METHOD']);
  }

  // Argument repeated more than once in the GET request.
  $arg_list = explode('&', ($_SERVER['QUERY_STRING']));
  $repeat_arg = FALSE;
  $multiple_list = array();
  foreach ($arg_list as $arg) {
    $arg_parts = explode('=', $arg);
    if (!isset($multiple_list[$arg_parts[0]])) {
      // Make a new entry and set it to 0.
      $multiple_list[$arg_parts[0]] = 0;
    }
    $multiple_list[$arg_parts[0]]++;
  }
  foreach ($multiple_list as $argument => $argument_count) {
    if ($argument_count > 1) {
      if ($argument == 'verb') {
        islandora_oai_add_error('multipleVerb');
      }
      else {
        islandora_oai_add_error('sameArgument');
      }
      $repeat_arg = TRUE;
    }
  }

  // Set request URL.
  if (isset($_GET['q'])) {
    $request_url = $base_url . '/' . $_GET['q'];
  }
  else {
    $request_url = $base_url;
  }

  header('Content-Type: text/xml');
  // Add the request.
  $writer->startElement('request');

  // Check to see if we have a bad verb.
  $error_list = islandora_oai_get_errors();

  if (!count($error_list)) {
    $writer->writeAttribute('verb', $verb);
  }
  $writer->text($request_url);
  $writer->endElement();

  // Switch on verb and dispatch requests.
  if (!isset($verb)) {
    islandora_oai_add_error('noVerb');
  }
  elseif (!$repeat_arg) {
    switch ($verb) {

      case 'GetRecord':
        islandora_oai_get_record($writer, $args);
        break;

      case 'Identify':
        islandora_oai_identify($writer, $args);
        break;

      case 'ListIdentifiers':
        islandora_oai_list_id_rec($writer, $args);
        break;

      case 'ListMetadataFormats':
        islandora_oai_list_metadata_formats($writer, $args);
        break;

      case 'ListRecords':
        islandora_oai_list_id_rec($writer, $args, TRUE);
        break;

      case 'ListSets':
        islandora_oai_list_sets($writer, $args);
        break;

      default:
        islandora_oai_add_error('badVerb', $verb);
    }
  }

  // Grab errors again for output.
  $error_list = islandora_oai_get_errors();

  // Get all errors and add them to the response.
  if ($error_list) {
    foreach ($error_list as $code => $verb) {
      $writer->startElement('error');
      $writer->writeAttribute('code', $code);
      $writer->text($verb);
      $writer->endElement();
    }
  }
  $writer->endDocument();
  $writer->flush();
}

/**
 * Returns errors generated during an OAI request.
 *
 * @return bool|array
 *   FALSE when no errors, an array containing all errors encountered otherwise.
 */
function islandora_oai_get_errors() {
  // Get the errors array and convert it to a string.
  $errors = islandora_oai_add_error('get');

  // Return false if no errors.
  if (!count($errors)) {
    return FALSE;
  }
  return $errors;
}

/**
 * Routs a request to add an error to the error function in a sensical way.
 *
 * @param string $code
 *   Error code to be added, or 'get' if we want to get the errors array.
 * @param string $argument
 *   The argument causing the error.
 * @param string $value
 *   The argument value causing the error.
 *
 * @return NULL|array
 *   Nothing is returned if not 'get', an array of errors otherwise.
 */
function islandora_oai_add_error($code, $argument = '', $value = '') {
  return islandora_oai_errors($code, $argument, $value);
}

/**
 * Define errors based on OAI2 specification.
 *
 * Helper function for islandora_oai_add_error and islandora_oai_get_errors.
 * Basically faking a singleton with getter and setter methods.
 * OAI2 specification: http://www.openarchives.org/OAI/openarchivesprotocol.html
 *
 * @param string $code
 *   Error code to be added, or 'get' if we want to get the errors array.
 * @param string $argument
 *   The argument causing the error.
 * @param string $value
 *   The argument value causing the error.
 *
 * @return NULL|array
 *   Nothing is returned if not 'get', an array of errors otherwise.
 */
function islandora_oai_errors($code, $argument = '', $value = '') {
  // Store errors in a static array.
  static $errors;

  // Initialize $errors.
  if (!is_array($errors)) {
    $errors = array();
  }

  switch ($code) {

    // Return errors if this is a get errors request.
    case 'get':
      return $errors;

    // Clear out errors and return nothing.
    case 'reset':
      $errors = array();
      return;

    case 'badArgument':
      $text = t("The argument '@argument' (value=@value) included in the request is not valid.", array('@argument' => $argument, '@value' => $value));
      break;

    case 'badGranularity':
      $text = t("The value '@value' of the argument '@argument' is not valid.", array('@value' => $value, '@argument' => $argument));
      $code = 'badArgument';
      break;

    case 'badResumptionToken':
      $text = t("The resumptionToken '@value' does not exist or has already expired.", array('@value' => $value));
      break;

    case 'badRequestMethod':
      $text = t("The request method '@argument' is unknown.", array('@argument' => $argument));
      $code = 'badVerb';
      break;

    case 'badVerb':
      $text = t("The verb '@argument' provided in the request is illegal.", array('@argument' => $argument));
      break;

    case 'cannotDisseminateFormat':
      $text = t("The metadata format '@value' given by @argument is not supported by this repository.", array('@value' => $value, '@argument' => $argument));
      break;

    case 'exclusiveArgument':
      $text = t('The usage of resumptionToken as an argument allows no other arguments.');
      $code = 'badArgument';
      break;

    case 'idDoesNotExist':
      $text = t("The value '@value' of the identifier is illegal for this repository.", array('@value' => $value));
      break;

    case 'missingArgument':
      $text = t("The required argument '@argument' is missing in the request.", array('@argument' => $argument));
      $code = 'badArgument';
      break;

    case 'noRecordsMatch':
      $text = t('The combination of the given values results in an empty list.');
      break;

    case 'noMetadataFormats':
      $text = t('There are no metadata formats available for the specified item.');
      break;

    case 'noVerb':
      $text = t('The request does not provide any verb.');
      $code = 'badVerb';
      break;

    case 'noSetHierarchy':
      $text = t('This repository does not support sets.');
      break;

    case 'sameArgument':
      $text = t('Do not use the same argument more than once.');
      $code = 'badArgument';
      break;

    case 'sameVerb':
      $text = t('Do not use the same verb more than once.');
      $code = 'badVerb';
      break;

    case 'multipleVerb':
      $text = t('Cannot have more than one verb in the same request.');
      $code = 'badVerb';
      break;

    default:
      $text = t("Unknown error:
        code: '@code',
        argument: '@argument',
        value: '@value'",
        array('@code' => $code, '@argument' => $argument, '@value' => $value));
      $code = 'badArgument';
  }
  $errors[$code] = $text;
}

/**
 * Retrieve an individual metadata record from a repository.
 *
 * @param string $writer
 *   The XMLWriter object being passed by reference for constructing our output.
 * @param array $args
 *   Argument of the $_POST or $_GET request.
 *
 * @return bool
 *   FALSE if we encountered an error.
 */
function islandora_oai_get_record(&$writer, $args) {
  foreach ($args as $key => $val) {
    switch ($key) {
      case 'identifier':
        // Replace the first _ with a : so we have a pid.
        $val = preg_replace('/_/', ':', $val, 1);
        // Strip off the the prefix such that we are left with a Fedora pid.
        $id_prefix = trim(variable_get('islandora_oai_repository_identifier', 'drupal-site.org'));
        $identifier = str_replace('oai:' . $id_prefix . ':', '', $val);
        break;

      case 'metadataPrefix':
        $result = db_query("SELECT * FROM {islandora_oai_metadata_formats} WHERE metadata_prefix=:metadata_prefix", array(':metadata_prefix' => $val));
        if ($result->rowCount() > 0) {
          $metadata_prefix = $val;
        }
        else {
          islandora_oai_add_error('cannotDisseminateFormat', $key, $val);
          return FALSE;
        }
        break;

      default:
        islandora_oai_add_error('badArgument', $key, $val);
        return FALSE;
    }
  }
  if (!isset($identifier)) {
    islandora_oai_add_error('missingArgument', 'identifier');
    return FALSE;
  }
  if (!isset($metadata_prefix)) {
    islandora_oai_add_error('missingArgument', 'metadataPrefix');
    return FALSE;
  }
  $record_existence = islandora_oai_filter_function('GetRecord');
  if (isset($record_existence['file'])) {
    require_once $record_existence['file'];
  }
  $record = call_user_func($record_existence['function'], array(
    'pid' => $identifier,
  ));
  if (!$record) {
    islandora_oai_add_error('idDoesNotExist', '', $identifier);
    return FALSE;
  }
  $writer->startElement('GetRecord');
  islandora_oai_build_record_response($writer, $record, TRUE, $metadata_prefix);
  $writer->endElement();
}

/**
 * Get the date granularity.
 *
 * @param string $date
 *   The date string to match against.
 *
 * @return string|bool
 *   The granularity of the date, or FALSE.
 */
function islandora_oai_get_date_granularity($date) {
  if (preg_match("/^\d\d\d\d\-\d\d\-\d\d$/", $date)) {
    return 'YYYY-MM-DD';
  }
  elseif (preg_match("/^(\d\d\d\d\-\d\d\-\d\d)T(\d\d:\d\d:\d\d)Z$/", $date)) {
    return 'YYYY-MM-DDThh:mm:ssZ';
  }
  else {
    return FALSE;
  }
}

/**
 * Validate the date format (from or until in the request).
 *
 * @param string $date
 *   The date passed through the request.
 *
 * @return bool
 *   TRUE if the date format is valid, FALSE otherwise.
 */
function islandora_oai_check_date_format($date) {
  static $granularity;
  $new_granularity = islandora_oai_get_date_granularity($date);

  if (!$new_granularity) {
    return FALSE;
  }

  if ($granularity) {
    if ($granularity != $new_granularity) {
      return FALSE;
    }
  }
  else {
    $granularity = $new_granularity;
  }

  // Validate date based on granularity.
  if ($granularity == 'YYYY-MM-DDThh:mm:ssZ') {
    $checkstr = '([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})T([0-9]{2}):([0-9]{2}):([0-9]{2})Z';
  }
  else {
    $checkstr = '([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})';
  }

  if (preg_match("/$checkstr/", $date, $regs)) {
    if (checkdate($regs[2], $regs[3], $regs[1])) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

/**
 * Generate a random value for resumptionToken.
 *
 * @return int
 *   A random number to be used for a resumption token ID.
 */
function islandora_oai_get_random_token() {
  return mt_rand(12345, mt_getrandmax());
}

/**
 * Provides the repository identity information.
 *
 * @param object $writer
 *   The XMLWriter object being passed by reference for constructing our output.
 * @param array $args
 *   Argument of the $_POST or $_GET request.
 *
 * @return bool
 *   FALSE if an error is encoutered.
 */
function islandora_oai_identify(&$writer, $args) {
  $error_text = islandora_oai_get_errors();

  // Catch for a bad argument.
  if (count($error_text) > 0 && (count($args) > 0)) {
    foreach ($args as $key => $val) {
      islandora_oai_add_error('badArgument', $key, $val);
      return FALSE;
    }
  }

  $earliest_datestamp = islandora_oai_get_earliest_datetime();
  $writer->startElement('Identify');
  $writer->writeElement('repositoryName', variable_get('islandora_oai_repository_name', 'Repository'));
  $identify_base_url = url(variable_get('islandora_oai_path', 'oai2'), array('absolute' => TRUE));
  $writer->writeElement('baseURL', $identify_base_url);
  $writer->writeElement('protocolVersion', '2.0');
  $writer->writeElement('adminEmail', variable_get('islandora_oai_admin_email', 'N/A'));
  $writer->writeElement('earliestDatestamp', $earliest_datestamp);
  $writer->writeElement('deletedRecord', 'no');
  $writer->writeElement('granularity', 'YYYY-MM-DDThh:mm:ssZ');
  $writer->writeElement('compression', 'yes');

  // Description.
  $writer->startElement('description');

  // OAI Identifer.
  $writer->startElement('oai-identifier');
  $writer->writeAttribute('xmlns', 'http://www.openarchives.org/OAI/2.0/oai-identifier');
  $writer->writeAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
  $writer->writeAttribute('xsi:schemaLocation', 'http://www.openarchives.org/OAI/2.0/oai-identifier
    http://www.openarchives.org/OAI/2.0/oai-identifier.xsd');
  $writer->writeElement('scheme', 'oai');
  $writer->writeElement('repositoryIdentifier', trim(variable_get('islandora_oai_repository_identifier', 'drupal-site.org')));
  $writer->writeElement('delimiter', ':');
  $writer->writeElement('sampleIdentifier', 'oai:' . trim(variable_get('islandora_oai_repository_identifier', 'drupal-site.org') . ':123'));
  $writer->endElement();
  $writer->endElement();
}

/**
 * Retrieve the earliest datetime of content using Fedora's RI.
 *
 * @return string
 *   The earliest datestamp retrieved from Fedora.
 */
function islandora_oai_get_earliest_datetime() {
  static $earliest_datestamp;
  if (!$earliest_datestamp) {
    switch (variable_get('islandora_oai_query_backend', 'sparql')) {
      case 'sparql':
        $query = <<<EOQ
SELECT ?object ?date
FROM <#ri>
WHERE {
  ?object <fedora-model:state> <fedora-model:Active> ;
          <fedora-model:hasModel> <info:fedora/fedora-system:FedoraObject-3.0> ;
          <fedora-view:lastModifiedDate> ?date .
}
ORDER BY ?date
LIMIT 1
EOQ;
        $connection = islandora_get_tuque_connection();
        $query_results = $connection->repository->ri->sparqlQuery($query);
        $result = reset($query_results);
        $timestamp = strtotime($result['date']['value']);
        $earliest_datestamp = gmdate('Y-m-d\TH:i:s\Z', $timestamp);
        break;

      case 'solr':
        $date_field = variable_get('islandora_oai_date_field', 'fgs_lastModifiedDate_dt');
        $qp = new IslandoraSolrQueryProcessor();
        $params = array(
          'sort' => "$date_field asc",
          'limit' => 1,
        );
        $qp->buildQuery(format_string('!date_field:* AND !state_field:Active', array(
          '!date_field' => $date_field,
          '!state_field' => variable_get('islandora_oai_solr_state_field', 'fgs_state_s'),
        )));
        $qp->executeQuery(FALSE);
        $result = reset($qp->islandoraSolrResult['response']['objects']);
        $earliest_datestamp = $result['solr_doc'][$date_field];
        break;

    }
  }
  return $earliest_datestamp;
}

/**
 * Lists identifiers or records depending on values.
 *
 * @param object $writer
 *   The XMLWriter object being passed by reference for constructing our output.
 * @param array $args
 *   Arguments of the POST or GET Request
 * @param bool $list_rec
 *   List identifiers if FALSE, records if TRUE.
 *
 * @return bool
 *   FALSE if an error is encountered.
 */
function islandora_oai_list_id_rec(&$writer, $args, $list_rec = FALSE) {
  $query_args = array();
  // Max records to send in one response.
  $max_records = variable_get('islandora_oai_max_size', '20');
  $expire_seconds = variable_get('islandora_oai_expire_time', '86400');
  $expire_time = gmstrftime('%Y-%m-%dT%TZ', time() + $expire_seconds);
  // Handle error and set variables.
  foreach ($args as $key => $val) {
    switch ($key) {
      case 'from':
        if (!isset($from)) {
          $from = $val;
        }
        else {
          islandora_oai_add_error('badArgument', $key, $val);
          return FALSE;
        }
        // Ensure valid date/time format.
        if (islandora_oai_check_date_format($from)) {
          $from_time = islandora_oai_datetime_to_time($from);
        }
        else {
          islandora_oai_add_error('badGranularity', 'from', $from);
          return FALSE;
        }
        break;

      case 'until':
        if (!isset($until)) {
          $until = $val;
        }
        else {
          islandora_oai_add_error('badArgument', $key, $val);
          return FALSE;
        }
        // Ensure valid date/time format.
        if (islandora_oai_check_date_format($until)) {
          $until_time = islandora_oai_datetime_to_time($until);
        }
        else {
          islandora_oai_add_error('badGranularity', 'until', $until);
          return FALSE;
        }
        break;

      case 'set':
        if (!isset($set)) {
          $set = $val;
        }
        else {
          islandora_oai_add_error('badArgument', $key, $val);
          return FALSE;
        }
        break;

      case 'metadataPrefix':
        if (!isset($metadata_prefix)) {
          $result = db_query("SELECT * FROM {islandora_oai_metadata_formats} WHERE metadata_prefix=:metadata_prefix ", array(':metadata_prefix' => $val));

          if ($result->rowCount() > 0 && !isset($metadata_prefix)) {
            $metadata_prefix = $val;
          }
          else {
            islandora_oai_add_error('cannotDisseminateFormat', $key, $val);
            return FALSE;
          }
        }
        else {
          islandora_oai_add_error('badArgument', $key, $val);
          return FALSE;
        }
        break;

      case 'resumptionToken':
        // If a resumptionToken is present, we cannot have any other arguments.
        if (count($args) > 1) {
          islandora_oai_errors('clear');
          islandora_oai_add_error('exclusiveArgument');
          return FALSE;
        }
        elseif (!isset($resumption_token)) {
          $resumption_token = $val;
        }
        else {
          islandora_oai_add_error('badArgument', $key, $val);
          return FALSE;
        }
        break;

      default:
        islandora_oai_add_error('badArgument', $key, $val);
        return FALSE;
    }
  }

  // Resume previous session?
  if (isset($resumption_token)) {
    $token = islandora_oai_get_token($resumption_token);
    if (!is_object($token)) {
      return FALSE;
    }
    elseif ($token->timestamp < (time() - variable_get('islandora_oai_expire_time', '86400'))) {
      islandora_oai_add_error('badResumptionToken', '', $resumption_token);
      return FALSE;
    }
    $metadata_prefix = $token->metadata_prefix;
    $query_args = explode('#', $token->queryargs);
    $set = $token->setargs;
    if (isset($query_args) && (!empty($query_args[0]) || !empty($query_args[1]))) {
      $from = $query_args[0];
      $until = $query_args[1];
    }
  }
  // Make a new session.
  else {
    $token = new stdClass();
    $token->deliveredrecords = 0;
    $token->extquery = '';

    // Error checking.
    if (!isset($metadata_prefix)) {
      islandora_oai_add_error('missingArgument', 'metadataPrefix');
      return FALSE;
    }

    if (isset($from) && isset($until)) {
      if ($from_time > $until_time) {
        islandora_oai_add_error('badArgument', 'until and from', $from . ' and ' . $until);
        return FALSE;
      }
    }
  }
  $params = array(
    'token' => $token,
    'set' => isset($set) ? $set : FALSE,
    'from' => isset($from) ? $from : FALSE,
    'until' => isset($until) ? $until : FALSE,
  );
  // If it's ListRecords go grab the ListRecords request.
  if ($list_rec) {
    $list_records_request = islandora_oai_filter_function('ListRecords');
    if (isset($list_records_request['file'])) {
      require_once $list_records_request['file'];
    }
    $query_results = call_user_func($list_records_request['function'], $params);
  }
  // ListIdentifiers request.
  else {
    $list_identifiers_request = islandora_oai_filter_function('ListIdentifiers');
    if (isset($list_identifiers_request['file'])) {
      require_once $list_identifiers_request['file'];
    }
    $query_results = call_user_func($list_identifiers_request['function'], $params);
  }
  $num_rows = $query_results['total_records'];
  if ($num_rows) {
    if ($list_rec) {
      $writer->startElement('ListRecords');
    }
    else {
      $writer->startElement('ListIdentifiers');
    }
    foreach ($query_results['records'] as $record) {
      islandora_oai_build_record_response($writer, $record, $list_rec, $metadata_prefix);
    }

    if ($num_rows - $token->deliveredrecords > $max_records) {
      if (!isset($metadata_prefix)) {
        $metadata_prefix = 'oai_dc';
      }
      $token_string = islandora_oai_get_random_token();
      $arg_string = implode("#", $query_args);
      $delivered_records = (int) $token->deliveredrecords + $max_records;

      db_insert('islandora_oai_tokens')
        ->fields(array(
          'id' => $token_string,
          'deliveredrecords' => $delivered_records,
          'queryargs' => isset($arg_string) ? $arg_string : NULL,
          'setargs' => isset($set) ? $set : NULL,
          'metadata_prefix' => $metadata_prefix,
          'timestamp' => time(),
        ))
        ->execute();

      islandora_oai_write_token_xml($writer, $num_rows, $token->deliveredrecords, $expire_time, $token_string);
    }
    // Last delivery, return empty ResumptionToken.
    elseif (isset($resumption_token)) {
      islandora_oai_write_token_xml($writer, $num_rows, $token->deliveredrecords);
    }
    $writer->endElement();
  }
  else {
    islandora_oai_add_error('noRecordsMatch');
    return FALSE;
  }
}

/**
 * Build a record response given a record.
 *
 * @param object $writer
 *   The XMLWriter object being passed by reference for constructing our output.
 * @param array $record
 *   An array containing at least:
 *   -pid (string): The pid of the record that is being processed.
 *   -date (string): The date timestamp of the record of the lastModified date
 *   in Zulu format described below.
 *   Additional parameters may be present but the former are required.
 * @param bool $full_record
 *   Whether to display one record or not.
 * @param string $metadata_prefix
 *   The metadata prefix we are searching for.
 */
function islandora_oai_build_record_response(&$writer, $record, $full_record = FALSE, $metadata_prefix = 'oai_dc') {
  $identifier = $record['pid'];
  $identifier = str_replace(':', '_', $identifier);
  $repo_id = variable_get('islandora_oai_repository_identifier', 'drupal-site.org');
  $date = new DateTime((string) $record['date'], new DateTimeZone('UTC'));
  $date_stamp = $date->format("Y-m-d\TH:i:s\Z");

  if ($full_record) {
    $writer->startElement('record');
  }

  $writer->startElement('header');
  $writer->writeElement('identifier', 'oai:' . $repo_id . ':' . $identifier);
  $writer->writeElement('datestamp', $date_stamp);

  $set_membership = islandora_oai_filter_function('set_membership');
  if (isset($set_membership['file'])) {
    require_once $set_membership['file'];
  }
  $sets = call_user_func($set_membership['function'], array(
    'record' => $record,
  ));
  foreach ($sets as $set) {
    $set = str_replace('info:fedora/', '', $set);
    $set_spec = str_replace(':', '_', $set);
    $writer->writeElement('setSpec', $set_spec);
  }
  $writer->endElement();

  // List record values.
  if ($full_record) {
    $record_xml = islandora_oai_filter_function('response_xml');
    if (isset($record_xml['file'])) {
      require_once $record_xml['file'];
    }
    $oai_output = call_user_func($record_xml['function'], array(
      'metadata_prefix' => $metadata_prefix,
      'pid' => $record['pid'],
    ));
    if (!empty($oai_output)) {
      $writer->startElement('metadata');
      $writer->writeRaw($oai_output);
      $writer->endElement();
    }
    // Close our record.
    $writer->endElement();
  }
}

/**
 * Converts a time string to a DateTime string.
 *
 * @param string $datetime
 *   Time string pulled from the request.
 *
 * @return DateTime
 *   A DateTime string.
 */
function islandora_oai_datetime_to_time($datetime) {
  $time = strtotime($datetime);
  return $time;
}

/**
 * Generate the resumption token given a token string.
 *
 * @param object $writer
 *   The XMLWriter object being passed by reference for constructing our output.
 * @param int $num_rows
 *   The complete list size.
 * @param int $delivered_records
 *   The number of records that have been delivered.
 * @param string $expire_time
 *   The expiration time of the resumption token.
 * @param string $token
 *   The ID of the resumption token.
 */
function islandora_oai_write_token_xml(&$writer, $num_rows, $delivered_records, $expire_time = NULL, $token = NULL) {
  if ($token) {
    $writer->startElement('resumptionToken');
    $writer->writeAttribute('expirationDate', $expire_time);
    $writer->writeAttribute('completeListSize', $num_rows);
    $writer->writeAttribute('cursor', $delivered_records);
    $writer->text($token);
    $writer->endElement();
  }
  // Last delivery, return empty ResumptionToken.
  else {
    $writer->startElement('resumptionToken');
    $writer->writeAttribute('completeListSize', $num_rows);
    $writer->writeAttribute('cursor', $delivered_records);
    $writer->endElement();
  }
}

/**
 * Retrieves a resumption token from the db.
 *
 * @param string $resumption_token
 *   The ID of the resumption token we are searching for.
 *
 * @return bool|stdObject
 *   FALSE if the token doesn't exist, the token object otherwise.
 */
function islandora_oai_get_token($resumption_token) {
  $token_query = db_select('islandora_oai_tokens', 'i')
    ->fields('i', array())
    ->condition('id', $resumption_token)
    ->range(0, 1)
    ->execute()
    ->fetchObject();

  if ($token_query && $token_query->deliveredrecords) {
    return $token_query;
  }
  else {
    islandora_oai_add_error('badResumptionToken', '', $resumption_token);
    return FALSE;
  }
}

/**
 * List all the Metadata formats of this repository.
 *
 * @param object $writer
 *   The XMLWriter object being passed by reference for constructing our output.
 * @param array $args
 *   Arguments of the $_GET or $_POST request.
 *
 * @return bool
 *   FALSE if an error is encountered.
 */
function islandora_oai_list_metadata_formats(&$writer, $args) {
  foreach ($args as $key => $val) {
    switch ($key) {
      case 'identifier':
        $repo_id = trim(variable_get('islandora_oai_repository_identifier', 'drupal-site.org'));
        $identifier = str_replace('oai:' . $repo_id . ':', '', $val);

        $pid_query = str_replace('_', ':', $identifier);
        $query = <<<EOQ
ASK {
  <info:fedora/$pid_query> <fedora-model:hasModel> <info:fedora/fedora-system:FedoraObject-3.0> .
}
EOQ;
        $connection = islandora_get_tuque_connection();
        $query_results = $connection->repository->ri->sparqlQuery($query);
        $query_result = reset($query_results);
        if (!$query_results || $query_result === FALSE || strtolower($query_result['k0']['value']) != 'true') {
          islandora_oai_add_error('idDoesNotExist', 'identifier', $identifier);
          return FALSE;
        }

        break;

      case 'metadataPrefix':
        $result = db_query("SELECT * FROM {islandora_oai_metadata_formats} WHERE metadata_prefix=:metadata_prefix", array(':metadata_prefix' => $val));
        if ($result->rowCount() > 0) {
          $metadata_prefix = $val;
        }
        else {
          islandora_oai_add_error('cannotDisseminateFormat', $key, $val);
          return FALSE;
        }

        break;

      default:
        islandora_oai_add_error('badArgument', $key, $val);
        return FALSE;
    }
  }

  $list_meta_data_formats = array();
  $result = db_query("SELECT * FROM {islandora_oai_metadata_formats}");

  foreach ($result as $fmt) {
    $list_meta_data_formats[] = array(
      'metadataPrefix' => $fmt->metadata_prefix,
      'schema' => $fmt->oai2_schema,
      'metadataNamespace' => $fmt->metadata_namespace,
    );
  }

  if (count($list_meta_data_formats) > 0) {
    $writer->startElement('ListMetadataFormats');
    foreach ($list_meta_data_formats as $entry) {
      $writer->startElement('metadataFormat');
      $writer->writeElement('metadataPrefix', $entry['metadataPrefix']);
      $writer->writeElement('schema', $entry['schema']);
      $writer->writeElement('metadataNamespace', $entry['metadataNamespace']);
      $writer->endElement();
    }
    $writer->endElement();
  }
  else {
    islandora_oai_add_error('noMetadataFormats');
  }
}

/**
 * ListSets of this repository implemented as a list of collections.
 *
 * @param object $writer
 *   The XMLWriter object being passed by reference for constructing our output.
 *
 * @param array $args
 *   Arguments of the $_POST or the $_GET request.
 *
 * @return bool
 *   FALSE if an error is encountered.
 */
function islandora_oai_list_sets($writer, $args) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  // Max records to send in one response.
  $max_records = variable_get('islandora_oai_max_size', '20');
  $expire_seconds = variable_get('islandora_oai_expire_time', '86400');
  $expire_time = gmstrftime('%Y-%m-%dT%TZ', time() + $expire_seconds);
  // @TODO: Add support for descriptive metadata for sets?
  // http://www.openarchives.org/OAI/openarchivesprotocol.html#ListSets.
  foreach ($args as $key => $val) {
    switch ($key) {
      case 'resumptionToken':
        // If a resumptionToken is present, we cannot have any other arguments.
        if (count($args) > 1) {
          islandora_oai_errors('clear');
          islandora_oai_add_error('exclusiveArgument');
          return FALSE;
        }
        elseif (!isset($resumption_token)) {
          $resumption_token = $val;
        }
        break;

      default:
        islandora_oai_add_error('badArgument', $key, $val);
        return FALSE;
    }
  }
  // Resume previous session?
  if (isset($resumption_token)) {
    $token = islandora_oai_get_token($resumption_token);
    if (!is_object($token)) {
      return FALSE;
    }
    elseif ($token->timestamp < (time() - $expire_seconds)) {
      islandora_oai_add_error('badResumptionToken', '', $resumption_token);
      return FALSE;
    }
  }
  // Make a new session.
  else {
    $token = new stdClass();
    $token->deliveredrecords = 0;
    $token->extquery = '';
  }
  $params = array(
    'token' => $token,
    'max_records' => $max_records,
  );

  $list_sets_request = islandora_oai_filter_function('ListSets');
  if (isset($list_sets_request['file'])) {
    require_once $list_sets_request['file'];
  }
  $query_results = call_user_func($list_sets_request['function'], $params);

  $total_records = $query_results['total_records'];
  $print_list_set = FALSE;
  foreach ($query_results['records'] as $result) {
    // Logic to print our ListSets only if we have valid records. Will only fire
    // once.
    if (!$print_list_set) {
      $writer->startElement('ListSets');
      $print_list_set = TRUE;
    }
    $writer->startElement('set');
    $set_name = $result['label'];

    if (empty($set_name)) {
      $set_name = t('Empty label');
    }
    $set_pid = str_replace(':', '_', $result['pid']);
    $writer->writeElement('setSpec', $set_pid);
    $writer->writeElement('setName', $set_name);

    if (isset($result['description'])) {
      $writer->startElement('setDescription');
      $writer->writeRaw($result['description']);
      $writer->endElement();
    }
    $writer->endElement();
  }
  // Need to close our ListSets element.
  if ($print_list_set) {
    if ($total_records - $token->deliveredrecords > $max_records) {
      $token_string = islandora_oai_get_random_token();
      $delivered_records = (int) $token->deliveredrecords + $max_records;

      db_insert('islandora_oai_tokens')
        ->fields(array(
          'id' => $token_string,
          'deliveredrecords' => $delivered_records,
          'timestamp' => time(),
        ))
        ->execute();
      islandora_oai_write_token_xml($writer, $total_records, $token->deliveredrecords, $expire_time, $token_string);
    }
    // Last delivery, return empty ResumptionToken.
    elseif (isset($resumption_token)) {
      islandora_oai_write_token_xml($writer, $total_records, $token->deliveredrecords);
    }
    $writer->endElement();
  }
  else {
    islandora_oai_add_error('noSetHierarchy');
  }
}
