<?php

/**
 * @file
 * Template for generating the bookmark list column view.
 *
 * Available variables:
 * - $list_links: Links to the other list views.
 * - $column_count: How many columns to print.
 * - $objects: An array keyed on PIDs contianing an array of markup pieces.
 */
?>
<div>
  <?php print($list_links); ?>
  <?php foreach(range(1, $column_count) as $column): ?>
    <div class="islandora-bookmark-list-column">
      <?php if (isset($objects[$column])): ?>
        <?php foreach($objects[$column] as $pid => $object_markup): ?>
          <div>
            <?php foreach($object_markup as $markup): ?>
              <?php print $markup; ?>
            <?php endforeach; ?>
          </div>
        <?php endforeach; ?>
      <?php endif ?>
    </div>
  <?php endforeach; ?>
</div>
