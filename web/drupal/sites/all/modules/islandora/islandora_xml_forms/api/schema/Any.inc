<?php

/**
 * @file
 * Models an <xs:element> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexType');
module_load_include('inc', 'xml_schema_api', 'SimpleType');

/**
 * Models an <xs:element> DOMElement.
 */
class XMLSchemaAny extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'any';

  /**
   * Constructor function for the XMLSchemaAny class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node = NULL) {
    parent::__construct($schema, $node);
  }

  /**
   * Returns the local name of this object.
   *
   * @return string
   *   The local name of this object.
   */
  protected function getLocalName() {
    return self::LOCAL_NAME;
  }

  /**
   * Gets an array of valid names for this XML Schema.
   *
   * Not implemented.
   *
   * @return array
   *   An array of valid names.
   */
  public function getValidNames() {
    $names = array();
    // @todo implement!
    return $names;
  }

  /**
   * Adds any valid children using the specified parent InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent, with an insert order applied, as an InsertOrderNode.
   *
   * @return InsertOrderAny
   *   The mapped node, with all children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    $names = $this->getValidNames();
    list($min, $max) = $this->getMinMaxOccurs();
    return new InsertOrderAny($names, $min, $max, $parent);
  }

}
