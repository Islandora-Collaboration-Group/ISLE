<?php

/**
 * @file
 * New hook definitions.
 */

/**
 * Allow the definition of XML populators.
 *
 * @return array
 *   An associative array mapping unique (module-prefixed?) keys to arrays
 *   containing:
 *   - title
 *   - description
 *   - type
 *     - inline
 *     - form
 *   - form: If type is inline, an array containing a form definition. If type
 *     is "form", the name of a form to use. The form specified wi
 *   - output: Only necessary when "type" is "inline". An associative array
 *     mapping datastream IDs to arrays containing:
 *     - callback: The function used in the submit handler to process into the
 *       desired XML format. Should accept the $form and $form_state as
 *       parameters and return either a file object, content, or NULL if the
 *       callback will add the datastream itself. If a file-object or content,
 *       we will create a managed datastream; if needing to create content in
 *       another control group, grab the object and construct/ingest the
 *       datastream manually. If FALSE is returned from the callback, then we
 *       assume that we are doing something magical.
 *     - label callback: An optional function to get the label. Default is
 *       "<datastream id> datastream". Takes the $form, $form_state and result
 *       from 'callback' as input.
 *     - undo callback: An optional function to call to "undo" the effects of
 *       the callback. When a datastream was created, the default is to purge
 *       it, and when it already existed on the object, the default is to do
 *       nothing. Should accept the $form, $form_state, $datastream_id and
 *       $info array (listing these callbacks and mimetype info).
 *     - mimetype: The mimetype to apply to the output (defaults to
 *       "text/xml").
 *   - files: An array of arrays, each representing a set of parameters which
 *     might be sent to module_load_include().
 */
function hook_islandora_populator() {
  return array(
    'my_awesome_module_transmogrifier' => array(
      'title' => t('Transmogrifier'),
      'description' => t('Does things.'),
      'type' => 'inline',
      'form' => array(
        'file' => array(
          '#type' => 'managed_file',
        ),
      ),
      'output' => array(
        'STUFF' => array(
          'callback' => 'my_awesome_module_transmogrify',
          'mimetype' => 'application/json',
        ),
      ),
      'files' => array(
        array('inc', 'my_awesome_module', 'includes/transmogrifier'),
      ),
    ),
  );
}
