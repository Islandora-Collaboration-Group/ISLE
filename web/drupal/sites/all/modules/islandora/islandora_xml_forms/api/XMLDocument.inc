<?php

/**
 * @file
 * Class for the XML Document.
 */

module_load_include('inc', 'xml_form_api', 'NodeRegistry');
module_load_include('inc', 'xml_form_api', 'Namespaces');
module_load_include('inc', 'xml_schema_api', 'Schema');
module_load_include('inc', 'php_lib', 'String');

/**
 * Class XMLDocument.
 */
class XMLDocument {

  /**
   * The state of the document when this object is put to sleep.
   *
   * @var string
   */
  protected $xml;

  /**
   * All of the namespaces required for the document.
   *
   * @var Namespaces
   */
  public $namespaces;

  /**
   * The uri to the schema.
   *
   * @var string
   */
  public $schema;

  /**
   * Stores associations between form elements and DOMNodes.
   *
   * @var NodeRegisty
   */
  public $registry;

  /**
   * The document this object represents.
   *
   * When this object is serialized the document is stored as a string in the
   * member variable $xml.
   *
   * @var DOMDocument
   */
  public $document;

  /**
   * The xpath used to query the document.
   *
   * @var DOMXPath
   */
  protected $xpath;

  /**
   * Create an XMLDocument.
   *
   * @param string $root_name
   *   The root name of the document to create.
   * @param Namespaces $namespaces
   *   An instantiated class of namespaces to append to the root.
   * @param string $schema_uri
   *   The schema URI to use.
   * @param string $xml
   *   Optional XML to use to generate the document.
   */
  public function __construct($root_name, Namespaces $namespaces, $schema_uri, $xml = NULL) {
    $this->namespaces = $namespaces;
    $this->schema = $this->createSchema($schema_uri);
    $this->document = $this->createDocument($root_name, $namespaces, $xml);
    $this->xpath = new DOMXPath($this->document);
    $this->registry = new NodeRegistry();
  }

  /**
   * Determine what member variables to serialize.
   *
   * DOMDocuments and DOMNodes cannot be serialized.
   *
   * @return array
   *   An array containing 'namespaces', 'schema', 'registry' and 'xml' strings.
   */
  public function __sleep() {
    $this->xml = $this->document->saveXML();
    return array('namespaces', 'schema', 'registry', 'xml');
  }

  /**
   * Reinitialize this object after it has be un-serialized.
   */
  public function __wakeup() {
    $this->document = $this->createDOMDocument();
    $this->document->loadXML($this->xml);
    $this->xpath = new DOMXPath($this->document);
    $this->registry->restore($this->document);
  }

  /**
   * Creates a XMLSchema object.
   *
   * @param string $schema
   *   The URI to the schema. May be NULL or an empty string.
   *
   * @return XMLSchema|bool
   *   XMLSchema object on success, NULL on failure.
   */
  protected function createSchema($schema) {
    if (is_non_empty_string($schema)) {
      try {
        return new XMLSchema($schema);
      }
      catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
    }
    return NULL;
  }

  /**
   * Creates a DOMDocument either from a provided XML string or from scratch.
   *
   * @param string $root_name
   *   The root name of the document to create.
   * @param Namespaces $namespaces
   *   Namespaces that need to be applied to the document.
   * @param string $xml
   *   XML to be loaded into the document.
   *
   * @return DOMDocument
   *   A DOMDocument created using the parameters passed in.
   */
  protected function createDocument($root_name, Namespaces $namespaces, $xml) {
    $document = $this->createDOMDocument();
    // Load from XML or create the root node.
    isset($xml) ?
        $document->loadXML($xml) :
        $document->appendChild(new DOMElement($root_name, NULL, $namespaces->getDefaultURI()));
    $this->namespaces->addNamespacesAttributesTo($document->documentElement);
    return $document;
  }

  /**
   * Validates this object's document against the schema if it exists.
   *
   * @return bool
   *   TRUE if the document is valid or no schema is provided; FALSE otherwise.
   */
  public function valid() {
    if (isset($this->schema)) {
      return $this->schema->validate($this->document);
    }
    return TRUE;
  }

  /**
   * Get the namespace URI for the given prefix.
   *
   * @param string $prefix
   *   The prefix to check the URI for.
   *
   * @return string
   *   The URI associated with the prefix.
   */
  public function getNamespaceURI($prefix = NULL) {
    if (isset($prefix)) {
      isset($this->namespaces[$prefix]) ? $this->namespaces[$prefix] : NULL;
    }
    return $this->defaultURI;
  }

  /**
   * Converts the document to an XML string.
   *
   * @return string
   *   the XMLDocument as a string.
   */
  public function saveXML() {
    return $this->document->saveXML();
  }

  /**
   * Query the document.
   *
   * @param string $path
   *   XPath string.
   * @param DOMNode $context
   *   Context node, if NULL the context will be the entire document.
   *
   * @throws Exception
   *   Upon the failure of the XPath query.
   *
   * @return DOMNodeList
   *   A list of DOMNodes resulting from the query.
   */
  public function query($path, DOMNode $context = NULL) {
    // Prevent Drupal from catching the error.
    libxml_use_internal_errors(TRUE);
    $number_of_errors_before_query = count(libxml_get_errors());
    $results = isset($context) ? $this->xpath->query($path, $context) : $this->xpath->query($path);
    $number_of_errors_after_query = count(libxml_get_errors());
    // Allow Drupal to catch errors.
    libxml_use_internal_errors(FALSE);
    $invalid_query = $results === FALSE;
    $lib_xml_error = $number_of_errors_after_query > $number_of_errors_before_query;
    $query_failed = $invalid_query || $lib_xml_error;
    if ($query_failed) {
      $error = libxml_get_last_error();
      $invalid_query_msg = "XPath query failed; either the expression is malformed or the context node is invalid.";
      $msg = $invalid_query ? $invalid_query_msg : $error->message;
      $msg .= '</br>Path: ' . $path;
      $msg .= isset($context) ? '</br>Context: ' . ($context->tagName) : '';
      $msg .= '</br>' . $this->xml;
      throw new Exception($msg);
    }
    return $results;
  }

  /**
   * Create new DOMDocument with pretty formatting.
   *
   * @return \DOMDocument
   *   The newly created DOMDocument
   */
  private function createDOMDocument() {
    $document = new DOMDocument();
    $document->preserveWhiteSpace = FALSE;
    $document->formatOutput = TRUE;

    return $document;
  }

}
