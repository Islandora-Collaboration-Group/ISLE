# Islandora JW Player [![Build Status](https://travis-ci.org/Islandora/islandora_jwplayer.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_jwplayer)

## Introduction

A Islandora viewer module using [JW Player](http://www.jwplayer.com/).

**NOTE**: JW Player is free for non commercial use.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Libraries API](https://drupal.org/project/libraries)
* [JW Player version 6](https://github.com/jwplayer/jwplayer/releases/tag/v6.12.0)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

Install the JW Player available [here](https://github.com/jwplayer/jwplayer/releases/tag/v6.12.0) to your `sites/all/libraries` folder.  Build instructions are available on JWPlayer's [README](https://github.com/jwplayer/jwplayer#build-instructions).

### Notes

You must have previously registered for an account with JWPlayer when version 6 was being offered to legally use the library.  Registering now only licenses you to use version 7 as per JWPlayer's [terms of service](https://www.jwplayer.com/tos/).  If you are a new user looking to display video content, please use the [Islandora Video.js](https://github.com/Islandora/islandora_videojs) module.

## Configuration

Select JW Player as a viewer in Administration » Islandora » Solution Pack Configuration » Video Solution Pack (admin/islandora/solution_pack_config/video) and/or Administration » Islandora » Solution Pack Configuration » Audio Solution Pack (admin/islandora/solution_pack_config/audio).

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+JWPlayer).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jared Whiklo](https://github.com/whikloj)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
