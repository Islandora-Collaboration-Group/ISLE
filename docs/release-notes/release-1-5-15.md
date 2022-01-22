# Release Notes - ISLE v.1.5.15, 2021-12

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation
* David Keiser-Clark (Williams College), documentation, code updates
* Noah Smith (Born-Digital), code updates, documentation, testing
* Yamil Suarez (Berklee College of Music), documentation and testing)

### ISLE (main project)

* This is an ISLE security and feature update release.
  * Images have been updated along with the ISLE config

### ISLE Docker Images

#### isle-apache

* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.1.0-19`
* `Composer` upgraded to [commit / hash](9c234603a06f27041dca6b639a16ebc1f27ea22b) (v 1.10.25)
* `FITS` **held** at version `1.5.0` despite a Jan 3, 2022 release. Breaks Behat tests and displays ingest and derivatives errors due to missing Java classpath and jars.
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
* `ImageMagick` upgraded to version `7.1.0-19`
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
* Apache `Tomcat` upgraded to `8.5.75`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
