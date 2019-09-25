# Replacing Mulgara with Blazegraph

## Use Cases & User Stories

* As a repository administrator, I expect to be able to ingest millions of objects into an ISLE-based system without incurring significant performance degradation.

## New Functionality

* The new `isle-blazegraph` image was built using [Blazegraph™](https://www.blazegraph.com/) which is a ultra high-performance graph database supporting Apache TinkerPop™ and RDF/SPARQL APIs. It supports up to 50 Billion edges on a single machine. Formerly known as BigData. The intended use is solely for maintaining performant support of large Fedora repositories containing millions of objects.

* Why use this component with ISLE?
  * Islandora users who have Fedora repositories with over 600K~ objects ingested have reported issues with using the default Mulgara triplestore used for indexing objects ingested into the Fedora repository. These issues include crashes, extreme performance slowdowns,system timeouts and more. Blazegraph is used to replace Mulgara as the triplestore and to deliver a higher level of performance for larger Fedora repositories. Please note the threshold given above for object count is an compilation of the anecdotal ranges gathered by the Islandora community. In some cases the number is as low as 600K~ when endusers report challenges, others haven't encountered issues until the 1 million object count.

* How can I tell how many Fedora objects I have to determine if I should use this component?
  * Walkthrough on getting triple counts. (TO DO)

* A new Fedora image that has slight image and functional changes is required to be used in tandem with Blazegraph. These image changes are primarily edits to the `fedora.fcg` file to use Blazegraph instead of Mulgara.
  
* Additionally, a new `confd` configuration within the Fedora image and linked environmental variable has been added to the `.env` to allow users to "toggle" between Blazegraph and Mulgara as desired. In the event a user toggles between either desired triplestore, a reindex of the Fedora repository is required. With Fedora repositories of 600K+ objects or more, **the indexing process will take double-digit hours to days** depending on the complexity of the object relationships, ontology, etc.

* No new software is required to be installed on the ISLE host machine, only new Docker containers, images and configurations are added to the ISLE platform.

---

## System Requirements

* [ISLE](https://github.com/Islandora-Collaboration-Group/ISLE) release version `1.3.0`
  * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

### ISLE Images

(_These tags are for usage during Phase II Sprints only and will change during the release process._)

(_Phase II Sprints only_)

| Service | Repository | Tag |
| ---     | ---        | --- |
| Apache | [islandoracollabgroup/isle-apache](https://cloud.docker.com/u/borndigital/repository/docker/islandoracollabgroup/isle-apache/tags) | `ISLE-v.1.3.0-dev`|
| Blazegraph | [islandoracollabgroup/isle-blazegraph](https://cloud.docker.com/u/borndigital/repository/docker/islandoracollabgroup/isle-blazegraph) | `ISLE-v.1.3.0-dev`|
| Fedora | [islandoracollabgroup/isle-fedora](https://cloud.docker.com/u/borndigital/repository/docker/islandoracollabgroup/isle-fedora/tags) | `ISLE-v.1.3.0-dev`|
| Image-services | [islandoracollabgroup/isle-imageservices](https://cloud.docker.com/u/borndigital/repository/docker/islandoracollabgroup/isle-imageservices) | `ISLE-v.1.3.0-dev` |
| MySQL | [islandoracollabgroup/isle-mysql](https://cloud.docker.com/u/borndigital/repository/docker/islandoracollabgroup/isle-mysql) | `ISLE-v.1.3.0-dev` |
| Portainer | [portainer/portainer](https://hub.docker.com/r/portainer/portainer) | `latest` |
| Solr  | [islandoracollabgroup/isle-solr](https://cloud.docker.com/u/borndigital/repository/docker/islandoracollabgroup/isle-solr/tags) | `ISLE-v.1.3.0-dev` |
| Traefik | [traefik/traefik](https://hub.docker.com/_/traefik) | `1.7.9` |

* Additional systems overhead, including:
  
  * Add an additional 2 GB RAM in total ISLE Host memory for Blazegraph to run
  
  * Plan on an additional 25-100+ GB disk space for “big_data” resource index persistence if running on `Staging` and/or `Production` ISLE instances.
    * This index (`/var/bigdata/bigdata.jnl`) will grow quickly overtime and file sizes of over 25GB or more are possible with Fedora repositories that have over 1 million objects.

  * To persist this file and directory in a bind-mount instead of a Docker volume, an edit will need to be made to the `docker-compose.yml` file in the `blazegraph` service, `volume` section.
    * Change `isle-blazegraph-data` to your ISLE Host system specific bind-mount path. **Example:**
      * `- /mnt/isle-blazegraph-data:/var/bigdata`
  * This disk should be a separate data disk that should be 25-100+ GB in capacity. Plan on resizing this disk when using Blazegraph with large Fedora repositories of over 600K+ of objects.

---

## Adoption Process Overview

* The installation instructions below will walk you through how to setup and run the optional Blazegraph container for use as an replacement triplestore for Fedora repositories with 600K+ of objects on either an ISLE demo or local instance.

* You'll start by backing up all important data as needed.

* You'll stop any running containers

* You'll download new ISLE images temporarily tagged as `ISLE-v.1.3.0-dev` instead of the standard ISLE `1.3.0`.
  * **Please note:** _This is a temporary process until all ISLE Phase II UAT testing is completed and the images can be released._
  * You'll download a new ISLE image called `isle-blazegraph:ISLE-v.1.3.0-dev`

* You'll make additional edits and modifications to the following ISLE configuration files:
  * `docker-compose.local.yml`
  * `local.env`

* You'll restart your containers with the new services having been added and configured.

* You'll re-index your Fedora Resource & SQL indices and ensure that the new Blazegraph triplestore is displaying triples.

* You can _optionally_ use and/or setup the new [TICK stack](tickstack.md) to monitor this container and service on `Staging` and `Production` instances.

---

## Installation

### Assumptions

* Prior to installation, enduser will have a running ISLE system using the current release of `1.3.0` images.

* This installation process will give the functionality as stated in the `Systems Requirements` image table above for `Blazegraph` testing and even `TICK` stack usage.

* Assumes you're prepared to configure your ISLE system to use Blazegraph instead of the default Mulgara triplestore.

* Assumes the new Blazegraph port `8084` will not be open to the public Internet only to select trusted administrators.

---

### Installation Instructions

* Shut down your running containers if any. Precautionary step.
  * `docker-compose down`

* For Phase II UAT testing of Blazegraph and Varnish the image tags of these services are already in place to use the newly created `borndigital` images for testing.

* Within the `local.env` or `demo.env` file for testing, there is a new block of ENV variables.

  * If using Blazegraph, you will have to change the `FEDORA_RESOURCE_INDEX=mulgara` to `FEDORA_RESOURCE_INDEX=blazegraph`

  * Whether using Mulgara or Blazegraph, please leave the `FEDORA_WEBAPP_HOME` value to the default below.

**Example:**
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

* On the `demo` and/or `local` instance, you will have to uncomment the volume at the bottom of the `docker-compose.demo.yml` or `docker-compose.local.yml` file to use Blazegraph.

* Pull down the new isle images
  * `docker-compose pull`

* Spin up new containers
  * `docker-compose up -d`

* Wait a few minutes (_3-8 depending on the size of your site_) for the site and services to come up.

#### Check & Rebuild Fedora when using Blazegraph

* Q: Does Fedora start up properly?
  * Navigate to your domain e.g. http://yourdomain:8081/manager/html
  * Login using the correct admin / tomcat pw ) (check tomcat.env)
  * Click on the Fedora link or navigate to http://yourdomain:8081/fedora/objects and click search.
  * Objects might appear if working but they should only be a small amount of content models at this point.

* If you have switched to Blazegraph, then you'll need to re-index the Fedora repository so that the new Blazegraph triplestore is used instead of the previously used Mulgara triplestore.

  * **WARNING** - With Fedora repositories of 600K+ objects or more, **these indexing processes will take double-digit hours to days** depending on the complexity of the object relationships, ontology, etc.
  
  * When re-indexing in this manner, we recommend the use of the `screen` program which will allow an end-user to disengage from a long continuous bash session and terminal based command without breaking the process. (_optional and typically used more often in a `Staging` and/or `Production` environment_)

    * You may need to install `screen` on your ISLE host server. Skip this if using on a demo or local instance.
      * Ubuntu - `sudo apt-get install screen`
      * CentOS - `sudo yum install screen`

  * Once installed simply run `screen` in your terminal. You might be instructed to hit the Spacebar or the Return key to proceed with the `screen` session.

  * `docker-exec -it isle-fedora-ld bash` (_Example for demo and/or local_)
  
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

Example is for a `Staging` instance

```bash

  isle-blazegraph:
    image: borndigital/isle-blazegraph:ISLE-v.1.3.0-dev
    container_name: isle-blazegraph-${CONTAINER_SHORT_ID}
    environment:
      - JAVA_MAX_MEM=4096M
      - JAVA_MIN_MEM=1024M
    env_file:
      - staging.env
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

---

## Uninstallation Instructions

* Comment out the `isle-blazegraph` service again within your `docker-compose.*.yml` file
* Change the the `blazegraph` back to `mulgara` within your `local.env` or `demo.env` file
* Start up the ISLE containers again.
* Re-index Fedora following the Fedora re-indexing steps provided in the `Check & Rebuild Fedora when using Blazegraph` section.

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
