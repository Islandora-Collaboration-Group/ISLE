# Tuque [![Build Status](https://travis-ci.org/Islandora/tuque.png?branch=1.x)](https://travis-ci.org/Islandora/tuque)

## Introduction

This is the [API](https://github.com/Islandora/islandora/wiki/Working-With-Fedora-Objects-Programmatically-Via-Tuque) that Islandora uses to communicate with Fedora Commons.

## Requirements

* PHP 5.3+

## Installation

Tuque is expected to be in one of two paths:

 * sites/all/libraries/tuque (libraries directory may need to be created)
 * islandora_folder/libraries/tuque

## Configuration

There is a configuration option that if set in the ini will override the control group of the RELS-EXT and RELS-INT datastreams. We default these control groups to X if the setting is not present.
Setting this to M can increase the stability and performance of Fedora.

```
[Tuque]
tuque.rels_ds_control_group = M
```

USE AT YOUR OWN RISK!

There are [issues](https://jira.duraspace.org/browse/FCREPO-849) that are inconsistent across Fedora versions and not fully explored with making the relation datastreams managed.

Tests with the Islandora UI and Fedora 3.6.2 have not shown issues.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/APPENDIX+G+-+All+About+Tuque).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jonathan Green](https://github.com/jonathangreen)
* [Diego Pino](https://github.com/DiegoPino)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
