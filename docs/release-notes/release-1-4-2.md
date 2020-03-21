# Release Notes - ISLE v.1.4.2, 2020-03-31

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation
* Francis Dunham (Born Digital), testing


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
* `apt-get` dist-upgrades for dependencies (sec updates)
* `ImageMagick` upgraded to version `7.0.10-1`
* `Composer` upgraded to [commit / hash](https://github.com/composer/composer/commit/b912a45da3e2b22f5cb5a23e441b697a295ba011) March 13th, 2020 (v 1.10.1)
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/563ecfb55ca77c0fc5ea19e4885e00f55ec82ca9) Feb 13th, 2020 (v. 2.3.1)

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.4.1 to 1.4.2
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.4.1 to 1.4.2
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.4.1 to 1.4.2
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `ImageMagick` upgraded to version `7.0.10-1`
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/563ecfb55ca77c0fc5ea19e4885e00f55ec82ca9) Feb 13th, 2020 (v. 2.3.1)

#### isle-mysql

* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-solr

* ISLE Tomcat base image upgrade from 1.4.1 to 1.4.2
  * `adoptopenjdk/openjdk8` base image (sec updates)
* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image (security updates)
* `apt-get` dist-upgrades for dependencies (sec updates)
* `tomcat` upgraded to `8.5.53`

#### isle-varnish

* `apt-get` dist-upgrades for dependencies (sec updates)

#### ISLE-Ansible

* Ansible deploy script received modifications for new versions of Docker, Docker-compose and additional software.