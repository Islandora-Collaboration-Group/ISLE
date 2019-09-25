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

* The `isle-varnish` image was built using [Varnish](https://www.varnish-cache.org/) software to improve end-user performance when and if Drupal/Apache performance is inadequate for high-traffic production environments. Varnish can help to reduce load on the Apache web server and to increase the responsiveness of the Drupal site and Solr searches.

* The ISLE Varnish configuration uses an memory (RAM) based cache or _malloc_ with a default setting of `256 MB` which can and should be increased as needed. If you need to read more about how to use or change Varnish's cache settings, please start [here](https://varnish-cache.org/docs/4.1/users-guide/storage-backends.html)
  * **Please note:** There is a new Docker ENV setting within the `production.env` which allows users to dynamically increase or decrease the amount of stored materials within the Varnish _malloc_ cache.
    ```bash
    ## Varnish Cache Memory Allocation
    VARNISH_MALLOC=256m
    ```

* The Drupal module [Varnish](https://www.drupal.org/project/varnish) will need to be installed on the Drupal / Islandora site to provide further integration between the Varnish container and your website. There is an included script (isle_varnish_drupal_module_installer.sh) to install enable and configure the Drupal module on an existing site found in the ISLE `scripts/varnish` directory. This script will also include the Drupal [Purge](https://www.drupal.org/project/purge) and [Expire](https://www.drupal.org/project/expire) modules.

---

## System Requirements

* [ISLE](https://github.com/Islandora-Collaboration-Group/ISLE) release version `1.3.0`
  * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

* Existing ISLE `Local`, `Staging` and `Production` systems and running websites

* Existing ISLE git repository

* Existing Drupal / Islandora git repository

* Ability to allocate additional free memory to the Varnish container as needed. The tuning and configuration of Varnish can vary based on system resources and traffic, as we recommend that you start out with a smaller memory allocation and test the results.
  * **Example configuration** for a Production ISLE host server using `16 GB` of memory.
    * Expect to allocate about 40 - 50% of the host server memory for all of the Java / Tomcat based images
      * `isle-fedora` should have the most e.g. `4096 MB` or `4 GB`
      * `isle-solr` should have the second most e.g. `2048 MB` or `2 GB`
      * `isle-imageservices` should have the third most e.g. `1024 MB` or `1 GB`
      * If using `isle-blazegraph`, this image should also be using a minimum of `4096 MB` or `4 GB` of memory
    * Keep about `2 -3 GB` free for the remaining images e.g. `isle-apache`, `isle-mysql` etc.
    * This would leave you roughly `1 - 2 GB` to allocate to the Varnish cache. Start with `256` to `512 MB` and work your way up as needed.
      * You can adjust the amount that Varnish puts into memory in the supplied `.env` file
        * On the line: `VARNISH_MALLOC=256m` you can change the amount of memory to a higher value other than the default `256` Megabytes. We recommend that you start with 1-2 GB for now to tune further as your resources and needs warrant.
  * There are additional potential memory allocation and tuning recommendations for Varnish from [Varnish-software](https://info.varnish-software.com/blog/understanding-varnish-cache-memory-usage)
  * If you need to read more about how to use or change Varnish's cache settings, please start [here](https://varnish-cache.org/docs/4.1/users-guide/storage-backends.html)

* **Recommendation** Adding more memory to the `Production` ISLE host system from the default recommended `16 GB` might be recommended here if running all optional components e.g. `isle-varnish`,  `isle-blazegraph`, the TICK stack, etc.

* There is no new software required on any ISLE host machine.

---

### ISLE Images

* Following the installation steps below, an enduser will configure  / edit their ISLE running system(s) to ultimately use the following images and tags from Docker-Hub:

(_**Please note:** The tags below are for testing during Phase II Sprints only and will change during the release process._)

| Service | Repository | Tag |
| ---     | ---        | --- |
| Apache | [borndigital/isle-apache](https://cloud.docker.com/u/borndigital/repository/docker/borndigital/isle-apache/tags) | `1.3.0-dev`|
| Blazegraph | [borndigital/isle-blazegraph](https://cloud.docker.com/u/borndigital/repository/docker/borndigital/isle-blazegraph/tags) | `1.3.0-dev`|
| Fedora | [borndigital/isle-fedora](https://cloud.docker.com/u/borndigital/repository/docker/borndigital/isle-fedora/tags) | `1.3.0-dev`|
| Image-services | [borndigital/isle-imageservices](https://cloud.docker.com/u/borndigital/repository/docker/borndigital/isle-imageservices) | `1.3.0-dev` |
| MySQL | [borndigital/isle-mysql](https://cloud.docker.com/u/borndigital/repository/docker/borndigital/isle-mysql) | `1.3.0-dev` |
| Portainer | [portainer/portainer](https://hub.docker.com/r/portainer/portainer) | `latest` |
| Solr  | [borndigital/isle-solr](https://cloud.docker.com/u/borndigital/repository/docker/borndigital/isle-solr/tags) | `1.3.0-dev` |
| Traefik | [traefik/traefik](https://hub.docker.com/_/traefik) | `1.7.9` |
| Varnish | [borndigital/isle-varnish](https://cloud.docker.com/u/borndigital/repository/docker/borndigital/isle-varnish) | `1.3.0-dev`|

---

## Adoption Process Overview

The installation instructions below will walk you through how to setup and run the optional Varnish container on only your ISLE `Production` system to cache assets for highly trafficked Islandora sites in addition to adding new Drupal modules to your existing `Production` Drupal / Islandora website to interact and manage the Varnish cache.

**Please note**: ISLE endusers are of course welcome to run Varnish on their `Staging` systems if they want but it is recommended that endusers simply test on their `Local` instances, deploy code changes to both `Staging` and `Production` but only run Varnish on their `Production` ISLE systems.

* You'll start by backing up all important data as needed.

* On your `Local` ISLE Apache container, you'll run the Varnish installation script and then check in the resulting Drupal code changes into your Islandora / Drupal git repository.

* Following best practices of "code up and data down", pull the resulting Drupal code changes from your `Local` upstream to your `Staging` and `Production` systems.

* **RELEASE TESTING** You'll download new ISLE images temporarily tagged as `1.3.0-dev` instead of the standard ISLE `1.3.0`.
  * **Please note:** _This is a temporary process until all ISLE Phase II UAT testing is completed and the images can be released._
  * You'll download a new ISLE image called `isle-varnish:1.3.0-dev`

* You'll make additional edits and modifications to the following ISLE configuration files on your `Local` system, check them into git and then pull the code on your `Staging` and `Production` systems.
  * `docker-compose.production.yml`
    * Uncommenting the varnish service section
    * Commenting out a line in the apache service section
  * `production.env`
    * Making any necessary edits to the new Varnish ENV section

* You'll perform a `docker-compose pull` to pull down any new images.

* You'll restart your containers with the new services having been added and configured.

* You'll install the new Drupal module called Varnish.

* You can employ some of the methods mentioned below to ensure that the new Varnish caching service is working.

* You can use the production.env to change settings and "tune" the resource allocation for Varnish.

---

## Installation

### Assumptions

* Prior to installation, enduser will have a running ISLE system using the current release of `1.3.0.` images.

* This installation process will give the functionality as stated in the `Systems Requirements` image table above for `Varnish` testing and usage.

* Running ISLE `Local`, `Staging` and `Production` sites and systems with ingested objects and content.

---

### Installation Instructions

#### ISLE Local development

* Shut down your running containers on your ISLE `Local` instance.
  * `docker-compose down`

#### Edits - docker-compose.production.yml file

* **Release testers** - (You'll need to copy this commented out Varnish section below into your `docker-compose.local.yml` or `docker-compose.demo.yml` at the bottom below the Blazegraph section and above the `# Defined networks` section. You'll then need to uncomment everything between `varnish:` and `labels`. Do not uncomment the `logging` section on your test demo or local as most likely they don't use TICK.)

* Within your `docker-compose.production.yml` file, you'll need to uncomment the following section:

```bash
# Start - Varnish service section
## (Optional-component): Uncomment lines below to run ISLE with the Varnish cache

#  varnish:
#    image: borndigital/isle-varnish:1.3.0-dev
#    container_name: isle-varnish-${CONTAINER_SHORT_ID}
#    env_file:
#      - production.env
#      - .env
#    networks:
#      isle-internal:
#    depends_on:
#      - mysql
#      - fedora
#      - solr
#      - apache
#      - traefik
#    labels:
#      - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
#      - traefik.port=6081
#      - traefik.enable=true
#      - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /cantaloupe"
#    logging:
#      driver: syslog
#      options:
#        tag: "{{.Name}}"

# END - Varnish service section
```

so that it will now look like this and its formatting should line up appropriately with other ISLE services.

```bash
# Start - Varnish service section
## (Optional-component): Uncomment lines below to run ISLE with the Varnish cache

  varnish:
    image: borndigital/isle-varnish:1.3.0-dev
    container_name: isle-varnish-${CONTAINER_SHORT_ID}
    env_file:
      - production.env
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
      - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /cantaloupe"
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"

# END - Varnish service section
```

* **Please note:** If you **are not** using TICK with your `Production` system, then you don't need to uncomment the entire `logging:` area and lines. Leave them uncommented.

* Comment out the last line in your `apache` service `labels` section so that Varnish can "take over" handling and routing web traffic.

```bash
    labels:
      - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
      - traefik.enable=true
    #  - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /adore-djatoka, /cantaloupe"
```

* No change to traefik service: the varnish service now handles web traffic to your main domain.

---

#### Varnish Edits - production.env file

* **Release testers** - (You'll need to copy this commented out Varnish section below into your `local.env` or `demo.env` at the bottom below the Blazegraph section and above the `Logs` section. You'll then need to follow the same instructions for uncommenting lines.

* Within your `docker-compose.production.yml` file, you'll need to uncomment the following section and lines so that:

```bash
## Varnish
## Varnish Admin port is 6082
## varnish is substituted for default value of localhost to be open to apache
## otherwise Drupal varnish module cannot connect via CLI
#VARNISH_ADMIN=varnish
#VARNISH_ADMIN_PORT=6082
## Varnish backend
## the apache service is the "backend" for varnish
#VARNISH_BACKEND=apache
#VARNISH_BACKEND_PORT=80
## Varnish Cache Memory Allocation
#VARNISH_MALLOC=256m
## Maximum amount of connections
#VARNISH_MAX_CONNECTIONS=300
## Varnish secret aka Control key
#VARNISH_SECRET=isle_varnish_secret
## Varnish port
#VARNISH_VARNISH_PORT=6081
```

becomes:

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

* **Please note** ISLE endusers should only make edits or changes to the following lines:
  * VARNISH_MALLOC=256m - _you can increase this default setting as needed otherwise leave the default._
  
  * VARNISH_MAX_CONNECTIONS=300 - _you can increase this default setting as needed otherwise leave the default._
  
  * VARNISH_SECRET=isle_varnish_secret - _you can change this value to any alpha-numeric sequence._

---

* Pull down the new isle images
  * `docker-compose pull`

* Spin up new containers
  * `docker-compose up -d`

* Install the Varnish Drupal modules on your `Local` Drupal site
  * Use the supplied installer script: (_Examples given below, you may need to change container ids and paths accordingly to match your local environment_)
    * On the `Production` host server, copy this script to your apache container. Replace `{{ container_short_id }}` with your respective `Local` container id.
      * `docker cp scripts/varnish/isle_varnish_drupal_module_installer.sh isle-apache-{{ container_short_id }}:/var/www/html/isle_varnish_drupal_installer.sh`
    * Change permissions on the script
      * `docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html/ && chmod +x isle_varnish_drupal_installer.sh"`
    * Run the script
      * `docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html && ./isle_varnish_drupal_installer.sh"`

* Commit the changed files to your Islandora / Drupal git repository.
  * Navigate to this path on your `Local` host machine, typically this is the path bind-mounted in your `docker-compose.local.yml` to the Apache container's `/var/www/html` directory
  * `git status` - this will show you all of the changed files.
  * `git add path/file` - add each changed file as necessary
  * `git commit -m "Adding Varnish Drupal modules"` - Your git message can be anything of your choosing
  * `git push origin your_git_branch` - Replace `your_git_branch` with the actual git branch you are using for Islandora / Drupal development

* You can now access this new module in the `Home >> Administration >> Configuration >> Development >> Varnish` section of your Drupal site.
  * Please note: We recommend the following settings. All other settings should be handled by the vsets within the installer script.
    * `Flush page cache on cron?` set to `Disabled`
    * `Varnish version` set to `4.x`
    * `Varnish Control Key Append newline` checkbox should be `checked` with a `check mark`
    * `The Front domains list` can be left empty
    * `Varnish Cache Clearing` set to `Drupal Default`
    * `Varnish ban type` set to `Normal`

  * All other settings e.g. `Varnish Control Terminal` and `Varnish Control Key` are handled by the local.env ENV variables. (_if testing on local_)

* There will also be two other new Drupal modules to access and use:
  * **Purge** - Accessible from `Home » Administration » Configuration » Development » Performance`
    * The setting here is handled by the installer script, vset and Varnish ENV. No need to change this value by the enduser.
  * **Cache Expiration** - Accessible from `Home » Administration » Configuration » System`
    * The setting here is initially handled by the installer script, vset and Varnish ENV. Enduser can modify as needed but a further explination beyond default settings is out of scope of this document. Recommend using this Drupal modules help page if needed.

* There should be a nice green checkmark at the bottom to indicate `Varnish running` once / if you have also spun up the Varnish container. If you choose to not spin up and configure the Varnish container on your `Local`, your `Local` ISLE system will continue to run properly but these three Drupal modules may have a red warning or two about not being able to connect to Varnish.

* If testing Varnish on your `Local`, open up a web browser and navigate to your `Local` website.
  * If you recently restarted your Docker containers, this may take a few minutes depending on the size of the site.
    * You might first see a Traefik `Bad Gateway` page for a minute or two. You'll need to refresh the page.
    * You might then see a Varnish followed by a `Error 503 Backend fetch failed` page for a minute or two. You'll need to again refresh the page.
      * Example Varnish error upon site startup.
    * You should now see your Drupal website after a few minutes.  

**Example Varnish error**
```bash
Error 503 Backend fetch failed

Backend fetch failed
Guru Meditation:

XID: 3

Varnish cache server
```

* If continuing to test on your `Local`, please review the section below `How to verify that Varnish is working` for available testing methods.

* Once satisfied that everything is running properly on your `Local`, move onto the `Deployment to Production and Staging` section.

---

#### Deployment to Production and Staging

* Once satisfied that everything is running properly on your `Local`, you'll need to commit all remaining ISLE config changes as well to push upstream to your `Production` instance. While you can keep Varnish running on your local, we suggest that you instead back out the ISLE config changes on your `Local` and only keep them on `Production`.

* Commit the changed files to your ISLE git repository.
  * Navigate to the root of your ISLE project via a terminal or git GUI client.
  * `git status` - this will show you all of the changed files.
  * `git add path/file` - add each changed file as necessary
  * `git commit -m "Adding Varnish to Production"` - Your git message can be anything of your choosing
  * `git push origin master` - (_Replace `master` with the actual git branch you are using for ISLE development if needed_)

##### Quick deployment to Staging for code parity

* On your `Staging` system:
  * Shutdown your containers from your ISLE project directory root found typically in `/opt/`
    * `docker-compose down`
  * Run `git pull origin master` - (_Replace `master` with the actual git branch you are using for ISLE development if needed_)
    * While you may not be deploying Varnish to your `Staging` system, it is a wise idea to not have code drift.
  * Repeat this process with your Islandora / Drupal code to ensure parity between `Staging` and `Production`
  * Spin your containers back up
  * Run the supplied installer script: (_Examples given below, you may need to change container ids and paths accordingly to match your Staging environment_)
    * On the `Staging` host server, copy this script to your apache container. Replace `{{ container_short_id }}` with your respective `Staging` container id.
      * `docker cp scripts/varnish/isle_varnish_drupal_module_installer.sh isle-apache-{{ container_short_id }}:/var/www/html/isle_varnish_drupal_installer.sh`
    * Change permissions on the script
      * `docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html/ && chmod +x isle_varnish_drupal_installer.sh"`
    * Run the script
      * `docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html && ./isle_varnish_drupal_installer.sh"`
    * The script will complain about the Drupal modules being enabled but will complete.
   * Perform a quick QC of the site and system to ensure no issues have resulted. Once finished move onto deploying to `Production`

##### Deployment to Production

* On your `Production` system:
  * Shutdown your containers from your ISLE project directory root found typically in `/opt/`
    * `docker-compose down`
  * Run `git pull origin master` - (_Replace `master` with the actual git branch you are using for ISLE development if needed_)
  * Repeat this process with your Islandora / Drupal code using the appropriate git branch as needed

* Pull down the new isle images
  * `docker-compose pull`

* Spin up new containers
  * `docker-compose up -d`

 * Run the supplied installer script: (_Examples given below, you may need to change container ids and paths accordingly to match your Production environment_)
  * On the `Production` host server, copy this script to your apache container. Replace `{{ container_short_id }}` with your respective `Staging` container id.
    * `docker cp scripts/varnish/isle_varnish_drupal_module_installer.sh isle-apache-{{ container_short_id }}:/var/www/html/isle_varnish_drupal_installer.sh`
  * Change permissions on the script
    * `docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html/ && chmod +x isle_varnish_drupal_installer.sh"`
  * Run the script
    * `docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html && ./isle_varnish_drupal_installer.sh"`
  * The script will complain about the Drupal modules being enabled but will complete.

* You can now access this new module in the `Home >> Administration >> Configuration >> Development >> Varnish` section of your Drupal site.
  * Please note: We recommend the following settings. All other settings should be handled by the vsets within the installer script.
    * `Flush page cache on cron?` set to `Disabled`
    * `Varnish version` set to `4.x`
    * `Varnish Control Key Append newline` checkbox should be `checked` with a `check mark`
    * `The Front domains list` can be left empty
    * `Varnish Cache Clearing` set to `Drupal Default`
    * `Varnish ban type` set to `Normal`

  * All other settings e.g. `Varnish Control Terminal` and `Varnish Control Key` are handled by the production.env ENV variables.

* There will also be two other new Drupal modules to access and use:
  * **Purge** - Accessible from `Home » Administration » Configuration » Development » Performance`
    * The setting here is handled by the installer script, vset and Varnish ENV. No need to change this value by the enduser.
  * **Cache Expiration** - Accessible from `Home » Administration » Configuration » System`
    * The setting here is initially handled by the installer script, vset and Varnish ENV. Enduser can modify as needed but a further explination beyond default settings is out of scope of this document. Recommend using this Drupal modules help page if needed.

* There should be a nice green checkmark at the bottom to indicate `Varnish running` once / if you have also spun up the Varnish container. If any of the three Drupal modules have a red warning or two about not being able to connect to Varnish, then you'll need to retrace your steps and troubleshoot.

* Open up a web browser and navigate to your `Production` website.
  * If you recently restarted your Docker containers, this may take a few minutes depending on the size of the site.
    * You might first see a Traefik `Bad Gateway` page for a minute or two. You'll need to refresh the page.
    * You might then see a Varnish followed by a `Error 503 Backend fetch failed` page for a minute or two. You'll need to again refresh the page.
      * Example Varnish error upon site startup.
    * You should now see your Drupal website after a few minutes.  

**Example Varnish error**
```bash
Error 503 Backend fetch failed

Backend fetch failed
Guru Meditation:

XID: 3

Varnish cache server
```

* If continuing to test on your `Production` system, please review the section below `How to verify that Varnish is working` for available testing methods.

* We also recommend a final QC on your `Production` system, reviewing all logs, displayed objects and searches.

---

## How to verify that Varnish is working

Any of these methods below will work for testing on your `Local` or general use on `Production`

### Method 1 - Visit your website in a web browser

* Open up a web browser and navigate to your website.

* If you recently restarted your Docker containers, this may take a few minutes depending on the size of the site.
  * You might first see a Traefik `Bad Gateway` page for a minute or two. You'll need to refresh the page.
  * You might then see a Varnish followed by a `Error 503 Backend fetch failed` page for a minute or two. You'll need to again refresh the page.
    * Example Varnish error upon site startup.

```bash
Error 503 Backend fetch failed

Backend fetch failed
Guru Meditation:

XID: 3

Varnish cache server
```
  
* You should now see your website after a few minutes.

### Method 2 - Inspect the headers on one of your site's webpages

* Launch a web

* Inspecting the headers of different Drupal pages for the words hit.
  * (TO DO) Expand on `hit` or `miss` examples

### Check the Varnish backend admin tool for links
* Checking the Varnish cli for registered content / page requests
  * (TO DO) Expand on using Varnish console with 1 -2 examples

### Curl a url and review the headers

* Run a curl command on your local laptop to your site with `curl -I https://yourwebsitehere.com`

The output should look something like this example output below:

```bash
curl -I https://demo.born-digital.com

HTTP/2 200 
accept-ranges: bytes
age: 2075
cache-control: public, max-age=10800
content-language: en
content-type: text/html; charset=utf-8
date: Mon, 23 Sep 2019 19:27:42 GMT
etag: W/"1569264015-0-gzip"
expires: Sun, 19 Nov 1978 05:00:00 GMT
last-modified: Mon, 23 Sep 2019 18:40:15 GMT
server: Apache/2.4.41 (Ubuntu)
vary: Cookie,Accept-Encoding
via: 1.1 varnish-v4
x-cache: HIT
x-cache-hits: 19
x-content-type-options: nosniff
x-drupal-cache: HIT
x-frame-options: SAMEORIGIN
x-generator: Drupal 7 (http://drupal.org)
x-varnish: 983097 3
```

**Please note:** This information below is a callout for the enduser to understand indicates that Varnish is not only caching the page but the Varnish cache has received previous "hits" or requests for this page and its contents.

```bash
via: 1.1 varnish-v4
x-cache: HIT
x-cache-hits: 19
```

---

## How to clear the Varnish cache

* Everytime the Varnish container is stopped and restarted, the Varnish cache will be reset and rebuilt. (_Easiest and most recommended method_)

* Shell into the Varnish container (_Shouldn't involve restarting the container_). Replace `{{ container_short_id }}` with your respective container id.
  * `docker exec -it isle-varnish-{{ container_short_id }} bash`
  * `varnishadm -T 127.0.0.1:6082 url.purge .`

* Additional curl commands and vcl edits can be found within the [Varnish 4.1 documentation](https://varnish-cache.org/docs/4.1/users-guide/purging.html)

## Varnish utilities and tools

There are multiple tools that can be used to interacte with the Varnish cache. All will require that you use them on the running Varnish container.

* [varnishtop](https://varnish-cache.org/docs/4.1/reference/varnishlog.html) - The varnishtop utility reads varnishd shared memory logs and presents a continuously updated list of the most commonly occurring log entries.
* [varnishstat](https://varnish-cache.org/docs/4.1/reference/varnishstat.html#varnishstat-1) - for Varnish cache statistics
* [varnishlog](https://varnish-cache.org/docs/4.1/reference/varnishlog.html) - a utility that can read the contents of the in-memory log that Varnish provides
* [varnishhist](https://varnish-cache.org/docs/4.1/reference/varnishhist.html) - The varnishhist utility reads varnishd(1) shared memory logs and presents a continuously updated histogram
* [varnishadm](https://varnish-cache.org/docs/4.1/reference/varnishadm.html) - type in `varnishadm` and then `help` for additional commands. Utility used to control the Varnish cache.
* Additional commands can be found in the [Varnish Reporting & Statistics section](https://varnish-cache.org/docs/4.1/users-guide/operation-statistics.html)

---

## Using Varnish & the TICK stack

* Please first follow the instructions for installing and using the [TICK stack](tickstack.md)

* If you're pushing log events to [TICK](tickstack.md), this snippet of code below (_logging instructions_) at the bottom of **every** ISLE service within your `docker-compose.production.yml` file should be uncommented. This should include the Varnish service. By default the uncommented Varnish section in the `Production` `docker-compose.production.yml` file will have this snippet of code below.

```bash
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

****Example:****

```bash
varnish:
  image: islandoracollabgroup/isle-varnish:1.3.0
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

---

## Uninstallation Instructions

* Shutdown the ISLE containers on your `Local` system
  * Comment out the Varnish service section in either of the `docker-compose.production.yml` or `docker-compose.local.yml` file(s)
    * Uncomment the last line in the Apache `labels` section in either of the `docker-compose.production.yml` or `docker-compose.local.yml` file(s)
* Comment out the Varnish section again in the `production.env` or `local.env` file
* Startup the ISLE containers again.
* Shell into the Apache container
  * `cd /var/www/html`
  * `drush dis varnish purge expire`
  * `drush pm-uninstall varnish purge expire`
* Commit the resulting code changes in both the ISLE and Islandora git repositories.
* Deploy these changes to your `Staging` and `Production` systems

---

## Need help?

* Please use the following as resources for institutions or endusers needing support

  * [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody!
    * The [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT

  * [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support

  * [ISLE Github Issues queue](https://github.com/Islandora-Collaboration-Group/ISLE/issues) - Post your issues, bugs and requests for technical documentation here.

---

## Additional Resources

* [Varnish 4.1.x Documentation](https://varnish-cache.org/docs/4.1/index.html)

* [The Varnish Users Guide](https://varnish-cache.org/docs/4.1/users-guide/index.html)

* **Please note:**:
  * [Varnish Software](https://www.varnish-software.com/) is the commercial wing of the Varnish.
    * Varnish Admin Console is a [paid](https://www.varnish-software.com/solutions/varnish-enterprise/varnish-administration-console-2/) not for free product that is a GUI for Varnish Cache. The language around this feature is vague and sometimes misleadingly used in tutorials as software anyone can use. There are trials but ultimately this is a paid product.
  * [Varnish Cache](https://www.varnish-software.com/community/varnish-cache/) is the open-source project maintained by Varnish Software and intended to be used by anyone for free.
