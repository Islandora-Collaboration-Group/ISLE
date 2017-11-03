<?php

/**
 * @file
 * Class to prepopulate an XML form.
 */

module_load_include('inc', 'xml_form_api', 'XMLDocument');
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * XML form prepopulator class.
 */
class XMLFormPrePopulator {

  /**
   * Node Registry.
   *
   * @var NodeRegistry
   */
  protected $registry;

  /**
   * Create FormPopulater.
   *
   * @param XMLDocument $document
   *   The document to populate.
   */
  public function __construct(XMLDocument $document) {
    $this->registry = $document->registry;
  }

  /**
   * Populate a Drupal form.
   *
   * @param FormElement $root
   *   The root element of the form to populate.
   */
  public function prePopulate(FormElement $root) {
    $this->prePopulateElement($root);
    $root->eachDecendant(array($this, 'prePopulateElement'));
  }

  /**
   * Populate a Drupal form from a given element.
   *
   * @param FormElement $element
   *   The element of the form to populate from.
   */
  public function prePopulateElement(FormElement $element) {
    $value = $this->getDefaultValue($element);
    $this->setDefaultValue($element, $value);
  }

  /**
   * Get the default value for the given element.
   *
   * @param FormElement $element
   *   The element to get the default value for.
   *
   * @return string
   *   The default value of the element.
   */
  protected function getDefaultValue(FormElement $element) {
    $node = $this->registry->get($element->hash);
    return $this->getNodeValue($node);
  }

  /**
   * Set the FormElement default value if appropriate.
   *
   * @param FormElement $element
   *   The FormElement to set the default value for.
   * @param string $value
   *   The value to set as the default for that FormElement.
   */
  protected function setDefaultValue(FormElement $element, $value) {
    if (isset($value)) {
      $element->default_value = $value;
    }
  }

  /**
   * Get the value of the node.
   *
   * Only return values for DOMAttr and DOMElements that only contain character
   * data.
   *
   * @param DOMNode $node
   *   The DOMNode to get the value for.
   *
   * @return string
   *   The DOMNode's value if appropriate, otherwise return NULL.
   */
  protected function getNodeValue(DOMNode $node = NULL) {
    if (empty($node)) {
      return NULL;
    }
    switch (get_class($node)) {
      case 'DOMAttr':
        return $node->nodeValue;

      case 'DOMElement':
        if (!dom_element_has_child_elements($node)) {
          // May not be good...
          return $node->nodeValue;
        }
        return NULL;

      default:
        return NULL;
    }
  }

}
