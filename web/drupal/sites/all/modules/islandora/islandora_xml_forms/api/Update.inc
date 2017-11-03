<?php

/**
 * @file
 * Implementation of the ActionInterface to update an element.
 */

module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');
module_load_include('inc', 'php_lib', 'DOMHelpers.inc');

/**
 * Update implementation class for the Action Interface.
 */
class Update implements ActionInterface {

  /**
   * Path to the element to update.
   *
   * @var Path
   */
  protected $path;

  /**
   * Path to the element's definition within its schema.
   *
   * @var string
   */
  protected $schema;

  /**
   * Construct Update ActionInterface.
   *
   * @param array $params
   *   An array containing two elements:
   *   'path' - the XPath to be used in this instance of Update.
   *   'context' - an instance of ContextType (__DEFAULT/DOCUMENT/PARENT/SELF).
   */
  public function __construct(array &$params) {
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
    $this->schema = isset($params['schema']) ? $params['schema'] : NULL;
  }

  /**
   * Retrieves an array of values that can be passed on to a Drupal form.
   *
   * Nigel Sez: Used by the form builder???
   *
   * @return array
   *   The array of return values.
   */
  public function toDrupalForm() {
    return array(
      'path' => $this->path->path,
      'context' => (string) $this->path->context,
    );
  }

  /**
   * Determines whether or not we should execute an action on a form element.
   *
   * @param XMLDocument $document
   *   The document we want to check.
   * @param FormElement $element
   *   The form element inside the document we want to check.
   * @param mixed $value
   *   The value of the element we want to check.
   *
   * @return bool
   *   TRUE if the value passed in is set; FALSE otherwise.
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL) {
    return isset($value);
  }

  /**
   * Executes the action on the form element.
   *
   * @param XMLDocument $document
   *   The document we want to check.
   * @param FormElement $element
   *   The form element inside the document we want to check.
   * @param mixed $value
   *   The value of the element we want to check.
   *
   * @return bool
   *   TRUE.
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    // Filter the value.
    $value = isset($value) ?
        htmlspecialchars(html_entity_decode($value, ENT_QUOTES, 'UTF-8'), ENT_QUOTES, 'UTF-8') :
        NULL;
    $results = $this->path->query($document, $element);
    $results = dom_node_list_to_array($results);
    foreach ($results as $node) {
      $this->doUpdate($node, $value);
    }
    return TRUE;
  }

  /**
   * Update the given DOMNode with the provided mixed value.
   *
   * @param DOMNode $node
   *   The DOMNode to update.
   * @param mixed $value
   *   The value to update the DOMNode with.
   */
  protected function doUpdate(DOMNode $node, $value) {
    $node->nodeValue = $value;
  }

}
