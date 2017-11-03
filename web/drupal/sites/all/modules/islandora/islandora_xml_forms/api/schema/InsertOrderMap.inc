<?php

/**
 * @file
 * Builds a map that defines where elements can be inserted.
 */

module_load_include('inc', 'xml_schema_api', 'Schema');
module_load_include('inc', 'xml_schema_api', 'Element');

/**
 * Models Insert order constraints.
 */
abstract class InsertOrderNode {

  /**
   * The parent of this InsertOrderNode.
   *
   * @var InsertOrderNode
   */
  public $parent;

  /**
   * The minimum bound as an integer, or the string 'unbounded' if none.
   *
   * @var mixed
   */
  public $min;

  /**
   * The maximum bound as an integer, or the string 'unbounded' if none.
   *
   * @var mixed
   */
  public $max;

  /**
   * Create an InsertOrderNode.
   *
   * @param mixed $min
   *   The minimum bound as an integer, or the string 'unbounded' if none.
   * @param mixed $max
   *   The maximum bound as an integer, or the string 'unbounded' if none.
   * @param InsertOrderNode $parent
   *   The parent InsertOrderNode, if it exists.
   */
  public function __construct($min, $max, InsertOrderNode $parent = NULL) {
    $this->min = $min;
    $this->max = $max;
    $this->parent = $parent;
  }

  /**
   * Attempts to store the given DOMElement in a matched constraint.
   *
   * @param DOMElement $element
   *   The DOMElement to store.
   *
   * @return bool
   *   TRUE on success, FALSE on failure.
   */
  abstract public function storeMatch(DOMElement $element);

  /**
   * Gets an array of DOMElements in their correct insert order.
   *
   * @return DOMElement[]
   *   The array of DOMElements in the correct order.
   */
  abstract public function getOrder();

  /**
   * Checks if this node or a child node meets this constraint.
   *
   * Does not account for occurrence constraints.
   *
   * @param DOMElement $element
   *   The DOMElement to check.
   *
   * @return bool
   *   TRUE if it meets the constraints, FALSE otherwise.
   */
  abstract public function constraintMet(DOMElement $element);

  /**
   * Checks if this constraint matched its max number of occurrences.
   *
   * @return bool
   *   TRUE if it has, FALSE otherwise.
   */
  abstract public function maximumConstraintsMet();

  // @TODO: create two more abstract public functions - matchesConstraint() and
  // matches(), each with an argument of the DOMElement $element.
}

/**
 * Models a generic compositor constraint (either All, Sequence or Choice).
 */
abstract class InsertOrderCompositor extends InsertOrderNode {

  /**
   * Child Insert Order Constraints. Compositors can repeat.
   *
   * @var InsertOrderNode[][]
   */
  public $children;

  /**
   * Create an InsertOrderCompositor.
   *
   * @param mixed $min
   *   The minimum bound as an integer, or the string 'unbounded' if none.
   * @param mixed $max
   *   The maximum bound as an integer, or the string 'unbounded' if none.
   * @param InsertOrderCompositor $parent
   *   The parent InsertOrderCompositor, if it exists.
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->children = array();
    $this->children[0] = array();
  }

  /**
   * If a parent InsertOrderCompositor gets repeated, this will be called.
   */
  public function __clone() {
    $children = $this->children[0];
    $this->children = array();
    $this->children[0] = array();
    foreach ($children as $child) {
      $this->addChild(clone $child);
    }
  }

  /**
   * Add a child to the Compositor's definition.
   *
   * @param InsertOrderNode $child
   *   The child to add to the Compositor's definition.
   */
  public function addChild(InsertOrderNode $child) {
    $this->children[0][] = $child;
  }

  /**
   * Create new occurrence of this compositor.
   *
   * @return bool
   *   TRUE if the occurrence is created, FALSE otherwise.
   */
  public function createOccurrence() {
    if (!$this->maximumConstraintsMet()) {
      $new = clone $this->children[0];
      $this->children[] = $new;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Determines if this has matched its max number of constraints.
   *
   * @return bool
   *   TRUE if it has, FALSE otherwise.
   */
  public function maximumConstraintsMet() {
    $count = count($this->children);
    return is_numeric($this->max) ? $count == $this->max : FALSE;
  }

  /**
   * Attempts to store the given DOMElement in a matched constraint.
   *
   * @param DOMElement $element
   *   The DOMElement to store.
   *
   * @return bool
   *   TRUE if the attempt was successful; FALSE otherwise.
   */
  public function storeMatch(DOMElement $element) {
    if ($this->constraintMet($element)) {
      // A child constraint matches this element.
      if ($this->attemptStore($element)) {
        return TRUE;
      }
      else {
        if ($this->createOccurrence()) {
          // Attempt to store.
          return $this->attemptStore($element);
        }
      }
    }
    return FALSE;
  }

  /**
   * A helper function used by InsertOrderCompositor::storeMatch().
   *
   * Attempts to store the element.
   *
   * @param DOMElement $element
   *   The element InsertOrderCompositor::storeMatch() should attempt to store.
   *
   * @return bool
   *   TRUE if the attempt was successful; FALSE otherwise.
   */
  protected function attemptStore(DOMElement $element) {
    $count = count($this->children);
    for ($i = 0; $i < $count; $i++) {
      foreach ($this->children[$i] as $child) {
        if ($child->storeMatch($element)) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Gets an array of DOMElements in their correct insert order.
   *
   * @return DOMElement[]
   *   An array of DOMElements in their correct insert order.
   */
  public function getOrder() {
    $order = array();
    $count = count($this->children);
    for ($i = 0; $i < $count; $i++) {
      foreach ($this->children[$i] as $child) {
        $order = array_merge($order, $child->getOrder());
      }
    }
    return $order;
  }

  /**
   * Checks if this node or a child node meets this constraint.
   *
   * Does not account for occurrence constraints.
   *
   * @param DOMElement $element
   *   The element to check against.
   *
   * @return bool
   *   TRUE if the constraint is met; FALSE otherwise.
   */
  public function constraintMet(DOMElement $element) {
    foreach ($this->children[0] as $child) {
      if ($child->constraintMet($element)) {
        return TRUE;
      }
    }
    return FALSE;
  }

}

/**
 * Models an All InsertOrder constraint.
 */
class InsertOrderAll extends InsertOrderCompositor {

  /**
   * Create an InsertOrderAll node.
   *
   * @param mixed $min
   *   The minimum bound as an integer, or the string 'unbounded' if none.
   * @param mixed $max
   *   The maximum bound as an integer, or the string 'unbounded' if none.
   * @param InsertOrderCompositor $parent
   *   The parent InsertOrderCompositor, if it exists.
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

}

/**
 * Models a Choice InsertOrder constraint.
 */
class InsertOrderChoice extends InsertOrderCompositor {

  /**
   * Create an InsertOrderChoice node.
   *
   * @param mixed $min
   *   The minimum bound as an integer, or the string 'unbounded' if none.
   * @param mixed $max
   *   The maximum bound as an integer, or the string 'unbounded' if none.
   * @param InsertOrderCompositor $parent
   *   The parent InsertOrderChoice, if it exists.
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

}

/**
 * Models a Sequence InsertOrder node.
 */
class InsertOrderSequence extends InsertOrderCompositor {

  /**
   * Create an InsertOrderSequence node.
   *
   * @param mixed $min
   *   The minimum bound as an integer, or the string 'unbounded' if none.
   * @param mixed $max
   *   The maximum bound as an integer, or the string 'unbounded' if none.
   * @param InsertOrderCompositor $parent
   *   The parent InsertOrderSequence, if it exists.
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

}

/**
 * Models the Elements InsertOrder constraint.
 *
 * DOMElements get stored as matches in this Object.
 */
class InsertOrderElement extends InsertOrderNode {

  /**
   * The name of the Element that this models.
   *
   * @var string
   */
  public $name;

  /**
   * The DOMElements that match this constraint.
   *
   * @var DOMElement[]
   */
  public $matches;

  /**
   * Constructor function for the InsertOrderElement class.
   *
   * @param string $name
   *   The name of the Element that this models.
   * @param mixed $min
   *   The minimum bound as an integer, or the string 'unbounded' if none.
   * @param mixed $max
   *   The maximum bound as an integer, or the string 'unbounded' if none.
   * @param InsertOrderCompositor $parent
   *   The parent InsertOrderElement, if it exists.
   */
  public function __construct($name, $min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->name = $name;
    $this->matches = array();
  }

  /**
   * Forces clones not to copy the matched DOMElements.
   */
  public function __clone() {
    $this->matches = array();
  }

  /**
   * Checks if this constraint has reached its maximum number of constraints.
   *
   * @return bool
   *   TRUE if it has; FALSE otherwise.
   */
  public function maximumConstraintsMet() {
    $count = count($this->matches);
    return is_numeric($this->max) ? $count == $this->max : FALSE;
  }

  /**
   * Checks if this constraint has reached its minimum number of constraints.
   *
   * @TODO: This isn't being used at the moment.
   *
   * @return bool
   *   TRUE if it has, FALSE if it hasn't.
   */
  public function minimumConstraintsMet() {
    $count = count($this->matches);
    $within_min_range = $this->min <= $count;
    // If max is non-numeric, it's assumed to be 'unbounded'.
    $within_max_range = is_numeric($this->max) ? $count <= $this->max : TRUE;
    return $within_min_range && $meets_max_range;
  }

  /**
   * Checks if an element matches this element's non-occurrence constraints.
   *
   * @param DOMElement $element
   *   The element to check against.
   *
   * @return bool
   *   TRUE if it does; FALSE otherwise.
   */
  public function constraintMet(DOMElement $element) {
    return $element->localName == $this->name;
  }

  /**
   * Attempts to store element if it meets this InsertOrderNode's constraints.
   *
   * @param DOMElement $element
   *   The element to store.
   *
   * @return bool
   *   TRUE if successfully stored; FALSE otherwise.
   */
  public function storeMatch(DOMElement $element) {
    if ($this->constraintMet($element) && !$this->maximumConstraintsMet()) {
      $this->matches[] = $element;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets the DOMElements matched by this constraint.
   *
   * @return DOMElement[]
   *   An array of DOMElements matched by this constraint.
   */
  public function getOrder() {
    return $this->matches;
  }

}

/**
 * Models the Any Element constraint.
 *
 * DOMElements get stored as matches in this Object.
 */
class InsertOrderAny extends InsertOrderNode {

  /**
   * The names of the elements that this models.
   *
   * @var string[]
   */
  public $names;

  /**
   * The DOMElements that match this constraint.
   *
   * @var DOMElement[]
   */
  public $matches;

  /**
   * Constructor function for the InsertOrderElement class.
   *
   * @param string[] $names
   *   The name of the Element that this models.
   * @param mixed $min
   *   The minimum bound as an integer, or the string 'unbounded' if none.
   * @param mixed $max
   *   The maximum bound as an integer, or the string 'unbounded' if none.
   * @param InsertOrderNode $parent
   *   The parent InsertOrderNode, if it exists.
   */
  public function __construct(array $names, $min, $max, InsertOrderNode $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->names = $names;
    $this->matches = array();
  }

  /**
   * Forces clones not to copy the matched DOMElements.
   */
  public function __clone() {
    $this->matches = array();
  }

  /**
   * Checks if this constraint has matched its max number of constraints.
   *
   * @return bool
   *   TRUE if it has; FALSE otherwise.
   */
  public function maximumConstraintsMet() {
    $count = count($this->matches);
    return is_numeric($this->max) ? $count == $this->max : FALSE;
  }

  /**
   * Checks if the minimum set of constraints been met.
   *
   * @TODO: This isn't being used at the moment.
   *
   * @return bool
   *   TRUE if the it has been met; FALSE otherwise.
   */
  public function minimumConstraintsMet() {
    $count = count($this->matches);
    $within_min_range = $this->min <= $count;
    // If max is non-numeric its assumed to be 'unbounded'.
    $within_max_range = is_numeric($this->max) ? $count <= $this->max : TRUE;
    return $within_min_range && $meets_max_range;
  }

  /**
   * Checks if an element matches this element's non-occurrence constraints.
   *
   * @param DOMElement $element
   *   The element to check against.
   *
   * @return bool
   *   TRUE if it does; FALSE otherwise.
   */
  public function constraintMet(DOMElement $element) {
    $local_name = $element->localName;
    foreach ($this->names as $name) {
      if ($name == $local_name) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Attempts to store element if it meets this InsertOrderNode's constraints.
   *
   * @param DOMElement $element
   *   The element to store.
   *
   * @return bool
   *   TRUE if successfully stored; FALSE otherwise.
   */
  public function storeMatch(DOMElement $element) {
    if ($this->constraintMet($element) && !$this->maximumConstraintsMet()) {
      $this->matches[] = $element;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets the DOMElements matched by this constraint.
   *
   * @return DOMElement[]
   *   An array of DOMElements matched by this constraint.
   */
  public function getOrder() {
    return $this->matches;
  }

}

/**
 * A tree-like structure that models where elements can be insert into.
 */
class InsertOrderMap {

  /**
   * The schema.
   *
   * @var XMLSchema
   */
  protected $schema;

  /**
   * Create an InsertOrderMap.
   *
   * @param XMLSchema $schema
   *   The schema to use for this InsertOrderMap.
   */
  public function __construct(XMLSchema $schema) {
    $this->schema = $schema;
  }

  /**
   * Gets a tree of InsertOrderNodes defining how to arrange the DOMElements.
   *
   * @param string $path
   *   The XPath query representing the node to use for the InsertOrderMap.
   *
   * @return InsertOrderNode
   *   An InsertOrderNode generated from the node returned from $path.
   */
  public function getInsertOrderMap($path) {
    $element = new XMLSchemaElement($this->schema, $this->schema->getUniqueNode($path));
    if (isset($element->complexType)) {
      return $element->complexType->asInsertOrderNode();
    }
  }

  /**
   * Inserts a given child element into the right position of its parent.
   *
   * @param string $path
   *   XPath query to use when grabbing the InsertOrderMap for this operation.
   * @param DOMElement $parent
   *   The parent element to insert the child into.
   * @param DOMElement $new
   *   The child to insert into the parent element.
   */
  public function insert($path, DOMElement $parent, DOMElement $new) {
    $map = $this->getInsertOrderMap($path);
    $remaining_children = $this->populateInsertOrderMap($map, $this->getChildrenToReorder($parent, $new));
    $insert_order = $this->getChildrenInsertOrder($map, $remaining_children);
    $this->removeChildElements($parent);
    $this->appendChildElements($parent, $insert_order);
  }

  /**
   * Stores the child DOMElements' insert order.
   *
   * Any remaining DOMElements are returned.
   *
   * @param InsertOrderNode $map
   *   The insertOrderNode used to attempt to store the children using matched
   *   constraints.
   * @param DOMElement[] $children
   *   An array of DOMElements to attempt to store.
   *
   * @return DOMElement[]
   *   An array of child elements.
   */
  protected function populateInsertOrderMap(InsertOrderNode $map, array $children) {
    do {
      $matches = FALSE;
      foreach ($children as $key => $child) {
        if ($map->storeMatch($child)) {
          unset($children[$key]);
          $matches = TRUE;
        }
      }
    } while ($matches == TRUE);
    return $children;
  }

  /**
   * Gets a list of child DOMElements to reorder.
   *
   * This ... actually seems to also append a child DOMElement to the parent,
   * which ... huh? Maybe it should be named something different?
   *
   * @param DOMElement $parent
   *   The parent element under which get the children.
   * @param DOMElement $new
   *   The new child to add to the parent.
   *
   * @return DOMElement[]
   *   An array of child DOMElements, including the newly-appended one.
   */
  protected function getChildrenToReorder(DOMElement $parent, DOMElement $new) {
    // Existing child elements.
    $children = dom_node_children($parent, 'DOMElement');
    // Add the new child to the set of children.
    $children[] = $new;
    return $children;
  }

  /**
   * Get an array of child DOMElements in their correct insert order.
   *
   * @param InsertOrderNode $map
   *   The parent element under which to get the children.
   * @param array $remaining_children
   *   The remaining children to merge into the insert order.
   *
   * @return DOMElement[]
   *   An array of DOMElements in the correct insert order.
   */
  protected function getChildrenInsertOrder(InsertOrderNode $map, array $remaining_children) {
    // Now use the map to generate the new order for elements. This doesn't work
    // with mixed content!
    $insert_order = $map->getOrder();
    // Allows for out of order composition, when the final element is
    // added this block should not be entered.
    if (count($remaining_children) > 0) {
      $insert_order = array_merge($insert_order, $remaining_children);
    }
    return $insert_order;
  }

  /**
   * Remove all child elements from a parent element.
   *
   * @param DOMElement $parent
   *   The parent element to remove the child elements from.
   */
  protected function removeChildElements(DOMElement $parent) {
    // Child Elements.
    $children = dom_node_children($parent, 'DOMElement');
    foreach ($children as $child) {
      if (isset($child->parentNode)) {
        $child->parentNode->removeChild($child);
      }
    }
  }

  /**
   * Re-insert the child elements in the correct order.
   *
   * @param DOMElement $parent
   *   The parent element to re-insert the child elements into.
   * @param DOMElement[] $children
   *   An array of child elements to re-insert into the parent element.
   */
  protected function appendChildElements(DOMElement $parent, array $children) {
    foreach ($children as $child) {
      $parent->appendChild($child);
    }
  }

}
