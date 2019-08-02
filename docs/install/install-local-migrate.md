# Local ISLE Installation - Migrate your existing Islandora site

_Expectations:  It takes an average of **60 - 120 minutes** to read this documentation and complete this installation._

This `Local` ISLE Installation builds a local environment for the express purpose of migrating a previously existing Islandora site onto the ISLE platform. If you need to build a brand new local development site, please **stop** and use the [Local ISLE Installation - New site](install-local-new.md) instructions instead.

This `Local` ISLE Installation will use a copy of a currently running Production Drupal website and an empty Fedora repository for endusers to test migration to ISLE and even for further site development or and design with the end goal of deploying to ISLE Staging and Production environments for public use. The final goal would be to cut over from the existing non-ISLE Production and Staging servers to their new ISLE counterparts.

You will now be able to change the ability to view locally in your browser from the Demo url `https://isle.localdomain` to a new domain of your choice for example `https://yourprojectnamehere.localdomain`.

This document has directions on how you can check in newly created ISLE code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Staging and finally to Production. The [ISLE Installation - Environments](install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE endusers should use for the `.env`, `local.env`, etc.

This document **does not** have directions on how you can check in previously existing Drupal / Islandora code into a git repository and assumes this step has already happened. The directions below will explain how to clone Drupal / Islandora code from a previously existing Drupal / Islandora git repository that should already be accessible to you.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Local ISLE Installation is intended for a local laptop workstation.
* This Local ISLE installation is intended for a brand new ISLE site for further Drupal theme development, ingest testing etc.
* Using ISLE version `1.2.0` or higher
* You have git installed on your local laptop or workstation.
* You have access to a private git repository in [Github](github.com), [Bitbucket](bitbucket.org), [Gitlab](gitlab.com) etc.
  * If you do not, please contact your IT department for git resources
  * If they do not have git repository resources, suggest you create an account with one of the online providers mentioned above.
  * **WARNING:** Only use **Private** git repositories given the sensitive nature of the configuration files.
  * **DO NOT** share these git repos publicly.
* You have a previously existing private Drupal / Islandora git repository

---

## Step 0. Copy Production data to your local

Be sure to run a backup of any current non-ISLE systems prior to copying or exporting any files.

#### Drupal site files & code

1. Copy the `/var/www/html/sites/default/files` directory from your Production Apache server to an appropriate storage /project area on your local. You'll move this directory in later steps.

2. Locate and note the previously existing private Drupal / Islandora git repository. You'll be cloning this into place once the ISLE project has been cloned in later steps.

#### Drupal site database

* Export the MySQL database for the current Production Drupal site in use and copy it to your local in an easy to find place. In later steps you'll be directed to import this file.
  * Drupal website databases can have a multitude of names and conventions. Confer with the appropriate IT departments for your institution's database naming conventions.
  * Recommended that the production databases be exported using the `.sql` and `.gz` file formats e.g. `prod_drupal_site_2018.sql.gz` for better compression and minimal storage footprint.
  * If the end user is running multi-sites, there will be additional databases to export.
  * Do not export the `fedora3` database
  * If possible, on the production Apache webserver, run `drush cc all` from the command line on the production server in the `/var/www/html` directory PRIOR to any db export(s). Otherwise issues can occur on import due to all cache tables being larger than `innodb_log_file_size` allows

#### Solr schema & Islandora transforms

This data can be tricky to contend with and as such, ISLE maintainers recommend the following strategies in this area.

* (Easiest, minimal effort) - Run "stock" ISLE. 
  * Don't copy any schemas, xslts etc and opt to use ISLE's selection of schemas xslts and other default configurations. Import some samples objects from your existing Fedora repository and see if objects display properly in searches as you like.

* (Easy but requires changes & testing) - Bind mount in existing transforms and schemas.
  * Copy your current production Solr schema.xml to your local laptop
  * once the ISLE project is cloned to your laptop and checked into git, create a new directory in 
  * Add a new line after **Line 88** to your `docker-compose.local.yml`
    * config/solr/schema.xml

* **Local - Migration** - Step 0. Data needs and gathering
  * apache files and git code
  * transforms - 3 methods
  * Copy all from your current non-ISLE Islandora production server

---

## Step 1: Edit `/etc/hosts` File

Enable the Local ISLE Installation to be viewed locally on workstation browser as: e.g. `https://yourprojectnamehere.localdomain`.

* Please use these instructions to [Edit the "/etc/hosts" File](../install/install-Local-edit-hosts-file.md).

* Replace `isle` or `yourprojectnamehere` with your domain / project name e.g. `yourprojectnamehere.localdomain`

---

## Step 2: Setup git for the ISLE project

**Please note:** The commands given below are for command line usage of git. GUI based clients such as the [SourceTree App](https://www.sourcetreeapp.com/) may be easier for endusers to use for the git process.

* Within your git repository provider / hoster e.g [Github](github.com), [Bitbucket](bitbucket.org), [Gitlab](gitlab.com), create two new empty git repositories:
  1. ISLE project config - e.g. `yourprojectnamehere-isle`
  2. Drupal / Islandora site code - e.g. `yourprojectnamehere-islandora`

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
origin	git@yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (fetch)
origin	git@yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (push)
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

## Step clone Production site to /data/apache/html

* **Local - Migration** - might have pre-existing git repos. Find it and clone to 
  * mkdir -p data/apache/html
  * git clone yourprojectnamehere.git data/apache/html or at another directory of your choice (as long line 131 of the `docker-compose.local.yml` matches the other location)
  * Assumes you have site in Git PRIOR to if not then you'll need to check your Drupal site into a git repo following the same commands from the local new site.

---

## Step 3: Edit the `.env` File to change to the Local Environment

* Open a `terminal` (Windows: open `PowerShell`)

* Navigate to your ISLE project directory. (You may already be in this directory if you are coming from the [Software Dependencies](../install/host-software-dependencies.md).)

* Open `.env` file by running: `nano .env`
  * _For endusers familiar with editing files on the command line, vim, emacs or alternative tools can be used in lieu of nano_

* Change only the following lines in the `.env` file so that the resulting values look like the following: **Please note: the following below is an example not actual values you should use. Use one word to describe your project and follow the conventions below accordingly**
  * **Line 9 -** `COMPOSE_PROJECT_NAME=yourprojectnamehere_local`
  * **Line 10 -** `BASE_DOMAIN=yourprojectnamehere.localdomain`
  * **Line 11 -** _leave default setting of `ld` as is. Do not change._
  * **Line 12 -** `COMPOSE_FILE=docker-compose.local.yml`

* Enter `Cntrl` and the letter `o` together to write the changes to the file.

* Enter `Cntrl` and the letter `x` together to exit the file

---

## Step 4: Create new users and passwords by editing `local.env`

* Open up the `local.env` file in a text editor of your choice.

* Where ever there is a comment like:
  * `# Replace this comment with a ...` - add a value of your choice. Typically there are suggestions like `26 alpha-numeric characters` but in some cases the instructions change to `# Replace this comment with a easy to read but short database user e.g.` so **review** carefully.

* In many cases the username is already pre-populated. If it doesn't have a comment directing you to change or add a value after the `=`, then don't change it.

* Once finished, save the file and close it.

* Once you have added all of the passwords, database and user names as directed by the in-line comments of the `local.env`, there is one additional file to be updated.
  * Open up the `config/apache/settings_php/settings.local.php` file.
  * Between the '', replace the `#Replace this...` comments with the appropriate values from the `local.env` file.
    * **Line 251** - 'database' => '#Replace this with the value of Line 23 (DRUPAL_DB) in the local.env'
    * **Line 252** - 'username' => '#Replace this with the value of Line 26 (DRUPAL_DB_USER) in the local.env'
    * **Line 253** - 'password' => '#Replace this with the value of Line 29 (DRUPAL_DB_PASS) in the local.env'
     * **Line 290** - 'password' => '#Replace this with the value of Line 62 (DRUPAL_HASH_SALT) in the local.env'

* Once finished, save the file and close it.

---

## Step 5: Create new self-signed certs for your project

* Open up the `scripts/proxy/ssl-certs/local.sh` in a text editor of your choice.

* Follow the in-line instructions to add your project's name to the appropriate areas.

* Once finished, save the file and close it.

* _Using the same open terminal / Powershell_, navigate to `/pathto/yourprojectname-here/scripts/proxy/ssl-certs/`

* `chmod +x local.sh`

* This command will generate new self-signed SSL keys using your `yourprojectname-here.localdomain` domain. This now secures the local site.
  * `./local.sh`
  * The generated keys can now be found in `proxy/ssl-certs`

* Add the SSL .pem and .key file names generated from running `local.sh` to the `proxy/traefik.local.toml` file on lines 27 & 28:
  * Example:
    * **Line 27** - `certFile = "/certs/yourprojectname-here.localdomain.pem"`
    * **Line 28** - `keyFile = "/certs/yourprojectname-here.localdomain.key"`

---

## Step 6: Download the ISLE images

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_):
  * _Using the same open terminal / Powershell_
  * `docker-compose pull`

## Step 7: Launch Process

* _Using the same open terminal / Powershell_
  * `docker-compose up -d`

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* After the above process is completed using the already open terminal or Powershell again.
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with `isle-` are expected to have a `STATUS` of `Up` (for x time).
      * **If any of these are not `UP`, then use [Local ISLE Installation: Troubleshooting](../install/install-Local-troubleshooting.md) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, oe (c) older exited ISLE containers that maybe should be removed. --->

## Step 8 Import mysql database

* Use a GUI - steps
* Use the command line - steps

---

## Step 

* **Local - Migration** - might need to include a drush script to change user information. 

---

## Step 8: Run Islandora / Drupal site Install Script

We highly recommend that you first review the contents of the `docker-compose.local.yml` file as `line 26` uses bind mounts for the intended Drupal Code instead of using default Docker volumes. This allows users to perform local Drupal site development with an IDE. This line is a suggested path and users are free to change values to the left of the `:` to match their Apache data folder of choice. However we recommend starting out with the default setting below.
Default: `- ./data/apache/html:/var/www/html:cached`

This process may take 10 - 20 minutes (_depending on system and internet speeds_)

  * **Local - Migration** - Run migration scripts

    * migration vsets
    * re-install fedora content pack setup

* You can determine the name of the Apache container by running `docker ps`. make note of the Apache container name, you'll need to use it for the commands below.

* Run the migration vsets site script on the Apache container by copying and pasting this command:
```
docker exec -it your-Apache-containername  bash path to script
```

* Run the re-install fedora content pack setup script on the Apache container by copying and pasting this command:
```
docker exec -it your-Apache-containername bash path to script
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

---

## Step 9: Test the Site

* In your web browser, enter this URL: `https://yourprojectname-here.localdomain`
<!--- TODO: Add error message and how to proceed (click 'Advanced...') --->
* Note: You may see an SSL error warning that the site is unsafe. It is safe, it simply uses "self-signed" SSL certs. Ignore the error and proceed to the site.
* Log in to the local Islandora site with the credentials you created in `local.env` (`DRUPAL_ADMIN_USER` and `DRUPAL_ADMIN_PASS`)
  * You can also attempt to use login credentials that the Production server would have stored in its database.
* If the newly created Drupal login doesn't work then, you'll need to:
  * Shell into the Apache container: 
    * `docker exec -it your-Apache-containername bash`
  * `cd /var/www/html`
  * Create the user found in `DRUPAL_ADMIN_USER` and set its password to the value of `DRUPAL_ADMIN_PASS` as you previously created in `local.env`. In the example below swap-out `DRUPAL_ADMIN_USER` & `DRUPAL_ADMIN_PASS` with those found in `local.env`
    * `drush user-create DRUPAL_ADMIN_USER --mail="youremailaddresshere" --password="DRUPAL_ADMIN_PASS";`
    * `drush user-add-role "administrator" DRUPAL_ADMIN_USER`
    * exit the container
    * Attempt to login again

---

## Step 10: Ingest Sample Objects

The Islandora Collaboration Group provides a set of [Islandora Sample Objects](https://github.com/Islandora-Collaboration-Group/islandora-sample-objects) with corresponding metadata for testing Islandora's ingest process. These sample objects are organized by solution pack and are zipped for faster bulk ingestion.

* To download these sample objects, clone them to your computer's desktop:
```
git clone https://github.com/Islandora-Collaboration-Group/islandora-sample-objects.git
```

* Follow these ingestion instructions [How to Add an Item to a Digital Collection](https://wiki.duraspace.org/display/ISLANDORA/How+to+Add+an+Item+to+a+Digital+Collection)
* (Note: [Getting Started with Islandora](https://wiki.duraspace.org/display/ISLANDORA/Getting+Started+with+Islandora) contains explanations about content models, collections, and datastreams.)
* After ingesting content, you will need to add an Islandora Simple Search block to the Drupal structure. (The default search box will only search Drupal content, not Islandora content.)
    * Select from the menu: `Structure > Blocks > Islandora Simple Search`
    * Select: `Sidebar Second`
    * Click: `Save Blocks` at bottom of page
    * You may now search for ingested objects that have been indexed by SOLR

## Step 11: Check-in the newly created Drupal / Islandora site code into a git repository

* _Using the same open terminal / Powershell_

* Navigate to the `data` directory within your local ISLE project
  * `cd data/apache/html`

* Create a local git repository
  * `git init`

* Add all of the files to this local git repository
  * `git add .`

* Create a local git commit with an appropriate message to preserve these changes.
  * `git commit -m "message here"`
    * Between the `""` add a message like "Setting up Drupal site" that reflects the changes you are making in the Drupal code.
    * `git commit -m "Setting up Drupal site"`

* Add the git `remote` (this will be remote / cloud based git repository that you'll push changes to) This can be Bitbucket, Github or Gitlab.
  * For example: `git remote add origin git@yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-islandora.git`

* Push the changes to the remote git repository on the `master` branch
  * `git push -u origin master`

* Then continue to develop work within this follow your institutional best practice to application git repository

---

* Once you are ready to deploy your finished Drupal site, you can move onto the [Staging ISLE Installation - New site](install-staging.md) instructions.

---

## Additional Resources
* [ISLE Installation - Environments](install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE endusers should use for the `.env`, `local.env`, etc.
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator tools.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Local ISLE Installation
