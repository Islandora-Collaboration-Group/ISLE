<?php

/**
 * @file
 * Create implementation of the ActionInterface.
 */

module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');
module_load_include('inc', 'xml_schema_api', 'InsertOrderMap');

/**
 * Create action class for XMLDocuments.
 */
class Create implements ActionInterface {

  /**
   * Create types.
   */
  const ELEMENT = 'element';
  const ATTRIBUTE = 'attribute';
  const XML = 'xml';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const Element = self::ELEMENT;
  const Attribute = self::ATTRIBUTE;
  // @codingStandardsIgnoreEnd

  /**
   * Path to the parent element, where the DOMNode will be created.
   *
   * @var Path
   */
  protected $path;

  /**
   * Path the this elements definition in the schema.
   *
   * @var path
   */
  protected $schemaPath;

  /**
   * Either Create::ELEMENT, Create::ATTRIBUTE, Create::XML
   * @var integer
   */
  protected $type;

  /**
   * The full name for the Element or Attribute we are creating.
   * @var string
   */
  protected $name;

  /**
   * The namespace prefix for the Element or Attribute we are creating.
   * @var string
   */
  protected $prefix;

  /**
   * The local name for the Element or Attribute we are creating.
   * @var string
   */
  protected $localName;

  /**
   * The XML snippet we are creating.
   * @var string
   */
  protected $xml;

  /**
   * The value used to create the Element, Attribute or XML Snippit.
   * @var string
   */
  protected $value;

  /**
   * Constructor function for the Create class.
   *
   * @param array $params
   *   An array of parameters that can be passed in.
   */
  public function __construct(array &$params) {
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
    $this->schemaPath = isset($params['schema']) ? trim($params['schema']) : NULL;
    $this->value = trim($params['value']);
    $this->type = $params['type'];
    switch ($this->type) {
      case self::XML:
        $this->xml = $this->value;
        break;

      case self::ELEMENT:

      case self::ATTRIBUTE:
        $this->name = $this->value;
        $tag_name = explode(':', $this->name);
        if (count($tag_name) > 1) {
          list($this->prefix, $this->localName) = $tag_name;
        }
        else {
          list($this->localName) = $tag_name;
        }
        break;
    }
  }

  /**
   * Retrieves an array of values that can be passed on to a Drupal form.
   *
   * @return array
   *   The array of return values.
   */
  public function toDrupalForm() {
    return array(
      'path' => $this->path->path,
      'context' => (string) $this->path->context,
      'schema' => $this->schemaPath,
      'type' => $this->type,
      'prefix' => $this->prefix,
      'value' => $this->value,
    );
  }

  /**
   * Determines whether or not we should execute an action on a form element.
   *
   * @param XMLDocument $document
   *   The document we want to check.
   * @param FormElement $element
   *   The form element inside the document we want to check.
   * @param mixed $value
   *   The value of the element we want to check.
   *
   * @return bool
   *   FALSE if the element didn't exist when the form was first rendered.
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL) {
    // @TODO: add more parameters to determine if an element should be created.
    return $document->registry->isRegistered($element->hash) === FALSE;
  }

  /**
   * Executes the action on the form element.
   *
   * @param XMLDocument $document
   *   The document we want to check.
   * @param FormElement $element
   *   The form element inside the document we want to check.
   * @param mixed $value
   *   The value of the element we want to check.
   *
   * @return bool
   *   TRUE if executed; FALSE otherwise.
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    $results = $this->path->query($document, $element);
    if ($results && $results->length > 0) {
      // Filter the value.
      $value = isset($value) ?
          htmlspecialchars(html_entity_decode($value, ENT_QUOTES, 'UTF-8'), ENT_QUOTES, 'UTF-8') :
          NULL;
      $parent = $results->item(0);
      $created = $this->doCreate($document, $value);
      $this->append($document, $created, $parent);
      $document->registry->register($element->hash, $created);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Creation helper for action execution.
   *
   * @param XMLDocument $document
   *   The document passed in from the execution function.
   * @param mixed $value
   *   The value to create on within that document.
   *
   * @return DOMNode
   *   A DOMNode to use for action execution.
   */
  protected function doCreate(XMLDocument $document, $value) {
    $action = array(
      self::ELEMENT => 'createElement',
      self::ATTRIBUTE => 'createAttribute',
      self::XML => 'createXML',
    );
    try {
      return $this->{$action[$this->type]}($document, $value);
    }
    catch (Exception $e) {
      $value = htmlentities($this->value);
      $path = (string) $this->path;
      $context = (string) $this->path->context;
      $msg = "Failed to create {$this->type}. Value: {$value}.</br> Path: {$path}<br/>Context: {$context}</br> Error: {$e->getMessage()}";
      throw new Exception($msg);
    }
  }

  /**
   * Creates a DOMElement from the value passed in with the XMLDocument.
   *
   * @param XMLDocument $document
   *   The document to create a DOMElement for.
   * @param mixed $value
   *   The value of the DOMElement.
   *
   * @return DOMElement
   *   A DOMElement with the appropriate value that was passed in.
   */
  protected function createElement(XMLDocument $document, $value) {
    if (isset($this->prefix)) {
      $uri = $document->namespaces->getURI($this->prefix);
      return $document->document->createElementNS($uri, $this->name, $value);
    }
    else {
      $uri = $document->namespaces->getDefaultURI();
      return (isset($uri)) ?
          $document->document->createElementNS($uri, $this->name, $value) :
          $document->document->createElement($this->name, $value);
    }
  }

  /**
   * Creates a DOMAttr from the value passed in with the XMLDocument.
   *
   * @param XMLDocument $document
   *   The document to create a DOMAttr.
   * @param mixed $value
   *   The value to apply to the DOMAttr.
   *
   * @return DOMAttr
   *   The DOMAttr created from the passed-in value.
   */
  protected function createAttribute(XMLDocument $document, $value) {
    if (isset($this->prefix)) {
      $uri = $document->namespaces->getURI($this->prefix);
      $attribute = $document->document->createAttributeNS($uri, $this->name);
      $attribute->value = $value;
      return $attribute;
    }
    else {
      $attribute = $document->document->createAttribute($this->name);
      $attribute->value = $value;
      return $attribute;
    }
  }

  /**
   * Returns a DOMNode from the given DOMElement.
   *
   * @param XMLDocument $document
   *   The document to create a DOMNode for.
   * @param DOMElement $value
   *   The value of the DOMNode.
   *
   * @throws Exception
   *   Throws errors if there are any additional ones for Drupal to catch.
   *
   * @return DOMNode
   *   The DOMNode that is created.
   */
  protected function createXML(XMLDocument $document, $value) {
    // Sigh... Oh DOM-API, you're so backwards and useless.
    // We have to do this for namespaces to work correctly; there is no clean
    // workaround for this. User will have to put both the prefix and namespace
    // on elements where they want the prefix to stay.
    $root_attributes = '';
    foreach ($document->namespaces as $prefix => $uri) {
      $attribute = is_non_empty_string($prefix) ?
          "xmlns:$prefix" :
          "xmlns";
      $root_attributes .= "$attribute = '$uri' ";
    }
    $xml = strtr($this->value, array('%value%' => $value));
    $xml = "<root {$root_attributes}>$xml</root>";
    $this->fragment = $document->document->createDocumentFragment();
    // Prevent Drupal from catching the error.
    libxml_use_internal_errors(TRUE);
    $number_of_errors_before = count(libxml_get_errors());
    $this->fragment->appendXML($xml);
    $number_of_errors_after = count(libxml_get_errors());
    // Allow Drupal to catch errors.
    libxml_use_internal_errors(FALSE);
    if ($number_of_errors_after > $number_of_errors_before) {
      $error = libxml_get_last_error();
      throw new Exception($error->message);
    }
    $root = $this->fragment->firstChild->firstChild;
    return $root;
  }

  /**
   * Appends a DOMNode as a child node to a parent DOMElement.
   *
   * @param XMLDocument $document
   *   The document containing a DOMElement to append a child to.
   * @param DOMNode $child
   *   The child node to append.
   * @param DOMElement $parent
   *   The parent element to append the child to.
   */
  protected function append(XMLDocument $document, DOMNode $child, DOMElement $parent) {
    $has_schema = isset($document->schema);
    $has_schema_path = isset($this->schemaPath) && trim($this->schemaPath) != '';
    if ($has_schema && $has_schema_path) {
      $insert_order = new InsertOrderMap($document->schema);
      $insert_order->insert($this->schemaPath, $parent, $child);
    }
    else {
      $parent->appendChild($child);
    }
  }

}
