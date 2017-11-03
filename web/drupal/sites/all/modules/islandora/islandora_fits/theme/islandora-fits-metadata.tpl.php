<?php

/**
 * @file
 * This is a template used to display the technical metadata that
 * is present in the TECHMD datastream.
 *
 * islandora_object is a fedora tuque Object. Additional documentation
 * can be gathered from the islandora-object.tpl.php present in the Islandora
 * module.
 *
 * islandora_fits_table contains the required table definitions needed
 * in rendering a table. It contains the header fields as well as the data
 * to be placed in rows for each individual tool that it encounters.
 *
 * islandora_fits_fieldsets contains fieldset markup to structure the output
 * of the individual tables.
 */
?>

<?php drupal_set_title(t('@label Technical Metadata', array('@label' => $variables['islandora_object']->label))); ?>

<?php foreach ($variables['islandora_fits_table'] as $name => $table): ?>
  <?php print drupal_render($variables['islandora_fits_fieldsets'][$name]); ?>
<?php endforeach; ?>
