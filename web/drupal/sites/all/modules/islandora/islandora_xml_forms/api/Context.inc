<?php

/**
 * @file
 * Defines the three types of XPath context that can be used when defining forms
 * (document, parent, self). Also it defines a Class for determining which
 * DOMNode a given context refers to.
 */

module_load_include('inc', 'php_lib', 'Enum');

/**
 * Enumerated types for Context.
 */
class ContextType extends Enum {
  const __DEFAULT = 'document';
  const DOCUMENT = 'document';
  const PARENT = 'parent';
  const SELF = 'self';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const __default = self::__DEFAULT;
  const Document = self::DOCUMENT;
  const Parent = self::PARENT;
  const Self = self::SELF;
  // @codingStandardsIgnoreEnd

}

/**
 * Stores the type of context a Path uses, and retrieve the DOMNode a context
 * refers to.
 */
class Context {

  /**
   * The type of context this class represents.
   *
   * @var ContextType
   */
  protected $type;

  /**
   * Create XPath Context Object.
   *
   * @param ContextType $type
   *   The context for the XPath object as defined by ContextType.
   */
  public function __construct(ContextType $type) {
    $this->type = $type;
  }

  /**
   * Checks if the DOMNode for this context exists for the given FormElement.
   *
   * @param XMLDocument $document
   *   Document to check.
   * @param FormElement $element
   *   Element to check.
   *
   * @return bool
   *   TRUE/FALSE based on its existence.
   */
  public function exists(XMLDocument $document, FormElement $element) {
    try {
      $node = $this->getNode($document->registry, $element);
      return TRUE;
    }
    catch (XMLFormsContextException $e) {
      return FALSE;
    }
  }

  /**
   * Gets the node defined by this context for the given form element.
   *
   * @param XMLDocument $document
   *   The document to get the node for.
   * @param FormElement $element
   *   The node to grab.
   *
   * @return DOMNode
   *   If the context node is found it is returned, NULL otherwise.
   */
  public function getNode(XMLDocument $document, FormElement $element) {
    switch ($this->type->val) {
      case ContextType::DOCUMENT:
        return NULL;

      case ContextType::PARENT:
        return $this->getParent($document, $element);

      case ContextType::SELF:
        return $this->getSelf($document, $element);
    }
  }

  /**
   * Gets the parent context node of the provided FormElement.
   *
   * @param XMLDocument $document
   *   The document to get the element from.
   * @param FormElement $element
   *   The element to get parent context from.
   *
   * @throws XMLFormsContextNotFoundException
   *   Does so if no parent form element context is found.
   * @throws XMLFormsContextDefinitionException
   *   Does so if no parent form element defines a create or read action.
   *
   * @return DOMNode
   *   If found the parent node is returned, otherwise NULL.
   */
  protected function getParent(XMLDocument $document, FormElement $element) {
    $parent = $element->parent;
    while ($parent) {
      $selected_node = isset($parent->actions->read);
      $created_node = isset($parent->actions->create);
      if ($selected_node || $created_node) {
        // We've found a parent that should have a node registered.
        if ($document->registry->isRegistered($parent->hash)) {
          return $document->registry->get($parent->hash);
        }
        throw new XMLFormsContextNotFoundException($this->type, $element);
      }
      // Check next Parent.
      $parent = $parent->parent;
    }
    throw new XMLFormsContextDefinitionException($this->type, $element);
  }

  /**
   * Gets the self context node of the provided Form Element.
   *
   * @param XMLDocument $document
   *   The document to grab self context from.
   * @param FormElement $element
   *   The element to grab self context from within that document.
   *
   * @throws XMLFormsContextNotFoundException
   *   Does so if no context is found for the element.
   *
   * @return DOMNode
   *   The DOMNode for the provided Form Element's 'self' context.
   */
  protected function getSelf(XMLDocument $document, FormElement $element) {
    if ($document->registry->isRegistered($element->hash)) {
      return $document->registry->get($element->hash);
    }
    throw new XMLFormsContextNotFoundException($this->type, $element);
  }

  /**
   * Returns a string describing this context.
   *
   * @return string
   *   String describing the context.
   */
  public function __toString() {
    return (string) $this->type;
  }

}

/**
 * Represents exceptions that can occur when looking for the context DOMNode.
 */
class XMLFormsContextException extends Exception {

  /**
   * Constructor function for the XMLFormsContextException class.
   *
   * @param ContextType $type
   *   The context type to build an exception for.
   * @param FormElement $element
   *   The form element being referred to when the exception is thrown.
   * @param Exception $message
   *   The error message to throw.
   */
  public function __construct(ContextType $type, FormElement $element, $message) {
    $variable_description = "The Form Element<br/>";
    $variable_description .= "&nbsp;Location: '{$element->getLocation()}'<br/>";
    $variable_description .= "&nbsp;Title: '{$element['#title']}'</br>";
    $variable_description .= "&nbsp;Type: '{$element['#type']}'</br>";
    $variable_description .= "&nbsp;Context: '$type->val'</br>";
    $message = $variable_description . 'Error: ' . $message;
    parent::__construct($message, 0);
  }

  /**
   * String to describe the error.
   *
   * @return string
   *   String to return.
   */
  public function __toString() {
    return __CLASS__ . ": [{$this->code}]: {$this->message}\n";
  }

}

/**
 * Represents Exceptions that can be attributed directly to a mis-configured
 * element context definition.
 */
class XMLFormsContextDefinitionException extends XMLFormsContextException {

  /**
   * Constructor function for the XMLFormsContextDefinitionException class.
   *
   * @param ContextType $type
   *   The context type to build an exception for.
   * @param FormElement $element
   *   The form element being referred to when the exception is thrown.
   */
  public function __construct(ContextType $type, FormElement $element) {
    $message = "Specifies an XPath context of {$type} but none is defined. Check the form defintion";
    parent::__construct($type, $element, $message);
  }

}

/**
 * The given context DOMNode could not be found. In some cases, this is
 * acceptable; in others, it is not.
 */
class XMLFormsContextNotFoundException extends XMLFormsContextException {

  /**
   * Constructor function for the XMLFormsContextNotFoundException class.
   *
   * @param ContextType $type
   *   The context type to build an exception for.
   * @param FormElement $element
   *   The form element being referred to when the exception is thrown.
   */
  public function __construct(ContextType $type, FormElement $element) {
    $message = "The DOMNode associated with the context {$type->val} was not found.";
    parent::__construct($type, $element, $message);
  }

}
