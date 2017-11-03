<?php

/**
 * @file
 * Abstraction for the XMLSchemaLOCAL_NAME classes.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'php_lib', 'LazyMembers');

/**
 * Contains functions common to the all schema element tags.
 */
abstract class XMLSchemaNode {

  /**
   * Array of DOMElement nodes.
   *
   * @var DOMElement[]
   */
  protected static $nodes = array();

  /**
   * The top level schema.
   *
   * @var XMLSchema
   */
  protected $schema;

  /**
   * An instance of LazyMembers based on the $node passed into XMLSchemaNode.
   *
   * @var LazyMembers
   */
  protected $protected;

  /**
   * Registers the $node used to instantiate this class in the $nodes array.
   *
   * @param XMLSchemaNode $node
   *   The XMLSchemaNode to register.
   */
  public static function register(XMLSchemaNode $node) {
    self::$nodes[] = $node;
  }

  /**
   * Determines whether the given DOMElement is registered.
   *
   * @param DOMElement $element
   *   The DOMElement to confirm registry for.
   *
   * @return bool
   *   TRUE if it is registered; FALSE otherwise.
   */
  public static function isRegistered(DOMElement $element) {
    foreach (self::$nodes as $node) {
      if ($element->isSameNode($node->node)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Attempts to find the given DOMElement in the array of registered nodes.
   *
   * @param DOMElement $element
   *   The element to search for.
   *
   * @return mixed
   *   A DOMElement if a match is found; NULL otherwise.
   */
  public static function find(DOMElement $element) {
    foreach (self::$nodes as $node) {
      if ($element->isSameNode($node->node)) {
        return $node;
      }
    }
    return NULL;
  }

  /**
   * Constructor function for the XMLSchemaNode class.
   *
   * @param XMLSchema $schema
   *   The schema to use for the DOMElement.
   * @param DOMElement $node
   *   The node to use for this instance.
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    $this->protected = new LazyMembers(array('node' => $node));
    self::register($this);
    $this->validateConstructorArguments($node);
    $this->schema = $schema;
  }

  /**
   * Validates the constructor's arguments.
   *
   * @param DOMElement $node
   *   The DOMElement passed into this class during instantiation.
   *
   * @throws Exception
   *   If the LOCAL_NAME of the node passed in doesn't match the expected.
   */
  protected function validateConstructorArguments(DOMElement $node) {
    if ($node->localName != $this->getLocalName()) {
      throw new Exception("Argument invalid; expected local name: {$this->getLocalName()} got: {$node->localName}.");
    }
  }

  /**
   * Gets the subclass's local name.
   *
   * @return string
   *   The subclass's local name.
   */
  abstract protected function getLocalName();

  /**
   * Converts this object to an InsertOrderMapNode.
   *
   * @param InsertOrderNode $parent
   *   The parent InsertOrderNode.
   */
  abstract public function asInsertOrderNode(InsertOrderNode $parent = NULL);

  /**
   * Implementation of the __isset() overloading property.
   *
   * @param string $name
   *   The name of the member property to check if isset().
   *
   * @return bool
   *   TRUE if it is set; FALSE otherwise.
   */
  public function __isset($name) {
    return isset($this->protected->$name);
  }

  /**
   * Implementation of the __unset() overloading property.
   *
   * @param string $name
   *   The name of the member property to unset.
   */
  public function __unset($name) {
    unset($this->protected->$name);
  }

  /**
   * Gets a protected member for read access.
   *
   * @param string $name
   *   The name of the member property to get.
   */
  public function __get($name) {
    return $this->protected->$name;
  }

  /**
   * Set the protected member's value.
   *
   * @param string $name
   *   The name of the protected member property to set.
   * @param mixed $value
   *   The value to set the property to.
   */
  public function __set($name, $value) {
    $this->protected->$name = $value;
  }

  /**
   * Find all immediate children with the given local name.
   *
   * @param string $local_name
   *   A LOCAL_NAME to use when filtering children.
   *
   * @return DOMElement[]
   *   An array of children found using the search.
   */
  protected function findChildren($local_name = NULL) {
    return isset($local_name) ?
        dom_node_filter_children($this->node, 'DOMElement', 'localName', $local_name) :
        dom_node_children($this->node, 'DOMElement');
  }

  /**
   * Finds a unique child among the immediate children of this element.
   *
   * @param string $local_name
   *   The LOCAL_NAME to use in the search.
   *
   * @return DOMElement|NULL
   *   Either the only child found, the first child found out of many,
   *   or NULL if there weren't any.
   */
  protected function findChild($local_name) {
    $children = $this->findChildren($local_name);
    if (count($children) == 1) {
      return array_shift($children);
    }
    return NULL;
  }

  /**
   * Create all child elements of the given type.
   *
   * @param string $local_name
   *   An optional LOCAL_NAME to filter the children by.
   *
   * @return XMLSchemaNode[]
   *   An array of XMLSchemaLOCAL_NAME objects.
   */
  protected function createChildren($local_name = NULL) {
    $output = array();
    $children = $this->findChildren($local_name);
    foreach ($children as $child) {
      $output[] = $this->createNodeTypedObject($child);
    }
    return $output;
  }

  /**
   * Create a child element of the given type.
   *
   * @param string $local_name
   *   A LOCAL_NAME to filter the children by.
   *
   * @return XMLSchemaNode|NULL
   *   An XMLSchemaLOCAL_NAME object if a child is found and created; NULL
   *   otherwise.
   */
  protected function createChild($local_name) {
    $child = $this->findChild($local_name);
    return isset($child) ? $this->createNodeTypedObject($child) : NULL;
  }

  /**
   * Creates an a object defined by the LOCAL_NAME of the given DOMNode.
   *
   * @param DOMElement $node
   *   The DOMNode to create a typed object for.
   *
   * @return XMLSchemaNode|NULL
   *   An XMLSchemaLOCAL_NAME object if that class exists; NULL otherwise.
   */
  protected function createNodeTypedObject(DOMElement $node) {
    $class_name = "XMLSchema{$node->localName}";
    $object = self::find($node);
    if (isset($object)) {
      return $object;
    }
    elseif (class_exists($class_name)) {
      return new $class_name($this->schema, $node);
    }
    return NULL;
  }

  /**
   * Get the min and max occurs.
   *
   * @return array
   *   An array containing two keys (minimum, maximum). Each are set to 1 if
   *   their respective attribute isn't set for this node.
   */
  public function getMinMaxOccurs() {
    $min = $this->node->hasAttribute('minOccurs') ? $this->node->getAttribute('minOccurs') : 1;
    $max = $this->node->hasAttribute('maxOccurs') ? $this->node->getAttribute('maxOccurs') : 1;
    return array($min, $max);
  }

}
