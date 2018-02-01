### Alpha isle.localdomain Quickstart Guide Notes

This Alpha Quickstart guide is intended to be the **fastest** method of using ISLE to test.

This guide enables an enduser to spin up and install the "baked in" sample site, `isle-localdomain` for review and testing.

Previous guides used a manual "build" process which is still possible but not necessary for most endusers due to complexity and process length.

Current documentation will now refer to an "install" process using Dockerhub images instead of a "build" process where the enduser manually built these images.

If one would like to manually build the images please refer to one of the following Alpha Build guides:

* For [Linux Host Servers](alpha_build_guide_linux.md)

* For [MacOS Host Servers](alpha_build_guide_mac.md)

### Dockerhub images
Four Dockerhub images have already been "built" and are stored in a repository thus saving the endusers hours of build time.  Use the images tagged with `alpha2`.

* isle-apache [https://hub.docker.com/r/islandoracollabgroup/isle-apache/](https://hub.docker.com/r/islandoracollabgroup/isle-apache/)  

* isle-fedora [https://hub.docker.com/r/islandoracollabgroup/isle-fedora/](https://hub.docker.com/r/islandoracollabgroup/isle-fedora/)  

* isle-mysql [https://hub.docker.com/r/islandoracollabgroup/isle-mysql/](https://hub.docker.com/r/islandoracollabgroup/isle-mysql/)  

* isle-solr  [https://hub.docker.com/r/islandoracollabgroup/isle-solr/](https://hub.docker.com/r/islandoracollabgroup/isle-solr/)  

### Document Assumptions / Prerequisites:  

* The Host Server has already been setup and is running. If one has not setup the Host server please follow one of the following links below and then return to this document please.  

    * Host Server setup for [**CentOS 7**](host_server_setup_centos.md)  

    * Host Server setup for [**Ubuntu 16.04 LTS**](host_server_setup_ubuntu.md)  

    * Host Server setup for [**Mac OS**](host_server_setup_macos.md)  

* By default the `isle.localdomain` `docker-compose.yml` file is configured for Linux Host Servers. (found at the root of the ISLE Project directory)

    * If one is using a **Mac OS** Host server, then edit the `docker-compose.yml` file to ensure the following lines look like this:  
```
    - ./config/isle_localdomain/apache/macosx_settings.php:/var/www/html/sites/default/settings.php  
```

### isle_localdomain install process (same for all host server types)

**Please note:** *The first container (MySQL, isle-mysql, mysql) has to be running PRIOR to all others (including fedora & apache) due to a race condition (fedora starts prior to mysql being ready to accept connections). This improper state will be fixed at a later point in the project.*  **DO NOT RUN** `docker-compose up -d` during the initial install process as this will download all images and then run all containers at the same time which will trigger this race condition and subsequent chain of service failures.

* **During the initial install process** do the following steps to individually download each image and run each container.

#### 0. Add the ISLE Host server IP to the `docker-compose.yml` file

* Lines 39 & 68 both require the same value of the Host server IP address for the `extra_hosts` setting.

   **Example:**

```
   extra_hosts:
     - "isle.localdomain:192.168.1.1"
```
* Open a text editor, add this same IP to both lines and save the file.

* **Please note:**
    * This IP can be from a VM run on a local laptop or workstation.
    * If using Docker for Mac, then comment out these `extra_hosts` sections and add the following value of `127.0.0.1 isle.localdomain` to the laptop / workstation's `/etc/hosts` file.     

#### 1. MySQL image pull & container launch (10-15 mins)

* `docker pull islandoracollabgroup/isle-mysql:alpha2`  
* `docker-compose up -d mysql`  

#### 2. Fedora image pull & container launch (20 - 30 mins)  

* `docker pull islandoracollabgroup/isle-fedora:alpha2`  
* `docker-compose up -d fedora`  

* **(Optional but recommended)**
  * After spinning up fedora container, check if the Fedora service is running prior to advancing.
  * Navigate to http://fedora:8080/manager/html a popup login prompt should appear.
    * using the supplied username and password at the bottom of this page, enter the appropriate values from the **Fast facts** section at the bottom of the page.
      * see 2. Fedora container - service Tomcat, user is admin
    * upon login a large display of running Tomcat applications should display, scroll down to `fedora`
  * The application state / status should be `true`
  * If `false` appears instead, attempt to restart the fedora service manually.
    * Select the `restart` button to the right of the status area.
  * If it still fails, review the mounted fedora logs. The `docker-compose.yml` file will indicate where the logs are located.
  * using a command like `tail -n 300 <path to fedora logs>/fedora.log` should display enough information to troubleshoot.


#### 3. Solr image pull & container launch (10 - 20 mins)  

* `docker pull islandoracollabgroup/isle-solr:alpha2`  
* `docker-compose up -d solr`  

#### 4. Apache image pull & container launch (30 - 45 mins)

* `docker pull islandoracollabgroup/isle-apache:alpha2`  

* `docker-compose up -d apache`  

* **Please note:** *This container on occasion has failed to start initially for as of yet unlogged and unknown reasons.*  
    * One can check if the container is running: `docker ps` (shows only running containers)  
    * One can check if the container stopped running or "exited": `docker ps -a` (shows all containers running or not)  

#### 5. Install script on Apache container (20 - 40 mins)

* Run the following shell scripts manually on the apache container  
    * `docker exec -it isle-apache bash`
    * `cd /tmp/isle_drupal_build_tools/`
    * `chmod 777 *.sh`
    * `./apache_provision.sh`

* Once finished `cntrl-D` or type `exit` to get out of the apache container
* Test (QC) the resulting setup by opening a web browser to the `isle.localdomain` URL of the new ISLE sample site (i.e. http://isle.localdomain)

**Please note:** The cronjob setting in the `apache_provision.sh` script is still commented out as this will need to be flowed into the Docker build process prior. Issue with default Docker root user vs using islandora user. Drupal cron can run properly.

**Total build process** takes 15 - 35 minutes (_depending on system and internet speeds_)

___

### Fast Facts

`islandora` user on host server uses `isle2017` as password

#### 1. MySQL container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| mysql                   | isle-mysql         | MySQL 5.6     | 3306          |


| Account        | Password              | Database         | Perms                         |
| -------------  | -------------         | -------------    | -------------                 |      
| root           | islemysqlrootpw2017   | **ALL**          | **ALL**                       |
| fedora_admin   | dockerfeddb2017       | fedora3          | **All** except `Grant` option |
| islandora_user | islandoradockerdb2017 | islandora_docker | **All** except `Grant` option |

  ---

#### 2. Fedora container
| Compose Service Name | Container Name  | Software      | Ports                                            |
| :-------------:      | :-------------: | ------------- | -------------                                    |      
| fedora               | isle-fedora     | see below     | 8080, 80 (on container) mapped to 8777 (on host) |


| Software                         | Version           |
| -------------                    | -------------     |
| Fedora                           | 3.8.1             |
| Apache                           | 2.4.7             |
| Drupalfilter                     | 3.8.1             |
| Gsearch (w/remote SOLR plugin)   | 2.8+ (DG patched) |
| (DG) GSearch Extensions          | 0.13              |
| (DG) Islandora Transforms (XSLTs)| as of 11/2017     |
| Tomcat                           | 7.x               |  
| Oracle JDK                       | 8.x               |
| Djatoka                          | 1.1               |

| Account           | Password                      | Service       | URL           |
| -------------     | -------------                 | ------------- | ------------- |      
| fedoraAdmin       | dockerFEDadmin2017            | Fedora        | http://127.0.0.1:8080/fedora/describe                          |
| fedoraIntCallUser | dockerfedoraIntCallUserpw2017 | Fedora        | http://127.0.0.1:8080/fedora/objects                           |
| anonymous         | anonymous                     | Fedora        | ---                                                            |
| fgsAdmin          | dockerfgsAdminpw2017          | Gsearch       | http://127.0.0.1:8080/fedoragsearch/rest?operation=updateIndex |
| admin             | dockertcadminpw2017           | Tomcat        | http://127.0.0.1:8080/manager/html                             |
| manager           | dockertcmanpw2017             | Tomcat        | http://127.0.0.1:8080/manager/html                             |
| --                | --                            | Djatoka       | http://127.0.0.1:8080/adore-djatoka/                           |
| --                | --                            | Apache        | http://127.0.0.1:8777                                          |

___

#### 3. Solr container

| Compose Service Name | Container Name  | Software      | Ports                                              |
| :-------------:      | :-------------: | ------------- | -------------                                      |      
| solr                 | isle-solr       | see below     | 8993, 8080 (on container) mapped to 8091 (on host) |


| Software               | Version       |
| -------------          | ------------- |
| Solr                   | 4.10.4        |
| Tomcat                 | 7.x           |  
| Oracle JDK             | 8.x           |
| (DG) Basic Solr Config | 4.10.x branch |

| Account           | Password                      | Service       | URL                                |
| -------------     | -------------                 | ------------- | -------------                      |
| admin             | dockertcadminpw2017           | Tomcat        | http://127.0.0.1:8091/manager/html |
| manager           | dockertcmanpw2017             | Tomcat        | http://127.0.0.1:8091/manager/html |
| --                | --                            | Solr          | http://127.0.0.1:8091/solr/        |


___

#### 4. Apache container

| Compose Service Name | Container Name  | Software      | Ports                                      |
| :-------------:      | :-------------: | ------------- | -------------                              |      
| apache                  | isle-apache        | see below     | 80 |


| Software      | Version       |
| ------------- | ------------- |
| Apache        | 2.4.7         |
| Oracle JDK    | 8.x           |
| Djatoka       | 1.1           |
| Drupal        | 7.56          |
| PHP           | 5.6           |
| Islandora     | 7.x           |

| Account                | Password                      | Service               | URL                                          |
| -------------          | -------------                 | -------------         | -------------                                |
| islandora_docker_admin | islandoradockeradminpw2017    | Drupal site admin     | http://islandora-docker.com             |
