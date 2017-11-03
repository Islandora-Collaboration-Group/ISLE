<?php

/**
 * @file
 * Models a <xs:extension> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Any');
module_load_include('inc', 'xml_schema_api', 'Choice');
module_load_include('inc', 'xml_schema_api', 'Sequence');
module_load_include('inc', 'xml_schema_api', 'Group');

/**
 * Models an <xs:extension> element.
 */
class XMLSchemaExtension extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'extension';

  /**
   * Constructor function for the XMLSchemaExtension class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $base = $node->getAttribute('base');
    $this->protected->addMembers(array(
      'base' => $this->findExtension($base),
      'choice' => $this->createChild(XMLSchemaChoice::LOCAL_NAME),
      'sequence' => $this->createChild(XMLSchemaSequence::LOCAL_NAME),
      'group' => $this->createChild(XMLSchemaGroup::LOCAL_NAME),
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
   * Finds the appropriate extension for this class node's 'base' attribute.
   *
   * @param string $base
   *   The 'base' attribute for this class's node.
   *
   * @return mixed
   *   An XMLSchemaAny or XMLSchemaComplexType based on the node's 'base'.
   */
  protected function findExtension($base) {
    if ($base == 'xs:anyType') {
      return new XMLSchemaAny($this->schema);
    }
    else {
      $node = $this->schema->findGlobalType($base);
      return new XMLSchemaComplexType($this->schema, $node);
    }
  }

  /**
   * Determines whether the node's parent is an XMLSchemaComplexContent node.
   *
   * @return bool
   *   TRUE if it is, FALSE otherwise.
   */
  public function extendsComplexContent() {
    return $this->node->parentNode->localName == 'complexContent';
  }

  /**
   * Determines whether the node's parent is an XMLSchemaSimpleContent node.
   *
   * @return bool
   *   TRUE if it is, FALSE otherwise.
   */
  public function extendsSimpleContent() {
    return $this->node->parentNode->localName == 'simpleContent';
  }

  /**
   * Adds children using the specified parent InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent, with an insert order applied, as an InsertOrderNode.
   *
   * @return InsertOrderAny
   *   The mapped node, with children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    if ($this->extendsComplexContent()) {
      list($min, $max) = $this->getMinMaxOccurs();
      $map_node = new InsertOrderSequence($min, $max, $parent);
      // Base class.
      $map_node->addChild($this->base->asInsertOrderNode());
      foreach ($this->children as $child) {
        $map_node->addChild($child->asInsertOrderNode($map_node));
      }
      return $map_node;
    }
    return NULL;
  }

}
