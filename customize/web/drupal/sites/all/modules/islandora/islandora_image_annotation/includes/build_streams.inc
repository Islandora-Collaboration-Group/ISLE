<?php

/**
 * @file
 * Functions to build and retrieve OAC RDF datastreams in fedora.
 *
 * @todo clean up the height/width code using paged content.
 */

/**
 * Builds initial suite of xml's for Shared Canvas installation.
 *
 * @global url $base_url
 *
 * @param string $pid
 *   Pid of the object.
 */
function islandora_image_annotation_build_manifest($pid) {
  module_load_include('inc', 'islandora_image_annotation', 'includes/utils');
  $dsid = islandora_image_annotation_get_dsid_from_model($pid);
  global $base_url;
  $item = islandora_object_load($pid);

  $title = $item->label;
  $file_array = array();
  $doc_url = "$base_url/islandora/object/$pid/datastream/$dsid/view/";
  // XML datastreams used by Shared Canvas must be echoed though ajax.
  $doc_path_url = "$base_url/islandora/anno/serve/$pid";
  $dimensions = islandora_image_annotation_get_height_and_width($pid);

  $height = $dimensions['height'];
  $width = $dimensions['width'];
  $file_array['Manifest'] = islandora_image_annotation_get_manifest($doc_url, $doc_path_url);
  $file_array['ImageAnnotations'] = islandora_image_annotation_get_image_annotations($doc_url, $doc_path_url, $height, $width, $title);
  $file_array['NormalSequence'] = islandora_image_annotation_get_normal_sequence($doc_url, $doc_path_url, $pid, $height, $width);
  $file_array['AnnotationList'] = islandora_image_annotation_get_annotation_list($doc_url, $doc_path_url);
  $flat_pid = str_replace(':', '_', $pid);
  $file_path = file_directory_path() . "/$flat_pid/manifest";
  if (!is_dir($file_path)) {
    mkdir($file_path, 0777, TRUE);
  }
  foreach ($file_array as $name => $content) {
    $item["$name"]->content = $content;
  }
  rmdir($file_path);
  rmdir(file_directory_path() . "/$flat_pid");
  return "completed";
}

/**
 * Returns basic manifest for unpopulated document.
 *
 * @param URL $doc_url
 *   URL of the doc.
 *
 * @param URL $doc_path_url
 *   Path where we are looking for the manifest.
 *
 * @return XML
 *   XML generated in the RDF builder.
 */
function islandora_image_annotation_get_manifest($doc_url, $doc_path_url) {
  $timestamp = date(DATE_W3C);
  module_load_include('inc', 'islandora_image_annotation', 'includes/rdf_builder');
  $rdf = new IslandoraRdfBuilder();

  $name = "$doc_url/Manifest";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/Aggregation'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/Manifest'));
  $children[] = $rdf->buildChild('ore:aggregates', array('rdf:resource' => "$doc_url/NormalSequence"));
  $children[] = $rdf->buildChild('ore:aggregates', array('rdf:resource' => "$doc_url/AnnotationList"));
  $children[] = $rdf->buildChild('ore:aggregates', array('rdf:resource' => "$doc_url/ImageAnnotations"));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_path_url/Manifest/manifest.xml";
  $children = array();
  $children[] = $rdf->buildChild('ore:describes', array('rdf:resource' => "$doc_url/Manifest"));
  $children[] = $rdf->buildChild('dcterms:modified', NULL, "$timestamp");
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/ResourceMap'));
  $rdf->addDescriptionNode($name, $children);

  $name = "urn:uuid:07fbb971-6acb-48ce-9ed4-1789dfe5b3a0";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://purl.org/dc/terms/Agent'));
  $children[] = $rdf->buildChild('foaf:name', NULL, "SharedCanvas: Python/rdflib");
  $children[] = $rdf->buildChild('foaf:mbox', NULL, "info@islandora.ca");
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_url/NormalSequence";
  $children = array();
  $children[] = $rdf->buildChild('ore:isDescribedBy', array('rdf:resource' => "$doc_path_url/NormalSequence/NormalSequence.xml"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/Aggregation'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/Sequence'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.w3.org/1999/02/22-rdf-syntax-ns#List'));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_path_url/NormalSequence/NormalSequence.xml";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/ResourceMap'));
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_url/ImageAnnotations";
  $children = array();
  $children[] = $rdf->buildChild('ore:isDescribedBy', array('rdf:resource' => "$doc_path_url/ImageAnnotations/ImageAnnotations.xml"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/Aggregation'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/ImageAnnotationList'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.w3.org/1999/02/22-rdf-syntax-ns#List'));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_path_url/ImageAnnotations/ImageAnnotations.xml";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/ResourceMap'));
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');
  $rdf->addDescriptionNode($name, $children);
  return $rdf->printRdf();
}

/**
 * Returns a NormalSequence document.
 *
 * We consider annotate single pages, so the document refrences a single canvas.
 *
 * @param URL $doc_url
 *   The URL of the document.
 *
 * @param URL $doc_path_url
 *   The path to the normal sequence XML file.
 *
 * @param string $pid
 *   The pid of the object.
 *
 * @param int $height
 *   The height.
 *
 * @param int $width
 *   The width.
 *
 * @return XML
 *   XML generated in the RDF builder.
 */
function islandora_image_annotation_get_normal_sequence($doc_url, $doc_path_url, $pid, $height, $width) {
  module_load_include('inc', 'islandora_image_annotation', 'includes/rdf_builder');
  $timestamp = date(DATE_W3C);
  $rdf = new IslandoraRdfBuilder();
  $rdf->root->setAttribute('xmlns:exif', "http://www.w3.org/2003/12/exif/ns#");

  $name = "$doc_url/NormalSequence";
  $children = array();
  $children[] = $rdf->buildChild('rdf:first', array('rdf:resource' => "$doc_url/Canvas"));
  $children[] = $rdf->buildChild('rdf:rest', array('rdf:resource' => "http://www.w3.org/1999/02/22-rdf-syntax-ns#nil"));
  $children[] = $rdf->buildChild('ore:aggregates', array('rdf:resource' => "$doc_url/Canvas"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => "http://www.w3.org/1999/02/22-rdf-syntax-ns#List"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => "http://www.openarchives.org/ore/terms/Aggregation"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => "http://dms.stanford.edu/ns/Sequence"));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_path_url/NormalSequence/NormalSequence.xml";
  $children = array();
  $children[] = $rdf->buildChild('ore:describes', array('rdf:resource' => "$doc_url/NormalSequence"));
  $children[] = $rdf->buildChild('dcterms:modified', NULL, "$timestamp");
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/ResourceMap'));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_url/Canvas";
  $children = array();
  $children[] = $rdf->buildChild('exif:height', array('rdf:datatype' => "http://www.w3.org/2001/XMLSchema#integer"), $height);
  $children[] = $rdf->buildChild('exif:width', array('rdf:datatype' => "http://www.w3.org/2001/XMLSchema#integer"), $width);
  $children[] = $rdf->buildChild('dc:title', NULL, "$pid canvas");
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/Canvas'));
  $rdf->addDescriptionNode($name, $children);

  $name = "urn:uuid:07fbb971-6acb-48ce-9ed4-1789dfe5b3a0";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://purl.org/dc/terms/Agent'));
  $children[] = $rdf->buildChild('foaf:name', NULL, "SharedCanvas: Python/rdflib");
  $children[] = $rdf->buildChild('foaf:mbox', NULL, "info@islandora.ca");
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');

  return $rdf->printRdf();
}

/**
 * Creates and returns Imageannotation Stream for new Shared Canvas.
 *
 * @param URL $doc_url
 *   The URL of the document.
 *
 * @param URL $doc_path_url
 *   The path to the image annotations XML.
 *
 * @param int $height
 *   The height of the annotation.
 *
 * @param int $width
 *   The width of the annotation.
 *
 * @param string $title
 *   The title of the annotation.
 *
 * @return XML
 *   XML generated in the RDF builder.
 */
function islandora_image_annotation_get_image_annotations($doc_url, $doc_path_url, $height, $width, $title) {
  module_load_include('inc', 'islandora_image_annotation', 'includes/rdf_builder');
  module_load_include('inc', 'islandora', 'includes/utilities');
  $rdf = new IslandoraRdfBuilder();
  $rdf->root->setAttribute('xmlns:exif', "http://www.w3.org/2003/12/exif/ns#");
  $rdf->root->setAttribute('xmlns:cnt', "http://www.w3.org/2008/content#");
  $rdf->root->setAttribute('xmlns:oa', "http://www.w3.org/ns/openannotation/core/");
  $option_uuid = islandora_image_annotation_get_uuid();
  $first_resource = islandora_image_annotation_get_uuid();
  $timestamp = date(DATE_W3C);

  $name = "$doc_url/ImageAnnotations";
  $children = array();
  $children[] = $rdf->buildChild('ore:aggregates', array('rdf:resource' => 'urn:uuid:46e74f54-a8e2-41f1-a3a1-24660472c584'));
  $children[] = $rdf->buildChild('rdf:first', array('rdf:resource' => 'urn:uuid:b6cf1868-68f9-4994-90dd-240a4a959358'));
  $children[] = $rdf->buildChild('rdf:rest', array('rdf:nodeID' => 'bCfJsWeh546'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/Aggregation'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/ImageAnnotationList'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.w3.org/1999/02/22-rdf-syntax-ns#List'));
  // @TODO add elements for aggregated annotations.
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_path_url/ImageAnnotations/ImageAnnotations.xml";
  $children = array();
  $children[] = $rdf->buildChild('dcterms:modified', NULL, "$timestamp");
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');
  $children[] = $rdf->buildChild('ore:describes', array('rdf:resource' => "$doc_url/ImageAnnotations"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.openarchives.org/ore/terms/ResourceMap'));
  $rdf->addDescriptionNode($name, $children);

  $name = "$first_resource";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/ImageAnnotation'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://www.w3.org/ns/openannotation/core/Annotation'));
  $children[] = $rdf->buildChild('oa:hasBody', array('rdf:resource' => $option_uuid));
  $children[] = $rdf->buildChild('oa:hasTarget', array('rdf:resource' => "$doc_url/Canvas"));
  $rdf->addDescriptionNode($name, $children);

  $name = "urn:uuid:e5c12519-e756-4442-a83d-556848c32714";
  $children = array();
  $children[] = $rdf->buildChild('foaf:name', NULL, "SharedCanvas: Python/rdflib");
  $children[] = $rdf->buildChild('foaf:mbox', NULL, "info@islandora.ca");
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://purl.org/dc/terms/Agent'));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_url";
  $children = array();
  $children[] = $rdf->buildChild('exif:width', array('rdf:datatype' => 'http://www.w3.org/2001/XMLSchema#integer'), $height);
  $children[] = $rdf->buildChild('exif:height', array('rdf:datatype' => "http://www.w3.org/2001/XMLSchema#integer"), $width);
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://purl.org/dc/dcmitype/Image'));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/ImageBody'));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_url/Canvas";
  $children = array();
  $children[] = $rdf->buildChild('exif:width', array('rdf:datatype' => 'http://www.w3.org/2001/XMLSchema#integer'), $height);
  $children[] = $rdf->buildChild('exif:height', array('rdf:datatype' => "http://www.w3.org/2001/XMLSchema#integer"), $width);
  $children[] = $rdf->buildChild('dc:title', NULL, "$title");
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/Canvas'));
  $rdf->addDescriptionNode($name, $children);

  $name = "$option_uuid";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => 'http://dms.stanford.edu/ns/ImageChoice'));
  $children[] = $rdf->buildChild('dms:option', array('rdf:resource' => "$doc_url"));
  $rdf->addDescriptionNode($name, $children);

  return $rdf->printRdf();
}

/**
 * Creates and returns AnnotationList for new Shared Canvas.
 *
 * @param URL $doc_url
 *   The document URL.
 *
 * @param URL $doc_path_url
 *   Path document path URL.
 *
 * @return XML
 *   XML generated in the RDF builder.
 */
function islandora_image_annotation_get_annotation_list($doc_url, $doc_path_url) {

  module_load_include('inc', 'islandora_image_annotation', 'includes/rdf_builder');
  $timestamp = date(DATE_W3C);
  $rdf = new IslandoraRdfBuilder();
  $rdf->root->setAttribute('xmlns:oa', "http://www.w3.org/ns/openannotation/core/");

  $name = "$doc_url/AnnotationList";
  $children = array();
  $children[] = $rdf->buildChild('rdf:rest', array('rdf:resource' => "http://www.w3.org/1999/02/22-rdf-syntax-ns#nil"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => "http://dms.stanford.edu/ns/AnnotationList"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => "http://www.w3.org/1999/02/22-rdf-syntax-ns#List"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => "http://www.openarchives.org/ore/terms/Aggregation"));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_url/Canvas";
  $children = array();
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => "http://dms.stanford.edu/ns/Canvas"));
  $rdf->addDescriptionNode($name, $children);

  $name = "$doc_path_url/AnnotationList/AnnotationList.xml";
  $children = array();
  $children[] = $rdf->buildChild('ore:describes', array('rdf:resource' => "$doc_url/AnnotationList"));
  $children[] = $rdf->buildChild('rdf:type', array('rdf:resource' => ""));
  $children[] = $rdf->buildChild('dc:format', NULL, 'application/rdf+xml');
  $children[] = $rdf->buildChild('dcterms:modified', NULL, "$timestamp");
  $rdf->addDescriptionNode($name, $children);

  return $rdf->printRdf();
}

/**
 * Retrieves height and width from the RELS-INT.
 *
 * Falls back to Djatoka if the dimensions are not stored
 *
 * @param string $pid
 *   The pid of the object.
 *
 * @return array
 *   Key value array containing the height and width with their values.
 */
function islandora_image_annotation_get_height_and_width($pid) {
  module_load_include('inc', 'islandora_image_annotation', 'includes/utils');
  $dsid = islandora_image_annotation_get_dsid_from_model($pid);
  $item = islandora_object_load($pid);
  $height_width_arr = NULL;
  if (isset($item['RELS-INT'])) {
    $content = $item['RELS-INT']->content;
    $sxml = simplexml_load_string($content);
    $sxml->registerXPathNamespace('islandora-page', "info:islandora/islandora-system:def/pageinfo#");
    $sxml->registerXPathNamespace('islandora-relsint', 'http://islandora.ca/ontology/relsint#');
    $sxml->registerXPathNamespace('rdf', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#');
    $sxml->registerXPathNamespace('fits', "http://hul.harvard.edu/ois/xml/ns/fits/fits_output");
    $image_height = $sxml->xpath('//rdf:Description[@rdf:about="info:fedora/' . $pid . '/' . $dsid . '"]/islandora-relsint:height');
    $image_width = $sxml->xpath('//rdf:Description[@rdf:about="info:fedora/' . $pid . '/' . $dsid . '"]/islandora-relsint:width');
    if (!$image_height || !$image_width) {
      // Page of a book might have a different namespace.
      $image_height = $sxml->xpath('//rdf:Description[@rdf:about="info:fedora/' . $pid . '/' . $dsid . '"]/islandora-page:height');
      $image_width = $sxml->xpath('//rdf:Description[@rdf:about="info:fedora/' . $pid . '/' . $dsid . '"]/islandora-page:width');
    }
    if (!$image_height || !$image_width) {
      // Have seen fits:imageHeight fits:imageWidth in the wild.
      $image_height = $sxml->xpath('//rdf:Description[@rdf:about="info:fedora/' . $pid . '/' . $dsid . '"]/fits:imageHeight');
      $image_width = $sxml->xpath('//rdf:Description[@rdf:about="info:fedora/' . $pid . '/' . $dsid . '"]/fits:imageWidth');
    }
    // We could have a RELS-INT without a height and width for our object.
    if (!$image_height || !$image_width) {
      $height_width_arr = islandora_image_annotation_get_image_dimensions_drupal($pid, $dsid, TRUE);
    }
    else {
      $image_height = (string) $image_height[0];
      $image_width = (string) $image_width[0];
      $height_width_arr = array(
        'height' => $image_height,
        'width' => $image_width,
      );
    }
  }
  else {
    // Fall back to Djatoka.
    return islandora_image_annotation_get_image_dimensions_drupal($pid, $dsid, TRUE);
  }
  return $height_width_arr;
}

/**
 * Retrieve image dimensions of an object from Djatoka.
 *
 * @param string $pid
 *   Pid of the object.
 *
 * @param string $dsid
 *   The DSID we are searching in.
 *
 * @param bool $update
 *   Whether we update the RELS-INT with the height and width.
 *
 * @return array
 *   Key value array containing the height, width, extension, mime and size.
 *
 * @throws Exception
 *   Target datastream does not exist.
 */
function islandora_image_annotation_get_image_dimensions_drupal($pid, $dsid, $update = FALSE) {

  if (empty($dsid)) {
    throw Exception('Could not find a datastream id to load for image height and width');
  }
  $object = islandora_object_load($pid);

  $image_file = file_save_data($object[$dsid]->content, 'temporary://');
  $real_path = drupal_realpath($image_file->uri);
  $info = image_get_info($real_path);
  if (empty($info)) {
    drupal_set_message(t('There was an error getting the image size.'), 'error');
    return NULL;
  }

  if ($update) {
    islandora_image_annotation_update_or_create_relsint($pid, $dsid, $info);
  }
  file_delete($image_file);
  return $info;
}

/**
 * Update the object's RELS-INT datastream to contains the height and width.
 *
 * @TODO Modify this function as it could be DESTRUCTIVE if something else is
 * being stored in RELS-INT.
 *
 * @param string $pid
 *   The pid of the object.
 *
 * @param string $dsid
 *   The DSID which this RELS-INT applies to.
 *
 * @param array $height_width_arr
 *   An array containing the height and width of the image.
 */
function islandora_image_annotation_update_or_create_relsint($pid, $dsid, $height_width_arr) {
  if (!isset($height_width_arr['width']) || !isset($height_width_arr['height'])) {
    watchdog('islandora', 'Error adding RELS-INT stream for object %pid. no height or width specified', array('pid%' => $pid));
  }
  $rels_int_str = <<<XML
    <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
  <rdf:Description rdf:about="info:fedora/XPID/XTIFF">
    <width xmlns="http://islandora.ca/ontology/relsint#">XWIDTH</width>
    <height xmlns="http://islandora.ca/ontology/relsint#">XHEIGHT</height>
  </rdf:Description>
</rdf:RDF>
XML;
  $item = islandora_object_load($pid);
  if (!isset($item[$dsid])) {
    // No datastream to create the RELS-INT on.
    return FALSE;
  }
  if (!isset($item['RELS-INT'])) {
    $to_replace = array('XPID', 'XWIDTH', 'XHEIGHT', 'XTIFF');
    $replace_with = array(
      $pid,
      $height_width_arr['width'],
      $height_width_arr['height'],
      $dsid,
    );
    $rels_int_str = str_replace($to_replace, $replace_with, $rels_int_str);
    try {
      $rels_int_ds = $item->constructDatastream('RELS-INT', 'X');
      $rels_int_ds->mimetype = 'text/xml';
      $rels_int_ds->label = 'RELS-INT';
      $rels_int_ds->content = $rels_int_str;
      // Create RELS-INT.
      $item->ingestDatastream($rels_int_ds);
    }
    catch (Exception $e) {
      watchdog('islandora', 'Error adding RELS-INT stream for object %pid', array('pid%' => $pid));
    }
  }
  else {
    // We assume entries do not exist as we have just tried to load this info.
    $rels_ds = $item['RELS-INT'];
    $doc = new DOMDocument();
    $doc->loadXML($rels_ds->content);
    $rdf = $doc->documentElement;
    $description = $doc->createElement('rdf:Description');
    $about = $doc->createAttribute('rdf:about');
    $about->value = "info:fedora/$pid/$dsid";
    $description->appendChild($about);
    $width = $doc->createElement('width', $height_width_arr['width']);
    $height = $doc->createElement('height', $height_width_arr['height']);
    $width->setAttribute('xmlns', "http://islandora.ca/ontology/relsint#");
    $height->setAttribute('xmlns', "http://islandora.ca/ontology/relsint#");
    $description->appendChild($width);
    $description->appendChild($height);
    $rdf->appendChild($description);
    $xml = $doc->saveXML();
    $item['RELS-INT']->content = $xml;
  }
}

/**
 * Echoes Fedora streams for javascript retrieval.
 *
 * @param string $pid
 *   The pid of the object.
 * @param string $stream
 *   Which stream to serve.
 */
function islandora_image_annotation_islandora_serve($pid, $stream) {
  global $base_url;
  module_load_include('inc', 'islandora_image_annotation', 'includes/utils');
  $dsid = islandora_image_annotation_get_dsid_from_model($pid);
  $doc_url = "$base_url/islandora/object/$pid/datastream/$dsid/view";
  $doc_path_url = "$base_url/islandora/anno/serve/$pid";

  drupal_add_http_header('Content-Type', 'application/rdf+xml');
  if ($stream == 'Manifest') {
    $manifest = islandora_image_annotation_get_manifest($doc_url, $doc_path_url);
    echo $manifest;
    return;
  }
  $dimensions = islandora_image_annotation_get_height_and_width($pid);

  $height = $dimensions['height'];
  $width = $dimensions['width'];

  if ($stream == 'NormalSequence') {
    $sequence = islandora_image_annotation_get_normal_sequence($doc_url, $doc_path_url, $pid, $height, $width);
    echo $sequence;
    return;
  }
  if ($stream == 'ImageAnnotations') {
    $image_annotations = islandora_image_annotation_get_image_annotations($doc_url, $doc_path_url, $height, $width, NULL);
    echo $image_annotations;
    return;
  }
}

/**
 * Creates a UUID.
 *
 * @return string
 *   A UUID.
 * @throws Exception
 */
function islandora_image_annotation_get_uuid() {
  $connection = islandora_get_tuque_connection();
  $configuration['id'] = $connection->repository->getNextIdentifier(NULL, TRUE);
  $data = explode(":", $configuration['id']);
  // Return just the uuid portion of the identifier.
  return $data[1];
}
