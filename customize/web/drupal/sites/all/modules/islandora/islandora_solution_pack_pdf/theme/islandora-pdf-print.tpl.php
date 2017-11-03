<?php

/**
 * @file
 * This is the template file for the print pdf object
 *
 * @TODO: Add documentation about this file and the available variables
 * $islandora_object - the object being displayed
 * $islandora_preview_img - the preview image for printing
 */
?>

<div class="islandora-pdf-object islandora" vocab="http://schema.org/" prefix="dcterms: http://purl.org/dc/terms/" typeof="Article">
  <div class="islandora-pdf-content-wrapper clearfix">
    <?php if (isset($islandora_preview_img)): ?>
      <div class="islandora-pdf-print">
        <?php print $islandora_preview_img; ?>
      </div>
    <?php endif; ?>
  </div>
</div>