<?php

/**
 * @file
 * Miscellaneous functions.
 */

/**
 * Get the URL for the given PID.
 *
 * @param string $pid
 *   A PID for which to get the URL.
 *
 * @return string
 *   The URL for the given PID.
 */
function islandora_xmlsitemap_get_link_url($pid) {
  return ISLANDORA_XMLSITEMAP_PATH_PREFIX . $pid;
}

/**
 * Ge the "subtype" associated we're using.
 *
 * We essentially map back to the odd-looking "info:fedora/$pid" format, so as
 * to be extremely likely to be unique.
 *
 * @param string $pid
 *   The PID for which to get the subtype ID.
 *
 * @return string
 *   The subtype ID for the given PID.
 */
function islandora_xmlsitemap_get_subtype_id($pid) {
  return "info:fedora/$pid";
}

/**
 * Add or update the link entry for the given PID.
 *
 * @param string $pid
 *   The PID for which to (re)generate a link entry.
 * @param array $sandbox
 *   A reference to the batch sandbox, in which we can persist some value
 *   between iterations. Optional.
 *
 * @return array
 *   The link structure, which should have already been entered into the
 *   {xmlsitemap} table.
 */
function islandora_xmlsitemap_add_or_update_link($pid, &$sandbox = NULL) {
  $object = islandora_object_load($pid);
  $link = array(
    'type' => 'custom',
    'loc' => islandora_xmlsitemap_get_link_url($pid),
    'subtype' => islandora_xmlsitemap_get_subtype_id($pid),
    'changefreq' => XMLSITEMAP_FREQUENCY_MONTHLY,
  );
  if ($object) {
    $link['access'] = islandora_object_access(ISLANDORA_VIEW_OBJECTS, $object, drupal_anonymous_user());
    $link['lastmod'] = $object->lastModifiedDate->getTimestamp();
    $sandbox['last_modified'] = (string) $object->lastModifiedDate;
  }
  else {
    // Assume that anonymous users won't be able to access, if we can't here.
    $link['access'] = FALSE;
  }
  $link += (array) islandora_xmlsitemap_get_current($pid);
  if (!isset($link['id'])) {
    module_load_include('inc', 'islandora_xmlsitemap', 'includes/db');
    try {
      islandora_xmlsitemap_insert_increment_record($pid);
    }
    catch (PDOException $e) {
      // Don't do anything in this case; we already have an ID.
    }
    $link['id'] = islandora_xmlsitemap_get_id_for_record($pid);
  }

  return xmlsitemap_link_save($link);
}

/**
 * Get an array representing the current link.
 *
 * @param string $pid
 *   A PID for which to try to get the newest entry. We add values to the
 *   "subtype" column, which should allow us to better identify our URLs.
 */
function islandora_xmlsitemap_get_current($pid) {
  $q = <<<EOQ
SELECT *
FROM {xmlsitemap}
WHERE type = 'custom' AND (
  subtype = :sub_id OR loc = :loc
)
EOQ;

  return db_query_range($q, 0, 1, array(
    ':sub_id' => islandora_xmlsitemap_get_subtype_id($pid),
    ':loc' => islandora_xmlsitemap_get_link_url($pid),
  ))->fetch(PDO::FETCH_ASSOC);
}
