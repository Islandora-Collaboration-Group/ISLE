<?php

/**
 * @file
 * Models an <xs:restriction> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'All');
module_load_include('inc', 'xml_schema_api', 'Choice');
module_load_include('inc', 'xml_schema_api', 'Sequence');
module_load_include('inc', 'xml_schema_api', 'Group');

/**
 * Models an <xs:restriction> element.
 */
class XMLSchemaRestriction extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'restriction';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const LocalName = self::LOCAL_NAME;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor function for the XMLSchemaRestriction class.
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
      'all' => $this->createChild(XMLSchemaAll::LOCAL_NAME),
      'choice' => $this->createChild(XMLSchemaChoice::LOCAL_NAME),
      'sequence' => $this->createChild(XMLSchemaSequence::LOCAL_NAME),
      'group' => $this->createChild(XMLSchemaGroup::LOCAL_NAME),
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
   * Would probably do something fancy, but it actually just returns NULL.
   *
   * @param string $base
   *   Not even used.
   *
   * @return NULL
   *   yougetnothing.youlose.gooddaysir.willywonka.gif
   */
  protected function findExtension($base) {
    return NULL;
  }

  /**
   * Determines whether the parent node's type is complexContent.
   *
   * @return bool
   *   TRUE if it is; FALSE otherwise.
   */
  public function restrictsComplexContent() {
    return $this->node->parentNode->localName == 'complexContent';
  }

  /**
   * Determines whether the parent node's type is simpleContent.
   *
   * @return bool
   *   TRUE if it is; FALSE otherwise.
   */
  public function restrictsSimpleContent() {
    return $this->node->parentNode->localName == 'simpleContent';
  }

  /**
   * Determines whether the parent node's type is simpleType.
   *
   * @return bool
   *   TRUE if it is; FALSE otherwise.
   */
  public function restrictsSimpleType() {
    return $this->node->parentNode->localName == 'simpleType';
  }

  /**
   * Adds children using the specified parent InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent, with an insert order applied, as an InsertOrderNode.
   *
   * @return InsertOrderNode
   *   The mapped node, with children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    if ($this->restrictsComplexContent()) {
      return $this->base->asInsertOrderNode($parent);
    }
    return NULL;
  }

}
