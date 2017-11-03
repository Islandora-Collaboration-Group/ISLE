<?php

/**
 * @file
 * Models an <xs:element> element.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexType');
module_load_include('inc', 'xml_schema_api', 'SimpleType');

/**
 * Models an <xs:element> DOMElement.
 */
class XMLSchemaElement extends XMLSchemaNode {

  /**
   * Constants.
   */
  const LOCAL_NAME = 'element';

  /**
   * If $node is a reference, then this is the DOMElement $node references.
   *
   * This is otherwise equivalent to $node.
   *
   * @var DOMElement
   */
  protected $definition;

  /**
   * The DOMElement $node references if it exists.
   *
   * @var DOMElement
   */
  protected $reference;

  /**
   * Constructor function for the XMLSchemaElement class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $type = new LazyMember(array($this, 'createType'));
    $this->protected->addMember('type', $type);
    if ($this->getType()->localName == 'complexType') {
      $this->protected->addMember('complexType', $type);
    }
    elseif ($this->getType()->localName == 'simpleType') {
      $this->protected->addMember('simpleType', $type);
    }
  }

  /**
   * Creates a node based on the LOCAL_NAME constant of the given node's class.
   *
   * @return mixed
   *   Returns a new XMLSchemaLOCAL_NAME class object associated with that node
   *   if one is found; if no class exists, returns FALSE.
   */
  public function createType() {
    $node = $this->getType();
    return isset($node) ? $this->createNodeTypedObject($node) : NULL;
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
   * @return InsertOrderAny
   *   The mapped node, with children appended.
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    list($min, $max) = $this->getMinMaxOccurs();
    return new InsertOrderElement($this->getName(), $min, $max, $parent);
  }

  /**
   * Gets the definition of the element.
   *
   * @return DOMElement
   *   If $node is a reference, then this is the DOMElement $node references.
   *   Otherwise, this is equivalent to $node.
   */
  public function getDefinition() {
    if (empty($this->definition)) {
      $this->definition = $this->isReference() ?
          $this->getReference() :
          $this->node;
    }
    return $this->definition;
  }

  /**
   * Determines if this element is a reference.
   *
   * @return bool
   *   TRUE if it is a reference, FALSE otherwise.
   */
  public function isReference() {
    return $this->node->hasAttribute('ref');
  }

  /**
   * Gets the element this element refers to.
   *
   * @return DOMElement
   *   The DOMElement this element refers to.
   */
  public function getReference() {
    if (empty($this->reference) && $this->isReference()) {
      $name = $this->node->getAttribute('ref');
      $this->reference = $this->schema->findGlobalElement($name);
    }
    return $this->reference;
  }

  /**
   * Determines if this element has a type.
   *
   * @return bool
   *   TRUE if the element has a type, FALSE otherwise.
   */
  public function isTyped() {
    return $this->getDefinition()->hasAttribute('type');
  }

  /**
   * Determines whether or not this element is anonymous (i.e. has no type).
   *
   * @return bool
   *   TRUE if the element is anonymous, FALSE otherwise.
   */
  public function isAnonymous() {
    return $this->isTyped() == FALSE;
  }

  /**
   * Gets the type of element that this is from its definition.
   *
   * @return DOMElement
   *   The type of element this is, if one os available.
   */
  public function getType() {
    $definition = $this->getDefinition();
    if ($this->isAnonymous()) {
      $child = dom_node_find_child($definition, 'DOMElement', 'localName', 'complexType');
      if (empty($child)) {
        $child = dom_node_find_child($definition, 'DOMElement', 'localName', 'simpleType');
      }
      // If key, keyRef, unique, return NULL.
      return $child;
    }
    else {
      $type_name = $definition->getAttribute('type');
      return $this->schema->findGlobalType($type_name);
    }
  }

  /**
   * Gets the name of this element.
   *
   * @return string
   *   The name of the element, or the name of the referred element if this is a
   *   reference.
   */
  public function getName() {
    if ($this->isReference()) {
      return $this->node->getAttribute('ref');
    }
    else {
      return $this->node->getAttribute('name');
    }
  }

}
