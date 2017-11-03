<?php

/**
 * @file
 * This file lists and documents all available hook functions to manipulate
 * data.
 */

/**
 * This hook allows modules to modify citation text during export.
 *
 * Note: This modification of output text triggers during export of rtf, and pdf
 * citations.
 *
 * @param string $text
 *   The output text to be added to the export file.
 *
 * @param AbstractObject $object
 *   The current object being exported to text.
 */
function hook_citation_exporter_modify_exporting_citation_text_alter(&$text, $object) {
  // Any modifications to text will be exported to the output file.
  // The object may be used to pull out data from any relevant datastreams and
  // added to the text variable.
  $text = str_replace("</div></div>", " - PID: {$object->pid}</div></div>", $text);
}
