# Objective Forms [![Build Status](https://travis-ci.org/Islandora/objective_forms.png?branch=7.x)](https://travis-ci.org/Islandora/objective_forms)

## Introduction

The Objective Forms module contains a series of functions and classes that allow Drupal forms to be treated as Fedora objects. It provides back-end support to XML Forms so that Drupal's built-in form functions and classes can be used when filling out metadata.

Some important notes:

* Each form element is assigned a unique hash Form Property to identify it, as #hash.
* Each form element that is created is stored in a registry, and it will persist though out the lifetime of the form even if it's removed from the form. Ancestry of Form Elements is stored, so if a Form Element is cloned we will be able to determine the Form Element that is was cloned from.
* Form Properties can be objects. To define new Form Properties, implement the hook `objectify_properties`.
* Forms will be auto-populated from `$form_states['values']`.
* There is a FormStorage class that can be used to store any persistent data.

## Requirements

This module requires the following modules/libraries:

* [PHP Lib](https://github.com/islandora/php_lib)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

N/A

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Objective+Forms).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Diego Pino](https://github.com/diegopino)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
