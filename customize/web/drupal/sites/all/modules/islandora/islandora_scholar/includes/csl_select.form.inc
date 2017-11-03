<?php

/**
 * @file
 * Form for selecting CSL to display.
 */

/**
 * A select for selecting which CSL to use.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param string $pid
 *   The PID of the object the citation will be generated for.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_scholar_citation_select_form(array $form, array &$form_state, $pid) {
  if (variable_get('islandora_scholar_users_choose_display_csl', FALSE)) {
    return array(
      'citation_select' => array(
        '#type' => 'select',
        '#options' => CSL::GetNames(),
        '#default_value' => CSL::GetDefaultName(),
        '#title' => t('Style'),
        '#description' => t('Choose the citation style.'),
        '#attached' => array(
          'js' => array(
            drupal_get_path('module', 'islandora_scholar') . '/js/citation_select.js',
            array(
              'type' => 'setting',
              'data' => array('islandora_scholar' => array('pid' => $pid)),
            ),
          ),
        ),
        '#attributes' => array(
          'class' => array('islandora-scholar-citation-select'),
        ),
      ),
    );
  }
}
