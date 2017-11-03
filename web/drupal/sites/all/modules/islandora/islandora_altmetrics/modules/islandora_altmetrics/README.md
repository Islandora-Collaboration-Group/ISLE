# Islandora Altmetrics

## Introduction

Islandora Altmetrics provides a Altmetrics badge on objects, indicating social media interactions. Applies to any object with a DOI, within selected content models. For use with Islandora Badges.

This modules uses the [Altmetrics API](http://api.altmetric.com/). Before using this module one should familiarize oneself with the licensing.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Scholar](https://github.com/Islandora/islandora_scholar)
* [Islandora Badges](../../)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration path is admin/islandora/tools/badges/altmetrics (Administration > Islandora > Islandora Utility Modules > Islandora Badges Configuration > Altmetrics).

There are two administration options:

* Altmetric Badge
     * provide one of the [badge types](http://api.altmetric.com/embeds.html#badge-types) defined by Altmetrics. Default is the small rectangular badges. 
* Altmetrics Popover
     * defines the location where the [popover displays](http://api.altmetric.com/embeds.html#popovers).

The module provides a block, Islandora Altmetrics, that can be placed in a block region.

![](https://raw.githubusercontent.com/wiki/dmoses/islandora_altmetrics/islandora_altmetrics_block.png)

The block comes with some default configurations.

![](https://raw.githubusercontent.com/wiki/dmoses/islandora_altmetrics/islandora_altmetrics_block_config.png)

Once enabled the badge is displayed on objects that have a DOI as configured and some metrics.  If it has a DOI but does not display,  the article does not currently have any metrics.

![](https://raw.githubusercontent.com/wiki/dmoses/islandora_altmetrics/islandora_altmetrics_display.png)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Altmetrics).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [William Panting](https://github.com/willtp87)
* [Brandon Weigel](https://github.com/bondjimbond)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## Notes

Built by [William Panting](https://github.com/willtp87) and tested by [Don Moses](https://github.com/dmoses) during the first Islandora Conference's Hackfest - August 7, 2015.
Absorbed into Islandora Badges by [Brandon Weigel](https://github.com/bondjimbond) during the 2016 iCamp BC in Vancouver.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
