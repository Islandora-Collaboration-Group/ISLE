# Release Notes - ISLE v.1.5.11, 2021-09

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation
* David Keiser-Clark (Williams College), documentation, code updates
* Noah Smith (Born-Digital), code updates, documentation, testing

### ISLE (main project)

* This is an ISLE security and feature update release.
  * Images have been updated along with the ISLE config

### ISLE Docker Images

#### isle-apache

* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.1.0-8`
* `Composer` upgraded to [commit / hash](b2ffe854401d063de8a3bf6b0811884243a381ba) (v 1.10.22)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-blazegraph

* ISLE Tomcat base image upgrade
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-fedora

* ISLE Tomcat base image upgrade
* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Maven` **held** at version `3.6.3` despite a recent April `3.8.1` release. Breaks POM dependencies and blocks mirrors.
* Apache `Ant` **held** at version `1.10.9` despite a recent April `1.10.10` release. Upstream dependencies fail to download in Github Actions.
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-imageservices

* ISLE Tomcat base image upgrade
* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.1.0-8`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-mysql

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-solr

* ISLE Tomcat base image upgrade
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-tomcat

* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Tomcat` upgraded to `8.5.71`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
