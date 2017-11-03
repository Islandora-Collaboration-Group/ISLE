<?php

/**
 * @file
 * XMLSchema class.
 */

module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'php_lib', 'String');
module_load_include('inc', 'xml_form_api', 'SchemaTag');
module_load_include('inc', 'xml_schema_api', 'XmlSchemaCache');

/**
 * XMLSchema class.
 *
 * Nigel sez: "Ignoring imported documents for the most part, as they are not
 * used much in FGDC; this will be an issue we will need to address in the
 * future."
 */
class XMLSchema {

  /**
   * Constants
   */
  const PREFIX = 'xs';
  const URI = 'http://www.w3.org/2001/XMLSchema';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const prefix = self::PREFIX;
  const uri = self::URI;
  // @codingStandardsIgnoreEnd

  /**
   * A protected members object generated during this object's instantiation.
   *
   * @var ReadOnlyProtectedMembers
   */
  protected $protected;

  /**
   * The actual schema for this object.
   *
   * @var DOMDocument
   */
  protected $schema;

  /**
   * A DOMXpath object created from this object's $schema.
   *
   * @var DOMXPath
   */
  protected $xpath;

  /**
   * An array of include Schemas.
   *
   * @var Schema[]
   */
  protected $includes;

  /**
   * An array of imported Schemas.
   *
   * @var Schema[]
   */

  protected $imports;

  /**
   * An array of all created schemas identified by file name.
   *
   * @var Schema[]
   */
  protected static $schemas;

  /**
   * Constructor function for the XMLSchema class.
   *
   * @param string $filename
   *   Filename to add to the protected members array.
   * @param string $namespace
   *   Namespace to add to the protected members array.
   */
  public function __construct($filename, $namespace = NULL) {
    $filename = trim($filename);
    self::$schemas[$filename] = $this;
    $this->protected = new ReadOnlyProtectedMembers(array(
      'filename' => $filename,
      'location' => $this->getSchemaLocation($filename),
      'namespace' => $namespace,
    ));
    $this->loadSchema($filename);
  }

  /**
   * Get a list of members to serialize.
   *
   * @return array
   *   An array of ('protected', 'includes', 'imports').
   */
  public function __sleep() {
    return array('protected', 'includes', 'imports');
  }

  /**
   * Reinitialize this object, when un-serialized.
   */
  public function __wakeup() {
    $this->loadSchema($this->filename);
  }

  /**
   * Get a read-only protected variable.
   *
   * @param string $name
   *   The name of the variable to get.
   *
   * @return mixed
   *   The variable as a property of this object's protected members.
   */
  public function __get($name) {
    return $this->protected->$name;
  }

  /**
   * Set the value of a protected member.
   *
   * @param string $name
   *   The name of the protected member to set.
   * @param mixed $value
   *   The value to set it to.
   */
  public function __set($name, $value) {
    $this->protected->$name = $value;
  }

  /**
   * Gets the location of the schema file.
   *
   * @param string $filename
   *   The schema filename.
   *
   * @return string
   *   The location of the schema file.
   */
  protected function getSchemaLocation($filename) {
    $index = strrpos($filename, '/');
    return drupal_substr($filename, 0, $index);
  }

  /**
   * Gets the full path to a schema (Nigel sez: "maybe...").
   *
   * @param string $filename
   *   The filename to get the full path for.
   *
   * @return string
   *   The full path to the schema.
   */
  protected function getFullPathToSchema($filename) {
    $doc = new DOMDocument();
    $filename = trim($filename);
    $scheme = parse_url($filename, PHP_URL_SCHEME);
    if ($scheme == 'http') {
      return $filename;
    }
    else {
      $scheme = parse_url($this->location, PHP_URL_SCHEME);
      if ($scheme == 'http') {
        $matches = array();
        $location = $this->location;
        if (($count = preg_match_all('/\.\.\//', $filename, $matches, PREG_OFFSET_CAPTURE)) > 0) {
          for ($i = 0; $i < $count; $i++) {
            $index = strrpos($location, '/');
            $location = drupal_substr($location, 0, $index);
          }
          // Add +3 for "../".
          $filename = drupal_substr($filename, $matches[0][$count - 1][1] + 3);
        }
        $path = $location . '/' . $filename;
        return $path;
      }
      else {
        return realpath($this->location . '/' . $filename);
      }
    }
  }

  /**
   * Load the schema.
   *
   * @param string $filename
   *   The schema to load.
   */
  protected function loadSchema($filename) {
    $this->schema = $this->loadDocument($filename);
    $this->configureXPath();
    $this->loadIncludedFiles();
    $this->loadImportedFiles();
  }

  /**
   * Loads the given DOMDocument.
   *
   * @param string $filename
   *   The filename of the DOMDocument to load.
   *
   * @throws Exception
   *   If we can't find a document, just fail; we shouldn't attempt processing.
   *
   * @return DOMDocument
   *   The loaded DOMDocument.
   */
  protected function loadDocument($filename) {
    if (($doc = XmlSchemaCache::Get($filename)) != FALSE) {
      return $doc;
    }
    else {
      $doc = new DOMDocument();
      if (!@$doc->load($filename)) {
        throw new Exception(t('Failed to find document %filename', array('%filename' => $filename)));
      }
      XmlSchemaCache::Set($filename, $doc);
      return $doc;
    }
  }

  /**
   * Configure this schema's XPath.
   */
  protected function configureXPath() {
    $this->xpath = new DOMXPath($this->schema);
    $this->xpath->registerNamespace(self::PREFIX, self::URI);
    $this->registerNamespaces();
  }

  /**
   * Register namespaces for use in XPath queries.
   */
  protected function registerNamespaces() {
    $simple = simplexml_import_dom($this->schema->documentElement);
    $namespaces = $simple->getNamespaces(TRUE);
    foreach ($namespaces as $prefix => $uri) {
      $this->xpath->registerNamespace($prefix, $uri);
    }
  }

  /**
   * Load included schemas.
   */
  protected function loadIncludedFiles() {
    $this->includes = array();
    $included_files = $this->getIncludedFiles();
    foreach ($included_files as $include_file) {
      $filename = $this->getFullPathToSchema($include_file);
      if (empty(self::$schemas[$filename])) {
        // Included files share the same namespace.
        $this->includes[$filename] = new XMLSchema($filename, $this->namespace);
      }
      else {
        $this->includes[$filename] = self::$schemas[$filename];
      }
    }
  }

  /**
   * Gets a list of schema files that this schema includes.
   *
   * @return string[]
   *   An array of included schema files.
   */
  protected function getIncludedFiles() {
    $included_files = array();
    $result = $this->xpath->query('//xs:include');
    if ($result && $result->length > 0) {
      for ($i = 0; $i < $result->length; $i++) {
        $filename = $result->item($i)->getAttribute('schemaLocation');
        $included_files[] = $filename;
      }
    }
    return $included_files;
  }

  /**
   * Load imported schemas.
   */
  protected function loadImportedFiles() {
    $this->imports = array();
    $imported_files = $this->getImportedFiles();
    foreach ($imported_files as $import_file) {
      list($filename, $namespace) = $import_file;
      $filename = $this->getFullPathToSchema($filename);
      if (empty(self::$schemas[$filename])) {
        $this->imports[$filename] = new XMLSchema($filename, $namespace);
      }
      else {
        $this->imports[$filename] = self::$schemas[$filename];
      }
    }
  }

  /**
   * Gets a list of schema files that this schema imports.
   *
   * @return array
   *   An array of arrays, each containing the 'schemaLocation' and 'namespace'
   *   attributes of the imported files.
   */
  protected function getImportedFiles() {
    $imported_files = array();
    $result = $this->xpath->query('//xs:import');
    if ($result && $result->length > 0) {
      for ($i = 0; $i < $result->length; $i++) {
        $location = $result->item($i)->getAttribute('schemaLocation');
        $namespace = $result->item($i)->getAttribute('namespace');
        $imported_files[] = array($location, $namespace);
      }
    }
    return $imported_files;
  }

  /**
   * Checks to see if the given DOMDocument is valid.
   *
   * @param DOMDocument $doc
   *   The document to check.
   *
   * @return bool
   *   TRUE if the document validates against the cached schema file; FALSE
   *   otherwise.
   */
  public function validate(DOMDocument $doc) {
    $filename = XmlSchemaCache::GetCachedFilename($this->filename);
    if ($filename != FALSE) {
      return $doc->schemaValidate($filename);
    }
  }

  /**
   * Query the given path in this schema, and all included or imported schemas.
   *
   * @param string $path
   *   An XPath query to make.
   *
   * @return DOMNode[]
   *   An array of DOMNodes returned from the query.
   */
  public function query($path) {
    if ($this->schema) {
      $results = dom_node_list_to_array($this->xpath->query($path));
      foreach ($this->includes as $schema) {
        $results = array_merge($results, $schema->query($path));
      }
      return $results;
    }
    return array();
  }

  /**
   * Gets a unique node at the given path in this schema.
   *
   * If the node does not exist or is not unique an exception is thrown.
   *
   * @param string $path
   *   The XPath to use to get the node.
   *
   * @throws Exception
   *   If the node was not unique.
   *
   * @return DOMNode
   *   The unique DOMNode returned by the schema.
   */
  public function getUniqueNode($path) {
    $results = $this->query($path);
    if (count($results) != 1) {
      throw new Exception('Node at path was not unique. Path: ' . $path);
    }
    return array_shift($results);
  }

  /**
   * Gets all element tags within this schema.
   *
   * @return DOMNode[]
   *   An array of DOMNodes with the xs:element tag.
   */
  public function getAllElements() {
    return $this->query('//xs:element');
  }

  /**
   * Looks for a given global type.
   *
   * Type is defined as a complexType or simpleType.
   *
   * @param string $name
   *   The type name to look for.
   *
   * @return DOMNode|NULL
   *   The found DOMNode if it is the only one found; NULL if none or multiple
   *   are found.
   */
  public function findGlobalType($name) {
    if (isset($name) && $name != '') {
      $path = "(/xs:schema/xs:complexType[@name='$name']) | (/xs:schema/xs:simpleType[@name='$name'])";
      $results = $this->query($path);
      return count($results) == 1 ? array_shift($results) : NULL;
    }
    return NULL;
  }

  /**
   * Looks for a given global element.
   *
   * @param string $name
   *   The name of the element to look for.
   *
   * @return DOMNode|NULL
   *   The found DOMNode if it is the only one found; NULL if none or multiple
   *   are found.
   */
  public function findGlobalElement($name) {
    if (isset($name) && $name != '') {
      $path = "/xs:schema/xs:element[@name='$name']";
      $results = $this->query($path);
      return count($results) == 1 ? array_shift($results) : NULL;
    }
    return NULL;
  }

  /**
   * Looks for a given group element.
   *
   * @param string $name
   *   The name of the element to look for.
   *
   * @return DOMNode|NULL
   *   The found DOMNode if it is the only one found; NULL if none or multiple
   *   are found.
   */
  public function findGroup($name) {
    if (isset($name) && $name != '') {
      $path = "/xs:schema/xs:group[@name='$name']";
      $results = $this->query($path);
      return count($results) == 1 ? array_shift($results) : NULL;
    }
    return NULL;
  }

}
