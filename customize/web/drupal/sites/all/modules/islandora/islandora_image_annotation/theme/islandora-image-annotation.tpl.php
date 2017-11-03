<?php

/**
 * @file
 * Displays the image annotations list canvas and dialog box.
 */
?>
<div id="islandora-image-annotation">
  <div class="islandora-image-annotation-left-column">
    <div id="islandora-image-annotation-tabs">
      <ul>
        <li><a href="#islandora-image-annotation-list">Image Annotations</a></li>
      </ul>
      <?php print $list; ?>
    </div>
  </div>
  <div class="islandora-image-annotation-right-column">
    <?php if (user_access(ISLANDORA_IMAGE_ANNOTATION_CREATE)): ?>
      <button id="islandora-image-annotation-create-annotation-button"><?php print t('Annotate'); ?></button>
    <?php endif; ?>
    <button id="islandora-image-annotation-full-window-button"><?php print t('Full Window'); ?></button>
    <button id="islandora-image-annotation-toggle-annotation-display-button" value="show"><?php print t('Show Annotation(s)'); ?></button>
    <div class="clearfix"></div>
    <?php print $canvas; ?>
    <?php print $logo; ?>
  </div>
  <?php print $dialog_box; ?>
</div>