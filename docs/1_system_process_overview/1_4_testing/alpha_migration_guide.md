### ISLE Alpha Migration Guide (draft 01.25.2018 v1)

This Alpha Migration guide is the intended process for endusers to migrate their existing production Islandora environment to their respective ISLE Islandora containers and volumes.


### Assumptions / Prerequisites
* ISLE Host server setup has been completed

  * If one has not setup the ISLE host server please refer to here:

     * Manual Host Server setup for [**CentOS 7**](host_server_setup_centos.md) w/ ISLE repo clone steps

     * Manual Host Server setup for [**Ubuntu 16.04 LTS**](host_server_setup_ubuntu.md) w/ ISLE repo clone steps   

     * [Ansible install](host_server_setup_ansible.md) setup for (Ubuntu / Centos) includes ISLE repo clone steps

* The fully qualified domain name of their ISLE host server that will run all of the containers has been created and resolves properly in DNS.

* The enduser has the IP address of their ISLE host server documented

* The enduser has the expected fully qualified domain name (fqdn) of the new first Islandora / Drupal website documented and assigned to the appropriate IP in DNS.

    * _Please Note: this IP address and fqdn are NOT the of current running Production Islandora site!_


* The enduser has ssh access to ISLE host server

* The enduser has the ability or access to create git repositories

* The enduser (or an appropriate IT resource) can **COPY** data, configuration files etc. safely from their Islandora Production server(s) to the new ISLE host server.

* Production data, configuration files etc. have been **copied** from the currently running Islandora production server to the new ISLE host server following the checklist below of required data, configuration files etc.

### Migration to ISLE Process Overview

As this is a large guide, here's a quick not very detailed overview of what's going to happen in the next steps:

* Ensure that the destination ISLE host server has the same (or more) amount of storage as the production server.
* Create appropriate Islandora Production data storage structure on new ISLE host server
* Copy current production data and config files as directed by the [export checklist](alpha_migration_export_checklist) to an appropriate location on the new ISLE host Server.
* Create the required new config directory by copying the template `/opt/ISLE/config/isle-prod-project.institution` to a renamed directory within `/opt/ISLE/config`
* Compare the template settings within the new renamed directory of `/opt/ISLE/config/enduser-renamed-directory.institution` to the current production config files. Merge or edit as necessary in the new isle config directory for use with ISLE.
   * There may be some additional work to compare and merge in previous enduser customizations of the Solr `schema.xml` to the new ISLE config.
   * There may be some additional work to compare and merge in previous enduser customizations of the Solr `solrconfig.xml` to the new ISLE config.
   * There may be some additional work to compare and merge in previous enduser customizations of the Solr `stopwords` to the new ISLE config.
   * There may be some additional work to compare and merge in previous enduser customizations of the Islandora Transform (XSLTs) files to the new ISLE versions.        
* Based on new paths, edit the `docker-compose.yml` file to
   * Point to the new associated `islandora_production_data_storage` structure.
   * Point to the new directories and config settings in `/opt/ISLE/config/enduser-renamed-directory.institution`
* Spin up new containers one at a time with the new config settings
* Check that all services are running properly
* Repeat entire process (_if necessary_) for additional ISLE platforms e.g. _production, staging and development_


### Step 1: Create appropriate Islandora Production data storage structure on new ISLE host server

_Friendly note to endusers: While the following process may seem overly cautious or redundant, it saves time and establishes a safer conditions for endusers to work with valuable data._

It is recommended that endusers use a large volume or network attached drive that can store a backup copy of the entire production storage, an merged copy of the production storage and associated config files as outlined in the **Migration Export Checklist** [page](alpha_migration_export_checklist.md) and additional datastores.

* In an appropriate area / path on one's intended ISLE host server e.g. `/opt/` or `/mnt/`, create a directory e.g. `islandora_production_data_storage` with the following sub-directories:

  * **Example directory structure**:
```
    islandora_production_data_storage/   
    ├── apache  
    ├── fedora   
    │   ├── apache  
    │   ├── fedora  
    │   └── gsearch  
    ├── mysql  
    ├── solr  
    └── tomcat  
```

* It is recommended that original copies of data be first copied here to allow for faster workflow as well as for endusers to easily sort through production data.

* This directory will serve as the "backup" of all production data about to be migrated into ISLE

* Once the migration process is confirmed as completed and successful, the enduser can decide if this data is still worth keeping or backed up.

---

### Step 2: **COPY** in all of the directories or files found on the **Migration Export Checklist** to the appropriate sub-directory on the ISLE host server as outlined above.  

Please review the **Migration Export Checklist** [page](alpha_migration_export_checklist.md) and ensure all production data detailed within has been **COPIED** over to the ISLE Host Server PRIOR to proceeding with this guide.


---

### Step 3: Setup Git repo for institutional Docker configuration

This process is necessary for running multiple versions of ISLE e.g. production, staging and development / sandbox. The config folder

* Create a private Git repository (Github.com, Bitbucket.com, Gitlab.com or private institutional Git repository)

* Navigate to the ISLE directory `/opt/ISLE/config/``

* Create a directory on the ISLE server directory e.g. `/opt/ISLE/config/isle-prod-project.institution`

* Instantiate this directory as a git repository
  * Directions for this process
    * [https://help.github.com/articles/set-up-git/](https://help.github.com/articles/set-up-git/)
    * [https://help.github.com/articles/create-a-repo/](https://help.github.com/articles/create-a-repo/)


```
**TO DO**: Add more git URLs non github.com
```

* Copy the contents of the `/opt/ISLE/config/isle-prod-project.institution` to this new directory e.g. `/opt/ISLE/config/isle-prod-project.institution`

* The enduser will need to add the `/home/islandora/.ssh/id_rsa.pub` as a git ssh deploy key to be able to push pull from the server.

---

### Step 4: Copy in production files on the Isle Host Server to the new institutional Docker config directory within the ISLE project directory

* Copy in the `highlighted` files from their locations `/pathto/islandora_production_data_storage/` to the appropriate directories of `/opt/ISLE/config/isle-prod-project.institution`

* **Resulting example structure**: `/opt/ISLE/config/isle-prod-project.institution`
```
isle-prod-project.institution/
├── apache
│   ├── php.ini
│   ├── settings.php
│   ├── site.conf
│   └── tmpreaper
│       └── cron
├── docker-compose.yml
├── fedora
│   ├── apache
│   │   └── site.conf
│   ├── fedora
│   │   ├── fedora-users.xml
│   │   ├── fedora.fcfg
│   │   ├── filter-drupal.xml
│   │   ├── log4j.properties
│   │   ├── logback.xml
│   │   └── repository-policies
│   │       ├── default
│   │       │   ├── deny-apim-if-not-localhost.xml
│   │       │   ├── deny-inactive-or-deleted-objects-or-datastreams-if-not-administrator.xml
│   │       │   ├── deny-reloadPolicies-if-not-localhost.xml
│   │       │   ├── deny-unallowed-file-resolution.xml
│   │       │   ├── permit-anything-to-administrator.xml
│   │       │   ├── permit-apia-unrestricted.xml
│   │       │   ├── permit-dsstate-check-unrestricted.xml
│   │       │   ├── permit-oai-unrestricted.xml
│   │       │   ├── permit-serverStatus-unrestricted.xml
│   │       │   └── readme.txt
│   │       └── islandora
│   │           ├── permit-apim-to-authenticated-user.xml
│   │           ├── permit-getDatastream-unrestricted.xml
│   │           ├── permit-getDatastreamHistory-unrestricted.xml
│   │           └── permit-upload-to-authenticated-user.xml
│   ├── gsearch
│   │   ├── fedoragsearch.properties
│   │   ├── fgsconfig-basic-configForIslandora.properties
│   │   ├── fgsconfigObjects.properties
│   │   └── repository.properties
│   ├── tmpreaper
│   │   └── cron
│   └── tomcat
│       ├── server.xml
│       ├── tomcat-users.xml
│       └── web.xml
├── mysql
│   ├── create_drupal_user.sql
│   ├── create_fedora_user.sql
│   ├── my.cnf
│   └── production_site_db.sql
└── solr
    ├── solr
    │   ├── schema.xml
    │   ├── solr.xml
    │   ├── solrconfig.xml
    │   └── stopwords.txt
    └── tomcat
        ├── server.xml
        ├── tomcat-users.xml
        └── web.xml
```

### Step 5: Edit the `docker-compose.yml` file

**Mysql Service section**
```
services:
  mysql:
```

* In the `image:` section,the image names to the appropriate images needed
    * _As of Jan 25, 2018 please use the alpha2 tag (this documentation will change)_


* In the `environment:` section, change the MYSQL_ROOT_PASSWORD

* In the `volumes:` section, change the following:
      - /pathto/isle_production_data_storage/data/mysql:/var/lib/mysql
      - /pathto/isle_production_data_storage/data/mysql/log:/var/log/mysql

    * **Please Note:** _The docker bind-mount process will automatically create the `/data/mysql` directories_

* In the `container_name:` section, change to `isle-mysql-institution` or `isle-mysql-v1`

* In the `extra_hosts:` section, change: (**this is NOT to be literally copied, example only**)
      - "isle.institution.edu:isle_host_server_IP"  to "yourislesite.institution.edu:10.10.10.10"

**Fedora Service section**
```
fedora:
```

```
**TO DO:**  remaining edits for fedora

**TO DO:**  solr service edits

TO DO : apache service edits

**TO DO:**  git commit changes & push to repo

**TO DO:**  git clone from the institutional git repo e.g. `ISLE-config` to the /opt/ISLE/config/ (on ISLE host server using terminal)

```

### Step 7: Review or Pull down ISLE Docker images

_Please Note: You may have already done this in setting up the host server manually and / or with Ansible. However it is always a good idea to review and check using the first command below._

* Check if all ISLE images have been downloaded
  * `docker image ls`

```
  * **TO DO:**  show sample output here
```

  * If yes, then proceed to Step 8

  * If no, the perform the following:
    * `docker pull islandoracollabgroup/isle-mysql:alpha2`
    * `docker pull islandoracollabgroup/isle-fedora:alpha2`
    * `docker pull islandoracollabgroup/isle-solr:alpha2`
    * `docker pull islandoracollabgroup/isle-apache:alpha2`

### Step 8: Spin up mysql container and import production databases

* `cd /opt/ISLE/config/isle-prod-project.institution`
* `docker-compose up -d mysql`

```
**TO DO:**  steps and scripts for mysql cli import
       use GUI and recommended clients e.g. Sequel pro, Navicat etc.
       if encountering errors use _Alternative longer method_ from above

       * _Alternative longer method_
         * one can skip running the drush command on the production apache webserver
         * export the databases as usual from the production mysql server
         * import databases into the isle-mysql container (_with errors being ignored_)
         * truncate all tables that start with `cache` on the isle-mysql container
         * export this new database to the `mysql` directory on the isle host server
         * delete all tables (_not the database itself_) on the isle-mysql container
         * Reimport the new lighter database to the isle-mysql container


```

### Step 9: Spin up fedora container and run reindex processes
```
**TO DO:**  refine this
* Staying within `/opt/ISLE/config/isle-prod-project.institution`
* `docker-compose up -d fedora`
* check if fedora is running properly e.g. `http://isle-prod-project.institution:8080/manager/html`
* `docker exec -it isle-fedora-institution bash`
* turn off tomcat `service tomcat stop`
* reindex Fedora RI `process steps here`
* reindex SQL RI `process steps here`
* confirm PID contents in SQL table `QC process here`
```

### Step 10: Spin up solr container and rerun index processes
```
**TO DO:**  refine this
* Staying within `/opt/ISLE/config/isle-prod-project.institution`
* `docker-compose up -d solr`
* check if solr is running properly e.g. `http://isle-prod-project.institution:8777/manager/html`
* `docker exec -it isle-fedora-institution bash` NOTE FEDORA NOT SOLR
* reindex SOLR `process steps here` Use screen
* TAKES HOURS DEPENDING ON DATA SIZE
```

### Step 11: Spin up apache container and run provision script
```
**TO DO:**  refine this
* Staying within `/opt/ISLE/config/isle-prod-project.institution`
* `docker-compose up -d apache`
* check if apache is running properly e.g. `http://isle-prod-project.institution`
* `docker exec -it isle-apache-institution bash`
* cd /tmp location
* `/tmp/isle-build-tools/apache-provision.sh` (check if this is appropriate)
* Check site and outline qC process
```
