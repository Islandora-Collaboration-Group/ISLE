<?php

/**
 * @file
 * File originally belongs to the biblio project.
 *
 * @todo include licence info.
 */

class RTF {

  protected $colourTable = array();
  protected $colourRGB;

  protected $fontTable = array();
  protected $fontsArray = array();
  protected $fontFace;
  protected $fontSize;

  protected $infoTable = array();
  protected $pageWidth;
  protected $pageHeight;
  protected $pageSize;
  protected $pageOrientation;
  protected $rtfVersion;
  protected $tabWidth;
  protected $document;
  protected $buffer;
  protected $inch;
  protected $cm;
  protected $mm;

  /**
   * Contructor.
   */
  public function __construct() {
    $this->inch = 1440;
    $this->cm = 567;
    $this->mm = 56.7;

    $this->fontsArray[] = array(
      "name" => "Arial",
      "family" => "swiss",
      "charset" => 0,
    );

    $this->fontsArray[] = array(
      "name" => "Times New Roman",
      "family" => "roman",
      "charset" => 0,
    );

    $this->fontsArray[] = array(
      "name" => "Verdana",
      "family" => "swiss",
      "charset" => 0,
    );

    $this->fontsArray[] = array(
      "name" => "Symbol",
      "family" => "roman",
      "charset" => 2,
    );

    $this->setDefaultFontFace(0);
    $this->setDefaultFontSize(24);
    $this->setPaperSize(5);
    $this->setPaperOrientation(1);
    $this->rtfVersion = 1;
    $this->tabWidth = 360;
  }

  /**
   * Set the default font face.
   */
  public function setDefaultFontFace($face) {
    // $font is interger.
    $this->fontFace = $face;
  }

  /**
   * Set the default font size.
   */
  public function setDefaultFontSize($size) {
    $this->fontSize = $size;
  }

  /**
   * Set the title.
   */
  public function setTitle($title = "") {
    $this->infoTable["title"] = $title;
  }

  /**
   * Set the author.
   */
  public function setAuthor($author = "") {
    $this->infoTable["author"] = $author;
  }

  /**
   * Set the operator.
   */
  public function setOperator($operator = "") {
    $this->infoTable["operator"] = $operator;
  }

  /**
   * Set the paper size.
   */
  public function setPaperSize($size = 0) {

    // 1 => Letter (8.5 x 11 inch)
    // 2 => Legal (8.5 x 14 inch)
    // 3 => Executive (7.25 x 10.5 inch)
    // 4 => A3 (297 x 420 mm)
    // 5 => A4 (210 x 297 mm)
    // 6 => A5 (148 x 210 mm)
    // Orientation considered as Portrait.
    switch ($size) {
      case 1:
        $this->pageWidth = floor(8.5 * $this->inch);
        $this->pageHeight = floor(11 * $this->inch);
        $this->pageSize = 1;
        break;

      case 2:
        $this->pageWidth = floor(8.5 * $this->inch);
        $this->pageHeight = floor(14 * $this->inch);
        $this->pageSize = 5;
        break;

      case 3:
        $this->pageWidth = floor(7.25 * $this->inch);
        $this->pageHeight = floor(10.5 * $this->inch);
        $this->pageSize = 7;
        break;

      case 4:
        $this->pageWidth = floor(297 * $this->mm);
        $this->pageHeight = floor(420 * $this->mm);
        $this->pageSize = 8;
        break;

      case 5:
      default:
        $this->pageWidth = floor(210 * $this->mm);
        $this->pageHeight = floor(297 * $this->mm);
        $this->pageSize = 9;
        break;

      case 6:
        $this->pageWidth = floor(148 * $this->mm);
        $this->pageHeight = floor(210 * $this->mm);
        $this->pageSize = 10;
        break;

    }
  }

  /**
   * Set the Paper Orientation.
   */
  public function setPaperOrientation($orientation = 0) {
    // 1 => Portrait.
    // 2 => Landscape.
    switch ($orientation) {
      case 1:
      default:
        $this->pageOrientation = 1;
        break;

      case 2:
        $this->pageOrientation = 2;
        break;

    }
  }

  /**
   * Add colour.
   */
  public function addColour($hexcode) {
    // Get the RGB values.
    $this->hex2rgb($hexcode);

    // Register in the colour table array.
    $this->colourTable[] = array(
      "red" => $this->colourRGB["red"],
      "green" => $this->colourRGB["green"],
      "blue" => $this->colourRGB["blue"],
    );
  }

  /**
   * Convert HEX to RGB (#FFFFFF => r255 g255 b255).
   */
  public function hex2rgb($hexcode) {
    $hexcode = str_replace("#", "", $hexcode);
    $rgb = array();
    $rgb["red"] = hexdec(substr($hexcode, 0, 2));
    $rgb["green"] = hexdec(substr($hexcode, 2, 2));
    $rgb["blue"] = hexdec(substr($hexcode, 4, 2));

    $this->colourRGB = $rgb;
  }

  /**
   * Convert newlines into \par.
   */
  public function nl2par($text) {
    $text = str_replace("\n", "\\par ", $text);

    return $text;
  }

  /**
   * Add a text string to the document buffer.
   */
  public function addText($text) {
    $text = str_replace("\n", "", $text);
    $text = str_replace("\t", "", $text);
    $text = str_replace("\r", "", $text);

    $this->document .= $text;
  }

  /**
   * Ouput the RTF file.
   */
  public function getDocument() {
    $this->buffer .= "{";
    // Header.
    $this->buffer .= $this->getHeader();
    // Font table.
    $this->buffer .= $this->getFontTable();
    // Colour table.
    $this->buffer .= $this->getColourTable();
    // File Information.
    $this->buffer .= $this->getInformation();
    // Default font values.
    $this->buffer .= $this->getDefaultFont();
    // Page display settings.
    $this->buffer .= $this->getPageSettings();
    // Parse the text into RTF.
    $this->buffer .= $this->parseDocument();
    $this->buffer .= "}";
    return $this->buffer;
  }

  /**
   * Get the Header.
   */
  public function getHeader() {
    $header_buffer = "\\rtf{$this->rtfVersion}\\ansi\\deff0\\deftab{$this->tabWidth}\n\n";

    return $header_buffer;
  }

  /**
   * Get the Font table.
   */
  public function getFontTable() {

    $font_buffer = "{\\fonttbl\n";
    foreach ($this->fontsArray as $fnum => $farray) {
      $font_buffer .= "{\\f{$fnum}\\f{$farray['family']}\\fcharset{$farray['charset']} {$farray['name']}}\n";
    }
    $font_buffer .= "}\n\n";

    return $font_buffer;
  }

  /**
   * Get the Colour table.
   */
  public function getColourTable() {
    $colour_buffer = "";
    if (count($this->colourTable) > 0) {
      $colour_buffer = "{\\colortbl;\n";
      foreach ($this->colourTable as $cnum => $carray) {
        $colour_buffer .= "\\red{$carray['red']}\\green{$carray['green']}\\blue{$carray['blue']};\n";
      }
      $colour_buffer .= "}\n\n";
    }

    return $colour_buffer;
  }

  /**
   * Get Information.
   */
  public function getInformation() {
    $info_buffer = "";
    if (count($this->infoTable) > 0) {
      $info_buffer = "{\\info\n";
      foreach ($this->infoTable as $name => $value) {
        $info_buffer .= "{\\{$name} {$value}}";
      }
      $info_buffer .= "}\n\n";
    }

    return $info_buffer;
  }

  /**
   * Get the default font settings.
   */
  public function getDefaultFont() {
    $font_buffer = "\\f{$this->fontFace}\\fs{$this->fontSize}\n";

    return $font_buffer;
  }

  /**
   * Get the page display settings.
   */
  public function getPageSettings() {
    if ($this->pageOrientation == 1) {
      $page_buffer = "\\paperw{$this->pageWidth}\\paperh{$this->pageHeight}\n";
    }
    else {
      $page_buffer = "\\paperw{$this->pageHeight}\\paperh{$this->pageWidth}\\landscape\n";
    }
    $page_buffer .= "\\pgncont\\pgndec\\pgnstarts1\\pgnrestart\n";
    return $page_buffer;
  }

  /**
   * Convert special characters to ASCII.
   */
  public function specialCharacters($text) {
    $text_buffer = "";
    for ($i = 0; $i < strlen($text); $i++) {
      $text_buffer .= $this->escapeCharacter($text[$i]);
    }
    return $text_buffer;
  }

  /**
   * Convert special characters to ASCII.
   */
  public function escapeCharacter($character) {
    $escaped = "";
    if (ord($character) >= 0x00 && ord($character) < 0x20) {
      $escaped = "\\'" . dechex(ord($character));
    }
    if ((ord($character) >= 0x20 && ord($character) < 0x80) || ord($character) == 0x09 || ord($character) == 0x0A) {
      $escaped = $character;
    }
    if (ord($character) >= 0x80 and ord($character) < 0xFF) {
      $escaped = "\\'" . dechex(ord($character));
    }
    switch (ord($character)) {
      case 0x5C:
      case 0x7B:
      case 0x7D:
        $escaped = "\\" . $character;
        break;
    }

    return $escaped;
  }

  /**
   * Parse the text input to RTF.
   */
  public function parseDocument() {
    $doc_buffer = html_entity_decode($this->document, ENT_QUOTES, 'UTF-8');
    $doc_buffer = utf8_decode($doc_buffer);
    $doc_buffer = $this->specialCharacters($doc_buffer);
    if (preg_match("/<ul>(.*?)<\/ul>/mi", $doc_buffer)) {
      $doc_buffer = str_replace("<ul>", "", $doc_buffer);
      $doc_buffer = str_replace("</ul>", "", $doc_buffer);
      $doc_buffer = preg_replace("/<li>(.*?)<\/li>/mi", "\\f3\\'B7\\tab\\f{$this->fontFace} \\1\\par", $doc_buffer);
    }

    $doc_buffer = preg_replace("/<p>(.*?)<\/p>/mi", "\\1\\par ", $doc_buffer);
    $doc_buffer = preg_replace("/<strong>(.*?)<\/strong>/mi", "\\b \\1\\b0 ", $doc_buffer);
    // XXX: False positive: Thinks "e" flag is in use:
    // http://drupal.org/node/750148
    // @codingStandardsIgnoreStart
    $doc_buffer = preg_replace("/<em>(.*?)<\/em>/mi", "\\i \\1\\i0 ", $doc_buffer);
    // @codingStandardsIgnoreEnd
    $doc_buffer = preg_replace("/<i>(.*?)<\/i>/mi", "\\i \\1\\i0 ", $doc_buffer);
    $doc_buffer = preg_replace("/<u>(.*?)<\/u>/mi", "\\ul \\1\\ul0 ", $doc_buffer);
    // XXX: False positive: Thinks "e" flag is in use:
    // http://drupal.org/node/750148
    // @codingStandardsIgnoreStart
    $doc_buffer = preg_replace("/<strike>(.*?)<\/strike>/mi", "\\strike \\1\\strike0 ", $doc_buffer);
    // @codingStandardsIgnoreEnd
    $doc_buffer = preg_replace("/<sub>(.*?)<\/sub>/mi", "{\\sub \\1}", $doc_buffer);
    $doc_buffer = preg_replace("/<sup>(.*?)<\/sup>/mi", "{\\super \\1}", $doc_buffer);

    $doc_buffer = preg_replace("/<h1>(.*?)<\/h1>/mi", "\\fs48\\b \\1\\b0\\fs{$this->fontSize}\\par ", $doc_buffer);
    $doc_buffer = preg_replace("/<h2>(.*?)<\/h2>/mi", "\\fs36\\b \\1\\b0\\fs{$this->fontSize}\\par ", $doc_buffer);
    $doc_buffer = preg_replace("/<h3>(.*?)<\/h3>/mi", "\\fs27\\b \\1\\b0\\fs{$this->fontSize}\\par ", $doc_buffer);

    $doc_buffer = preg_replace("/<hr(.*?)>/i", "\\brdrb\\brdrs\\brdrw30\\brsp20 \\pard\\par ", $doc_buffer);
    $doc_buffer = str_replace("<br/>", "\\par ", $doc_buffer);
    $doc_buffer = str_replace("<br />", "\\par ", $doc_buffer);
    $doc_buffer = str_replace("<tab>", "\\tab ", $doc_buffer);

    $doc_buffer = $this->nl2par($doc_buffer);

    return $doc_buffer;
  }

}
