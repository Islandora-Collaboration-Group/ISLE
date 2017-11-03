<?php

/**
 * @file
 * Models an <xs:group> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexType');
module_load_include('inc', 'xml_schema_api', 'SimpleType');

/**
 * Models an <xs:group> DOMElement.
 */
class XMLSchemaGroup extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'group';

  /**
   * Constructor function for the XMLSchemaGroup class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(array(
      'all' => $this->createType(XMLSchemaAll::LOCAL_NAME),
      'choice' => $this->createType(XMLSchemaChoice::LOCAL_NAME),
      'sequence' => $this->createType(XMLSchemaSequence::LOCAL_NAME),
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
   * Creates an object defined by the the children filtered via $local_name.
   *
   * @param string $local_name
   *   The LOCAL_NAME to use when filtering child nodes.
   *
   * @return XMLSchemaNode
   *   An XMLSchemaLOCAL_NAME node.
   */
  protected function createType($local_name) {
    $group = $this->schema->findGroup($this->getName);
    $children = dom_node_filter_children($group, 'DOMElement', 'localName', $local_name);
    if (count($children) == 1) {
      $child = array_shift($children);
      return $this->createNodeTypedObject($child);
    }
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
    $content = $this->getContent();
    if ($content) {
      return $content->asInsertOrderNode($parent);
    }
  }

  /**
   * The content of the node, based on type.
   *
   * @return mixed
   *   The node content, or NULL if there is nothing to return.
   */
  protected function getContent() {
    if (isset($this->all)) {
      return $this->all;
    }
    if (isset($this->choice)) {
      return $this->choice;
    }
    if (isset($this->sequence)) {
      return $this->sequence;
    }
    return NULL;
  }

}
