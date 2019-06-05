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

## Systems Requirements

* [ISLE](https://github.com/Islandora-Collaboration-Group/ISLE) release version `1.1.1`
  * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

### ISLE Images

* Following the installation steps below, an enduser will configure  / edit their ISLE running system(s) to ultimately use the following images and tags from Docker-Hub:

(_These tags are for usage during Phase II Sprints only and will change during the release process._)

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

---

## Adoption Process Overview

*

*

---

## Installation Instructions

### Installation Assumptions

* Prior to installation, enduser will have a running ISLE system using the current release of `1.1.1.` images.

* This installation process will give the functionality as stated in the `Systems Requirements` image table above for `Varnish` testing and TICK stack usage.

### Installation

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

* Please note: if you intend to use TICK with Varnish please change the following image tags of these services from `1.1.1` to `dashboards-dev`
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

---

#### Varnish Edits - docker-compose.yml file (_TICK only instructions_)

* If you're pushing log events to TICK, add this snippet of code below (_logging instructions_) to the bottom of **every** ISLE service.

```bash
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

**For example:**

```bash

solr:
    # build:
    #   context: ../images/isle-solr
    image: islandoracollabgroup/isle-solr:dashboards-dev
    container_name: isle-solr-${CONTAINER_SHORT_ID}
    environment:
      - JAVA_MAX_MEM=512M
      - JAVA_MIN_MEM=0
    env_file:
      - tomcat.env
    networks:
      - isle-internal
    ports:
      - "8082:8080"
    depends_on:
      - mysql
    volumes:
      - isle-solr-data:/usr/local/solr
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```        

* Additionally you'll need to remove or comment out every line or reference to logs from the `volumes` section.

**For example:**

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

#### Varnish Edits - docker-compose.yml file _continued..._

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
* Add a new block of ENV variables to your main .env file:

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

---
