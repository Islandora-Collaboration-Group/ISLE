
# Islandora Citeproc

## Introduction

Provides a Drupal interface to the citeproc-php library. [Cite-Proc demo page](http://gsl-nagoya-u.net/http/pub/citeproc-demo/demo.html).

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Scholar](https://github.com/islandora/islandora_scholar)
* [CSL](https://github.com/Islandora/islandora_scholar/tree/7.x/modules/csl)

Additionally, it is necessary to install the [citeproc-php](https://github.com/Islandora/citeproc-php) library into the `sites/all/libraries` directory, such that the main `CiteProc.php` file is located at `sites/all/libraries/citeproc-php/CiteProc.php`.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Enable the module via Administration » Modules (admin/modules)

## Troubleshooting/Issues

The citeproc-php library has been modified, so do not just it replace with the latest version. class_rtf.php comes from the biblio drupal project, and has been modified for
use here.

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Bryan Brown](https://github.com/bryjbrown)
* [Don Richards](https://github.com/DonRichards)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
