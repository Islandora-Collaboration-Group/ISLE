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
  This allows guidance in how one would configure ISLE for different scenarios.
  * New docker-compose files and environment profiles created for:
    * [Demo](../install/install-demo.md) - use this to spin up ISLE quickly just to check it out
    * Local ([new](../install/install-local-new.md), [migrate](../install/install-local-migrate.md)) - use this for a local development environment
    * Staging ([new](../install/install-staging-new.md), [migrate](../install/install-staging-migrate.md)) -- use this to setup a staging environement for your production website
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
    * Single container: docker logs -f <container-name>
    * All containers: docker logs --tail=0 --follow
    * To learn more: https://docs.docker.com/engine/reference/commandline/logs/
  * Docker-compose method: (when using the Docker json driver and TICK is not on)
    * Single container: docker-compose logs -f  <container-name>
    * All containers: docker-compose logs --tail=0 --follow
    * To learn more: https://docs.docker.com/compose/reference/logs/
  * Use the TICK Log viewer if TICK is setup and using the Docker syslog driver. Please read the 
    ISLE TICK documentation to see how to set this up [TICK Stack Page](../optional-components/tickstack.md)

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

