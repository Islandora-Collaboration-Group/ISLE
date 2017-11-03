<?php
/**
 * @file
 * Template for the Tags Element.
 */
?>
<div class="xml-form-elements-tags">
  <?php print $textfield; ?>
  <input type="image" src="<?php print $image_path ?>add.png" name="<?php print $add['#name'] ?>" id="<?php print $add['#id'] ?>" value="add">
  <div class="hidden-controls">
    <?php 
      print ((is_array($edit) && array_key_exists('#children', $edit)) ? $edit['#children'] : '');
      print ((is_array($remove) && array_key_exists('#children', $remove)) ? $remove['#children'] : '');
  ?>
  </div>
  <div class="hidden-tags">
    <?php foreach ($tags as $tag): ?>
      <input type="hidden" name="<?php print $tag['#name']; ?>" id="<?php print $tag['#id']; ?>" class="form-tag" value="<?php print "{$tag['#value']}"; ?>"/>
    <?php endforeach ?>
  </div>
  <span class="tag-list">
    <?php foreach ($tags as $tag): ?>
      <span title="<?php print "{$tag['#value']}" ?>">
        <span class="edit-tag" onclick="jQuery('#<?php print $edit[$tag['#hash']]['#id'] ?>').trigger('mousedown'); return false;"><?php print check_plain("{$tag['#value']}") ?></span>
        <span class="remove-tag" onclick="jQuery('#<?php print $remove[$tag['#hash']]['#id'] ?>').trigger('mousedown'); return false;"></span>
      </span>
    <?php endforeach ?>
  </span>
</div>
