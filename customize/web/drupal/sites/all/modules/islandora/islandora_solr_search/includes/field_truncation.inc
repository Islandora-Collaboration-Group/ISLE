<?php

/**
 * @file
 * Class to assist in field truncation, paying special attention to HTML.
 */

/**
 * Islandora Field Truncation.
 */
class IslandoraSolrFieldTruncation {
  protected $writer;
  protected $output = '';
  protected $wordSafe;
  protected $addEllipsis;
  protected $wordsafeLength;

  const GENERAL_DOM = <<<EOXML
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<body>@replace@</body>
</html>
EOXML;

  const XHTML_URI = 'http://www.w3.org/1999/xhtml';

  /**
   * Class Constructor function.
   *
   * @param string $text
   *   The text to truncate.
   * @param int $desired_length
   *   The desired length to truncate to.
   * @param bool $word_safe
   *   Attempt to truncate on a word boundary.
   * @param bool $add_ellipsis
   *   Add '...' to the end of the truncated string.
   * @param int $wordsafe_length
   *   If $word_safe is true, the minimum acceptable length for truncation.
   *   No effect if $word_safe is false.
   */
  public function __construct($text, $desired_length, $word_safe = TRUE, $add_ellipsis = TRUE, $wordsafe_length = 1) {
    $this->wordSafe = $word_safe;
    $this->addEllipsis = $add_ellipsis;
    $this->wordsafeLength = $wordsafe_length;

    if (strpos($text, '<') === FALSE && strpos($text, '>') === FALSE) {
      // No apparent markup... No need for more processing.
      $this->output = $this->truncateText($text, $desired_length);
      return;
    }

    $this->writer = new XMLWriter();
    $this->writer->openMemory();
    $this->process($this->document($text)->getElementsByTagName('body')->item(0), $desired_length);
    $this->output = $this->writer->outputMemory();
  }

  /**
   * Helper; load up text into a DOMDocument.
   *
   * Adapted from filter_load_dom(), but made to do XML instead of HTML.
   *
   * @param string $text
   *   A string which may or may not contain markup elements.
   *
   * @return DOMDocument
   *   A loaded XHTML document containing the text.
   *
   * @see filter_load_dom()
   */
  protected function document($text) {
    $dom = new DOMDocument();
    $dom_data = str_replace("@replace@", $text, static::GENERAL_DOM);

    // The dom can Fail to load on elements that are self
    // closing (such as <br/>) or special chars like ><. This is why we choose
    // to load the data as HTML instead.
    if (!@$dom->loadXML($dom_data)) {
      @$dom->loadHTML($dom_data);
    }
    return $dom;
  }

  /**
   * Truncate text according to input parameters.
   *
   * @param string $text
   *   The string to truncate.
   * @param int $remaining
   *   The remaining truncation length.
   */
  protected function truncateText($text, $remaining) {
    return truncate_utf8(
      $text,
      $remaining,
      $this->wordSafe,
      $this->addEllipsis,
      $this->wordsafeLength
    );
  }

  /**
   * Process and close DOMElements as they appear in the Doc.
   *
   * @param DOMElement $el
   *   The DOMElement to process.
   * @param int $remaining
   *   The max truncation length.
   *
   * @throws Exception
   */
  protected function process(DOMElement $el, &$remaining) {
    foreach ($el->childNodes as $child) {
      switch ($child->nodeType) {
        case XML_ELEMENT_NODE:
          if (!$child->namespaceURI || $child->namespaceURI === static::XHTML_URI) {
            $this->writer->startElement($child->nodeName);
          }
          else {
            $this->writer->startElementNS($child->prefix, $child->localName, $child->namespaceURI);
          }
          foreach ($child->attributes as $attribute) {
            if ($attribute->namespaceURI) {
              $this->writer->writeAttributeNS($attribute->prefix, $attribute->localName, $attribute->namespaceURI, $attribute->nodeValue);
            }
            else {
              $this->writer->writeAttribute($attribute->nodeName, $attribute->nodeValue);
            }
          }
          $this->process($child, $remaining);
          $this->writer->endElement();
          break;

        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
          $current = $child->nodeValue;
          $current_length = drupal_strlen($current);

          $this->writer->text($this->truncateText($current, $remaining));
          $remaining -= $current_length;
          break;

        default:
          throw new Exception("Unhandled XML node type: $child->nodeType");
      }
      if ($remaining <= 0) {
        return;
      }
    }
  }

  /**
   * Return output to string.
   */
  public function __toString() {
    return $this->output;
  }

}
