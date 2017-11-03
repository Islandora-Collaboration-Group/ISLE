# Islandora Importer [![Build Status](https://travis-ci.org/Islandora/islandora_importer.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_importer)

## Introduction 

Made primarily for ingesting objects described by MODS and DC, and currently imports ZIP files.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Basic Collection](https://github.com/Islandora/islandora_solution_pack_collection)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Select whether or not to 'Use filename as Datastream label' in Administration » Islandora » Zip Importer (admin/islandora/zipimporter).

![Configuration](https://camo.githubusercontent.com/96bdf5509afc3522ab1302b94a70606a013d79e8/687474703a2f2f692e696d6775722e636f6d2f7a613148706d712e706e67)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Importer).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## FAQ

Q. How does Importer interact with XACML POLICY datstreams?

A. It doesn't. A POLICY datastream will not be applied to objects ingested via Zip File Importer on ingest, even if the policy of the parent collection is set to apply to all new objects ingested into the collection. You can retroactively apply a policy to objects imported via Zip File Importer by updating the policy on the parent collection and telling it to apply to "all children of this collection."

## Maintainers/Sponsors

Current maintainers:

* [Adam Vessey](https://github.com/adam-vessey)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
