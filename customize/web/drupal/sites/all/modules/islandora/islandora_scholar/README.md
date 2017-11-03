# Islandora Scholar [![Build Status](https://travis-ci.org/Islandora/islandora_scholar.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_scholar)

## Introduction

Based on the UPEI scholar module, Islandora Scholar is designed to implement a feature-rich Institutional Repository.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr](https://github.com/Islandora/islandora_solr_search)
* [Bibutils](https://github.com/Islandora/islandora_scholar/tree/7.x/modules/bibutils) (included in /modules)
* [Citeproc](https://github.com/Islandora/islandora_scholar/tree/7.x/modules/citeproc) (included in /modules)
* [CSL](https://github.com/Islandora/islandora_scholar/tree/7.x/modules/csl) (included in /modules)

Additionally, as this module requires the Citeproc module, it is necessary to install the [citeproc-php](https://github.com/Islandora/citeproc-php) library into the `sites/all/libraries` directory, such that the main `CiteProc.php` file is located at `sites/all/libraries/citeproc-php/CiteProc.php`. More information is available in [Citeproc's README.md file](https://github.com/Islandora/islandora_scholar/blob/7.x/modules/citeproc/README.md).

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set the path for `Sherpa/RoMEO` in Administration » Islandora » Solution pack configuation » Scholar (admin/islandora/solution_pack_config/scholar).

![image](https://cloud.githubusercontent.com/assets/2738244/19038481/b1391c48-8949-11e6-9db9-3f681380c65f.png)

Islandora 7 has re-defined how we are displaying citations to the user. As such, existing citations should be updated using the provided Drush script. The command creates PDF derivatives for any existing attached PDFs as the Google PDF Viewer has been removed in favor of displaying just the PREVIEW datastream.

This can be done by running: 
`drush -u 1 islandora-scholar-update-citations`

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Scholar).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Bryan Brown](https://github.com/bryjbrown)
* [Don Richards](https://github.com/DonRichards)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

