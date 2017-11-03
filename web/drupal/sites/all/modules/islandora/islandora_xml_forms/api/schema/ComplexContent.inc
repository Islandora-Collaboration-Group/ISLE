<?php

/**
 * @file
 * Models an <xs:complexContent> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Extension');
module_load_include('inc', 'xml_schema_api', 'Restriction');

/**
 * Models an <xs:complexContent> element.
 */
class XMLSchemaComplexContent extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'complexContent';

  /**
   * Constructor function for the XMLSchemaComplexContent class.
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
   * Returns the content of this object.
   *
   * @return mixed
   *   The content of the node if set; NULL otherwise.
   */
  protected function getContent() {
    if ($this->extension) {
      return $this->extension;
    }
    elseif ($this->restriction) {
      return $this->restriction;
    }
    return NULL;
  }

  /**
   * Adds child nodes using the specified parent InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent, with an insert order applied, as an InsertOrderNode.
   *
   * @return InsertOrderAny
   *   The mapped node, with all children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    $content = $this->getContent();
    if ($content) {
      return $content->asInsertOrderNode($parent);
    }
  }

}
