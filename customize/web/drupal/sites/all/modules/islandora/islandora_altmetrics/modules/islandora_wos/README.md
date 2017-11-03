# Islandora Web of Science

## Introduction

Islandora Web of Science provides block which displays Web of Science citation counts on objects for use with Islandora Badges.

Take note: 
* You can only get AMR credentials if your institution is a Web of Science subscriber, BUT
* Citation counts and links (including list of citing articles) can be viewed by the general public

### Required credentials
Uses the Web of Science [Article Match Retrieval (AMR)](http://ipscience-help.thomsonreuters.com/LAMRService/WebServiceOperationsGroup/requestAPIWoS.html) service. Credentials are required to use the API, but not to view the results. Web of Science subscribers are entitled to credentials - contact your account manager to get set up.

Badges will only display on objects that have a MODS datastream and a DOI (digital object identifier).

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Scholar](https://github.com/Islandora/islandora_scholar)
* [Islandora Badges](../../)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration path is admin/islandora/tools/badges/wos (Administration > Islandora > Islandora Utility Modules > Islandora Badges Configuration > Web of Science).

Administration fields:

* Web of Science username/password
     * No default is provided, and the service won't work without these. 
     * If you are a WOS subscriber, you can request AMR credentials from your account manager.
* Web of Science badge type
     * You can choose between plain text or an automatically-generated image
     * Plain text is offered for custom styling. The text badge has the CSS clas "wos_badge" to make this easier.

## Styling

The Web of Science block (text badge option) creates a div with the class "wos_badge" to facilitate CSS styling.
The Drupal [Block Class module](https://www.drupal.org/project/block_class) helps to facilitate block positioning with CSS.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Brandon Weigel](https://github.com/bondjimbond)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
