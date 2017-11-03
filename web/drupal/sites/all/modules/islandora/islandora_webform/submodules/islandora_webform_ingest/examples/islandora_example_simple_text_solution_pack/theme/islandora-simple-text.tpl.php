<?php

/**
 * @file
 * This is the template file for the object page for example simple text
 *
 * @TODO: add documentation about file and available variables
 */
?>

<div class="islandora-example-simple-text-object islandora" vocab="http://schema.org/" prefix="dcterms: http://purl.org/dc/terms/" typeof="TextObject">
  <div class="islandora-example-simple-text-object-content-wrapper clearfix">
    <?php if (isset($islandora_content)): ?>
      <div class="islandora-example-simple-text-content">
        <?php print $islandora_content; ?>
      </div>
    <?php endif; ?>
  </div>
  <div class="islandora-example-simple-text-metadata">
    <?php print $description; ?>
    <?php if (isset($html_description)): ?>
      <?php print $html_description; ?>
    <?php endif; ?> 
    <?php if ($parent_objects): ?>
      <div>
        <h2><?php print t('Related to'); ?></h2>
        <ul>
          <?php foreach ($parent_objects as $parent_object): ?>
            <li><?php print l($parent_object->label, "islandora/object/{$parent_object->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
    <?php print $metadata; ?>
    <?php if (isset($datastreams)): ?>
    <fieldset class="collapsible collapsed" style="display: block; clear:both">
    <legend><span class="fieldset-legend"><?php print t('File details'); ?></span></legend>
      <div class="fieldset-wrapper">
    <table>
      <tr>
        <th><?php print t('ID'); ?></th>
        <th><?php print t('Label'); ?></th>
        <th><?php print t('Size'); ?></th>
        <th><?php print t('Mimetype'); ?></th>
        <th><?php print t('Created'); ?></th>
      </tr>
      <?php foreach($datastreams as $key => $value): ?>
      <tr>
          <td><?php if(isset($value['id'])): ?><?php print $value['id']; ?><?php endif; ?></td>
          <td><?php if(isset($value['label_link'])): ?><?php print $value['label_link']; ?><?php endif; ?></td>
          <td><?php if(isset($value['size'])): ?><?php print $value['size']; ?><?php endif; ?></td>
          <td><?php if(isset($value['mimetype'])): ?><?php print $value['mimetype']; ?><?php endif; ?></td>
          <td><?php if(isset($value['created_date'])): ?><?php print $value['created_date']; ?><?php endif; ?></td>
      </tr>
      <?php endforeach; ?>
    </table>
    </div>
  </fieldset>
  <?php endif; ?>
  </div>
</div>
