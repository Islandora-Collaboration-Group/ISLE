### ISLE Alpha Migration Guide (draft 01.25.2018 v1)

This Alpha Migration guide is intended the process for endusers to migrate an existing production Islandora environment to their respective Islandora containers and volumes.


### Assumptions / Prerequisites
* ISLE Host server setup has been completed
  * If one has not setup the ISLE host server please refer to here:

     * Manual Host Server setup for [**CentOS 7**](host_server_setup_centos.md) w/ ISLE repo clone steps

     * Manual Host Server setup for [**Ubuntu 16.04 LTS**](host_server_setup_ubuntu.md) w/ ISLE repo clone steps   

     * [Ansible install](host_server_setup_ansible.md) setup for (Ubuntu / Centos) includes ISLE repo clone steps

```
**TO DO:**  Separate page please
      add section to Manual pages for generating a ssh key for the `islandora` user.  
       Ansible handles this but missing from the three manual pages above.  
       * open up a terminal / cli prompt on the ISLE host server and enter the commands  
       * `mkdir /home/islandora/.ssh`  
       * `chmod -Rv 700 /home/islandora/.ssh`  
       * `cd /home/islandora/.ssh`  
       * `ssh-keygen`  
       * Follow the prompts **TO DO:**  what are the prompts?  
```

* The fully qualified domain name of their ISLE host server that will run all of the containers has been created and resolves properly in DNS.

* The enduser has the IP address of their ISLE host server documented

* The enduser has the expected fully qualified domain name (fqdn) of the new first Islandora / Drupal website documented and assigned to the appropriate IP in DNS.

    * _Please Note: this IP address and fqdn are NOT the of current running Production Islandora site!_


* The enduser has ssh access to ISLE host server

* The enduser has the ability or access to create git repositories

* The enduser (or an appropriate IT resource) can **COPY** data, configuration files etc. safely from their Islandora Production server(s) to the new ISLE host server.

* Production data, configuration files etc. have been **copied** from the currently running Islandora production server to the new ISLE host server following the checklist below of required data, configuration files etc.

  * See section **Production Copy / Export Checklist** below

```
**TO DO:** Review Ben's outlines from https://github.com/Islandora-Collaboration-Group/ISLE/issues/80
```

### Migration Process Explained

* Here's what's going to happen / why?
* Copying from prod location to prod location on ISLE host Server
* Blah blah about mering into config diff solr etc

* repeat entire process (if necessary) for additional ISLE platform e.g. production, staging and development



### Production Copy / Export Checklist
Checklist of materials to **COPY** from the current running institutional Islandora Production server's to the appropriate storage location / directory on the new ISLE Host Server.

**Please Note:** _Ubuntu / Debian style paths are used for for all example file locations, endusers might have different locations for these files HOWEVER the names etc should be roughly the same._

```
**QUESTION:** IS THIS STEP 2? (this feels like a separate page)
```

**Apache**

* `html` (_Entire directory unless size prohibits_)

* `settings.php` (_if running multi-sites separate or rename appropriately e.g. multisite2_name_settings.php, multisite3_name_settings.php and so on..._)

* `php.ini`

* `yoursite_apache_vhost.conf` (_file will have different name_)

**Fedora**

* `yoursite_fedora_apache_vhost.conf` (_if you use Adore-Djatoka with a reverse proxy otherwise it is possible this file is not necessary_)

* the entire `data` directory from the `/usr/local/fedora`

* `fedora.fcfg`

* `fedora-users.xml`

* `filter-drupal.xml`


**Gsearch**
* `fedoragsearch.properties`

* `fgsconfig-basic-configForIslandora.properties`

* `fgsconfigObjects.properties`

* `repository.properties`

* Entire `islandora_transforms` directory and contents

* `foxmlToSolr.xslt`


**MySQL**

* All production databases exported e.g. `drupal_site_2018.sql.gz`

* `my.cnf`

**Tomcat**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/tomcat`

  * /var/lib/tomcat7/conf/`server.xml`

  * /var/lib/tomcat7/conf/`tomcat-users.xml`

**Solr**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/solr`

  * /var/lib/tomcat7/webapps/solr/collection1/conf/`schema.xml`

  * /var/lib/tomcat7/webapps/solr/collection1/conf/`solrconfig.xml`

  * /var/lib/tomcat7/webapps/solr/collection1/conf/`stopwords.txt`


### Step 1: Create appropriate Islandora Production data storage structure on new ISLE host server

_Friendly note to user: this may seem redudnadt . Trust me_ saves time and workflow plus if data is alreted. Allowes

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

### Step 2: Create appropriate ISLE Production data storage structure on new ISLE host server

* In an appropriate area / path on one's intended ISLE host server e.g. `/opt/` or `/mnt/`, create a directory e.g. `isle_production_data_storage` with no additional sub-directories:

---

### Step 3: **COPY** in all of the following `highlighted` directories or files from the appropriate Islandora Production server(s) to the appropriate sub-directory on the ISLE host server as outlined above.  

```
**QUESTION**: IS THIS THE CHECKLIST? (this feels like a separate page)
```

**Apache**

Copy the following below to `/pathto/islandora_production_data_storage/apache`

  * /var/www/`html` (_Entire contents unless size prohibits_)
    * (_If `html` is not used, then substitute with the appropriate directory for the Islandora / Drupal site_)


  * /var/www/html/sites/default/`settings.php` (_if running multi-sites separate or rename appropriately e.g. multisite2_name_settings.php, multisite3_name_settings.php and so on..._)

  * /etc/`php.ini`

  * /etc/apache2/sites-available/`yoursite.conf`

**Fedora**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/fedora/apache`

  * /etc/apache2/sites-available/`yourfedorasite.conf` (_if you use Adore-Djatoka with a reverse proxy otherwise it is possible this file is not necessary_)


* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/fedora/fedora`

  * /usr/local/fedora/server/config/`fedora.fcfg`

  * /usr/local/fedora/data/fedora-xacml-policies/`repository-policies`

  * /usr/local/fedora/server/config/`fedora-users.xml`

  * /usr/local/fedora/server/config/`filter-drupal.xml`


**Gsearch**  

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/fedora/gsearch`

    * /var/lib/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/`fedoragsearch.properties`

    * /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/`fgsconfig-basic-configForIslandora.properties`

    * /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/`fgsconfigObjects.properties`

    * /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/repository/FgsRepos/`repository.properties`

    * /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/`islandora_transforms`

    * /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/`foxmlToSolr.xslt`


**MySQL**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/mysql`

  * copies of all production databases exported in a similar format to this `drupal_site_2018-01-23.sql.gz`

    * **RECOMMENDATION**
      * Run `drush cc all` on the production sites PRIOR to db export(s)

        * Otherwise issues can occur on import due to all cache tables being larger than `innodb_log_file_size` allows  

        * _Alternative longer method_
          * one can skip running the drush command on the production apache webserver
          * export the databases as usual from the production mysql server
          * import databases into the isle-mysql container (_with errors being ignored_)
          * truncate all tables that start with `cache` on the isle-mysql container
          * export this new database to the `mysql` directory on the isle host server
          * delete all tables (_not the database itself_) on the isle-mysql container
          * Reimport the new lighter database to the isle-mysql container

  * /etc/`my.cnf`

**Tomcat**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/tomcat`

  * /var/lib/tomcat7/conf/`server.xml`

  * /var/lib/tomcat7/conf/`tomcat-users.xml`

**Solr**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/solr`

    * /var/lib/tomcat7/webapps/solr/collection1/conf/`schema.xml`

    * /var/lib/tomcat7/webapps/solr/collection1/conf/`solrconfig.xml`

    * /var/lib/tomcat7/webapps/solr/collection1/conf/`stopwords.txt`

---

### Step 4: Setup Git repo for institutional Docker configuration

_Please Note_ Because  config folder defines the entire platform setup eseentially keeping the sample

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

* Copy the contents of the `/opt/ISLE/config/production_template/` to this new directory e.g. `/opt/ISLE/config/isle-prod-project.institution`

* The enduser will need to add the `/home/islandora/.ssh/id_rsa.pub` as a git ssh deploy key to be able to push pull from the server.

---

### Step 5: Copy in production files on the Isle Host Server to the new institutional Docker config directory within the ISLE project directory

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

### Step 6: Edit the `docker-compose.yml` file

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

DONE
