<?php

/**
 * @file
 * Models an <xs:list> element.
 */

module_load_include('inc', 'xml_schema_api', 'Node');

/**
 * Models an <xs:list> DOMElement.
 */
class XMLSchemaList extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LOCAL_NAME = 'list';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const LocalName = self::LOCAL_NAME;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor function for the XMLSchemaList class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
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
   * Doesn't do anything, as <xs:list> shouldn't return an InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent, with an insert order applied, as an InsertOrderNode.
   *
   * @return NULL
   *   What it says on the tin.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    return NULL;
  }
}
