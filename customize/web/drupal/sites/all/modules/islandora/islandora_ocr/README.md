# Islandora OCR [![Build Status](https://travis-ci.org/Islandora/islandora_ocr.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_ocr)

## Introduction

This module acts a Toolkit for generating OCR and word coordinate information. At the moment it relies exclusively on Tesseract to generate this information.

### Tesseract

Tesseract is an OCR engine that was developed at HP Labs between 1985 and 1995. It is currently being developed at Google. Recognized as one of the most accurate open source OCR engines available, Tesseract will read binary, grey, or colour images and output text.

A TIFF reader that will read uncompressed TIFF images is also included. Islandora Book Solution Pack currently uses Tesseract version 3.02.02, which can be obtained from the project home page. Lower versions are not supported.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Tesseract](https://github.com/tesseract-ocr/tesseract/wiki)
* [ImageMagic](http://www.imagemagick.org/) (Optional, Required for OCR preprocessing)

Tesseract installation will differ depending on your operating system; please see the
[Tesseract Wiki](https://github.com/tesseract-ocr/tesseract/wiki) for detailed instructions.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

In Administration » Islandora » Islandora Utility Modules » OCR Tool (admin/islandora/tools/ocr), configure the following:

* Set the path for Tesseract
* Select languages available for OCR
* Enable/disable Solr Fast Vector Highlighting
* Set Solr field containing OCR text and the maximum number of results to return in a Solr query

![Configuration](https://camo.githubusercontent.com/0c1fd39bad0200eb1bb0ed36ae761dfe50665ba6/687474703a2f2f692e696d6775722e636f6d2f4c386e704f61502e706e67)

### Solr result highlighting

To have Islandora viewers recognize Solr search results and highlight them, one will need to configure Solr to index the HOCR in a particular fashion.

The field that the HOCR is stored in must have the following attributes: `indexed="true" stored="true" termVectors="true" termPositions="true" termOffsets="true"`

Each text node of each element in the HOCR datastream must be placed in order in a single value for the Solr field with all whitespace sub strings normalized to a single space.

Any objects that were previously ingested but require this functionality will need to be re-indexed.

[XSLT Reference Implementation for GSearch](https://github.com/discoverygarden/islandora_transforms/blob/master/XML_text_nodes_to_solr.xslt)


### Tesseract

Tesseract provides many languages which can be downloaded from [here](https://github.com/tesseract-ocr/tessdata).

To install just unzip them in your tessdata directory, typically located at `/usr/local/share/tessdata`

If you want to add your own langauges or train your Tesseract for your specific needs please review the documentation [here](https://github.com/tesseract-ocr/tesseract/wiki/TrainingTesseract)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+OCR).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Daniel Aitken](https://github.com/qadan)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
