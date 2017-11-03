<?php

/**
 * @file
 * Template file for range slider.
 *
 * @TODO: complete doxygen doc.
 */
?>

<div class="range-slider-fromto" id="range-slider-fromto-<?php print $form_key; ?>">
  <span class="range-slider-from">
    <?php print $range_from; ?>
  </span>
   - 
  <span class="range-slider-to">
    <?php print $range_to; ?>
  </span>
</div>
<div class="date-range-slider-gap">
  <?php print $gap; ?>
</div>
<div id="slider-amount-<?php print $form_key; ?>"></div>
<div id="date-range-slider-canvas-<?php print $form_key; ?>" class="date-range-slider-canvas-wrapper"></div>
<div id="date-range-slider-<?php print $form_key; ?>"></div>
