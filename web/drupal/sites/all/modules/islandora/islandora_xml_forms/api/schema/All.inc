<?php

/**
 * @file
 * Models an <xs:all> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'Element');

/**
 * Models an <xs:all> element.
 */
class XMLSchemaAll extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'all';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const LocalName = self::LOCAL_NAME;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor function for the XMLSchemaAll class.
   *
   * @param XMLSchema $schema
   *   The schema to use to model the element.
   * @param DOMElement $node
   *   The element to use.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers('element', $this->createChildren(XMLSchemaElement::LOCAL_NAME));
  }

  /**
   * Returns the local name of this element.
   *
   * @return string
   *   The local name string of the element.
   */
  protected function getLocalName() {
    return self::LOCAL_NAME;
  }

  /**
   * Adds all possible children using the specified parent InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent node, with an insert order applied, as an InsertOrderNode.
   *
   * @return InsertOrderAll
   *   The mapped node, with all children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    list($min, $max) = $this->getMinMaxOccurs();
    $map_node = new InsertOrderAll($min, $max, $parent);
    foreach ($this->children as $child) {
      $map_node->addChild($child->asInsertOrderNode($map_node));
    }
    return $map_node;
  }

}
