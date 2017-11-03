# Islandora Checksum [![Build Status](https://travis-ci.org/Islandora/islandora_checksum.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_checksum)

## Introduction

A simple module to allow repository managers to enable the creation of a checksum for objects. If enabled, the following checksum algorithms are available: MD5, SHA-1, SHA-256, SHA-384, SHA-512. 

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Enable and set checksum type in Administration » Islandora » Islandora Utility Modules » Checksum (admin/islandora/tools/checksum). To retroactively enable checksums on existing objects, enable and set checksum type if you have not already done so, and choose a collection and click on the 'Enable' button. **Note**: Enabling checksums retroactively regenerates derivatives for all objects in the target collection but only for those without checksum properties.

![Configuration](https://wiki.duraspace.org/download/attachments/69833526/screenshot_checksum_admin.png?version=1&modificationDate=1509042384043&api=v2)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Checksum).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Mark Jordan](https://github.com/mjordan)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

Also include any Travis gotcha's here. 

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
