# Islandora Badges[![Build Status](https://travis-ci.org/Islandora/islandora_badges.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_badges)

## Introduction

Islandora Badges displays various metrics (and other) badges on objects. Each badge is created by a submodule. Available badges include:
* Altmetrics: display social media interactions
* Scopus: Citation counts via the Scopus database
* Web of Science: Citation counts via Web of Science
* oaDOI: Provides a link to a fulltext document for objects without a PDF datastream, via the oadoi.org API

Badges will only display on objects that have a MODS datastream and a DOI (digital object identifier). The xpath to the DOI field is configurable.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Scholar](https://github.com/Islandora/islandora_scholar)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration path is admin/islandora/tools/badges (Administration > Islandora > Islandora Utility Modules > Islandora Badges Configuration).

There are two administration fields:

* DOI XPath
     * The XPath to the DOI element e.g. /mods:mods/mods:identifier[@type="doi"] 
     * A default is included and should serve most repositories, but you can change it if yours is located elsewhere.
* Content models
     * Choose which CModels are able to display badges (Currently only applies to Altmetrics)
     
## Submodules

These modules provide the actual badges:

* [Islandora Altmetrics](modules/islandora_altmetrics/)
* [Islandora Scopus](modules/islandora_scopus/)
* [Islandora Web of Science](modules/islandora_wos/)
* [Islandora oaDOI](modules/islandora_oadio/)

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Brandon Weigel](https://github.com/bondjimbond)
* [William Panting](https://github.com/willtp87)
## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## Notes

Originally forked from [William Panting's](https://github.com/willtp87) [Islandora Altmetrics](https://github.com/Islandora/islandora_altmetrics) module by [Mark Jordan](https://github.com/mjordan) and [Brandon Weigel](https://github.com/bondjimbond) at the second iCampBC in July 2016. Much thanks to Mark for continued help and mentorship as I use this module to learn coding. Thanks also to [Marcus Barnes](https://github.com/MarcusBarnes) for demonstrating how to assign array data to variables, which allowed me to figure out the Web of Science branch.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
