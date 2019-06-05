# Replacing Mulgara with Blazegraph

## Use Cases & User Stories

* As a repository administrator, I expect to be able to ingest millions of objects into an ISLE-based system without incurring significant performance degradation.

## New Functionality

* Several paragraphs describing the functional differences between Blazegraph and Mulgara
  
  * [Blazegraph™](https://www.blazegraph.com/) is a ultra high-performance graph database supporting Apache TinkerPop™ and RDF/SPARQL APIs. It supports up to 50 Billion edges on a single machine. Formerly known as BigData.

* Setting expectations for the upgrade.

  * Why use this component with ISLE?
    * Islandora users who have Fedora repositories with over 600K~ objects ingested have reported issues with using the default Mulgara triplestore used for indexing objects ingested into the Fedora repository. These issues include crashes, extreme performance slowdowns,system timeouts and more. Blazegraph is used to replace Mulgara as the triplestore and to deliver a higher level of performance for larger Fedora repositories. Please note the threshold given above for object count is an compilation of the anecdotal ranges gathered by the Islandora community. In some cases the number is as low as 600K~ when endusers report challenges, others haven't encountered issues until the 1 million object count.

  * How can I tell how many Fedora objects I have to determine if I should use this component?
    * Walkthrough on getting triple counts. (TO DO)

* Additional notes commenting on tradeoffs in removing Mulgara

* Additional notes concerning administrative overhead and/or necessary process changes
  * A new Fedora image that has slight image and functional changes is required to be used in tandem with Blazegraph. These image changes are primarily edits to the `fedora.fcg` file to use Blazegraph instead of Mulgara. 
  * Additionally, a new `confd` configuration within the Fedora image and linked environmental variable has been added to the `.env` to allow users to "toggle" between Blazegraph and Mulgara as desired. In the event a user toggles between either desired triplestore, a reindex of the Fedora repository is required. With Fedora repositories of 600K+ objects or more, **the indexing process will take hours to days** depending on the complexity of the object relationships, ontology etc.

* No new software is required to be installed on the ISLE host machine, only new Docker containers, images and configurations are added to the ISLE platform.

## Systems Requirements

* ISLE running system using the following images and tags from Docker-Hub:

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


* ISLE configuration from the [dashboards-dev](https://github.com/Born-Digital-US/ISLE/tree/dashboards-dev) branch on Born-Digital Fork
  * `git clone -b dashboards-dev https://github.com/Born-Digital-US/ISLE.git`

* Additional systems overhead, including:
  
  * Add an additional 2 GB RAM in total ISLE Host memory for Blazegraph to run
  
  * Plan on an additional 25 - 100+ GB disk space for “big_data” resource index persistence.
    * This index (`/var/bigdata/bigdata.jnl`) will grow quickly overtime and file sizes of over 25GB or more are possible with Fedora repositories that have over 1 million objects.

  * To persist this file and directory in a bind-mount instead of a Docker volume, an edit will need to be made to the `docker-compose.yml` file in the `blazegraph` service, `volume` section.
    * Change `isle-blazegraph-data` to your ISLE Host system specific bind-mount path. For example: 
      * `- /mnt/isle-blazegraph-data:/var/bigdata`
  * This disk should be a separate data disk that should be 25 - 100+ GB in capacity. Plan on resizing this disk when using Blazegraph with large Fedora repositories of over 600K+ of objects.

---

## Assumptions

* Assumes you have a running ISLE system using the `dashboards-dev` images

* Assumes you've configured your ISLE system to use Blazegraph

* Assumes port 8084 is not open to the public Internet only to select trusted administrators.

---

## Installation / Use with ISLE

*
*
*

---

## Installation

*
*

---

### Docker-compose.yml edits

*
*

---

## Troubleshooting

*
*

---

## Release Notes

*
*

---

## Additional Resources

### Check the Blazegraph triples count

Checking the triples count is an easy way to double-check if objects are being indexed properly and added to Blazegraph's triplestore.

To check the triples count in Blazegraph:

* Navigate to the Blazegraph Admin panel http://isle.localdomain:8084/blazegraph/#query
  * Replace `isle.localdomain` with your real domain when using in `Staging` or `Production`

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