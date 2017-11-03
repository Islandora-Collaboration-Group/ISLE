<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_basic_image_admin(array $form, array &$form_state) {
  $form['islandora_basic_image_upscale_images'] = array(
    '#type' => 'checkbox',
    '#title' => t('Upscale images for MEDIUM_SIZE derivatives?'),
    '#description' => t('If selected will cause images that are smaller than 500x700 to be upscaled.'),
    '#default_value' => variable_get('islandora_basic_image_upscale_images', TRUE),
  );
  return system_settings_form($form);
}
