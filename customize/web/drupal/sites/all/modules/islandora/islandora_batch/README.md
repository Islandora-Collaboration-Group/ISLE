# Islandora Batch [![Build Status](https://travis-ci.org/Islandora/islandora_batch.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_batch)

## Introduction

This module implements a batch framework, as well as a basic ZIP/directory ingester.

The ingest is a two-step process:

* Preprocessing: The data is scanned, and a number of entries created in the
  Drupal database.  There is minimal processing done at this point, so it can
  complete outside of a batch process.
* Ingest: The data is actually processed and ingested. This happens inside of
  a Drupal batch.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)

Additionally, installing and enabling [Views](https://drupal.org/project/views)
will allow additional reporting and management displays to be rendered.


# Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

After you have installed and enabled the Islandora Batch module, go to Administration » Islandora » Islandora Utility Modules » Islandora Batch Settings (admin/islandora/tools/batch) to configure the module.

![Configuration menu](https://cloud.githubusercontent.com/assets/10052068/18972680/23935662-8668-11e6-8a21-4c52d7aac69f.png)

You should make sure that the path to your java executable is correct.  Optionally, if you have the Drupal Views module enabled, you can also have the module link back to the Batch Queue in its results messages

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Batch)

### Usage

The base ZIP/directory preprocessor can be called as a drush script (see `drush help islandora_batch_scan_preprocess` for additional parameters):

Drush made the `target` parameter reserved as of Drush 7. To allow for backwards compatability this will be preserved.
The `target` option requires the full path to your archive from root directory. e.g. /var/www/drupal/sites/archive.zip

Drush 7 and above:

`drush -v -u 1 --uri=http://localhost islandora_batch_scan_preprocess --type=zip --scan_target=/path/to/archive.zip`

Drush 6 and below:

`drush -v -u 1 --uri=http://localhost islandora_batch_scan_preprocess --type=zip --target=/path/to/archive.zip`

This will populate the queue (stored in the Drupal database) with base entries.

For the base scan, files are grouped according to their basename (without extension). DC, MODS or MARCXML stored in a *.xml or binary MARC stored in a *.mrc will be transformed to both MODS and DC, and the first entry with another extension will be used to create an "OBJ" datastream. Where there is a basename with no matching .xml or .mrc, some XML will be created which simply uses the filename as the title.

The queue of preprocessed items can then be processed:

`drush -v -u 1 --uri=http://localhost islandora_batch_ingest`

A fuller example, which preprocesses large image objects for inclusion in the collection with PID "yul:F0433", is:

Drush 7 and above:

`drush -v -u 1 --uri=http://digital.library.yorku.ca islandora_batch_scan_preprocess --content_models=islandora:sp_large_image_cmodel --parent=yul:F0433 --parent_relationship_pred=isMemberOfCollection --type=directory --scan_target=/tmp/batch_ingest`

Drush 6 and below:

`drush -v -u 1 --uri=http://digital.library.yorku.ca islandora_batch_scan_preprocess --content_models=islandora:sp_large_image_cmodel --parent=yul:F0433 --parent_relationship_pred=isMemberOfCollection --type=directory --target=/tmp/batch_ingest`

then, to ingest the queued objects:

`drush -v -u 1 --uri=http://digital.library.yorku.ca islandora_batch_ingest`

### Customization

Custom ingests can be written by [extending](http://github.com/Islandora/islandora_batch/wiki/How-To-Extend) any of the existing preprocessors and batch object implementations. Checkout the [example implemenation](http://github.com/Islandora/islandora_batch/wiki/Example-Implementation-Tutorial) for more details.

### Clearing the semaphore table

If a user kills a Drush batch ingest, or a batch ingest initiated via the web GUI dies for some reason, it is impossible to start another batch ingest until the Islandora Batch entry in Drupal's `semaphore` table expires. You may clear this entry manually within your database, but doing so may impact other batch ingest jobs that are running. If you are sure no other batch ingest jobs are running, delete the row from Drupal's `semaphore` table where the `name` is 'islandora_batch_ingest'.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Diego Pino](https://github.com/DiegoPino)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
