# Release Notes - ISLE v.1.5.15, 2022-01

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

* Docker base image changed to [eclipse-temurin:8-jdk](https://hub.docker.com/layers/eclipse-temurin/library/eclipse-temurin/8-jdk/images/sha256-83fb6396891390b6305af57aa4b0bc41d45d24d4e2645615cc56763519201eee?context=explore)
  * Newer patched Java - `jdk8u312-b07` - Same foundation, same project new branding.
* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.1.0-19`
* `Composer` upgraded to `1.10.25` - [commit / hash](9c234603a06f27041dca6b639a16ebc1f27ea22b)
* `FITS` upgraded to version `1.5.1` with missing `xml-apis.jar` patch.
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

* Docker base image changed to [eclipse-temurin:8-jdk](https://hub.docker.com/layers/eclipse-temurin/library/eclipse-temurin/8-jdk/images/sha256-83fb6396891390b6305af57aa4b0bc41d45d24d4e2645615cc56763519201eee?context=explore)
  * Newer patched Java - `jdk8u312-b07` - Same foundation, same project new branding.
* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Tomcat` upgraded to `8.5.75`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated
