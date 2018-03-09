## Development Site Guide - isle.localdomain


_Expectations: It takes an average of **15-45 minutes** to read this entire document and perform the installation as prescribed._

Follow this guide to spin up and install ISLE with no URL utilizing the built-in `isle-localdomain` name for review and testing.

This test ISLE / Islandora environment (`isle.localdomain`) includes an un-themed Drupal website and empty Fedora repository for endusers to develop code, test ingests, test metadata, update fields in SOLR indexing and otherwise "kick the tires" on ISLE prior for further usages e.g. creating a new ISLE production site or migrating a current Islandora production site to ISLE.


### Assumptions / Prerequisites

* Laptop / workstation that conforms to the specifications outlined in the [System Requirements](../System_Requirements.md)

* Instructions below assume a MacOS or Linux laptop or workstation. Windows users may have to swap out various tools as needed.

* This test site guide is designed for a local laptop / workstation running one of the following:

     * using the supplied Vagrant setup found in the `vagrant` directory at the root of the ISLE project (_Vagrant with Virtualbox_)
     * a manually setup CentOS or Ubuntu Virtualbox VM (_no Vagrant_)
        * Docker installed and running on the VM as directed by one of the host server setup pages.
        * Can connect to the VM by SSH using a terminal on their laptop or workstation.
     * [Docker for Mac](https://www.docker.com/docker-mac)

---

### Edit /etc/localhost

#### Edit 1: Vagrant only

* **Vagrant** If you are using Vagrant on a laptop move on to the next section (_Summary: Test site launch process_) as this part is handled automatically.

---

#### Edit 1: VM only

* **VM** If you are using a local Virtualbox VM (CentOS / Ubuntu) but not Vagrant to host ISLE, one will need to get the VM IP address prior to attempting the  steps below.

*Please note:** By default the example tool given below of `nano` may not be installed on the VM.
  * If the enduser hasn't installed it before on a CentOS VM, then please run `sudo yum install nano`.
  * If the enduser hasn't installed it before on a Ubuntu VM, then please run `sudo apt-get install nano`.
  * If you are familiar with other command-line tools then you are welcome to substitute.

* Add the values of `<VM IP here> isle.localdomain apache solr mysql fedora` to the laptop / workstation's `/etc/hosts` file.   

* Open a terminal on the local laptop

* Enter: `sudo nano /etc/hosts`
  * _For endusers familiar with editing files on the command line, vim or alternative tools can be used in lieu of nano_

* Enter the laptop enduser password

* Add the values below the `127.0.0.1` entry in the `/etc/hosts` file.

* `<VM IP here> isle.localdomain` with a space in between the entries.  

**Example** (_Please do not literally use the IP given below, it may be different after the manual CentOS VM creation_)

```
  127.0.0.1 localhost
  192.168.10.10 isle.localdomain`
```

* Enter `Cntrl` and the letter `o` together to write the changes to the file.
* Enter `Cntrl` and the letter `x` together to exit the file.

---

#### Edit 1: Docker for Mac only

* **Docker For Mac** If you are using Docker For Mac, then use the IP address of `127.0.0.1`

* Open a terminal on the local laptop

* Enter: `sudo nano /etc/hosts`
  * _For endusers familiar with editing files on the command line, vim or alternative tools can be used in lieu of nano_

* Enter the laptop enduser password

* Add the values next to the `127.0.0.1  localhost` entry in the `/etc/hosts` file.

    * `127.0.0.1 localhost isle.localdomain` with a space in between the entries.  

  * Enter `Cntrl` and the letter `o` together to write the changes to the file.

  * Enter `Cntrl` and the letter `x` together to exit the file.

---

### Summary: Test site launch process

* If using Vagrant or a VM, the steps below assume you are still shelled in (connected by SSH) into the ISLE Host VM via a terminal on a local laptop.

* If using Docker for Mac, continue to use the open terminal and navigate (`cd`) to the ISLE project directory.

The steps below are for all users (_Vagrant, VM, non Vagrant and Docker for Mac users alike._)

The install times stated below for each container are highly dependent on the enduser's available Internet bandwidth and could take more or less time accordingly.

* run `docker-compose up -d` - this is going to download and start all five images (roughly 600GB of data so it may take a little while depending on your connection)

   * Check if the containers are running: `docker ps` (shows only running containers)

   * If you don't see all five containers try: `docker ps -a` (shows all containers running or not)

**If all five containers are NOT running, then proceed to the Troubleshooting section first before advancing to the "Install script on Apache container" section below**

-----

#### Install script on Apache container

* Run the following shell scripts manually on the apache container  
    * `docker exec -it isle-apache bash`
    * `cd /tmp/isle_drupal_build_tools/`
    * `chmod 777 *.sh`
    * `./install_isle_ld_site.sh`

* Once finished press the `Cntrl` and `d` keys or type `exit` to get out of the apache container
* Test (QC) the resulting setup by opening a web browser to the `isle.localdomain` URL of the new ISLE sample site (i.e. http://isle.localdomain)

**Please note:** The cronjob setting in the `apache_provision.sh` script is still commented out as this will need to be flowed into the Docker build process prior. Issue with default Docker root user vs using islandora user. Drupal cron can run properly.

**Total build process** takes 50 - 120 minutes (_depending on system and internet speeds_)


### Troubleshooting

If you don't see five running containers, then stop the running containers with `docker-compose down -d` and start the containers one at a time following the instructions below:

*  MySQL image pull & container launch

    `docker pull islandoracollabgroup/isle-mysql:latest`

    `docker-compose up -d mysql`

*  Fedora image pull & container launch

    `docker pull islandoracollabgroup/isle-fedora:latest`

    `docker-compose up -d fedora`

    (Optional but recommended troubleshooting step)

    * Please note the Tomcat service requires about  one to three minutes to startup and as such if the enduser rushes to the URL supplied below, the service page maytime out or be reported as unreachable. Give it a little time.
    * After spinning up fedora container, check if the Fedora service is running prior to advancing.
    * Navigate to http://fedora:8080/manager/html a popup login prompt should appear.
    * Enter the user name of `admin` and the password of `ild_tc_adm_2018`
    * Upon login a large display of running Tomcat applications should display, scroll down to fedora
    * The application state / status should be true
    * If false appears instead, attempt to restart the fedora service manually.
    * Select the restart button to the right of the status area.
    * If it still fails, review the mounted fedora logs. The docker-compose.yml file will indicate where the logs are located.
    * Using terminal and then entering a command like `tail -n 300 - <path to ISLE project/data/fedora/log/tomcat:/usr/local/tomcat/logs/fedora.log` should display enough information to troubleshoot or restart the entire startup process.

* Solr image pull & container launch

    `docker pull islandoracollabgroup/isle-solr:latest`

    `docker-compose up -d solr`

* Apache image pull & container launch

    `docker pull islandoracollabgroup/isle-apache:latest`

    `docker-compose up -d apache`

* Proxy image pull & container launch

    `docker pull islandoracollabgroup/isle-proxy:latest`

    `docker-compose up -d proxy`

-----

### Fast Facts

`islandora` user on the ISLE host server uses `islandora` as the password.

**Glossary**

* `hostip` can mean

1. the IP of the Vagrant Host VM (CentOS / Ubuntu ) e.g. `http://10.10.10.130`

2. the IP of the Non-Vagrant Host VM (CentOS / Ubuntu ) e.g. `http://<VM IP here>`

3. `127.0.0.1`

---

#### 1. MySQL container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| mysql                | isle-mysql-ld   | MySQL 5.6     | 3306          |


| Account        | Password              | Database         | Perms                         |
| -------------  | -------------         | -------------    | -------------                 |      
| root           | ild_mysqlrt_2018      | **ALL**          | **ALL**                       |
| fedora_admin   | ild_feddb_2018        | fedora3          | **All** except `Grant` option |
| isle_ld_user   | isle_ld_db2018        | isle_ld          | **All** except `Grant` option |

  ---

#### 2. Fedora container
| Compose Service Name | Container Name  | Software      | Ports                                            |
| :-------------:      | :-------------: | ------------- | -------------                                    |      
| fedora               | isle-fedora-ld  | see below     | 8080, 80 (on container) mapped to 8777 (on host) |


| Software                         | Version           |
| -------------                    | -------------     |
| Fedora                           | 3.8.1             |
| Apache                           | 2.4.7             |
| Drupalfilter                     | 3.8.1             |
| Gsearch (w/remote SOLR plugin)   | 2.8+ (DG patched) |
| (DG) GSearch Extensions          | 0.13              |
| (DG) Islandora Transforms (XSLTs)| as of 3/2018      |
| Tomcat                           | 7.x               |  
| Oracle JDK                       | 8.x               |
| Djatoka                          | 1.1               |

| Account           | Password                      | Service       | URL           |
| -------------     | -------------                 | ------------- | ------------- |      
| fedoraAdmin       | ild_fed_admin_2018            | Fedora        | http://hostip:8080/fedora/describe                          |
| fedoraIntCallUser | ild_fed_IntCallUser_2018      | Fedora        | http://hostip:8080/fedora/objects                           |
| anonymous         | anonymous                     | Fedora        | ---                                                         |
| fgsAdmin          | ild_fgs_admin_2018            | Gsearch       | http://hostip:8080/fedoragsearch/rest?operation=updateIndex |
| admin             | ild_tc_adm_2018               | Tomcat        | http://hostip:8080/manager/html                             |
| manager           | ild_tc_man_2018               | Tomcat        | http://hostip:8080/manager/html                             |
| --                | --                            | Djatoka       | http://hostip:8080/adore-djatoka/                           |

___

#### 3. Solr container

| Compose Service Name | Container Name  | Software      | Ports                                              |
| :-------------:      | :-------------: | ------------- | -------------                                      |      
| solr                 | isle-solr-ld    | see below     | 8080 (on container) mapped to 8091 (on host)       |


| Software               | Version       |
| -------------          | ------------- |
| Solr                   | 4.10.4        |
| Tomcat                 | 7.x           |  
| Oracle JDK             | 8.x           |
| (DG) Basic Solr Config | 4.10.x branch |

| Account           | Password        | Service       | URL                             |
| -------------     | -------------   | ------------- | -------------                   |
| admin             | ild_tc_adm_2018 | Tomcat        | http://hostip:8091/manager/html |
| manager           | ild_tc_man_2018 | Tomcat        | http://hostip:8091/manager/html |
| --                | --              | Solr          | http://hostip:8091/solr/        |


___

#### 4. Apache container

| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| apache               | isle-apache-ld  | see below     | 80, 443       |


| Software      | Version       |
| ------------- | ------------- |
| Apache        | 2.4.7         |
| Oracle JDK    | 8.x           |
| Djatoka       | 1.1           |
| Drupal        | 7.57          |
| PHP           | 5.6           |
| Islandora     | 7.x           |

| Account                | Password                      | Service               | URL                      |
| -------------          | -------------                 | -------------         | -------------            |
| isle_localdomain_admin | isle_localdomain_adminpw2018  | Drupal site admin     | https://isle.localdomain |


#### 5. Proxy container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |
| proxy                | isle-proxy      | Nginx 1.13    | 80 ,443       |
