<?php

/**
 * @file
 * Defines a FormDefinition class.
 *
 * This allows XML form definitions to be processed by the FormGenerator, and
 * other classes.
 */

module_load_include('inc', 'php_lib', 'String');
module_load_include('inc', 'php_lib', 'Array');

/**
 * The version number and related transforms/schemas of an XMLFormDefinition.
 */
class XMLFormDefinitionVersion {

  /**
   * The version which this class represents.
   *
   * @var int
   */
  protected $version;

  /**
   * The path to the directory where all the schema definitions are stored.
   *
   * @return string
   *   What it says on the tin.
   */
  public static function getPathToSchemaDefinitionDirectory() {
    return drupal_get_path('module', 'xml_form_api') . '/data/schemas';
  }

  /**
   * The path to the directory where all the XSL Transformations are stored.
   *
   * @return string
   *   Exactly what you'd expect.
   */
  public static function getPathToXSLTransformDirectory() {
    return drupal_get_path('module', 'xml_form_api') . '/data/transforms';
  }

  /**
   * Gets a list of all the known versions of a XML Form Definition.
   *
   * @return array
   *   An array where all the values correspond to the defined versions of an
   *   XML Form Definition, in order from oldest to most recent.
   */
  public static function getAllVersions() {
    static $versions = NULL;
    if (empty($versions)) {
      $versions = array();
      $path = self::getPathToSchemaDefinitionDirectory();
      $files = scandir($path);
      foreach ($files as $filename) {
        if (preg_match('/^.*\.xsd$/', $filename)) {
          $version = str_replace('.xsd', '', $filename);
          $versions[] = (int) $version;
        }
      }
      asort($versions);
    }
    return $versions;
  }

  /**
   * Checks to see if the given version number is valid.
   *
   * Valid schema numbers are defined by having a corresponding Schema file with
   * the same version.
   *
   * @param int $version
   *   A XML Form Definition version number.
   *
   * @return bool
   *   TRUE if the version is valid; FALSE otherwise.
   */
  public static function isValid($version) {
    $versions = self::getAllVersions();
    return array_search($version, $versions) !== FALSE;
  }

  /**
   * Gets the latest schema version number.
   *
   * @return int
   *   The latest version number.
   */
  public static function getLatestVersion() {
    $versions = self::getAllVersions();
    $latest_version = array_pop($versions);
    return new XMLFormDefinitionVersion($latest_version);
  }

  /**
   * Gets the version number of the XML Form Definition.
   *
   * Also validates the document against that version number.
   *
   * @param DOMDocument $definition
   *   The XML Form Definition.
   *
   * @return float|bool
   *   The version of the XML Form Definition, or FALSE if invalid.
   */
  public static function getVersion(DOMDocument $definition) {
    $declares_version = $definition->documentElement->hasAttribute('version');
    if ($declares_version) {
      $version = (int) $definition->documentElement->getAttribute('version');
      return self::isValid($definition, $version);
    }
    else {
      // Files with a version of 0 or 1 may not have their version declared.
      // This checks manually to confirm in these cases.
      $undeclared_versions = array(
        new XMLFormDefinitionVersion(0),
        new XMLFormDefinitionVersion(1),
      );
      foreach ($undeclared_versions as $version) {
        if (self::isValid($definition, $version)) {
          return $version;
        }
      }
    }
    // Could not find the version.
    return FALSE;
  }

  /**
   * Creates an instance of the XMLFormDefinitionVersion.
   *
   * @param int $version
   *   The XML Form Definition Schema number.
   */
  public function __construct($version) {
    if (self::isValid($version)) {
      $this->version = $version;
    }
  }

  /**
   * Gets the numerical representation of this version.
   *
   * @return int
   *   The version number.
   */
  public function get() {
    return $this->version;
  }

  /**
   * Gets the next most recent version number from the version number given.
   *
   * @return XMLFormDefinitionVersion
   *   The next version number if given is not the latest; FALSE otherwise.
   */
  public function getNextVersion() {
    $versions = self::getAllVersions();
    $position = array_search($this->get(), $versions);
    $next_position = ++$position;
    if (isset($versions[$next_position])) {
      $next_version = $versions[$next_position];
      return new XMLFormDefinitionVersion($next_version);
    }
    return FALSE;
  }

  /**
   * Checks to see if the XML Form Definition is at the most recent version.
   *
   * @return bool
   *   TRUE if it is the latest, FALSE otherwise.
   */
  public function isLatestVersion() {
    $latest = self::getLatestVersion();
    return $this->get() == $latest->get();
  }

  /**
   * Gets the filename of the Schema that represents this version.
   *
   * @return string
   *   The schema filename.
   */
  public function getSchemaFileName() {
    $path = self::getPathToSchemaDefinitionDirectory();
    return $path . '/' . $this->version . '.xsd';
  }

  /**
   * Gets the filename of the XSL Transform that represents this version.
   *
   * @return string
   *   The transform filename.
   */
  public function getTransformFileName() {
    $path = self::getPathToXSLTransformDirectory();
    return $path . '/' . $this->version . '.xsl';
  }

  /**
   * Gets an XSLTProcessor.
   *
   * The XSLTProcessor uses XMLFormDefinition::getTransformFileName(), which
   * requires XMLFormDefinition::version to be set.
   *
   * @return bool|XSLTProcessor
   *   The appropriate XSLTProcessor, or FALSE if the transform doesn't exist.
   */
  public function getTransform() {
    $filename = $this->getTransformFileName();
    if (file_exists($filename)) {
      $xsl = new DOMDocument();
      $xsl->load($filename);
      $xslt = new XSLTProcessor();
      $xslt->importStyleSheet($xsl);
      return $xslt;
    }
    return FALSE;
  }

}

/**
 * Validates and transforms XML Form Definitions into Drupal form arrays.
 */
class XMLFormDefinition {

  /**
   * The DOMDocument that represents the Form Definition.
   *
   * @var DOMDocument
   */
  public $definition;

  /**
   * Gets the version number of the XML Form Definition.
   *
   * Also validates the document against that version number.
   *
   * @throws Exception
   *  If the Version can not be determined or if the definition is not valid.
   *
   * @param DOMDocument $definition
   *   The XML Form Definition.
   *
   * @return float
   *   The version of the XML Form Definition, or FALSE if invalid.
   */
  public static function getVersion(DOMDocument $definition) {
    if (empty($definition->documentElement)) {
      throw new Exception(t('XML form definition is not valid.'));
    }
    $declares_version = $definition->documentElement->hasAttribute('version');
    if ($declares_version) {
      $version = (int) $definition->documentElement->getAttribute('version');
      $version = new XMLFormDefinitionVersion($version);
      if (self::isValid($definition, $version)) {
        return $version;
      }
      throw new Exception(t('XML form definition is not valid.'));
    }
    else {
      // Files with a version of 0 or 1 may not have their version declared.
      // Check manually to see if this is the case.
      $undeclared_versions = array(
        new XMLFormDefinitionVersion(0),
        new XMLFormDefinitionVersion(1),
      );
      foreach ($undeclared_versions as $version) {
        if (self::isValid($definition, $version)) {
          return $version;
        }
      }
    }
    // Could not find the version.
    throw new Exception(t('Failed to determine the version of the XML form definition'));
  }

  /**
   * Uses the XML Form Definition version's schema to validate the definition.
   *
   * @param DOMDocument $definition
   *   The XML Form Definition.
   * @param XMLFormDefinitionVersion $version
   *   The schema version, if none is give the latest is used.
   *
   * @return bool
   *   TRUE if the XML Form Definition is valid, FALSE otherwise.
   */
  public static function isValid(DOMDocument $definition, XMLFormDefinitionVersion $version = NULL) {
    $version = isset($version) ? $version : XMLFormDefinitionVersion::getLatestVersion();
    $file_name = $version->getSchemaFileName();
    return @$definition->schemaValidate($file_name);
  }

  /**
   * Upgrades the XML Form Definition to the next available version.
   *
   * If the document is already at its latest version nothing occurs.
   *
   * @param DOMDocument $definition
   *   The XML Form Definition to upgrade. May be modified by this function.
   *
   * @return XMLFormDefinitionVersion
   *   The upgraded version of the XML Form Definition.
   */
  public static function upgradeVersion(DOMDocument &$definition) {
    $current = self::getVersion($definition);
    $next = $current->getNextVersion();
    if ($next) {
      // There exists a version to upgrade to.
      $xslt = $next->getTransform();
      $definition = $xslt->transformToDoc($definition->documentElement);
      return $next;
    }
    // Do nothing.
    return $current;
  }

  /**
   * Repeatedly upgrades the XML Form Definition till it reaches the latest.
   *
   * @param DOMDocument $definition
   *   The XML Form Definition to upgrade.
   *
   * @return XMLFormDefinition
   *   The upgraded XML Form Definition.
   */
  public static function upgradeToLatestVersion(DOMDocument $definition) {
    do {
      $version = self::upgradeVersion($definition);
    } while (!$version->isLatestVersion());
    return $definition;
  }

  /**
   * Creates an instance of the XMLFormDefinition.
   *
   * @param DOMDocument $definition
   *   The form definition to construct.
   *
   * @throws Exception
   *   If the form definition is invalid.
   */
  public function __construct(DOMDocument $definition) {
    $this->definition = self::upgradeToLatestVersion($definition);
    if (!self::isValid($this->definition)) {
      throw new Exception('Unable to create XMLFormDefinition.');
    }
  }

  /**
   * Gets the definition.
   *
   * @return DOMDocument
   *   The form definition.
   */
  public function get() {
    return $this->definition;
  }

  /**
   * Extracts the form section of the definition into a array.
   *
   * @return array
   *   The extracted form declaration from the XML Form Definition.
   *   This is also a valid Drupal form.
   */
  public function getForm() {
    $definition = simplexml_import_dom($this->definition);
    return $this->getElement($definition->form);
  }

  /**
   * Creates an XMLDocument instance from the XML Form Definition's properties.
   *
   * @param string $xml
   *   The metadata to initialize the XMLDocument with.
   *
   * @return XMLDocument
   *   An initialized XMLDocument.
   */
  public function createXMLDocument($xml = NULL) {
    module_load_include('inc', 'xml_form_api', 'XMLDocument');
    $properties = $this->getProperties();
    // @todo change to name.
    $root_name = $properties['root_name'];
    $schema_uri = isset($properties['schema_uri']) ? $properties['schema_uri'] : NULL;
    $default_uri = isset($properties['default_uri']) ? $properties['default_uri'] : NULL;
    $namespaces = new Namespaces($properties['namespaces'], $default_uri);
    return new XMLDocument($root_name, $namespaces, $schema_uri, $xml);
  }

  /**
   * Extracts the properties section of the definition into a array.
   *
   * @return array
   *   The extracted properties from the XML Form Definition.
   */
  public function getProperties() {
    $paths = array(
      'root_name' => '/definition/properties/root_name',
      'default_uri' => '/definition/properties/namespaces/@default',
      'schema_uri' => '/definition/properties/schema_uri',
    );
    $properties = array();
    $xpath = new DOMXPath($this->definition);
    foreach ($paths as $key => $path) {
      $results = $xpath->query($path);
      if ($results->length) {
        $properties[$key] = $results->item(0)->nodeValue;
      }
    }
    $results = $xpath->query('/definition/properties/namespaces/namespace');
    $count = $results->length;
    for ($i = 0; $i < $count; $i++) {
      $node = $results->item($i);
      $prefix = $node->getAttribute('prefix');
      $properties['namespaces'][$prefix] = $node->nodeValue;
    }
    return $properties;
  }

  /**
   * Gets the array representation of an Element from the XML Form Definition.
   *
   * @param SimpleXMLElement $element
   *   The element declaration to transform into its Drupal form.
   *
   * @return array
   *   A Drupal form equivalent of the XML Form Definition element declaration.
   */
  protected function getElement(SimpleXMLElement $element) {
    $properties = $this->getElementProperties($element);
    return array_merge($properties, $this->getElementChildren($element));
  }

  /**
   * Transforms an element declaration's properties to Drupal form equivalent.
   *
   * @param SimpleXMLElement $element
   *   An element declaration from the XML Form Definition.
   *
   * @return array
   *   The Drupal representation of this element's properties declaration.
   */
  protected function getElementProperties(SimpleXMLElement $element) {
    $output = array();
    if (isset($element->properties)) {
      $properties = $element->properties->children();
      foreach ($properties as $property) {
        $name = self::getElementPropertyName($property);
        $output["#$name"] = $this->getElementProperty($property);
      }
    }
    return $output;
  }

  /**
   * Gets the value of an XML Form Definition property.
   *
   * This is found within the properties section of an element declaration.
   *
   * @param SimpleXMLElement $property
   *   An element property declaration from the XML Form Definition.
   *
   * @return mixed
   *   The value of the property.
   */
  protected function getElementProperty(SimpleXMLElement $property) {
    $children = $property->children();
    if (count($children) == 0) {
      // If cast fails the string is returned.
      $type = cast_string_to_type((string) $property);
      // XXX: This is to handle the renaming of the datepicker element for built
      // in forms. Remove this at some point.
      return $type === 'datepicker' ? XML_FORM_ELEMENTS_DATEPICKER_THEME : $type;
    }
    $output = array();
    foreach ($children as $child) {
      $name = cast_string_to_type((string) $this->getElementPropertyName($child));
      $output[$name] = $this->getElementProperty($child);
    }
    return $output;
  }

  /**
   * Gets the name to use as the array index of the property in the Drupal form.
   *
   * @param SimpleXMLElement $property
   *   An element property declaration from the XML Form Definition.
   *
   * @return string
   *   The property name.
   */
  protected function getElementPropertyName(SimpleXMLElement $property) {
    if (isset($property['key'])) {
      $key = (string) $property['key'];
      return strcasecmp($key, 'NULL') == 0 ? NULL : $key;
    }
    return $property->getName();
  }

  /**
   * Transforms all child declarations of an element to their Drupal equivalent.
   *
   * @param SimpleXMLElement $element
   *   The element declaration to transform into its Drupal form equivalent.
   *
   * @return array
   *   All the transformed child declarations.
   */
  protected function getElementChildren(SimpleXMLElement $element) {
    $output = array();
    if (isset($element->children)) {
      $elements = $element->children->children();
      foreach ($elements as $element) {
        $key = isset($element['name']) ? (string) $element['name'] : NULL;
        array_add($output, $key, $this->getElement($element));
      }
    }
    return $output;
  }

}

/**
 * Creates an XML Form Definition from a Drupal form and an array of properties.
 */
class XMLFormDefinitionGenerator {

  /**
   * Creates a DOMDocument that defines an XML Form.
   *
   * @param array $properties
   *   The form properties.
   * @param array $form
   *   A Drupal form.
   *
   * @return DOMDocument
   *   An XML Form Definition.
   */
  public static function Create(array &$properties, array &$form) {
    $latest = XMLFormDefinitionVersion::getLatestVersion();
    $definition = new SimpleXMLElement("<definition xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' version='{$latest->get()}'/>");
    self::AddProperties($definition, $properties);
    // @todo no registry defined...
    self::AddElement($definition->addChild('form'), new FormElement(NULL, $form));
    $xml = $definition->asXML();
    $definition = new DOMDocument();
    $definition->loadXML($xml);
    return $definition;
  }

  /**
   * Adds the properties to the root element of the XML Form Definition.
   *
   * @param SimpleXMLElement $definition
   *   The root element of the XML Form Definition.
   * @param array $properties
   *   The form properties.
   */
  protected static function AddProperties(SimpleXMLElement $definition, array &$properties) {
    $form_properties = $definition->addChild('properties');
    if (isset($properties['root_name'])) {
      $form_properties->addChild('root_name', $properties['root_name']);
    }
    if (isset($properties['schema_uri'])) {
      $form_properties->addChild('schema_uri', $properties['schema_uri']);
    }
    $namespaces = $form_properties->addChild('namespaces');
    if ($properties['default_uri']) {
      $namespaces->addAttribute('default', $properties['default_uri']);
    }
    if (isset($properties['namespaces'])) {
      foreach ($properties['namespaces'] as $prefix => $uri) {
        $namespace = $namespaces->addChild('namespace', $uri);
        $namespace->addAttribute('prefix', $prefix);
      }
    }
  }

  /**
   * Adds the form to a parent element of the XML Form Definition.
   *
   * @param SimpleXMLElement $parent
   *   The parent element to add the new element to.
   * @param FormElement $element
   *   The element to add.
   */
  protected static function AddElement(SimpleXMLElement $parent, FormElement $element) {
    $properties = $parent->addChild('properties');
    foreach ($element->controls as $key => $value) {
      self::AddElementProperty($properties, $key, $value);
    }
    $children = $parent->addChild('children');
    foreach ($element->children as $key => $child) {
      $element = $children->addChild('element');
      $element->addAttribute('name', $key);
      self::AddElement($element, $child);
    }
  }

  /**
   * Adds an element property to the properties of an element declaration.
   *
   * @param SimpleXMLElement $properties
   *   The properties section of an element declaration.
   * @param string $key
   *   The property's name.
   * @param mixed $value
   *   The property's value.
   */
  protected static function AddElementProperty(SimpleXMLElement $properties, $key, $value) {
    $property = self::CreateElementProperty($properties, $key);
    self::SetElementProperty($property, $value);
  }

  /**
   * Creates an XML Tag representation of a property.
   *
   * @param SimpleXMLElement $properties
   *   The parent which the new property will belong to.
   * @param string $key
   *   The potential tag name for the newly-created property.
   *
   * @return SimpleXMLElement
   *   The created Element Property.
   */
  protected static function CreateElementProperty(SimpleXMLElement $properties, $key) {
    $key = trim($key, '#');
    if (!self::IsValidXMLTag($key)) {
      $property = $properties->addChild('index');
      $property->addAttribute('key', cast_type_to_string($key));
      return $property;
    }
    return $properties->addChild($key);
  }

  /**
   * Sets the Element Property.
   *
   * @param SimpleXMLElement $property
   *   The property whose value will be set.
   * @param mixed $value
   *   The value to assign to the $property.
   */
  protected static function SetElementProperty(SimpleXMLElement $property, $value) {
    $value = ($value instanceof FormPropertyInterface) ? $value->toDrupalForm() : $value;
    if (is_array($value)) {
      foreach ($value as $key => $item) {
        self::AddElementProperty($property, $key, $item);
      }
    }
    else {
      $property[0] = cast_type_to_string($value);
    }
  }

  /**
   * Checks to see if the given tag name can be used for an XML tag.
   *
   * Numbers are invalid XML tags.
   *
   * @param string $tag_name
   *   The proposed tag name.
   *
   * @return bool
   *   TRUE if the name is valid FALSE otherwise.
   */
  protected static function IsValidXMLTag($tag_name) {
    // Start [A-Z] | "_" | [a-z].
    // Everything else is start plus | "-" | "." | [0-9].
    return (preg_match('/^[a-zA-Z_][a-zA-Z0-9_]*$/', $tag_name) > 0) ? TRUE : FALSE;
  }

}
