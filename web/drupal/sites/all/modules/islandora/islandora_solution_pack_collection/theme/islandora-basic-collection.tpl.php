<?php

/**
 * @file
 * islandora-basic-collection.tpl.php
 *
 * @TODO: needs documentation about file and variables
 */
?>

<div class="islandora islandora-basic-collection">
    <?php $row_field = 0; ?>
    <?php foreach($associated_objects_array as $associated_object): ?>
      <div class="islandora-basic-collection-object islandora-basic-collection-list-item clearfix">
        <dl class="<?php print $associated_object['class']; ?>">
            <dt>
              <?php if (isset($associated_object['thumb_link'])): ?>
                <?php print $associated_object['thumb_link']; ?>
              <?php endif; ?>
            </dt>
            <dd class="collection-value <?php print isset($associated_object['dc_array']['dc:title']['class']) ? $associated_object['dc_array']['dc:title']['class'] : ''; ?> <?php print $row_field == 0 ? ' first' : ''; ?>">
              <?php if (isset($associated_object['thumb_link'])): ?>
                <strong><?php print filter_xss($associated_object['title_link']); ?></strong>
              <?php endif; ?>
            </dd>
            <?php if (isset($associated_object['dc_array']['dc:description']['value'])): ?>
              <dd class="collection-value <?php print $associated_object['dc_array']['dc:description']['class']; ?>">
                <?php print filter_xss($associated_object['dc_array']['dc:description']['value']); ?>
              </dd>
            <?php endif; ?>
        </dl>
      </div>
    <?php $row_field++; ?>
    <?php endforeach; ?>
</div>
