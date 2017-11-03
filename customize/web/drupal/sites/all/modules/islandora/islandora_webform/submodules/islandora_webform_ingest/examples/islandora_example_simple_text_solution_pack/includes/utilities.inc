<?php

/**
 * Gets any objects that the given object has a parent relationship with.
 *
 * Parent relationships are not defined here, so we assume any
 *
 * This function gets its info from the RELS-EXT directly rather than through an
 * risearch.
 *
 * @param AbstractObject $object
 *   The object whose parents will be returned.
 *
 * @return array
 *   An array of FedoraObject's that the given object has a
 *   (RELS-EXT) relationship with.
 */
function islandora_example_simple_text_get_parents_from_rels_ext(AbstractObject $object) {
  $parentobjects = array();
  try {
    $parentobjects = $object->relationships->get(FEDORA_RELS_EXT_URI,NULL,NULL,RELS_TYPE_URI);
  }
  catch (RepositoryException $e) {
    return array();
  }
  $map = function ($o) {
    if (($o['predicate']['namespace'] != 'info:fedora/fedora-system:def/model#') && ($o['object']['literal']!=TRUE)){
      // Don't count fedora model relations or literals
      return islandora_object_load($o['object']['value']);
    }
  };

  $parentobjects = array_map($map, $parentobjects);
  return array_filter(array_unique($parentobjects));
}