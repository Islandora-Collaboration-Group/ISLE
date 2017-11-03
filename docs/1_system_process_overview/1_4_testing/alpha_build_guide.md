### Alpha Build Pre-Requisites


* This current alpha has been build and tested on **Docker For Mac** only. (11/2017)
     * Please install the [latest version](https://download.docker.com/mac/stable/Docker.dmg) from the [Docker store](https://store.docker.com/editions/community/docker-ce-desktop-mac)  


* This alpha build uses `islandora-docker.com` as the test domain along with the Docker Compose service names e.g. `db, fedora, web` etc.
     * To ensure this domain resolves properly, one will need to edit their local `/etc/hosts` file.
     * Open up a terminal and enter: `vi /etc/hosts`
     * Add the following:
      >127.0.0.1       localhost islandora-docker.com fedora web fedora.islandora-docker.com web.islandora-docker.com

---

### Build process

**Please note:** *The first container (MySQL, isle-db, db) has to be built and running PRIOR to all others (including Fedora & web) due to a race condition (fedora starts prior to mysql being ready to accept connections). This improper state will be fixed at a later point in the project.*  

* **DO NOT RUN** `docker-compose up -d` during the initial build process as this will build and run all containers at the same time which will trigger the above mentioned race condition and subsequent chain of service failures.

#### 1. MySQL container (10-15 mins)

* `docker-compose build db`  
* `docker-compose up -d db`  

#### 2. Fedora container (30 - 45 mins)  

* `docker-compose build fedora`  
* `docker-compose up -d fedora`  

#### 3. Solr container (20 - 40 mins)  

* `docker-compose build solr`  
* `docker-compose up -d solr`  

#### 4. Web container (45 - 60 mins)

* `docker-compose build web`  
* `docker-compose up -d web`  
    * **Please note:** *This container on occasion has failed to start initially for as of yet unlogged and unknown reasons.*
        * One can check if the container is running: `docker ps` (shows only running containers)  
        * One can check if the container stopped running or "exited": `docker ps -a` (shows all containers running or not)  

#### 5. Install script on Web container (45 - 60 mins)

The following manual steps will be folded into the web build process.

* Run the drush.sh script manually on the web container  
      * `docker exec -it isle-web bash`
      * `cd /tmp`
      * `chmod 777 drush.sh`
      * `./drush.sh`

* Fix open-seadragon issue
    * `cd /var/www/html/sites/all/libraries`
    * `wget https://github.com/openseadragon/openseadragon/releases/download/v2.2.1/openseadragon-bin-2.2.1.zip`
    * `unzip https://github.com/openseadragon/openseadragon/releases/download/v2.2.1/openseadragon-bin-2.2.1.zip`
    * `mv openseadragon-bin-2.2.1 openseadragon`
    * `cd /var/www/html/sites/all/modules`
    * `drush -u 1 en islandora_openseadragon`
    * The following message will appear:
      > The following extensions will be enabled: islandora_openseadragon  
        Do you really want to continue? (y/n): y
    * Hit `y` to continue
      * The following message should appear:
      > islandora_openseadragon was enabled successfully. [ok]
    * Enter: `drush openseadragon-plugin`

* Once finished `cntrl-D` or type `exit` to get out of the web container & QC the resulting setup

**Please note:** The cronjob setting in the `drush.sh` script is commented out as this will need to be flowed into the Docker build process prior. Issue with default Docker root user vs using islandora user. Drupal cron can run properly.



#### Total build process takes 2.5 - 4 hours (depending on system and internet speeds)

___

### Fast Facts

#### 1. MySQL container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| db                   | isle-db         | MySQL 5.6     | 3306          |


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

#### 4. Web container

| Compose Service Name | Container Name  | Software      | Ports                                      |
| :-------------:      | :-------------: | ------------- | -------------                              |      
| web                  | isle-web        | see below     | 80 (on container) mapped to 8888 (on host) |


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
| islandora_docker_admin | islandoradockeradminpw2017    | Drupal site admin     | http://islandora-docker.com:8888             |
