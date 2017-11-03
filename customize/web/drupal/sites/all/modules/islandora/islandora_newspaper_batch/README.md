# Islandora Newspaper Batch [![Build Status](https://api.travis-ci.org/Islandora/islandora_newspaper_batch.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_newspaper_batch)

## Introduction

This module extends the Islandora batch framework so as to provide a Drush and
GUI option to add newspaper issues and pages to an existing newspaper object.

The ingest is a three-step process:

* Create the "parent" to act as a container for the newpaper to ingest into
* Preprocessing: The data is scanned and a number of entries are created in the
  Drupal database.  There is minimal processing done at this point, so preprocessing can
  be completed outside of a batch process.
* Ingest: The data is actually processed and ingested. This happens inside of
  a Drupal batch.

**Usage**

Before you can ingest a newspaper you must first create the "parent" to hold the pages. Go to [http://localhost:8000/islandora/object/islandora%3Anewspaper_collection](http://localhost:8000/islandora/object/islandora%3Anewspaper_collection)

_Manage > Overview > Add an object to this Collection > Give it a Name > Ingest_<br/>

The base ZIP/directory preprocessor can be called as a drush script (see `drush help islandora_newspaper_batch_preprocess` for additional parameters):

```
drush help islandora_newspaper_batch_preprocess
Preprocessed newspaper issues into database entries.

Options:
 --aggregate_ocr                           A flag to cause OCR to be aggregated to issues, if OCR is also being generated per-page.                     
 --content_models                          A comma-separated list of content models to assign to the objects. Only applies to the "newspaper issue"     
                                           level object.                                                                                                
 --create_pdfs                             A flag to cause PDFs to be created in newspaper issues. Page PDF creation is dependant on the configuration  
                                           within Drupal proper.                                                                                        
 --directory_dedup                         A flag to indicate that we should avoid repreprocessing newspaper issues which are located in directories.   
 --do_not_generate_ocr                     A flag to allow for conditional OCR generation.                                                              
 --email_admin                             A flag to notify the site admin when the newspaper issue is fully ingested (depends on Rules being enabled). 
 --namespace                               The namespace for objects created by this command.  Defaults to namespace set in Fedora config.              
 --parent                                  The collection to which the generated items should be added.  Only applies to the "newspaper issue" level    
                                           object. If "directory" and the directory containing the newspaper issue description is a valid PID, it will  
                                           be set as the parent. If this is specified and itself is a PID, all newspapers issue will be related to the  
                                           given PID. Required.                                                                                         
 --parent_relationship_pred                The predicate of the relationship to the parent. Defaults to "isMemberOf".                                   
 --parent_relationship_uri                 The namespace URI of the relationship to the parent. Defaults to                                             
                                           "info:fedora/fedora-system:def/relations-external#".                                                         
 --target                                  The target to directory or zip file to scan. Required.                                                       
 --type                                    Either "directory" or "zip". Required.                                                                       
 --wait_for_metadata                       A flag to indicate that we should hold off on trying to ingest newspaper issues until we have metadata       
                                           available for them at the newspaper issue level.

Aliases: inbp
```

Drush made the `target` parameter reserved as of Drush 7. To allow for backwards compatability this will be preserved.

_Note:_
* You may need to `$cd /var/www/drupal` for these drush commands to work
* --parent=islandora:{PID}

### Drush 7 and above:

`drush -v --user=admin --uri=http://localhost islandora_newspaper_batch_preprocess --type=directory --scan_target=/path/to/issues --namespace=dailyplanet --parent=islandora:1`

### Drush 6 and below:

`drush -v -u 1 --uri=http://localhost islandora_newspaper_batch_preprocess --type=directory --target=/path/to/issues --namespace=dailyplanet --parent=islandora:1`

This will populate the queue (stored in the Drupal database) with base entries. Note that the --parent parameter must be a newspaper, not a collection. 

Batches must be broken up into separate directories (or Zip files), such that each directory at the "top" level (in the target directory or Zip file) represents a newspaper issue. Newspaper pages are their own directories inside of each newsppaper issue directory.

Files are assigned to object datastreams based on their basename, so a folder structure like:

```terminal
my_batch/
.
└── issue1
    ├── 1
    │   └── OBJ.tif
    ├── 2
    │   └── OBJ.tif
    └── MODS.xml
```

would result in a two-page newspaper issue. Other files, with base names corresponding to datastream IDs, can be included in each page subfolder, such as JP2.jp2, OCR.txt, and TN.jpg:

```terminal
my_batch/
.
└── issue1
    ├── 1
    │   ├── JP2.jp2
    │   ├── JPG.jpeg
    │   ├── OBJ.tif
    │   ├── OCR.asc
    │   └── TN.jpeg
    ├── 2
    │   ├── JP2.jp2
    │   ├── JPG.jpeg
    │   ├── OBJ.tif
    │   ├── OCR.asc
    │   └── TN.jpeg
    └── MODS.xml
```

If files like these are included, they will be used as the content of their respective datastreams, and the batch process will not recreate the datastreams, which can speed up batch ingests substantially.

A file named --METADATA--.xml can contain either MODS, DC or MARCXML which is used to fill in the MODS or DC streams (if not provided explicitly). Similarly, --METADATA--.mrc (containing binary MARC) will be transformed to MODS and then possibly to DC, if neither are provided explicitly.

If no MODS is provided at the newspaper issue level - either directly as MODS.xml, or transformed from either a DC.xml or the "--METADATA--" file discussed above - the directory name will be used as the title.

### The queue of preprocessed items can then be processed:

`drush -v --user=admin --uri=http://localhost islandora_batch_ingest`

**Sample Issue-level MODS.xml file**

Here is a sample MODS file describing a newspaper issue. Note that it does not contain a `<relatedItem>` element as a direct child of `<mods>`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
    <titleInfo>
      <title>Canadian Jewish Review, June 1, 1928</title>
    </titleInfo>
    <originInfo>
      <place>
        <placeTerm>Toronto, Ontario</placeTerm>
      </place>
      <publisher>Canadian Jewish Review </publisher>
      <dateIssued encoding="iso8601">1928-06-01</dateIssued>
    </originInfo>
    <language>
      <languageTerm>eng</languageTerm>
    </language>
    <subject>
      <topic>Jews, Canadian -- Ontario -- Toronto -- History -- Newspapers</topic>
      <topic>Jews, Canadian -- Quebec -- Montreal -- History -- Newspapers</topic>
      <topic>Jews -- History -- 20th century -- Newspapers</topic>
      <topic>Jews -- Canada -- Periodicals</topic>
      <topic>Canada -- History -- 20th century -- Newspapers</topic>
      <topic>Ontario -- History -- 20th century -- Newspapers</topic>
      <topic>Quebec -- History -- 20th century -- Newspapers</topic>
      <topic>Toronto (Ont.) -- History -- 20th century -- Newspapers</topic>
      <topic>Montreal (Que.) -- History -- 20th century -- Newspapers</topic>
    </subject>
    <identifier>Cjewish-1928-06-01</identifier>
</mods>
```

*You may get a warning. "Failed to get issued date from MODS for dailyplanet:1"*<br/>
Explanation found in the [wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Newspaper+Batch)

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Batch](https://github.com/Islandora/islandora_batch)
* [Islandora Newspaper Solution Pack](https://github.com/Islandora/islandora_solution_pack_newspaper)


# Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

N/A

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Newspaper+Batch).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [William Panting](https://github.com/willtp87)

Sponsors:

* [Simon Fraser University Library](http://www.lib.sfu.ca/)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
