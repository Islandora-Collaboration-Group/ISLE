# Release Notes - ISLE v.1.5.1, 2020-08

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation


### ISLE (main project)

* This is an ISLE security and feature update release.  
* Images have been updated along with the ISLE config
* Apache fix for file naming
* Cantaloup bug fix

### Upgrade

If you are coming from ISLE releases `1.3.0` then you **need to follow the instructions** given in the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/) documentation.

If you are coming from an ISLE release older then `1.3.0`, please read the release notes for `1.3.0` to understand the paradigm shift that happened between previous releases. You will also need to follow the instructions given in the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/) documentation.

### ISLE Docker Images

#### isle-apache

* `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `S6 overlay` upgraded to [2.0.0.1](https://github.com/just-containers/s6-overlay/releases/tag/v2.0.0.1)
* `ImageMagick` upgraded to version `7.0.10-24`
* `Composer` upgraded to [commit / hash](https://github.com/composer/composer/commit/ed106feacef086c1fe511f535ad3988d383493d9) July 18th, 2020 (v 1.10.9)
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/cbee7891a0ee664dd83ca09553d2e30da716a883) June 30th, 2020 (v. 2.3.1)

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.5.0 to 1.5.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.5.0 to 1.5.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `Apache Ant` upgrade from 1.10.7 to 1.10.8

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.5.0 to 1.5.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `Cantaloupe` upgraded to version `4.1.6`
  * Kakadu symlinks removed
* `ImageMagick` upgraded to version `7.0.10-24`
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/cbee7891a0ee664dd83ca09553d2e30da716a883) June 30th, 2020 (v. 2.3.1)  

#### isle-mysql

* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-solr

* ISLE Tomcat base image upgrade from 1.5.0 to 1.5.1
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image (security updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `S6 overlay` upgraded to [2.0.0.1](https://github.com/just-containers/s6-overlay/releases/tag/v2.0.0.1)
* `tomcat` upgraded to `8.5.57`

#### isle-varnish

* `S6 overlay` upgraded to [2.0.0.1](https://github.com/just-containers/s6-overlay/releases/tag/v2.0.0.1)
* `apt-get` dist-upgrades for dependencies (sec updates)
