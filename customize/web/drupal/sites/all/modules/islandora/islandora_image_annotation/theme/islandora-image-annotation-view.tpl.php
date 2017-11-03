<?php

/**
 * @file
 * The container for the Image Annotations, displayed as a single tab.
 */
?>
<div class="islandora-anno-wrapper">
  <div class="colmask">
    <div class="colleft">
      <!-- Tabs -->
      <div id="tabs">
        <ul>
          <li><a href="#image-annotations">Image Annotations</a></li>
        </ul>
        <?php print $anno_list_pane; ?>
      </div>
    </div>
    <div id="colright" class="colright">
      <button id="create_annotation" class="menu_button">Annotate</button>
      <button id="full-window-button" class="menu_button"><?php print t('Full Window'); ?></button>
      <div class="image-annotation-wrapper">
        <!-- Persist a single player and build new interface to it -->
        <div id="canvas-body-wrapper">
          <?php print $anno_img_pane; ?>
        </div>
      </div>
    </div>
  </div>
</div>
