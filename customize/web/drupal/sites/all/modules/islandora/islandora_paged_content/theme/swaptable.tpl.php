<?php
/**
 * @file
 */
?>
<div id='<?php print $element['#id']; ?>'>
  <div class="swaptable-display-select">
    <?php print drupal_render($element['display']); ?>
  </div>
  <table class="swaptable-wrapper">
    <tbody>
      <tr>
        <td valign="top" style="vertical-align: top;">
          <?php print theme('table', $table['left']); ?>
          <?php print theme('pager', $pager['left']); ?>
          <?php print drupal_render($element['page']['left']); ?>
        </td>
        <td valign="top" style="vertical-align: top;">
          <?php print theme('table', $table['right']); ?>
          <?php print theme('pager', $pager['right']); ?>
          <?php print drupal_render($element['page']['right']); ?>
        </td>
      </tr>
    </tbody>
  </table>
  <?php print drupal_render($element['order']); ?>
  <?php print drupal_render($element['modified']); ?>
  <?php print drupal_render($element['load']); ?>
</div>
