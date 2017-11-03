<?php

/**
 * @file
 * DB utility functionality for form builder.
 */

/**
 * Get all the default DC XSLT mappings.
 *
 * @return array
 *   The mappings keyed on id.
 */
function xml_form_builder_get_default_dc_xslt_mappings() {
  $query = db_select('xml_form_builder_default_xslts', 'mappings')
    ->fields('mappings', array());

  return $query->execute()
    ->fetchAllAssoc('id', PDO::FETCH_ASSOC);
}

/**
 * Get all the default DC XSLT mappings.
 *
 * @return array
 *   The mappings keyed on id with the XSLT name as well.
 */
function xml_form_builder_get_default_dc_xslt_mappings_with_xslt_name() {
  $query = db_select('xml_form_builder_default_xslts', 'mappings')
    ->fields('mappings', array());

  $alias = $query->join('xml_form_builder_xslts', 'x', 'x.xslt_id = mappings.xslt_id');
  $query->addField($alias, 'name', 'xslt_name');

  return $query->execute()
    ->fetchAllAssoc('id', PDO::FETCH_ASSOC);
}

/**
 * Get all the default XSLTs.
 *
 * @return array
 *   The XSLTs keyed on xslt_id.
 */
function xml_form_builder_get_xslts() {
  $query = db_select('xml_form_builder_xslts', 'XSLTs')
    ->fields('XSLTs', array());

  return $query->execute()
    ->fetchAllAssoc('xslt_id', PDO::FETCH_ASSOC);
}

/**
 * Get a transform string.
 *
 * @param string|array $content_model
 *   The content model(s) to get the transform for.
 * @param string $dsid
 *   The datastream ID to get the transform for.
 *
 * @return string
 *   The transform.
 */
function xml_form_builder_get_default_transform($content_model, $dsid) {
  $query = db_select('xml_form_builder_xslts', 'x')
    ->fields('x', array('xslt'));

  $alias = $query->join('xml_form_builder_default_xslts', 'd', 'x.xslt_id = d.xslt_id');

  return $query->condition("$alias.content_model", $content_model)
    ->condition("$alias.dsid", $dsid)
    ->execute()
    ->fetchColumn();
}

/**
 * Add an XSLT.
 *
 * @param string $transform
 *   The transform to add to the database.
 * @param string $name
 *   Unique human readable designation.
 *
 * @return int
 *   The database entry's ID.
 */
function xml_form_builder_add_xslt($transform, $name) {
  return db_insert('xml_form_builder_xslts')
    ->fields(
      array(
        'xslt' => $transform,
        'name' => $name,
      )
    )
    ->execute();
}

/**
 * Remove the XSLT.
 *
 * @param int $xslt_id
 *   The id of the XSLT to remove.
 */
function xml_form_builder_remove_xslt($xslt_id) {
  db_delete('xml_form_builder_xslts')
    ->condition('xslt_id', $xslt_id)
    ->execute();
}

/**
 * Add a default DC transform mapping.
 *
 * @param string $content_model
 *   The content model to apply the XSLT to.
 * @param string $dsid
 *   The datastream to apply the XSLT to.
 * @param int $xslt_id
 *   The database ID of the XSLT to apply.
 * @param string $name
 *   Unique human readable designation.
 *
 * @return int
 *   The database entry's ID.
 */
function xml_form_builder_add_default_dc_transform_mapping($content_model, $dsid, $xslt_id, $name) {
  return db_insert('xml_form_builder_default_xslts')
    ->fields(
      array(
        'content_model' => $content_model,
        'dsid' => $dsid,
        'xslt_id' => $xslt_id,
        'name' => $name,
      )
    )
    ->execute();
}

/**
 * Remove a default DC transform mapping.
 *
 * @param int $id
 *   The id of the mapping to remove.
 */
function xml_form_builder_remove_default_dc_transform_mapping($id) {
  db_delete('xml_form_builder_default_xslts')
    ->condition('id', $id)
    ->execute();
}

/**
 * Determine if an XSLT is being used in a default mapping.
 *
 * @param int $xslt_id
 *   The UID of the XSLT to check for.
 *
 * @return bool
 *   TRUE if the XSLT is in a transform, FALSE otherwise.
 */
function xml_form_builder_xslt_is_a_default($xslt_id) {
  $count = db_select('xml_form_builder_default_xslts', 'x')
    ->fields('x', array('xslt_id'))
    ->condition("xslt_id", $xslt_id)
    ->execute()
    ->rowCount();

  if ($count) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
