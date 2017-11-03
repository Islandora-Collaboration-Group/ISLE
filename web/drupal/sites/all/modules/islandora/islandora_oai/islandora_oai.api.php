<?php

/**
 * @file
 * Hooks provided by Islandora OAI.
 */

/**
 * Get module supplied XSL files for transforming content to output for OAI.
 *
 * @return array
 *   Returns an associative array where the key is the URI to the file on the
 *   file system and the value is the name of the file.
 */
function hook_islandora_oai_get_xsl_files() {
}

/**
 * Identify the availability of a custom request handler for OAI requests.
 *
 * @return array
 *   An associative array containing:
 *   -label: A string describing the handler.
 *   -description: A string describing the request handler.
 *   -configuration (optional): A relative menu path to custom configuration for
 *   the handler.
 *   -requests: An array containing modifiable requests and function to call.
 *   Each of the requests is considered optional and will always default back
 *   to the generic implementation if not implemented in a custom handler.
 *   Each request takes a parameters array as defined below. Available requests
 *   to override:
 *    -ListIdentifiers: The ListIdentifiers OAI request, params array contains:
 *      -set: String of the PID of the set being searched for, FALSE if no set
 *      argument.
 *      -from: String of the from time starting point, FALSE if no argument.
 *      -until: String of the until ending point, FALSE if no until argument.
 *      -token: Object representing a token.
 *    Expected to return an array of arrays containing:
 *      -total_records: An integer with the number of total results for the
 *      request.
 *      -records: An array of arrays containing at least:
 *        -pid: A string representing the PID of the result.
 *        -date: A string date timestamp used to identify the lastModified date
 *        of the record.
 *    More fields may be present to be used by custom handlers defined below
 *    but the former two fields are required for every implementation.
 *    -ListRecords: The ListRecords OAI request, params array contains:
 *      -set: String of the PID of the set being searched for, FALSE if no set
 *      argument.
 *      -from: String of the from time starting point, FALSE if no argument.
 *      -until: String of the until ending point, FALSE if no until argument.
 *      -token: Object representing a token.
 *    Expected to return an array of arrays containing at least:
 *      -total_records: An integer with the number of total results for the
 *      request.
 *      -records: An array of arrays containing at least:
 *        -pid: A string representing the PID of the result.
 *        -date: A string date timestamp used to identify the lastModified date
 *        of the record.
 *    -ListSets: The ListSets OAI request, params array contains:
 *      -token: A resumption token object specifying information about a
 *      previous request, or a new empty one.
 *      -max_records: An integer of the maximum number of records to return per
 *      request.
 *    Expected to return an array containing:
 *      -total_records: An integer with the number of total results for the
 *      request.
 *      -records: An array of arrays containing:
 *        -pid: A string representing the PID of the set.
 *        -label: A string containing the label of the set.
 *        -description (optional): An XML string detailing the set.
 *    -GetRecord: The GetRecord OAI request, params array contains:
 *      -pid: A string representing the PID of the record being requested.
 *    Expected to return FALSE if the record does not exist, otherwise an array
 *    containing at least:
 *      -pid: A string representing the PID of the result.
 *      -date: A string date timestamp used to identify the lastModified date
 *      of the record.
 *    More fields may be present to be used by custom handlers defined below
 *    but the former two fields are required for every implementation.
 *    -response_xml: Used to construct the individual XML response for a record,
 *    params array contains:
 *      -metadata_prefix: String describing the metadata prefix of the request
 *      being executed.
 *      -pid: A string representing the PID of the record being requested.
 *    Expected to return a string containing the XML to be rendered, or NULL
 *    if an error is encountered.
 *    -set_membership: Used to construct the setSpec portion of record
 *    responses, params array contains:
 *      -pid: A string representing the PID of the result.
 *      -date: A string date timestamp used to identify the lastModified date
 *      of the record.
 *    More fields may be present to be used by custom handlers defined above
 *    but the former two fields are required for every implementation.
 *    Expected to return an array containing a list of PIDs of the sets that
 *    the record belongs to.
 */
function hook_islandora_oai_identify_request_handler() {
  return array(
    'my_cool_oai_handler' => array(
      'label' => t('My Cool OAI Handler'),
      'description' => t('Provides a standard OAI implementation for Islandora.'),
      'configuration' => 'admin/islandora/tools/my-cool-oai/handler',
      'requests' => array(
        'ListIdentifiers' => array(
          'file' => drupal_get_path('module', 'my_cool_oai_handler') . '/includes/handler.inc',
          'function' => 'my_cool_oai_handler_retrieve_records_or_identifiers',
        ),
        'ListRecords' => array(
          'file' => drupal_get_path('module', 'my_cool_oai_handler') . '/includes/handler.inc',
          'function' => 'my_cool_oai_handler_retrieve_records_or_identifiers',
        ),
        'ListSets' => array(
          'file' => drupal_get_path('module', 'my_cool_oai_handler') . '/includes/handler.inc',
          'function' => 'my_cool_oai_handler_retrieve_sets',
        ),
        'GetRecord' => array(
          'file' => drupal_get_path('module', 'my_cool_oai_handler') . '/includes/handler.inc',
          'function' => 'my_cool_oai_handler_retrieve_record',
        ),
        'response_xml' => array(
          'file' => drupal_get_path('module', 'my_cool_oai_handler') . '/includes/handler.inc',
          'function' => 'my_cool_oai_handler_object_response_xml',
        ),
        'set_membership' => array(
          'file' => drupal_get_path('module', 'my_cool_oai_handler') . '/includes/handler.inc',
          'function' => 'my_cool_oai_handler_get_membership',
        ),
      ),
    ),
  );
}

/**
 * Get module supplied XSLT params for self transform.
 *
 * * @param AbstractObject $object
 *   The object being called for the OAI request
 *
 * @param string $metadata_prefix
 *   The handler metadata prefix for the request.
 *
 * @return array
 *   Returns an associative array of arrays. The key of each array is the
 *   namespace for the parameters, where the array contains the params to be
 *   passed to the XSLT transform.
 */
function hook_islandora_oai_self_transform_params($object, $metadata_prefix) {
}
