<?php

/**
 * @file
 * Classes for the XML Form Processor.
 */

module_load_include('inc', 'xml_form_api', 'XMLDocument');

/**
 * Encapsulates the required data to perform a CRUD action.
 */
class XMLFormProcessAction {

  /**
   * ActionInterface to perform.
   *
   * @var ActionInterface
   */
  public $action;

  /**
   * FormElement to be processed.
   *
   * @var FormElement
   */
  public $element;

  /**
   * Submitted Form Value.
   *
   * @var mixed
   */
  public $value;

  /**
   * Creates an XMLFormProcessAction instance.
   *
   * @param ActionInterface $action
   *   The appropriate action that will be performed.
   * @param FormElement $element
   *   The FormElement that the action will be performed on.
   * @param mixed $value
   *   The value that will be used in execution.
   */
  public function __construct(ActionInterface $action, FormElement $element, $value = NULL) {
    $this->action = $action;
    $this->element = $element;
    $this->value = $value;
  }

  /**
   * Executes the action.
   *
   * @param XMLDocument $document
   *   The document to execute on.
   *
   * @return bool
   *   Generally TRUE on execution and FALSE otherwise. Specifics depend on the
   *   implemented ActionInterface in XMLFormProcessAction::__construct().
   */
  public function execute(XMLDocument $document) {
    return $this->action->execute($document, $this->element, $this->value);
  }

}

/**
 * Process the form to generate a Document.
 */
class XMLFormProcessor {

  /**
   * Submitted form values.
   *
   * @var FormValues
   */
  protected $values;

  /**
   * Document to be modified with the submitted values.
   *
   * @var XMLDocument
   */
  protected $document;

  /**
   * Registry to access form element information for modifying the document.
   *
   * @var NodeRegistry
   */
  protected $nodeRegistry;

  /**
   *
   * @var FormElementRegistry
   */
  protected $elementRegistry;

  /**
   * Creates a XMLFormProcessor instance.
   *
   * This is used to modify the given document with the values and actions
   * specified by the submitted form.
   *
   * @param FormValues $values
   *   The submitted values for this form.
   * @param XMLDocument $document
   *   The document to be modified with the submitted values.
   * @param FormElementRegistry $registry
   *   Registry to access form element information for modifying the document.
   */
  public function __construct(FormValues $values, XMLDocument $document, FormElementRegistry $registry) {
    $this->values = $values;
    $this->document = $document;
    $this->nodeRegistry = $document->registry;
    $this->elementRegistry = $registry;
  }

  /**
   * The actual form processor.
   *
   * @param FormElement $element
   *   The element we are processing on.
   *
   * @return XMLDocument
   *   The processed XML Document.
   */
  public function process(FormElement $element) {
    // The order of action execution here is important. Create actions should
    // take place before updates; newly created elements can be registered as we
    // can update newly created elements. Delete must come last, as we want the
    // update actions to run on elements that may then later be removed.
    // At last we clean up our nodeRegistry. This is necessary because after
    // deletion we can end with orphan domnodes and attributes attached to this.
    $elements = $element->flatten();
    $filtered_elements = $this->filterElements($elements);
    $this->createNodes($this->getActions($filtered_elements, 'create'));
    $this->modifyNodes($this->getActions($filtered_elements, 'update'));
    // This does nothing right now.
    // Delete actions always return false to shouldExecute().
    $this->modifyNodes($this->getActions($filtered_elements, 'delete'));
    $this->modifyNodes($this->getRemovedFormElementsDeleteActions($element));
    $this->cleanupNodeRegistry($element);
    return $this->document;
  }

  /**
   * Returns an array of actions for the element array passed in.
   *
   * @param array $elements
   *   The array of elements to get actions for.
   * @param string $type
   *   The type of action to get.
   *
   * @return array
   *   An array of actions.
   */
  protected function getActions(array &$elements, $type) {
    $actions = array();
    foreach ($elements as $key => $element) {
      $value = $this->values->getValue($element->hash);
      $action = isset($element->actions->$type) ? $element->actions->$type : NULL;
      if (isset($action) && $action->shouldExecute($this->document, $element, $value)) {
        $actions[] = new XMLFormProcessAction($action, $element, $value);
        // Remove from the list of actionable elements.
        unset($elements[$key]);
      }
    }
    return $actions;
  }

  /**
   * Creates DOMNodes in the document.
   *
   * @param array $actions
   *   Actions to use when creating nodes.
   */
  protected function createNodes(array $actions) {
    // The create actions are looped in a while statement to allow for out-of-
    // order construction of elements.
    do {
      $continue = FALSE;
      foreach ($actions as $key => $action) {
        if ($action->execute($this->document)) {
          $continue = TRUE;
          unset($actions[$key]);
        }
      }
    } while ($continue);
  }

  /**
   * Updates/Delete's DOMNodes in the document.
   *
   * @param array $actions
   *   Actions to use when modifying nodes.
   */
  protected function modifyNodes(array $actions) {
    foreach ($actions as $action) {
      $action->execute($this->document);
    }
  }

  /**
   * Cleanups orphan DOMNodes from the registry.
   *
   * @param FormElement $element
   *   The element to check against.
   */
  protected function cleanupNodeRegistry(FormElement $element) {
    // Checks if the form elements associated with a DOMNodes where
    // removed. If any found, verifies if the associated DOMNodes are
    // attributes of an orphan node or descendants of one that had a delete
    // action processed, removing those from nodeRegistry.
    // This is needed because the serialization and unserialization
    // process of nodeRegistry with orphan DOMNodes results in invalid
    // XPaths that silently fail when unserializing the super
    // structure a.k.a $form_state.
    $registered = $this->nodeRegistry->getRegistered();
    $elements = $element->flatten();
    $filtered_elements = $this->filterElements($elements);
    foreach ($registered as $hash => &$node) {
      if (isset($filtered_elements[$hash]) === FALSE) {
        if (isset($node->nodeType) && $node->nodeType === XML_ATTRIBUTE_NODE) {
          if (!isset($node->ownerElement->parentNode)) {
            $this->nodeRegistry->unregister($hash);
          }
        }
        else {
          $currentelement = $this->elementRegistry->get($hash);
          $delete = isset($currentelement->actions->delete) ? $currentelement->actions->delete : NULL;
          if (isset($delete)) {
            $elements_tounregister = array_keys($currentelement->flatten());
            foreach ($elements_tounregister as $descendant_hash) {
              $this->nodeRegistry->unregister($descendant_hash);
              unset($registered[$descendant_hash]);
            }
          }
        }
      }
    }
  }

  /**
   * If registered node element is no longer in the form, add a delete action.
   *
   * @param FormElement $element
   *   The element to get delete actions for.
   *
   * @return array
   *   An array of XMLFormProcessActions.
   */
  protected function getRemovedFormElementsDeleteActions(FormElement $element) {
    $actions = array();
    $elements = $element->flatten();
    $filtered_elements = $this->filterElements($elements);
    $registered = $this->nodeRegistry->getRegistered();
    foreach ($registered as $hash => $node) {
      if (isset($filtered_elements[$hash]) === FALSE) {
        $element = $this->elementRegistry->get($hash);
        $delete = isset($element->actions->delete) ? $element->actions->delete : NULL;
        if (isset($delete)) {
          $actions[] = new XMLFormProcessAction($delete, $element);
        }
      }
    }
    return $actions;
  }

  /**
   * Filters out elements that are hidden by #access and not to be processed.
   *
   * @param array $elements
   *   An array containing flattened FormElements.
   *
   * @return array
   *   An array of filtered FormElements.
   */
  protected function filterElements($elements) {
    $filter_function = function ($filter_element) {
      if (isset($filter_element->controls['#access'])) {
        return $filter_element->controls['#access'];
      }
      return TRUE;
    };
    $filtered_elements = array_filter($elements, $filter_function);
    return $filtered_elements;
  }
}
