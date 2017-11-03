<?php

/**
 * @file
 * Models an <xs:union> element.
 */

module_load_include('inc', 'xml_schema_api', 'Node');

/**
 * Models an <xs:union> DOMElement.
 */
class XMLSchemaUnion extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'union';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const LocalName = self::LOCAL_NAME;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor function for the XMLSchemaUnion class.
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
   * Included because it's in the abstraction, but (currently?) unused.
   *
   * @param InsertOrderNode $parent
   *   Mainly there because it's in the abstraction. Won't do a thing.
   *
   * @return NULL
   *   Always null.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    return NULL;
  }

}
