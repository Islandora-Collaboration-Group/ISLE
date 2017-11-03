<?php

/**
 * @file
 * The dialog box used to create new annotations.
 */
?>
<div id="islandora-image-annotation-dialog" style="display: none;">
  <div class="icons">
    <?php print $annotation_icons; ?>
  </div>
  <hr/>
  <label><?php print t('Title:'); ?></label>
  <?php print $title; ?>
  <label><?php print t('Type:'); ?></label>
  <?php print $type; ?>
  <?php if ($use_entity_tagging): ?>
    <div class="entity-type">
      <label><?php print t('Entity'); ?></label>
      <select name="entity_type" style="width:100px;">
        <!-- We don't translate the text of these options as it's posted to the
            entities module and used to control what search is performed. Yuck. -->
        <option value="person"><?php print 'Tag Person'; ?></option>
        <option value="event"><?php print 'Tag Event'; ?></option>
        <option value="place"><?php print 'Tag Place'; ?></option>
        <option value="organization"><?php print 'Tag Organization'; ?></option>
      </select>
      <div class="ui-widget">
        <input name="entity"/>
      </div>
      <!-- Stores the PID -->
      <input name="entity_id" type="hidden"/>
    </div>
  <?php endif; ?>
  <div class="color">
    <label><?php print t('Color:'); ?></label>
    <input name="color" type="hidden" value="#91843c" class="color-picker" size="7" />
  </div>
  <div class="stroke">
    <label><?php print t('Stroke Width:'); ?></label>
    <?php print $stroke_width; ?>
  </div>
  <label><?php print t('Annotation:'); ?></label>
  <textarea name="text" cols="40" rows="5"></textarea>
  <!-- The Canvas this annotation belongs to. -->
  <input name="canvas" type="hidden" />
  <!-- The Shape to use. -->
  <input name="shape" type="hidden" />
</div>