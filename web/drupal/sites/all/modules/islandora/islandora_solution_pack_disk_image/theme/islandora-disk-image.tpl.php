<?php
/**
 * @file
 * This is the template file for the object page for disk images
 *
 * @TODO: add documentation about file and available variables
 */
?>

<div class="islandora-disk-image-object islandora">
  <div class="islandora-disk-image-content-wrapper clearfix">
    <?php if (isset($islandora_content)): ?>
      <div class="islandora-disk-image-content">
        <?php print $islandora_content; ?>
      </div>
    <?php endif; ?>
    </div>
    <div class="islandora-disk-image-metadata">
      <?php print $description; ?>
    <div>
      <h2><?php print t('Download'); ?></h2>
        <ul>
          <?php if (isset($islandora_disk_image)): ?>
            <li>Disk Image: <?php print $islandora_disk_image; ?>
          <?php endif; ?>
        </ul>
    </div>
    <?php if ($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $collection): ?>
            <li><?php print l($collection->label, "islandora/object/{$collection->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
    <?php print $metadata; ?>
  </div>
</div>
