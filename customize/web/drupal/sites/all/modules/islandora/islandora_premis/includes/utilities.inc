<?php

/**
 * @file
 * Helper functions used by this module.
 */

/**
 * Run an XSLT, and return the results.
 *
 * @param array $info
 *   An associative array of parameters, containing:
 *   - input: The input XML in a string.
 *   - xsl: The path to an XSLT file.
 *   - functions: An optional value indicating PHP functions to make available
 *     in the XSLT. Either a string indicating the name of the function, or an
 *     array of multiple strings, each indicating the name of a function.
 *   - parameters: An associative array of associative arrays. Keys of the
 *     outer array indicate the namespace URI of the parameters to insert (for
 *     no namespace, use the empty string). Inner keys indicate parameter names,
 *     with values being the values for the give parameter.
 *
 * @return string
 *   The transformed XML, as a string.
 */
function islandora_premis_run_xsl_transform($info) {
  $info += array(
    'functions' => array(),
    'parameters' => array(),
  );

  $xsl = new DOMDocument();
  $xsl->load($info['xsl']);
  $input = new DOMDocument();
  $input->loadXML($info['input']);
  $processor = new XSLTProcessor();
  $processor->importStylesheet($xsl);
  if ($info['functions']) {
    $processor->registerPHPFunctions($info['functions']);
  }
  foreach ($info['parameters'] as $ns => $parameters) {
    $processor->setParameter($ns, $parameters);
  }

  // @ suppresses warnings regarding unregistered prefixes.
  return @$processor->transformToXML($input);
}

/**
 * Grabs FOXML the given object, and returns it transformed to PREMIS.
 *
 * @param AbstractObject $object
 *   A Fedora object containing a FOXML datastream.
 *
 * @return array|bool
 *   An array containing the serialized FOXML and the path to the stylesheet,
 *   or FALSE if the FOXML is not retrievable.
 */
function islandora_premis_transform_foxml_to_premis(AbstractObject $object) {
  $foxml_str = islandora_premis_get_foxml($object);
  if (!$foxml_str) {
    drupal_not_found();
  }

  // See https://jira.duraspace.org/browse/ISLANDORA-2016.
  $creating_application_info = system_get_info('module', 'islandora');

  return islandora_premis_run_xsl_transform(array(
    'input' => $foxml_str,
    'xsl' => drupal_get_path('module', 'islandora_premis') . '/xml/foxml_to_premis.xsl',
    'functions' => array(
      'islandora_premis_get_object_characteristics_extension',
    ),
    'parameters' => array(
      '' => array(
        'premis_agent_name_organization' => variable_get('islandora_premis_agent_name', 'Your Organization'),
        'premis_agent_identifier_organization' => variable_get('islandora_premis_agent_identifier', 'some_unique_string'),
        'premis_agent_identifier_type' => variable_get('islandora_premis_agent_identifier_type', 'MARC Organization Codes'),
        'premis_agent_type_organization' => variable_get('islandora_premis_agent_type', 'organization'),
        'premis_object_creating_application_name' => $creating_application_info['name'],
        'fedora_commons_version' => islandora_premis_get_fc_version(),
      ),
    ),
  ));
}

/**
 * Produce HTML representing PREMIS content.
 *
 * @param AbstractObject $object
 *   A Fedora object containing a MODS datastream.
 *
 * @return string
 *   The transformed HTML, as a string.
 */
function islandora_premis_transform_premis_to_html(AbstractObject $object) {
  return islandora_premis_run_xsl_transform(array(
    'input' => islandora_premis_transform_foxml_to_premis($object),
    'xsl' => drupal_get_path('module', 'islandora_premis') . '/xml/premis_to_html.xsl',
  ));
}


/**
 * Renders the PREMIS tab.
 *
 * @pararm AbstractObject $object
 *   The object the PREMIS belongs to.
 *
 * @return array
 *   A renderable array representing the tab.
 */
function islandora_premis_view(AbstractObject $object) {
  $tab = array();
  if (islandora_object_access('download premis metadata', $object)) {
    $tab['link'] = array('#markup' => l(t('Download PREMIS'), "islandora/object/{$object->id}/manage/premis/download"));
  }
  if (islandora_object_access('view premis metadata', $object)) {
    $tab['premis'] = array('#markup' => islandora_premis_transform_premis_to_html($object));
  }
  return $tab;
}

/**
 * Gets FOXML via tuque.
 *
 * @pararm AbstractObject $object
 *   The object the FOXML belongs to
 *
 * @return string|bool
 *   The FOXML, or FALSE if the FOXML is not available.
 */
function islandora_premis_get_foxml(AbstractObject $object) {
  $foxml = FALSE;
  // Use the Tuque API-M to export the FOXML for the object.
  try {
    $connection = islandora_get_tuque_connection();
    $foxml = $connection->api->m->export($object->id, array(
      'format' => 'info:fedora/fedora-system:FOXML-1.1',
      'context' => 'migrate',
      'encoding' => 'UTF-8',
    ));
  }
  catch (Exception $e) {
    drupal_set_message(t(
        "Error retrieving FOXML required to generate PREMIS: '@message'",
        array("@message" => $e->getMessage())));
    watchdog_exception(
      'islandora_premis',
      $e,
      'Error retrieving FOXML required to generate PREMIS: %type: !message in %function (line %line of %file).'
    );
  }

  return $foxml;
}

/**
 * Performs call to Fedora REST API to get version number.
 *
 * @return string
 *   A string representing the Fedora Commons version
 */
function islandora_premis_get_fc_version() {
  $tuque = islandora_get_tuque_connection();
  $repo_description = $tuque->repository->api->a->describeRepository();
  return $repo_description['repositoryVersion'];
}

/**
 * Get XML for use as the OBJ's "object characteristics extension".
 *
 * @param string $pid
 *   The ID of the object for which XML is to be grabbed.
 * @param string $dsid
 *   The ID of the datastream on the object for which XML is to be grabbed.
 *
 * @return DOMDocument|bool
 *   A DOMDocument containing the XML, or FALSE if none is available.
 */
function islandora_premis_get_object_characteristics_extension($pid, $dsid) {
  $object = islandora_object_load($pid);
  if ($dsid == 'OBJ') {
    $fits_dsid = variable_get('islandora_fits_techmd_dsid', 'TECHMD');
    if (isset($object[$fits_dsid]) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$fits_dsid])) {
      $doc = new DOMDocument();
      $doc->loadXML($object[$fits_dsid]->content);
      return $doc;
    }
  }

  return FALSE;
}
