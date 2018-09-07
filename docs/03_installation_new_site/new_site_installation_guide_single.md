_Expectations:  It may take at least a minimum of **4 - 6 hours or more** to read this entire document and perform the installation as prescribed. This is not a quick process._

This guide documents how an enduser can spin up and install a single ISLE / Islandora environment tailored to use only one unique domain / URL and one ISLE environment.

A new ISLE / Islandora environment can include the option to create an un-themed Drupal website and empty Fedora repository for endusers to develop code, perform ingests, edit metadata, update fields in SOLR indexing all essential in ultimately creating a new ISLE production site.

While this checklist will attempt to point out most of the usage challenges or pitfalls, ISLE assumes no responsibility or liability in this matter should an enduser have customizations beyond what this guide outlines.

**Please note:** There is a [Glossary](../glossary.md) with relevant terms to help guide installation.

## Index of related documents
* [New Site Example User Story](new_site_example_user_story.md)

---


## Assumptions / Prerequisites

* Comfortability with ISLE. Recommend first setting up the ISLE Test Site (`isle.localdomain`) via the [Install Start Here](../install_start_here.md) guide. If you have already done this, please proceed.

* Host Server that conforms to the specifications outlined in the [Host Server Specifications](../01_installation_host_server/host_server_system_specifications.md)

* This new site guide is designed for a single ISLE Host server that has already followed the appropriate setup and configuration instructions in the `Create a new ISLE site` section of the [Install Start Here](../install_start_here.md) guide.

* Instructions below also assume a MacOS or Linux laptop or workstation to be used in conjunction with the ISLE Host Server for deploying configs, code, files etc. Windows users may have to adjust / swap out various tools as needed.

* These directions also depend on the type of local computer used to connect via browser to Islandora.

* Have an existing domain name that works - is set up with DNS etc...

* Have [SSL Certificates](../glossary.md#systems) previously created for the web domain. (_Please work with the appropriate internal IT resource to provision these files for your domain_)

* ISLE project has been cloned to BOTH your local laptop/workstation AND the ISLE host server


## Overview

* Setup a Private Code Repository
    * Most of the work in this guide involves careful editing of the various configuration and settings files that customize the pieces of Islandora (database, repository, web-server, etc...).
    * Doing this work in a code repository makes it easier to correct errors and to repeat the process for additional servers without needing to replicate all the work.
    * Since the edits could include things like passwords, it's important to make this a private repository.

* Customizing for your Environment
    * Many of the steps below describe adding the domain name or other specific bits of information into files or appending those bits to file names.
    * In these cases this guide will call out the customization point AND provide an example - it's important not to literally copy paste the example!

## Create Private Code Repository

**ON your local laptop/workstation:**

* Using a git repository of your choice (GitHub, GitLab, Bitbucket, etc.) create a PRIVATE remote git repo - see the specific code repository documentation online for setup instructions.

* Open a terminal and navigate to `/opt/ISLE` (_or where you cloned the ISLE directory on your local workstation._)

* Copy the isle-new-site directory to a new directory with a name of your choice

   * Example: where "digital-collections.yourdomain.com" is your server domain name: 
   * `cp -R isle-newsite-sample digital-collections.example.edu`
   * Note, please do not use this literal value.

* `cd` into the newly copied `digital-collections.example.edu` directory and type: `git init` to imitate this directory as a code repository.

* `git remote add NameOfYourRepository URLofYourRepository` to connect your local repository to the remote you set up in the above steps.
  * **NOTE** replace "NameOfYourRepository" and "URLofYourRepository" with the name of your git repository and its URL

* You are now ready to perform the customization edits in this directory (you can use a text editor of choice now don't have to stay in terminal - just locate the folder in the finder and open file in text editor)

##  Edits

<<<<<<< HEAD
### .env file:

* Edit the .env file and change the values of COMPOSE_PROJECT_NAME, BASE_DOMAIN, and CONTAINER_SHORT_ID. e.g. for a production site you may use:

    `COMPOSE_PROJECT_NAME=isleproduction`

    `BASE_DOMAIN=mydomain.edu`

    `CONTAINER_SHORT_ID=prod`
=======
### Docker Environment File:

* Edit the file: **.env** accordingly:

    * COMPOSE_PROJECT_NAME to something unique (e.g. `COMPOSE_PROJECT_NAME=isle-production-collections`)
    * BASE_DOMAIN to your domainname (e.g. `BASE_DOMAIN=digital-collections.example.edu`)
    * CONTAINER_SHORT_ID to something unique.  It is appended to the end of all running containers, keep it _short_ (e.g. `CONTAINER_SHORT_ID=prod`)
>>>>>>> upstream/master

**Please note:** Much of the file is already with comments guiding the enduser to key areas or files to edit or modify accordingly.

---

## Config directories

Now proceed to make edits to files within the config subdirectories:

This will involve your adding **domain name** (digital-collections.example.edu), desired passwords to **ensure your safety**:

There are seven (7) subdirectories which have the appropriate settings for each respective container and service:

* apache
* mysql
* traefik - The proxy
* tomcat - This applies settings to all instances of Tomcat.
* fedora
* gsearch
* solr

----

### Apache directory

The `apache` subdirectory contains all specific configurations and overrides necessary for the ISLE apache image and resulting container to function properly with your changes. This is the webserver that serves the Islandora / Drupal website. These instructions assume you are installing a NEW SITE.

* `cd` into the `config/apache` folder (`cd config/apache`).

* Clone the current [ISLE Drupal Build tools](https://github.com/Islandora-Collaboration-Group/isle_drupal_build_tools): 
`git clone https://github.com/Islandora-Collaboration-Group/isle_drupal_build_tools -b 7.x-1.11`

* `cd` into the build tools directory: (`cd isle_drupal_build_tools`)

* Edit the file: `isle_drush_make\settings.php`

    * Lines 251-253: add your database name, database user, and database password    

    * Line 288: to include a 45+ alpha-numeric characters drupal hash between the quotes after this text: `$drupal_hash_salt = '';`

        * You'll need to create this value, recommend using a password generator tool. Ensure only alpha-numeric characters are used, no symbols etc.

    * Line 311: Review and ensure `# $base_url = ` is still commented out

* Edit the file `isle_drush_make\islandora.drush.make` to add or remove Islandora modules.

  * To disable modules please use `;` to comment out the lines, or delete them entirely.

* Edit the file: `isle_islandora_installer.sh`

    * Find the section `## Site install` and edit:
    `drush site-install -y --account-name=newsite_admin --account-pass=newsite_adminpw --account-mail=admin@newsite.com --site-name="ISLE New Site Sample"`

    * Change the following values in that line above to the appropriate names and passwords for your site.

        *  --account-name= Drupal admin account
        *  --account-pass= Drupal admin account password
        *  --account-mail= Email address that can be associated with Drupal admin account
        *  --site-name= Name of your new ISLE website

#### Apache - sites-enabled

* Within the `sites-enabled` directory, rename the file `newsite-sample.conf` to your domain name - example:

    * `digital-collections.example.edu.conf`

* Edit the previously named `newsite-sample.conf` file and change lines 3 and 4 to point to the location of your apache logs on the container - example:

    * `ErrorLog /var/log/apache2/digital-collections.example.edu.ssl.error.log`

    * `CustomLog /var/log/apache2/digital-collections.example.edu.ssl.access.log combined`

----

### Fedora directory

The `fedora` subdirectory contains all specific configurations and overrides necessary for the ISLE fedora image and resulting container to function properly with your changes. This is the Fedora repository that will contain all objects, metadata etc.

* Within the `config/fedora` subdirectory, **add new passwords** in the following files:

     * Lines 3, 8, 14: `fedora-users.xml`  (change all applicable passwords for fedora users)
     * Line 598: `fedora.fcfg`  (change the password to the `fedora_admin` **database user password** only)
     * Line 15: `filter-drupal.xml`  (change the associated Drupal site database name , user and password, do not use settings for the `fedora3` database.)

* (_Optional_) Edit the contents of the repository-policies subdirectory as necessary **ONLY IF YOU NEED TO**. If you have changed them uncomment the section in docker-compose.yml.

------

### GSearch directory (aka Fedora Generic Search)

* Within the `gsearch` directory, edit the file: `fedoragsearch.properties` at line 7 and add a space after the equal sign and then add the new *gsearch fgsAdmin user password*.

     * ` -  fedoragsearch.soapPass                = new_fgsAdmin_password_here`

* Edit the file: `fgsconfig-basic-configForIslandora.properties`

     * Line 26: add the new *gsearch fgsAdmin user password*.

        * `gsearchPass=new_fgsAdmin_password_here`

    * Line 67: Add the new fedora *fedora admin password*.

        * `fedoraPass=new_fedoraAdmin_password_here`

* Edit the file: `fedora/gsearch/fgsconfigObjects.properties` at line 15 and add a space after the equal sign and then add the new *fedora admin password*.

     * `fgsconfigObjects.fedoraPass            = new_fedoraAdmin_password_here`

* Edit the file: `fedora/gsearch/repository.properties` at line 7 and add a space after the equal sign and then add the new *fedora admin password*.

     * `fgsrepository.fedoraPass        = new_fedoraAdmin_password_here`

----

### Mysql directory

The `mysql` subdirectory contains all specific configurations and overrides necessary for the ISLE mysql image and resulting container to function properly with your changes. This is the Mysql database server that will contain two databases, one for the Islandora / Drupal website and the other for the Fedora repository.

* (_Optional_) Edit the Mysql configuration file `my.cnf` as needed otherwise leave alone.


#### Mysql - initscripts

This subdirectory houses SQL scripts necessary for a one time creation of your associated new site and `fedora3` database.

You'll want to rename `newsite_sample_db.sql` to the database or domain name of your choice.

* Edit the contents of `newsite_sample_db.sql` to create the new drupal site database and user.

    * Line 1: Change the database name from `newsite_sample_db` to the database name of your choice.

    * Line 2: Change the database user name from `newsite_sample_db_user` to the database user name of your choice.

    * Line 3: At almost the end of the line, change the value of `newsite_sample_db.*` to the to the database name of your choice ensuring the `.*` remain without a space.

    * Line 3: At the end of the line, change the value of `newsite_sample_db_user'` to the to the database user name of your choice ensuring the values remain with in the `''`quotes without spaces. Do not alter the remaining code (`@'%';'`) beyond that point.


* Edit the contents of `fedora3` to change the `fedora_admin` user password only.

    * Line 2: Change the `fedora_admin` user password from `newsite_sample_fedora_admin_pw` to the password of your choice.

    * It is not recommended to change anything else.

-------

### Traefik directory

If need be, please refer to the **Systems** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

The `traefik` subdirectory contains all specific configurations necessary for the Traefik proxy to function properly with your changes.

#### ssl-certs

Copy your SSL certs into the `certs` subdirectory and tell Traefik about them.

If need be, please refer to the **SSL certificate** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

There are also additional links for the enduser to learn how to combine the SSL Certificate File with any available SSL Certificate Chain File for the `proxy` process to work properly.

* Copy your SSL certificates for the ISLE Proxy into `config/traefik/certs`. They will and should have different names than the examples provided below.

    * There can only be 2 files involved in this process.

        * 1 x SSL Certificate Key File e.g. `newsite-sample-key.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.key` or `.pem`

        * 1 x SSL Certificate File e.g. `newsite-sample.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

* Edit the `config/traefik/traefik.toml` file:
  * Change line 78 and 79:
    *  `certFile = "/certs/isle.localdomain.cert"`  ## Change to reflect your CERT, CRT, or PEM
    *  `keyFile = "/certs/isle.localdomain.key"`  ## Change to reflect your KEY, or PEM.

  * Change line 167 `domain = "isle.localdomain"` to your domain (this is unnecessary but is important for consistency)

-----

### Solr directory

The `solr` subdirectory contains all specific configurations and overrides necessary for the ISLE solr image and resulting container to function properly with your changes. This is the SOLR search server that properly indexes all objects and metadata to provide speedy in-depth search functionality for the Islandora / Drupal site of the objects and collections contained within the Fedora repository.

If need be, please refer to the **Solr** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

* (_Optional_) Within the **solr** subdirectory:

    * Make the appropriate customizations or changes to the following files as you see fit otherwise you can leave these files alone and SOLR will work out of the box.

        * `schema.xml`

        * `solr.xml`

        * `solrconfig.xml`

        * `stopwords.txt`

--------

## Final steps

If need be, please refer to the **Git** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

* Now that all the changes are made (be sure to save), ISLE should be ready to test. First you'll need to push these changes to your private code repository.

    * Open a terminal - `cd` to the config directory you've been making the changes in...

    * `git status` this will show you all the files that have been modified and ready to be added to your private repo along with handy paths for the next steps.

    * `git add -A` to commit all changes.

    * Enter `git status` again - everything should be in green now as all modified files have been added - if anything's still red use git add and the path to add it

    * then run `git commit -m "initial config commit"` inside the double quotes is the commit message you can say whatever you want in this message - so for example if this is the config for your dev instance you could say that...

    * Enter `git push {NAME OF YOUR REPOSITORY REMOTE} master`

      * This will push all your changes to the repo.

      * Further changes should be made on git branches for different servers or environments e.g. `prod, stage, dev` or to master branch for this same server.

--------

## Clone Custom Configuration to Host Server

**Please note:** If running a (multi) ISLE environment, you will make separate git config directories to this effect and repeat these steps accordingly.

* Open a terminal - ssh into your host server using the `islandora` user

* `cd` to `/opt/ISLE/config/`

* Because your customizations are in a **private** repository, you'll need to add this islandora user's `id_rsa.pub` key to the git repository settings via it's settings profile within the appropriate website.

* This key is found in `/home/islandora/.ssh/id_rsa.pub`

* One can simply run `cat /home/islandora/.ssh/id_rsa.pub` and copy the output (ensure no whitespaces or extra returns)

* Go to the repository web site with a browser and locate your private repository.

      * Locate the ssh key entry location (in Bitbucket this is in Settings / Access Keys /

      * **Add** an ssh key w/ the label **isle host server islandora deploy user** paste in the key text and click **Add Key**

* Back at your terminal command line, run `git clone URLpathtoyourremoteprivaterepo.git .` (replacing "URLpathtoyourremoteprivaterepo" with the URL to the repository provided by the website)

* `cd` into the newly cloned directory

## Spin up ISLE containers!

* run `docker-compose up -d`

* run `docker exec -it yourApacheContainerNameHere bash`

     * Using the name from the `docker-composer.yml` file here (_typically: isle-apache-prod, or -stage, etc..._)

* `cd /tmp/isle_drupal_build_tools`

* Run: `./install_new_site.sh`

* Give this process 15 - 25 minutes (_depending on the speed of the ISLE Host server internet connection_)

* Check the newly created and running new site by opening a browser and navigating to your site domain e.g. `https://digital-collections.example.edu`, you should now see an un-themed Drupal site.

--------
