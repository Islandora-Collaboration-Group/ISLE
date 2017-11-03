<?php

/**
 * @file
 * Models an <xs:complexType> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexContent');
module_load_include('inc', 'xml_schema_api', 'SimpleContent');
module_load_include('inc', 'xml_schema_api', 'All');
module_load_include('inc', 'xml_schema_api', 'Choice');
module_load_include('inc', 'xml_schema_api', 'Sequence');
module_load_include('inc', 'xml_schema_api', 'Group');

/**
 * Models an <xs:complexType> DOMElement.
 */
class XMLSchemaComplexType extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'complexType';

  /**
   * Constructor function for the XMLSchemaComplexType class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(array(
      'complexContent' => $this->createChild(XMLSchemaComplexContent::LOCAL_NAME),
      'simpleContent' => $this->createChild(XMLSchemaSimpleContent::LOCAL_NAME),
      'all' => $this->createChild(XMLSchemaAll::LOCAL_NAME),
      'choice' => $this->createChild(XMLSchemaChoice::LOCAL_NAME),
      'sequence' => $this->createChild(XMLSchemaSequence::LOCAL_NAME),
      'group' => $this->createChild(XMLSchemaGroup::LOCAL_NAME),
    )
    );
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
   * Adds child nodes using the specified parent InsertOrderNode.
   *
   * @param InsertOrderNode $parent
   *   The parent, with an insert order applied, as an InsertOrderNode.
   *
   * @return InsertOrderAny
   *   The mapped node, with children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    $content = $this->getContent();
    if ($content) {
      return $content->asInsertOrderNode($parent);
    }
  }

  /**
   * Returns the content of this object.
   *
   * @return mixed
   *   The content of the node if set; NULL otherwise.
   */
  protected function getContent() {
    if (isset($this->complexContent)) {
      return $this->complexContent;
    }
    if (isset($this->simpleContent)) {
      return $this->simpleContent;
    }
    if (isset($this->all)) {
      return $this->all;
    }
    if (isset($this->choice)) {
      return $this->choice;
    }
    if (isset($this->sequence)) {
      return $this->sequence;
    }
    if (isset($this->group)) {
      return $this->group;
    }
    return NULL;
  }

}
