## MacOS Docker version of the Development Site Guide - isle.localdomain

**WORK IN PROGRESS**

Until recently, a networking bug made the Docker-for-Mac setup for ISLE an unreliable practice. Thanks to major efforts from the open-source software community (esp. Diego - thanks Diego!) the source of this bug (an extraneous jar file) has been located.

The latest versions of ISLE have not yet been tested using Docker-for-Mac. Once that has been done and the experience is documented - the rest of this page will be more accurate. Currently this page is merely a suggestion of steps, not a proper walk-through. If you do successfully install ISLE w/ Docker-for-Mac and feel like contributing your documented steps - please send them to the ISLE contact, post an issue on github or make a pull request on documentation files.



------------

This guide enables an enduser to spin up and install the "baked in" sample ISLE / Islandora environment, `isle-localdomain` for review and testing.

This test ISLE / Islandora environment (`isle.localdomain`) includes an un-themed Drupal website and empty Fedora repository for endusers to develop code, test ingests, test metadata, update fields in SOLR indexing and otherwise "kick the tires" on ISLE prior for further usages e.g. creating a new ISLE production site or migrating a current Islandora production site to ISLE.

While this checklist will attempt to point out most of the usage challenges or pitfalls, ISLE assumes no responsibility or liability in this matter should an enduser have customizations beyond what this guide outlines.

**Notes**

* The first container (MySQL, isle-mysql, mysql) **has to be running PRIOR** to all others (including fedora & apache) due to a race condition (fedora starts prior to mysql being ready to accept connections). This improper state will be fixed at a later point in the project.  

* **DO NOT RUN** `docker-compose up -d` during the initial build process as this will build and run all containers at the same time which will trigger the above mentioned race condition and subsequent chain of service failures.

### Assumptions / Prerequisites

* Enduser has a local laptop / workstation that conforms to the specifications outlined in the [ISLE MVP Host Specifications Guide](../mvpspecs.md)

* This testsite guide is designed for a local laptop / workstation running one of the following:
   * using the supplied Vagrant setup found in the `vagrant` directory at the root of the ISLE project (_Vagrant with Virtualbox_)
   * a manually setup CentOS Virtualbox VM (_no Vagrant_)

* Enduser has Docker installed and running on the CentOS VM as directed by one of the following guides:

  * [Host Local - Ansible Setup Guide](host_local_setup_ansible.md)
  * [Host Local - CentOS Setup Guide](host_local_setup_centos.md)
  * [Host Local -  MacOS Setup Guide](host_local_setup_macos.md)
  * [Host Local - Ubuntu Setup Guide](host_local_setup_ubuntu.md)

* The four ISLE images that already been "built" and are stored in a cloud repository (Dockerhub) which save the enduser hours of build time are currently tagged with `latest`.

  * isle-apache [https://hub.docker.com/r/islandoracollabgroup/isle-apache/](https://hub.docker.com/r/islandoracollabgroup/isle-apache/)  

  * isle-fedora [https://hub.docker.com/r/islandoracollabgroup/isle-fedora/](https://hub.docker.com/r/islandoracollabgroup/isle-fedora/)  

  * isle-mysql [https://hub.docker.com/r/islandoracollabgroup/isle-mysql/](https://hub.docker.com/r/islandoracollabgroup/isle-mysql/)  

  * isle-solr  [https://hub.docker.com/r/islandoracollabgroup/isle-solr/](https://hub.docker.com/r/islandoracollabgroup/isle-solr/)  

* The enduser has sshed (shelled in) to their local Host VM or server using a terminal on their laptop or workstation.

---

### 0. docker-compose.yml file edits

If the enduser is using Vagrant on their laptop move on to **Edit 2** in this section as this process is handled automatically.

If and only if the enduser is **NOT using Vagrant** on their local laptop and is setting up Virtualbox manually, then please do the following below first on the local laptop.

**Please note:** By default the example tool given below of `nano` is not installed on CentOS. If the enduser hasn't installed it before on the CentOS VM, then please run `sudo yum install nano`.

**Edit 1 (conditional)**

If the enduser is not using Vagrant to create their CentOS VM, then during the manual process of installing CentOS (not given as a guide in ISLE documentation) they will need to figure out what the IP is of the VM in order to do the following:

* Add the values of `<CentosVM IP here> isle.localdomain apache solr mysql fedora` to the laptop / workstation's `/etc/hosts` file.   

  * Open a terminal on the local laptop

  * Enter: `sudo nano /etc/hosts`
    * _For endusers familiar with editing files on the command line, vim or alternative tools can be used in lieu of nano_

  * Enter the laptop enduser password

  * Add the values below the `127.0.0.1` entry in the `/etc/hosts` file.

    * `<CentosVM IP here> isle.localdomain apache solr mysql fedora` with a space in between the entries.  

  **Example** (_Please do not literally use the IP given below, it may be different after the manual CentOS VM creation_)

```
127.0.0.1 localhost
192.168.10.10 isle.localdomain apache solr mysql fedora`
```

  * Enter `Cntrl` and the letter `o` together to write the changes to the file.

  * Enter `Cntrl` and the letter `x` together to exit the file.

---

**Edit 2**  

This edit should be made regardless of using Vagrant etc. (same for all operating systems)

* Open up a terminal and ssh into the running ISLE Host VM

* `cd /opt/ISLE` _or wherever the enduser cloned the ISLE Project to on the ISLE Host VM_

*  Enter: `sudo nano /opt/ISLE/docker-compose.yml`
    * _For endusers familiar with editing files on the command line, vim or alternative tools can be used in lieu of nano_

* Add the ISLE Host VM IP to the `extra_hosts` sections on Lines 39 & 68 in the `docker-compose.yml` file at the root of the ISLE project directory.

* Lines 39 & 68 both require the same value of the Host server IP address for the `extra_hosts` setting after the `isle.localdomain:` and before the double quotes.

**Example for Vagrant users:**

```
   extra_hosts:
     - "isle.localdomain:10.10.10.130"
```

**Example for Non-Vagrant users:**

```
   extra_hosts:
     - "isle.localdomain:<CentosVM IP here>"
```

---

### Test site install process (same for all operating systems)

The steps below are for both Vagrant and non Vagrant users alike.

* **Please note:** *The first container (MySQL, isle-mysql, mysql) has to be running PRIOR to all others (including fedora & apache) due to a race condition (fedora starts prior to mysql being ready to accept connections). This improper state will be fixed at a later point in the project.*  

* **DO NOT RUN** `docker-compose up -d` during the initial install process as this will download all images and then run all containers at the same time which will trigger this race condition and subsequent chain of service failures.

* **During the initial install process** do the following steps to individually download each image and run each container.

* The install times stated below for each container are highly dependent on the enduser's available Internet bandwidth and could take more or less time accordingly.

* The steps below assume the enduser is still shelled in (sshed) into the ISLE Host VM via a terminal on their local laptop.

#### 1. MySQL image pull & container launch (10 - 20 mins)

* `docker pull islandoracollabgroup/isle-mysql:latest`  
* `docker-compose up -d mysql`  

#### 2. Fedora image pull & container launch (10 - 30 mins)  

* `docker pull islandoracollabgroup/isle-fedora:latest`  
* `docker-compose up -d fedora`  

* **(Optional but recommended)**

Please note the Tomcat service requires about 1 -3 minutes to startup and as such if the enduser rushes to the URL supplied below, the service page maytime out or be reported as unreachable. Give it a little time.

  * After spinning up fedora container, check if the Fedora service is running prior to advancing.
  * Navigate to http://fedora:8080/manager/html a popup login prompt should appear.
    * Enter the user name of `admin` and the password of `ild_tc_adm_2018`
      * Also found at the bottom of this page in the **Fast Facts** section, see `2. Fedora container - service Tomcat`
  * Upon login a large display of running Tomcat applications should display, scroll down to `fedora`
  * The application state / status should be `true`
  * If `false` appears instead, attempt to restart the fedora service manually.
    * Select the `restart` button to the right of the status area.
  * If it still fails, review the mounted fedora logs. The `docker-compose.yml` file will indicate where the logs are located.
    * Using terminal and then entering a command like `tail -n 300 - <path to ISLE project/data/fedora/log/tomcat:/usr/local/tomcat/logs/fedora.log` should display enough information to troubleshoot or restart the entire startup process.


#### 3. Solr image pull & container launch (10 - 20 mins)  

* `docker pull islandoracollabgroup/isle-solr:latest`  

* `docker-compose up -d solr`  

#### 4. Apache image pull & container launch (10 - 30 mins)

* `docker pull islandoracollabgroup/isle-apache:latest`  

* `docker-compose up -d apache`  

* **Please note:** *This container on occasion during project testing has failed to start initially for as of yet obvious and fully repeatable reasons.*  
    * One can check if the container is running: `docker ps` (shows only running containers)  
    * One can check if the container stopped running or "exited": `docker ps -a` (shows all containers running or not)  

#### 5. Install script on Apache container (10 - 20 mins)

* Run the following shell scripts manually on the apache container  
    * `docker exec -it isle-apache bash`
    * `cd /tmp/isle_drupal_build_tools/`
    * `chmod 777 *.sh`
    * `./apache_provision.sh`

* Once finished press the `Cntrl` and `d` keys or type `exit` to get out of the apache container
* Test (QC) the resulting setup by opening a web browser to the `isle.localdomain` URL of the new ISLE sample site (i.e. http://isle.localdomain)

**Please note:** The cronjob setting in the `apache_provision.sh` script is still commented out as this will need to be flowed into the Docker build process prior. Issue with default Docker root user vs using islandora user. Drupal cron can run properly.

**Total build process** takes 50 - 120 minutes (_depending on system and internet speeds_)

___

### Fast Facts and Passwords

`islandora` user on the ISLE host server uses `islandora` as the password.

**Please note:**

Whenever the value `http://hostip` used for accessing other ISLE services (not the main Islandora site) appears below, the enduser can replace `hostip` with one of the following:

When in doubt use the first!

1. `isle.localdomain` e.g `http://isle.localdomain.`

2. the IP of the Vagrant Host VM (CentOS) e.g. `http://10.10.10.130`

3. the IP of the Non-Vagrant Host VM (CentOS) e.g. `http://<CentosVM IP here>`

---

#### 1. MySQL container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| mysql                | isle-mysql      | MySQL 5.6     | 3306          |


| Account        | Password              | Database         | Perms                         |
| -------------  | -------------         | -------------    | -------------                 |      
| root           | ild_mysqlrt_2018      | **ALL**          | **ALL**                       |
| fedora_admin   | ild_feddb_2018        | fedora3          | **All** except `Grant` option |
| isle_ld_user   | isle_ld_db2018        | isle_ld          | **All** except `Grant` option |

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
| (DG) Islandora Transforms (XSLTs)| as of 1/2018      |
| Tomcat                           | 7.x               |  
| Oracle JDK                       | 8.x               |
| Djatoka                          | 1.1               |

| Account           | Password                      | Service       | URL           |
| -------------     | -------------                 | ------------- | ------------- |      
| fedoraAdmin       | ild_fed_admin_2018            | Fedora        | http://hostip:8080/fedora/describe                          |
| fedoraIntCallUser | ild_fed_IntCallUser_2018      | Fedora        | http://hostip:8080/fedora/objects                           |
| anonymous         | anonymous                     | Fedora        | ---                                                            |
| fgsAdmin          | ild_fgs_admin_2018            | Gsearch       | http://hostip:8080/fedoragsearch/rest?operation=updateIndex |
| admin             | ild_tc_adm_2018               | Tomcat        | http://hostip:8080/manager/html                             |
| manager           | ild_tc_man_2018               | Tomcat        | http://hostip:8080/manager/html                             |
| --                | --                            | Djatoka       | http://hostip:8080/adore-djatoka/                           |
| --                | --                            | Apache        | http://hostip:8777                                          |

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

| Account           | Password        | Service       | URL                                |
| -------------     | -------------   | ------------- | -------------                      |
| admin             | ild_tc_adm_2018 | Tomcat        | http://hostip:8091/manager/html |
| manager           | ild_tc_man_2018 | Tomcat        | http://hostip:8091/manager/html |
| --                | --              | Solr          | http://hostip:8091/solr/        |


___

#### 4. Apache container

| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| apache               | isle-apache        | see below  | 80            |


| Software      | Version       |
| ------------- | ------------- |
| Apache        | 2.4.7         |
| Oracle JDK    | 8.x           |
| Djatoka       | 1.1           |
| Drupal        | 7.56          |
| PHP           | 5.6           |
| Islandora     | 7.x           |

| Account                | Password                      | Service               | URL                     |
| -------------          | -------------                 | -------------         | -------------           |
| isle_localdomain_admin | isle_localdomain_adminpw2018  | Drupal site admin     | http://isle.localdomain |
