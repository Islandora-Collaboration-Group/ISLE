# Book Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_book.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_book)

## Introduction

Allows for the creation of book and page objects (islandora:bookCModel,islandora:pageCModel). Books are essentially a collection of pages, containing only metadata and optionally a PDF file of the book. A book's PDF can be generated from each page. Pages are based on an uploaded tiff of the page. From the uploaded TIFF its possible to generate images for use in the "Islandora Internet Archive Bookreader". It is also possible to generate PDF files per page. OCR and OCR coordinate data can also be generated from the uploaded TIFF.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Paged Content](https://github.com/Islandora/islandora_paged_content)
* [Islandora Large Image Solution Pack](https://github.com/Islandora/islandora_solution_pack_large_image)
* [Islandora OCR](https://github.com/Islandora/islandora_ocr)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Select configuration options for page derivatives, Parent Solr Field, and select a viewer for the book object and page objects in Administration » Islandora » Solution pack configuration (admin/islandora/solution_pack_config/book).

![Configuration](https://camo.githubusercontent.com/e913af25f82dd8ff640dd11b337f64b5a9dea62f/687474703a2f2f692e696d6775722e636f6d2f3749434a66655a2e706e67)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Book+Solution+Pack).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors
Current maintainers:

* [Rosie Le Faive](https://github.com/rosiel)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
