# Islandora Entities CSV Import [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_entities.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_entities)

## Introduction

This module is for adding person entities to Islandora using a .csv file. 

## Requirements

This module requires the following modules/libraries:
* [Islandora](https://github.com/islandora/islandora)
* [Islandora Basic Collection](https://github.com/Islandora/islandora_solution_pack_collection)
* [Islandora Entities](https://github.com/Islandora/islandora_solution_pack_entities)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Prepare a comma-delimited CSV file using the column names below. Only columns with names in the list will be processed; 
all others will be ignored. Any comma within a field must be replaced with a double pipe ie - 'Nursing, Department of' 
must be replaced with 'Nursing|| Department of'.

Multiple arguments within one column can be separated with a tilde (~). However, this may yield unexpected results 
(missing XML attributes, improper nesting) if used outside the following fields: FAX, PHONE, EMAIL, POSITION. 


```
STATUS
POSITION
EMAIL
BUILDING
ROOM_NUMBER
IDENTIFIER
TERM_OF_ADDRESS
GIVEN_NAME
FAMILY_NAME
FAX
PHONE
DISPLAY_NAME
DEPARTMENT
BUILDING
CAMPUS
NAME_DATE
STREET
CITY
STATE
COUNTRY
POSTCODE
START_DATE
END_DATE
ROOM_NUMBER
BUILDING
CAMPUS
```

This will be transformed into the following MADS record:

```xml
<mads xmlns="http://www.loc.gov/mads/v2" xmlns:mads="http://www.loc.gov/mads/v2"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
    <authority>
        <name type="personal">
            <namePart type="given">[GIVEN_NAME]</namePart>
            <namePart type="family">[FAMILY_NAME]</namePart>
            <namePart type="termsOfAddress">[TERM_OF_ADDRESS]</namePart>
            <namePart type="date">[NAME_DATE]</namePart>
        </name>
        <titleInfo>
            <title>[DISPLAY_NAME]</title>
        </titleInfo>
    </authority>
    <affiliation>
        <organization>[DEPARTMENT]</organization>
        <position>[POSITION]</position>
        <address>
		<email>[EMAIL]</email>
		<phone>[PHONE]</phone>
		<fax>[FAX]</fax>
		<street>[STREET]</street>
		<city>[CITY]</city>
		<state>[STATE]</state>
		<country>[COUNTRY]</country>
		<postcode>[POSTCODE]</postcode>
		<start_date>[START_DATE]</start_date>
		<end_date>[END_DATE]</end_date>
	</address>
    </affiliation>
    <note type="address">[ROOM_NUMBER] [BUILDING] [CAMPUS]</note>
    <identifier type="u1">[IDENTIFIER]</identifier>
    <note type="status">[STATUS]</note>
</mads>
```

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Entities+Solution+Pack).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Rosie Le Faive](https://github.com/rosiel)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
