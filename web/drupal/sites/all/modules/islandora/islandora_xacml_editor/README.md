# Islandora XACML Editor [![Build Status](https://travis-ci.org/Islandora/islandora_xacml_editor.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_xacml_editor)

## Introduction

The Islandora XACML Editor provides a graphical user interface to edit XACML policies for objects in a repository or collection. It adds a new tab to each collection called Child Policy and a tab to each item called Item Policy, where permissions can be set on a per User or per Role basis for:

* Object Management: Controls who can set XACML policies for an object/collection.
* Object Viewing: Controls who can view an object/collection.
* Datastreams and MIME types: Controls who can view datastreams by DSID and MIME type.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora XACML API](https://github.com/Islandora/islandora_xacml_editor/tree/7.x/api)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

### Fedora Configuration

It may be desirable--and in fact necessary for some modules--to disable/remove one of the default XACML policies which denies any interactions with the POLICY datastream to users without the "administrator" role.

This policy is located here:
`$FEDORA_HOME/data/fedora-xacml-policies/repository-policies/default/deny-policy-management-if-not-administrator.xml`

### Solr Searching Hook

In order to comply with XACML restrictions placed on objects, a hook is used to filter results that do not conform to a searching user's roles and name. This hook will not function correctly if the Solr fields for `ViewableByUser` and `ViewableByRole` are not defined correctly as they are set in the XSLT. These values can be set through the admin page for the module.

![image](https://cloud.githubusercontent.com/assets/2371345/9816201/d7e9a1e6-5871-11e5-90a0-51381eaf8fcb.png)

### Notes

The XACML editor hooks into ingesting through the interface. When a child is added through the interface, the parent's POLICY will be applied if one exists.

If XACML policies are written or edited by hand, it may result in unexpected behaviour.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/XACML+Editor).

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
