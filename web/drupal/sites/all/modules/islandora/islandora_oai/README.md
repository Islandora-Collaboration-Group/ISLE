# Islandora OAI [![Build Status](https://travis-ci.org/Islandora/islandora_oai.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_oai)

## Introduction

This module provides an implementation of a data provider according to the Open Archives Initiative Protocol for Metadata Harvesting (OAI-PMH) for an Islandora Fedora repository with a solr index. By implementing the islandora_oai module, you can expose content (its metadata) as an OAI-PMH repository. It will then be accessible by OAI harvesters. For further OAI documentation. For more info, please see [this]( http://www.openarchives.org/OAI/openarchivesprotocol.html) link.

With this module you can: 

* Receive OAI-PMH request of service providers in form of HTTP request (GET or POST)
* Handle the OAI-PMH request
* Respond to OAI-PMH request in form of HTTP response (XML) to the service provider

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search/)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the basic details of the repository in Administration » Islandora » Islandora OAI (admin/islandora/tools/islandora-oai).

![Configuration](https://cloud.githubusercontent.com/assets/4957043/19118707/4a310ec2-8aeb-11e6-876c-dd9e116c0688.png)

## Configuration - the next page

If you select "configure" you see the following screens:

![Solr and content model config](https://cloud.githubusercontent.com/assets/4957043/19119122/be537f5a-8aec-11e6-9e47-12909df40a83.png)

* Solr date field - A datestamp to be appended to the metadata via the Solr index.
* Solr RELS-EXT collection field - Fields entered here establish the object relationship of metadata to be passed on to the harvester.
* Solr XACML role field - The site's Solr fields defining viewing permissions.
* Solr hasModel field - The site's Solr field defining an object's content model.
* Exclude Content Models - A list of content models, defined by their PID, to exclude from harvests. - if you exclude the collection content model, it disables the use of "sets" in OAi, as the name of the set is made by taking the PID of the collection and replacing the colon with an underscore.
* Exclude objects within the "islandora" namespace
* Append on dc.identifier.thumbnail to OAI_DC requests? - this only has an effect on OAI_DC output.

## Metadata Format

This section allows you to configure the settings for the OAI-PMH's metadata_prefix verb; Islandora uses XSL files to define the method for transforming your site's metadata datastreams into a format compatible with the OAI-PMH. Islandora OAI comes with two XSL files; they convert the MODS datastream of an object to either Electronic Thesis and Dissertation Metadata Standard format or Dublin Core format, which then can be served up to a harvester.
You will have to fill out and save this section for each metadata prefix you wish to enable.
Below are sample configs for a oai_dc prefix and a mods prefix.

![oai_dc setup](https://cloud.githubusercontent.com/assets/4957043/19119982/7ace7d18-8aef-11e6-857e-94a68daba0fa.png)

![mods setup](https://cloud.githubusercontent.com/assets/4957043/19120023/a27c3f6c-8aef-11e6-805e-0fb494053e11.png)

After you have exposed content types and some fields, your repository is available at /oai2

Some example requests are as follows:

* `*/oai2?verb=Identify`
* `*/oai2?verb=ListMetadataFormats`
* `*/oai2?verb=ListIdentifiers&metadataPrefix=oai_dc`
* `*/oai2?verb=GetRecord&metadataPrefix=oai_dc&identifier=PID`
* `*/oai2?verb=ListRecords&metadataPrefix=oai_dc`

Services like WorldCat expect links back to the object such as a Handle URL. If your metadata doesn't have this there are two approaches that can be used. Self transforming XSLTs can be used to add specific elements tailored to individual needs. However, there is options in configuration to append on URL values to the XML output of OAI. Each metadata prefix has an individual set of configuration. If selected, a user can define where the object URL will get appended in the output returned.

Similarily, OCLC's Digital Collection Gateway can take advatange of thumbnail URLs for rendering. This option is only currently available for oai_dc requests. If selected, a URL to the object's thumbnail will be added as a <dc:identifier.thumbnail> if the object has a thumbnail.

If existing content has already been harvested and/or the url and thumbnail are not mapping in Digital Collection Gateway, you will need to map those manually in the 'Metadata Map' for a given collection/set.

### Customization

By default the vanilla islandora_oai module provides a very basic output. It is possible to add additional content to the description field of the repository. This includes pointing at other harvesters and repositories, branding information etc. An example of how to implement these can be referenced in the [6.x version of the module](https://github.com/Islandora/islandora_oai/blob/6.x/islandora_oai.module#L534-L604).

### Notes

The original 6.x version of this module was based off of the OAI2ForCCK module located [here](http://drupal.org/project/oai2forcck).

The responses generated by this module have been validated against [Open Archives' Validation](http://www.openarchives.org/Register/ValidateSite).

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+OAI).

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
