<?php

/**
 * @file
 * Models an <xs:simpleContent> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Extension');
module_load_include('inc', 'xml_schema_api', 'Restriction');

/**
 * Models an <xs:simpleContent> element.
 */
class XMLSchemaSimpleContent extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LOCAL_NAME = 'simpleContent';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const LocalName = self::LOCAL_NAME;
  // @codingStandardsIgnoreEnd

  /**
   * Constructor function for the XMLSchemaSimpleContent class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(array(
      'extension' => $this->createChild(XMLSchemaExtension::LOCAL_NAME),
      'restriction' => $this->createChild(XMLSchemaRestriction::LOCAL_NAME),
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
