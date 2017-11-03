<?php

/**
 * @file
 * Class for storing and querying XPaths.
 */

module_load_include('inc', 'xml_form_api', 'Context');

/**
 * A class used to store/query XPaths.
 */
class Path {

  /**
   * XPath to query with.
   *
   * @var string
   */
  public $path;

  /**
   * Query Context - either __DEFAULT, DOCUMENT, PARENT or SELF.
   *
   * @var string
   */
  public $context;

  /**
   * Creates a Path instance.
   *
   * @param string $path
   *   The XPath to create an instance for.
   * @param Context $context
   *   The query context of the XPath.
   */
  public function __construct($path, Context $context) {
    $this->path = trim($path);
    $this->context = $context;
  }

  /**
   * Queries the path with the context defined by the provided FormElement.
   *
   * @param XMLDocument $document
   *   The document containing the element to query.
   * @param FormElement $element
   *   The element inside the document to query.
   *
   * @see Context::getNode()
   *
   * @return DOMNodeList
   *   A list of nodes corresponding to the XPath query.
   */
  public function query(XMLDocument $document, FormElement $element) {
    $context = $this->context->getNode($document, $element);
    return $document->query($this->path, $context);
  }

  /**
   * Function to create a string from the XPath query.
   */
  public function __toString() {
    return $this->path;
  }

}
