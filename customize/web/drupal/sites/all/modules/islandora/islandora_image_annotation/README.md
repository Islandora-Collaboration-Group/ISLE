# Islandora Image Annotation [![Build Status](https://travis-ci.org/Islandora/islandora_image_annotation.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_image_annotation)

## Introduction

Adds the ability to add annotations to images.  This solution pack piggy backs 
on top of other Islandora Solution Packs such as 
[Basic Image](https://github.com/Islandora/islandora_solution_pack_image/) and 
[Large Image](https://github.com/Islandora/islandora_solution_pack_large_image/) 
by adding a new tab to existing views. This tab includes the annotation tools.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr](https://github.com/Islandora/islandora_solr_search/)
* [Taxonomy](https://www.drupal.org/documentation/modules/taxonomy)
* [jQuery Update](https://www.drupal.org/project/jquery_update)

### jQuery Update

Image Annotation requires jQuery 1.8.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

To enable the annotation tab for a content model visit Administration 
» Islandora » Image Annotation (admin/islandora/image_annotation). 
From there you can choose which CModels you want to integrate the annotation 
tool with.  You will need to tell it what datastream to use as well, You should 
choose a datastream with a mimetype of image/jpeg or image/png. The Taxonomy
Column allows the annotation tool to use taxonomy terms for categories.

Annotation categories also depend on the selected radio button under Annotation 
Categories. If you want to depend on taxonomies choose administrator defined.

![Configuration](https://camo.githubusercontent.com/3c2af45923ee5ec1608098ea26fc09c107a82175/687474703a2f2f692e696d6775722e636f6d2f4348307a5673382e706e67)

### Customization

Searching:

Included is an xslt designed for gsearch.  If this xslt is included in your 
existing gsearch index xslt it will index the Annotation fields and make them 
searchable in solr.  You will also need to configure the Islandora Solr client 
so that it is aware of the new fields.  If you are using custom Solr request 
handlers you will have to update them as well.

If you have solr configured properly and have Annotation Categories set as user 
defined you will have a type ahead for the Annotations categories section.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Image+Annotation).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## FAQ

Q. Can I have annotations for a book?

A. If you want the annotations for book, do not check the book content model on
the admin page. In order for annotations for book you check the paged content 
model.

## Maintainers/Sponsors

Current maintainers:

* [Nigel Banks](https://github.com/nigelgbanks)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

