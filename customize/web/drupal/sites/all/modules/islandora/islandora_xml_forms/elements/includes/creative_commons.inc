<?php

/**
 * @file
 * Code for creative commons form elements.
 */

/**
 * Creates a creative_commons form element.
 *
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The element definition.
 */
function xml_form_elements_creative_commons($element, &$form_state) {
  $modification_options = array(
    'y' => t('Yes'),
    'n' => t('No'),
    'sa' => t('Yes, as long as others share alike'),
  );

  $countries = array(
    'international' => t('International'),
    'ar' => t('Argentina'),
    'au' => t('Australia'),
    'at' => t('Austria'),
    'be' => t('Belgium'),
    'br' => t('Brazil'),
    'bg' => t('Bulgaria'),
    'ca' => t('Canada'),
    'cl' => t('Chile'),
    'cn' => t('China Mainland'),
    'co' => t('Colombia'),
    'cr' => t('Costa Rica'),
    'hr' => t('Croatia'),
    'cz' => t('Czech Republic'),
    'dk' => t('Denmark'),
    'ec' => t('Ecuador'),
    'eg' => t('Egypt'),
    'ee' => t('Estonia'),
    'fi' => t('Finland'),
    'fr' => t('France'),
    'de' => t('Germany'),
    'gr' => t('Greece'),
    'gt' => t('Guatemala'),
    'hk' => t('Hong kong'),
    'hu' => t('Hungary'),
    'in' => t('India'),
    'ie' => t('Ireland'),
    'il' => t('Israel'),
    'it' => t('Italy'),
    'jp' => t('Japan'),
    'lu' => t('Luxembourg'),
    'mk' => t('Macedonia'),
    'my' => t('Malaysia'),
    'mt' => t('Malta'),
    'mx' => t('Mexico'),
    'nl' => t('Netherlands'),
    'nz' => t('New Zealand'),
    'no' => t('Norway'),
    'pe' => t('Peru'),
    'ph' => t('Philippines'),
    'pl' => t('Poland'),
    'pt' => t('Portugal'),
    'pr' => t('Puerto Rico'),
    'ro' => t('Romania'),
    'rs' => t('Serbia'),
    'sg' => t('Singapore'),
    'si' => t('Slovenia'),
    'za' => t('South Africa'),
    'kr' => t('South Korea'),
    'es' => t('Spain'),
    'se' => t('Sweden'),
    'ch' => t('Switzerland'),
    'tw' => t('Taiwan'),
    'th' => t('Thailand'),
    'uk' => t('UK: England & Wales'),
    'scotland' => t('Uk: Scotland'),
    'ug' => t('Uganda'),
    'us' => t('United states'),
    'vn' => t('Vietnam'),
  );

  $commercial_options = array(
    'y' => t('Yes'),
    'n' => t('No'),
  );

  // Get an ID for ajax.
  if (isset($form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'])) {
    $license_output_id = $form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'];
  }
  else {
    $license_output_id = drupal_html_id('license_output');
    $form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'] = $license_output_id;
  }

  // Figure out license state.
  if (!isset($form_state['input'][$element['#name']]) && isset($element['#default_value'])) {
    // Reversing this array facilitates value_callback mangling by community.
    $default_value_array = array_reverse(explode('/', $element['#default_value']));
    $properties = explode('-', array_pop($default_value_array));

    $commercial = 'y';
    $derivatives = 'y';

    foreach ($properties as $property) {
      switch ($property) {
        case 'by':
          break;

        case 'nc':
          $commercial = 'n';
          break;

        case 'nd':
          $derivatives = 'n';
          break;

        case 'sa':
          $derivatives = 'sa';
          break;
      }
    }

    $jurisdiction = empty($default_value_array[1]) ? 'international' : $default_value_array[1];
  }
  else {
    $derivatives = isset($form_state['input'][$element['#name']]['license_fieldset']['allow_modifications']) ? $form_state['input'][$element['#name']]['license_fieldset']['allow_modifications'] : 'y';
    $commercial = isset($form_state['input'][$element['#name']]['license_fieldset']['allow_commercial']) ? $form_state['input'][$element['#name']]['license_fieldset']['allow_commercial'] : 'y';
    $jurisdiction = isset($form_state['input'][$element['#name']]['license_fieldset']['license_jurisdiction']) ? $form_state['input'][$element['#name']]['license_fieldset']['license_jurisdiction'] : 'ca';
  }

  // Form elements.
  $element['license_fieldset'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#title' => t('License'),
  );

  $element['license_fieldset']['allow_modifications'] = array(
    '#type' => 'select',
    '#title' => t('Allow modifications of your work?'),
    '#options' => $modification_options,
    '#default_value' => $derivatives,
    '#ajax' => array(
      'wrapper' => $license_output_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );

  $element['license_fieldset']['allow_commercial'] = array(
    '#type' => 'select',
    '#title' => t('Allow commercial uses of your work?'),
    '#options' => $commercial_options,
    '#default_value' => $commercial,
    '#ajax' => array(
      'wrapper' => $license_output_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );

  $element['license_fieldset']['license_jurisdiction'] = array(
    '#type' => 'select',
    '#title' => t('License Jurisdiction'),
    '#options' => $countries,
    '#default_value' => $jurisdiction,
    '#ajax' => array(
      'wrapper' => $license_output_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );
  // Value gets populated if default value is populated.
  if (current_path() == 'system/ajax' || !$element['#value'] ||
    (isset($element['#default_value']) && $element['#value'] == $element['#default_value'])) {
    $element['#tree'] = TRUE;
  }
  else {
    // I win form builder.
    $element['#tree'] = FALSE;
  }

  $response = xml_form_elements_get_creative_commons($commercial, $derivatives, $jurisdiction);
  if ($response) {
    $form_state['storage']['xml_form_elements'][$element['#name']]['license_uri'] = (string) $response->{'license-uri'};
    $element['license_fieldset']['license_output'] = array(
      '#type' => 'item',
      '#id' => $license_output_id,
      '#markup' => '<strong>' . t('Selected license:') . '</strong><div>' . $response->html->asXml() . '</div>',
    );
  }
  else {
    $form_state['storage']['xml_form_elements'][$element['#name']]['license_uri'] = xml_form_elements_creative_commons_value($commercial, $derivatives, $jurisdiction);
    $element['license_fieldset']['license_output'] = array();
  }

  return $element;
}

/**
 * Ajax callback to render the CC license.
 */
function xml_form_elements_creative_commons_ajax(&$form, &$form_state) {
  $parents = $form_state['triggering_element']['#parents'];
  array_pop($parents);
  $creative_commons_element = drupal_array_get_nested_value($form, $parents);

  return $creative_commons_element['license_output'];
}

/**
 * Ask the creative commons REST API for a license.
 *
 * Left as a function for use by community modules.
 *
 * @param string $commercial
 *   'y' to allow 'n' to disallow.
 * @param string $derivatives
 *   'y' to allow 'n' to disallow.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 *
 * @return mixed
 *   SimpleXMLElement the return from the REST API.
 *   FALSE if failed request.
 */
function xml_form_elements_get_creative_commons($commercial, $derivatives, $jurisdiction) {
  $response = drupal_http_request(url(
    'http://api.creativecommons.org/rest/1.5/license/standard/get',
    array(
      'query' => array(
        'commercial' => $commercial,
        'derivatives' => $derivatives,
        'jurisdiction' => $jurisdiction,
      ),
    )
  ));
  if ($response->code != 200) {
    return FALSE;
  }
  return simplexml_load_string($response->data, 'SimpleXMLElement');
}

/**
 * Gets the value for the element.
 *
 * @param string $commercial
 *   'y' to allow 'n' to disallow.
 * @param string $derivatives
 *   'y' to allow 'n' to disallow.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 *
 * @return string
 *   The value of the element.
 */
function xml_form_elements_creative_commons_value($commercial, $derivatives, $jurisdiction) {
  $arguments = '';
  if ($commercial == 'n') {
    $arguments = "$arguments-nc";
  }
  if ($derivatives == 'n') {
    $arguments = "$arguments-nd";
  }
  elseif ($derivatives == 'sa') {
    $arguments = "$arguments-sa";
  }
  return "http://creativecommons.org/licenses/by$arguments/2.5/$jurisdiction/";
}
