<?php
/**
 * @file
 * Adminstration settings for the Islandora MARCXML module.
 */

/**
 * Adminstration form for the Islandora MARCXML module.
 */
function islandora_marcxml_admin_settings(array $form, array &$form_state) {
  $form['islandora_marcxml_allow_editing_of_existing_mods'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user to upload a new copy of the MARCXML to be re-transformed and stored as MODS.'),
    '#description' => t("This provides an extra option to be available under the 'edit' link present on objects that have MODS datastreams."),
    '#default_value' => variable_get('islandora_marcxml_allow_editing_of_existing_mods', FALSE),
  );
  return system_settings_form($form);
}
