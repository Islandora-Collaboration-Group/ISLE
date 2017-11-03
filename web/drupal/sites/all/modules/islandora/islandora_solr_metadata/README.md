# Islandora Solr Metadata [![Build Status](https://travis-ci.org/Islandora/islandora_solr_metadata.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solr_metadata)

## Introduction

Provides an interface to construct configurations used for displaying metadata on Islandora objects.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr Search](http://github.com/Islandora/islandora_solr_search)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

The Islandora Solr Metadata module is used by selecting it to be the default metadata display viewer at Administration » Islandora » Metadata Display (admin/islandora/metadata).

![Configuration](https://camo.githubusercontent.com/f5a44185e2c1e7f81e0f76d10a885640e2281479/687474703a2f2f692e696d6775722e636f6d2f6661356f3566582e706e67)

General configuration and metadata field configurations is available at Administration » Islandora » Solr Index » Solr Index (admin/islandora/search/islandora_solr/metadata).

![Configuration](https://camo.githubusercontent.com/ae1155798564091ff4623aebe039ef962f8ce9ff/687474703a2f2f692e696d6775722e636f6d2f724b65764e4c632e706e67)

## Notes

* To use Solr Metadata with Scholar's citation/thesis cModels, please enable the `Use Standard Metadata Display` option in Administration >> Islandora >> Solution pack configuration >> Scholar (/admin/islandora/solution_pack_config/scholar).

![Configuration](https://cloud.githubusercontent.com/assets/2052902/25194782/7e9bef70-2509-11e7-9167-737ea21982d0.png)

### Customization

The backbone of this module is to allow users to select fields indexed in their Solr as what drives metadata displays. This allows for the creation of heterogenous displays pulled from many sources from something that is already easily available.

The Islandora Solr Metadata module uses templates to fuel the markup displayed when it's the defined viewer for an object. As such these are overwritable to alter the display of the metadata as seen fit.

For a more indepth look at the metadata display framework and an example module implementation see the [Islandora wiki](http://github.com/Islandora/islandora/wiki/Metadata-Display-Viewers).

It's to be noted that you can have a content model associated with more than one configuration at a time. Similarly, on objects with two content models, two or more configurations could respond to display the markup for the object. These cases are handled by merging the displays based around the weight. Take for example the case where you have two responding configurations where the first configuration contains the a and c fields and the second the b and d fields. The metadata display output would then be in the following order: a, b, c, d. As such, it's at the discretion of the user, through the creation of configurations, to determine how they want their metadata to be displayed.

## Documentation

This module's documentation is also available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Solr+Metadata).

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
