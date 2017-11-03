<?php

/**
 * @file
 * Models an <xs:sequence> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'Element');

/**
 * Models an <xs:sequence> element.
 */
class XMLSchemaSequence extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'sequence';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const LocalName = self::LOCAL_NAME;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor function for the XMLSchemaSequence class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(array(
      'element' => $this->createChildren(XMLSchemaElement::LOCAL_NAME),
      'choice' => $this->createChildren(XMLSchemaChoice::LOCAL_NAME),
      'sequence' => $this->createChildren(XMLSchemaSequence::LOCAL_NAME),
      'group' => $this->createChildren(XMLSchemaGroup::LOCAL_NAME),
      // All children in the correct order.
      'children' => $this->createChildren(),
    ));
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
   * Adds children using the specified parent InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent, with an insert order applied, as an InsertOrderNode.
   *
   * @return InsertOrderSequence
   *   The mapped node, with children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    list($min, $max) = $this->getMinMaxOccurs();
    $map_node = new InsertOrderSequence($min, $max, $parent);
    foreach ($this->children as $child) {
      $map_node->addChild($child->asInsertOrderNode($map_node));
    }
    return $map_node;
  }

}
