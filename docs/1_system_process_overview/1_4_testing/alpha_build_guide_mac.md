### Alpha Manual Build - MacOS
Please note this an alternative method of building the `isle.localdomain` Docker images and the slower of the two processes.

Please refer to the [1.4. -Testing - isle.localdomain Quickstart Guide](alpha_isle_localdomain_quickstart.md) for the **faster** version.

Or use the [1.4. -Testing - Migration Guide](alpha_migration_guide.md) after completing this process, if the enduser is going to edit the Dockerfile to make changes prior to the build.

### Alpha Manual Build Pre-Requisites

* The Host Server has already been setup and is running. If one has not setup the Host server please follow one of the following links below and then return to this document please.

    * Host Server setup for [**CentOS 7**](host_server_setup_centos.md)

    * Host Server setup for [**Ubuntu 16.04 LTS**](host_server_setup_ubuntu.md)

    * Host Server setup for [**Mac OS**](host_server_setup_macos.md)

* By default the `Docker-Compose.yml` file is configured for Linux Host Servers.

    * If one is using a **Mac OS** Host server, then edit the `docker-compose.yml` file to ensure the following lines look like this:   
```
    # - ./customize/apache/site/linux_settings.php:/var/www/html/sites/default/settings.php
      - ./customize/apache/site/macosx_settings.php:/var/www/html/sites/default/settings.php  
```

---

### Alpha Manual Build MacOS

**Please note:** *The first container (MySQL, isle-mysql, mysql) has to be built and running PRIOR to all others (including fedora & apache) due to a race condition (fedora starts prior to mysql being ready to accept connections). This improper state will be fixed at a later point in the project.*  

* **DO NOT RUN** `docker-compose up -d` during the initial build process as this will build and run all containers at the same time which will trigger the above mentioned race condition and subsequent chain of service failures.

#### 1. MySQL image build & container launch (10-15 mins)

* `docker-compose build mysql`  
* `docker-compose up -d mysql`  

#### 2. Fedora image build & container launch (30 - 45 mins)  

* `docker-compose build fedora`  
* `docker-compose up -d fedora`  

#### 3. Solr image build & container launch (20 - 40 mins)  

* `docker-compose build solr`  
* `docker-compose up -d solr`  

#### 4. Apache image build & container launch (45 - 60 mins)

* `docker-compose build apache`

* Edit the `docker-compose.yml` file to ensure the following lines look like this:   
```
    # - ./customize/apache/site/linux_settings.php:/var/www/html/sites/default/settings.php
    - ./customize/apache/site/macosx_settings.php:/var/www/html/sites/default/settings.php  
```

* `docker-compose up -d apache`  
    * **Please note:** *This container on occasion has failed to start initially for as of yet unlogged and unknown reasons.*
        * One can check if the container is running: `docker ps` (shows only running containers)  
        * One can check if the container stopped running or "exited": `docker ps -a` (shows all containers running or not)  

#### 5. Install script on Apache container (45 - 60 mins)

* Run the following shell scripts manually on the apache container  
      * `docker exec -it isle-apache bash`
      * `cd /tmp`
      * `chmod 777 *.sh`
      * `./make_site.sh`
      * `./install_site.sh`

* Once finished `cntrl-D` or type `exit` to get out of the apache container & QC the resulting setup

**Please note:** The cronjob setting in the `install_site.sh` script is commented out as this will need to be flowed into the Docker build process prior. Issue with default Docker root user vs using islandora user. Drupal cron can run properly manually.


#### Total build process takes 2.5 - 4 hours (depending on system and internet speeds)

___

### Fast Facts

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
| apache                  | isle-apache        | see below     | 80                                   |


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
| islandora_docker_admin | islandoradockeradminpw2017    | Drupal site admin     | http://islandora-docker.com                  |
