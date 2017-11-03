<?php
/**
 * @file
 * This is the template file for the object page for organization objects.
 */
?>

<div class="islandora-object islandora">
<?php if (isset($variables['tn'])): ?>
  <dl class="islandora-object-tn islandora-department">
    <dt>
      <img src="<?php print $variables['tn']; ?>"/>
    </dt>
  </dl>
<?php endif; ?>

<?php if (isset($variables['metadata'])): ?>
  <div class="departmental_metadata">
    <?php print $variables['metadata']; ?>
  </div>
<?php endif; ?>
</div>
