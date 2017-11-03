# Islandora Bookmark [![Build Status](https://travis-ci.org/Islandora/islandora_bookmark.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_bookmark)

## Introduction

The Islandora Bookmark module allows users to build, manage, share and track lists of objects. Manage your bookmarks under /islandora-bookmark. There are two ways to add bookmarks to your lists:

1. Through object pages: every object page will have a little tool at the bottom to bookmark the
current object.
2. Through Solr searches: Islandora Bookmark provides a SOLR display with functionality to
bookmark objects based on a custom Solr query.

# Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr](https://github.com/islandora/islandora_solr_search) (optional)


## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure general bookmark settings in Administration » Islandora » Islandora Utility Modules » Bookmark (admin/islandora/tools/islandora-bookmark).

![Configuration](https://camo.githubusercontent.com/e851eaa47d5ba712ff7e8579a91dd6da2b38ae64/687474703a2f2f692e696d6775722e636f6d2f7664747444534d2e706e67)

Enable 'Bookmark' in Administration » Islandora » Solr index » Solr Settings (admin/islandora/search/islandora_solr/settings).

Set the permissions in Administration » People (admin/people/permissions). There are permissions to administer, use, and share bookmarks. Anonymous users can never share bookmarks.

The admin page has an option to create a default bookmark list for each individual user (Users cannot delete their default lists). The first time you enable default bookmark lists, you will have to log out and log back in before you can view your default list.

If a user is anonymous, adds items to a bookmark list, and then logs in those items will be added to a new list, or appended to their default list.

### Customization

Table cells referencing the bookmarked objects in both bookmark lists and bookmark Solr results can be modified. One way is by overriding the default template file located at `theme/islandora-bookmark-object-display.tpl.php`. Another way is to use `hook_islandora_bookmark_object_markup()` to let another module define the content and markup.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Bookmark).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## FAQ

Q. How do I set up bookmarking through Solr?

A. Islandora Bookmark provides a Islandora Solr primary display which has to be enabled in the solr client admin page (admin/islandora/search/islandora_solr). To set up Islandora Solr check the module's documentation. Islandora Solr comes with a 'Display switch' block which lets you switch primary displays. When enabled, the bookmark display can be accessed by appending ?display=bookmark to the Solr results URL.

## Maintainers/Sponsors
Current maintainers:

* [Diego Pino](https://github.com/DiegoPino)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
