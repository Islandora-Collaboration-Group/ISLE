# Release Notes - ISLE v.1.1.1, 2019-05-01

## Updates and testing performed by ISLE Committer Gavin Morris, Born-Digital

### ISLE (main project)

* `docker-compose.yml`
    * Release header changed to `1.1.1` with date of release
    * All isle-image tags changed to `1.1.1`
    * Upgraded `traefik` to `1.7.9`

### Images

#### isle-ubuntu-basebox

* Updated `ubuntu:bionic` build image
* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `S6_OVERLAY` to `1.21.7.0`
* Upgraded `Java (Server JRE)` to `jre-8u202 /1.8.0_202`

#### isle-tomcat

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `tomcat` to `8.5.40`

#### isle-fedora

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Updated `Islandora XACML policies` to [Jan 2019 commit](https://github.com/Islandora/islandora-xacml-policies/commit/73deba6b90f6de40d72c890233ba6ff0159031de)
* Upgraded `Islandora Drupal Filter` to `7.1.12`
* Upgraded `Apache Maven` to `3.6.0`
* Updated `Discovery Garden's basic-solr 4.10.x config` to newest [March 19, 2019 commit](https://github.com/discoverygarden/basic-solr-config/commit/0a7cdf3447b033e1b6614b80155a3441d46f9eec)
* Updated `Discovery Garden's Islandora Transforms` to [March 19, 2019 commit](https://github.com/discoverygarden/islandora_transforms/commit/cdfef63b7e2f6740c06e56bfa32d496c09837ae8)

#### isle-imageservices

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Updated `openjpeg` to [Dec 21,2018 commit](https://github.com/uclouvain/openjpeg/commit/51f097e6d5754ddae93e716276fe8176b44ec548)
* Upgraded Imagemagick to `7.0.8-42`

#### isle-solr
* Server package management updates via `apt-get`

#### isle-apache
* Upgraded Imagemagick to version `ImageMagick-7.0.8-42.x86_64`
* Upgraded Composer to version `1.8.5`
* Upgraded Drush to `8.2.3` with `8.1.18` as fallback
* Upgraded FITS to `1.4` (Nov 2018)

#### isle-mysql
* Updated `mysql:5.7` build image
