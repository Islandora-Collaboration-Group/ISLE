# Release Notes - ISLE v.1.3.0, 2019-09-27

### Contributions to this release from:

* David Keiser-Clark (Williams College), documentation
* Gavin Morris (Born-Digital), code updates, documentation and testing
* Mark McFate (Grinnell College), documentation, code updates and testing
* Mark Sandford (Colgate University), documentation
* Bethany Seeger (Amherst College), testing and release manager
* Noah Smith (Born Digital), code updates and testing
* Shaun Trujillo (Mount Holyoke College), documentation review

### ISLE (main project)


* This is an ISLE security release.  Images have been updated and only minor adjustments in version happened.

### Upgrade

If you are coming from ISLE releases `1.3.0` - there will be minamal steps to update. 
If you are coming from an ISLE release older then `1.3.0`, please read the release notes for `1.3.0` to understand
the paradigm shift that happened between previous releases. 

### ISLE Docker Images

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image (security updates)
* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `tomcat` to `8.5.47`

#### isle-fedora

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`

#### isle-imageservices

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* `ImageMagick` updates to version `7.0.9-5`
* `OpenJpeg` upgraded to commit / hash November 17th, 2019 (v. 2.3.1)

#### isle-solr

* Server package management updates via `apt-get`

#### isle-apache

* `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get`
* Upgraded `ImageMagick` to version `7.0.9-5`
* `Composer` upgraded from commit / hash November 1st, 2019  (v 1.9.1)
* `Drush` upgraded to `8.3.1`

#### isle-mysql

* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-blazegraph

* `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get
* ISLE Tomcat base image upgrade from `1.3.0` to `1.4.0`

#### isle-varnish

* Server package management updates via `apt-get`
