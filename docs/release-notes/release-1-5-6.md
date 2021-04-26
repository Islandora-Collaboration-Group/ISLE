# Release Notes - ISLE v.1.5.6, 2021-04

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation
* David Keiser-Clark (Williams College), documentation, code updates
* Yamil Suarez (Berklee College of Music), documentation and testing)

### ISLE (main project)

* This is an ISLE security and feature update release.
  * Images have been updated along with the ISLE config

### ISLE Docker Images

#### isle-apache

* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.0.11-6`
* `Composer` upgraded to [commit / hash](b3a97c79a408cb7f36da93368df27f77e69fc169) April 18th, 2021 (v 1.10.21)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
  
#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.5.5 to 1.5.6
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.5.5 to 1.5.6
* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Maven` **held** at version `3.6.3` despite a recent April `3.8.1` release. Breaks POM dependencies and blocks mirrors.
* Apache `Ant` updated to version `1.10.10` as of April 17th
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.5.5 to 1.5.6
* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.0.11-6`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-mysql

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-solr

* ISLE Tomcat base image upgrade from 1.5.5 to 1.5.6
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-tomcat

* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Tomcat` upgraded to `8.5.65`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
