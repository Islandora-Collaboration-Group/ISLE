# Islandora PDF.js [![Build Status](https://travis-ci.org/Islandora/islandora_pdfjs.png?branch=7.x)](https://travis-ci.org/islandora/islandora_pdfjs)

## Introduction

An Islandora viewer module using [Mozilla PDF.js](http://mozilla.github.io/pdf.js/).

## Requirements

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [PDF.js](http://mozilla.github.io/pdf.js/)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

You also need to [Download](http://mozilla.github.io/pdf.js/getting_started/#download) and install the generic build of [PDF.js](http://mozilla.github.io/pdf.js) and move the directory to sites/libraries/pdfjs folder, or run `drush pdfjs-plugin`. 

Note: If you use the Drush command, ensure that your `.drush` directory contains the install script `islandora_pdfjs.drush.inc`. If it doesn't, move (not copy) the script from the `islandora_pdfjs` module's root directory to your `.drush` folder before you run the drush command.

## Configuration

Currently the PDF.js viewer can be used as the viewer for:

* the [PDF Solution Pack](https://github.com/Islandora/islandora_solution_pack_pdf) 
  * Administration » Islandora » Solution pack configuration » PDF Solution Pack (admin/islandora/solution_pack_config/pdf).
* the [Book Solution Pack](https://github.com/Islandora/islandora_solution_pack_book) (both the book and the page object are options)
  * Administration » Islandora » Solution pack configuration » Book Solution Pack (admin/islandora/solution_pack_config/book). 
* [Scholar](https://github.com/Islandora/islandora_scholar) 
  * Administration » Islandora » Solution pack configuration » Scholar (admin/islandora/solution_pack_config/scholar). 

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+PDF.js).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Nelson Hart](https://github.com/nhart)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)


**Note** This module requires PDF.js. PDF.js is licensed under an [Apache2 License](https://github.com/mozilla/pdf.js/blob/master/LICENSE)
