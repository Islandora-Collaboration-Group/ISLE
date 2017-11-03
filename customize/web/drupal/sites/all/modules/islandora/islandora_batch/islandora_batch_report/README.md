# Islandora Batch Report

## Introduction

This module adds the ability to send out a daily islandora batch ingest email report.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Batch](https://github.com/islandora/islandora_batch)
* [Rules](https://www.drupal.org/project/rules)

Additionally, enabling Rules Admin UI will allow for custom configuration of the email and the ability to disable it.

# Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Current setup requires the drupal cron to be running. Through admin menu Islandora > Islandora Utilities Modules > Islandora Batch Reporting Settings
a time can be set to limit when the daily report will be sent out to prevent sites that run fequent cron checks from sending up emails too early.  The last
report sent will be tracked and also used to limit the report to be sent out once.

### Usage

The default rule is configured to send an email to drupal site admin email when cron is run. This email contains a batch ingest report with daily totals of the batch objects/sets ingested, a breakdown of objects ingested by content model, and the remaining objects/sets that remain to be ingested. 
This is meant to be called by cron before the end of the day and only delivered once with daily totals.

### Customization

Custom changes can be made with Rule UI enabled through Configuration > Workflow > Rules where the default rule **islandora_batch_report_email_report** can be overridden completely by a site admin or disabled.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
