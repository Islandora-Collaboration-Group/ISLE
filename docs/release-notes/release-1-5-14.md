# Release Notes - ISLE v.1.5.14, 2021-12

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation
* David Keiser-Clark (Williams College), documentation, code updates
* Noah Smith (Born-Digital), code updates, documentation, testing
* Mark McFate, (Grinnell College), testing and code updates

### ISLE (main project)

* This is an ISLE security and feature update release.
  * Images have been updated along with the ISLE config

### ISLE Docker Images

#### isle-apache

* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.1.0-18`
* `Composer` upgraded to [commit / hash](eef6844b9e99f10a61001855cf833fc6eb382e2b) (v 1.10.24)
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
* `ImageMagick` upgraded to version `7.1.0-18`
* `Cantaloupe` upgraded to version `4.1.11`
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
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
