<?php

/**
 * @file
 * Callbacks and functions used in the Edit Form Page.
 *
 * These are used for rendering the Form Builder GUI.
 */

/**
 * Includes all the required CSS/JS files needed to render the Form Builder GUI.
 *
 * @param string $form_name
 *   The name of the form to edit.
 *
 * @return string
 *   The html where the Form Builder GUI will be rendered.
 */
function xml_form_builder_edit($form_name) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');

  if (!XMLFormDatabase::Exists($form_name)) {
    drupal_set_message(t('Form "%name" does not exist.', array('%name' => $form_name)), 'error');
    drupal_not_found();
    exit();
  }

  xml_form_builder_edit_include_css();
  xml_form_builder_edit_include_js();
  xml_form_builder_create_element_type_store();
  xml_form_builder_create_element_store($form_name);
  xml_form_builder_create_properties_store($form_name);
  return '<div id="xml-form-builder-editor"></div>';
}

/**
 * Includes all the required CSS files needed to render the Form Builder GUI.
 */
function xml_form_builder_edit_include_css() {
  $path = drupal_get_path('module', 'xml_form_builder');
  $ext_path = $path . '/lib/ext/';
  $css_path = $path . '/css/';
  $css_files = array(
    $css_path . 'Clear.css',
    $css_path . 'EditForm.css',
    $ext_path . 'resources/css/ext-all.css',
  );
  foreach ($css_files as $file) {
    drupal_add_css($file);
  }
}

/**
 * Includes all the required JS files needed to render the Form Builder GUI.
 */
function xml_form_builder_edit_include_js() {
  $path = drupal_get_path('module', 'xml_form_builder');
  $ext_path = $path . '/lib/ext/';
  $js_path = $path . '/js/';
  $js_files = array(
    $ext_path . 'ext-all.js',
    $js_path . 'Formbuilder.js',
    $js_path . 'FormGrid.js',
    $js_path . 'Tooltips.js',
    $js_path . 'ElementModel.js',
    $js_path . 'PropertiesModel.js',
    $js_path . 'MainPanel.js',
    $js_path . 'DisplayPanel.js',
    $js_path . 'TreePanel.js',
    $js_path . 'ElementForm.js',
    $js_path . 'PropertiesForm.js',
    $js_path . 'PreviewPanel.js',
  );
  foreach ($js_files as $file) {
    drupal_add_js($file);
  }
}

/**
 * Save changes made to the form definition client side.
 *
 * Transforms the submited JSON into a Form Definition which is then stored in
 * the database as an XML Form Definition.
 *
 * @param string $form_name
 *   The name of the form to update.
 *
 * @throws Exception
 *   If unable to instantiate the JSON form definition, or generate the XML form
 *   definition.
 */
function xml_form_builder_edit_save($form_name) {
  module_load_include('inc', 'xml_form_builder', 'JSONFormDefinition');
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
  try {
    // @TODO: this data needs to be sanitized. Can we get this data through the
    // form API?
    $definition = new JSONFormDefinition($_POST['data']);
    list($properties, $form) = $definition->getPropertiesAndForm();
    $definition = XMLFormDefinitionGenerator::Create($properties, $form);
    XMLFormDatabase::Update($form_name, $definition);
  }
  catch (Exception $e) {
    $msg = "File: {$e->getFile()}<br/>Line: {$e->getLine()}<br/>Error: {$e->getMessage()}";
    drupal_set_message(filter_xss($msg), 'error');
  }
}

/**
 * Creates an ExtJS store containing all the possible form field types.
 *
 * Uses inline javascript to create the ExtJS store.
 *
 * Used in the Element Type Combo Box, in the Element form of the Form Builder
 * GUI.
 */
function xml_form_builder_create_element_type_store() {
  $types = xml_form_builder_get_element_types();
  $data = array();
  foreach ($types as $type) {
    $data[] = array(
      'display' => $type,
      'value' => $type,
    );
  }
  $data = json_encode($data);
  $js = <<<EOT
  Ext.formbuilder.elementTypeStore = Ext.data.Store({
    storeId: 'ElementTypes',
    fields: ['display', 'value'],
    proxy: {
      type: 'memory',
      reader: {
        type: 'json'
      },
      writer: {
        type: 'json'
      }
    },
    data: $data
  });
EOT;
  drupal_add_js($js, array('type' => 'inline', 'scope' => JS_DEFAULT));
}

/**
 * Creates an ExtJS store containing the form properties.
 *
 * Uses inline javascript to create the ExtJS store.
 *
 * Used to populate the 'Form Properties' Form.
 *
 * @param string $form_name
 *   The name of the form which properties will be used.
 */
function xml_form_builder_create_properties_store($form_name) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  module_load_include('inc', 'xml_form_builder', 'JSONFormDefinitionGenerator');
  module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
  $definition = new XMLFormDefinition(XMLFormDatabase::Get($form_name));
  $data = JSONFormDefinitionGenerator::GetProperties($definition->getProperties());
  $js = <<<EOT
  Ext.formbuilder.propertiesStore = Ext.create('Ext.data.Store', {
    storeId: 'PropertiesStore',
    model: 'Properties',
    autoLoad: true,
    autoSync: true,
    proxy: {
      type: 'memory',
      data: $data,
      reader: {
        type: 'json'
      }
    }
 });
 Ext.formbuilder.propertiesStore.sync();
EOT;
  drupal_add_js($js, array('type' => 'inline', 'scope' => JS_DEFAULT));
}

/**
 * Creates an ExtJS store containing the form elements.
 *
 * Uses inline javascript to create the ExtJS store.
 *
 * Used to populate the 'Elements Tree'.
 *
 * @param string $form_name
 *   The name of the form which will be used.
 */
function xml_form_builder_create_element_store($form_name) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  module_load_include('inc', 'xml_form_builder', 'JSONFormDefinitionGenerator');
  module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
  $definition = new XMLFormDefinition(XMLFormDatabase::Get($form_name));
  $data = JSONFormDefinitionGenerator::GetForm($definition->getForm());
  $js = <<<EOT
  Ext.formbuilder.elementStore = Ext.create('Ext.data.TreeStore', {
    model: 'Element',
    root: $data
 });
EOT;
  drupal_add_js($js, array('type' => 'inline', 'scope' => JS_DEFAULT));
}

/**
 * Gets a list of all the existing element types defined by the elements hook.
 *
 * @return array
 *   A list of all the existing element types.
 */
function xml_form_builder_get_element_types() {
  $output = &drupal_static(__FUNCTION__);

  if (!isset($output)) {
    $types = array_keys(module_invoke_all('element_info'));
    $output = drupal_map_assoc($types);
    ksort($output);
  }

  return $output;
}
