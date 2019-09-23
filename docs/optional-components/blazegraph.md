# Replacing Mulgara with Blazegraph

## Use Cases & User Stories

* As a repository administrator, I expect to be able to ingest millions of objects into an ISLE-based system without incurring significant performance degradation.

## New Functionality

* Several paragraphs describing the functional differences between Blazegraph and Mulgara
  
  * The new `isle-blazegraph` image was built using [Blazegraph™](https://www.blazegraph.com/) which is a ultra high-performance graph database supporting Apache TinkerPop™ and RDF/SPARQL APIs. It supports up to 50 Billion edges on a single machine. Formerly known as BigData. The intended use is solely for maintaining performant support of large Fedora repositories containing millions of objects.

  * Why use this component with ISLE?
    * Islandora users who have Fedora repositories with over 600K~ objects ingested have reported issues with using the default Mulgara triplestore used for indexing objects ingested into the Fedora repository. These issues include crashes, extreme performance slowdowns,system timeouts and more. Blazegraph is used to replace Mulgara as the triplestore and to deliver a higher level of performance for larger Fedora repositories. Please note the threshold given above for object count is an compilation of the anecdotal ranges gathered by the Islandora community. In some cases the number is as low as 600K~ when endusers report challenges, others haven't encountered issues until the 1 million object count.

  * How can I tell how many Fedora objects I have to determine if I should use this component?
    * Walkthrough on getting triple counts. (TO DO)

* Additional notes commenting on tradeoffs in removing Mulgara

* Additional notes concerning administrative overhead and/or necessary process changes
  
  * A new Fedora image that has slight image and functional changes is required to be used in tandem with Blazegraph. These image changes are primarily edits to the `fedora.fcg` file to use Blazegraph instead of Mulgara. 
  
  * Additionally, a new `confd` configuration within the Fedora image and linked environmental variable has been added to the `.env` to allow users to "toggle" between Blazegraph and Mulgara as desired. In the event a user toggles between either desired triplestore, a reindex of the Fedora repository is required. With Fedora repositories of 600K+ objects or more, **the indexing process will take double-digit hours to days** depending on the complexity of the object relationships, ontology, etc.

* No new software is required to be installed on the ISLE host machine, only new Docker containers, images and configurations are added to the ISLE platform.

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
| Blazegraph | [islandoracollabgroup/isle-blazegraph](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-blazegraph) | `dashboards-dev`|
| Fedora | [islandoracollabgroup/isle-fedora](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-fedora/tags) | `blazegraph.dashboards-dev`|
| Image-services | [islandoracollabgroup/isle-imageservices](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-imageservices) | `dashboards-dev` |
| MySQL | [islandoracollabgroup/isle-mysql](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-mysql) | `dashboards-dev` |
| Portainer | [portainer/portainer](https://hub.docker.com/r/portainer/portainer) | `latest` |
| Solr  | [islandoracollabgroup/isle-solr](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-solr/tags) | `dashboards-dev` |
| Traefik | [traefik/traefik](https://hub.docker.com/_/traefik) | `1.7.9` |


* Additional systems overhead, including:
  
  * Add an additional 2 GB RAM in total ISLE Host memory for Blazegraph to run
  
  * Plan on an additional 25-100+ GB disk space for “big_data” resource index persistence.
    * This index (`/var/bigdata/bigdata.jnl`) will grow quickly overtime and file sizes of over 25GB or more are possible with Fedora repositories that have over 1 million objects.

  * To persist this file and directory in a bind-mount instead of a Docker volume, an edit will need to be made to the `docker-compose.yml` file in the `blazegraph` service, `volume` section.
    * Change `isle-blazegraph-data` to your ISLE Host system specific bind-mount path. **Example:** 
      * `- /mnt/isle-blazegraph-data:/var/bigdata`
  * This disk should be a separate data disk that should be 25-100+ GB in capacity. Plan on resizing this disk when using Blazegraph with large Fedora repositories of over 600K+ of objects.

---

## Adoption Process Overview

* The installation instructions below will walk you through how to setup and run the optional Blazegraph container for use as an replacement triplestore for Fedora repositories with 600K+ of objects.

- You'll start by backing up all important data as needed.

* You'll stop any running containers

- You'll download new ISLE images temporarily tagged as `dashboards-dev` instead of the standard ISLE `1.1.1`. 
  * **Please note:** _This is a temporary process until all ISLE Phase II UAT testing is completed and the images can be released._
  * You'll download a new ISLE image called `isle-blazegraph:dashboards-dev`

* You'll make additional edits and modifications to the following ISLE configuration files:
  * `docker-compose.yml`
  * `.env`

- You'll restart your containers with the new services having been added and configured.

* You'll re-index your Fedora Resource & SQL indices and ensure that the new Blazegraph triplestore is displaying triples.

- You can _optionally_ use and/or setup the new [TICK stack](tickstack.md) to monitor this container and service.

---

## Installation

### Assumptions

* Prior to installation, enduser will have a running ISLE system using the current release of `1.1.1.` images.

- This installation process will give the functionality as stated in the `Systems Requirements` image table above for `Blazegraph` testing and even `TICK` stack usage.

* Assumes you're prepared to configure your ISLE system to use Blazegraph instead of the default Mulgara triplestore.

- Assumes the new Blazegraph port `8084` will not be open to the public Internet only to select trusted administrators.

---

### Installation Instructions

* Shut down your running containers
  * `docker-compose down`

- Make the below mentioned edits to:
  * `docker-compose.yml` file
  * `.env` file

#### Blazegraph Edits - docker-compose.yml file

* For Phase II UAT testing of TICK, Blazegraph and Varnish please change the following image tags of these services from `1.1.1` to `dashboards-dev`
  * Apache
      * `image: islandoracollabgroup/isle-apache:1.1.1` should now become `image: islandoracollabgroup/isle-apache:dashboards-dev`
  * Image-services
      * `image: islandoracollabgroup/isle-imageservices:1.1.1` should now become `image: islandoracollabgroup/isle-imageservices:dashboards-dev`
  * MySQL 
    * `image: islandoracollabgroup/isle-mysql:1.1.1` should now become `image: islandoracollabgroup/isle-mysql:dashboards-dev`
  * Solr
    * `image: islandoracollabgroup/isle-solr:1.1.1` should now become `image: islandoracollabgroup/isle-solr:dashboards-dev`

* Add the new `Blazegraph`service to your docker-compose file:

```bash
isle-blazegraph:
    #build:
    #  context: ./images/isle-blazegraph
    image: islandoracollabgroup/isle-blazegraph:dashboards-dev
    container_name: isle-blazegraph-${CONTAINER_SHORT_ID}
    environment:
      - JAVA_MAX_MEM=4096M
      - JAVA_MIN_MEM=1024M
    env_file:
      - tomcat.env
    networks:
      - isle-internal
    ports:
      - "8084:8080"
    volumes:
      - isle-blazegraph-data:/var/bigdata
```

* Edit the `Fedora` service definition so that the default Fedora tag (1.1.1. as of June 5,2019_)is replaced with the `isle-fedora:blazegraph.dashboards-dev` image for use with Blazegraph. This Fedora image is configured to use either the Blazegraph or default Mulgara triplestore. By default it is set to Mulgara  

```bash
  fedora:
    # build:
    #   context: ../images/isle-fedora
    image: islandoracollabgroup/isle-fedora:1.1.1
```

becomes

```bash

 fedora:
    # build:
    #   context: ../images/isle-fedora
    image: islandoracollabgroup/isle-fedora:blazegraph.dashboards-dev
```

---

#### Blazegraph Edits - .env file

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

* Add another new block of ENV variables to your main .env file. Above the logging section and beneath all other defined services is fine. These ENV variables are for Blazegraph logging.

```bash

## Blazegraph
# These logs contain information and output concerning the Blazegraph
# graph-database and Fedora triplestore
# 
# Available output values range from most verbose to least (left to right): 
# ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF (Turns off logging)
#
### Recommended levels are WARN for all logs except the Rule execution log otherwise
# performance issues could occur. This is a lot of data suddenly being logged. Only
# Use in the event of needed issue debugging. Otherwise do not change from default settings.
#
# Change these log levels below for only for general purpose debugging of Blazegraph
# Default setting is: WARN
BLAZEGRAPH_ROOT_CATEGORY_LOG=WARN
BLAZEGRAPH_BIGDATA_LOG=WARN
BLAZEGRAPH_BIGDATA_BTREE_LOG=WARN

# Rule execution log.
# Default setting is: INFO
BLAZEGRAPH_BIGDATA_RULE_LOG=INFO

```

* Add another new block of ENV variables to your main .env file. Underneath the `### Fedora internal call password ` section and above the `## End Fedora Repository` section is ideal.

  * If using Blazegraph, you will have to change the `FEDORA_RESOURCE_INDEX=mulgara` to `FEDORA_RESOURCE_INDEX=blazegraph`

  * Whether using Mulgara or Blazegraph, please leave the `FEDORA_WEBAPP_HOME` value to the default below.

```bash
### Fedora Resource Index
#
# Only one of two values (mulgara) or (blazegraph) can be used below for FEDORA_RESOURCE_INDEX 
# By DEFAULT the value of (mulgara) is used to set which type of Fedora Resource Index can be used
# https://wiki.duraspace.org/display/FEDORA38/Resource+Index
# If you would like to use blazegraph instead, you'll also need to use its image
# https://github.com/Islandora-Collaboration-Group/isle-blazegraph
# Please note: If you mistype or leave this value blank, more than likely Fedora won't work properly.
FEDORA_RESOURCE_INDEX=mulgara

# Set below as a variable for Fedora to start up properly, helps with env-set.sh 
FEDORA_WEBAPP_HOME=/usr/local/tomcat/webapps/fedora

## End Fedora Repository
```

* Pull down the new isle images
  * `docker-compose pull`

* Spin up new containers
  * `docker-compose up -d`

* Wait a few minutes (_3-8 depending on the size of your site_) for the site and services to come up.

#### Check Fedora

* Q: Does Fedora start up properly?
  * Navigate to your domain e.g. http://yourdomain:8081/manager/html or 
  * Login using the correct admin / tomcat pw ) (check tomcat.env)
  * Click on the Fedora link or navigate to http://yourdomain:8081/fedora/objects and click search. 
  * Objects might appear if working but they should only be a small amount of content models at this point.

* If you have switched to Blazegraph, then you'll need to re-index the Fedora repository so that the new Blazegraph triplestore is used instead of the previously used Mulgara triplestore. 

  * **WARNING** - With Fedora repositories of 600K+ objects or more, **these indexing processes will take double-digit hours to days** depending on the complexity of the object relationships, ontology, etc. 
  
  * When re-indexing in this manner, we recommend the use of the `screen` program which will allow an end-user to disengage from a long continuous bash session and terminal based command without breaking the process.

    * You may need to install `screen` on your ISLE host server.
      * Ubuntu - `sudo apt-get install screen`
      * CentOS - `sudo yum install screen`

  * Once installed simply run `screen` in your terminal. You might be instructed to hit the Spacebar or the Return key to proceed with the `screen` session.

  * `docker-exec -it isle-fedora-ld bash`
  
  * `cd utility_scripts`

  * `./rebuildFedora.sh`

  * This process should start spewing out a large column of data

  * To disengage hold down the `control` key and the `a` key, then tap the `d` key. This will get you out of the screen session. 

  * To check on the progress, simply type `screen -r`, check the progress and then disengage again.

  * Once that is complete run the SOLR reindex script
    * `docker-exec -it isle-fedora-ld bash -c "/bin/sh /utility_scripts/updateSolrIndex.sh`

* Verify it's working by:
  * Navigating to https://yourdomain and you can still see the Drupal site. This may take a few minutes depending on the size of the site.

### Check the Blazegraph triples count

Checking the triples count is an easy way to double-check if objects are being indexed properly and added to Blazegraph's triplestore.

To check the triples count in Blazegraph:

* Navigate to the Blazegraph Admin panel http://isle.localdomain:8084/blazegraph/#query
  * Replace `isle.localdomain` with your real domain when using in Staging or Production

* Copy and paste the following query into the field with the text (_Input a SPARQL query_):

```bash
SELECT (COUNT(*) AS ?triples) WHERE {?s ?p ?o}
```

* Click the `Execute` button.

* The output at the bottom will be a numerical value that will look something like:

```bash
-----------
| triples |
| ------- |
| 8280864 |
-----------
```

* This value should increase as more objects are ingested and indexed by Fedora. The difference is that instead of these values being in the previously used Mulgara triplestore, they are now handled by Blazegraph.

---

## Using Blazegraph & the TICK stack

* If you're pushing log events to [TICK](tickstack.md), add this snippet of code below (_logging instructions_) to the bottom of **every** ISLE service within your `docker-compose.yml` file.

```bash
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

****Example:****

```bash

  isle-blazegraph:
    #build:
    #  context: ./images/isle-blazegraph
    image: islandoracollabgroup/isle-blazegraph:dashboards-dev
    container_name: isle-blazegraph-${CONTAINER_SHORT_ID}
    environment:
      - JAVA_MAX_MEM=4096M
      - JAVA_MIN_MEM=1024M
    env_file:
      - tomcat_blazegraph.env
    networks:
      - isle-internal
    ports:
      - "8084:8080"
    volumes:
      - isle-blazegraph-data:/var/bigdata
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

* Additionally you'll need to remove or comment out every line or reference to logs from the `volumes` section of each service.

****Example:****

```bash
    volumes:
      - isle-blazegraph-data:/var/bigdata
      - ./logs/blazegraph:/usr/local/tomcat/logs
```

becomes

```bash
    volumes:
      - isle-blazegraph-data:/var/bigdata
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

* [Blazegraph Documentation](https://wiki.blazegraph.com/wiki/index.php/Main_Page)
