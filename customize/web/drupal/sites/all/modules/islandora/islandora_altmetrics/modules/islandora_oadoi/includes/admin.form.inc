<?php
/**
 * @file
 * oaDOI badge admin options
 */

/**
 * Admin form: configure PDF link text.
 */
function islandora_oadoi_admin_form($form, $form_settings) {
  $form['islandora_oadoi_linktext'] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#description' => t('Text to use when generating external download link.'),
    '#default_value' => variable_get('islandora_oadoi_linktext', 'Link to PDF'),
  );
  return system_settings_form($form);
}
