### ISLE Migration Guide (draft 02.5.2018)

This Migration guide is the intended process for endusers to migrate their existing production Islandora environment to their respective ISLE Islandora containers and volumes.


### Assumptions / Prerequisites
* ISLE Host server setup has been completed.

     * _If one has not setup the ISLE host server please refer to the appropriate resource below:_

        * [Host Remote Setup Guide - CentOS](host_remote_setup_centos.md)

        * [Host Remote Setup Guide - Ubuntu](host_remote_setup_ubuntu.md)

        * [Host Remote Setup Guide - Ansible](host_remote_setup_ansible.md) (setup for both Ubuntu & Centos)

* The fully qualified domain name of their ISLE host server that will run all of the containers has been created and resolves properly in DNS.

* The enduser has the IP address of their ISLE host server documented

* The enduser has the expected fully qualified domain name(s) (fqdn) of the Islandora / Drupal website(s) documented and assigned to the appropriate IP(s) in DNS.

* The enduser has ssh access to ISLE host server

* The enduser has the ability or access to create git repositories

* The enduser (or an appropriate IT resource) can **COPY** data, configuration files etc. safely from their Islandora Production server(s) to the new ISLE host server.

* Production data, configuration files etc. have been **copied** from the currently running Islandora production server to the new ISLE host server following the checklist below of required data, configuration files etc.

---

### Migration to ISLE Process Overview

As this is a large guide, here's a quick not very detailed overview of what's going to happen in the next steps:

* Ensure that the destination ISLE host server has the same (or more) amount of storage as the production server.
* Create appropriate Islandora Production data storage structure on new ISLE host server
* Copy current production data and config files as directed by the [export checklist](migration_export_checklist.md) to an appropriate location on the new ISLE host Server.
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
* Repeat entire process (_if necessary_) for additional ISLE environments e.g. _production, staging and development_

---

### Step 1: Create appropriate Islandora Production data storage structure on new ISLE host server

_Friendly note to endusers: While the following process may seem overly cautious or redundant, it saves time and establishes a safer conditions for endusers to work with valuable data._

It is recommended that endusers use a large volume or network attached drive that can store a backup copy of the entire Islandora production storage, an merged copy of the ISLE production storage and associated config files as outlined in the [Migration Export Checklist](migration_export_checklist.md) and additional datastores.

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

#### Step 2: Create appropriate ISLE Production data storage structure on new ISLE host server

_Friendly note to endusers: While the following process may seem overly cautious or redundant, it saves time and establishes a safer conditions for endusers to continue to work with valuable data._

In the previous step the `islandora_production_data_storage` directory was setup as a source of materials to continue to **COPY** from i.e. **Islandora Production Data**.

This step is to now create the working directory for **ISLE Production Data**.

It is recommended that endusers again use a large volume or network attached drive that can store the entire ISLE production storage (which is the merged copy of the Islandora production storage and associated config files as outlined in the [Migration Export Checklist](migration_export_checklist.md) and additional datastores.)

* In an appropriate area / path on one's intended ISLE host server e.g. `/opt/` or `/mnt/`, create a parent directory e.g. `isle_production_data_storage` with the appropriate institutional ISLE environment as a sub-directory. Create as many as required. For this guide only the production environment will be created e.g. `enduser-renamed-directory-prod.institution`

  * **Example guide directory structure**:
```
    /path_to/isle_production_data_storage/
             └── enduser-renamed-directory-prod.institution
                 ├── apache  
                 ├── fedora   
                 ├── mysql  
                 └── solr  
```

* While the `/path_to/isle_production_data_storage/` directory will serve as the storage for all ISLE production data, it is highly important to further differentiate future ISLE production data by environment e.g. hence the use of `enduser-renamed-directory-prod.institution`.

This structure ensures no accidental data overwrites between ISLE environments and proper functioning.

* **Example future directory structure**:
```
  /path_to/isle_production_data_storage/
           |
           ├──enduser-renamed-directory-dev.institution
           |  ├── apache  
           |  ├── fedora   
           |  ├── mysql  
           |  └── solr  
           |
           ├──enduser-renamed-directory-prod.institution
           |  ├── apache  
           |  ├── fedora   
           |  ├── mysql  
           |  └── solr  
           |
           ├──enduser-renamed-directory-stage.institution
           |  ├── apache  
           |  ├── fedora   
           |  ├── mysql  
           |  └── solr     
           |
```

* Later steps outlined in this guide will have the enduser copy data to these directories.

* The `docker-compose.yml` file found within `/opt/ISLE/config/enduser-renamed-directory-prod.institution` is ready for editing to "point" to these data directories. This process will be explained in further detail in Step 6 of this guide.

* Once the migration process is confirmed as completed and successful, the enduser will continue to backup up this data.

### Step 3: Copy Production Data to ISLE Host server

Please review and follow the [Migration Export Checklist](migration_export_checklist.md) to ensure all production data detailed within has been **COPIED** over to the ISLE Host Server **PRIOR** to proceeding further with this Migration guide.

Once all steps have been followed, continue on to Step 4.

---

### Step 4: Setup config directory within ISLE Git repo for institutional Docker configuration

This process is necessary for running multiple versions of ISLE e.g. production, staging and development / sandbox. The config folder is the location for creating multiple versions of ISLE environments such as the example provided below.

This example below displays, along with the baked in ISLE sample dev site, an institution running a development, staging and production environment all on one ISLE host server. This would be a recommended "typical" setup for long term usage.
```
ISLE/config/  
├──enduser-renamed-directory-dev.institution      
├──enduser-renamed-directory-prod.institution    (this guide uses this as an example only)
├──enduser-renamed-directory-stage.institution
├──isle_localdomain                              (ISLE sample site)   
└──isle-prod-project.institution                 (only copy this template / never delete or modify)
```

For a more detailed explanation please refer to section `2.7 Managing Multiple Environments.`

**Please note:** While one over time can maintain and update profiles in the `config` directory manually, it is highly recommended that this new directory be kept in a private git repository for ease of use in making and preserving changes. For more information on how to use git in an ISLE multi environment setup, please refer to section [2.7 Environments Git Structure](../../2_enduser_guide/2_7_managing_multiple_environments/env-git-structure.md)

**Setup process**  

* Create a private Git repository on their associated Git server (`private institutional Git repository`) or git hosting service (`Github.com, Bitbucket.com, Gitlab.com`)  
   * The enduser may need to add the `/home/islandora/.ssh/id_rsa.pub` to the appropriate git repository as a git ssh deploy key to be able to push pull from the server.

* Navigate to the ISLE directory `/opt/ISLE/config/``

* Create a directory on the ISLE server directory e.g. `/opt/ISLE/config/enduser-renamed-directory-prod.institution`

* Copy the contents of the `/opt/ISLE/config/isle-prod-project.institution` to this new directory e.g. `/opt/ISLE/config/enduser-renamed-directory-prod.institution`

---

### Step 5: Edit, merge or copy in Islandora production files or data to the new ISLE Production config or data directories

* Compare the data and settings of the files found within `islandora_production_data_storage`, and then merge, edit or copy as necessary with the templated settings found within the newly renamed directory of `/opt/ISLE/config/enduser-renamed-directory-prod.institution` as guided in the [Migration Merge Checklist](migration_merge_checklist.md).

---

### Step 6: Edit the `docker-compose.yml` file

Edit the `docker-compose.yml` found within the `/opt/ISLE/config/enduser-renamed-directory-prod.institution` directory as suggested in the [Migration Docker Compose Edit Guidelines](migration_docker_compose.md).

---

### Step 7: Review or pull down ISLE Docker images

_Please Note: You may have already done this in setting up the host server manually and / or with Ansible. However it is always a good idea to review and check using the first command below._

* Check if all ISLE images have been downloaded
  * `docker image ls`

```
  * **TO DO:**  show sample output here
```

  * If yes, then proceed to Step 7

  * If no, the perform the following:
    * `docker pull islandoracollabgroup/isle-mysql:alpha2`
    * `docker pull islandoracollabgroup/isle-fedora:alpha2`
    * `docker pull islandoracollabgroup/isle-solr:alpha2`
    * `docker pull islandoracollabgroup/isle-apache:alpha2`

---

### Step 8: Spin up mysql container and import production databases

* `cd /opt/ISLE/config/enduser-renamed-directory-prod.institution`
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

---

### Step 9: Spin up fedora container and run reindex processes
```
**TO DO:**  refine this
* Staying within `/opt/ISLE/config/enduser-renamed-directory-prod.institution`
* `docker-compose up -d fedora`
* check if fedora is running properly e.g. `http://isle-prod-project.institution:8080/manager/html`
* `docker exec -it isle-fedora-institution bash`
* turn off tomcat `service tomcat stop`
* reindex Fedora RI `process steps here`
* reindex SQL RI `process steps here`
* confirm PID contents in SQL table `QC process here`
```

---

### Step 10: Spin up solr container and rerun index processes
```
**TO DO:**  refine this
* Staying within `/opt/ISLE/config/enduser-renamed-directory-prod.institution`
* `docker-compose up -d solr`
* check if solr is running properly e.g. `http://isle-prod-project.institution:8777/manager/html`
* `docker exec -it isle-fedora-institution bash` NOTE FEDORA NOT SOLR
* reindex SOLR `process steps here` Use screen
* TAKES HOURS DEPENDING ON DATA SIZE
```

---

### Step 11: Spin up apache container and run provision script
```
**TO DO:**  refine this
* Staying within `/opt/ISLE/config/enduser-renamed-directory-prod.institution`
* `docker-compose up -d apache`
* check if apache is running properly e.g. `http://isle-prod-project.institution`
* `docker exec -it isle-apache-institution bash`
* cd /tmp location
* `/tmp/isle-build-tools/apache-provision.sh` (check if this is appropriate)
* Check site and outline QC process
```
