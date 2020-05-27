# Release Notes - ISLE v.1.5.0, 2020-05

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Noah Smith (Born-Digital), code updates, documentation, Matomo profile addition.
* Mark Sandford (Colgate University), documentation
* Hertzel Armengol (Born-Digital), testing & Dockerfile fixes
* Francis Dunham (Born-Digital), testing

### ISLE (main project)

* This is an ISLE security and feature update release.  
* Images have been updated along with the ISLE config
* Matomo docs and config files added
* fix for bfg cleanup script URL
* default .env removed and replaced with sample.env
  * docs to be added on how to use sample.env

### Upgrade

If you are coming from ISLE releases `1.3.0` then you **need to follow the instructions** given in the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/) documentation.

If you are coming from an ISLE release older then `1.3.0`, please read the release notes for `1.3.0` to understand the paradigm shift that happened between previous releases. You will also need to follow the instructions given in the [Cleanup ISLE git repository for Release v.1.4.0](https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/) documentation.

### ISLE Docker Images

#### isle-apache

* `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `ImageMagick` upgraded to version `7.0.10-11`
* `Composer` upgraded to [commit / hash](https://github.com/composer/composer/commit/4d7f8d40f9788de07c7f7b8946f340bf89535453) May 6th, 2020 (v 1.10.1)
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/64689d05dfaaf52105581d93fb1eb173b20829a4) Apr 18th, 2020 (v. 2.3.1)
* Volume removed from Dockerfile to stop overlay2 issues
* Vhost fix for cantaloupe IIIF 4.1.5 upgrade and changes

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.4.2 to 1.5.0
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.4.2 to 1.5.0
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.4.2 to 1.5.0
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `Cantaloupe` upgraded to version `4.1.5`
  * Cantaloupe caching turned off due to viewing issues
  * `delegates.rb` able to use two configs
* `ImageMagick` upgraded to version `7.0.10-11`
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/64689d05dfaaf52105581d93fb1eb173b20829a4) Apr 18th, 2020 (v. 2.3.1)
* Adds new proxy settings for IIIF to allow X-islandora token to work in IAB and Openseadragon viewers for Books and Large Images

#### isle-mysql

* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-solr

* ISLE Tomcat base image upgrade from 1.4.2 to 1.5.0
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image (security updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `tomcat` upgraded to `8.5.55`

#### isle-varnish

* `apt-get` dist-upgrades for dependencies (sec updates)
