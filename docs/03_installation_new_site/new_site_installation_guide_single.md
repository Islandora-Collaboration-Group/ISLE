_Expectations:  It may take at least a minimum of **4 - 6 hours or more** to read this entire document and perform the installation as prescribed. This is not a quick process._

This guide documents how an enduser can spin up and install a single ISLE / Islandora environment tailored to use only one unique domain / URL and one ISLE environment.

A new ISLE / Islandora environment can include the option to create an un-themed Drupal website and empty Fedora repository for endusers to develop code, perform ingests, edit metadata, update fields in SOLR indexing all essential in ultimately creating a new ISLE production site.

While this checklist will attempt to point out most of the usage challenges or pitfalls, ISLE assumes no responsibility or liability in this matter should an enduser have customizations beyond what this guide outlines.

**Please note:** There is a [Glossary](../glossary.md) with relevant terms to help guide installation.

## Index of related documents
* [New Site Example User Story](new_site_example_user_story.md)

---


## Assumptions / Prerequisites

* Comfortability with ISLE. Recommend first setting up the [ISLE Test Site](../02_installation_test/ild_installation_guide.md) (`isle.localdomain`). If you have already done this, please proceed.

* Host Server that conforms to the specifications outlined in the [Host Server Specifications](../01_installation_host_server/host_server_system_specifications.md)

* This new site guide is designed for a single ISLE Host server that has already followed the appropriate setup and configuration instructions in the [New ISLE section](/index.md#new-isle) of the guide.

* Instructions below also assume a MacOS or Linux laptop or workstation to be used in conjunction with the ISLE Host Server for deploying configs, code, files etc. Windows users may have to adjust / swap out various tools as needed.

* These directions also depend on the type of local computer used to connect via browser to Islandora.

* Have an existing domain name that works - is set up with DNS etc...

* Have [SSL Certificates](../glossary.md#systems) previously created for the web domain. (_Please work with the appropriate internal IT resource to provision these files for your domain_)

* ISLE project has been cloned to BOTH your local laptop/workstation AND the ISLE host server


## Overview

* The .env and tomcat.env files are your primary resources for customizing your ISLE stack.
  * As a result your .env file will container passwords and usernames and must be treated with the utmost care. **Never** share you .env that contains information related to your site. 

<!-- * Setup a Private Code Repository
    * Most of the work in this guide involves careful editing of the various configuration and settings files that customize the pieces of Islandora (database, repository, web-server, etc...).
    * Doing this work in a code repository makes it easier to correct errors and to repeat the process for additional servers without needing to replicate all the work.
    * Since the edits could include things like passwords, it's important to make this a private repository. -->
<!-- 
* Customizing your Environment
    * Many of the steps below describe adding the domain name or other specific bits of information into files or appending those bits to file names.
    * In these cases this guide will call out the customization point AND provide an example - it's important not to literally copy paste the example! -->

<!-- ## Create Private Code Repository

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
 -->

##  Edits to make!

## Docker Environment Files:

There are .env files that exist in your cloned copy of the repository. This section describes what these files do, and their importance to your stack! Chiefly these files are tasked with automatically configuring and setting all ISLE systems to work together in the stack. ISLE removes the need of editing the more complex config files that are part of the Islandora stack manually. Just .env it!

You should edit these files with unique users/passwords, your domain name, site-name, etc. for your own _unique_ instance of ISLE to come alive.

**Edit the file: **.env** and **tomcat.env** before you up (`docker-compose up`)**

**REMEMBER: never share or post your complete .env publicly... EVER! Use caution, and when in doubt ask a maintainer for help (i.e., share the file privately with a maintainer)**

### Master Section:
    * COMPOSE_PROJECT_NAME to something unique (e.g. `COMPOSE_PROJECT_NAME=isle-production-collections`)
      * This variable is appended to things Docker objects like volume names and network names.
    * BASE_DOMAIN to your domainname (e.g. `BASE_DOMAIN=digital-collections.example.edu`)
      * This variable specifies your domain name!
    * CONTAINER_SHORT_ID to something unique (e.g. `CONTAINER_SHORT_ID=prod`).
      * This variable is appended to the end of all running containers, keep it _short_!
    

### Database Section:
| .env Variable                   | Purpose                               | ISLE Services updated  | What it does                                                                                                                            |
|-------------------------------  |-------------------------------------  |------------------------------------ |---------------------------------------------------------------------------------------------------------------------------------------  |
| MYSQL_ROOT_PASSWORD             | Set the `root` password for MySQL     | MySQL, Fedora, Apache               | Allows Fedora and Apache to update their relevant databases as well as to configure themselves to work together.                        |
| FEDORA_DB                       | Set the name of Fedora database       | MySQL, Fedora                       | Specifies which database to create or use for Fedora data.                                                                              |
| DRUPAL_DB                       | Set the name of the Drupal database   | MySQL, Apache, Fedora               | Specifies which database to create or use for Fedora data. Updates components of Fedora so it can read the Drupal database for users.   |
| FEDORA_DB_USER DRUPAL_DB_USER   | Sets the MySQL user                   | MySQL, Apache, Fedora               | Specifies names of Database users.                                                                                                      |
| FEDORA_DB_PASS DRUPAL_DB_PASS   | Sets MySQL user passwords             | MySQl, Apache, Fedora               | Specifies passwords of Database users.                                                                                                  |

### Islandora (DRUPAL) Section:
| .env Variable       | Purpose                                     | ISLE Services updated  | What it does                                                                                                                                                                                    |
|-------------------- |-------------------------------------------  |------------------------------------ |------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| DRUPAL_SITE_NAME    | Sets the Drupal Site Name variable          | Drupal                              | Sets the name of your Islandora Website; e.g.:  "<Your Group's Name> Digital Collections"                                                                                                       |
| DRUPAL_ADMIN_USER   | Set the name of Drupal admin                | Drupal                              | Specifies the 'admin user' for your Islandora website.                                                                                                                                          |
| DRUPAL_ADMIN_PASS   | Set the password of Drupal admin            | Drupal                              | Specifies the password of 'admin user' for your Islandora website.                                                                                                                              |
| DRUPAL_ADMIN_EMAIL  | Set the email of Drupal admin               | Drupal                              | Specifies the email address of the 'admin user' for your Islandora site.                                                                                                                        |
| DRUPAL_HASH_SALT    | Secures your installation by hashing data   | Drupal                              | Secures your install of Drupal (Islandora) by hashing (obscuring) key data. Use [password generater tool](https://passwordsgenerator.net/) to create a HASH_SALT, remember alphanumeric characters ONLY (no special characters).  |

### Fedora Repository Section:
| .env Variable         | Purpose                           | ISLE Services updated           | What it does                                                                                                                                                |
|---------------------  |---------------------------------- |-------------------------------  |------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| FEDORA_ADMIN_USER     | Set the name of Fedora admin      | Fedora                          | Sets the master administrator username to login to the Fedora repository. You login with this user.                                                         |
| FEDORA_ADMIN_PASS     | Set the password of Fedora admin  | Fedora                          | Sets the master administrator password to login to the Fedora repository.                                                                                   |
| FEDORA_GSEARCH_USER   | Sets the username for FGS         | Fedora, Fedora Generic Search   | Sets the Fedora Generic Search (FGS) username used to login to the Fedora repository. FGS uses this user to connect to Fedora to update your search index.  |
| FEDORA_GSEARCH_PASS   | Sets the password for FGS         | Fedora, Fedora Generic Search   | Sets the password for FGS to gain access to your repository.                                                                                                |
| FEDORA_INTCALL_USER   | Sets the internal call user       | Fedora                          | The internal call username                                                                                                                                  |
| FEDORA_INTCALL_PASS   | Sets the internal call password   | Fedora                          | The internal call password                                                                                                                                  |

### Image Services Section:
| .env Variable                       | ISLE Services updated   | What it does                                                                                                                          |
|-----------------------------------  |-----------------------  |-------------------------------------------------------------------------------------------------------------------------------------- |
| CANTALOUPE_ADMIN_INTERFACE_ENABLE   | ImageServices           | Enables or Disables the Cantaloupe IIIF /admin control panel. Locatied at http://hostip:8083/cantaloupe/admin when true, else false.  |
| CANTALOUPE_ADMIN_USER               | ImageServices           | Set the admin username to login to the admin panel.                                                                                   |
| CANTALOUPE_ADMIN_PASS               | ImageServices           | Set the admin password to login to the admin panel.                                                                                   |

### Tomcat.env applies to all Tomcat instances
| Tomcat.env Variable   | ISLE Services updated         | What it does                                            |
|---------------------  |-----------------------------  |-------------------------------------------------------  |
| TOMCAT_ADMIN_USER     | Fedora, Solr, ImageServices   | Set the admin username to login to the admin panel.     |
| TOMCAT_ADMIN_PASS     | Fedora, Solr, ImageServices   | Set the admin password to login to the admin panel.     |
| TOMCAT_MANAGER_USER   | Fedora, Solr, ImageServices   | Set the manager username to login to the admin panel.   |
| TOMCAT_MANAGER_PASS   | Fedora, Solr, ImageServices   | Set the manager password to login to the admin panel.   |

---

## Config Directory

The config directory has many purposes like holding customized configuration files mounted to specific containers (which we have no covered here), but for a single simple site we only use it to hold our proxy configs and as a place to store our SSL Certificate and Key.

-------

### Proxy directory

If need be, please refer to the **Systems** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

The `proxy` subdirectory contains all specific configurations necessary for the Traefik proxy to function properly with your changes.

If need be, please refer to the **SSL certificate** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

There are also additional links for the enduser to learn how to combine the SSL Certificate File with any available SSL Certificate Chain File for the `proxy` process to work properly.

* Copy your SSL certificates for the ISLE Proxy into `config/proxy/ssl-certs`. They will and should have different names than the examples provided below.

    * There can only be 2 files involved in this process.

        * 1 x SSL Certificate Key File e.g. `newsite-sample-key.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.key` or `.pem`

        * 1 x SSL Certificate File e.g. `newsite-sample.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

* Edit the `config/proxy/traefik.toml` file:
  * Change line 27 and 28:
    *  `certFile = "/certs/newsite-sample.cert"`  ## Change to reflect your CERT, CRT, or PEM
    *  `keyFile = "/certs/newsite-sample-key.key"`  ## Change to reflect your KEY, or PEM.


<!-- -----

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

* `cd` into the newly cloned directory -->

---

## Spin up ISLE containers!

* Run `docker-compose up -d`

* Wait for the stack to completely initialize, about 5 minutes (typically much less).

* Run `docker exec -it isle-apache-<CONTAINER_SHORT_ID> bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh`
     * Using the <CONTAINER_SHORT_ID> value from the .env file here (_typically: isle-apache-prod, or -stage, -dev, etc._)

* Give this process 15 - 25 minutes (_depending on the speed of the ISLE Host server internet connection_)

* Check the newly created and running new site by opening a browser and navigating to your site domain e.g. `https://digital-collections.example.edu`, you should now see an un-themed Drupal site.

--------
