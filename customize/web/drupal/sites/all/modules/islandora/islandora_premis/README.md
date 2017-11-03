# Islandora PREMIS [![Build Status](https://travis-ci.org/Islandora/islandora_premis.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_premis)

## Introduction

This module produces XML and HTML representations of [PREMIS](http://www.loc.gov/standards/premis/) metadata for objects in your repository. Currently, it documents all fixity checks performed on datastreams, includes 'agent' entries for your insitution and for the Fedora Commons software and, maps contents of each object's "rights" elements in DC datastreams to equivalent PREMIS "rightsExtension" elements.

PREMIS XML describing all the datastreams in an object [looks like this](https://gist.github.com/mjordan/8256978).

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)

Recommended modules:

* [Islandora Checksum](https://github.com/islandora/islandora_checksum): create checksums.
* [Islandora Checksum Checker](https://github.com/islandora/islandora_checksum_checker): periodically verifies checksums on datastreams and will populate your Islandora objects' audit logs with fixity checking 'events' that map to PREMIS.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set your institution's 'agent' settings Administration » Islandora » Islandora Utility Modules » PREMIS (admin/islandora/tools/premis).

![Configuration](https://camo.githubusercontent.com/0db86df2c2cde6c0054aa65b604064b714f683e7/687474703a2f2f692e696d6775722e636f6d2f535347613550462e706e67)

Set your permissions for 'View PREMIS metadata' and 'Download PREMIS metadata' at Administration » People » Permissions (admin/people/permissions).

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+PREMIS).

## Troubleshooting

Having problems/Solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## FAQ

## Maintainers

* [Mark Jordan](https://github.com/mjordan)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
