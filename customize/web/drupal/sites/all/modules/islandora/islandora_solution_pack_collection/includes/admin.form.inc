<?php

/**
 * @file
 * Handles the display and submission of this modules admin form.
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
function islandora_basic_collection_admin(array $form, array &$form_state) {
  $backend_options = module_invoke_all('islandora_basic_collection_query_backends');
  $map_to_title = function ($backend) {
    return $backend['title'];
  };

  $form = array(
    // Display options.
    'display_generation_fieldset' => array(
      '#type' => 'fieldset',
      '#title' => t('Display Generation'),
      '#states' => array(
        'invisible' => array(
          ':input[name="islandora_basic_collection_disable_display_generation"]' => array('checked' => TRUE),
        ),
      ),
      'islandora_collection_display' => array(
        'islandora_basic_collection_page_size' => array(
          '#type' => 'textfield',
          '#title' => t('Default collection objects per page'),
          '#default_value' => variable_get('islandora_basic_collection_page_size', '12'),
          '#description' => t('The default number of objects to show in a collection view.'),
        ),
        'islandora_basic_collection_disable_count_object' => array(
          '#type' => 'checkbox',
          '#title' => t('Disable object count query in collection overview'),
          '#default_value' => variable_get('islandora_basic_collection_disable_count_object', FALSE),
          '#description' => t("Disabling the object count query can improve performance when loading the overview for large collections."),
        ),
        'islandora_basic_collection_default_view' => array(
          '#type' => 'select',
          '#title' => t('Default collection view style.'),
          '#default_value' => variable_get('islandora_basic_collection_default_view', 'grid'),
          '#options' => array(
            'list' => t('List'),
            'grid' => t('Grid'),
          ),
        ),
        'islandora_basic_collection_display_backend' => array(
          '#type' => 'radios',
          '#title' => t('Display Generation'),
          '#options' => array_map($map_to_title, $backend_options),
          '#default_value' => variable_get('islandora_basic_collection_display_backend', ISLANDORA_BASIC_COLLECTION_LEGACY_BACKEND),
        ),
      ),
    ),
    'islandora_basic_collection_disable_display_generation' => array(
      '#type' => 'checkbox',
      '#title' => t('Completely disable default collection display generation.'),
      '#default_value' => variable_get('islandora_basic_collection_disable_display_generation', FALSE),
      '#description' => t("Disabling display generation allows for alternate collection displays to be used."),
    ),
    'islandora_basic_collection_admin_page_size' => array(
      '#type' => 'textfield',
      '#title' => t('Objects per page during collection management'),
      '#default_value' => variable_get('islandora_basic_collection_admin_page_size', '10'),
      '#description' => t('The number of child objects to show per page in the migrate/share/delete interface.'),
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
    ),
    'islandora_basic_collection_disable_collection_policy_delete' => array(
      '#type' => 'checkbox',
      '#title' => t('Disable deleting the collection policy'),
      '#default_value' => variable_get('islandora_basic_collection_disable_collection_policy_delete', TRUE),
      '#description' => t("Disables the 'delete' link for the COLLECTION_POLICY datastream."),
    ),
    // Metadata display.
    'metadata_display_fieldset' => array(
      '#type' => 'fieldset',
      '#title' => t('Metadata display'),
      'islandora_collection_metadata_display' => array(
        '#type' => 'checkbox',
        '#title' => t('Display object metadata'),
        '#description' => t('Display object metadata below the collection display.'),
        '#default_value' => variable_get('islandora_collection_metadata_display', FALSE),
      ),
    ),
  );

  // Define the elements that appear on a collection objects display page.
  // The key's match up with the form elements array keys.
  $page_content = array(
    'description' => array(
      'name' => t("Description"),
      'description' => t("An objects description field"),
    ),
    'collections' => array(
      'name' => t("In Collection"),
      'description' => t("Indicates which collections this object belongs to"),
    ),
    'wrapper' => array(
      'name' => t("Fieldset Metadata"),
      'description' => t("An objects metadata collection set"),
    ),
    'islandora_basic_collection_display' => array(
      'name' => t("Object Content"),
      'description' => t("Main object page content, such as configured viewers"),
    ),
  );

  $form['metadata_display_fieldset']['islandora_basic_collection_metadata_info_table_drag_attributes'] = array(
    '#theme_wrappers' => array('fieldset'),
    '#tree' => TRUE,
    '#title' => t("Page content placement"),
    '#theme' => 'islandora_basic_collection_metadata_table_drag_components',
    '#description' => t('Use the table below to determine the rendering order of page and metadata content.'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_basic_collection_display_backend"]' => array('!value' => ISLANDORA_BASIC_COLLECTION_LEGACY_BACKEND),
        ':input[name="islandora_collection_metadata_display"]' => array('checked' => TRUE),
      ),
    ),
  );

  $config = variable_get('islandora_basic_collection_metadata_info_table_drag_attributes', array());

  foreach ($page_content as $key => $data) {
    $form['metadata_display_fieldset']['islandora_basic_collection_metadata_info_table_drag_attributes'][$key] = array();
    $element =& $form['metadata_display_fieldset']['islandora_basic_collection_metadata_info_table_drag_attributes'][$key];
    if (!isset($config[$key])) {
      $config[$key] = array();
    }
    $config[$key] += array(
      'weight' => 0,
      'omit' => 0,
    );

    $element['#weight'] = $config[$key]['weight'];
    $element['label'] = array(
      '#type' => 'item',
      '#markup' => $data['name'],
    );
    $element['textfield'] = array(
      '#type' => 'item',
      '#markup' => $data['description'],
    );
    $element['weight'] = array(
      '#type' => 'textfield',
      '#default_value' => $element['#weight'],
      '#size' => 3,
      '#attributes' => array(
        'class' => array(
          'item-row-weight',
        ),
      ),
    );
    $element['omit'] = array(
      '#type' => 'checkbox',
      '#default_value' => $config[$key]['omit'],
      '#attributes' => array(
        'class' => array(
          'item-row-weight',
        ),
        'title' => t('Hide the selected element from display, marking it as an invisible element in the DOM.'),
      ),
    );
  }

  return system_settings_form($form);
}
