# Islandora Simple Workflow [![Build Status](https://travis-ci.org/Islandora/islandora_simple_workflow.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_simple_workflow)

## Introduction

A simple editorial workflow for Islandora. Ingested objects are inactive until approved.

When this module is enabled, ingested objects will have an inactive state. If a user has the permission "Bypass default inactive object state" the default behaviour (object with active state) will persist. The "Drupal Super User" (uid = 1) will always bypass the inactive state.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Simple Workflow objects can be managed at Administration » Islandora » Simple Workflow objects (admin/islandora/tools/simple_workflow/list).

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Simple+Workflow).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jordan Dukart](https://github.com/jordandukart)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
