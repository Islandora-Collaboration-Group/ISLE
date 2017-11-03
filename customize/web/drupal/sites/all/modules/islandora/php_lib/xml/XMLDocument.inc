<?php
// @codingStandardsIgnoreStart
/**
 * @file
 * Tempory dump for classes and functions.
 */
module_load_include('inc', 'islandora_metadata', 'Enum');

/**
 * Enumerated Types.
 */
enum('XMLSchemaFormat', array('DTD', 'XSD', 'relaxNG'));

/**
 * Validates DOMDocuments.
 */
class XMLDocumentValidator {

  /**
   * The type of validation.
   *
   * @var XMLValidatorType
   */
  private $type;

  /**
   * The filename of the validition document.
   *
   * @var string
   */
  private $filename;

  /**
   *
   * @param XMLValidatorType $type
   * @param string $filename
   */
  public function __construct(XMLSchemaFormat $type, $filename = NULL) {
    $this->type = $type;
    $this->filename = $filename;
    if ($this->filename === NULL && $type !== XMLSchemaFormat::DTD()) {
      throw new InvalidArgumentException('No file provided.');
    }
  }

  /**
   *
   * @param DOMDocument $document
   * @return boolean
   */
  public function isValid(DOMDocument $document) {
    switch ($this->type) {
      case XMLSchemaFormat::DTD():
        return $document->validate();

      case XMLSchemaFormat::XSD():
        return $document->schemaValidate($this->filename);

      case XMLSchemaFormat::RNG():
        return $document->relaxNGValidate($this->filename);
    }
  }

}

/**
 *
 */
class XMLDocumentNamepaces {
  /**
   * Constants.
   */
  const xmlns = 'http://www.w3.org/2000/xmlns/';

  /**
   * The default namespace, it can be NULL.
   *
   * @var string
   */
  protected $default;

  /**
   * An array of namespaces declarations where the key is the prefix and the value is the uri.
   *
   * For example: array('prefix' => 'uri');
   *
   * @var array
   */
  protected $namespaces;

  /**
   * Creates an XMLDocumentNamepaces Instance.
   *
   * @param string $default
   *   The default namespace for the document.
   * @param array $namespaces
   *   An array of namespaces declarations where the key is the prefix and the value is the uri.
   */
  public function __construct($default = NULL, array $namespaces = NULL) {
    $this->default = $default;
    $this->namespaces = isset($namespaces) ? $namespaces : array();
  }

  /**
   * Gets the namespace URI associated with the given namespace prefix if defined.
   *
   * @param string $prefix
   *   The namespace prefix.
   *
   * @return string
   *   The namespace URI if defined FALSE otherwise.
   */
  public function getURI($prefix) {
    return isset($this->namespaces[$prefix]) ? $this->namespaces[$prefix] : FALSE;
  }

  /**
   * Gets the namespace prefix associated with the given namespace URI if defined.
   *
   * @param string $uri
   *   The namespace URI.
   *
   * @return string
   *   The namespace prefix if defined FALSE otherwise.
   */
  public function getPrefix($uri) {
    $prefix = array_search($uri, $this->namespaces);
    return ($prefix !== FALSE) ? $prefix : FALSE;
  }

  /**
   * Gets the default namespace URI if defined.
   *
   * @return string
   *   The default namespace URI, if defined FALSE othewise.
   */
  public function getDefaultURI() {
    return isset($this->default) ? $this->default : FALSE;
  }

  /**
   *
   * @param DOMXPath $xpath
   */
  public function registerNamespaces(DOMXPath $xpath) {
    if ($this->default) {
      $xpath->registerNamespace('default', $this->default);
    }
    foreach ($this->namespaces as $prefix => $uri) {
      $xpath->registerNamespace($prefix, $uri);
    }
  }

}

/**
 * Handles generic XML concepts and data.
 *
 * Underscore prevents collision with the defined class in XML forms, one day they should be the same class.
 */
class _XMLDocument extends DOMDocument {

  /**
   *
   * @var XMLValidator
   */
  protected $validator;
  protected $namepaces;

  /**
   *
   * @var DOMXPath
   */
  protected $xpath;

  /**
   *
   * @param DOMDocument $document
   * @param XMLValidator $validator
   */
  public function __construct(XMLDocumentNamepaces $namespaces, XMLDocumentValidator $validator = NULL) {
    parent::__construct();
    $this->validator = $validator;
    $this->xpath = new DOMXPath($this);
    $this->namespaces = $namespaces;
    $this->namespaces->registerNamespaces($this->xpath);
  }

  /**
   * Loads an XML document from a file.
   *
   * @param string $filename
   *   The path to the XML document.
   * @param int $options
   *   Bitwise OR of the libxml option constants.
   */
  public function load($filename, $options = NULL) {
    $ret = parent::load($filename, $options);
    if ($ret) {
      $this->xpath = new DOMXPath($this);
      $this->namespaces->registerNamespaces($this->xpath);
    }
    return $ret;
  }

  /**
   * Loads an XML document from a string.
   *
   * @param string $source
   *   The string containing the XML.
   * @param int $options
   *   Bitwise OR of the libxml option constants.
   *
   * @return
   * Returns TRUE on success or FALSE on failure. If called statically, returns a DOMDocument or FALSE on failure.
   */
  public function loadXML($source, $options = NULL) {
    $ret = parent::loadXML($source, $options);
    if ($ret) {
      $this->xpath = new DOMXPath($this);
      $this->namespaces->registerNamespaces($this->xpath);
    }
    return $ret;
  }

  /**
   * Removed function.
   */
  public function loadHTMLFile($filename) {
    throw new Exception(__FUNCTION__ . ' is not supported.');
  }

  /**
   * Removed function.
   */
  public function saveHTML() {
    throw new Exception(__FUNCTION__ . ' is not supported.');
  }

  /**
   * Checks if this XML document is valid.
   *
   * If no method of validation is provided its always assumed to be valid.
   *
   * @return boolean
   *   TRUE if valid, FALSE otherwise.
   */
  public function isValid() {
    return isset($this->validator) ? $this->validator->isValid($this) : TRUE;
  }

}
// @codingStandardsIgnoreEnd
