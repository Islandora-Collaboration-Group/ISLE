<?php
// @codingStandardsIgnoreStart
/**
 * @file
 * A collection of functions to help with DOM processing.
 */

module_load_include('inc', 'php_lib', 'String');

/**
 * Gets the a xpath for the given node.
 *
 * The returned xpath will return the given node only.
 *
 * @param DOMNode $node
 *
 * @return type
 */
function get_dom_node_xpath(DOMNode $node) {
  $path = get_dom_node_xpath_fragment($node);
  while (($node = $node->parentNode) && get_class($node) == 'DOMElement') {
    $name = get_dom_node_xpath_fragment($node);
    $path = "$name/$path";
  }
  return "/$path";
}

/**
 * Gets a fragment of the xpath for the given node.
 *
 * @param DOMNode $node
 *
 * @return type
 */
function get_dom_node_xpath_fragment(DOMNode $node) {
  if (get_class($node) == 'DOMAttr') {
    return "@{$node->nodeName}";
  }
  else {
    $name = "*[local-name() = '{$node->localName}']";
    if (isset($node->parentNode)) {
      $index = get_dom_node_xpath_index($node->parentNode, $node);
      return "{$name}[$index]";
    }
    return $name;
  }
}

/**
 * Gets the child index for the given child element in its parent,
 * for the purpose of being used to generate an xpath.
 *
 * @param DOMNode $parent
 * @param DOMNode $child
 *
 * @return int
 */
function get_dom_node_xpath_index(DOMNode $parent, DOMNode $child) {
  $xpath = new DOMXPath($child->ownerDocument);
  $name = $child->localName;
  $results = $xpath->query("child::*[local-name() = '$name']", $parent);
  $count = $results->length;
  $index = 1;
  for ($i = 0; $i < $count; $i++) {
    $node = $results->item($i);
    if ($child->isSameNode($node)) {
      break;
    }
    $index++;
  }
  return $index;
}

/**
 * Converts a DOMNodeList to an array.
 *
 * @param DOMNodeList $list
 *
 * @return array
 */
function dom_node_list_to_array(DOMNodeList $list) {
  $output = array();
  $count = $list->length;
  for ($i = 0; $i < $count; $i++) {
    $output[] = $list->item($i);
  }
  return $output;
}

/**
 * Formats and indents the given DOMDocument.
 *
 * @param DOMDocument $document
 */
function format_dom_document(DOMDocument $document) {
  $filename = drupal_get_path('module', 'php_lib') . '/js/pretty-print/PrettyPrint.xsl';
  $xsl = new DOMDocument();
  $xsl->load($filename);
  $proc = new XSLTProcessor();
  $proc->importStylesheet($xsl);
  $xml = $proc->transformToXml($document);
  $document->loadXML($xml);
}

/**
 * Includes the files required for the DOMDocument pretty print to function.
 */
function dom_document_pretty_print_include_files() {
  $path = drupal_get_path('module', 'php_lib');
  drupal_add_js("$path/js/pretty-print/prettify.js");
  drupal_add_css("$path/js/pretty-print/prettify.css");
  // @todo move pretty print to a its own module or a lib folder.
}

/**
 * Pretty print the given document, to the browswer.
 *
 * @param DOMDocument $document
 */
function dom_document_pretty_print(DOMDocument $document, $ret = FALSE) {
  global $base_url;
  format_dom_document($document);
  $content = htmlentities($document->saveXML());
  $content = append_to_new_line($content, '<br/>');
  if ($ret) {
    return "<code class='prettyprint xml' style='overflow: scroll; position: relative;'>$content</code>";
  }
  else {
    $path = $base_url . '/' . drupal_get_path('module', 'php_lib');
    echo("<script type='text/javascript' src='$path/js/pretty-print/prettify.js'></script>");
    echo("<link type='text/css' rel='stylesheet' media='all' href='$path/js/pretty-print/prettify.css'>");
    echo("<script type='text/javascript' src='$path/js/pretty-print/prettify.js'></script>");
    echo("<code class='prettyprint xml' style='overflow: scroll; position: relative;'>$content</code>");
    echo("<script type='text/javascript'>prettyPrint();</script>");
  }
}

/**
 *
 * @param DOMElement $element
 * @param string $type
 * @return array
 */
function dom_node_children(DOMNode $node, $type = NULL) {
  $children = dom_node_list_to_array($node->childNodes);
  if (isset($type)) {
    return array_filter_type($children, $type);
  }
  return $children;
}

/**
 *
 * @param DOMNode $node
 * @param string or int or object... $type
 * @param string or int or object... $property
 * @param string or int or object... $value
 */
function dom_node_filter_children(DOMNode $node, $type, $property, $value) {
  $output = array();
  $children = dom_node_children($node, $type);
  foreach ($children as $child) {
    if ($child->$property == $value) {
      $output[] = $child;
    }
  }
  return $output;
}

/**
 * Finds unquie child on params.
 *
 * @param DOMNode $node
 * @param string or int or object... $type
 * @param string or int or object... $property
 * @param string or int or object... $value
 *
 * @return type
 */
function dom_node_find_child(DOMNode $node, $type, $property, $value) {
  $children = dom_node_filter_children($node, 'DOMElement', 'localName', 'complexType');
  if (count($children) == 1) {
    return array_shift($children);
  }
  return NULL;
}

/**
 * Calls a function for each child node of the given element node.
 *
 * @param DOMElement $element
 * @param mixed $child_type
 * @param mixed $user_func
 * @param ... Arguments for the user function.
 */
function dom_element_for_each_child(DOMElement $element, $child_type, $user_func /* Variable Arguments */) {
  $user_func_args = func_get_args();
  array_splice($user_args, 2);
  // Remove the $element and $user_func.
  $children = dom_element_children($element, $child_type);
  foreach ($children as $child) {
    $args = array_merge(array($child), $user_func_args);
    if (call_user_func_array($user_func, $user_func_args) === FALSE) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Checks if a given node has child DOMElement's.
 *
 * @param DOMNode $node
 *
 * @return int
 */
function dom_element_has_child_elements(DOMNode $node) {
  $xpath = new DOMXPath($node->ownerDocument);
  $result = $xpath->query('child::*', $node);
  return $result->length > 0;
}
// @codingStandardsIgnoreEnd
