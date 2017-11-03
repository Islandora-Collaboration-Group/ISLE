<?php
/**
 * @file
 * Displays the newspaper page controls.
 */
?>
<div class="islandora-newspaper-controls">
  <?php print theme('item_list', array('items' => $controls, 'attributes' => array('class' => array('items', 'inline')))); ?>
</div>
