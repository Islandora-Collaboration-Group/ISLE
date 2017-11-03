# Islandora Populator [![Build Status](https://travis-ci.org/Islandora/islandora_populator.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_populator)

## Introduction

A framework to facilitate the population of XML forms during the usual ingest workflow. When an object is being an ingested, a page will be added as part of the ingest process that will provide the option to use the populator. Populator options include DOI, EndNote, PMID, and RIS. Populator also works with Islandora XSLT Populator, which can be accessed at /admin/islandora/tools/xslt_populator.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Restrict to certain content models in in Administration » Islandora » Populator Configuration (admin/islandora/populator). This will remove the ingest step from content models not selected.

![image](https://cloud.githubusercontent.com/assets/2371345/9911924/0419b852-5c7b-11e5-903e-7a0f76c88b9c.png)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Populator).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jordan Dukart](https://github.com/jordandukart)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
