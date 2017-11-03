<?php
/**
 * @file
 * Holds the administration form for Islandora Entities.
 */

/**
 * This is the administration form for islandora_entities.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_entities_settings($form, &$form_state) {

  $form['islandora_entities_query_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Entity searching Solr field.'),
    '#default_value' => variable_get(
      'islandora_entities_query_field',
      'eaccpf_name_complete_et'
    ),
    '#description' => t('Solr Field for entity searching.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => TRUE,
  );

  $form['islandora_entities_citation_base_query'] = array(
    '#type' => 'textarea',
    '#title' => t('Solr base query for citations.'),
    '#default_value' => variable_get(
      'islandora_entities_citation_base_query',
      '+RELS_EXT_hasModel_uri_mt:"ir:citationCModel"'),
    '#description' => t('Solr query for citation searching.'),
    '#required' => TRUE,
  );

  $form['islandora_entities_thesis_base_query'] = array(
    '#type' => 'textarea',
    '#title' => t('Solr base query for theses.'),
    '#default_value' => variable_get(
      'islandora_entities_thesis_base_query',
      '+RELS_EXT_hasModel_uri_mt:"ir:thesisCModel"'),
    '#description' => t('Solr query for thesis searching.'),
    '#required' => TRUE,
  );

  $form['islandora_entities_people_collection'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection to ingest person entities in.'),
    '#default_value' => variable_get(
      'islandora_entities_people_collection',
      'islandora:entity_collection'
    ),
    '#description' => t('Used by dependent modules provide links to the collection.'),
    '#required' => TRUE,
  );

  $form['islandora_entities_events_collection'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection to ingest event entities in.'),
    '#default_value' => variable_get(
      'islandora_entities_events_collection',
      'islandora:entity_collection'
    ),
    '#description' => t('Used by dependent modules provide links to the collection.'),
    '#required' => TRUE,
  );

  $form['islandora_entities_organizations_collection'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection to ingest organization entities in.'),
    '#default_value' => variable_get(
      'islandora_entities_organizations_collection',
      'islandora:entity_collection'
    ),
    '#description' => t('Used by dependent modules provide links to the collection.'),
    '#required' => TRUE,
  );

  $form['islandora_entities_places_collection'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection to ingest place entities in.'),
    '#default_value' => variable_get(
      'islandora_entities_places_collection',
      'islandora:entity_collection'
    ),
    '#description' => t('Used by dependent modules provide links to the collection.'),
    '#required' => TRUE,
  );

  $form['islandora_entities_scholar_title_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for scholar title.'),
    '#default_value' => variable_get(
      'islandora_entities_scholar_title_solr_field',
      'MADS_title_mt'
    ),
    '#description' => t('Solr field for autocompletion.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => TRUE,
  );

  $form['islandora_entities_department_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for department.'),
    '#default_value' => variable_get(
      'islandora_entities_department_solr_field',
      'MADS_department_mt'
    ),
    '#description' => t('Solr field for autocompletion.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => TRUE,
  );

  $form['islandora_entities_disambiguated_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for disambiguated full name.'),
    '#default_value' => variable_get(
      'islandora_entities_disambiguated_solr_field',
      'MADS_disambiguated_fullname_mt'
    ),
    '#description' => t('Solr field for autocompletion.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => TRUE,
  );

  $form['islandora_entities_last_name_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for last name.'),
    '#default_value' => variable_get(
      'islandora_entities_last_name_solr_field',
      'MADS_family_mt'
    ),
    '#description' => t('Solr field for autocompletion.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => TRUE,
  );

  $form['islandora_entities_query_sort'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field to sort citations and theses by.'),
    '#default_value' => variable_get(
      'islandora_entities_query_sort',
      ''
    ),
    '#description' => t('Used to sort citations and theses on scholar profile pages. Should contain the solr field followed by a space and either "asc" or "desc". For example, "mods_originInfo_keyDate_yes_dateIssued_dt asc" will sort on the dateIssued field in ascending order.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => FALSE,
  );

  $form['islandora_entities_citation_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Citations Display'),
    '#default_value' => variable_get(
      'islandora_entities_citation_number',
      '20'
    ),
    '#description' => t('Number of Citations to show under Recent Citations/Citations Tab'),
    '#required' => TRUE,
  );

  $form['islandora_entities_citation_author_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for author of citations and theses.'),
    '#default_value' => variable_get(
      'islandora_entities_citation_author_solr_field',
      'mods_name_personal_displayForm_mt'
    ),
    '#description' => t('Solr field to display as author under the Citation and Theses tabs.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => TRUE,
  );

  $form['islandora_entities_linking_content_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr field for linking content to person entities.'),
    '#default_value' => variable_get(
      'islandora_entities_linking_content_solr_field',
      'mods_name_personal_displayForm_mt'
    ),
    '#description' => t('Solr field used to link citations and theses to person entities.'),
    '#autocomplete_path' => 'islandora_solr/autocomplete_luke',
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
