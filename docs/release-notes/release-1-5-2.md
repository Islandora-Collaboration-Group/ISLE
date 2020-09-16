# Release Notes - ISLE v.1.5.2, 2020-09

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation


### ISLE (main project)

* This is an ISLE security and feature update release.  
* Images have been updated along with the ISLE config

### Upgrade

If you are coming from ISLE releases `1.3.0` then you **need to follow the instructions** given in the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/) documentation.

If you are coming from an ISLE release older then `1.3.0`, please read the release notes for `1.3.0` to understand the paradigm shift that happened between previous releases. You will also need to follow the instructions given in the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/) documentation.

### ISLE Docker Images

#### isle-apache

* `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `S6 overlay` upgraded to [2.1.0.0](https://github.com/just-containers/s6-overlay/releases/tag/v2.1.0.0)
* `ImageMagick` upgraded to version `7.0.10-29`
* `Composer` upgraded to [commit / hash](https://github.com/composer/getcomposer.org/commits/masterede2f57b5074fa0e21429430dcd521992bfd830f) September 11th, 2020 (v 1.10.13)
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/0f16986738725799237548ce6a2ea12516850e72) September 16th, 2020 (v. 2.3.1)

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.5.1 to 1.5.2
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.5.1 to 1.5.2
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.5.1 to 1.5.2
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `ImageMagick` upgraded to version `7.0.10-29`
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/0f16986738725799237548ce6a2ea12516850e72) September 16th, 2020 (v. 2.3.1)  

#### isle-mysql

* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-solr

* ISLE Tomcat base image upgrade from 1.5.1 to 1.5.2
* `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image (security updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `S6 overlay` upgraded to [2.1.0.0](https://github.com/just-containers/s6-overlay/releases/tag/v2.1.0.0)
* `tomcat` upgraded to `8.5.58`

#### isle-varnish

* `S6 overlay` upgraded to [2.1.0.0](https://github.com/just-containers/s6-overlay/releases/tag/v2.1.0.0)
* `apt-get` dist-upgrades for dependencies (sec updates)
