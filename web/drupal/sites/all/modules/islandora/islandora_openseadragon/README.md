# Islandora OpenSeadragon [![Build Status](https://travis-ci.org/Islandora/islandora_openseadragon.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_openseadragon)

# Introduction

A module to integrate OpenSeadragon and Islandora. Supports a custom Djatoka tilesource and a IIIF tilesource.

Based in spirit from the JS component of Kevin Clarke's [FreeLib-Djatoka](https://github.com/ksclarke/freelib-djatoka)

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [OpenSeadragon](https://github.com/openseadragon/openseadragon/releases/download/v2.2.1/openseadragon-bin-2.2.1.zip)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

[Download](https://github.com/openseadragon/openseadragon/releases/download/v2.2.1/openseadragon-bin-2.2.1.zip) and install the Openseadragon library to your sites/libraries folder, or run `drush openseadragon-plugin`. Openseadragon 2.2.1 is known to work well with Islandora.

Note: If you use the Drush command, it is advisable to Move (not copy) the install script to your `.drush` folder and run it.

## Configuration

### Djatoka Image Server

#### Drupal 

Set the paths for 'Djatoka server base URL' and configure OpenSeadradon in Administration » Islandora » OpenSeadragon (admin/islandora/module).

![Configuration](https://camo.githubusercontent.com/c1bf991b5cc758a4420444564a91b286007e6f6e/687474703a2f2f692e696d6775722e636f6d2f4e6566597169432e706e67)

If you have an *existing* install it's required to update Openseadragon to it's latest version. You can do this quickly 
with the provided Drush command.

```bash
drush openseadragonplugin
```

#### Apache Reverse Proxy

Reverse proxy config: We make the assumption that we (reverse) proxy Djatoka, to fix the same-origin issue.

For Apache, with Drupal running on the same box as Apache, a couple lines like:

```
ProxyPass /adore-djatoka http://localhost:8080/adore-djatoka
ProxyPassReverse /adore-djatoka http://localhost:8080/adore-djatoka
```

in the Apache config somewhere (either the main apache.conf, httpd.conf, or in and arbitrarily named `*.conf` in your Apache's conf.d directory should suffice to establish the reverse proxy.

In Debian derived systems one will need to create location entries for each proxy or remove the Deny from All in mod_proxy's conf file.

### IIIF

Any [IIIF](http://iiif.io) image server can be used the the IIIF tile source. The IIIF tile source provides a full URL to the datastream to be displayed as the IIIF `identifier`. The IIIF server needs to be configured to resolve this full URL to retrieve the image. 

The [Cantaloupe 🍈](https://medusa-project.github.io/cantaloupe/) IIIF image server can be configured to resolve these identifiers using the [`HttpResolver`](https://medusa-project.github.io/cantaloupe/manual/3.3/resolvers.html#HttpResolver) with no prefix specified.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Open+Seadragon)

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
