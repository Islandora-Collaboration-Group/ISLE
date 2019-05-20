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

---

# ISLE Release 1.1 (Codenamed `Pembroke`)

* Base Images to serve them all:
    * In Docker fashion two new base-images were created for the 1.1 ISLE stack. There are major benefits both for the contributors and users of ISLE.
    * Keeping our users safe has never been easier.  For users and contributors alike when a security update or patch arrives we (the ISLE Contributors) need only update the base image.  By virtue of inheritance these updates are automatically inherited and built into each and every component of the ISLE stack.
    * Quick downloads, faster launch: these shared base images decrease the time to pull and launch the ISLE stack.  Since the images have the same layers (like a cake) they are downloaded only once and reused as building blocks for the rest of the ISLE stack.
    * This also significantly decreases the overall size of all images because we are able to finely tune and trim our base images so they’re stable, without being bloated by unneeded components.
        * [ISLE-Ubuntu-Base](https://github.com/Islandora-Collaboration-Group/isle-ubuntu-basebox)
        * [ISLE-Tomcat-Base](https://github.com/Islandora-Collaboration-Group/isle-tomcat)
* Image-Services
    * Image Services with an `S`. ImageServices is a 1.2 GB container that simultaneously runs both Adore-Djatoka and [Cantaloupe IIIF](https://medusa-project.github.io/cantaloupe/).  
    * By default the ISLE-Installer prefers Cantaloupe IIIF over AD, but AD remains a component for backward compatibility to be deprecated in a future release.
* ISLE-Apache
    * Automatically pull the newest build tools on launch, never be with an outdated stack.  By default the Apache container will connect to GitHub and pull the ISLE-build.  This is controlled by an environment variable and can be disabled.
    * In future releases we intend to make it so that you can fork our build tools, or make your own, and set your own repository for the container to fetch on boot.
        * Note that while currently running the build tools is only done once we’re thinking ahead. In future planned iterations of ISLE-Apache the build tools will separate core Drupal code (`core`) from the Islandora modules (`contrib`).  TL;DR: We hope to bring automated Drupal core updates, as well as offer an option to update Islandora modules -- though we acknowledge this might not be for everyone!
* ImageMagick 7.x (ISLE images involved: ImageServices and Apache)
    * ImageMagick, the tool which converts and handles derivative generation (in most part) has been updated to the latest 7.x branch and is compiled with JP2 read and write support from [OpenJPEG](http://www.openjpeg.org/).
    * This update also includes the PHP Imagick extension which is now built from source to support the new version of ImageMagick with JP2 support.
* ISLE-Proxy is now magiek
    * We have replaced NGiNX with [Traefik](https://traefik.io/)
    * Traefik is a modern reverse proxy with several key benefits
        * Institutions and users alike can use ACME for the automatic and free creation and maintenance of SSL certificates from Let's Encrypt (https://letsencrypt.org/)
* Added [Portainer](https://portainer.io), a visual dashboard of your running stack(s)
    * Provides quick access to logs, shells, and startup/shutdown functions for running containers.
* Logging
    * Services are now logged out to Docker while also being stored on the containers.
    * In case we’ve missed anything all uncaught logs are still logged by S6 overlay.
* .env for your Environment
    * Lorem Ipsum
* Muli-Environment (e.g., dev, stage, prod) in a greatly simplified manner
    * Repo for multi-env incoming
