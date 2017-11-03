<?php

/**
 * @file
 * Callbacks for creating updating / deleting annotations.
 */

define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_DC', 'http://purl.org/dc/elements/1.1/');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_DCTERMS', 'http://purl.org/dc/terms/');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_DCTYPE', 'http://purl.org/dc/dcmitype/');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_OA', 'http://www.w3.org/ns/openannotation/core/');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_CNT', 'http://www.w3.org/2008/content#');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_DMS', 'http://dms.stanford.edu/ns/');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_RDF', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_ORE', 'http://www.openarchives.org/ore/terms/');
define('ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_EXIF', 'http://www.w3.org/2003/12/exif/ns#');

/**
 * Adds an annotation to the give object.
 *
 * The following parameters are expected to be posted for this function to work.
 */
function islandora_image_annotation_settings(AbstractObject $object) {
  module_load_include('inc', 'islandora_image_annotation', 'includes/utils');
  drupal_json_output(islandora_image_annotation_get_settings($object));
  drupal_exit();
}

/**
 * Adds an annotation to the give object.
 *
 * The following parameters are expected to be posted for this function to work.
 */
function islandora_image_annotation_get_callback(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  islandora_view_datastream($object['OAC']);
}

/**
 * Adds an annotation to the give object.
 *
 * @todo Change the JS code to post plain RDF so it doesn't need to be
 * converted.
 *
 * The $_POST data should contain the following fields.
 *   - data: An RDF document encoded as a fragment of HTML.
 *
 * @param AbstractObject $object
 *   The object in which the new annotation will 'annotate'.
 */
function islandora_image_annotation_add_callback(AbstractObject $object) {
  try {
    $annotation_object = islandora_image_annotation_create_annotation($object);
    // Ingest the object and return it's PID to the client side application.
    islandora_add_object($annotation_object);
    echo $annotation_object->id;
  }
  catch (IslandoraImageAnnotationPropertyMissingException $e) {
    drupal_add_http_header('Status', '400 Bad Request');
  }
  drupal_exit();
}

class IslandoraImageAnnotationPropertyMissingException extends Exception {}

/**
 * Create a new annotation object.
 *
 * @param AbstractObject $object
 *   The object in which the new annotation will 'annotate'.
 * @param DOMDocument $rdf
 *   A DOMDocument containing the parsed RDF, as might be returned by
 *   islandora_image_annotation_html_to_rdf(). If not passed, we will try to
 *   parse this out of $_POST['data'].
 */
function islandora_image_annotation_create_annotation(AbstractObject $object, $rdf = NULL) {
  global $user;
  module_load_include('inc', 'islandora', 'includes/utilities');
  if ($rdf === NULL) {
    // Transform the posted HTML into RDF.
    // @ignore security_17
    $rdf = islandora_image_annotation_html_to_rdf($_POST['data']);
  }
  $properties = islandora_image_annotation_parse_rdf($rdf);
  // We require at minimum a label, type, and urn to create the annotation.
  if (empty($properties['label']) || empty($properties['type']) || empty($properties['urn'])) {
    throw new IslandoraImageAnnotationPropertyMissingException('One of "label", "type" or "urn" missing.');
  }
  // Create the new annotation object.
  $namespace = islandora_get_namespace($object->id);
  $annotation_object = $object->repository->constructObject($namespace);
  $annotation_object->label = $properties['label'];
  $annotation_object->models = 'islandora:OACCModel';
  $annotation_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $object->id);
  $annotation_object->relationships->add(FEDORA_RELS_EXT_URI, 'isAnnotationOf', $object->id);
  $annotation_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'targetedBy', $user->name, RELS_TYPE_PLAIN_LITERAL);
  $annotation_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'targets', $object->id);
  $annotation_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isAnnotationType', $properties['type'], RELS_TYPE_PLAIN_LITERAL);
  $annotation_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasURN', $properties['urn'], RELS_TYPE_PLAIN_LITERAL);
  if (!empty($properties['entity'])) {
    $annotation_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isEntity', $properties['entity'], RELS_TYPE_PLAIN_LITERAL);
  }
  // Create Datastream for the RDF.
  $oac_datastream = $annotation_object->constructDatastream('OAC', 'X');
  $oac_datastream->label = 'OAC';
  $oac_datastream->mimetype = 'text/xml';
  $oac_datastream->content = $rdf->saveXML();
  $annotation_object->ingestDatastream($oac_datastream);
  return $annotation_object;
}

/**
 * Converts the given HTML fragment to RDF.
 *
 * @param string $html_fragment
 *   The HTML fragment to convert, it's expected to be in a format generated by
 *   the client side javascript.
 *
 * @return DOMDocument
 *   The given fragment encoded as RDF XML.
 */
function islandora_image_annotation_html_to_rdf($html_fragment) {
  $module_path = drupal_get_path('module', 'islandora_image_annotation');
  $processor = new XSLTProcessor();
  $xsl = new DOMDocument();
  $xsl->load("$module_path/xslt/html_to_annotation_list.xsl");
  $input = new DomDocument();
  $input->loadXML(trim($html_fragment));
  $processor->importStylesheet($xsl);
  return $processor->transformToDoc($input);
}

/**
 * Parses the given Annotation's RDF XML extracting values of interest.
 *
 * @param DOMDocument $rdf
 *   The Annotation's RDF to parse.
 *
 * @return array
 *   An associative array of values extracted from the given RDF XML.
 *   - label: The label of the annotation.
 *   - type: The type of the annotation.
 */
function islandora_image_annotation_parse_rdf(DOMDocument $rdf) {
  $xpath = new DOMXPath($rdf);
  $type = ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_OA . 'Annotation';
  $urn = $xpath->evaluate("string(//rdf:Description[rdf:type/@rdf:resource='$type']/@rdf:about)");
  return array(
    'label' => $xpath->evaluate('string(//dc:title[1])'),
    'type' => $xpath->evaluate('string(//dc:type[1])'),
    // Entity is optional, and may not be present.
    'entity' => $xpath->evaluate('string(//dcterms:relation[1])'),
    'urn' => $urn,
  );
}

/**
 * Updates the annotation with the posted data.
 *
 * The $_POST data should contain the following fields.
 *  - label: The new label for the annotation
 *  - type: The new type for the annotation
 *  - content: The new text content of the annotation.
 *
 * @param AbstractObject $object
 *   The annotation to update.
 */
function islandora_image_annotation_update_callback(AbstractObject $object) {
  $label = check_plain($_POST['label']);
  $type = check_plain($_POST['type']);
  $content = check_plain($_POST['content']);
  $xml = simplexml_load_string($object['OAC']->content);
  $result = $xml->xpath('//dc:title[1]');
  $result[0][0] = $label;
  $result = $xml->xpath('//dc:type[1]');
  $result[0][0] = $type;
  $content_as_text = ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_CNT . 'ContentAsText';
  $svg_constraint = ISLANDORA_IMAGE_ANNOTATION_NAMESPACE_OA . 'SvgConstraint';
  $query = <<<EOT
  //rdf:Description[rdf:type/@rdf:resource = "$content_as_text" and
    not(rdf:type/@rdf:resource = "$svg_constraint")]/cnt:chars[1]
EOT;
  $result = $xml->xpath($query);
  $result[0][0] = $content;
  $object['OAC']->content = $xml->asXML();
  // Update the RELS-EXT portion.
  $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'isAnnotationType', NULL, RELS_TYPE_PLAIN_LITERAL);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isAnnotationType', $type, RELS_TYPE_PLAIN_LITERAL);
}

/**
 * Deletes the given annotation object identified by the uuid.
 *
 * The following parameters are expected to be posted for this function to work.
 *
 * @param AbstractObject $object
 *   The annotation to delete.
 */
function islandora_image_annotation_delete_callback(AbstractObject $object) {
  islandora_delete_object($object);
}

/**
 * Gets a list of object title's that match the given type.
 *
 * @param string $type
 *   The type to limit the titles to.
 */
function islandora_image_annotation_title_search($type = '') {
  if (!module_exists('islandora_solr')) {
    drupal_json_output(array());
    drupal_exit();
  }
  $solr_field = variable_get('islandora_image_annotation_solr_field_title', 'mods_titleInfo_title_s');
  $data = array(
    'q' => format_string('!field:"!value"', array(
      '!field' => variable_get('islandora_image_annotation_solr_field_genre', 'mods_genre_s'),
      '!value' => $type,
    )),
    'fl' => "PID, $solr_field",
    'start' => '0',
    'rows' => '10000',
    'wt' => 'json',
    'qt' => variable_get('islandora_solr_request_handler', 'standard'),
    'sort' => "$solr_field asc",
  );
  $solr = variable_get('islandora_solr_url', 'localhost:8080/solr');
  $solr_url = url($solr . '/select', array('query' => $data));
  $result_request = drupal_http_request($solr_url);
  $results = json_decode($result_request->data, TRUE);
  $titles = array();
  foreach ($results['response']['docs'] as $doc) {
    $titles[] = $doc[$solr_field];
  }
  $titles = array_unique($titles);
  sort($titles);
  drupal_json_output($titles);
  drupal_exit();
}
