# Islandora Solr Views [![Build Status](https://travis-ci.org/Islandora/islandora_solr_views.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solr_views)

## Introduction

Exposes Islandora Solr results through a drupal view.

Inspired by [Apache Solr Views](http://drupal.org/project/apachesolr_views).

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Apache Solr](https://lucene.apache.org/solr/) - 1.4 or higher.
* [Views (3.x)](https://www.drupal.org/project/views)

## Installation
 
Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.
   
## Configuration

Upon enabling the Drupal Views, Drupal Views UI and Islandora Solr Search modules, the Views configuration screen should be available at Administration » Structure » Views (admin/structure/views). From here, you can click 'Add new view' to start creating a Solr View.

![Configuration](https://wiki.duraspace.org/download/attachments/34646092/Show.png)

On the next screen, enter a name for your view, and from the 'Show' drop-down menu, select 'Islandora Solr'. You can now click the 'Continue & Edit' button if you'd like to manipulate the view now, or 'Save & Exit' if you would like to edit it later.

From here, most of the configuration and setup for your view is exactly the same as for any other view, with a few significant differences. Islandora Solr Views adds Solr indices to the `FIELDS`, `FILTER CRITERIA` and `SORT CRITERIA` sections.
    
## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Solr+Views).

## Troubleshooting/Issues
 
Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jordan Dukart](https://github.com/jordandukart)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
