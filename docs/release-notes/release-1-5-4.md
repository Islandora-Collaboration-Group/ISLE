# Release Notes - ISLE v.1.5.4, 2021-02

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation

### ISLE (main project)

* This is an ISLE security and feature update release.  
  * Images have been updated along with the ISLE config

### ISLE Docker Images

#### isle-apache

* `adoptopenjdk:8-jdk-hotspot` is now base image
  * security and package updates
  * base image upgraded to `Ubuntu 20.04 LTS`
* `apt-get` dist-upgrades for dependencies security and package updates
* `S6 overlay` upgraded to [2.2.0.1](https://github.com/just-containers/s6-overlay/releases/tag/v2.2.0.1)
* `ImageMagick` upgraded to version `7.0.10-61`
* `Composer` upgraded to [commit / hash](fa8ea54c9ba4dc3b13111fb4707f9c3b2d4681f6) January 27th, 2021 (v 1.10.20)
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/0bda7188b7b545232a341f1d978b1e4feda46fc2) January 10th, 2020 (v. 2.4.0)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.5.3 to 1.5.4
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.5.3 to 1.5.4
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.5.3 to 1.5.4
* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.0.10-61`
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/0bda7188b7b545232a341f1d978b1e4feda46fc2) January 10th, 2020 (v. 2.4.0)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-mysql

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-solr

* ISLE Tomcat base image upgrade from 1.5.3 to 1.5.4
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-tomcat

* `adoptopenjdk:8-jdk-hotspot` is now base image
  * security and package updates
  * base image upgraded to `Ubuntu 20.04 LTS`
* `apt-get` dist-upgrades for dependencies security and package updates
* `S6 overlay` upgraded to [2.2.0.1](https://github.com/just-containers/s6-overlay/releases/tag/v2.2.0.1)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `S6 overlay` upgraded to [2.2.0.1](https://github.com/just-containers/s6-overlay/releases/tag/v2.2.0.1)
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
