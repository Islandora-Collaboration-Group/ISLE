# Islandora Scopus

## Introduction

Islandora Scopus displays a badge showing how many articles in Scopus have cited your object. For use with Islandora Badges.

You do NOT need to be a Scopus subscriber to use this module. You can [get a Scopus API key for free](http://dev.elsevier.com/sc_apis.html).

Uses the [Scopus Abstract Citation Count API](https://api.elsevier.com/documentation/AbstractCitationCountAPI.wadl). A default API key is included, but this is intended for demo purposes only and is very limited. You should [get your own free API key from Scopus](https://dev.elsevier.com/apikey/create).

Badges will only display on objects that have a MODS datastream and a DOI (digital object identifier). 

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Scholar](https://github.com/Islandora/islandora_scholar)
* [Islandora Badges](../../)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration path is admin/islandora/tools/badges/scopus (Administration > Islandora > Islandora Utility Modules > Islandora Badges Configuration > Scopus).

There is one administration field:

* Scopus API Key
     * Required to acquire citation counts. Do not use the included default in production - it's for demo purposes only. 
     * You can get a [free API key](https://dev.elsevier.com/apikey/create) without subscribing

Once enabled, a block is available, which displays a badge on pages that have a DOI as configured and some metrics.  If it has a DOI but no badge appears, then the article does not currently have any metrics.

## Styling

The Drupal [Block Class module](https://www.drupal.org/project/block_class) helps to facilitate block positioning with CSS.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Brandon Weigel](https://github.com/bondjimbond)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
