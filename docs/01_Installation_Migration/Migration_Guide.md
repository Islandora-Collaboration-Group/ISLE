

##### Migrate an Existing Islandora Environment to ISLE

This Migration guide will help you migrate your existing production Islandora environment to utilize this ISLE framework for easily maintaining Islandora. This guide will walk you through how to identify and copy your institution's Islandora data and files (including your data volume, Drupal site or sites, and commonly customized xml and xslt files) to your ISLE framework.


Prerequisites:

1. Host Server set up according to the [instructions](../00Host_Server/Host_Server_Setup.md)

1. Have a domain name that works - is set up with DNS etc...

1. Have SSL Certificates for the domain

1. ISLE project has been cloned to BOTH your local laptop/workstation AND the host server - see [Setup Guide](../00Host_Server/Host_Server_Setup)

2. Disk space on or mounted to the Host Server large enough to store a **copy** of your fedora data store

2. Access to that server from your local workstation via SSH (`islandora` user with sudo privileges)

3. Access to the current Islandora production server

4. Usernames/Passwords for key parts of your stack which are used **for** the migration.
    * Drupal SQL information: username, password, database name can be obtained from your original `www/sites/default/settings.php`
    * Fedora SQL information: username, password, database name can be obtained from your original `fedora/server/config/fedora.fcfcg`
    * Fedora users: please have a copy of your `fedora-users.xml`
    * Tomcat users: please have a copy of your `tomcat-users.xml` OR use the default login: admin,ild_tc_adm_2018 (for both fedora and solr)

5. SQL dump (export) of the current production site's Drupal database


--------------------

## Overview of steps

*tl;dr* Copy old stuff over to host server w/ checklist, create new private repo for configs, merge old configs into new configs w/ checklist, edit docker-compose.yml to point to new configs, spin up containers, go into fedora container & reindex, qc site - Done.

- **COPY** the drupal/islandora site and configuration files from the running production Islandora (there's a checklist)

- create a new directory for the /config folders and files, copy the ISLE repository /config into it, and make this into a private repository

- following the steps in this document, edit the /config files so they have the proper site-specific information (there's a checklist for this too!)

-  **NOTE** some of this will involve fairly complicated merging of files to account for custom edits that may have been made to your production Islandora site and/or to bring configuration files up to work with newer software versions. This is by far the trickiest part of the migration and definitely a good place to seek help from the community if you get stuck.

- Edit the docker-compose.yml file to point to all these fine new config files.
- Download ISLE images from Dockerhub and spin up the ISLE Containers (takes a while depending on network speeds)
- go into the fedora container and do re-index (takes a little while)
- go to the new site and QC!

-------------------

## Detailed Steps

* Setup a Private Code Repository
      * Most of the work in this guide involves careful editing of the various configuration and settings files that customize the pieces of Islandora (database, repository, web-server, etc...).
      * Doing this work in a code repository makes it easier to correct errors and to repeat the process for additional servers without needing to replicate all the work.
      * Since the edits will include things like passwords, it's important to make this a private repository.

* Customizing for your Environment
      * Many of the steps below describe adding the domain name or other specific bits of information into files or appending those bits to file names.
      * In these cases this guide will call out the customization point AND provide an example - it's important not to literally copy paste the example!
      * Your best guide for these customizations are the source files from your currently running Islandora environment.

## Create Private Code Repository

**ON your local laptop/workstation:**

* On the repository of your choice (GitHub, GitLab, Bitbucket, etc.) create a PRIVATE remote git repo - see the specific code repository documentation online for setup instructions.

* Open a terminal - navigate to /opt/ISLE or where you put the ISLE directory on your local workstation.

* Create a directory named `yourdomain-config` (where "yourdomain" is your server domain name)

example:  `digital-collectionsconfig`

* Locate the directory called `config` and copy all the contents to the newly created directory

* `cd` into the newly copied and renamed yourdomainconfig directory and type:
 `git init` to initate this directory as a code repository.

* `git remote add NameOfYourRepository URLofYourRepository` to connect your local repository to the remote you set up in the above steps. [**NOTE** replace "NameOfYourRepository" and "URLofYourRepository" with the name of your repository and its URL]

* You are now ready to perform the customization edits in this directory (you can use a text editor of choice now don't have to stay in terminal - just locate the folder in the finder and open file in text editor)

-------

## Migration Export checklist

* following the checklist of materials, **COPY** from your current running institutional Islandora Production server(s) to your working directories on the local workstation.

* in the same /opt/ISLE directory create a new sub-directory (you can call this `current-production-config`)

* copy the files from your current live system into this directory as directed in the [Migration Export Checklist](migration_export_checklist.md)

* These copied files will be the source for edits and merges - just to be very clear, please don't work directly on the files in your currently running production system!

* To be even more clear, you should have two nearly identical `/config` directories, **A.** copied from the cloned ISLE repository and renamed with your domain, and **B.** copied from your currently running Islandora environment.

* **The goal is to merge all site-specific data (domain names, variables, usernames, passwords, etc..) AND all site-specific customizations or settings from the files in directory B. into the matching files in directory A.**


* Compare the data and settings of the files found within directory **B.** `current-production-config`, and then merge, edit or copy as necessary with the templated settings found within the directory **A.** `yourdomain-config` as guided in the [Migration Merge Checklist](migration_merge_checklist.md).

* Customizations: Carefully compare the following most frequently customized files with the new, default versions found within your new ISLE config folder. Use a "Diff" tool (example: [Beyond Compare](https://www.scootersoftware.com/download.php) to merge any desired customizations from your production Islandora files to persist within the new ISLE config folder of files:

   * Compare and merge the Solr files: `schema.xml`
   * Compare and merge the Solr files: `solrconfig.xml`
   * Compare and merge the Solr files: `stopwords`
   * Compare and merge the Fedora GSearch Islandora Transform (XSLTs) folder of files: `islandora_transforms`


   * Edit the `docker-compose.yml` file to:
   * Point to the new directories and config settings in `yourdomain-config`

---------



##Final steps

* Now that all the changes are made (be sure to save), ISLE should be ready to test. First you'll need to push these changes to your private code repository.

    * Open a terminal - `cd` to the config directory you've been making the changes in...

    * `git status`  this will show you all the files that have been modified and ready to be added to your private repo along with handy paths for the next steps.

    * `git add /pathtoyourmodifiedfile` (replace "pathtoyourmodifiedfile" with the path to your config directory)

    * run these:

    `git add apache`
    `git add fedora`
    `git add mysql`
    `git add proxy`
    `git add solr`
    `git add docker-compose.yml`

    * run `git status` again - everything should be in green now as all modified files have been added - if anything's still red use git add and the path to add it

    * then run `git commit -m "initial config commit"` inside the double quotes is the commit message you can say whatever you want in this message - so for example if this is the config for your dev instance you could say that...

    * run `git push origin master` this will push all your changes to the repo further changes should be made on branches for different servers or to master branch for this same server


--------

##Clone Custom Configuration to Host Server

* Open a terminal - ssh into your host server using the `islandora` user

* `cd` to `/opt/ISLE/config/`

* because your customizations are in a **private** repository, you'll need to add this islandora user's `id_rsa.pub` key to the repository via the website.

* This key is found in `/home/islandora/.ssh/id_rsa.pub`

* One can simply run `cat /home/islandora/.ssh/id_rsa.pub` and copy the output (ensure no whitespaces or extra returns)

* Go to the repository web site with a browser and locate your private repository.

      * locate the ssh key entry location (in Bitbucket this is in Settings / Access Keys /
      * **add** an ssh key w/ the label **islandora host server** paste in the key text and click **Add Key**

* Back at your terminal command line, run `git clone URLpathtoyourremoteprivaterepo.git .` (replacing "URLpathtoyourremoteprivaterepo" with the URL to the repository provided by the website)

* `cd` into the newly cloned directory - this is a good time to check that the ISLE directory contains your `yourdomain-config` directory and that it reflects all the edits and customizations.

## Spin up ISLE containers!


### Review or pull down ISLE Docker images

_Please Note: You may have already done this in setting up the host server manually and / or with Ansible. However it is always a good idea to review and check using the first command below._

* Check if all ISLE images have been downloaded
  * `docker image ls`


  * If yes, then proceed to Step 7

  * If no, the perform the following:
    * `docker pull islandoracollabgroup/isle-mysql:latest`
    * `docker pull islandoracollabgroup/isle-fedora:latest`
    * `docker pull islandoracollabgroup/isle-solr:latest`
    * `docker pull islandoracollabgroup/isle-apache:latest`
    * `docker pull islandoracollabgroup/isle-proxy:latest`

---
### Spin up the proxy container

* `cd /opt/ISLE/yourdomain-config`
* `docker-compose up -d proxy`



### Spin up mysql container and import production databases

* `cd /opt/ISLE/yourdomain-config`
* `docker-compose up -d mysql`

Two methods for connecting to the MySQL Database (GUI / CLI) pick one.

1. One may use SQL GUI clients e.g. Sequel Pro, Navicat, PHPMyAdmin etc.

2. If above not practical, one may connect to the MySQL container and run the following.

    * `docker exec -it isle-mysql-institution bash` to connect to the container
    * appropriate mysql commands here: consult MySQL documentation - https://dev.mysql.com/doc/refman/5.7/en/

The following are STEPS, not literal commands to prepare your db for ISLE (_doing this because we've found there are errors using exported db from production because of caches - causes problems - steps below help you remove this problem_)

* import the production databases into the isle-mysql-institution container (_with errors being ignored_)
* truncate all tables that start with `cache` on the isle-mysql-institution container
* export this new database to the `mysql` directory on the isle host server
* delete all tables (_not the database itself_) on the isle-mysql-institution container
* Reimport the new lighter database to the isle-mysql container




---

### Spin up fedora container and run reindex processes

Staying within `/opt/ISLE/yourdomain-config`

* `docker-compose up -d fedora`
   * _optional_ check if fedora is running properly e.g. `http://isle-prod-project.institution:8080/manager/html`
* `docker exec -it isle-fedora-institution bash`

### Reindex Fedora RI (1/3)
* `cd /usr/local/tomcat/conf/bin/`
* `./shutdown.sh`
* Wait 7 - 10 seconds for the service to stop properly
* Navigate to the fedora installation directory and run the following command for the first Fedora reindex
`cd /usr/local/fedora/server/bin /bin/sh fedora-rebuild.sh -r org.fcrepo.server.resourceIndex.ResourceIndexRebuilder > /home/islandora/fedora_ri.log 2>&1`
* This process may take upwards of 5-10 minutes depending on the number of objects in Fedora repository. If you want to follow the process, you can enter `tail -f /home/islandora/fedora_ri.log` it will tell you when complete.

### Reindex SQL RI (2/3)

* Truncate all existing tables within the Fedora (fedora3 or fedora) database on the MySQL server. (If necessary, see MySQL documentation on how to truncate tables).

   * Option 1: Truncate by GUI Application
     * Using a GUI based application e.g. Sequel Pro, access the fedora database and right click on the table information on the right hand side, the option to Truncate will appear, click on it to perform the action

   * Option 2: Truncate by Command Line - see MySQL documentation - be sure to use `mysql` or `127.0.0.1` as the database host if connected to the mysql container on the CLI.

* Navigate to the fedora installation directory and run the following command for the second MySQL reindex
  * `cd /usr/local/fedora/server/bin`
  * `/bin/bash fedora-rebuild.sh -r org.fcrepo.server.utilities.rebuild.SQLRebuilder > /home/islandora/sql_ri.log 2>&1`

* This process may take upwards of 5-10 minutes depending on the number of objects in Fedora repository. If you want to follow the process, you can enter `tail -f /home/islandora/sql_ri.log` it will tell you when complete.

* Restart Tomcat service
  * `cd /usr/local/tomcat/conf/bin/`
  * `./startup.sh`
  * Wait 7 - 10 seconds for the service to start properly

-----

### Spin up solr container

* Staying within `/opt/ISLE/yourdomain-config`
* `docker-compose up -d solr`
  * (_optional_) check if solr is running properly e.g. `http://isle-prod-project.institution:8777/manager/html`

### Reindex Solr from Fedora container (3/3)

* `docker exec -it isle-fedora-institution bash` **NOTE FEDORA NOT SOLR!**

* As this third process can take hours, recommend using the screen program to be able to detach from the command line and server without terminating the process.
* Exit any active containers. one should now be ssh'ed in only as the islandora user.
* Enter “screen” at the prompt. (this should start a new screen session)
* `docker exec -it isle-fedora-institution bash`
* `cd /usr/local/tomcat/webapps/fedoragsearch/client`
* `/bin/sh runRESTClient.sh localhost:8080 updateIndex fromFoxmlFiles`
* This process will now ask you for the appropriate fgsAdmin username & password
* Once entered, a small amount of output will print out but nothing more to indicate the process is running
* At this point, type “Ctrl-A” and then “d” to detach the screen. This will return one to the original prompt. It is now safe to exit the server without killing the process.
* One can check progress via `htop` or `ps aux`
* To reattach, `sudo su` and then type:  `screen -r`
* You can now see the process has finished.
* To exit the screen session, type exit.
* Check the results on the site using Islandora simple search or the appropriate search method.

---

### Spin up apache container

* Staying within `/opt/ISLE/yourdomain-config`
* `docker-compose up -d apache`
    * (_optional_) check if apache is running properly e.g. `https://isle-prod-project.institution`
    * (_optional_) You may find that you need to re-run the `fix-permissions.sh` script
        * `docker exec -it isle-apache-institution bash`
        * `/bin/bash /tmp/isle_drupal_build_tools/fix-permissions.sh --drupal_path=/var/www/html --drupal_user=islandora --httpd_group=www-data`
* Check site and outline QC process
