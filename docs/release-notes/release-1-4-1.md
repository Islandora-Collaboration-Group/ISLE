# Release Notes - ISLE v.1.4.1, 2020-01-26

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation
* Noah Smith (Born Digital), code updates and testing
* Big thanks for all of Brandon Weigel's hard work in bug hunting and the Apache PR fix. (Arca (https://arcabc.ca) Manager and technical lead, and coordinator at BC ELN (http://bceln.ca).)
* More names to be added once release has been fully tested.

### ISLE (main project)

* This is an ISLE security release.  
* Images have been updated and only minor adjustments in versions happened.

### Upgrade

If you are coming from ISLE releases `1.3.0` then you **need to follow the instructions** given in
the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/)
documentation.

If you are coming from an ISLE release older then `1.3.0`, please read the release notes for `1.3.0` to understand
the paradigm shift that happened between previous releases. You will also need to follow the instructions given in
the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/)
documentation.

### ISLE Docker Images

#### isle-apache

* `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get`
* Accepted PR for `apache2.conf` work from @bondjimbond for .htaccess issue on Redhat specific ISLE Host servers
* Upgraded `ImageMagick` to version `7.0.9-8`
* `Composer` upgraded from commit / hash Jan 14, 2020 (v 1.9.1)
* `Drush` upgraded to `8.3.2`
* `OpenJpeg` upgraded to commit / hash Jan 13th, 2020 (v. 2.3.1)

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.4.0 to 1.4.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.4.0 to 1.4.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* `Maven` upgrade from 3.6.2. to 3.6.3

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.4.0 to 1.4.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* `ImageMagick` updates to version `7.0.9-8`
* `OpenJpeg` upgraded to commit / hashJan 13th, 2020 (v. 2.3.1)

#### isle-mysql

* `apt-get` dist-upgrades for dependencies (sec updates)
  * Additions to README.md for environmental variables & link to MySQL's license information
  * Added LICENSE file

#### isle-solr

* ISLE Tomcat base image upgrade from 1.4.0 to 1.4.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get`

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image (security updates)
* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `tomcat` to `8.5.50`

#### isle-varnish

* Server package management updates via `apt-get`