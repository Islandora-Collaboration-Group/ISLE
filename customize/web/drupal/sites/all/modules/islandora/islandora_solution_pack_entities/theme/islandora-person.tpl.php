<?php
/**
 * @file
 * This is the template file for the object page for person objects.
 */
?>
<div class="islandora-object islandora">

  <div class="col-left">
    <div class="islandora-object-image">
      <?php if (isset($variables['tn'])): ?>
        <img src="<?php print $variables['tn']; ?>"/>
      <?php endif; ?>
    </div>
    <div class="islandora-object-metadata">
      <?php if (isset($variables['metadata'])): ?>
        <?php print $variables['metadata']; ?>
      <?php endif; ?>
    </div>
    <div class="islandora-object-activities">
      <?php if (isset($variables['activities'])): ?>
        <?php print $variables['activities']; ?>
      <?php endif; ?>
    </div>
  </div>

  <div class="col-right">
    <div class="islandora-object-scholars">
      <?php if (isset($variables['fellow_scholars'])): ?>
        <?php foreach ($variables['fellow_scholars'] as $dept): ?>
          <?php print $dept; ?>
        <?php endforeach; ?>
      <?php endif; ?>
    </div>
  </div>

  <div class="islandora-object-content">
    <h3 class="bio"><?php print t('Biography'); ?></h3>
    <?php if (isset($variables['biography'])): ?>
      <p><?php print $variables['biography']; ?></p>
    <?php endif; ?>

    <?php if (isset($variables['recent_citations'])): ?>
      <?php print $variables['recent_citations']; ?>
    <?php endif; ?>
    <?php if (isset($variables['rss_feed'])): ?>
      <?php print $variables['rss_feed']; ?>
    <?php endif; ?>
  </div>

</div>
