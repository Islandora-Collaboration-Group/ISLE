# Varnish

## Use Cases & User Stories

* As a repository administrator, I expect that ISLE allows use of server-side caching to optimize the end-user experience without any input from me.
  * If I have a complex pages (including Solr search results), then I expect ISLE provides an optional HTTP accelerator to deliver cached versions 
  * _Negative user story:_ I do not expect that this optional HTTP accelerator will necessarily speed up asset delivery. Various Islandora components have their own caches for this purpose. ISLE documentation should, however, direct repository administrators to good resources if they have this use case.

* As a repository browser, I expect to be able to performantly browse an Islandora site, taking advantage of server-side caching to ensure speedy delivery of frequently-viewed pages.

* As a repository administrator, I expect to find clear documentation about how and why to configure varnish to meet my repository’s unique needs.
  * As a repository administrator, I expect to be able to easily clear the Varnish cache to view changes to the site in real time.

---

## New Functionality

* The new `isle-varnish` image was built using [Varnish](https://varnish-cache.org/intro/) software to improve end-user performance when and if Drupal/Apache performance is inadequate. The intended use is solely for high-traffic production environments.

* Several paragraphs describing the functional differences between Apache and Varnish

* Setting expectations for the upgrade.

* Additional notes commenting on tradeoffs

* Additional notes concerning administrative overhead and/or necessary process changes

* Additional systems overhead, including:
  * __ GB RAM for Varnish to run in memory
  * __ GB disk space for Varnish cache persistence

* No new software required on host machine

---

## System Requirements

* [ISLE](https://github.com/Islandora-Collaboration-Group/ISLE) release version `1.1.1`
  * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

### ISLE Images

* Following the installation steps below, an enduser will configure  / edit their ISLE running system(s) to ultimately use the following images and tags from Docker-Hub:

(_These tags are for usage during Phase II Sprints only and will change during the release process._)

(_Phase II Sprints only_)

| Service | Repository | Tag |
| ---     | ---        | --- | 
| Apache | [islandoracollabgroup/isle-apache](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-apache/tags) | `dashboards-dev`|
| Fedora | [islandoracollabgroup/isle-fedora](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-fedora/tags) | `blazegraph.dashboards-dev`|
| Image-services | [islandoracollabgroup/isle-imageservices](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-imageservices) | `dashboards-dev` |
| MySQL | [islandoracollabgroup/isle-mysql](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-mysql) | `dashboards-dev` |
| Portainer | [portainer/portainer](https://hub.docker.com/r/portainer/portainer) | `latest` |
| Solr  | [islandoracollabgroup/isle-solr](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-solr/tags) | `dashboards-dev` |
| Traefik | [traefik/traefik](https://hub.docker.com/_/traefik) | `1.7.9` |
| Varnish | [islandoracollabgroup/isle-varnish](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-varnish) | `1.1.1`|

* Additional systems overhead, including:

  * Add an additional 1-2 GB RAM in total ISLE Host memory for Varnish to keep the cache in memory.
    * You can adjust the amount that Varnish puts into memory in the supplied `.env` file
      * On the line: `VARNISH_MALLOC=256m` you can change the amount of memory to a higher value other than the default `256` Megabytes. We recommend that you do not attempt to exceed 1 GB for now.

---

## Adoption Process Overview

* The installation instructions below will walk you through how to setup and run the optional Varnish container to cache assets for highly trafficked Islandora sites.

* You'll start by backing up all important data as needed.

- You'll stop any running containers

* You'll download new ISLE images temporarily tagged as `dashboards-dev` instead of the standard ISLE `1.1.1`. 
  * **Please note:** _This is a temporary process until all ISLE Phase II UAT testing is completed and the images can be released._
  * You'll download a new ISLE image called `isle-varnish:1.1.1`

- You'll make additional edits and modifications to the following ISLE configuration files:
  * `docker-compose.yml`
  * `.env`

* You'll restart your containers with the new services having been added and configured.

- You can test by inspecting content in your web-browser and/or using Varnish command line tools to ensure that the new caching is occuring.

---

## Installation

### Assumptions

* Prior to installation, enduser will have a running ISLE system using the current release of `1.1.1.` images.

* This installation process will give the functionality as stated in the `Systems Requirements` image table above for `Varnish` testing and even [TICK](tickstack.md) stack usage.

---

### Installation Instructions

* Shut down your running containers
  * `docker-compose down`

* Make the below mentioned edits to:
  * `docker-compose.yml` file
  * `.env` file

#### Varnish Edits - docker-compose.yml file

* Add a new service to your docker-compose file:

```bash 
  varnish:
    # build:
    #   context: ../images/isle-varnish
    image: islandoracollabgroup/isle-varnish:1.1.1
    container_name: isle-varnish-${CONTAINER_SHORT_ID}
    env_file:
      - .env
    networks:
      isle-internal:
    depends_on:
      - mysql
      - fedora
      - solr
      - apache
      - traefik
    labels:
      - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
      - traefik.port=6081
      - traefik.enable=true
      - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /adore-djatoka, /cantaloupe"
  ```

* For Phase II UAT testing of [TICK](tickstack.md), [Blazegraph](blazegraph.md) and Varnish please change the following image tags of these services from `1.1.1` to `dashboards-dev`
  * Apache
      * `image: islandoracollabgroup/isle-apache:1.1.1` should now become `image: islandoracollabgroup/isle-apache:dashboards-dev`
  * Fedora
      * `image: islandoracollabgroup/isle-fedora:1.1.1` should now become `image: islandoracollabgroup/isle-fedora:dashboards-dev`
  * Image-services
      * `image: islandoracollabgroup/isle-imageservices:1.1.1` should now become `image: islandoracollabgroup/isle-imageservices:dashboards-dev`
  * MySQL 
    * `image: islandoracollabgroup/isle-mysql:1.1.1` should now become `image: islandoracollabgroup/isle-mysql:dashboards-dev`
  * Solr
    * `image: islandoracollabgroup/isle-solr:1.1.1` should now become `image: islandoracollabgroup/isle-solr:dashboards-dev`

* Edit your apache service to remove the `traefik` labels so that Varnish can "take over" handling and routing web traffic.

```bash
    #labels:
    #  - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
    #  - traefik.enable=true
    #  - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /adore-djatoka, /cantaloupe"
```

* No change to traefik service: the varnish service now handles web traffic to your main domain.

---

#### Varnish Edits - .env file

* Due to the usage of `dashboards-dev` images and an upcoming ISLE release that will by default change ISLE's log handling, the following block of ENV variables need to be added to your main .env for the `dashboards-dev` images to work in testing.

* Add this new block of ENV variables to your main .env file beneath all other defined services but above the `###################### Please stop editing! #######################` section is ideal.

```bash

###################### LOGS ######################
# Endusers can change log levels here for debugging
# Changing log levels will require a container restart.

## Apache Container Logs and Levels
#
### Apache Error log - lowercase only please
# This log is a combination of the Apache web server error and access log 
# for the domain. Please note it will register only web traffic from the
# the traefik container.
#
# Available output values range from most verbose to least (left to right): 
# trace8, ..., trace1, debug, info, notice, warn, error, crit, alert, emerg
#
### Recommended level is warn
APACHE_ERROR_LOGLEVEL=warn

## FITS Tool Set Log and Levels
# The File Information Tool Set (FITS) identifies, validates and extracts technical 
# metadata for a wide range of file formats.
# Use the following logs below to debug ingests of PDF, Video, audio and more.
# 
# Available output values range from most verbose to least (left to right): 
# ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF (Turns off logging)
#
### Recommended levels are ERROR or FATAL
#
# Use these logs for general purpose debugging
FITS_ROOTLOGGER_LOG_LEVEL=ERROR
FITS_CONSOLE_LOG_LEVEL=ERROR

FITS_UK_GOV_NATIONALARCHIVES_DROID_LOG_LEVEL=FATAL
FITS_EDU_HARVARD_HUL_OIS_JHOVE_LOG_LEVEL=FATAL
FITS_ORG_APACHE_TIKA_LOG_LEVEL=ERROR
FITS_NET_SF_LOG_LEVEL=ERROR
FITS_ORG_APACHE_PDFBOX_LOG_LEVEL=ERROR

## Fedora Container Loggers and Levels
#
# These logs contain information and output concerning the Fedora Commons Repository
#
# Available output values range from most verbose to least (left to right): 
# ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF (Turns off logging)
#
### Recommended levels are WARN
#
# Use these two logs below for general purpose debugging of Fedora
# Main FEDORA Logger
FEDORA_ORG_FCREPO_LOG=WARN
# All other Fedora loggers default level
FEDORA_ROOT_LOG=WARN

# Fedora Apache CXF™ services framework logger
FEDORA_ORG_APACHE_CXF_LOG=WARN

# Fedora Security Loggers previously known as the fesl.log
# This log is typically used for auditing and logging access to Fedora
FEDORA_ORG_FCREPO_SERVER_SECURITY_JAAS_LOG=WARN
FEDORA_ORG_FCREPO_SERVER_SECURITY_XACML_LOG=WARN

# Fedora Gsearch logs
#
# These logs contain information and output concerning the interaction of Fedora, Gsearch and 
# Solr Search. Given that most output is the result of successful Gsearch transforms & Solr search queries
# It is highly recommended that these logs be set to WARN due to the large amount of output.
#
# Available output values range from most verbose to least (left to right): 
# ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF (Turns off logging)
#
### Recommended levels are WARN
# 
# Use these two logs below for general purpose debugging of Fedoragsearch
GSEARCH_DK_DEFXWS_FEDORAGSEARCH_LOG=WARN
GSEARCH_ROOT_LOG=WARN

# All other Gsearch loggers default level. 
# Change levels below only for a larger scale of debugging.
### Recommended levels are WARN
GSEARCH_DK_DEFXWS_FGSZEBRA_LOG=WARN
GSEARCH_DK_DEFXWS_FGSLUCENE_LOG=WARN
GSEARCH_DK_DEFXWS_FGSSOLR_LOG=WARN


## Image Services Container Logs and Levels
# These logs contain information and output concerning the two image servers
# Cantaloupe and Adore-Djatoka.
# 
# Available output values range from most verbose to least (left to right): 
# ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF (Turns off logging)
#
### Recommended levels are ERROR
#
# Adore-Djatoka
ADORE_DJATOKA_ROOT_LOGGER=ERROR

# Cantaloupe (change this one first to debug)
# Cantaloupe loggers appear to prefer loglevels to be lowercase.
# otherwise values fail.
#
# Available output values range from most verbose to least (left to right): 
# `trace`, `debug`, `info`, `warn`, `error`, `all`, or `off`
#
### Recommended level is error
CANTALOUPE_LOG_APPLICATION_LEVEL=error

##### SOLR Log Levels #####
# These logs contain information and output concerning the Solr Search
# Given that most output is the result of successful Solr search queries
# It is highly recommended that these logs be set to OFF or WARN due to the
# large amount of output.
#
# Available output values range from most verbose to least (left to right): 
# ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF (Turns off logging)
#
### Recommended levels are WARN or OFF
#
# Main Solr log (change this one first to debug)
SOLR_ROOT_LOGGER=WARN
# These logs most likely can be kept at their default levels of WARN and for the third OFF.
SOLR_ORG_APACHE_ZOOKEEPER_LOG=WARN
SOLR_ORG_APACHE_HADOOP_LOG=WARN
SOLR_ORG_APACHE_SOLR_UPDATE_LOGGINGINFORSTREAM=OFF

## End Logs
```

* Add another new block of ENV variables to your main .env file. Above the logging section and beneath all other defined services is fine. 

* Please note we recommned that ISLE endusers only make changes to the following:

  * VARNISH_MALLOC=256m
  * VARNISH_MAX_CONNECTIONS=300
  * VARNISH_SECRET=isle_varnish_secret

```bash
## Varnish
## Varnish Admin port is 6082
## varnish is substituted for default value of localhost to be open to apache
## otherwise Drupal varnish module cannot connect via CLI
VARNISH_ADMIN=varnish
VARNISH_ADMIN_PORT=6082
## Varnish backend
## the apache service is the "backend" for varnish
VARNISH_BACKEND=apache
VARNISH_BACKEND_PORT=80
## Varnish Cache Memory Allocation
VARNISH_MALLOC=256m
## Maximum amount of connections
VARNISH_MAX_CONNECTIONS=300
## Varnish secret aka Control key
VARNISH_SECRET=isle_varnish_secret
## Varnish port
VARNISH_VARNISH_PORT=6081
```

* Pull down the new isle images
  * `docker-compose pull`

* Spin up new containers
  * `docker-compose up -d`

* Verify it's working by:
  * Navigating to https://yourdomain and you can still see the Drupal site. This may take a few minutes depending on the size of the site.
  * Inspecting the headers of different Drupal pages for the words hit. 
    * (TO DO) Expand on `hit` or `miss` examples
  * Checking the Varnish cli for registered content / page requests
    * (TO DO) Expand on using Varnish console with 1 -2 examples

---

## Using Varnish & the TICK stack

* Please first follow the instructions for installing and using the [TICK stack](tickstack.md)

* If you're pushing log events to [TICK](tickstack.md), add this snippet of code below (_logging instructions_) to the bottom of **every** ISLE service within your `docker-compose.yml` file.

```bash
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

****Example:****

```bash
varnish:
  # build:
  #   context: ../images/isle-varnish
  image: islandoracollabgroup/isle-varnish:1.1.1
  container_name: isle-varnish-${CONTAINER_SHORT_ID}
  env_file:
    - .env
  networks:
    isle-internal:
  depends_on:
    - mysql
    - fedora
    - solr
    - apache
    - traefik
  labels:
    - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
    - traefik.port=6081
    - traefik.enable=true
    - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /adore-djatoka, /cantaloupe"
  logging:
    driver: syslog
    options:
      tag: "{{.Name}}"        
```

* Additionally you'll need to remove or comment out every line or reference to logs from the `volumes` section of each service. Varnish by default doesn't log unless directed to manually.

****Example:****

```bash
    volumes:
      - isle-solr-data:/usr/local/solr
      - ./logs/solr:/usr/local/tomcat/logs
```

becomes

```bash
    volumes:
      - isle-solr-data:/usr/local/solr
```

---

## Uninstallation Instructions

*

*

---

## Maintenance Notes

*

*

---

## Need help?

* Please use the following as resources for institutions or endusers needing support

  * [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody!   
    * The [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT

  * [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support

  * [ISLE Github Issues queue](https://github.com/Islandora-Collaboration-Group/ISLE/issues) - Post your issues, bugs and requests for technical documentation here.

---

## Additional Resources

* [Varnish 4.x Documentation](https://varnish-cache.org/docs/4.1/index.html)

* **Please note:**: 
  * [Varnish Software](https://www.varnish-software.com/) is the commercial wing of the Varnish.
    * Varnish Admin Console is a [paid](https://www.varnish-software.com/solutions/varnish-enterprise/varnish-administration-console-2/) not for free product that is a GUI for Varnish Cache. The language around this feature is vague and sometimes misleadingly used in tutorials as software anyone can use. There are trials but ultimately this is a paid product.
  * [Varnish Cache](https://www.varnish-software.com/community/varnish-cache/) is the open-source project maintained by Varnish Software and intended to be used by anyone.
