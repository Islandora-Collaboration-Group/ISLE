<?php

/**
 * @file
 * XML Form Generator class.
 */

module_load_include('inc', 'php_lib', 'DOMHelpers.inc');

/**
 * Registers DOMNodes to FormElements.
 *
 * Also generates additional FormElements for FormElements that represent
 * several DOMNodes.
 */
class XMLFormGenerator {

  /**
   * The form we're generating.
   *
   * @var Form
   */
  protected $form;

  /**
   * The XMLDocument we're grabbing nodes from.
   *
   * @var XMLDocument
   */
  protected $document;

  /**
   * The XMLDocument registry.
   *
   * @var NodeRegistry
   */
  protected $registry;

  /**
   * Create an XMLFormGenerator.
   *
   * @param XMLForm $form
   *   The XMLForm we're generating.
   * @param XMLDocument $document
   *   The XMLDocument we're grabbing nodes from.
   */
  public function __construct(XMLForm $form, XMLDocument $document) {
    $this->form = $form;
    $this->document = $document;
    $this->registry = $document->registry;
  }

  /**
   * Generate additional elements based on the provided document.
   *
   * @param FormElement $root
   *   The root FormElement to work from.
   */
  public function generate(FormElement $root) {
    $this->processElement($root);
    $root->eachDecendant(array($this, 'processElement'));
  }

  /**
   * Registers and generates data for the given element.
   *
   * @param FormElement $element
   *   The element to generate.
   *
   * @return bool
   *   TRUE on success, FALSE if the element has already been registered.
   */
  public function processElement(FormElement $element) {
    if (!$this->registry->isRegistered($element->hash)) {
      $nodes = $this->findNodes($element);
      $node = array_shift($nodes);
      if (isset($node)) {
        $this->registry->register($element->hash, $node);
        $duplicates = $this->createDuplicates($element, count($nodes));
        $this->registerDuplicates($duplicates, $nodes);
        foreach ($duplicates as $duplicate) {
          $duplicate->eachDecendant(array($this, 'processElement'));
        }
      }
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Finds nodes for the given element.
   *
   * @param FormElement $element
   *   The element to grab nodes from.
   *
   * @return array
   *   An array containing any DOMNodes found.
   */
  protected function findNodes(FormElement $element) {
    $actions = $element->actions;
    $reader = $actions ? $actions->read : NULL;
    if ($reader) {
      $results = $reader->execute($this->document, $element);
      if ($results) {
        return dom_node_list_to_array($results);
      }
    }
    return array();
  }

  /**
   * Creates a given number of duplicates of a FormElement.
   *
   * @param FormElement $element
   *   The element to duplicate.
   * @param int $count
   *   The number of times to duplicate the element.
   *
   * @return array
   *   An array of duplicated FormElements.
   */
  protected function createDuplicates(FormElement $element, $count) {
    $output = array();
    for ($i = 0; $i < $count; $i++) {
      $clone = $this->form->duplicateOriginal($element->hash);
      $element->parent[] = $clone;
      $output[] = $clone;
    }
    return $output;
  }

  /**
   * Registers an array of duplicated elements to an array of nodes.
   *
   * @see XMLFormGenerator::createDuplicates()
   *
   * @param array $elements
   *   The array of elements to register.
   * @param array $nodes
   *   The array of nodes to register the elements to.
   */
  protected function registerDuplicates(array $elements, array $nodes) {
    $count = count($elements);
    for ($i = 0; $i < $count; $i++) {
      $element = $elements[$i];
      $node = $nodes[$i];
      $this->registry->register($element->hash, $node);
    }
  }

}
