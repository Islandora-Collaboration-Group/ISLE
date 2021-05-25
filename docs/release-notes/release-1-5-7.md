# Release Notes - ISLE v.1.5.7, 2021-05

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
* `ImageMagick` upgraded to version `7.0.11-13`
* `Composer` upgraded to [commit / hash](885ece8a6e1370b204b89b7a542169d25aa21177) May 21, 2021 (v 1.10.22)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
* [OpenJPEG](https://github.com/uclouvain/openjpeg) - reverted from HEAD to Jan 10 Release v2.4.0
  * Endusers can chose to build an image using HEAD instead.

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.5.6 to 1.5.7
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.5.6 to 1.5.7
* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Maven` **held** at version `3.6.3` despite a recent April `3.8.1` release. Breaks POM dependencies and blocks mirrors.
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.5.6 to 1.5.7
* `apt-get` dist-upgrades for dependencies security and package updates
* Cantaloupe upgraded to version `4.1.9`
* `ImageMagick` upgraded to version `7.0.11-13`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
* [OpenJPEG](https://github.com/uclouvain/openjpeg) - reverted from HEAD to Jan 10 Release v2.4.0
  * Endusers can chose to build an image using HEAD instead.
* Changes to Dockerfile, confd files and cantaloupe.properties to allow a toggling of dynamic Cantaloupe Processers for JP2 either OpenJpeg or KakaduDemo if expressely chosen.
* Endusers can make changes in .env file without having to maintain seperate image builds. Default will remain OpenJpeg

#### isle-mysql

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-solr

* ISLE Tomcat base image upgrade from 1.5.6 to 1.5.7
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-tomcat

* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Tomcat` upgraded to `8.5.66`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
