### Alpha Build Pre-Requisites

* This current alpha has been build and tested on a Digital Ocean droplet running a **Linux Ubuntu 16.04 LTS host**. (11/2017)

---

#### A. Host Server setup

* This alpha build uses `islandora-docker.com` as the test domain along with the Docker Compose service names e.g. `db, fedora, web` etc.
     * To ensure this domain resolves properly, one will need to edit the servers and/or their own local `/etc/hosts` file.
     * Open up a terminal and enter: `sudo vi /etc/hosts`
     * Add one or more of the following: (**Examples ONLY**)
         * Ubuntu Host Server (or VM)  
         > 127.0.0.1       localhost islandora-docker.com fedora web fedora.islandora-docker.com web.islandora-docker.com
         * Enduser local laptop
         > 192.156.123.26       islandora-docker.com fedora web fedora.islandora-docker.com web.islandora-docker.com

#### Install / run a Ubuntu 16.04 LTS server on  
* Virtualbox Virtual Machine (VM)  
* a Vagrant image (recommend using [bento/ubuntu-16.04](https://app.vagrantup.com/bento/boxes/ubuntu-16.04))  
* Cloud based server e.g. Digital Ocean or Amazon Web Services  
* Post-installation, add the appropriate ssh keys to the root user's `.ssh/authorized_keys` file.  

#### Docker setup

* Open a terminal, ssh to the Ubuntu server as root and install the following:  
     * `apt-get update`  
     * `apt-get install openssl git htop ntp`  

#### Install Docker  
* `curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -`  
* `add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"`  
* `apt-get update`  
* `apt-get install -y docker-ce`  
* Check if running `systemctl status docker`  

#### Create islandora user (as root)  
* `adduser islandora` (with password isle2017)  
* `visudo`  
* Add the following `islandora    ALL=(ALL:ALL) NOPASSWD:ALL`  
    * Under this section `root    ALL=(ALL:ALL) ALL`  
    * Hit `Cntrl-O` to write and save out file  
    * Hit `Cntrl-X` to exit file  
* Create Docker group `groupadd docker`  
* Add islandora user to docker group `usermod -aG docker islandora`  
* Exit out of the ssh session as the root user.


#### Install Docker-Compose (version 1.17.1 as of 11/16/2017) as islandora-user
* Open a terminal and ssh back into the Ubuntu Host Server/VM as the `islandora` user and perform the following:  
* `sudo curl -L https://github.com/docker/compose/releases/download/1.17.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose`  
* `sudo chmod +x /usr/local/bin/docker-compose`  


#### Clone ISLE repository
* `cd opt`
* `sudo git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`
   * This process will take 1 - 3 minutes depending on internet bandwidth
* `sudo chown -Rv islandora:islandora ISLE`

---

### B. Build process

**Please note:** *The first container (MySQL, isle-db, db) has to be built and running PRIOR to all others (including Fedora & web) due to a race condition (fedora starts prior to mysql being ready to accept connections). This improper state will be fixed at a later point in the project.*  

* **DO NOT RUN** `docker-compose up -d` during the initial build process as this will build and run all containers at the same time which will trigger the above mentioned race condition and subsequent chain of service failures.

#### 1. MySQL container (10-15 mins)

* `docker-compose build db`  
* `docker-compose up -d db`  

#### 2. Fedora container (20 - 30 mins)  

* `docker-compose build fedora`  
* `docker-compose up -d fedora`  

#### 3. Solr container (10 - 20 mins)  

* `docker-compose build solr`  
* `docker-compose up -d solr`  

#### 4. Web container (30 - 45 mins)

* `docker-compose build web`  

* Edit the `docker-compose.yml` file to ensure the following lines look like this:   
    > - ./customize/web/site/linux_settings.php:/var/www/html/sites/default/settings.php
      # - ./customize/web/site/macosx_settings.php:/var/www/html/sites/default/settings.php  

* `docker-compose up -d web`  
    * **Please note:** *This container on occasion has failed to start initially for as of yet unlogged and unknown reasons.*
        * One can check if the container is running: `docker ps` (shows only running containers)  
        * One can check if the container stopped running or "exited": `docker ps -a` (shows all containers running or not)  

#### 5. Install script on Web container (20 - 40 mins)

* Run the following shell scripts manually on the web container  
      * `docker exec -it isle-web bash`
      * `cd /tmp`
      * `chmod 777 *.sh`
      * `./make_site.sh`
      * `./install_site.sh`

* Once finished `cntrl-D` or type `exit` to get out of the web container & QC the resulting setup

**Please note:** The cronjob setting in the `install_site.sh` script is commented out as this will need to be flowed into the Docker build process prior. Issue with default Docker root user vs using islandora user. Drupal cron can run properly.

#### Total build process takes 1.5 -2.5 hours (depending on system and internet speeds)

___

### Fast Facts

`islandora` user on host server uses `isle2017` as password

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
