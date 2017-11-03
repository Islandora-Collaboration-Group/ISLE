<?php

/**
 * @file
 * Delete implementation of the ActionInterface.
 */

module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * Delete action class for XMLDocuments.
 */
class Delete implements ActionInterface {

  /**
   * Stored XPath to be used for new instances of Delete.
   * @var Path
   */
  protected $path;

  /**
   * Constructor function for the Delete class.
   *
   * @param array $params
   *   An array containing two elements:
   *   'path' - the XPath to be used in this instance of Delete.
   *   'context' - an instance of ContextType (__DEFAULT/DOCUMENT/PARENT/SELF).
   */
  public function __construct(array &$params) {
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
  }

  /**
   * Retrieves an array of values that can be passed on to a Drupal form.
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
   *   Currently only FALSE.
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL) {
    // @todo add additional parameters to determine if an element should be
    // deleted; at the moment, the elements are only deleted if they are removed
    // from the form.
    return FALSE;
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
   *   Currently only TRUE.
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    $results = $this->path->query($document, $element);
    $results = dom_node_list_to_array($results);
    foreach ($results as $node) {
      $this->doDelete($node);
    }
    return TRUE;
  }

  /**
   * Deletes a specified DOMNode.
   *
   * @param DOMNode $node
   *   The DOMNode to delete.
   */
  protected function doDelete(DOMNode $node) {
    if (isset($node->parentNode)) {
      $node->parentNode->removeChild($node);
    }
  }

}
