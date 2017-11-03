<?php

/**
 * @file
 * Class for converting MODS to RIS
 *
 * @author Nelson Hart <nelson@discoverygarden.ca>
 * @date Sept 10, 2012
 *
 * @todo Build method for combining multiple nodes into one RIS field that is
 * not repeatable.
 * Eg: AB is not a repeatable field, but in my xml, I have multiple abstracts
 * from PUBMED. This new method would take all the nodes and combine them into
 * one string.
 */

class RIS {

  /**
   * Specifies all the different RIS types
   *
   * @var array
   */
  protected $types = array(
    "abstract" => "ABST",
    "audiovisual material" => "ADVS",
    "aggregated database" => "AGGR",
    "ancient text" => "ANCIENT",
    "art work" => "ART",
    "bill" => "BILL",
    "blog" => "BLOG",
    "whole book" => "BOOK",
    "case" => "CASE",
    "book chapter" => "CHAP",
    "chart" => "CHART",
    "classical work" => "CLSWK",
    "computer program" => "COMP",
    "conference proceeding" => "CONF",
    "conference paper" => "CPAPER",
    "catalog" => "CTLG",
    "data file" => "DATA",
    "online database" => "DBASE",
    "dictionary" => "DICT",
    "electronic book" => "EBOOK",
    "electronic book section" => "ECHAP",
    "edited book" => "EDBOOK",
    "electronic article" => "EJOUR",
    "web page" => "ELEC",
    "encyclopedia" => "ENCYC",
    "equation" => "EQUA",
    "figure" => "FIGURE",
    "generic" => "GEN",
    "government document" => "GOVDOC",
    "grant" => "GRANT",
    "hearing" => "HEAR",
    "internet communication" => "ICOMM",
    "in press" => "INPR",
    "journal (full)" => "JFULL",
    "journal" => "JOUR",
    "journal article" => "JOUR",
    "legal rule or regulation" => "LEGAL",
    "manuscript" => "MANSCPT",
    "map" => "MAP",
    "magazine article" => "MGZN",
    "motion picture" => "MPCT",
    "online multimedia" => "MULTI",
    "music score" => "MUSIC",
    "newspaper" => "NEWS",
    "pamphlet" => "PAMP",
    "patent" => "PAT",
    "personal communication" => "PCOMM",
    "report" => "RPRT",
    "serial publication" => "SER",
    "slide" => "SLIDE",
    "sound recording" => "SOUND",
    "standard" => "STAND",
    "statute" => "STAT",
    "thesis/dissertation" => "THES",
    "unpublished work" => "UNPB",
    "video recording" => "VIDEO",
  );

  /**
   * Specifies required RIS fields
   *
   * @var array
   */
  protected $requiredFields = array("TY", "ER");

  /**
   * Specifies which RIS fields are repeatable
   *
   * @var array
   */
  protected $repeatFields = array("A2", "A3", "A4", "AU", "KW", "ED");

  /**
   * Specifies which RIS fields are allowed
   *
   * @var array
   */
  protected $allowedFields = array(
    "TY" => "Type of reference (must be the first tag)",
    "A2" => "Secondary Author (each author on its own line preceded by the tag)",
    "A3" => "Tertiary Author (each author on its own line preceded by the tag)",
    "A4" => "Subsidiary Author (each author on its own line preceded by the tag)",
    "AB" => "Abstract",
    "AD" => "Author Address",
    "AN" => "Accession Number",
    "AU" => "Author (each author on its own line preceded by the tag)",
    "C1" => "Custom 1",
    "C2" => "Custom 2",
    "C3" => "Custom 3",
    "C4" => "Custom 4",
    "C5" => "Custom 5",
    "C6" => "Custom 6",
    "C7" => "Custom 7",
    "C8" => "Custom 8",
    "CA" => "Caption",
    "CN" => "Call Number",
    "CY" => "Place Published",
    "DA" => "Date",
    "DB" => "Name of Database",
    "DO" => "DOI",
    "DP" => "Database Provider",
    "EP" => "End Page",
    "ET" => "Edition",
    "IS" => "Number",
    "J2" => "Alternate Title (this field is used for the abbreviated title of a book or journal name)",
    "KW" => "Keywords (keywords should be entered each on its own line preceded by the tag)",
    "L1" => "File Attachments (this is a link to a local file on the users system not a URL link)",
    "L4" => "Figure (this is also meant to be a link to a local file on the users's system and not a URL link)",
    "LA" => "Language",
    "LB" => "Label",
    "M1" => "Number",
    "M3" => "Type of Work",
    "N1" => "Notes",
    "NV" => "Number of Volumes",
    "OP" => "Original Publication",
    "PB" => "Publisher",
    "PY" => "Year 20001/01// or 2011/02/01/ or 2011/02/01/<Other date info>",
    "RI" => "Reviewed Item",
    "RN" => "Research Notes",
    "RP" => "Reprint Edition",
    "SE" => "Section",
    "SN" => "ISBN/ISSN",
    "SP" => "Start Page",
    "ST" => "Short Title",
    "T2" => "series title Secondary Title",
    "T3" => "Tertiary Title",
    "TA" => "Translated Author",
    "TI" => "Title",
    "TT" => "Translated Title",
    "UR" => "URL",
    "VL" => "Volume",
    "Y2" => "Access Date",
    "ED" => "editor bibutils had this field",
    "ID" => "ID of object exported. I've noticed zotero and endnote add this unique ID field on their exports",
    "ER" => "End of Reference",
  );

  /**
   * Map MODS field (XPath) to RIS field
   *
   * @var array
   */
  protected $map;

  /**
   * The returned text of the class
   *
   * @var string
   */
  protected $text;

  /**
   * Create a ris instance with the xpath map for RIS fields.
   */
  protected function __construct($map) {
    $missing = FALSE;
    foreach ($this->requiredFields as $field) {
      if (!isset($map[$field])) {
        $missing = TRUE;
      }
    }
    if ($missing == FALSE) {
      $this->map = $map;
    }
    else {
      drupal_set_message(t('Your RIS field map is missing required fields.'), 'error');
    }
  }

  /**
   * Gets an instance of this object.
   */
  protected static function getInstance() {
    static $instance = NULL;
    if ($instance === NULL) {
      module_load_include('inc', 'bibutils', 'includes/ris_map');
      $instance = new self(bibutils_ris_field_map());
    }
    return $instance;
  }

  /**
   * Convenience method to transform the given MODS record into RIS.
   */
  public static function transform($mods) {
    return self::getInstance()->render($mods);
  }

  /**
   * Convenience method to transform the given MODS record into RIS.
   */
  public static function transformFile($mods_file) {
    return self::getInstance()->render(file_get_contents($mods_file));
  }

  /**
   * Gets the type of RIS Document.
   *
   * Eg: Book (BOOK), Book Chapter (CHAP)
   *
   * @param string $type
   *   The type.
   *
   * @return string
   *   RIS document type
   */
  protected function getType($type) {
    $type = strtolower($type);
    return isset($this->types[$type]) ? $this->types[$type] : "STD";
  }

  /**
   * Get the RIS date format of a standard data format.
   *
   * @param string $mods_date
   *   Format can be: YYYY-MM-DD or YYYY-MM or YYYY
   *
   * @return string
   *   RIS date
   */
  protected function getDate($mods_date) {
    $return = "";
    if (preg_match('/^[0-9]{4}-[0-9]{2}-[0-9]{2}$/', $mods_date) || preg_match('/^[0-9]{4}-[0-9]{2}$/', $mods_date) || preg_match('/^[0-9]{4}$/', $mods_date)) {
      preg_match_all('/-/', $mods_date, $dashes);
      $return = str_replace("-", "/", $mods_date);
      for ($i = count($dashes[0]); $i <= 2; $i++) {
        $return .= "/";
      }
    }
    return $return;
  }

  /**
   * Get the RIS Year.
   *
   * @param string $mods_date
   *   Format can be: YYYY*
   *
   * @return string
   *   RIS Year
   */
  protected function getYear($mods_date) {
    $return = "";
    if (preg_match('/^[0-9]{4}/', $mods_date, $year)) {
      $return = $year[0];
    }
    return $return;
  }

  /**
   * Get the name from MODS.
   *
   * @param SimpleXMLElement $names
   *   XML of the mods:name to the processed format:
   *   <name>
   *    <namePart type="family">
   *    <namePart type="given">
   *   </name>
   *
   * @param string $field
   *   RIS field to assiocate the name with
   */
  protected function renderName($names, $field) {
    if (!empty($field)) {
      $names->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
      $given = $names->xpath("mods:namePart[@type = 'given']");
      $family = $names->xpath("mods:namePart[@type = 'family']");
      if (!empty($given[0]) && !empty($family[0])) {
        $this->text .= "$field  - {$family[0]}, {$given[0]}.\n";
      }
    }
  }

  /**
   * Takes the given MODS XML and runs the RIS map XPaths against it.
   *
   * @param string $mods
   *   A string containing a MODS record (or a collection of them).
   *
   * @return string
   *   RIS formated text
   */
  protected function render($mods) {
    $this->text = "";
    if ($this->map) {
      $mods_doc = new SimpleXMLElement($mods);
      $mods_doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
      $mods_records = $mods_doc->xpath('//mods:mods');

      foreach ($mods_records as $mods_record) {
        $mods_record->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
        foreach ($this->map as $field => $xpath) {
          if (array_key_exists($field, $this->allowedFields)) {
            if ($field == "TY") {
              $data = $mods_record->xpath($xpath);
              $value = isset($data[0]) ? $this->getType($data[0]) : 'STD';
              $this->text .= "$field  - $value\n";
              continue;
            }
            if ($field == "ER") {
              $this->text .= "$field  - \n\n\n";
              continue;
            }
            $data = $mods_record->xpath($xpath);
            foreach ($data as $key => $val) {
              $value = trim($val);
              if ($field == "AU" || $field == "ED") {
                $this->renderName($data[$key], $field);
              }
              elseif ($field == "DA") {
                $value = $this->getDate($value);
              }
              elseif ($field == "PY") {
                $value = $this->getYear($value);
              }

              if (!empty($value)) {
                $this->text .= "$field  - $value\n";
              }

              if (!in_array($field, $this->repeatFields)) {
                break;
              }
            }
          }
        }
      }
    }
    return $this->text;
  }

}
