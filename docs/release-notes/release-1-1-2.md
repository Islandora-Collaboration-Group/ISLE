# Release Notes - ISLE v.1.1.2, 2019-06-28

### Contributions to this release from: 

* David Keiser-Clark (Williams College), documentation
* Francesca Livermore (Wesleyan University), documentation
* Gavin Morris (Born-Digital), code updates and testing
* Mark Sandford (Colgate University), documentation
* Bethany Seeger (Amherst College), release manager
* Noah Smith (Born-Digital), code updates and testing
* Shaun Trujillo (Mount Holyoke College), code updates

### ISLE (main project)

* Documentation for ISLE is now found in the `docs` directory of the ISLE repository

* `docker-compose.yml`
  * Release header changed to `1.1.2` with date of release
  * All ISLE docker image tags changed to `1.1.2`
  
* Switched to using OpenJDK for Java because of Oracle license changes. This has resulted in a few changes to the docker images:
  * `isle-ubuntu-basebox` is removed from ISLE, replaced with [adoptopenjdk:openjdk8 image](https://hub.docker.com/r/adoptopenjdk/openjdk8), whose base image is `ubuntu:18.04` (ubuntu-bionic).  This means the underlying operating system setup is the same.
  * Tools pulled in from the `isle-ubuntu-basebox` and needed by child images have been moved to `isle-tomcat` and `isle-apache` as appropritate.
  * Djatoka is no longer running in `isle-imageservices`.  
  
* Installer script updates/changes:
  * OpenSeadragon vset was updated and made more complete for mobile device usage.
  * New vsets in the installer script to turn off Kakadu by default and to use Imagemagick with OpenJPG instead for JP2 derivative creation.  Please read more on Kakadu licensing on the [Cantaloupe website](https://cantaloupe-project.github.io/manual/4.1/processors.html#KakaduNativeProcessor).

### Docker Images

#### isle-tomcat

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `tomcat` to `8.5.42`

#### isle-fedora

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`

#### isle-imageservices

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Djatoka no longer running in image.

#### isle-solr

* Readme change

#### isle-apache

* Server package management updates via `apt-get`
* Apache Tika Tool is no longer used during TECHMD generation. It was causing the `/tmp` disk space in `isle-apache` to fill up during an ingest.   For more details please see [issue 96](https://github.com/Islandora-Collaboration-Group/ISLE/issues/96).

#### isle-mysql

* No change

