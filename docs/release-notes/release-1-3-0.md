# Release Notes - ISLE v.1.3.0, 2019-09

### Contributions to this release from:

* David Keiser-Clark (Williams College), documentation
* Gavin Morris (Born-Digital), code updates and testing
* Mark McFate (Grinnell College), testing and documentation
* Bethany Seeger (Amherst College), testing and release manager
* Noah Smith (Born Digital), code updates and testing
* Shaun Trujillo (Mount Holyoke College), code updates and testing

### ISLE (main project)

* Adds two new optional components for use on Production sytems
  * Varnish - caching web accelearator for high traffic sites
    * Optional Varnish service added to only Production & Local docker-compose files (commented out)
    * Varnish logging added in the .envs
  * Blazegraph - triplestore replacment for Mulgara for high volume ingest
    * Blazegraph / Fedora service changes in the .envs to allow toggling
    * Blazegraph logging added in the .envs
    * Optional Blazegraph service added to all docker-compose(s) (commented out)
* Automated Testing  - updated to include Varnish and Blazegraph in base test suite; and updated instructions to correspond with revised test repository location and keywords/names.
* Library of Congress standards proxy - High Volume Ingest
* Resource matrix document for - Local, staging, production e.g. RAM, CPU, tuning etc.

### Upgrade

If you are coming from ISLE releases `1.2.0` - there will be some additional steps to take to migrate to this new release.

* We recommend that you also review the following:
  * [ISLE Installation: Environments](../install/install-environments.md)
  * [Update ISLE](../update/update.md) documentation as well.

The following files have new Varnish and Blazegraph information and ENVs which may need you to review, edit and/or merge in previous settings:

* All `demo`, `local`, `staging`, `production` and `test` .env files have a new `### Fedora Resource Index` section with two new ENV variables.
  * There are in-line instructions on how to switch between the default mulgara and optional component blazegraph

* All `docker-compose.*.yml` files have a new optional Blazegraph service section, (_commented out_) along with a (_commented out_) Blazegraph volume definition
  * **Test** - `test.env` by default now tests Blazegraph instead of mulgara.

* **Production** - `docker-compose.production.yml` - New Varnish service added but commented out.
  * `production.env` - New Varnish section with ENV variables (_commented out_)

* **Local** - `docker-compose.local.yml` - New Varnish service added but commented out.
  * `local.env` - New Varnish section with ENV variables (_commented out_)

* **Test** - `docker-compose.test.yml` - New Varnish service added.
  * `test.env` - New Varnish section with ENV variables

### ISLE Docker Images

#### isle-tomcat

* `adoptopenjdk/openjdk8` base image (security updates)
* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `tomcat` to `8.5.45`

#### isle-fedora

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `Maven` to `3.6.2`
* Upgraded `Ant` to `1.10.7`
* Islandora transforms changes from August 15th
* Blazegraph work integrated
* Bug fix https://github.com/Islandora-Collaboration-Group/ISLE/issues/317

#### isle-imageservices

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Canteloupe upgraded from `4.0.1` to `4.0.3`
* Cleanup of service paths https://github.com/Islandora-Collaboration-Group/ISLE/issues/226

#### isle-solr

* Server package management updates via `apt-get`

#### isle-apache

* `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get`
* `PHP` version `7.1` updates
* Upgraded `FITS` to version `1.5.0`
* Upgraded `ImageMagick` to version `7.0.8-64`
* `Composer` upgraded from commit / hash August 2, 2019 to Sept 2, 2019 (stil v 1.9.0)

#### isle-mysql

* `apt-get` dist-upgrades for dependencies (sec updates)

#### isle-blazegraph

* `adoptopenjdk/openjdk8` base image (sec updates)
* Server package management updates via `apt-get
* ISLE Tomcat base image upgrade from `1.1.1` to `1.3.0`
* Blazegraph upgraded from `2.1.4` to `2.1.5`

#### isle-varnish

* Server package management updates via `apt-get`
* ISLE base image change from discontinued `isle-ubuntu-basebox` to `ubuntu:xenial` base image (sec updates)
* Varnish upgraded from `4.1.3-12` to `4.1.11`
