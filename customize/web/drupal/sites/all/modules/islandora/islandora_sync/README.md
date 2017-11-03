# Islandora Sync [![Build Status](https://travis-ci.org/Islandora/islandora_sync.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_sync)

## Introduction

Islandora Sync enables event-based synchronization of content between Fedora Commons and Drupal. Fedora content models may be mapped to Drupal node types and Fedora datastreams and their XML contents to Drupal fields. A more in-depth overview of the module's use and configuration can be found [here](https://github.com/Islandora/islandora_sync/wiki/Islandora-Sync-Use-and-Configuration).

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Field API](https://api.drupal.org/api/drupal/modules!field!field.module/group/field/7)
* [Field UI](https://api.drupal.org/api/drupal/modules!field!field.module/group/field/7)
* [Relation](https://drupal.org/project/relation)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

Islandora Sync Relation provides integration with the [Relation module](www.drupal.org/project/relation) for syncing Fedora object to object relationships as Drupalnode to node relationships.

Islandora Sync Field Collection integrates with the Field Collection module to allow for repeating groups of fields, similar to tabs of Islandora XML Forms.

## Configuation

Node type mappings to Fedora content models and related settings may be configured at
/admin/structure/types/manage/<node_bundle_machine_name>/fedora
To sync XML datastreams, XML configuration is done at /admin/islandora/tools/sync/xml-datastreams/

Features integration is provided to allow for programmatic deployment of configurations.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## FAQ

Q. Why can't I sync managed datastreams?

A. A Drupal issue may present problems when syncing managed datastreams from Fedora to Drupal, see [this](http://drupal.org/node/1443158) issue for a patch.

## Maintainers/Sponsors
Current maintainers:

* [Mitch MacKenzie](https://github.com/mitchmac)

This project has been sponsored by:

* UCLA Library.

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
