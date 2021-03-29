# Release Notes - ISLE v.1.5.5, 2021-03

### Contributions to this release from:

* Gavin Morris (Born-Digital), code updates, documentation, testing and release manager for this version
* Mark Sandford (Colgate University), documentation
* David Keiser-Clark (Williams College), documentation, code updates
* Mark McFate, (Grinnell College), documentation, testing and code updates
* Tom Shorock, (Univ. of Kansas), testing and code updates
* Brandon Weigel, (Arca), documentation, testing

### ISLE (main project)

* This is an ISLE security and feature update release.  
  * Images have been updated along with the ISLE config
### Release fixes

* ISLE v.1.5.5 [release fixes](https://github.com/Islandora-Collaboration-Group/ISLE/projects/5)

### ISLE Docker Images

#### isle-apache

* `apt-get` dist-upgrades for dependencies security and package updates
* `ImageMagick` upgraded to version `7.0.11-3`
* `Composer` upgraded to [commit / hash](fe1f339fb41eb09a49bbdbda83bb8043d02e24fd) March 10th, 2021 (v 1.10.20)
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/0bda7188b7b545232a341f1d978b1e4feda46fc2) January 10th, 2020 (v. 2.4.0)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

* Release fixes
  * https://github.com/Islandora-Collaboration-Group/ISLE/issues/187
  * https://github.com/Islandora-Collaboration-Group/ISLE/issues/195
  * https://github.com/Islandora-Collaboration-Group/ISLE/issues/411
  
#### isle-blazegraph

* ISLE Tomcat base image upgrade from 1.5.4 to 1.5.5
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-fedora

* ISLE Tomcat base image upgrade from 1.5.4 to 1.5.5
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

* Release fixes
  * https://github.com/Islandora-Collaboration-Group/ISLE/issues/190

#### isle-imageservices

* ISLE Tomcat base image upgrade from 1.5.4 to 1.5.5
* `apt-get` dist-upgrades for dependencies security and package updates
* `Cantaloupe` upgraded to version `4.1.8`
* `ImageMagick` upgraded to version `7.0.11-3`
* `OpenJpeg` upgraded to [commit / hash](https://github.com/uclouvain/openjpeg/commit/0bda7188b7b545232a341f1d978b1e4feda46fc2) January 10th, 2020 (v. 2.4.0)
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

* Release fixes
  * https://github.com/Islandora-Collaboration-Group/ISLE/issues/207

#### isle-mysql

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-solr

* ISLE Tomcat base image upgrade from 1.5.4 to 1.5.5
* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-tomcat

* `apt-get` dist-upgrades for dependencies security and package updates
* Apache `Tomcat` upgraded to `8.5.64`
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-varnish

* `apt-get` dist-upgrades for dependencies security and package updates
* Github Actions [workflow](https://github.com/marketplace/actions/build-and-push-docker-images) updated

#### isle-drupal-build-tools

* pdf.js now default viewer for PDFs and working by default now in install & migation scripts.

* Release fixes
  * https://github.com/Islandora-Collaboration-Group/ISLE/issues/194
