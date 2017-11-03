<?php

/**
 * @file
 * Models an <xs:simpleType> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'List');
module_load_include('inc', 'xml_schema_api', 'Union');
module_load_include('inc', 'xml_schema_api', 'Restriction');

/**
 * Models an <xs:simpleType> DOMElement.
 */
class XMLSchemaSimpleType extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'simpleType';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const LocalName = self::LOCAL_NAME;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor function for the XMLSchemaSimpleType class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(array(
      'restriction' => $this->createChild(XMLSchemaRestriction::LOCAL_NAME),
      'list' => $this->createChild(XMLSchemaList::LOCAL_NAME),
      'union' => $this->createChild(XMLSchemaUnion::LOCAL_NAME),
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
