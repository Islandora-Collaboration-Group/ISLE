# Islandora XML sitemap [![Build Status](https://travis-ci.org/Islandora/islandora_xmlsitemap.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_xmlsitemap)

## Introduction

Add URLs for Islandora objects to the XML sitemap module's database as custom links.  When the XML sitemap module creates its sitemap it will include these custom links.

## Requirements

This module requires the following modules and library:

* [Islandora](https://github.com/islandora/islandora)
* [xmlsitemap_custom](https://drupal.org/project/xmlsitemap) (Part of XML Sitemap)
* [islandora_solr](http://github.com/Islandora/islandora_solr_search)
* [Tuque](https://github.com/islandora/tuque)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set 'Last Modified Solr Field' and 'Maximum number of Islandora links to process at once' in Administration » Islandora » XML Sitemap Integration (admin/islandora/xmlsitemap).

![Configuration](https://camo.githubusercontent.com/407972e0a2c14bafd74924992c659021b800abb0/687474703a2f2f692e696d6775722e636f6d2f455a534f4b68372e706e67)

### Notes

Admins can configure the number of pids to process plus the Solr field to sort on.

To remove or edit links you can manage them in the XML sitemap Custom Links tab (admin/config/search/xmlsitemap/custom).

We have also implemented a number of hooks to automatically add/remove links to objects, including:

* `hook_islandora_object_purged()`
* `hook_islandora_object_ingested()`
* `hook_islandora_object_modified()`
* `hook_islandora_datastream_purged()`
* `hook_islandora_datastream_ingested()`
* `hook_islandora_datastream_modified()`

Objects must be publicly (anonymously) available to be included in the XML Sitemap.  Note that the listing of Custom Links in the Drupal XML Sitemap module does not necessarily equate to inclusion in the sitemap; an Islandora object may be listed in the Custom Links table (admin/config/search/xmlsitemap/custom) and still not be in the sitemap.xml file if the object is not publicly available.  If Islandora objects are not appearing in the XML Sitemap, check the following:

1. Ensure that the Islandora XML Sitemap and the Drupal XML Sitemap Custom Links modules are enabled.
1. Ensure that the anonymous Drupal user has the "View repository objects" permission (admin/people/permissions).
1. XACML permissions override the Drupal "View repository objects" permission.  Make sure that there are no extra XACML permissions on the object.

Please also note that objects marked as "inactive", whether manually or by using the [Simple Workflow](https://github.com/Islandora/islandora_simple_workflow) module, will still be indexed by default.

Larger sites with greater than 100,000 objects may encounter issues during the sitemap building process with the default configuration, such as the process hanging around a specific number indefinitely or exiting the process entirely before completion. These users may want to try unchecking the "Prefetch URL aliases during sitemap generation" option found on the xmlsitemap admin configuration page (/admin/config/search/xmlsitemap/settings) and trying the process again.

## Documentation

This module's documentation is also available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+XML+Sitemap).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Bryan Brown](https://github.com/bryjbrown)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
