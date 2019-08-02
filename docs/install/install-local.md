# Local ISLE Installation - New site

_Expectations:  It takes an average of **60 - 120 minutes** to read this documentation and complete this installation._

This `Local` ISLE Installation creates an un-themed Drupal website and an empty Fedora repository similar to the `Demo` installation but one that you can use to develop and design your Drupal site and Fedora collections with the end goal of deploying to ISLE Staging and Production environments for public use.

You will now be able to change the ability to view locally in your browser from the Demo url `https://isle.localdomain` to a new domain of your choice for example `https://yourprojectnamehere.localdomain`.

While this installation gets you a brand new local development site, it is **not** intended as a migration process of a previously existing Islandora site. If you need to build a local environment to migrate a previously existing Islandora site, please use the [Local ISLE Installation - Migrate existing site](install-local-migrate.md) instructions instead.

This document also has directions on how you can check in newly created ISLE & Islandora code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Staging and finally to Production.

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

---

## Step 8: Run Islandora / Drupal site Install Script

We highly recommend that you first review the contents of the `docker-compose.local.yml` file as `line 26` uses bind mounts for the intended Drupal Code instead of using default Docker volumes. This allows users to perform local Drupal site development with an IDE. This line is a suggested path and users are free to change values to the left of the `:` to match their Apache data folder of choice. However we recommend starting out with the default setting below.
Default: `- ./data/apache/html:/var/www/html:cached`

Initially when starting up, end-users will need to run the install script and then ultimately check the resulting Drupal code into a git repository. If you are familiar with that process then Step 9.

This process may take 10 - 20 minutes (_depending on system and internet speeds_)

* Run the install site script on the Apache container by copying and pasting this command:
```
docker exec -it isle-apache-ld bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh
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
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator tools.
* [Local ISLE Installation: Troubleshooting](../install/install-local-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Local ISLE Installation
