<?php

/**
 * @file
 * Defines an array to hold MODS XPaths for conveting to RIS
 *
 * @author Nelson Hart <nelson@discoverygarden.ca>
 * @date Sept 10, 2012
 */

/**
 * Gets the RIS field to MODS XPath mapping.
 *
 * @return array
 *   The RIS field to MODS XPath mapping.
 */
function bibutils_ris_field_map() {
  // XXX: Some comments go past 80 columns... Let's leave 'em be, for now.
  // @codingStandardsIgnoreStart
  return array(
    "TY" => "mods:genre",
    "A2" => "mods:relatedItem[@type = 'host']/mods:name[@type = 'corporate' and mods:role/mods:roleTerm = 'author']/mods:namePart",
    // "A3" => "Tertiary Author (each author on its own line preceded by the tag)",
    // "A4" => "Subsidiary Author (each author on its own line preceded by the tag)",
    "AB" => "mods:abstract[not(@displayLabel) or @displayLabel = 'combined-all-export']",
    // "AD" => "Author Address",
    "AN" => "mods:identifier[@type = 'accession']",
    "AU" => "mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'author']",
    // "C1" => "Custom 1",
    // "C2" => "Custom 2",
    // "C3" => "Custom 3",
    // "C4" => "Custom 4",
    // "C5" => "Custom 5",
    // "C6" => "Custom 6",
    // "C7" => "Custom 7",
    // "C8" => "Custom 8",
    // "CA" => "Caption",
    // "CN" => "Call Number",
    "CY" => "mods:originInfo/mods:place/mods:placeTerm",
    "DA" => "mods:originInfo/mods:dateIssued",
    // "DB" => "Name of Database",
    "DO" => "mods:identifier[@type = 'doi']",
    // "DP" => "Database Provider",
    // "EP" => "End Page",
    "ET" => "mods:originInfo/mods:edition",
    "IS" => "mods:part/mods:detail[@type = 'issue']/mods:number",
    "J2" => "mods:relatedItem[@type='host']/mods:titleInfo[@type='abbreviated']/mods:title",
    "KW" => "mods:note[@type = 'keywords']",
    // "L1" => "File Attachments (this is a link to a local file on the users system not a URL link)",
    // "L4" => "Figure (this is also meant to be a link to a local file on the users's system and not a URL link)",
    "LA" => "mods:language/mods:languageTerm",
    // "LB" => "Label",
    // "M1" => "Number",
    "M3" => "mods:note[@type = 'type of work']",
    // "N1" => "Notes",
    // "NV" => "Number of Volumes",
    // "OP" => "Original Publication",
    "PB" => "mods:originInfo/mods:publisher",
    "PY" => "mods:originInfo/mods:dateIssued",
    "RI" => "mods:note[@type = 'peer reviewed']",
    // "RN" => "Research Notes",
    // "RP" => "Reprint Edition",
    // "SE" => "Section",
    "SN" => "mods:identifier[@type = 'issn' or @type = 'isbn' ]",
    "SP" => "mods:part/mods:extent[@unit = 'page']/mods:list",
    // "ST" => "Short Title",
    "T2" => "mods:relatedItem[@type = 'host']/mods:titleInfo[not(@type)]/mods:title",
    "T3" => "mods:relatedItem[@type = 'series']/mods:titleInfo/mods:title",
    // "TA" => "Translated Author",
    "TI" => "mods:titleInfo/mods:title",
    // "TT" => "Translated Title",
    // "UR" => "URL",
    "VL" => "mods:part/mods:detail[@type = 'volume']/mods:number",
    // "Y2" => "Access Date",
    "ED" => "mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'editor']",
    // ID added at export time for getting PID.
    "ID" => "mods:identifier[@type = 'id export']",
    "ER" => "",
  );
  // @codingStandardsIgnoreEnd
}
