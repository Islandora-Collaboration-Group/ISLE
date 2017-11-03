# Islandora Checksum Checker [![Build Status](https://travis-ci.org/Islandora/islandora_checksum_checker.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_checksum_checker)

## Introduction

This module verifies the checksums derived from Islandora object datastreams and adds a PREMIS 'fixity check' entry to the object's audit log for each datastream checked. Please note that adding this entry updates the object (specifically, it changes the object's lastModifiedDate).

Islandora Checksum Checker needs to be periodically triggered using Drupal's cron functionality (or an alternative like [Elysia Cron](https://drupal.org/project/elysia_cron), or using an operating-system-level scheduler like Linux's crontab to run a drush command (documented below). 

With each run, the module performs checksum verification on a configurable list of object datastreams. When it has checked the datastreams of all objects (from oldest to newest), it will start from the beginning (i.e. with the oldest object in your repository) and repeat the verification cycle.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Checksum](https://github.com/islandora/islandora_checksum) (Optional)

This module is only useful if you use Fedora Commons to generate checksums on datastreams. The easiest way to have Fedora Commons generate checksums is to install and enable the Islandora Checksum module.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set the cron method, number of objects to check per cron run, datastram to check, who to sent report to in Administration » Islandora » Islandora Utility Modules » Checksum checker (admin/islandora/tools/checksum_checker).

![Configuration](https://camo.githubusercontent.com/c5e3d71e0ade7b3da4628d662017c5b6774e9ea8/687474703a2f2f692e696d6775722e636f6d2f7359366f7634412e706e67)

The two most common options for scheduling the verification are:

1. choose 'Drupal cron' in the module's admin settings and make sure that you have cron running on your site, and 

2. choose 'drush script' in the module's admin settings and set up a scheduled job using a utility like Linux's cron to trigger the verification via drush.

Option 1 is simplest because it requires no additional configuration but only works if all of the objects in your Islandora repository are viewable by the Drupal 'anonymous' user (since Drupal 7's cron runs as anonymous).

Option 2 is necessary if any of your objects are not viewable by anonymous. It also has the advantage of running the verification process independently from other tasks initiated be Drupal's built-in cron.

The drush command you need to run is `drush run-islandora-checksum-queue`. You should include drush's --root and --user options to define the path to your Drupal installation's root, and an Islandora user account that has privileges to view all datastreams, respectively. A typical Linux crontab entry (in this case, to run every hour) is:

```
  0 * * * * /usr/bin/drush --root=/path/to/drupal --user=fedoraAdmin run-islandora-checksum-queue
```

### Frequency of verification

How often you should run this command will depend on several factors, including how many objects you have in your Islandora repository and how many days or months you will tolerate between reverification of the same object's datastream checksums.

Assuming that you configure this module to check 50 objects every time it runs and that you have 10,000 objects in your Islandora repository, all objects will be checked every 8 days if you configure it to run every hour. If you configure this module to run every 6 hours, all objects will be checked every 50 days. 

Also, since the results of the verification are recorded in each object's audit log, the more often you verify checksums, the larger the audit logs (and therefore the objects themselves) become. Each time a datastream is checked, the object's audit log grows by about 450 bytes. An object that has five datastreams that are all being checked will grow by about 4.5 kB/month if it is checked twice a month. A 10,000-object repository will use about 43 MB of disk every month just to store the results of routine checksum verification in the objects' audit logs.

In addition, each time a datastream's checksum is verified, about twice as much data is written to your fedora.log as is stored in the object's audit log, so a more realistic estimate of how much disk space is consumed by routine checksum verification is three times the figures calculated above.

Checksum Checker provides two optional drush options that will calculate the number of objects to check per cron run. They are:  
* `--days-to-complete`: The number of days it should take to verify the checksums of all objects.
* `--cmd-run-frequency`: The number of hours between checksum runs (required if --days-to-complete is set). Note that this value (1 for each hour, 2 for every 2 hours, etc.) should correspond to the frequency configured in your server's crontab for running the drush command.

Using these two values, and the number of objects in your repository (which you do not need to provide), Checksum Checker will calculate the number of objects to check each cron run.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Checksum+Checker).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jonathan Green](https://github.com/jonathangreen)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
