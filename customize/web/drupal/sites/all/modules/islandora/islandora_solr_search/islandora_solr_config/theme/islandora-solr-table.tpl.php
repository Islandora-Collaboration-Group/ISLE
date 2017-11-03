<?php
/**
 * @file
 * Islandora solr table template
 *
 * Variables available:
 * - $header: Header results
 * - $rows: Row results
 * - $elements: Optional elements
 *
 * @see template_preprocess_islandora_solr_grid()
 */
?>

<?php if (empty($rows)): ?>
  <p class="no-results"><?php print t('Sorry, but your search returned no results.'); ?></p>
<?php else: ?>
  <table class="<?php print variable_get('islandora_solr_table_profile_table_class', ''); ?>">
    <?php foreach($header as $key => $value):?>
      <th>
        <?php print $value; ?>
      </th>
    <?php endforeach; ?>
    <?php foreach($rows as $key => $row):?>
      <tr>
      <?php foreach($row as $key => $value):?>
        <td>
          <?php print $value; ?>
        </td>
      <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </table>
<?php endif; ?>
