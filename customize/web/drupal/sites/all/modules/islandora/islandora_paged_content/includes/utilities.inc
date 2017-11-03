<?php

/**
 * @file
 * This file contains all the utility functions for paged content.
 * There are functions for:
 *   Getting pages for a paged content object
 *   Generating pdfs
 *   Generating image derivatives
 *   Generating OCR and HOCR
 */

/**
 * Gets all the pages in the given paged content object.
 *
 * @param AbstractObject $object
 *   The paged content object to fetch the pages from.
 *
 * @return array
 *   All the pages in the given paged content object. Ordered by sequence
 *   number. Each an array containing info.
 */
function islandora_paged_content_get_pages(AbstractObject $object) {
  $pages = array();
  // Try to use Solr first if enabled, or default to the RI if the Solr query
  // fails for some reason.
  if (module_exists('islandora_solr') && variable_get('islandora_paged_content_use_solr_for_dimensions', FALSE)) {
    $pages = islandora_paged_content_get_pages_solr($object);
    if (empty($pages)) {
      $pages = islandora_paged_content_get_pages_ri($object);
    }
  }
  else {
    $pages = islandora_paged_content_get_pages_ri($object);
  }
  // Sort the pages into their proper order.
  $sort = function($a, $b) {
    $a = (is_array($a) && isset($a['page'])) ? $a['page'] : 0;
    $b = (is_array($b) && isset($b['page'])) ? $b['page'] : 0;
    if ($a == $b) {
      return 0;
    }
    return ($a < $b) ? -1 : 1;
  };
  uasort($pages, $sort);
  return $pages;
}

/**
 * Helper function to retrieve the pages and dimensions from the RI.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a book.
 *
 * @return array
 *   All the pages in the given paged content object. Ordered by sequence
 *   number. Each an array containing info.
 */
function islandora_paged_content_get_pages_ri(AbstractObject $object) {
  $query = <<<EOQ
  PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
  SELECT ?pid ?page ?label ?width ?height
  FROM <#ri>
  WHERE {
    ?pid <fedora-rels-ext:isMemberOf> <info:fedora/{$object->id}> ;
         <fedora-model:label> ?label ;
         islandora-rels-ext:isSequenceNumber ?page ;
         <fedora-model:state> <fedora-model:Active> .
    OPTIONAL {
      ?pid <fedora-view:disseminates> ?dss .
      ?dss <fedora-view:disseminationType> <info:fedora/*/JP2> ;
           islandora-rels-ext:width ?width ;
           islandora-rels-ext:height ?height .
   }
  }
  ORDER BY ?page
EOQ;

  $results = $object->repository->ri->sparqlQuery($query);

  // Get rid of the "extra" info...
  $map = function($o) {
    foreach ($o as $key => &$info) {
      $info = $info['value'];
    }

    $o = array_filter($o);

    return $o;
  };
  $pages = array_map($map, $results);

  // Grab the PIDs...
  $get_pid = function($o) {
    return $o['pid'];
  };
  $pids = array_map($get_pid, $pages);

  // If we have some pages, combine our remapped results to produce an array
  // mapping pids to the values for that pid.
  $pages = count($pids) ? array_combine($pids, $pages) : array();
  return $pages;
}

/**
 * Helper function to retrieve the pages and dimensions from Solr.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a book.
 *
 * @return array
 *   All the pages in the given paged content object. Ordered by sequence
 *   number. Each an array containing info.
 */
function islandora_paged_content_get_pages_solr(AbstractObject $object) {
  $pages = array();
  $qp = new islandoraSolrQueryProcessor();
  $qp->buildQuery(format_string('@field:"@pid"', array(
      '@field' => variable_get('islandora_solr_member_of_field', 'RELS_EXT_isMemberOf_uri_ms'),
      '@pid' => "info:fedora/{$object->id}")
  ));
  $label_field = variable_get('islandora_solr_object_label_field', 'fgs_label_s');
  $sequence_field = variable_get('islandora_paged_content_sequence_number_field', 'RELS_EXT_isSequenceNumber_literal_ms');
  $height_field = variable_get('islandora_paged_content_solr_height_field', 'RELS_INT_height_literal_s');
  $width_field = variable_get('islandora_paged_content_solr_width_field', 'RELS_INT_width_literal_s');
  $qp->solrParams['fl'] = implode(', ', array(
    'PID',
    $label_field,
    $sequence_field,
    $height_field,
    $width_field,
  ));
  $qp->solrStart = 0;
  $qp->solrLimit = 100000;
  $qp->executeQuery(FALSE);
  if ($qp->islandoraSolrResult['response']['numFound'] > 0) {
    foreach ($qp->islandoraSolrResult['response']['objects'] as $page) {
      $pages[$page['PID']] = array(
        'pid' => $page['PID'],
        'page' => (string) reset($page['solr_doc'][$sequence_field]),
        'label' => $page['object_label'],
      );
      if (isset($page['solr_doc'][$width_field])) {
        $pages[$page['PID']]['width'] = $page['solr_doc'][$width_field];
      }
      if (isset($page['solr_doc'][$height_field])) {
        $pages[$page['PID']]['height'] = $page['solr_doc'][$height_field];
      }
    }
  }
  return $pages;
}

/**
 * Function get_page_progression.
 *
 * Gets the page progression to be used in a viewer from the given paged
 * content object.
 *
 * @param AbstractObject $object
 *   The paged content object to fetch the page progression from.
 *
 * @return string
 *   The page progression of the paged content object:  either left to right
 *   'lr' or right to left 'rl'.
 */
function islandora_paged_content_get_page_progression(AbstractObject $object) {
  $relationships = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'hasPageProgression');
  if (!empty($relationships)) {
    $progression = reset($relationships);
    return $progression['object']['value'];
  }
  else {
    return 'lr';
  }
}

/**
 * Sets page progression for an object.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @param string $progression
 *   The page progression for the object.
 */
function islandora_paged_content_set_page_progression(AbstractObject $object, $progression) {
  islandora_paged_content_unset_page_progression($object);
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasPageProgression', $progression, RELS_TYPE_PLAIN_LITERAL);
}

/**
 * Unsets page progression for an object.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 */
function islandora_paged_content_unset_page_progression(AbstractObject $object) {
  $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'hasPageProgression');
}

/**
 * Create/Update a datastream with the given parameters.
 *
 * @param AbstractObject $object
 *   The object to update.
 * @param string $file
 *   The absolute path to the file to use as content for the datastream.
 * @param string $dsid
 *   The datastream ID.
 * @param string $label
 *   The datastream label.
 * @param string $mimetype
 *   The datastream mimetype, if none given it will be guessed from the file
 *   extension.
 * @param string $control_group
 *   The datastream control group.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_paged_content_update_datastream(AbstractObject $object, $file, $dsid, $label = NULL, $mimetype = NULL, $control_group = 'M', $copy = TRUE) {
  if (!isset($mimetype)) {
    $mime_detector = new MimeDetect();
    $mimetype = $mime_detector->getMimetype($file);
  }
  $label = isset($label) ? $label : $dsid;

  if (!isset($object[$dsid])) {
    $ds = $object->constructDatastream($dsid, $control_group);
    $ds->mimetype = $mimetype;
    $ds->label = $label;
    $ds->setContentFromFile($file, $copy);
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object[$dsid];
    $ds->setContentFromFile($file, $copy);
    if ($ds->label != $label) {
      $ds->label = $label;
    }
    if ($ds->mimetype != $mimetype) {
      $ds->mimetype = $mimetype;
    }
  }
  return TRUE;
}

/**
 * Gets the given relationship if one is defined.
 *
 * Assumes only one relationship matches the given parameters.
 * Will return the given default if no relationship is found.
 *
 * @param object $relationships
 *   The Fedora relationships to check, either RELS-EXT or RELS-INT.
 * @param string $predicate_uri
 *   The predicate URI.
 * @param string $predicate
 *   The predicate.
 * @param mixed $default
 *   The default value to return if no relationship was found.
 *
 * @return mixed
 *   The first matching relationships value if found, otherwise the given
 *   default it returned.
 */
function islandora_paged_content_get_relationship($relationships, $predicate_uri, $predicate, $default = NULL) {
  $results = $relationships->get($predicate_uri, $predicate);
  return count($results) > 0 ? $results[0]['object']['value'] : $default;
}

/**
 * Sets a relationship removing any previous relationships.
 *
 * @param object $relationships
 *   The Fedora relationships to be modified, either RELS-EXT or RELS-INT.
 * @param string $predicate_uri
 *   The predicate URI.
 * @param string $predicate
 *   The predicate.
 * @param string $object
 *   The object.
 * @param bool $literal
 *   TRUE if the object is a literal, FALSE if it is an object.
 */
function islandora_paged_content_set_relationship($relationships, $predicate_uri, $predicate, $object, $literal = FALSE) {
  $relationships->remove($predicate_uri, $predicate, NULL, $literal);
  $relationships->add($predicate_uri, $predicate, $object, $literal);
}

/**
 * Gets only the enabled ingest derivatives.
 *
 * As defined by the user in the admin form.
 *
 * @param string $namespace
 *   The solution pack namespace whose admin form we're checking
 *
 * @return array
 *   The enabled ingest derivatives.
 */
function islandora_paged_content_get_enabled_ingest_derivatives($namespace) {
  $default_derivatives = drupal_map_assoc(array('pdf', 'image', 'ocr'));
  return variable_get("${namespace}_ingest_derivatives", $default_derivatives);
}

/**
 * Gets the datastream ID of the intended source for the given derivative.
 *
 * @param string $dsid
 *   The derived datastream ID.
 *
 * @return string
 *   The datastream ID of the source for the given derivative.
 */
function islandora_paged_content_get_page_derivative_source_id($dsid) {
  // @todo Change TIFF to OBJ, when done testing.
  $source_datastreams = array(
    'PDF' => 'OBJ',
    'OCR' => 'OBJ',
    'HOCR' => 'OBJ',
    'TN' => 'OBJ',
    'JPG' => 'OBJ',
    'JP2' => 'OBJ',
  );
  return $source_datastreams[$dsid];
}

/**
 * Checks if the given derivative datastream's intended source exists.
 *
 * @param AbstractObject $object
 *   The object to check.
 * @param string $dsid
 *   The derived datastream ID.
 *
 * @return bool
 *   TRUE if the source datastream exists, FALSE otherwise.
 */
function islandora_paged_content_page_has_derivative_source(AbstractObject $object, $dsid) {
  $source_dsid = islandora_paged_content_get_page_derivative_source_id($dsid);
  return isset($object[$source_dsid]);
}

/**
 * Gets the source file for the given, derivative datastream.
 *
 * @param AbstractObject $object
 *   The owner of the datastreams.
 * @param string $dsid
 *   The derived datastream ID.
 *
 * @return string
 *   The path to the source file.
 */
function islandora_paged_content_get_page_derivative_source(AbstractObject $object, $dsid) {
  if (!islandora_paged_content_can_derive($object, $dsid)) {
    return FALSE;
  }
  $source_dsid = islandora_paged_content_get_page_derivative_source_id($dsid);
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($object[$source_dsid]->mimeType);

  $base_name = "{$object->id}_{$source_dsid}.{$ext}";

  // Colons mess with some programs...
  $base_name = str_replace(':', '-', $base_name);

  $source_file = file_create_filename($base_name, 'temporary://');
  $object[$source_dsid]->getContent($source_file);
  return drupal_realpath($source_file);
}

/**
 * Checks if the given object can derive the given datastream.
 *
 * @param AbstractObject $object
 *   The object to check.
 * @param string $dsid
 *   The derived datastream ID.
 *
 * @return bool
 *   TRUE if the datastream can be derived, FALSE otherwise.
 */
function islandora_paged_content_can_derive(AbstractObject $object, $dsid) {
  $can_derive = FALSE;
  $to_derive = array(
    'islandora:newspaperCModel',
    'islandora:bookCModel',
    'islandora:pageCModel',
    'islandora:bookPageCModel',
    'islandora:newspaperPageCModel',
    'islandora:manuscriptCModel',
    'islandora:manuscriptPageCModel',
  );
  if (array_intersect($to_derive, $object->models)) {
    $check_function = array(
      'PDF' => 'islandora_paged_content_can_create_pdf',
      'OCR' => 'islandora_ocr_can_derive_ocr',
      'HOCR' => 'islandora_ocr_can_derive_ocr',
      'TN' => 'islandora_paged_content_can_create_images',
      'JP2' => 'islandora_paged_content_can_create_images',
      'JPG' => 'islandora_paged_content_can_create_images',
    );

    if (isset($check_function[$dsid])) {
      if ($dsid == "OCR" || $dsid == 'HOCR') {
        module_load_include('inc', 'islandora_ocr', 'includes/utilities');
      }

      $can_derive = function_exists($check_function[$dsid]) && $check_function[$dsid]();
    }

    $can_derive = $can_derive && islandora_paged_content_page_has_derivative_source($object, $dsid);
  }
  return $can_derive;
}

/**
 * Checks if it is possible to create image files.
 *
 * @return bool
 *   TRUE if it is possible, FALSE otherwise.
 */
function islandora_paged_content_can_create_images() {
  return module_exists('islandora_large_image');
}

/**
 * Checks if it is possible to create PDF files with imagemagick.
 *
 * @return bool
 *   TRUE if it is possible, FALSE otherwise.
 */
function islandora_paged_content_can_create_pdf() {
  // @todo Should also check if imagemagick is configured correctly.
  return module_exists('imagemagick');
}

/**
 * Checks if it is possible to combined PDF files with GhostScript.
 *
 * @return bool
 *   TRUE if it is possible, FALSE otherwise.
 */
function islandora_paged_content_can_combine_pdf() {
  $gs = variable_get('islandora_paged_content_gs', '/usr/bin/gs');
  return is_executable($gs);
}

/**
 * Appends a number of PDF files onto the given PDF file.
 *
 * @param array $file
 *   The PDF files to append onto.
 * @param array $files
 *   The PDF files to append.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_paged_content_pdf_append($file, array $files) {
  $temp_file = "$file.temp.pdf";
  copy($file, $temp_file);
  array_unshift($files, $temp_file);
  $ret = islandora_paged_content_pdf_combine($files, $file);
  file_unmanaged_delete($temp_file);
  return $ret;
}

/**
 * Combines the given PDF files into one output file.
 *
 * @param array $files
 *   The PDF files to be combined in order.
 * @param string $out
 *   The absolute path to the combined PDF file.
 *
 * @return bool
 *   TRUE on success, FALSE otherwise.
 */
function islandora_paged_content_pdf_combine(array $files, $out) {
  $gs = variable_get('islandora_paged_content_gs', '/usr/bin/gs');
  $files = implode(' ', $files);
  $command = "{$gs} -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile={$out} {$files}";
  $output = array(); $ret = 0;
  exec($command, $output, $ret);
  if ($ret != 0) {
    $variables = array(
      '@ret' => $ret,
      '@command' => $command,
      '!output' => implode('<br/>', $output),
    );
    watchdog('islandora_paged_content', 'GhostScript failed to combine PDFs.<br/>Error: @ret<br/>Command: @command <br/>Output !output', $variables, WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}

/**
 * Appends a series of OCR files to a consolidated OCR file.
 *
 * @param string $consolidated_ocr
 *   The consolidated OCR path.
 * @param array $files
 *   The files to be appended to the OCR.
 *
 * @return bool
 *   TRUE on success, FALSE on failure.
 */
function islandora_paged_content_ocr_append($consolidated_ocr, array $files) {
  $temp_file = drupal_tempnam("temporary://", "$consolidated_ocr.temp.txt");
  copy($consolidated_ocr, $temp_file);
  array_unshift($files, $temp_file);
  $ret = islandora_paged_content_ocr_combine($files, $consolidated_ocr);
  file_unmanaged_delete($temp_file);
  return $ret;
}

/**
 * Combines the given OCR files into one output file.
 *
 * @param array $files
 *   The OCR files to be combined, in order.
 * @param string $out
 *   The absolute path to the consolidated OCR file.
 *
 * @return bool
 *   TRUE on success, FALSE on fail.
 */
function islandora_paged_content_ocr_combine(array $files, $out) {
  $success = TRUE;
  foreach ($files as $file) {
    $ocr = file_get_contents($file);
    if ($ocr == FALSE) {
      $success = FALSE;
    }
    else {
      $success = file_put_contents($out, $ocr, FILE_APPEND) == FALSE ? FALSE : $success;
    }
  }
  return $success;
}

/**
 * Creates a PDF derivative for the given Page object.
 *
 * @param AbstractObject $object
 *   The page object that the derivative will be generated from, and added to.
 * @param array $options
 *   The options to be passed to convert.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_paged_content_page_derive_pdf_datastream(AbstractObject $object, array $options = array('-compress' => 'LZW')) {
  $pdf_file = islandora_paged_content_page_derive_pdf($object, $options);
  if (!$pdf_file) {
    return FALSE;
  }
  $ret = islandora_paged_content_update_datastream($object, $pdf_file, 'PDF');
  file_unmanaged_delete($pdf_file);
  return $ret;
}

/**
 * Creates a PDF derivative from the given Page object and convert options.
 *
 * @param AbstractObject $object
 *   The page object that the derivative will be generated from.
 * @param array $options
 *   The options to be passed to convert.
 *
 * @return string
 *   The absolute path to the derived file if successful, FALSE otherwise.
 */
function islandora_paged_content_page_derive_pdf(AbstractObject $object, array $options = array('-compress' => 'LZW')) {
  if (!islandora_paged_content_can_derive($object, 'PDF')) {
    return FALSE;
  }
  $source_file = islandora_paged_content_get_page_derivative_source($object, 'PDF');
  $pdf_file = islandora_paged_content_convert_to_pdf($source_file, $options);
  file_unmanaged_delete($source_file);
  return $pdf_file;
}

/**
 * Converts the given image to a PDF file, using the given options.
 *
 * @param string $image_file
 *   The absolute path to the image file.
 * @param array $options
 *   The options to be passed to convert.
 *
 * @return string
 *   The absolute path to the generated PDF if successful, FALSE otherwise.
 */
function islandora_paged_content_convert_to_pdf($image_file, array $options) {
  // Convert options into a string.
  $to_string = function(&$o, $k) {
    $o = "$k $o";
  };
  array_walk($options, $to_string);
  $options = implode(' ', $options);
  $pdf_file = "$image_file.pdf";
  $command_args = "{$options} {$image_file} {$pdf_file}";
  $output = array(); $ret = 0;
  if (_imagemagick_convert_exec($command_args, $output, $ret) !== TRUE) {
    $message = 'ImageMagick failed to create a PDF.<br/>Error: @ret<br/>Command: @command<br/>Output: !output';
    $variables = array(
      '@ret' => $ret,
      '@command' => "convert $command_args",
      '!output' => implode('<br/>', $output),
    );
    watchdog('islandora_paged_content', 'ImageMagick failed to create a PDF.<br/>Error: @ret<br/>Command: @command<br/>Output: !output', $variables, WATCHDOG_ERROR);
    return FALSE;
  }
  return $pdf_file;
}

/**
 * Creates and adds all the image derivatives for the given Page object.
 *
 * @param AbstractObject $object
 *   The page object that the derivative will be generated for.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_paged_content_page_derive_image_datastreams(AbstractObject $object) {
  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');
  $ret = islandora_large_image_create_all_derivatives($object);
  if ($ret) {
    // Add dimensions for the JP2.
    islandora_paged_content_add_dimensions_relationships($object);
  }
  return $ret;
}

/**
 * Add the dimensions to the RELS-INT for the JP2.
 *
 * This makes the dimensions accessible when we query for the pages.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @param bool $force
 *   Whether we are forcing the creation of dimensions or not.
 */
function islandora_paged_content_add_dimensions_relationships(AbstractObject $object, $force = FALSE) {
  $rels = $object['JP2']->relationships;
  $width_rels = $rels->get(ISLANDORA_RELS_EXT_URI, 'width');
  if ($force || empty($width_rels)) {
    // XXX: ... seems a little odd to grab them from the OBJ/tiff, but they're
    // more widely supported...
    $temp_file = drupal_tempnam('temporary://', 'dimensions');
    $object['OBJ']->getContent($temp_file);
    list($width, $height) = getimagesize($temp_file);

    islandora_paged_content_set_relationship($rels, ISLANDORA_RELS_EXT_URI,
        'width', $width, 1
      );
    islandora_paged_content_set_relationship($rels, ISLANDORA_RELS_EXT_URI,
        'height', $height, 1
      );

    drupal_unlink($temp_file);
  }
}

/**
 * Checks if it is possible to update a paged content's thumbnail image.
 */
function islandora_paged_content_can_update_paged_content_thumbnail(AbstractObject $object) {
  $pages = islandora_paged_content_get_pages($object);
  if (count($pages)) {
    $page = array_shift($pages);
    $page = islandora_object_load($page['pid']);
    return isset($page['TN']);
  }
  return FALSE;
}

/**
 * Updates the thumbnail from the first page in the paged content.
 *
 * @param AbstractObject $object
 *   The paged content object to update.
 *
 * @return bool
 *   TRUE on success, FALSE otherwise.
 */
function islandora_paged_content_update_paged_content_thumbnail(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!islandora_paged_content_can_update_paged_content_thumbnail($object)) {
    return FALSE;
  }
  $pages = islandora_paged_content_get_pages($object);
  $page = array_shift($pages);
  $page = islandora_object_load($page['pid']);
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($page['TN']->mimeType);
  // Windows will likely store temp data in a temp directory
  // rather than in memory. Since the colon is forbidden in
  // filenames, replace it with an underscore instead.
  $thumbnail_id = ((islandora_deployed_on_windows()) ? str_replace(':', '_', $page->id) : $page->id);
  $file = drupal_realpath("temporary://{$thumbnail_id}_TN.{$ext}");
  $page['TN']->getContent($file);
  $ret = islandora_paged_content_update_datastream($object,
    $file,
    'TN',
    'Thumbnail');
  file_unmanaged_delete($file);
  return $ret;
}

/**
 * Creates OCR and HOCR derivatives and ingests them.
 *
 * @param AbstractObject $object
 *   The page object that the derivatives will be generated for.
 * @param array $options
 *   The options for tesseract/gimp.
 *
 * @return bool
 *   TRUE on success or FALSE on failure.
 */
function islandora_paged_content_page_derive_ocr_datastreams(AbstractObject $object, array $options = NULL) {
  $options = islandora_paged_content_apply_language_relationship_to_options($object, $options);

  module_load_include('inc', 'islandora_ocr', 'includes/derivatives');
  $ret = islandora_ocr_derive_datastreams($object['OBJ'], $options);

  islandora_paged_content_set_relationship($object->relationships,
    ISLANDORA_RELS_EXT_URI,
    'hasLanguage',
    $options['language'],
    TRUE);
  return $ret;
}

/**
 * Creates OCR and HOCR derivatives and returns references to them.
 *
 * @param AbstractObject $object
 *   The page object that the derivatives will be generated for.
 * @param array $options
 *   The options for tesseract/gimp.
 *
 * @return array
 *   An associative array containing 'OCR' and 'HOCR', each of which either
 *   contain the path to the generated output from tesseract, or FALSE on
 *   failure.
 */
function islandora_paged_content_page_generate_ocr_datastreams(AbstractObject $object, array $options = NULL) {
  $options = islandora_paged_content_apply_language_relationship_to_options($object, $options);

  module_load_include('inc', 'islandora_ocr', 'includes/derivatives');
  $ret = islandora_ocr_generate_derivatives($object['OBJ'], $options);

  islandora_paged_content_set_relationship($object->relationships,
    ISLANDORA_RELS_EXT_URI,
    'hasLanguage',
    $options['language'],
    TRUE);
  return $ret;
}

/**
 * Helper function to create generation options from a language relationship.
 *
 * @param AbstractObject $object
 *   The object to check relationships for.
 * @param array|null $options
 *   The current tesseract/gimp generation options.
 *
 * @return array
 *   An array that at least contains a 'language' option matching the object's
 *   hasLanguage relationship, or 'eng' if there wasn't one.
 */
function islandora_paged_content_apply_language_relationship_to_options(AbstractObject $object, array $options = NULL) {
  $options = (array) $options;
  if (!isset($options['language'])) {
    $options['language'] = islandora_paged_content_get_relationship($object->relationships,
      ISLANDORA_RELS_EXT_URI,
      'hasLanguage',
      'eng');
  }
  return $options;
}

/**
 * Fetches the given page's dimensions.
 *
 * Firsts attempts to get it from the FITS metadata datastream, if that fails
 * it will use Djatoka.
 *
 * @param AbstractObject $object
 *   The page object.
 *
 * @return array
 *   An associative array containing the following fields.
 *   - width: The width of the image in pixels.
 *   - height: The width of the image in pixels.
 *   If this function should fail for some reason all the fields will be 0.
 */
function islandora_paged_content_get_page_dimensions(AbstractObject $object) {
  $dimensions = array('width' => 0, 'height' => 0);
  $metadata = islandora_paged_content_get_technical_metadata($object);
  $metadata = $metadata ? $metadata : islandora_paged_content_get_page_metadata_from_djatoka($object->id);
  return $metadata ? array_intersect_key($metadata, $dimensions) : $dimensions;
}

/**
 * Fetches the given page's technical metadata.
 *
 * The technical metadata is pull from the techincal metadata datastream created
 * by islandora_fits.
 *
 * @param AbstractObject $object
 *   The page object.
 *
 * @return array
 *   An associative array containing the following fields.
 *   - width: The width of the image in pixels.
 *   - height: The width of the image in pixels.
 *   If this function fails FALSE is returned.
 */
function islandora_paged_content_get_technical_metadata(AbstractObject $object) {
  $dsid = variable_get('islandora_fits_techmd_dsid', 'TECHMD');
  if (empty($object[$dsid])) {
    return FALSE;
  }
  $metadata = FALSE;
  $file = file_create_filename("{$object->id}_{$dsid}.xml", 'temporary://');
  $file = drupal_realpath($file);
  if (strpos($object[$dsid]->mimetype, 'xml') !== FALSE && $object[$dsid]->getContent($file)) {
    $doc = simplexml_load_file($file);
    $doc->registerXPathNamespace('ns', 'http://hul.harvard.edu/ois/xml/ns/fits/fits_output');
    $width_xpath = $doc->xpath('/ns:fits/ns:metadata/ns:image/ns:imageWidth');
    $width = (int) array_pop($width_xpath);
    $height_xpath = $doc->xpath('/ns:fits/ns:metadata/ns:image/ns:imageHeight');
    $height = (int) array_pop($height_xpath);
    $metadata = array('width' => $width, 'height' => $height);
  }
  file_unmanaged_delete($file);
  return $metadata;
}

/**
 * Fetches the given page's metadata via Djatoka.
 *
 * @param string $object_id
 *   The PID of the page to fetch the metadata from.
 *
 * @return array
 *   An associative array contatining the following string fields:
 *   - identifier: The URL to the resource.
 *   - imagefile: The path to the temp file being served.
 *   - width: The width of the image in pixels.
 *   - height: The width of the image in pixels.
 *   - dwtLevels: ???
 *   - levels: ???
 *   - compositingLayerCount: ???
 *   If the request to Djatoka fails then FALSE is returned.
 */
function islandora_paged_content_get_page_metadata_from_djatoka($object_id) {
  module_load_include('inc', 'islandora', 'includes/authtokens');
  $datastream_url = url("islandora/object/{$object_id}/datastream/JP2/view", array(
    'absolute' => TRUE,
    'query' => array(
      'token' => islandora_get_object_token($object_id, 'JP2'),
    ),
  ));

  $djatoka_url = variable_get('islandora_paged_content_djatoka_url', 'http://localhost:8080/adore-djatoka');
  $djatoka_url .= (substr($djatoka_url, -1) == '/') ? '' : '/';
  $djatoka_url = url($djatoka_url . "resolver", array(
    'query' => array(
      'url_ver' => 'Z39.88-2004',
      'rft_id' => $datastream_url,
      'svc_id' => 'info:lanl-repo/svc/getMetadata',
    ),
    'external' => TRUE,
  ));
  $request = drupal_http_request($djatoka_url);
  if ($request->code == '200') {
    return drupal_json_decode($request->data);
  }
  return FALSE;
}

/**
 * Returns paged object derivatives.
 *
 * @param string $context
 *   Indicator of which solution pack calling this function
 *
 * @return array
 *   Structured array of paged object derivatives
 */
function islandora_paged_content_paged_object_derivatives($context) {
  $derive = islandora_paged_content_get_enabled_ingest_derivatives($context);
  $paged_content_module_path = drupal_get_path('module', 'islandora_paged_content');
  $derivatives = array();

  if ($derive['image']) {
    $derivatives[] = array(
      'source_dsid' => NULL,
      'destination_dsid' => 'TN',
      'function' => array('islandora_paged_content_derive_thumbnail_of_parent'),
      'file' => "$paged_content_module_path/includes/derivatives.inc",
    );
  }
  if ($derive['pdf']) {
    $derivatives = array_merge(
      $derivatives,
      array(
        array(
          'source_dsid' => NULL,
          'destination_dsid' => 'PDF',
          'function' => array('islandora_paged_content_aggregate_pdf_derivative'),
          'file' => "$paged_content_module_path/includes/derivatives.inc",
        ),
      )
    );
  }
  if ($derive['ocr']) {
    $derivatives = array_merge(
      $derivatives,
      array(
        array(
          'source_dsid' => NULL,
          'destination_dsid' => 'OCR',
          'function' => array('islandora_paged_content_aggregate_ocr_derivative'),
          'file' => "$paged_content_module_path/includes/derivatives.inc",
        ),
      )
    );
  }
  return $derivatives;
}

/**
 * Returns page derivatives.
 *
 * @param string $context
 *   Indicator of which solution pack calling this function
 *
 * @return array
 *   Structured array of page derivatives
 */
function islandora_paged_content_page_derivatives($context) {
  $large_image_module_path = drupal_get_path('module', 'islandora_large_image');
  $paged_content_module_path = drupal_get_path('module', 'islandora_paged_content');
  $ocr_module_path = drupal_get_path('module', 'islandora_ocr');
  $derive = islandora_paged_content_get_enabled_ingest_derivatives($context);
  $derivatives = array();

  if (isset($derive['pdf']) && $derive['pdf']) {
    $derivatives[] = array(
      'source_dsid' => 'OBJ',
      'destination_dsid' => 'PDF',
      'function' => array('islandora_paged_content_derivatives_pdf'),
      'file' => "$paged_content_module_path/includes/derivatives.inc",
    );
  }

  if (isset($derive['ocr']) && $derive['ocr']) {
    $derivatives[] = array(
      'source_dsid' => 'OBJ',
      'destination_dsid' => 'OCR',
      'weight' => 0,
      'function' => array('islandora_ocr_derive_ocr'),
      'file' => "$ocr_module_path/includes/derivatives.inc",
    );
    $derivatives[] = array(
      'source_dsid' => 'OBJ',
      'destination_dsid' => 'HOCR',
      'weight' => 0,
      'function' => array('islandora_ocr_derive_hocr'),
      'file' => "$ocr_module_path/includes/derivatives.inc",
    );
  }
  if (isset($derive['image']) && $derive['image']) {
    $derivatives[] = array(
      'source_dsid' => 'OBJ',
      'destination_dsid' => 'JP2',
      'function' => array('islandora_large_image_create_jp2_derivative'),
      'file' => "$large_image_module_path/includes/derivatives.inc",
    );
    $derivatives[] = array(
      'source_dsid' => 'JP2',
      'destination_dsid' => NULL,
      'function' => array('islandora_paged_content_add_dimensions_relationships'),
      'file' => "$paged_content_module_path/includes/utilities.inc",
    );
    $derivatives[] = array(
      'source_dsid' => 'TN',
      'destination_dsid' => NULL,
      'function' => array('islandora_paged_content_derive_thumbnail_on_parent'),
      'file' => "$paged_content_module_path/includes/derivatives.inc",
    );
    $derivatives[] = array(
      'source_dsid' => 'OBJ',
      'destination_dsid' => 'TN',
      'function' => array('islandora_large_image_create_tn_derivative'),
      'file' => "$large_image_module_path/includes/derivatives.inc",
    );
    $derivatives[] = array(
      'source_dsid' => 'OBJ',
      'destination_dsid' => 'JPG',
      'function' => array('islandora_large_image_create_jpg_derivative'),
      'file' => "$large_image_module_path/includes/derivatives.inc",
    );
  }

  return $derivatives;
}

/**
 * Helper function for parsing the pages_to_move string.
 *
 * @param string $pages_to_move
 *   A string of the form '1, 2-4,5'.
 *
 * @return array
 *   The page numbers that are to be moved.
 */
function islandora_paged_content_parse_pages_to_move($pages_to_move) {
  $ranges = explode(',', $pages_to_move);
  $page_numbers_to_move = array();
  foreach ($ranges as $range) {
    $dash_index = strpos($range, '-');
    if ($dash_index) {
      $page_range_start = trim(substr($range, 0, $dash_index));
      $page_range_end = trim(substr($range, $dash_index + 1, strlen($range) - 1));
      $current_range = range($page_range_start, $page_range_end);
      $page_numbers_to_move = array_merge($page_numbers_to_move, $current_range);
    }
    else {
      $page_numbers_to_move[] = trim($range);
    }
  }

  return $page_numbers_to_move;
}

/**
 * Helper function to determine whether or not pdftotext is available.
 *
 * @return bool
 *   TRUE if it's available, FALSE if not.
 */
function islandora_paged_content_pdftotext_availability() {
  $pdftotext = variable_get('islandora_paged_content_pdftotext', '/usr/bin/pdftotext');
  return is_executable($pdftotext);
}


/**
 * Helper function to determine whether or not pdfinfo is available.
 *
 * @return bool
 *   TRUE if it's available, FALSE if not.
 */
function islandora_paged_content_pdfinfo_availability() {
  $pdfinfo = variable_get('islandora_paged_content_pdfinfo', '/usr/bin/pdfinfo');
  return is_executable($pdfinfo);
}

/**
 * Helper function to extract the text for a single page beginning at an offset.
 *
 * @param string $uri
 *   The URI or path to the file that is being examined.
 * @param string $offset
 *   The page number to extract it will be used as the first and last page
 *   value. Note that pdftotext won't complain about using an offset greater
 *   than the number of allotted pages of a PDF.
 * @param bool $layout
 *   Whether we are retaining layout of the PDF or not.
 * @param bool $raw_text
 *   Whether to extract the raw text from the PDF or not.
 *
 * @return bool|string
 *   FALSE if it fails, the URI of the text file otherwise.
 */
function islandora_paged_content_extract_text_from_pdf($uri, $offset, $layout = TRUE, $raw_text = FALSE) {
  $file_path = drupal_realpath($uri);
  // PDF doesn't exist or pdftotext isn't available, abort.
  if (!file_exists($file_path) || !islandora_paged_content_pdftotext_availability()) {
    return FALSE;
  }
  $pdftotext = variable_get('islandora_paged_content_pdftotext', '/usr/bin/pdftotext');
  $base_command = '!pdftotext_path !raw !layout -f !offset -l !offset !pdf_uri !output_file';
  $output_file = drupal_tempnam('temporary://', 'full');

  // Need to link the file because of UTF-8 encoding.
  $copy = islandora_paged_content_create_clean_link($uri, 'full');
  $command = format_string($base_command, array(
    '!pdftotext_path' => $pdftotext,
    '!layout' => $layout ? '-layout' : '',
    '!offset' => $offset,
    '!pdf_uri' => escapeshellarg(drupal_realpath($copy)),
    '!output_file' => drupal_realpath($output_file),
    '!raw' => $raw_text ? '-raw' : '',
  ));
  $output = array();
  $ret = 0;
  exec(escapeshellcmd($command), $output, $ret);
  drupal_unlink($copy);
  // UNIX exit code is 0, so this succeeded.
  if (!$ret) {
    return $output_file;
  }
  return FALSE;
}

/**
 * Helper function to parse the number of pages in a supplied PDF.
 *
 * @param string $uri
 *   The URI or path to the file that is being examined.
 *
 * @return bool|int
 *   FALSE if it fails, the number of pages in the PDF.
 */
function islandora_paged_content_length_of_pdf($uri) {
  $file_path = drupal_realpath($uri);
  // PDF doesn't exist or pdfinfo isn't available, abort.
  if (!file_exists($file_path) || !islandora_paged_content_pdfinfo_availability()) {
    return FALSE;
  }
  $pdfinfo = variable_get('islandora_paged_content_pdfinfo', '/usr/bin/pdfinfo');
  // Need to link the file because of UTF-8 encoding.
  $copy = islandora_paged_content_create_clean_link($uri, 'length');
  $command = escapeshellarg($pdfinfo) . ' ' . escapeshellarg(drupal_realpath($copy));
  $output = array();
  $ret = 0;
  exec(escapeshellcmd($command), $output, $ret);
  drupal_unlink($copy);
  // UNIX exit code is 0, so this succeeded.
  if (!$ret) {
    foreach ($output as $line) {
      if (strpos($line, 'Pages:') === 0) {
        return intval(str_replace('Pages:', '', $line));
      }
    }
  }
  return FALSE;
}

/**
 * Helper function to extract a page of a PDF into a TIFF.
 *
 * @param string $uri
 *   The URI or path to the file that is being examined.
 * @param string $offset
 *   The page number to extract it will be used as the first and last page
 *   value.
 * @param string $device
 *   Selects an alternate initial output device, as described above.
 * @param string $resolution
 *   The DPI of the resolution to be used.
 *
 * @return bool|string
 *   FALSE if it fails, the URI of the TIFF otherwise.
 */
function islandora_paged_content_extract_tiff_from_pdf($uri, $offset, $device, $resolution) {
  $file_path = drupal_realpath($uri);
  // PDF doesn't exist or gs isn't available, abort.
  if (!file_exists($file_path) || !islandora_paged_content_can_combine_pdf()) {
    return FALSE;
  }
  $gs = variable_get('islandora_paged_content_gs', '/usr/bin/gs');
  $output_file = drupal_tempnam('temporary://', "page{$offset}.tif");
  // Need to link the file because of UTF-8 encoding.
  $copy = islandora_paged_content_create_clean_link($uri, 'pdfforgs');
  // For all arguments see https://ghostscript.com/doc/current/Devices.htm#TIFF.
  if ($device == 'tiff64nc' || $device == 'tiff48nc') {
    // Both are 16bits per channel. No compression possible.
    // GhostScript versions 9.10 through 9.18 seem affected by a Bug
    // making this devices fail always.
    // @see http://git.ghostscript.com/?p=ghostpdl.git;a=commitdiff;h=f8e77523.
    $base_command = '!gs_path -q -dNOPAUSE -dBATCH -sDEVICE=!device -sCompression=none -sOutputFile=!output_file -r!resolution -dFirstPage=!offset -dLastPage=!offset !pdf_uri';
  }
  else {
    $base_command = '!gs_path -q -dNOPAUSE -dBATCH -sDEVICE=!device -sCompression=lzw -sOutputFile=!output_file -r!resolution -dFirstPage=!offset -dLastPage=!offset !pdf_uri';
  }
  $command = format_string($base_command, array(
    '!gs_path' => escapeshellarg($gs),
    '!device' => $device,
    '!offset' => $offset,
    '!output_file' => drupal_realpath($output_file),
    '!resolution' => (int) $resolution,
    '!pdf_uri' => escapeshellarg(drupal_realpath($copy)),
  ));
  $output = array();
  $ret = 0;
  exec(escapeshellcmd($command), $output, $ret);
  drupal_unlink($copy);
  if (!$ret) {
    return $output_file;
  }
  return FALSE;
}

/**
 * Helper function to split a PDF into an individual page.
 *
 * @param string $uri
 *   The URI or path to the file that is being examined.
 * @param string $offset
 *   The page number to extract it will be used as the first and last page
 *   value.
 *
 * @return bool|string
 *   FALSE if it fails, the URI of the PDF otherwise.
 */
function islandora_paged_content_chop_pdf($uri, $offset) {
  $file_path = drupal_realpath($uri);
  // PDF doesn't exist or gs isn't available, abort.
  if (!file_exists($file_path) || !islandora_paged_content_can_combine_pdf()) {
    return FALSE;
  }
  $gs = variable_get('islandora_paged_content_gs', '/usr/bin/gs');
  $output_file = drupal_tempnam('temporary://', "page{$offset}.pdf");
  // Need to link the file because of UTF-8 encoding.
  $copy = islandora_paged_content_create_clean_link($uri, 'pdfforgschop');
  $base_command = '!gs_path -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=!output_file -dFirstPage=!offset -dLastPage=!offset !pdf_uri';
  $command = format_string($base_command, array(
    '!gs_path' => escapeshellarg($gs),
    '!offset' => $offset,
    '!output_file' => drupal_realpath($output_file),
    '!pdf_uri' => escapeshellarg(drupal_realpath($copy)),
  ));
  $output = array();
  $ret = 0;
  exec(escapeshellcmd($command), $output, $ret);
  drupal_unlink($copy);
  if (!$ret) {
    return $output_file;
  }
  return FALSE;
}

/**
 * Helper to determine whether all requirements are met to extract from PDF.
 *
 * @return bool
 *   TRUE if all requirements are present, FALSE otherwise.
 */
function islandora_paged_content_can_extract_from_pdf() {
  return islandora_paged_content_can_combine_pdf() && islandora_paged_content_pdftotext_availability() && islandora_paged_content_pdfinfo_availability();
}

/**
 * Helper function to add the PDF datastream to an object.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @param object $file_obj
 *   The PDF file object that is being added.
 * @param bool $batch
 *   Whether we are adding this PDF in the batch context or not.
 */
function islandora_paged_content_add_pdf_to_object(AbstractObject $object, $file_obj, $batch = FALSE) {
  module_load_include('inc', 'islandora_paged_content', 'includes/db');
  $ingest = !isset($object['PDF']);
  if ($ingest) {
    $pdf = $object->constructDatastream('PDF', 'M');
  }
  else {
    $pdf = $object['PDF'];
  }
  $pdf->setContentFromFile(drupal_realpath($file_obj->uri), FALSE);
  if ($pdf->label != $file_obj->filename) {
    $pdf->label = $file_obj->filename;
  }
  if ($pdf->mimetype != $file_obj->filemime) {
    $pdf->mimetype = $file_obj->filemime;
  }
  if ($ingest) {
    $object->ingestDatastream($pdf);
  }
  if (!$batch) {
    islandora_paged_content_track_pdf_ingestion($object->id, $file_obj->fid);
  }
}

/**
 * Helper function to add all relationships to a paged content child.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @param array $params
 *   An array of parameters to be added as relationships containing:
 *   -model: A string representing the content model of the child.
 *   -parent: A string representing the PID of the parent.
 *   -page_number: A string representing the page number of the child.
 *   -language: A string representing the language of the text.
 */
function islandora_paged_content_add_relationships_to_child(AbstractObject $object, $params) {
  $rels_ext = $object->relationships;
  $object->models = $params['model'];
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageOf', $params['parent']);
  islandora_paged_content_set_relationship($rels_ext, FEDORA_RELS_EXT_URI, 'isMemberOf', $params['parent']);

  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', $params['page_number'], TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', $params['page_number'], TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
  if ($params['extract_text'] == 'none') {
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'generate_ocr', 'FALSE', RELS_TYPE_PLAIN_LITERAL);
  }
  else {
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'hasLanguage', $params['language'], TRUE);
  }
}

/**
 * Retrieves all applicable content models for paged content.
 */
function islandora_paged_content_retrieve_applicable_cmodels() {
  return module_invoke_all('islandora_paged_content_content_model_registry');
}

/**
 * Helper to create a filesystem link to the file at the given URI.
 *
 * Some utilities do not like working with UTF-8 paths, so we create a link
 * without any special characters to satisfy them. This may be associated with
 * the environment PHP uses to start subprocesses via exec() and the like.
 *
 * @param string $uri
 *   A URI which resolves to a locally-mounted filesystem.
 * @param string $prefix
 *   The prefix with which the link will be created, as per
 *   drupal_tempnam().
 * @param string $dest
 *   The destination in which to create the link.
 *
 * @return string|bool
 *   When successful, a string containing the path to either the new link or
 *   copy of the file if a link could not be created; otherwise, boolean FALSE.
 */
function islandora_paged_content_create_clean_link($uri, $prefix = 'clean', $dest = 'temporary://') {
  $path = drupal_realpath($uri);
  $attempts = 3;

  do {
    $clean = drupal_tempnam($dest, $prefix);
    // XXX: drupal_tempnam() creates the (empty) file, but we need to overwrite
    // it with a link, so drupal_unlink it out of place.
    drupal_unlink($clean);
    // XXX: Ideally, we could create symlinks to safely cross filesystem
    // boundaries, but it seems like there are some checks which look for files
    // explicitly (do not allow for symlinks).
    $created = @link($path, drupal_realpath($clean));
  } while (--$attempts > 0 && !$created);

  if ($created) {
    return $clean;
  }
  else {
    watchdog('islandora_paged_content', 'Unable to create link in @dest to "@uri"; copying "@uri" to @dest.', array(
      '@dest' => $dest,
      '@uri' => $uri,
    ));
    return file_unmanaged_copy($uri, $dest);
  }
}
