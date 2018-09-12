_Expectations:  It may take at least a minimum of **4 - 6 hours or more** to read this entire document and perform the installation as prescribed. This is not a quick process._

This guide documents how an enduser can spin up and install **multiple** ISLE / Islandora sites within multiple ISLE / Islandora environments e.g. (production, staging and development) all tailored to using their own unique domains / URLs.

A new ISLE / Islandora environment can include the option to create an un-themed Drupal website and empty Fedora repository for endusers to develop code, perform ingests, edit metadata, update fields in SOLR indexing all essential in ultimately creating a new ISLE production site.

While this checklist will attempt to point out most of the usage challenges or pitfalls, ISLE assumes no responsibility or liability in this matter should an enduser have customizations beyond what this guide outlines.

## Index of related documents
* [New Site Example User Story](new_site_example_user_story.md)

---

**Please note:** There is a [Glossary](../glossary.md) with relevant terms to help guide installation.

## Assumptions / Prerequisites

* Comfortability with ISLE. Recommend first setting up the [ISLE Test Site](02_installation_test/ild_installation_guide.md) (`isle.localdomain`). If you have already done this, please proceed.

* Host Server that conforms to the specifications outlined in the [Host Server Specifications](../01_installation_host_server/host_server_system_specifications.md)

* This new site guide is designed for a Host server that has already followed the appropriate setup and configuration instructions in the [Create a new ISLE site](03_installation_new_site/new_site_installation_guide_single.md) guide.

* Instructions below also assume a MacOS or Linux laptop or workstation to be used in conjunction with the ISLE Host Server for deploying configs, code, files etc. Windows users may have to adjust / swap out various tools as needed.

* These directions also depend on the type of local computer used to connect via browser to Islandora.

* Have an existing domain name that works - is set up with DNS etc...

* Have [SSL Certificates](../glossary.md#systems) previously created for the web domain. (_Please work with the appropriate internal IT resource to provision these files for your domain_)

* ISLE project has been cloned to BOTH your local laptop/workstation AND the ISLE host server

---

## Multiple ISLE / Islandora Environments
When attempting to configure multiple ISLE / Islandora environments, all sections labeled `(multi)` provide additional steps to follow and/or repeat.

This is primarily because only one `proxy` container is needed to manage all ISLE environments (e.g. production, staging and development) on one ISLE Host Server or VM.

When deploying multiple ISLE environments on one ISLE host server, a few extra steps are needed as well.

Once the initial ISLE environment has been setup for all following environments, you'll need to please do the following:

* Continue to use / copy the `isle-newsite-sample` directory for any new or additional ISLE environments as directed above.

* Per each section with a `(multi)` indicated, edit files accordingly but be aware that you'll be moving them into the appropriate subdirectory of the first created ISLE environment's `proxy` directory.

* No other ISLE environments are to a) use their own `proxy` directory and b) have a `proxy:` section within their separate `docker-compose.yml` files.

* Only use one `proxy` for multi ISLE environments. This container will route connections and traffic properly, keeping environments separate and data siloed.

Example:

Multiple ISLE environments enabled within the first ISLE config using a single `proxy`.

In this example the `dev` ISLE environment was created first for testing and additional environments `stage` and `prod` (production) were added but are still controlled by the `proxy` config within the first created ISLE environment `dev`. This is what the final results should look like. Please note, you could run more than one site this way and are only limited by the appropriate resources of your ISLE host server.

```
├── isle-newsite-dev.yourdomain.edu          (1st ISLE environment created with proxy subdirectory and proxy: section in its docker-compose.yml)
│   └── proxy
│       └── config
│           ├── sites-enabled
│           │   ├── edu.example.digital-collections-dev.conf
│           │   ├── edu.example.digital-collections-prod.conf
│           │   └── edu.example.digital-collections-stage.conf
│           ├── ssl-certs
│           │   ├── edu.example.digital-collections-dev-interm.pem
│           │   ├── edu.example.digital-collections-dev-key.pem
│           │   ├── edu.example.digital-collections-dev.pem
│           │   ├── edu.example.digital-collections-prod-interm.pem
│           │   ├── edu.example.digital-collections-prod-key.pem
│           │   ├── edu.example.digital-collections-prod.pem
│           │   ├── edu.example.digital-collections-stage-interm.pem
│           │   ├── edu.example.digital-collections-stage-key.pem
│           │   └── edu.example.digital-collections-stage.pem
│           └── upstreams.d
│               ├── edu.example.digital-collections-dev.conf
│               ├── edu.example.digital-collections-prod.conf
│               └── edu.example.digital-collections-stage.conf
|
├── isle-newsite-prod.yourdomain.edu            (3rd ISLE environment created, no proxy subdirectory, no proxy: section in its docker-compose.yml)
|
├── isle-newsite-sample                         
|
└── isle-newsite-stage.yourdomain.edu           (2nd ISLE environment created, no proxy subdirectory, no proxy section in its docker-compose.yml)
```

---

## Overview

* Setup a Private Code Repository
      * Most of the work in this guide involves careful editing of the various configuration and settings files that customize the pieces of Islandora (database, repository, web-server, etc...).
      * Doing this work in a code repository makes it easier to correct errors and to repeat the process for additional servers without needing to replicate all the work.
      * Since the edits could include things like passwords, it's important to make this a private repository.

* Customizing for your Environment
      * Many of the steps below describe adding the domain name or other specific bits of information into files or appending those bits to file names.
      * In these cases this guide will call out the customization point AND provide an example - it's important not to literally copy paste the example!

---

## Create Private Code Repository

**ON your local laptop/workstation:**

* Using a git repository of your choice (GitHub, GitLab, Bitbucket, etc.) create a PRIVATE remote git repo - see the specific code repository documentation online for setup instructions.

* Open a terminal - navigate to `/opt/ISLE/config` (_or where you put the ISLE directory on your local workstation._)

* Create a new directory with a name of your choice

   * Example: where "digital-collections.yourdomain.com" is your server domain name e.g.:  `config/digital-collections.example.edu`

   * Please do not use this literal value.

* Locate the directory within the `/opt/ISLE/config/` directory called `isle-newsite-sample` and copy all of its contents into your newly created directory.

* `cd` into the newly copied and renamed `digital-collections.example.edu` directory and type:
 `git init` to imitate this directory as a code repository.

* `git remote add NameOfYourRepository URLofYourRepository` to connect your local repository to the remote you set up in the above steps.

    * [**NOTE** replace "NameOfYourRepository" and "URLofYourRepository" with the name of your git repository and its URL]

* You are now ready to perform the customization edits in this directory (you can use a text editor of choice now don't have to stay in terminal - just locate the folder in the finder and open file in text editor)

##  Edits

### Docker compose file:

* Edit the file: **docker-compose.yml** accordingly:

      * Change container names based on the environment (prod, stage, dev, etc)

      * For example if you are building a dev server instance:

          `container_name: isle-solr-newsite`

          changes to:

          `container_name: isle-solr-dev`

          and so on...

**Please note:** Much of the file is already with comments guiding the enduser to key areas or files to edit or modify accordingly.

---

## Config directories

Now proceed to make edits to files within the config subdirectories:

a. to reflect **environment** (prod, stage, dev, etc...)

b. to involve your **domain name** (digital-collections.example.edu)

There are five subdirectories which have the appropriate settings for each respective container and service:

* apache
* fedora
* mysql
* proxy
* solr

----

### Apache directory

The `apache` subdirectory contains all specific configurations and overrides necessary for the ISLE apache image and resulting container to function properly with your changes. This is the webserver that serves the Islandora / Drupal website.

* (_Optional_) Edit the php.ini file to change various appropriate settings if needed otherwise leave alone.

    * Example - Line 820: change the `upload_max_filesize = 2000M` to a higher setting if desired. Currently at a 2GB upload limit.

* Edit the file: `settings.php`

    * Lines 251-253: add your database name, database user, and database password    

    * Line 288: to include a 45+ alpha-numeric characters drupal hash between the quotes after this text: `$drupal_hash_salt = '';`

        * You'll need to create this value, recommend using a password generator tool. Ensure only alpha-numeric characters are used, no symbols etc.

    * Line 311: Review and ensure `# $base_url = ` is still commented out

* Edit the file: `install_new_site.sh`  line 29:

    * `/usr/local/bin/drush site-install -y --account-name=newsite_admin --account-pass=newsite_adminpw --account-mail=admin@newsite.com --site-name="ISLE New Site Sample"`

    * Change the following values in that line above to the appropriate names and passwords for your site.

        *  --account-name= Drupal admin account
        *  --account-pass= Drupal admin account password
        *  --account-mail= Email address that can be associated with Drupal admin account
        *  --site-name= Name of your new ISLE website

---

#### Apache - sites-enabled

* Within the `sites-enabled` directory, rename the files `newsite-sample-ssl.conf` and `newsite-sample.conf` to your domain names - example:
    * `digital-collections.example.edu_ssl.conf`

    * `digital-collections.example.edu.conf`

* Edit the previously named `newsite-sample.conf` file and change lines 3 and 4 to point to the location of your apache logs on the container - example:

    * `ErrorLog /var/log/apache2/digital-collections.example.edu.ssl.error.log`

    * `CustomLog /var/log/apache2/digital-collections.example.edu.ssl.access.log combined`

* Edit the previously named `newsite-sample-ssl.conf` file and change lines 4 and 5 to point to the location of your apache logs on the container - example:

    * `ErrorLog /var/log/apache2/digital-collections.example.edu.ssl.error.log`

    * `CustomLog /var/log/apache2/digital-collections.example.edu.ssl.access.log combined`

* Edit the previously named `newsite-sample-ssl.conf` file and change lines 12 and 13 to point to the location of your certs on the container - example:

```

    SSLCertificateFile	/certs/newsite-sample.pem
    SSLCertificateChainFile /certs/newsite-sample-interm.pem
    SSLCertificateKeyFile /certs/newsite-sample-key.pem

```

---

#### Apache - ssl-certs

If need be, please refer to the **SSL certificate** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

* Copy your SSL certificates for Apache into the `apache/ssl-certs` subdirectory. They will and should have different names than the examples provided below dependent on the ISLE environment you are setting up e.g. (_production, staging or development_).

    * There can be up to 2 - 3 files involved in this process.

        * 1 x SSL Certificate Key File e.g. `newsite-sample-key.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.key` or `.pem`

        * 1 x SSL Certificate File e.g. `newsite-sample.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

        * 1 x SSL Certificate Chain File e.g. `newsite-sample-interm.pem`
            * This file may be **optional** in some setups but is generally recommended for use by the `apache` container when available.
            * It will not be used by the `proxy` container.
            * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

#### Apache - ssl-certs (multi)

* When creating multiple environments for ISLE, please change all of the file and key names accordingly to reflect the environment e.g. adding (`-prod, -stage, -dev` to file names). Later on, this process will assist in organizing proper filing of files for the `proxy` container and stop any situation where a file gets overwritten or improperly referenced by the wrong environment.

**Example:**

    * 1 x SSL Certificate Key File e.g. `newsite-dev-key.pem`
    * 1 x SSL Certificate File e.g. `newsite-dev.pem`
    * 1 x SSL Certificate Chain File e.g. `newsite-dev-interm.pem`

---

#### Apache - tmpreaper

* (_Optional_) Edit the cron file within the `apache/tmpreaper` subdirectory to change various appropriate settings if needed otherwise leave alone if you are not familiar with how cron jobs work.

* Currently this cronjob is set to run every 12 hours and using the tmpreaper utility to clean out and delete the contents of the /tmp folder if they are older than 4 days.

----

### Fedora directory

The `fedora` subdirectory contains all specific configurations and overrides necessary for the ISLE fedora image and resulting container to function properly with your changes. This is the Fedora repository that will contain all objects, metadata etc.


#### Fedora - fedora

* Within the `fedora/fedora` subdirectory, **add new passwords** in the following files:

     * Lines 3, 8, 14: `fedora-users.xml`  (change all applicable passwords for fedora users)
     * Line 598: `fedora.fcfg`  (change the password to the `fedora_admin` **database user password** only)
     * Line 15: `filter-drupal.xml`  (change the associated Drupal site database name , user and password, do not use settings for the `fedora3` database.)

* (_Optional_) Edit either of the log4j.properties or logback.xml files to change logging levels for the `fedora` container if needed, otherwise leave alone.

     * Please note: If making changes, you'll need to then edit the `docker-compose.yml` file accordingly in the `fedora` `volumes` section

     * Example add a line to make changes for the Fedora application logging.
     * `- ./fedora/fedora/logback.xml :/usr/local/fedora/server/config/logback.xml`

     * Example add a line to make changes for the Djatoka application logging.
     * `- ./fedora/fedora/logback.xml :/usr/local/tomcat/webapps/adore-djatoka/WEB-INF/classes/log4j.properties`     

* (_Optional_) Edit the contents of the repository-policies subdirectory as necessary **IF YOU NEED TO**, otherwise leave alone.

------

#### Fedora - gsearch

* Within the `fedora/gsearch` subdirectory, edit the file: `fedoragsearch.properties` at line 7 and add a space after the equal sign and then add the new *gsearch fgsAdmin user password*.

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

---

#### Fedora - tmpreaper

* (_Optional_) Edit the cron file within `fedora/tmpreaper` to change various appropriate settings if needed otherwise leave alone if you are not familiar with how cron jobs work.

* Currently this cronjob is set to run every 12 hours and using the tmpreaper utility to clean out and delete the contents of the /tmp folder if they are older than 4 days.

---

#### Fedora - tomcat

* Within the **tomcat** subdirectory:

     * Strongly recommend adding tomcat user passwords in file: `fedora/tomcat/tomcat-users.xml`

Example: ONLY ADD TOMCAT USERNAME/PASSWORD - can look like this but please use a stronger password with more than 10 alpha-numeric characters. Also note, they should NOT be the same password.:

```
          <!-- user manager can access only manager section -->
             <user username=“manager” password=“tomcat_manager_password" roles=“manager-gui” />  <!--enduser please add passwords / users and remove this comment -->

          <!-- user admin can access manager and admin section both -->
             <user username=“admin” password=“tomcat_admin_password” roles=“manager-gui,admin-gui” /> <!--enduser please add passwords / users and remove this comment -->

          </tomcat-users>

```

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

### Proxy directory

If need be, please refer to the **Systems** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

The `proxy` subdirectory contains all specific configurations and overrides necessary for the ISLE proxy image and resulting container to function properly with your changes. This is the Nginx reverse proxy server that properly routes all internal and external communication between the other four containers and the ISLE Host network.

#### Proxy - sites-enabled

* In the `config/sites-enabled` subdirectory, rename the `com.sample-newsite.conf` file to `com.yourdomain.conf` (where "yourdomain" is the domain name of your server but the suffix .com typically last is now first.)

    * Example: `edu.example.digital-collections.conf`

* Now edit the file...

    * Line 6: change `newsite-sample.com` to the domain name of your choice

        * Ensure a space between `server_name` and the domain name and a `;` after the domain name still remain.

    * Line 15: change `newsite-sample.com` to the domain name of your choice

        * Ensure a space between `server_name` and the domain name and a `;` after the domain name still remain.

    * Lines 18 & 19: Change to the name of your ssl certs (should be same names as in previously made `apache` edits above)

#### Proxy - sites-enabled (multi)

**Please note:** If you only intend to deploy one ISLE environment ever e.g. a Production environment, you do not necessarily have to edit all of the following. You can move onto the next section.

Otherwise, if you are planning on running multiple ISLE environments e.g. production, development etc together on one ISLE host server, please continue to edit your .conf file accordingly.

**Please also note:** The order of environment creation is not necessarily important e.g you don't need to setup a development environment prior to a production one. However in general, best practice is to test prior to opening up a server to the public.

* Line 54: Add the appropriate environment suffix e.g. `-prod, -stage, -dev etc.`

    * Example: Change `proxy_pass https://apache-internal;` to `proxy_pass https://apache-internal-dev;`

* Line 66: Add the appropriate environment suffix e.g. `-prod, -stage, -dev etc.`

    * Example: Change `proxy_pass http://fedora-internal/adore-djatoka;` to `proxy_pass http://fedora-internal-dev/adore-djatoka;`

* Line 74: Add the appropriate environment suffix e.g. `-prod, -stage, -dev etc.`

    * Example: Change `proxy_pass http://fedora-internal/fedora/get;` to `proxy_pass http://fedora-internal-dev/fedora/get;``

* Line 80: Add the appropriate environment suffix e.g. `-prod, -stage, -dev etc.`

    * Example: Change `proxy_pass http://fedora-internal/fedora/services;` to `proxy_pass http://fedora-internal-dev/fedora/services;`

* Line 86: Add the appropriate environment suffix e.g. `-prod, -stage, -dev etc.`

    * Example: Change `proxy_pass http://fedora-internal/fedora/describe;` to `proxy_pass http://fedora-internal-dev/fedora/describe;`

* Line 92: Add the appropriate environment suffix e.g. `-prod, -stage, -dev etc.`

    * Example: Change `proxy_pass http://fedora-internal/fedora/risearch;` to `proxy_pass http://fedora-internal-dev/fedora/risearch;``

Upon completing all edits, move the resulting file to the first setup ISLE environment `proxy/config/sites-enabled` directory.

Example directory contents with multiple ISLE environment configurations:

* `edu.example.digital-collections-dev.conf`

* `edu.example.digital-collections-stage.conf`

* `edu.example.digital-collections-prod.conf`

----

#### Proxy - ssl-certs

Copy your SSL certs into the ssl-certs subdirectory.

_DO NOT OVERWRITE OR DELETE the create_dhparam_pem file within this directory._

If need be, please refer to the **SSL certificate** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

There are also additional links for the enduser to learn how to combine the SSL Certificate File with any available SSL Certificate Chain File for the `proxy` process to work properly.

* Copy your SSL certificates for the ISLE Proxy into `proxy/ssl-certs`. They will and should have different names than the examples provided below.

* There can only be 2 files involved in this process.

    * 1 x SSL Certificate Key File e.g. `newsite-sample-key.pem`
        * This file is required.
        * Please also note that the file extensions can also be: `.key` or `.pem`

    * 1 x SSL Certificate File e.g. `newsite-sample.pem`
        * This file is required.
        * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

* Open a terminal, navigate to the is subdirectory, and run the `create_dhparam_pem.sh` script in `~/ISLE/config/isle-newsite-sample/proxy/ssl-certs/` subdirectory to create the `dhparam.pem` file (_if not already._) This file is critical to the SSL encryption process and for communication between the `proxy` and `apache` containers.

    * To run this script from within this subdirectory:  `./create_dhparam_pem.sh`.

    * This will take 10 -20 mins depending on the speed of your local laptop or workstation.

#### Proxy - ssl-certs (multi)

Copy all of your SSL certificates and keys for all environments into the ssl-certs subdirectory.

The proxy directory for the first setup ISLE environment is to be used for all following environments due to the current architecture of ISLE.

As per the instructions outlined previously in the section `Apache - ssl-certs (multi)`, when creating multiple environments for ISLE, please change all of the file and key names accordingly to reflect the environment e.g. adding (`-prod, -stage, -dev` to file names). Later on, this process will assist in organizing proper filing of files for the `proxy` container and stop any situation where a file gets overwritten or improperly referenced by the wrong environment.

**Example:**

* 1 x SSL Certificate Key File e.g. `newsite-dev-key.pem`
* 1 x SSL Certificate File e.g. `newsite-dev.pem`


* 1 x SSL Certificate Key File e.g. `newsite-stage-key.pem`
* 1 x SSL Certificate File e.g. `newsite-stage.pem`


* 1 x SSL Certificate Key File e.g. `newsite-prod-key.pem`
* 1 x SSL Certificate File e.g. `newsite-prod.pem`

For all following ISLE environments, please delete the proxy directory found within each environments config parent directory.

**Example:**

* 1st ISLE environment setup: Keep the proxy directory e.g. Keep this `~/opt/ISLE/config/isle-newsite-dev/proxy`

* 2nd ISLE environment setup: Delete the proxy directory e.g. remove this `~/opt/ISLE/config/isle-newsite-stage/proxy`

* 3rd ISLE environment setup: Delete the proxy directorye.g. `remove this~/opt/ISLE/config/isle-newsite-prod/proxy`

and so on...

--------

#### Proxy - upstreams.d

Single environment steps only below:

* Copy and rename: `sample-upstreams.conf.disabled` to the appropriate domain name of your choice e.g. `yourdomain-upstreams-dev.conf` while also removing the `.disabled` suffix.

* **Example A:** Lines 10 and 14 look like this for a single environment (no additional environments to be setup). See `proxy/config/sites-enable/com.sample-newsite.conf.disabled`.

**Please note:** If only ever deploying a single ISLE environment, you could chose not to add any suffixes and ISLE should function properly. This is entirely dependent on your organizational needs.

Example: Single Environment (defaults, no edits made)

```

      upstream fedora-internal {
        server fedora:8080 fail_timeout=0;
      }

      upstream fedora-internal {
        server apache:443 fail_timeout=0;
      }

```

#### Proxy - upstreams.d (multi)

* Copy and rename: `sample-upstreams.conf.disabled` to the appropriate domain name and environment type (`dev, prod, stage, etc...`) of your choice e.g. `yourdomain-upstreams-dev.conf` while also removing the `.disabled` suffix.

* Within the file, append the environment name after all instances of "internal" proceeded by a hyphen "-environment_type" like the example below:

* **Example A:** Lines 10 and 14 look like this for a development environment that will be run alongside more ISLE environments to follow. See `proxy/config/sites-enable/com.sample-newsite.conf.disabled`.

Example: Multi Environment (dev as example)

```

      upstream fedora-internal-dev {
        server fedora:8080 fail_timeout=0;
      }

      upstream fedora-internal-dev {
        server apache:443 fail_timeout=0;
      }

```

**Please note:** You do not add additional environment code here e.g. `-prod or -stage`. Each environment has its own .conf file to manage this seperately.

Upon completing all edits, move the resulting file to the first setup ISLE environment `proxy/config/upstreams.d` directory.

Example `proxy/config/sites-enabled/` directory contents running multi-environments.

* `edu.example.digital-collections-dev.conf`

* `edu.example.digital-collections-prod.conf`

* `edu.example.digital-collections-stage.conf`


#### Proxy - docker-compose.yml (only multi)

* Within each new `docker-compose.yml` file created for each new environment:

   * Delete the `proxy` section entirely

       * delete Lines 94 `proxy:` to Line 111.

   * Uncomment Lines 118 & 119

Example:

```

# Defined networks
networks:
  isle-internal:
  isle-external:
#    external: true            # edit if running a new multi environment
#    name: isle_isle-internal  # edit if running a new multi environment

```

should now become

```
# Defined networks
networks:
  isle-internal:
  isle-external:
    external: true          
    name: isle_isle-internal

```

* (_optional_) Once you have the 1st ISLE environment running, you can double check this value for later ISLE environments by entering:

    * `docker network ls` to review current running network types.

Example:

```
NETWORK ID          NAME                                        DRIVER              SCOPE
7361197ad494        bridge                                      bridge              local
7d37894dd4ca        host                                        host                local
d4994fca8ea7        isle_isle-external                          bridge              local
22518b818c29        isle_isle-internal                          bridge              local
```


-----

### Solr directory

The `solr` subdirectory contains all specific configurations and overrides necessary for the ISLE solr image and resulting container to function properly with your changes. This is the SOLR search server that properly indexes all objects and metadata to provide speedy in-depth search functionality for the Islandora / Drupal site of the objects and collections contained within the Fedora repository.

If need be, please refer to the **Solr** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

#### Solr - solr

* (_Optional_) Within the **solr** subdirectory:

    * Make the appropriate customizations or changes to the following files as you see fit otherwise you can leave these files alone and SOLR will work out of the box.

        * `schema.xml`

        * `solr.xml`

        * `solrconfig.xml`

        * `stopwords.txt`

#### Solr - tomcat

* Within the **tomcat** subdirectory:

     * Strongly recommend adding tomcat user passwords in file: `solr/tomcat/tomcat-users.xml` on lines 45 & 48.

Example: ONLY ADD TOMCAT USERNAME/PASSWORD - can look like this but please use a stronger password with more than 10 alpha-numeric characters. Also note, they should NOT be the same password.:

```
          <!-- user manager can access only manager section -->
             <user username=“manager” password=“tomcat_manager_password" roles=“manager-gui” />  <!--enduser please add passwords / users and remove this comment -->

          <!-- user admin can access manager and admin section both -->
             <user username=“admin” password=“tomcat_admin_password” roles=“manager-gui,admin-gui” /> <!--enduser please add passwords / users and remove this comment -->

          </tomcat-users>

```

--------

## Final steps

If need be, please refer to the **Git** section of the [Glossary](../glossary.md) for relevant terms to help guide installation.

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
    `git add newsite-sample-cheatsheet.md`

    * Enter `git status` again - everything should be in green now as all modified files have been added - if anything's still red use git add and the path to add it

    * then run `git commit -m "initial config commit"` inside the double quotes is the commit message you can say whatever you want in this message - so for example if this is the config for your dev instance you could say that...

    * Enter `git push origin master`

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
