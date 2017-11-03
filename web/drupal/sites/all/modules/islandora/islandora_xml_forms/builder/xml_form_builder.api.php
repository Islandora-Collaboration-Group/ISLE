<?php
/**
 * @file
 * This file lists and documents all available hook functions to manipulate
 * data.
 */

/**
 * This hook allows modules to add default forms to form builder.
 *
 * @return array
 *   An associative array mapping unique names to associative arrays containing
 *   a single key:
 *   - form_file: A string containing the path to the form definition, relative
 *     to the webserver's document root (such that I might be opened
 */
function hook_xml_form_builder_forms() {
  return array(
    'Unique Form Name' => array(
      'form_file' => 'full/path/to/form/definition/file.xml',
    ),
  );
}

/**
 * This hook allows modules to register XSLT transformations.
 *
 * @return array
 *   An associative array mapping a shortened name to the full path of the
 *   transformation.
 */
function hook_xml_form_builder_get_transforms() {
  return array(
    'awesome.xslt' => 'sites/all/modules/my_cool_module/transforms/awesome.xslt',
  );
}

/**
 * This hook allows modules to register XSLT self-transformations.
 *
 * @return array
 *   An associative array mapping a shortened name to the full path of the
 *   transformation.
 */
function hook_xml_form_builder_get_self_transforms() {
  return array(
    'cleanup.xslt' => 'sites/all/modules/my_cool_module/transforms/cleanup.xslt',
  );
}

/**
 * This hook allows modules to register default form associations.
 *
 * @return array
 *   An associative array mapping a unique name to an associative array which
 *   contains the following keys:
 *   - content_model: A string containing the PID for a content model.
 *   - form_name: A string identifying a form visible to XML Forms.
 *   - dsid: A string identifying what datastream should be created using the
 *     XML generated on submission.
 *   - title_field: An array containing the sequence of array keys identifying
 *     what value in the submitted form should be used as a title
 *   - transform: A string identifying which transform can be used on the XML
 *     to produce DC.  Can be "No Transform" if the form generates DC.
 *   - self_transform: A string identifying which transform can be used on the
 *     XML before producing final output. Can be "No Transform" if no self
 *     transform is being applied.
 *   - template: A string whose contents should be used to prepopulate the
 *     form.  Can be empty or FALSE.
 */
function hook_xml_form_builder_form_associations() {
  return array(
    // By convention, the unique name should start with your modules name.
    'unique_assoication_name' => array(
      'content_model' => 'islandora:sp_basic_image',
      'form_name' => 'Image DC Form',
      'dsid' => 'DC',
      'title_field' => array('titleInfo', 'title'),
      'transform' => 'No Transform',
      'template' => FALSE,
    ),
  );
}

/**
 * This hook allows modules to change form builder elements in the form array.
 *
 * The modification happens before the form schema is passed off to be
 * rendered to allow modifications to the base schema saved into the form
 * array. This is currently the only way to grant access to a form field
 * that is a part of a tab panel.
 *
 * @param array $form
 *   Modifications can be made to the form array allowing access to modify
 * values/attributes of the form elements after the form builder xml is loaded
 * into the form array.
 *
 * @param array $form_state
 *   Form State array.
 */
function hook_xml_form_builder_get_form_modify_definition_alter(&$form,
                                                                &$form_state) {

}
