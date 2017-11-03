# Large Image Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_large_image.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_large_image)

## Introduction

The Large Image Solution Pack loads all required Fedora objects and creates an empty collection object to accept TIFFs and JPEG2000s (JP2) and create derivatives, and also supports the installation of image viewers that can accommodate the larger resolution.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [ImageMagick](https://drupal.org/project/imagemagick)
* Kakadu (bundled with Djatoka)

*To successfully create derivative data streams, ImageMagick (for TN & JPG) and Kakadu (for JP2) need to be installed on the server.*

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the image-toolkit to use ImageMagick rather than GD in Administration > Configuration > Media > Image Toolkit (admin/config/media/image-toolkit). If GD is selected, TN and JPG datastreams will not be generated.

![Configuration](https://camo.githubusercontent.com/6ae64673716ddf1f58d0e4856d7d7a5d79845506/687474703a2f2f692e696d6775722e636f6d2f4f33735150654f2e706e67)


Select configuration options and viewer in Administration > Islandora > Large Image Collection (admin/islandora/large_image).

To use Kakadu, make sure that `kdu_compress` is available to the Apache user. Often users will create symbolic links from `/usr/local/bin/kdu_compress` to their installation of Kakadu that comes bundled with [Adore-Djatoka](http://sourceforge.net/apps/mediawiki/djatoka/index.php?title=Installation). Make sure that the required dynamic libraries that come with Kakadu are accessible to `kdu_compress` and `kdu_expand`. If they are not present, attempting to run either command from the terminal will inform you that the libraries are missing. You can also use a symbolic link from `/usr/local/lib` to include these libraries. Remember to restart the terminal so your changes take affect. Also, make sure the php settings allow for enough memory and upload size: `upload_max_filesize`, `post_max_size` and `memory_limit`.

![Configuration](https://camo.githubusercontent.com/3730f86cd795d7d989e1cbb9b5dfca5221228379/687474703a2f2f692e696d6775722e636f6d2f625335706834412e706e67)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Large+Image+Solution+Pack).

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
