# Release Notes - ISLE v.1.5.28, 2024-01

## Contributions to this release from

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version

## Announcements

* ISLE (legacy) has now switched to a quarterly release cycle.
  * `1.5.28` - January 2024 (Q1 2024)

### ISLE (main project)

* This is an ISLE security and feature update release.
  * Images have been updated along with the ISLE config

### ISLE Docker Images

#### isle-apache

* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.1.1-25`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-blazegraph

* ISLE Tomcat base image upgrade
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-fedora

* ISLE Tomcat base image upgrade
* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Maven` **held** at version `3.6.3`. Breaks POM dependencies and blocks mirrors.
* Apache `Ant` **held** at version `1.10.9`. Upstream dependencies fail to download in Github Actions.
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-imageservices

* ISLE Tomcat base image upgrade
* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.1.1-25`
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
* Apache `Tomcat` upgraded to `8.5.97`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
