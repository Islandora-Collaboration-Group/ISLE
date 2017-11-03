# Islandora Paged Content [![Build Status](https://travis-ci.org/Islandora/islandora_paged_content.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_paged_content)

## Introduction

Libraries for paged content in Islandora.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)

This module has the following optional requirements:
* [pdftotext](http://poppler.freedesktop.org) - Debian/Ubuntu `sudo apt-get install poppler-utils`
* [pdfinfo](http://poppler.freedesktop.org) -  Debian/Ubuntu `sudo apt-get install poppler-utils`

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set the path for `gs` (GhostScript), the externally accessible Djatoka URL, and the 'Solr page sequence number field' in Administration » Islandora » Solution pack configuration » Paged Content Module (admin/islandora/solution_pack_config/paged_content).

![Configuration](https://camo.githubusercontent.com/3dc4302d5d459ecf9d6b253be397d446243afde9/687474703a2f2f692e696d6775722e636f6d2f545a65423973772e706e67)

There is an option to set the page label to the page's sequence number. On ingest, each page's label will be set to its sequence number. When reordering pages, all of the page labels will be updated with the new sequence numbers.

You can also hide pages contained in a book the search results, so that only the parent object is returned.  If you use this option, make sure that you check the "Aggregate OCR?" box when ingesting your books.  
Otherwise, the book object will not receive an OCR datastream, and will not be returned in search results. 

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Paged+Content).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Diego Pino](https://github.com/DiegoPino)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

