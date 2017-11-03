<?php
/**
 * @file
 * Template file to style output.
 */
?>
<?php
  print $book_object_id ? l(t('Return to Book View'), "islandora/object/{$book_object_id}") : t('Orphaned page (no associated book)');
?>
<?php if (isset($viewer)): ?>
  <div id="book-page-viewer">
    <?php print $viewer; ?>
  </div>
<?php elseif (isset($object['JPG']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JPG'])): ?>
  <div id="book-page-image">
    <?php
      $params = array(
        'path' => url("islandora/object/{$object->id}/datastream/JPG/view"),
        'attributes' => array(),
      );
      print theme('image', $params);
    ?>
  </div>
<?php endif; ?>
<!-- @todo Add table of metadata values -->
