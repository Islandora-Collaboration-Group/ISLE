<?php

/**
 * @file
 * islandora-newspaper-page.tpl.php
 * This is the template file for the object page for newspaper
 *
 * Available variables:
 * - $object: The Islandora object rendered in this template file
 * - $islandora_content: A rendered image. By default this is the JPG datastream
 *   which is a medium sized image. Alternatively this could be a rendered
 *   viewer which displays the JP2 datastream image.
 * - $description: Rendered metadata descripton for the object.
 * - $metadata: Rendered metadata display for the binary object.
 *
 * @see template_preprocess_islandora_newspaper_page()
 * @see theme_islandora_newspaper_page()
 *
 */
?>
<div class="islandora-newspaper-object">
  <div class="islandora-newspaper-controls">
    <?php print theme('islandora_newspaper_page_controls', array('object' => $object)); ?>
  </div>
  <div class="islandora-newspaper-content-wrapper clearfix">
    <?php if ($content): ?>
      <div class="islandora-newspaper-content">
        <?php print $content; ?>
      </div>
    <?php endif; ?>
  </div>
  <div class="islandora-newspaper-page-metadata">
    <?php print $description; ?>
    <?php print $metadata; ?>
  </div>
</div>
