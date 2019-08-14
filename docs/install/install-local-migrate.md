# Local ISLE Installation: Migrate Existing Islandora Site

_Expectations:  It takes an average of **60 - 120 minutes** to read this documentation and complete this installation._

This `Local` ISLE Installation builds a local environment for the express purpose of migrating a previously existing Islandora site onto the ISLE platform. If you need to build a brand new local development site, please **stop** and use the [Local ISLE Installation: New Site](../install/install-local-new.md) instructions instead.

This `Local` ISLE Installation will use a copy of a currently running Production Drupal website and an empty Fedora repository for endusers to test migration to ISLE and even for further site development or and design with the end goal of deploying to ISLE Staging and Production environments for public use. The final goal would be to cut over from the existing non-ISLE Production and Staging servers to their new ISLE counterparts.

You will now be able to change the ability to view locally in your browser from the Demo url `https://isle.localdomain` to a new domain of your choice for example `https://yourprojectnamehere.localdomain`.

This document has directions on how you can check in newly created ISLE code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Staging and finally to Production. The [ISLE Installation: Environments](../install/install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE endusers should use for the `.env`, `local.env`, etc.

This document **does not** have directions on how you can check in previously existing Drupal / Islandora code into a git repository and assumes this step has already happened. The directions below will explain how to clone Drupal / Islandora code from a previously existing Drupal / Islandora git repository that should already be accessible to you.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Local ISLE installation is intended for an existing Production Drupal site to be imported for further ISLE migration testing, Drupal theme development, ingest testing etc on a local laptop or workstation.

* Using ISLE version `1.2.0` or higher

* Using Docker-compose `1.24.0` or higher

* You have git installed on your local laptop or workstation.

* You have access to a private git repository in [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com) etc.
  * If you do not, please contact your IT department for git resources
  * If they do not have git repository resources, suggest you create an account with one of the online providers mentioned above.
  * **WARNING:** Only use **Private** git repositories given the sensitive nature of the configuration files.
  * **DO NOT** share these git repos publicly.
* You have a previously existing private Drupal / Islandora git repository

---

## Index of Instructions

* Step 0: Copy Production data to your local
* Step 1: Edit `/etc/hosts` File
* Step 2: Setup git for the ISLE project
* Step 3: Git clone the Production Drupal site code
* Step 4: Edit the `.env` File to change to the Local Environment

* Step 5: Create new users and passwords by editing `local.env`
* Step 6: Create new self-signed certs for your project
* Step 7: Download the ISLE images
* Step 8: Launch Process
* Step 9: Import the Production MySQL Drupal database
* Step 10: Run Islandora / Drupal site scripts
* Step 11: Test the Site
* Step 12: Ingest Sample Objects

---

## Step 0: Copy Production data to your local

Be sure to run a backup of any current non-ISLE systems prior to copying or exporting any files.

#### Drupal site files & code

1. Copy the `/var/www/html/sites/default/files` directory from your Production Apache server to an appropriate storage /project area on your local. You'll move this directory in later steps.

2. Locate and note the previously existing private Drupal / Islandora git repository. You'll be cloning this into place once the ISLE project has been cloned in later steps.

#### Drupal site database

Prior to attempting this step, do consider the following:

* Drupal website databases can have a multitude of names and conventions. Confer with the appropriate IT departments for your institution's database naming conventions.

* Recommended that the production databases be exported using the `.sql` /or `.gz` file formats e.g. `prod_drupal_site_082019.sql.gz` for better compression and minimal storage footprint.

* If the end user is running multi-sites, there will be additional databases to export.

* Do not export the `fedora3` database or any system tables (such as `information_schema`, `performance_schema`, `mysql`)

* If possible, on the production Apache webserver, run `drush cc all` from the command line on the production server in the `/var/www/html` directory PRIOR to any db export(s). Otherwise issues can occur on import due to all cache tables being larger than `innodb_log_file_size` allows

##### Production Drupal site database export process

* Export the MySQL database for the current Production Drupal site in use and copy it to your local in an easy to find place. In later steps you'll be directed to import this file. **Please be careful** performing any of these potential actions below as the process impacts your Production site. If you are not comfortable or familiar with performing these actions, we recommend that you instead work with your available IT resources to do so.
  * You can use a MySQL GUI client for this process or if you have command line access to the MySQL database server
  `mysqldump -u username -p database_name > prod_drupal_site_082019.sql`
  * Copy this file down to your local laptop or workstation.

#### Fedora hash size (Conditional)

While this will depend on your pre-existing Production system, it is important to double-check this. If you have a larger hash size than the default ISLE system (##), and don't follow the settings below, ISLE may not function properly when your data has been migrated.

*  If you have larger Fedora collections, there is a possibility that you made changes to the `akubra-llstore.xml` file to allow for the creation of larger or deeper hash directories)
  * You will need to copy your `/usr/local/fedora/server/config/spring/akubra-llstore.xml` from your Production Fedora System to `./config/fedora/akubra-llstore.xml`
  * You will then need to add an extra line in the Fedora service (fedora) volumes section to bind mount this file in. This will guarantee proper Fedora data hash structure.
    * `- ./config/fedora/akubra-llstore.xml:/usr/local/fedora/server/config/spring/akubra-llstore.xml`

#### Solr schema & Islandora transforms

This data can be challenging depending on the level of customizations to contend with and as such, ISLE maintainers recommends following one of the three strategies outlined below.

##### (**Easy**) - Run "stock" ISLE

Don't copy any existing production Solr schemas, Gsearch xslts etc and opt instead to use ISLE's default versions. Import some objects from your existing Fedora repository and see if they display properly in searches as you like.

##### (**Intermediate**) - Bind mount in existing transforms and schemas

Bind mount in existing transforms and schemas  to override ISLE settings with your current Production version.

**WARNING** _Assumes you are running Solr 4.10.x., **only attempt** if you are running that version on Production._

* Copy these current production files and directory to your local laptop in an appropriate location.
  * Solr `schema.xml`
  * Gsearch `foxmltoSolr.xslt` file
  * Gsearch `islandora_transforms`

* Once Step 2 has been completed and the ISLE project is cloned to your laptop and checked into git, create a new directory in:
  * `config/solr`
  * `config/fedora/gsearch`

* Move your `schema.xml` file into `config/solr/`

* Move your `foxmltoSolr.xslt` file & `islandora_transforms` directory into `config/fedora/gsearch/`

* Add a new line in the Solr volumes section of your `docker-compose.local.yml`
  * `config/solr/schema.xml:/usr/local/solr/collection1/conf/schema.xml`

* Add new lines in the Fedora volumes section of your `docker-compose.local.yml`
```bash
- ./config/fedora/gsearch/islandora_transforms:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms`
- ./config/fedora/gsearch/foxmlToSolr.xslt:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/foxmlToSolr.xslt`
```

* **Please note:** You may need to further review paths in these above files and edit them to match ISLE system paths.  

* Continue the local setup as directed below and ultimately import some objects from your existing Fedora repository and see if they display properly in searches as you like.

##### (**Advanced**) - Diff & merge current production customization edits into ISLE configs
* Copy these current production files and directory to your local laptop in an appropriate location.
  * Solr `schema.xml`
  * Gsearch `foxmltoSolr.xslt` file
  * Gsearch `islandora_transforms`

* Run the Demo ISLE briefly to pull files for modification and correct ISLE system paths.
  * You can find these paths by running the Demo and copying these files out to an appropriate location. 
    * `docker cp isle-solr-ld:/usr/local/solr/collection1/conf/schema.xml schema.xml`
    * `docker cp isle-fedora-ld:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/foxmlToSolr.xslt foxmlToSolr.xslt`
    * `docker cp isle-fedora-ld:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms islandora_transforms`

* Using a diff tool (_software that allows one to compare and find the differences between two files_), compare:
  * your Production Solr `schema.xml` file to the ISLE demo `schema.xml` file.
  * your Production Gsearch `foxmltoSolr.xslt` file to the ISLE demo `foxmltoSolr.xslt` file.
  * your Production Gsearch `islandora_transforms` directory to the ISLE demo `islandora_transforms` directory.

* Look for edits and comments that indicate specific customization and make note of the differences.
  * Merge in the customizations into the ISLE versions.

* Once Step 2 has been completed  and the ISLE project is cloned to your laptop and checked into git, create a new directory in:
  * `config/solr`
  * `config/fedora/gsearch`

* Move your newly edited `schema.xml` file into the `config/solr/` directory.

* Move your newly edited `foxmltoSolr.xslt` file & `islandora_transforms` directory into the `config/fedora/gsearch/` directory

* Add a new line in the Solr volumes section of your `docker-compose.local.yml`
  * `config/solr/schema.xml:/usr/local/solr/collection1/conf/schema.xml`

* Add new lines in the Fedora volumes section of your `docker-compose.local.yml`
```bash
- ./config/fedora/gsearch/islandora_transforms:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms
- ./config/fedora/gsearch/foxmlToSolr.xslt:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/foxmlToSolr.xslt
```

* Continue the local setup as directed below and ultimately import some objects from your existing Fedora repository and see if they display properly in searches as you like.

---

## Step 1: Edit `/etc/hosts` File

Enable the Local ISLE Installation to be viewed locally on workstation browser as: e.g. `https://yourprojectnamehere.localdomain`.

* Please use these instructions to [Edit the "/etc/hosts" File](../install/install-demo-edit-hosts-file.md).

* Replace `isle` or `yourprojectnamehere` with your domain or project name: e.g. `yourprojectnamehere.localdomain`

---

## Step 2: Setup git for the ISLE project

**Please note:** The commands given below are for command line usage of git. GUI based clients such as the [SourceTree App](https://www.sourcetreeapp.com/) may be easier for endusers to use for the git process.

* Within your git repository provider / hoster e.g [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com), create this new empty git repositories:
  1. ISLE project config - e.g. `yourprojectnamehere-isle`

The git project name can be your institution name or the name of the collections you plan to deploy; your choice entirely. A very clear distinction between the ISLE and Islandora code should be made in the repo name. Do not confuse or label Drupal / Islandora site code as ISLE and vice-versa.

* Clone this newly created ISLE project to your local laptop or workstation.
  * Open a `terminal` (Windows: open `PowerShell`)
  * Navigate to an directory that will house your new ISLE project directory using the `cd` command.
  * `git clone https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git`

* Navigate to this directory
  * `cd /path/to/your/repo`

* Add the ICG ISLE git repository as a git upstream. (_the code below is a real path and command_)
  * `git remote add icg-upstream https://github.com/Islandora-Collaboration-Group/ISLE.git`
  * you can check by running this command: `git remote -v`

```bash  
icg-upstream	https://github.com/Islandora-Collaboration-Group/ISLE.git (fetch)
icg-upstream	https://github.com/Islandora-Collaboration-Group/ISLE.git (push)
origin	https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (fetch)
origin	https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (push)
```

* Run a git fetch
  * `git fetch icg-upstream`

* Pull down the ICG ISLE `master` branch into your local project's `master` branch
  * `git pull icg-upstream master`
  * if you `ls -lha` in this directory, you'll now have code.

* Push this code to your online git provider ISLE
  * `git push -u origin master`
  * This will take 2 - 5 mins depending on your internet speed.

* Now you have the current ISLE project code checked into git as foundation to make changes on specific to your local and project needs. You'll use this git upstream `icg-upstream` and process in the future to pull updates / releases from the main ISLE project.

---

## Step 3: Git clone the Production Drupal site code

This step assumes you have an existing Drupal /Islandora site checked into a git repository.

If not then you'll need to check your Drupal site into a git repo following the same commands from [Local ISLE Installation: New Site](../install/install-local-new.md) documentation.

* Using the still open `terminal` (Windows: `PowerShell`)
  * `mkdir -p data/apache/html`
  * `git clone https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-islandora.git data/apache/html`
    * (Optional) You can chose another directory of your choice as the bind-mounts match in the Apache services volume section within the `docker-compose.local.yml` matches that other location

* Move the previously copied Production Drupal `files` directory to inside the `data/apache/html/sites/default/` directory.

---

## Step 4: Edit the `.env` File to change to the Local Environment

* Open a `terminal` (Windows: open `PowerShell`)

* Navigate to your ISLE project directory. (You may already be in this directory if you are coming from the [Software Dependencies](../install/host-software-dependencies.md).)

* Open `.env` file by running: `nano .env`
  * _For endusers familiar with editing files on the command line, vim, emacs or alternative tools can be used in lieu of nano_

* Change only the following lines in the `.env` file so that the resulting values look like the following: **Please note: the following below is an example not actual values you should use. Use one word to describe your project and follow the conventions below accordingly**
  * `COMPOSE_PROJECT_NAME=yourprojectnamehere_local`
  * `BASE_DOMAIN=yourprojectnamehere.localdomain`
  * `CONTAINER_SHORT_ID=ld` _leave default setting of `ld` as is. Do not change._
  * `COMPOSE_FILE=docker-compose.local.yml`

* If you want to use a MySQL client with a GUI to import the Production MySQL Drupal database you'll need to uncomment the `ports` section of the MySQL service within the `docker-compose.local.yml` to open up the `3306` port. If you are already running MySQL on your local laptop, you'll have a port conflict either shutdown that service prior to running ISLE or change 3306:3306 to something like `9306:3306`. Please double-check.

* Enter `Cntrl` and the letter `o` together to write the changes to the file.

* Enter `Cntrl` and the letter `x` together to exit the file

---

**Please note:** We highly recommend that you also review the contents of the `docker-compose.local.yml` file as the Apache service volume section uses bind mounts for the intended Drupal Code instead of using default Docker volumes. This allows users to perform local Drupal site development with an IDE. This line is a suggested path and users are free to change values to the left of the `:` to match their Apache data folder of choice. However we recommend starting out with the default setting below.
Default: `- ./data/apache/html:/var/www/html:cached`

* Additionally, depending on your decision from **Step 0**, you may need to make additional edits to `docker-compose.local.yml` and move files into place as directed from the (**Intermediate**) and (**Advanced**) sections.

---

## Step 5: Create new users and passwords by editing `local.env`

You can reuse some of the older Production settings in the `local.env` if you like e.g. the database name `DRUPAL_DB`, database user `DRUPAL_DB_USER` even the drupal database user password `DRUPAL_DB_PASS` if that makes it easier. You'll want to avoid repeating passwords in the ISLE Staging and Production environments however.

* Open up the `local.env` file in a text editor of your choice.

* Where ever there is a comment like:
  * `# Replace this comment with a ...` - add a value of your choice. Typically there are suggestions like `26 alpha-numeric characters` but in some cases the instructions change to `# Replace this comment with a easy to read but short database user e.g.` so **review** carefully.

* In many cases the username is already pre-populated. If it doesn't have a comment directing you to change or add a value after the `=`, then don't change it.

* Once finished, save the file and close it.

* Once you have added all of the passwords, database and user names as directed by the in-line comments of the `local.env`, there is one additional file to be updated.
  * Open up the `config/apache/settings_php/settings.local.php` file.
  * Search for the `# ISLE Configuration`
  * Follow the in-line instructions below that comment
  * Between the '', replace the `#Replace this...` comments with the appropriate values from the `local.env` file.
    * Copy the value of `DRUPAL_DB` from the local.env and paste it into the '' below for the database e.g `'database' => 'institution_db',`
    * Copy the value of `DRUPAL_DB_USER` from the local.env between the '' below for the username e.g `'username' => 'institution_db_user',`
    * Copy the value of `DRUPAL_DB_PASS` from the local.env between the '' below for the password e.g `'password' => 'yourpasswordhere',`
    * Search for `# ISLE Configuration again`
    * Follow the in-line instructions below that comment
    * Copy the value of `DRUPAL_HASH_SALT` from the local.env between the '' below for the Drupal hash salt e.g `$drupal_hash_salt = 'yourhashhere';`

* Once finished, save the file and close it.

---

## Step 6: Create new self-signed certs for your project

* Open up the `./scripts/proxy/ssl-certs/local.sh` in a text editor of your choice.

* Follow the in-line instructions to add your project's name to the appropriate areas.

* Once finished, save the file and close it.

* _Using the same open terminal / Powershell_, navigate to `/pathto/yourprojectnamehere/scripts/proxy/ssl-certs/`
  * `cd ~/pathto/yourprojectnamehere/scripts/proxy/ssl-certs/`

* `chmod +x local.sh`

* This command will generate new self-signed SSL keys using your `yourprojectnamehere.localdomain` domain. This now secures the local site.
  * `./local.sh`
  * The generated keys can now be found in `./config/proxy/ssl-certs`

* Add the SSL .pem and .key file names generated from running `local.sh` to the `./config/proxy/traefik.local.toml` file.
  * Example:
    * - `certFile = "/certs/yourprojectnamehere.localdomain.pem"`
    * - `keyFile = "/certs/yourprojectnamehere.localdomain.key"`

---

## Step 7: Download the ISLE images

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_):
  * _Using the same open terminal / Powershell_
    * Navigate to the root of your ISLE project
      * `cd ~/pathto/yourprojectnamehere`
  * `docker-compose pull`

## Step 8: Launch Process

* _Using the same open terminal / Powershell_
  * `docker-compose up -d`

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* After the above process is completed using the already open terminal or Powershell again.
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with `isle-` are expected to have a `STATUS` of `Up` (for x time).
      * **If any of these are not `UP`, then use [ISLE Installations: Troubleshooting](install-troubleshooting.md) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, oe (c) older exited ISLE containers that maybe should be removed. --->

## Step 9: Import the Production MySQL Drupal database

* Method A: Use a MySQL client with a GUI
  * Configure the client with the following:
    * Host = `127.0.0.1`
    * Port: `3306` _or a different port if you changed it_
    * Username: `root`
    * Password: `YOUR_MYSQL_ROOT_PASSWORD` in the `local.env`)
  * Select the Drupal database `DRUPAL_DB` in the `local.env`)
  * Click File > Import (_or equivalent_)
  * Select your exported Production Drupal database file e.g. `prod_drupal_site_082019.sql.gz`
  * The import process will take 1 -3 minutes depending on the size.

* Method B: Use the command line
  * Copy the Production Drupal database file e.g. `prod_drupal_site_082019.sql.gz` to your ISLE MySQL container
    * Run `docker ps` to determine the mysql container name
    * `docker cp /pathto/prod_drupal_site_082019.sql.gz yourmysql-container-name:/prod_drupal_site_082019.sql.gz`
    * This might take a few minutes depending on the size of the file.
  * Shell into the mysql container. Replace the `DRUPAL...` below with the values from the `local.env`
    * `docker exec -it yourmysql-container-name bash`
    * `mysql -u DRUPAL_DB_USER -p DRUPAL_DB < prod_drupal_site_082019.sql.gz`
    * This might take a few minutes depending on the size of the file.
    * Exit out of the container when finished.

---

## Step 10: Run Islandora / Drupal site scripts

You can determine the name of the Apache container by running `docker ps`. make note of the Apache container name, you'll need to use it for the commands below.

* Run the `migration_site_vsets.sh` script on the Apache container. This will change Drupal database site settings only for ISLE connectivity.
  * Copy the `scripts/apache/migration_site_vsets.sh` to the root of the Drupal directory on your Apache container
    * `docker cp scripts/apache/migration_site_vsets.sh yourapache-container-name:/var/www/html/migration_site_vsets.sh`
  * Change the permissions on the script to make it executable
    * `docker exec -it your-apache-containername chmod +x /var/www/html/migration_site_vsets.sh`
  * Run the script
      * `docker exec -it your-apache-containername bash /var/www/html/migration_site_vsets.sh`

* Since you've imported an existing Drupal database, you'll need to reinstall the Islandora solution packs so the Fedora repository will be ready to ingest objects.
  * Copy the `scripts/apache/install_solution_packs.sh` to the root of the Drupal directory on your Apache container
    * `docker cp scripts/apache/install_solution_packs.sh yourapache-container-name:/var/www/html/install_solution_packs.sh`
  * Change the permissions on the script to make it executable
    * `docker exec -it your-apache-containername chmod +x /var/www/html/install_solution_packs.sh`
  * Run the script
      * `docker exec -it your-apache-containername bash /var/www/html/install_solution_packs.sh`  
      * This process will take a few minutes depending on the speed of your local and Internet connection.

```
| For Windows Users only |
| :-------------      |
| You may be prompted by Windows to: |
| - Share the C drive with Docker.  Click Okay or Allow.|
| - Enter your username and password. Do this.|
| - Allow vpnkit.exe to communicate with the network.  Click Okay or Allow (accept default selection).|
| - If the process seems to halt, check the taskbar for background windows.|


* You should see a lot of green [ok] messages.
* If the script appears to pause or prompt for `y/n`, DO NOT enter any values; the script will automatically answer for you.
* **Proceed only after this message appears:** `Clearing Drupal Caches. 'all' cache was cleared.`
```

---

## Step 11: Test the Site

* In your web browser, enter this URL: `https://yourprojectnamehere.localdomain`
<!--- TODO: Add error message and how to proceed (click 'Advanced...') --->
* Note: You may see an SSL error warning that the site is unsafe. It is safe, it simply uses "self-signed" SSL certs. Ignore the error and proceed to the site.
* Log in to the local Islandora site with the credentials you created in `local.env` (`DRUPAL_ADMIN_USER` and `DRUPAL_ADMIN_PASS`)
  * You can also attempt to use login credentials that the Production server would have stored in its database.
* If the newly created Drupal login doesn't work then, you'll need to:
  * Shell into the Apache container:
    * `docker exec -it your-apache-containername bash`
  * `cd /var/www/html`
  * Create the user found in `DRUPAL_ADMIN_USER` and set its password to the value of `DRUPAL_ADMIN_PASS` as you previously created in `local.env`. In the example below swap-out `DRUPAL_ADMIN_USER` & `DRUPAL_ADMIN_PASS` with those found in `local.env`
    * `drush user-create DRUPAL_ADMIN_USER --mail="youremailaddresshere" --password="DRUPAL_ADMIN_PASS";`
    * `drush user-add-role "administrator" DRUPAL_ADMIN_USER`
    * exit the container
    * Attempt to login again

---

## Step 12: Ingest Sample Objects

It is recommended that endusers migrating their sites opt to either import sample objects from their non-ISLE Production Fedora servers or use the following below:

The Islandora Collaboration Group provides a set of [Islandora Sample Objects](https://github.com/Islandora-Collaboration-Group/islandora-sample-objects) with corresponding metadata for testing Islandora's ingest process. These sample objects are organized by solution pack and are zipped for faster bulk ingestion.

* To download these sample objects, clone them to your computer's desktop:
```
git clone https://github.com/Islandora-Collaboration-Group/islandora-sample-objects.git
```

* Follow these ingestion instructions [How to Add an Item to a Digital Collection](https://wiki.duraspace.org/display/ISLANDORA/How+to+Add+an+Item+to+a+Digital+Collection)
* (Note: [Getting Started with Islandora](https://wiki.duraspace.org/display/ISLANDORA/Getting+Started+with+Islandora) contains explanations about content models, collections, and datastreams.)
* After ingesting content, you may need to add an Islandora Simple Search block to the Drupal structure. (The default search box will only search Drupal content, not Islandora content.) This might already exist in your current Drupal Production site as a feature.
    * Select from the menu: `Structure > Blocks > Islandora Simple Search`
    * Select: `Sidebar Second`
    * Click: `Save Blocks` at bottom of page
    * You may now search for ingested objects that have been indexed by SOLR

* After ingesting either the ICG sample objects or a selection of your pre-existing Fedora Production objects, continue to QC the migrated site, ensuring that objects display properly, the theme and design continue to work properly, there are no errors in the Drupal watchdog and everything matches the functionality of the previous non-ISLE Production Islandora Drupal site.

---

## Next Steps

Once you are ready to deploy your finished Drupal site, you may progress to:

* [Staging ISLE Installation: Migrate Existing Islandora Site](../install/install-staging-migrate.md)

---

## Additional Resources
* [ISLE Installation: Environments](../install/install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE endusers should use for the `.env`, `local.env`, etc.
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator tools.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Local ISLE Installation: Migrate Existing Islandora Site
