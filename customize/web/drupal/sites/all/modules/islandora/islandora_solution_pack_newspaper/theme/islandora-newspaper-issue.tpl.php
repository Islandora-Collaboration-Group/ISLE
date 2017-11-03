<?php

/**
 * @file
 */
?>
<div class="islandora-newspaper-issue clearfix">
  <span class="islandora-newspaper-issue-navigator">
    <?php print theme('islandora_newspaper_issue_navigator', array('object' => $object)); ?>
  </span>
  <?php if (isset($viewer_id) && $viewer_id != "none"): ?>
    <div id="book-viewer">
      <?php print $viewer; ?>
    </div>
  <?php else: ?>
    <?php print theme('islandora_objects', array('objects' => $pages)); ?>
  <?php endif; ?>
  <div class="islandora-newspaper-issue-metadata">
    <?php print $description; ?>
    <?php print $metadata; ?>
  </div>
</div>
