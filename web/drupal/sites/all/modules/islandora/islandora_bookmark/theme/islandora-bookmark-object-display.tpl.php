<?php

/**
 * @file
 * This is a template for generating the default markup for a Fedora object.
 *
 * Used in conjunction with Islandora Bookmark.
 *
 * The 'variables' array contains an array with information necessary for
 * creating an anchor tag
 *
 * @see: islandora_bookmark_object_display
 */
?>

<?php
  print l($object_url_info['markup'], $object_url_info['path'], array('query' => $object_url_info['params']))
?>
