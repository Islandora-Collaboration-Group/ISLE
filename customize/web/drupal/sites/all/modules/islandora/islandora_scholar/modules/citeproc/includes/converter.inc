<?php

/**
 * @file
 * Convert mods data into JSON for citeproc-js.
 */

/**
 * Converts the MODS to a JSON object that represents a Citataion.
 *
 * @param string $item
 *   A MODS Document.
 * @param mixed $key
 *   Key within the array. The ID of the citation object to create.
 *
 * @return string
 *   A JSON encoded string, that represents the Citataion.
 */
function convert_mods_to_citeproc_jsons_escape(&$item, $key) {
  if (is_string($item)) {
    $item = htmlentities($item, ENT_QUOTES, 'UTF-8');
  }
}

/**
 * Add the MODS namespace to the given element, if it isn't already there.
 */
function add_mods_namespace(SimpleXMLElement &$mods) {
  static $used_namespace = NULL;
  static $mods_namespace = 'http://www.loc.gov/mods/v3';

  $namespaces = $mods->getNamespaces();

  if (is_null($used_namespace)) {
    // The namespace is there; possibly default, though.
    if (array_search($mods_namespace, $namespaces) !== FALSE) {
      $used_namespace = $mods_namespace;
    }
    else {
      $used_namespace = '';
    }
  }

  if (array_key_exists('mods', $namespaces) === FALSE) {
    $mods->registerXPathNamespace('mods', $used_namespace);
  }
}

/**
 * Coerce data into the proper JSON-like format, recursively.
 *
 * Was required to use the output of convert_mods_to_citeproc_jsons() in
 * CiteProc-PHP.
 *
 * @param mixed $in
 *   Array to be converted to an object.
 *
 * @return object
 *   If $in is not an array, return $in. If $in is an associative array,
 *   transform it into an object (keys become properties), and recursively
 *   process values. If $in is a numerically indexed array, just recursively
 *   process children.
 */
function _citeproc_array_to_object($in) {

  if (!is_array($in)) {
    return $in;
  }
  elseif (count($in) > 0 && count(array_filter(array_keys($in), 'is_int')) == count($in)) {
    foreach ($in as &$value) {
      $value = _citeproc_array_to_object($value);
    }
    return $in;
  }
  elseif (count($in) > 0 && count(array_filter(array_keys($in), 'is_string')) == count($in)) {
    $obj = new stdClass();
    foreach ($in as $key => $val) {
      if ($val) {
        $obj->$key = _citeproc_array_to_object($val);
      }
    }
    return $obj;
  }
  else {
    return FALSE;
  }
}

/**
 * Perform a number of XPath queries to extract CSL info.
 *
 * Despite name, actually returns a PHP array. Run results through
 * _citeproc_array_to_object() before passing off to CiteProc-PHP.
 *
 * @param mixed $mods_in
 *   Either a DOMNode or a string representing a MODS record.
 *
 * @return array
 *   An associative array mapping CSL variable names to values we have
 *   extracted for them from the provided MODS record.
 */
function convert_mods_to_citeproc_jsons($mods_in) {
  $mods = NULL;
  if ($mods_in instanceof DOMNode) {
    $mods = simplexml_import_dom($mods_in);
  }
  else {
    try {
      $mods = simplexml_load_string($mods_in);
    }
    catch (Exception $e) {
      watchdog('citeproc', 'Got exception while parsing.  Message: !msg Errors: !error', array(
            '!msg' => $e->getMessage(),
            '!error' => libxml_get_errors()));
      return array();
    }
  }
  if ($mods instanceof SimpleXMLElement) {
    add_mods_namespace($mods);
    // Merge with main object.
    $names = convert_mods_to_citeproc_json_names($mods);
    $dates = convert_mods_to_citeproc_json_dates($mods);
    $type = convert_mods_to_citeproc_json_type($mods);
    $parented = "'$type'='book' or '$type'='chapter' or '$type'='article-journal'";
    $output = array_merge(array(
      'title' => convert_mods_to_citeproc_json_title($mods),
      'abstract' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:abstract'),
      'call-number' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:classification'),
      'collection-title' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:relatedItem[@type="series"]/mods:titleInfo[not(@type)]/mods:title'),
      'container-title' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:relatedItem[@type="host"]/mods:titleInfo[not(@type)]/mods:title'),
      'DOI' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:identifier[@type="doi"]'),
      'edition' => preg_replace('/[^0-9]/', '', convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:originInfo/mods:edition')),
      'event' => convert_mods_to_citeproc_json_event($mods),
      'event-place' => convert_mods_to_citeproc_json_event_place($mods),
      // 'genre' => convert_mods_to_citeproc_json_genre($mods),
      'ISBN' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:identifier[@type="isbn"]'),
      'volume' => (int) convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:part/mods:detail[@type="volume"]/mods:number'),
      'issue' => (int) convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:part/mods:detail[@type="issue"]/mods:number'),
      'note' => convert_mods_to_citeproc_json_note($mods),
      'number' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:relatedItem[@type="series"]/mods:titleInfo/mods:partNumber'),
      'page' => convert_mods_to_citeproc_json_page($mods),
      'publisher' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:originInfo/mods:publisher'),
      'publisher-place' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:originInfo/mods:place/mods:placeTerm'),
      'URL' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:location/mods:url'),
      'number-pmid' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:identifier[@type="accession"]'),
      'number-pmcid' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:identifier[@type="pmc"]'),
      'number-nihmsid' => convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:identifier[@type="mid"]'),
      'type' => convert_mods_to_citeproc_json_genre($mods),
    ),
    $names,
    $dates
    );
    if ($output['type'] == 'book') {
      unset($output['container-title']);
      unset($output['page']);
    }

    drupal_alter('convert_mods_to_citeproc_jsons', $output, $mods);

    return $output;
  }
  else {
    watchdog('citeproc', 'Not a SimpleXMLElement!');
    return array();
  }
}

/**
 * Convert the given MODS genre to JSON repersentation.
 *
 * @param mixed $mods
 *   A MODS document.
 *
 * @return string
 *   The JSON repersentation of the MODS genre.
 */
function convert_mods_to_citeproc_json_genre($mods) {
  $genre = convert_mods_to_citeproc_json_query($mods, '//mods:mods[1]/mods:genre');
  switch (trim($genre)) {
    case 'journal article':
      return 'article-journal';

    case 'book chapter':
    case 'book section':
      return 'chapter';

    case 'book':
      return 'book';

    default:
      return 'article-journal';
  }
}

/**
 * Gets the title property for the Citation.
 *
 * There may be multiple titles, and relying on the title[@type] is not a wholly
 * reliable method of determining the best title.
 *
 * MOST OFTEN THERE WILL ONLY BE ONE.
 * My answer is to take the *longest*.
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 *
 * @return string
 *   The title property for the Citation.
 */
function convert_mods_to_citeproc_json_title(SimpleXMLElement $mods) {
  $output = '';
  add_mods_namespace($mods);
  $titles = $mods->xpath("//mods:mods[1]/mods:titleInfo/mods:title");

  if (!empty($titles)) {
    while (list($num, $node) = each($titles)) {
      add_mods_namespace($node);
      $title = (string) $node;
      $subtitle = convert_mods_to_citeproc_json_query($node, '../mods:subTitle');
      $non_sort = convert_mods_to_citeproc_json_query($node, '../mods:nonSort');
      $title = !empty($subtitle) ? "$title: $subtitle" : $title;
      $title = !empty($non_sort) ? "$non_sort $title" : $title;
      // Choose the longest title.
      $output = strlen($title) > strlen($output) ? $title : $output;
    }
  }
  return !empty($output) ? $output : NULL;
}

/**
 * Gets the event property for the Citation.
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 *
 * @return string
 *   The event property for the Citation.
 */
function convert_mods_to_citeproc_json_event(SimpleXMLElement $mods) {
  $property = convert_mods_to_citeproc_json_query($mods, '//mods:mods[1][mods:genre[@authority="marcgt"][text()="conference publication"]]/mods:relatedItem/mods:titleInfo/mods:title');
  // marcgt.
  if (isset($property)) {
    return $property;
  }
  else {
    // zotero.
    return convert_mods_to_citeproc_json_query($mods, '//mods:mods[1][mods:genre[@authority="local"]="conferencePaper"]/mods:relatedItem/mods:titleInfo/mods:title');
  }
}

/**
 * Gets the event-place property for the Citation.
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 *
 * @return string
 *   The event-place property for the Citation.
 */
function convert_mods_to_citeproc_json_event_place(SimpleXMLElement $mods) {
  $property = convert_mods_to_citeproc_json_query($mods, '//mods:mods[1][mods:genre[@authority="marcgt"][text()="conference publication"]]/mods:originInfo/mods:place/mods:placeTerm');
  // marcgt.
  if (isset($property)) {
    return $property;
  }
  else {
    // zotero.
    return convert_mods_to_citeproc_json_query($mods, '//mods:mods[1][mods:genre[@authority="local"][text()="conferencePaper"]]/mods:originInfo/mods:place/mods:placeTerm');
  }
}

/**
 * Gets the note property for the Citation.
 *
 * KEYWORD seems to be useless to CSL
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 *
 * @return string
 *   The note property for the Citation.
 */
function convert_mods_to_citeproc_json_note(SimpleXMLElement $mods) {
  $notes = $mods->xpath("//mods:mods[1]/mods:note");
  $ouput = '';
  while (list($num, $note) = each($notes)) {
    $ouput .= $num + 1 . ". " . rtrim(strip_tags($note), '. ') . ".  ";
  }
  return !empty($ouput) ? $ouput : NULL;
}

/**
 * Gets the page property for the Citation.
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 *
 * @return string
 *   The page property for the Citation.
 */
function convert_mods_to_citeproc_json_page(SimpleXMLElement $mods) {
  $output = NULL;
  $pages = $mods->xpath("//mods:mods[1]/mods:part/mods:extent[@unit='pages']");
  // Note: "pages" is correct, but Zotero uses "page".
  if (empty($pages)) {
    $pages = $mods->xpath("//mods:mods[1]/mods:part/mods:extent[@unit='page']");
  }

  $pages = (!empty($pages) ? $pages[0] : NULL);
  if ($pages) {
    add_mods_namespace($pages);

    $total = $pages->xpath('mods:total');
    $total = (!empty($total) ? (string) $total[0] : NULL);

    $list = $pages->xpath('mods:list');
    $list = (!empty($list) ? (string) $list[0] : NULL);

    $start = $pages->xpath('mods:start');
    $start = (!empty($start) ? (string) $start[0] : NULL);

    $end = $pages->xpath('mods:end');
    $end = (!empty($end) ? (string) $end[0] : NULL);

    if ($total) {
      $output = $total;
    }
    elseif ($list) {
      $output = $list;
    }
    elseif ($start) {
      $output = $start;
      if ($end) {
        $output .= "-" . $end;
      }
    }
  }
  return $output;
}

/**
 * Gets the type property for the Citation.
 *
 * @auth='endnote'-- seems to map to the CSL types easier
 * @auth='marcgt' -- marcgt should be the preferred authority
 * @auth='local'  -- actually better at differentiating some types
 * not(@auth)     -- unauthoritative types from Bibutils
 * genre == 'book' is especially difficult
 * //mods/relatedItem[@type='host']/genre[@authority='marcgt'] == 'book' means
 * "Chapter"
 * //mods/genre[@authority='marcgt'] == 'book' means "Book"
 * *UNLESS* //mods/relatedItem[type='host']/titleInfo/title exists
 * *OR*     //mods/genre[@authority='local'] == 'bookSection'*
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 *
 * @return string
 *   The type property for the Citation.
 */
function convert_mods_to_citeproc_json_type(SimpleXMLElement $mods) {
  module_load_include('inc', 'citeproc', 'includes/mods_csl_type_conversion');
  module_load_include('inc', 'citeproc', 'includes/marcrelator_conversion');

  // First try:  Let's map the a provided Endnote genre...
  $output = _get_endnote_type($mods);

  // Second try: item's local marcgt genre.
  if (empty($output)) {
    $output = _get_marcgt_type($mods);
  }

  // Third try: item's parent marcgt genre (often applies to the original item
  // itself).
  if (empty($output)) {
    $output = _get_related_marcgt_type($mods);
  }
  // Last try: other authority types (most likely Zotero local).
  if (empty($output)) {
    $output = _get_other_types($mods);
  }
  return $output;
}

/**
 * Map from an EndNote 'ref-type' to a CSL type.
 *
 * Seems to map better than marcgt
 * 'ref-type' were added manually to the MODS as top-level genre with the
 * authority set to 'endnote'
 * @todo  Map all default types
 */
function _get_endnote_type(SimpleXMLElement $mods) {
  $output = '';

  $type_map = array(
    'Book Section' => 'chapter',
    'Book' => 'book',
    'Edited Book' => 'book',
    'Journal Article' => 'article-journal',
    // XXX:  This is a custom one for FJM...  Might need a hook?
    'Working Paper' => 'chapter',
    'Thesis' => 'thesis',
  );
  $types = $mods->xpath("//mods:mods[1]/mods:genre[@authority='endnote']");

  if (!empty($types)) {
    $type = $types[0];
    $output = $type_map[(string) $type];
  }

  return $output;
}

/**
 * Get the marcgt type.
 */
function _get_marcgt_type(SimpleXMLElement $mods) {
  $output = '';
  $type_marcgt = $mods->xpath("//mods:mods[1]/mods:genre[@authority='marcgt']");
  if (!empty($type_marcgt)) {
    $interim_type =& $type_marcgt[0];
    add_mods_namespace($interim_type);
    if (!strcasecmp($interim_type, 'book')) {
      $host_titles = $interim_type->xpath("../mods:relatedItem[@type='host']/mods:titleInfo/mods:title");
      if (!empty($host_titles)) {
        // This is but a chapter in a book.
        $output = 'chapter';
      }
      else {
        $output = 'book';
      }
    }
    else {
      $output = marcgt_to_csl((string) $interim_type);
    }
  }
  return $output;
}

/**
 * Get the related marcgt type.
 */
function _get_related_marcgt_type(SimpleXMLElement $mods) {
  $output = '';

  $type_marcgt_related = $mods->xpath("//mods:mods[1]/mods:relatedItem/mods:genre[@authority='marcgt']");
  if (!empty($type_marcgt_related)) {
    $interim_type = (string) $type_marcgt_related[0];
    if (!strcasecmp($interim_type, 'book')) {
      $output = 'chapter';
    }
    else {
      $output = marcgt_to_csl($interim_type);
    }
  }

  return $output;
}

/**
 * Get other types.
 */
function _get_other_types(SimpleXMLElement $mods) {
  $output = '';

  $types_local_auth = $mods->xpath("//mods:mods[1]/mods:genre[not(@authority='marcgt' or @authority='endnote')]");
  while (empty($output) && list($num, $type) = each($types_local_auth)) {
    $interim_type = (string) $type;
    $output = mods_genre_to_csl_type($interim_type);
  }

  return $output;
}

/**
 * Gets the type property for the Citation.
 *
 * There are a number of name-type vars which may be populated.
 * We will concern ourselves with the following:
 *  1. Author
 *  2. Editor
 *  3. Translator
 * There is no CSL var for "contributor", we will treat them as additional
 * authors.
 *
 * Each name may be either "corporate" or "given name / family name"
 *
 * As it's unlikely we'll have participles, suffixes, etc properly parsed
 * out, we will always pass the ("parse-names" : "true") flag with personal
 * names.
 *
 * mods namepart types (given, family) correspond to citeproc elements,
 * however, more precise mods elements (nonsort, etc.) do not.
 * @todo Make all name handling better.
 *
 * NAME(s) of RELATED ITEMS (host, series)
 * Zotero vs Bibutils do this a bit differently, but in bibutils it's common for
 * the editor of a book (containing a chapter, which is our bibliographic item)
 * to be associated with the relatedItem(host).
 * Also It'S A Shot In The Dark, But
 * Relateditem(series)->name->role->roleTerm=editor is plausible.
 *
 * Note also this section is *highly* repetitive of the section above and this
 * should probably be generalized, but for now a strict procedural reckoning
 * will have to suffice. The difference is in the very last section, where the
 * appropriate cs:names type is specified.
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 *
 * @return string
 *   The type property for the Citation.
 */
function convert_mods_to_citeproc_json_names(SimpleXMLElement $mods) {
  $queries = array(
    0 => array(
      // Path.
      '//mods:mods[1]/mods:name[normalize-space(mods:namePart)]',
      // Default Role.
      'author',
      // Valid Roles.
      array(
        'editor' => 'editor',
        'translator' => 'translator',
        'interviewer' => 'interviewer',
        'composer' => 'composer',
        'original' => 'original-author',
        'recipient' => 'recipient',
        'author' => 'author',
      ),
    ),
    1 => array(
      '//mods:mods[1]/mods:relatedItem[@type="host"]/mods:name',
      'container-author',
      array(
        'editor' => 'editor',
        'translator' => 'translator',
        'author' => 'container-author',
      ),
    ),
    2 => array(
      '//mods:mods[1]/mods:relatedItem[@type="series"]/mods:name',
      NULL,
      array(
        'editor' => 'collection-editor',
      ),
    ),
  );
  $output = array();
  foreach ($queries as $query) {
    list($path, $default_role, $valid_roles) = $query;
    $names = $mods->xpath($path);
    if (!empty($names)) {
      foreach ($names as $name) {

        add_mods_namespace($name);
        $role = convert_mods_to_citeproc_json_name_role($name, $valid_roles, $default_role);
        if ($role !== FALSE) {
          $parsed_name = convert_mods_to_citeproc_json_name($name);
          if ($parsed_name) {
            $output[$role][] = $parsed_name;
          }
        }
      }
    }
  }
  return $output;
}

/**
 * Gets the array repersentation of the javascript Citation's name properties.
 *
 * @param SimpleXMLElement $name
 *   A name element.
 *
 * @return array
 *   An array that embodies the name properties of a Citation javascript object.
 */
function convert_mods_to_citeproc_json_name(SimpleXMLElement $name) {
  $type = (string) $name->attributes()->type;
  $output = ($type == 'personal') ?
      convert_mods_to_citeproc_json_name_personal($name) :
      convert_mods_to_citeproc_json_name_corporate($name);
  $output = array_map('trim', $output);
  $output = array_map('ucfirst', $output);
  $output = array_filter($output);
  return $output;
}

/**
 * Gets the array repersentation of the Citation's personal name properties.
 *
 * @param SimpleXMLElement $name
 *   A name element.
 *
 * @return array
 *   An array that embodies the name properties of a Citation javascript object.
 */
function convert_mods_to_citeproc_json_name_personal(SimpleXMLElement $name) {
  $output = array();
  $name_parts = $name->xpath("mods:namePart");

  foreach ($name_parts as $name_part) {
    $type = (string) $name_part->attributes()->type;
    $content = (string) $name_part;

    // If the type is empty, it is a combined name last, first.
    if (empty($type)) {
      // Filter empty things out of the output, so we may potentially replace
      // them.
      $output = array_filter(array_map('trim', $output));

      $names = array_map('trim', explode(',', $content));
      $exploded = array();
      $exploded['family'] = array_shift($names);
      $exploded['given'] = array_shift($names);

      // If we appear to have a family (and potentially given) name part in our
      // combined field and are not currently outputting them, make them get
      // output.
      // XXX: May combine unexpectedly in the case a "given" typed part name is
      // specified, and our untyped field only contains a "family" name.
      $non_empty = array_filter($exploded);
      $parts_not_present = array_diff_key($non_empty, $output);
      if (!empty($parts_not_present)) {
        $output = $non_empty + $output;
      }
    }
    else {
      // Throw a period after single character values, as they likely represent
      // initials.
      $content .= (strlen($content) == 1) ? '. ' : ' ';
      $output[$type] = isset($output[$type]) ?
          $output[$type] . $content :
          $content;
    }
  }

  return $output;
}

/**
 * Gets the array repersentation of the Citation's corporate name properties.
 *
 * @param SimpleXMLElement $name
 *   A name element.
 *
 * @return array
 *   An array that embodies the name properties of a Citation javascript object.
 */
function convert_mods_to_citeproc_json_name_corporate(SimpleXMLElement $name) {
  $output = array();
  $name_parts = $name->xpath("mods:namePart");
  foreach ($name_parts as $name_part) {
    $content = (string) $name_part . ' ';
    $output['literal'] = isset($output['literal']) ? $output['literal'] . $content : $content;
  }
  return $output;
}

/**
 * Gets the role for the given name element.
 *
 * If no role can be determined it returns 'author' as a default.
 *
 * @param SimpleXMLElement $name
 *   A MODS name element.
 * @param array $valid_roles
 *   A map of mods role names to their citeproc equivalents.
 * @param string $default_role
 *   The role to use if a valid role is not found.
 *
 * @return string
 *   Gets the role of the given name.
 */
function convert_mods_to_citeproc_json_name_role(SimpleXMLElement $name, array $valid_roles, $default_role) {
  module_load_include('inc', 'citeproc', 'includes/marcrelator_conversion');
  $role_term = $name->xpath("mods:role/mods:roleTerm");
  if (isset($role_term[0])) {
    $role = strtolower((string) $role_term[0]);
    $role_term = $role_term[0];
  }
  else {
    $role = NULL;
  }

  if ($role) {
    $role_authority = (string) $role_term->attributes()->authority;
    $role_type = (string) $role_term->attributes()->type;

    if ($role_authority == 'marcrelator' && $role_type == 'code') {
      $role = marcrelator_code_to_term($role);
    }
    return array_key_exists($role, $valid_roles) ? $valid_roles[$role] : FALSE;
  }
  return $default_role;
}

/**
 * Try to parse data.
 */
function _try_parse_date(&$output, $date_string) {
  $parser = CSLDateParser::getInstance();
  $parser->returnAsArray();
  $output = $parser->parse($date_string);
}

/**
 * Get the dates.
 */
function convert_mods_to_citeproc_json_dates(SimpleXMLElement $mods) {
  $output = array();
  $season = convert_mods_to_citeproc_json_query($mods, "//mods:originInfo/mods:dateOther[@type='season']");
  $date_captured = convert_mods_to_citeproc_json_query($mods, "//mods:originInfo/mods:dateCaptured");
  if (!empty($date_captured)) {
    _try_parse_date($output['accessed'], $date_captured);
  }

  $date_issued = convert_mods_to_citeproc_json_query($mods, "//mods:originInfo/mods:dateIssued | //mods:mods[1]/mods:relatedItem[@type='host']/mods:part/mods:date");
  if (!empty($date_issued)) {
    _try_parse_date($output['issued'], $date_issued);
  }

  $date_created = convert_mods_to_citeproc_json_query($mods, "//mods:originInfo/mods:dateCreated");
  if (!empty($date_created) && empty($output['issued'])) {
    _try_parse_date($output['issued'], $date_created);
  }

  if (!empty($season) && isset($output['issued']['date-parts'][0]) && count($output['issued']['date-parts'][0]) == 1) {
    $seasons = array('', 'Spring', 'Summer', 'Fall', 'Winter');
    $output['issued']['season'] = (string) array_search($season, $seasons);
  }
  if (empty($output)) {
    $output['issued'] = array('string-literal' => '');
  }
  return $output;
}

/**
 * Gets the first result of the provided $path.
 *
 * @param SimpleXMLElement $mods
 *   A MODS document.
 * @param string $path
 *   An XPath.
 *
 * @return string
 *   The first results of the query if found NULL otherwise.
 */
function convert_mods_to_citeproc_json_query(SimpleXMLElement $mods, $path) {
  $property = $mods->xpath($path);
  return !empty($property) ? (string) $property[0] : NULL;
}
