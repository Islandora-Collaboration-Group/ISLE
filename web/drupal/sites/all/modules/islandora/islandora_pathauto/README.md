# Islandora Pathauto [![Build Status](https://travis-ci.org/Islandora/islandora_pathauto.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_pathauto)

## Introduction

Exposes Islandora objects to the alias-creating tools of [Pathauto](https://www.drupal.org/project/pathauto). 

## Requirements

* [Pathauto](https://www.drupal.org/project/pathauto)
* [Token](https://www.drupal.org/project/token)
* [Islandora](https://github.com/Islandora/islandora)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the path patterns using the usual Pathauto interface, at Administration » Configuration » Search and Metadata » URL Aliases » Patterns (admin/config/search/path/patterns). 

![Pathauto Configuration](https://raw.githubusercontent.com/wiki/Islandora/islandora_pathauto/images/sample-pathauto-configuration.png)

You can set one pattern to apply to all Islandora objects, and/or select individual content models to have custom path 
formats. To enable custom paths for specific content models, first enable the desired content models at Administration »
 Islandora » Islandora Utility Modules » Pathauto (admin/islandora/tools/islandora-pathauto).

![Configuration](https://raw.githubusercontent.com/wiki/Islandora/islandora_pathauto/images/islandora-pathauto-configuration.png)

Aliases can include the object's pid (`[fedora:pid]`), the Fedora label (`[fedora:label]`), the namespace (`[fedora:namespace]`), and/or the pid without the namespace (`[fedora:shortpid]`). See the documentation for [Pathauto](https://www.drupal.org/documentation/modules/pathauto) for more information on creating aliases, and read the FAQ below.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Pathauto).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## FAQ

Q. Why don't the datastreams also follow the alias of the object?

A. With pathauto alone, datastreams are NOT accessible at [object alias]/datastream/DSID. To get this functionality, enable the Drupal moodule [Sub-pathauto](https://www.drupal.org/project/subpathauto) and configure the maximum depth of sub-paths to be at least 3.

Q. Why is my object still visible at /islandora/object/PID?

A. Drupal aliases don't negate internal paths like islandora/object/PID. If you want the original islandora URLs to resolve (i.e. redirect) to the aliases, then enable [Global Redirect](https://www.drupal.org/project/globalredirect).

Q. Where'd the colon in my PID go?

A. By default, Pathauto removes punctuation such as the colon (:) from paths before creating aliases. This will result in PIDs that look like islandora123; if this is undesirable then configure the Pathauto setting under "punctuation"  at admin/config/search/path/settings to not remove the colon.
 
Q. Can I use this module to make the colon to show up as ':' instead of '%3A'?

A. No. Sorry. This happens deep in Drupal's internal link-writing code. 

Q. What if I have multiple content models for the same object? Can I give it multiple aliases?

A. No. Multiple aliases for the same object are not supported. All objects should have at most one of their content models enabled for pathauto.

## Maintainers/Sponsors

Current maintainers:

* [Nelson Hart](https://github.com/nhart)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
