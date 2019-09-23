# Release Notes - ISLE v.1.2.0, 2019-08-13

### Contributions to this release from:

* David Keiser-Clark (Williams College), documentation
* Gavin Morris (Born-Digital), code updates and testing
* Mark McFate (Grinnell College), testing and documentation
* Bethany Seeger (Amherst College), testing and release manager
* Noah Smith (Born Digital), code updates and testing
* Shaun Trujillo (Mount Holyoke College), code updates and testing

### ISLE (main project)

* New structure for launching [ISLE environments](../install/install-environments.md). 
  This allows guidance in how one would configure ISLE for different scenarios.  If you are upgrading from ISLE versions 1.1.1 or 1.1.2, see the [upgrade instructions](../update/update.md) to configure your existing environments for the updated workflow.
  * New docker-compose files and environment profiles created for:
    * [Demo](../install/install-demo.md) - use this to spin up ISLE quickly just to check it out
    * Local ([new](../install/install-local-new.md), [migrate](../install/install-local-migrate.md)) - use this for a local development environment
    * Staging ([new](../install/install-staging-new.md), [migrate](../install/install-staging-migrate.md)) -- use this to setup a staging environment for your production website
    * Production ([new](../install/install-production-new.md), [migrate](../install/install-production-migrate.md)) - use this for your production environment
    * Test - an environment for ISLE contributors; it includes automated testing code
  * New settings.php and php.ini files for each of the above configurations.
  * New traefik.toml files for each configuration.

* Adds TICK Dashboard as an optional feature in the Staging/Production/Test environments.
  TICK is a suite of open-source applications
  that are used together for the storing, visualization and monitoring of data (time series metrics)
  gathered from running systems. The TICK stack was created and is developed by Influxdata which is
  "...dedicated to open source and truly cares about helping developers get to results faster with
  less complexity and less code." To enable TICK on your system, please read through [Tick Stack Page](../optional-components/tickstack.md)

* Logging to physical files is turned off in each Docker container.  To view the logs now, you can use
  one of these methods:
  * Docker method: (when using the Docker json driver and TICK is not on)
    * Single container: docker logs -f <containername>
    * All containers: docker logs --tail=0 --follow
    * To learn more: https://docs.docker.com/engine/reference/commandline/logs/
  * Docker-compose method: (when using the Docker json driver and TICK is not on)
    * Single container: docker-compose logs -f  <containername>
    * All containers: docker-compose logs --tail=0 --follow
    * To learn more: https://docs.docker.com/compose/reference/logs/
  * Use the TICK Log viewer if TICK is setup and using the Docker syslog driver. Please read the 
    ISLE TICK documentation to see how to set this up [TICK Stack Page](../optional-components/tickstack.md)

### Upgrade

If you are coming from ISLE releases `1.1.1` & `1.1.2` - there will be some additional steps to take to migrate to this new release. 

* We recommend that you also review the following:
  * [ISLE Installation: Environments](../install/install-environments.md)
  * [Update ISLE](../update/update.md) documentation as well. 

The following files will need you to review, edit and/or merge in previous settings:

* `.env` - All previous variables with the exception of the four below have been moved to the demo, local, staging and production .env files. By default this file will be set to the demo environment. 
  * Default settings for ISLE. Change for other environments as needed.

```bash
COMPOSE_PROJECT_NAME=isle_demo
BASE_DOMAIN=isle.localdomain
CONTAINER_SHORT_ID=ld
COMPOSE_FILE=docker-compose.demo.yml
```
  * You'll need to change the settings above to match whatever ISLE environment you would like. You'll need to copy over the settings from your previous local, Staging and Production. Please note on Staging and Production, you'll be making a local commit but NOT pushing back to your main ISLE git repository. 

* All other users, names and passwords from your previous ISLE 1.1.1 & 1.1.2 `.env` will now need to be copied into the new environment .env files
  * `demo.env` - Prepopulated. Nothing to change.
  * `local.env` - Copy over all users, names and passwords from your previous Local ISLE 1.1.1 & 1.1.2 `.env` to the appropriate variable. Remove the comments and replace with the appropriate Local values.
  * `staging.env` - Copy over all users, names and passwords from your previous Staging ISLE 1.1.1 & 1.1.2 `.env` to the appropriate variable. Remove the comments and replace with the appropriate Staging values.
  * `production.env` - Copy over all users, names and passwords from your previous Production ISLE 1.1.1 & 1.1.2 `.env` to the appropriate variable. Remove the comments and replace with the appropriate Production values.

* `docker-compose.yml` - This file no longer exists in ISLE 1.2.0 and has been replaced by new docker-compose files and environment profiles created for:
  * **Demo** - `docker-compose.demo.yml` - use this to spin up ISLE quickly just to check it out.
  * **Local** - `docker-compose.local.yml` - use this for a local development environment. 
    * Copy over your previous ISLE Local docker-compose.yml settings as needed and merge into this file.
  * **Staging** - `docker-compose.staging.yml` -- use this for your staging environment. 
    * Copy over your previous ISLE Staging docker-compose.yml settings as needed and merge into this file.
  * **Production** - `docker-compose.production.yml`- use this for your production environment.
    * Copy over your previous ISLE Production docker-compose.yml settings as needed and merge into this file.

* There are also new `./config/apache/settings_php/settings.*.php` files used in ISLE 1.2.0 which are also seperated by enviornment e.g. `settings.local.php`, etc.
  * As instructed above, you'll need to ensure that for every `settings.*.php` file, the values from your previous `.env` file are copied in here as well.
  * Within the new `settings.*.php` files, search for two sections called `ISLE Configuration`. Follow the inline instructions as directed to determine which values should be copied in.

### ISLE Docker Images

#### isle-tomcat

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `tomcat` to `8.5.43`
* Upgraded `S6 overlay` to `1.22.1`
* Logging configuration changed

#### isle-fedora

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Upgraded `Maven` to `3.6.1`
* Upgraded `Ant` to `1.10.6`
* Logging configuration changed

#### isle-imageservices

* Server package management updates via `apt-get`
* Updated `GEN_DEP_PACKS` dependencies via `apt-get`
* Logging configuration changed

#### isle-solr

* Readme change
* Logging configuration changed

#### isle-apache

* Server package management updates via `apt-get`
* Upgraded `PHP` to version `7.1` from `5.61`
* Upgraded `FITS` to version `1.4.1`
* Upgraded `S6 overlay` to `1.22.1`
* Upgraded `ImageMagick` to version `7.0.8-56`
* Logging configuration changed

#### isle-mysql

* Logging configuration changed

