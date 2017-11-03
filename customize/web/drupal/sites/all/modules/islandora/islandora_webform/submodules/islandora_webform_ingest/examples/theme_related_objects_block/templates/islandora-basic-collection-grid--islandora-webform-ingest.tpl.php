<?php

/**
 * @file
 * islandora-basic-collection.tpl.php
 *
 * @TODO: needs documentation about file and variables
 */
$alter = array(
  'max_length' => 100,
  'ellipsis' => TRUE,
  'word_boundary' => TRUE,
  'html' => TRUE,
);
?>

<div class="islandora islandora-basic-collection">
  <div class="islandora-basic-collection-grid clearfix">
  <?php foreach($associated_objects_array as $key => $value): ?>
    <dl class="islandora-basic-collection-object <?php print $value['class']; ?>">
        <dt class="islandora-basic-collection-thumb"><?php print $value['thumb_link']; ?></dt>
      <dd class="islandora-basic-collection-caption"><?php print filter_xss($value['title_link']); ?></dd>
      <?php if (!empty($value['solr_data']['mods_abstract_ms'][0])) : ?>
      <dd class="islandora-mods-abstract"><?php print filter_xss(views_trim_text($alter, $value['solr_data']['mods_abstract_ms'][0])); ?></dd>
      <?php endif; ?>
    </dl>
  <?php endforeach; ?>
</div>
</div>
