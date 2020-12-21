# Release Notes - ISLE v.1.5.3, 2020-12

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation

### ISLE (main project)

* This is an ISLE security and feature update release.  
  * Images have been updated along with the ISLE config

* Branches change - `master` is no longer the default branch, `main` is now the default branch
  * `master` will be deprecated then moved out.

### Warnings & Notes

* **AS OF THE ISLE v.1.5.3 release in December 2020 (MANDATORY)**
  * The ISLE maintainers in agreement with the wider Islandora community have changed the default branch of `master` to `main` to work towards a more inclusive and open culture with removing language like "master or slave" as an important step.
  * As a result the `main` branch will now be the default branch.
  * ISLE users will need to continue to checkout `main` and pull from `main` as they once did with the now defunct `master` on all of their projects.
    * `git fetch --all`
    * `git checkout main`
    * `git pull origin main`
  * All future releases will be built and tagged from the `main` branches on all ISLE Docker images Github repositories as well.
  * After December 2020, no more updates will be avaible from the `master` branch which will be deprecated and ultimately deleted in 2021.

### ISLE Docker Images

#### isle-apache

* `adoptopenjdk/openjdk8` base image
  * security and package updates
  * base image upgraded to `Ubuntu 20.04 LTS`
* `apt-get` dist-upgrades for dependencies security and package updates
* `S6 overlay` upgraded to [2.1.0.2](https://github.com/just-containers/s6-overlay/releases/tag/v2.1.0.2)
* `ImageMagick` upgraded to version `7.0.10-51`
* `Composer` upgraded to [commit / hash](https://github.com/composer/getcomposer.org/commit/e3e43bde99447de1c13da5d1027545be81736b27) December 4th, 2020 (v 1.10.19)
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/5d0a8b08dcd3bcdf532c54702b5a88ec61b17918) December 5th, 2020 (v. 2.3.1)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds

#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.5.2 to 1.5.3
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.5.2 to 1.5.3
* `apt-get` dist-upgrades for dependencies security and package updates
* Apache Ant upgraded to version `1.10.9`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.5.2 to 1.5.3
* `apt-get` dist-upgrades for dependencies security and package updates
* `Cantaloupe` upgraded to version `4.1.7`
* `ImageMagick` upgraded to version `7.0.10-51`
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/5d0a8b08dcd3bcdf532c54702b5a88ec61b17918) December 5th, 2020 (v. 2.3.1)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds

#### isle-mysql

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds

#### isle-solr

* ISLE Tomcat base image upgrade from 1.5.2 to 1.5.3
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image
  * security and package updates
  * base image upgraded to `Ubuntu 20.04 LTS`
* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Tomcat` upgraded to `8.5.61`
* `S6 overlay` upgraded to [2.1.0.2](https://github.com/just-containers/s6-overlay/releases/tag/v2.1.0.2)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds

#### isle-varnish

* `S6 overlay` upgraded to [2.1.0.2](https://github.com/just-containers/s6-overlay/releases/tag/v2.1.0.2)
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) added for automated Docker image builds