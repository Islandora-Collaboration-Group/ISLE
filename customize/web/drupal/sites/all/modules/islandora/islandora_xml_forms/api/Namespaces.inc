<?php

/**
 * @file
 * Iterator class for namespaces.
 */

/**
 * Exceptions that may occur when building.
 */
class XMLFormsNamespaceException extends Exception {

  /**
   * Constructor function for the XMLFormsNamespaceException class.
   *
   * @param string $message
   *   A message to pass through from the exception.
   * @param int $code
   *   The exception code number.
   * @param null $previous
   *   The previous exception in the exception chain.
   */
  public function __construct($message = "", $code = 0, $previous = NULL) {
    parent::__construct($message, $code, $previous);
  }

}

/**
 * This class is meant to encapsulate namespaces.
 */
class Namespaces implements IteratorAggregate {
  /**
   * Constants
   */
  const XMLNS = 'http://www.w3.org/2000/xmlns/';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const xmlns = self::XMLNS;
  // @codingStandardsIgnoreEnd

  /**
   * The default namespace; it can be NULL.
   *
   * @var string
   */
  protected $default;

  /**
   * An array of namespaces declarations using the format ('prefix' => 'uri').
   *
   * @var array
   */
  protected $namespaces;

  /**
   * Creates a Namespaces instance.
   *
   * @param array $namespaces
   *   An array of namespaces declarations using the format ('prefix' => 'uri').
   * @param string $default
   *   The default namespace for the document.
   */
  public function __construct(array &$namespaces = NULL, $default = NULL) {
    $this->default = $default;
    $this->namespaces = isset($namespaces) ? $namespaces : array();
  }

  /**
   * Gets the URI associated with the given prefix.
   *
   * @param string $prefix
   *   The prefix to use when searching for URIs.
   *
   * @throws XMLFormsNamespaceException
   *   When the prefix being searched for isn't defined.
   */
  public function getURI($prefix) {
    if (isset($this->namespaces[$prefix])) {
      return $this->namespaces[$prefix];
    }
    throw new XMLFormsNamespaceException("Namespace URI with the prefix '$prefix' was not defined.");
  }

  /**
   * Gets the URI associated with the given prefix.
   *
   * @param string $uri
   *   The URI to use when searching for prefixes.
   *
   * @throws XMLFormsNamespaceException
   *   When the URI being searched for isn't defined.
   *
   * @return string
   *   The prefix found if no exception is thrown.
   */
  public function getPrefix($uri) {
    $prefix = array_search($uri, $this->namespaces);
    if ($prefix !== FALSE) {
      return $prefix;
    }
    throw new XMLFormsNamespaceException("Namespace URI '$uri' was not defined; could not find the prefix.");
  }

  /**
   * Gets the default namespace URI.
   *
   * @return string
   *   The default URI being asked for.
   */
  public function getDefaultURI() {
    return $this->default;
  }

  /**
   * Adds all the namespace as attributes to a given element.
   *
   * @param DOMElement $element
   *   The DOMElement to add the namespaces to.
   */
  public function addNamespacesAttributesTo(DOMElement $element) {
    if (isset($this->default)) {
      $element->setAttribute('xmlns', $this->default);
    }
    foreach ($this->namespaces as $prefix => $uri) {
      $element->setAttributeNS(self::XMLNS, 'xmlns:' . $prefix, $uri);
    }
  }

  /**
   * Gets an iterator that allows the user to iterate over the namespaces.
   *
   * @return ArrayIterator
   *   The ArrayIterator to run the class's array of namespaces through.
   */
  public function getIterator() {
    $namespaces = $this->namespaces;
    if (isset($this->default)) {
      $namespaces[''] = $this->default;
    }
    return new ArrayIterator($namespaces);
  }

}
