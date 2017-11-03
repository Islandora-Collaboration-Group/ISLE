# Basic Image Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_image.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_image)

## Introduction

Creates Image Collection to hold image objects. Adds support for Image objects based on JPEG/PNG/GIF image files.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [ImageMagick](https://drupal.org/project/imagemagick)

*To successfully create derivative data streams ImageMagick (TN & JPG) needs to be installed on the server.*

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the image-tool kit to use ImageMagick rather than GD in Administration > Configuration > Media > Image Toolkit (admin/config/media/image-toolkit). If GD is selected, TN and JPG datastreams will not be generated.

![Configuration](https://camo.githubusercontent.com/6ae64673716ddf1f58d0e4856d7d7a5d79845506/687474703a2f2f692e696d6775722e636f6d2f4f33735150654f2e706e67)

+Some features are disabled by default. Enable ImageMagick Advanced module.
+![0000 basic image 001](https://cloud.githubusercontent.com/assets/2738244/24003797/d6aea9da-0a3a-11e7-9c30-d40a0e5240b3.png)
If you wish to prevent image upscaling for images under 500x700 select the configuration option present at Administration > Islandora > Solution pack configuration > Basic Image Solution Pack (admin/islandora/solution_pack_config/basic_image).

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Basic+Image+Solution+Pack)

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors
Current maintainers:

* [Alan Stanley](https://github.com/ajstanley)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
