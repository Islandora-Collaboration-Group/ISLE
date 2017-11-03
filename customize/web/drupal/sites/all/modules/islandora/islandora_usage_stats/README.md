# Islandora Usage Stats [![Build Status](https://travis-ci.org/Islandora/islandora_usage_stats.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_usage_stats)

## Introduction

A module for Drupal 7 to track views and downloads of Islandora items. Features include:

* Toggle to ignore common bots
* View count uses session variables and defaults to a 5 minute cooldown for repeated requests
* Access log for all views and downloads
* IP Exclusion list to prevent artificially inflating counts while testing/developing/administrating
* Several customizable blocks to display metrics
* Report generating interface
* Object log views integration

Note:

* Does **not** respect XACML or namespace restrictions.
* This is a server-side tracking solution, as such a caching layer could impact it.  If this is impacting you a [solution](https://github.com/discoverygarden/islandora_ga_reports) using JavaScript may work better.

## Requirements

This module requires the following modules/libraries:

* [Tuque](https://github.com/islandora/tuque)
* [Islandora](https://github.com/islandora/islandora)
* [Islandora basic collection](https://github.com/Islandora/islandora_solution_pack_collection)
* [Views (3.x)](https://www.drupal.org/project/views)
* [Date](https://www.drupal.org/project/date)
* [Datepicker](https://www.drupal.org/project/datepicker)
* [Views Data Export](https://www.drupal.org/project/views_data_export)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration options are available at Islandora » Islandora Utility Modules » Islandora Usage Stats Settings (admin/islandora/tools/islandora_usage_stats).

![Configuration](https://raw.githubusercontent.com/wiki/islandora/islandora_usage_stats/images/usage_stats_configuration.jpg)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Usage+Stats).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Work based on code from https://github.com/roblib/islandora_scholar_upei and the scholar_tracking module for Drupal 6. Iterated on by Ryerson University Library and Archives (RULA) and discoverygarden inc.

Current maintainers:

* [Bryan Brown](https://github.com/bryjbrown)

Sponsors:

* [METRO](http://metro.org/)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
