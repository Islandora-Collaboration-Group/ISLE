# Local ISLE Installation: New Site

_Expectations:  It takes an average of **60 - 120 minutes** to read this documentation and complete this installation._

This `Local` ISLE Installation creates an un-themed Drupal website and an empty Fedora repository similar to the `Demo` installation but one that you can use to develop and design your Drupal site and Fedora collections with the end goal of deploying to ISLE Staging and Production environments for public use.

You will now be able to change the ability to view locally in your browser from the Demo url `https://isle.localdomain` to a new domain of your choice for example `https://yourprojectnamehere.localdomain`.

While this installation gets you a brand new local development site, it is **not** intended as a migration process of a previously existing Islandora site. If you need to build a local environment to migrate a previously existing Islandora site, please use the [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md) instructions instead.

This document also has directions on how you can check in newly created ISLE and Islandora code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Staging and finally to Production. The [ISLE Installation: Environments](../install/install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE endusers should use for the `.env`, `local.env`, etc.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Local ISLE installation is intended for a brand new ISLE site for further Drupal theme development, ingest testing, etc. on a personal computer.

* You will be using ISLE version `1.2.0` or higher

* You are using Docker-compose `1.24.0` or higher

* You have git installed on your personal computer.

* You have access to a private git repository in [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com), etc.
    * If you do not, please contact your IT department for git resources, or else create an account with one of the above providers.
    * **WARNING:** Only use **Private** git repositories given the sensitive nature of the configuration files.
    * **DO NOT** share these git repositories publicly.

---

## Index of Instructions

* Step 1: Edit `/etc/hosts` File
* Step 2: Setup git for the ISLE project
* Step 3: Edit the `.env` File to change to the Local Environment
* Step 4: Create new users and passwords by editing `local.env`
* Step 5: Create new self-signed certs for your project
* Step 6: Download the ISLE images
* Step 7: Launch Process
* Step 8: Run Islandora Drupal site Install Script
* Step 9: Test the Site
* Step 10: Ingest Sample Objects
* Step 11: Check-in the newly created Islandora Drupal site code into a git repository

---

## Step 1: Edit `/etc/hosts` File

Enable the Local ISLE Installation to be viewed locally on personal computer browser as: e.g. `https://yourprojectnamehere.localdomain`.

* Please use these instructions to [Edit the "/etc/hosts" File](../install/install-demo-edit-hosts-file.md).

* Replace `isle` or `yourprojectnamehere` with your domain or project name: e.g. `yourprojectnamehere.localdomain`

---

## Step 2: Setup git for the ISLE project

**Please note:** The commands given below are for command line usage of git. GUI based clients such as the [SourceTree App](https://www.sourcetreeapp.com/) may be easier for endusers to use for the git process.

Within your git repository provider / hoster (e.g [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com)), create two new empty git repositories:

1. ISLE project config - e.g. `yourprojectnamehere-isle`
    * This Git repository will hold your copy of the ISLE code along with your environment-specific customizations. Storing this in a code repository and following the workflow described here will save you a lot of time and confusion.
2. Islandora Drupal site code - e.g. `yourprojectnamehere-islandora`
    * This Git repository will hold your copy of the Islandora Drupal code along with your site specific customizations. Storing this in a code repository and following the workflow described here will save you a lot of time and confusion.

The git project name can be your institution name or the name of the collections you plan to deploy; your choice entirely. A very clear distinction between the ISLE and Islandora code should be made in the repository name. Do not confuse or label Islandora Drupal site code as ISLE and vice-versa.

Clone this newly created ISLE project to your personal computer

  * Open a `terminal` (Windows: open `PowerShell`)
  * Navigate to a directory that will house your new ISLE project directory using the `cd` command.
  * `git clone https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git`

Navigate to this directory

  * `cd /path/to/your/repository`

Add the ICG ISLE git repository as a git upstream. (_the code below is a real path and command_)

  * `git remote add icg-upstream https://github.com/Islandora-Collaboration-Group/ISLE.git`
  * you can check by running this command: `git remote -v`

```bash  
icg-upstream	https://github.com/Islandora-Collaboration-Group/ISLE.git (fetch)
icg-upstream	https://github.com/Islandora-Collaboration-Group/ISLE.git (push)
origin	https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (fetch)
origin	https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (push)
```

Run a git fetch

  * `git fetch icg-upstream`

Pull down the ICG ISLE `master` branch into your local project's `master` branch

  * `git pull icg-upstream master`
  * if you `ls -lha` in this directory, you'll now have code.

Push this code to your online git provider ISLE

  * `git push -u origin master`
  * This will take 2 - 5 mins depending on your internet speed.

Now you have the current ISLE project code checked into git as foundation to make changes on specific to your local and project needs. You'll use this git "icg-upstream" process in the future to pull updates and releases from the main ISLE project.

---

## Step 3: Edit the `.env` File to change to the Local Environment

Open a `terminal` (Windows: open `PowerShell`)

* Navigate to your ISLE project directory. (You may already be in this directory if you are coming from the [Software Dependencies](../install/host-software-dependencies.md).)

* Open `.env` file by running: `nano .env`
    * _For endusers familiar with editing files on the command line, vim, emacs or alternative tools can be used in lieu of nano_

* Change only the following lines in the `.env` file so that the resulting values look like the following: **Please note: the following below is an example not actual values you should use. Use one word to describe your project and follow the conventions below accordingly**
    * `COMPOSE_PROJECT_NAME=yourprojectnamehere_local`
    * `BASE_DOMAIN=yourprojectnamehere.localdomain`
    * `CONTAINER_SHORT_ID=ld` _leave default setting of `ld` as is. Do not change._
    * `COMPOSE_FILE=docker-compose.local.yml`

* If you want to use a MySQL client with a GUI to import the Production MySQL Drupal database you'll need to uncomment the `ports` section of the MySQL service within the `docker-compose.local.yml` to open up the `3306` port. If you are already running MySQL on your personal computer, you'll have a port conflict either shutdown that service prior to running ISLE or change 3306:3306 to something like `9306:3306`. Please double-check.

* Enter `Cntrl` and the letter `o` together to write the changes to the file.

* Enter `Cntrl` and the letter `x` together to exit the file

---

## Step 4: Create new users and passwords by editing `local.env`

* Open up the `local.env` file in a text editor.
    * Find each comment that begins with: `# Replace this comment with a ...` and follow the commented instructions to edit the passwords, database and user names.
        * **Review carefully** as some comments request that you replace with `...26 alpha-numeric characters` while others request that you create an `...easy to read but short database name`.
        * In many cases the username is already pre-populated. If it doesn't have a comment directing you to change or add a value after the `=`, then don't change it.
    * Once finished, save and close the file.

* Open up the `config/apache/settings_php/settings.local.php` file in a text editor.
    * Find the first comment that begins with: `# ISLE Configuration` and follow the commented instructions to edit the database, username and password.
    * Find the second comment that begins with: `# ISLE Configuration` and follow the instructions to edit the Drupal hash salt.
    * Once finished, save and close the file.

---

## Step 5: Create new self-signed certs for your project
**This step is optional, and does not currently work on Windows.**

* Open up the `./scripts/proxy/ssl-certs/local.sh` in a text editor.
    * Follow the in-line instructions to add your project's name to the appropriate areas.
    * Once finished, save and close the file.

* _Using the same open terminal / Powershell_, navigate to `/pathto/yourprojectnamehere/scripts/proxy/ssl-certs/`
    * `cd /pathto/yourprojectnamehere/scripts/proxy/ssl-certs/`
    * `chmod +x local.sh`

* The following command will generate new self-signed SSL keys using your `yourprojectnamehere.localdomain` domain. This now secures the local site.
    * `./local.sh`
    * The generated keys can now be found in `./config/proxy/ssl-certs`

* Add the SSL .pem and .key file names generated from running `local.sh` to the `./config/proxy/traefik.local.toml` file.
    * Example:
    * `certFile = "/certs/yourprojectnamehere.localdomain.pem"`
    * `keyFile = "/certs/yourprojectnamehere.localdomain.key"`

---

## Step 6: Download the ISLE images

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_).
* _Using the same open terminal / Powershell_
    * Navigate to the root of your ISLE project
    * `cd ~/pathto/yourprojectnamehere`
    * `docker-compose pull`

## Step 7: Launch Process

* _Using the same open terminal / Powershell_
    * `docker-compose up -d`

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* After the above process is completed using the already open terminal or Powershell again.
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with `isle-` are expected to have a `STATUS` of `Up` (for x time).
      * **If any of these are not `UP`, then use [ISLE Installations: Troubleshooting](install-troubleshooting.md) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, oe (c) older exited ISLE containers that maybe should be removed. --->

---

## Step 8: Run Islandora Drupal site Install Script

We highly recommend that you first review the contents of the `docker-compose.local.yml` file as the ISLE Apache service uses bind mounts for the intended Drupal Code instead of using default Docker volumes. This allows users to perform Local Islandora Drupal site development with an IDE. This line is a suggested path and users are free to change values to the left of the `:` to match their Apache data folder of choice. However we recommend starting out with the default setting below.
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

* In your web browser, enter this URL: `https://yourprojectnamehere.localdomain`
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

## Step 11: Check-in the newly created Islandora Drupal site code into a git repository

* _Using the same open terminal / Powershell_

* Navigate to the `data` directory within your local ISLE project
    * `cd data/apache/html`

* Create a local git repository
    * `git init`

* Add all of the files to this local git repository
    * `git add .`

* Create a local git commit with an appropriate message to preserve these changes.
    * Between the `""` add a message like "Setting up Drupal site" that reflects the changes you are making in the Drupal code.
    * `git commit -m "Setting up Drupal site"`

* Add the git `remote` (this will be remote / cloud based git repository that you'll push changes to). This can be Bitbucket, Github or Gitlab.
    * For example: `git remote add origin https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-islandora.git`

* Push the changes to the remote git repository on the `master` branch
    * `git push -u origin master`

* Then continue to develop work within this follow your institutional best practice to application git repository.

---

## Next Steps

Once you are ready to deploy your finished Drupal site, you may progress to:

* [Staging ISLE Installation: New Site](../install/install-staging-new.md)

---

## Additional Resources
* [ISLE Installation: Environments](../install/install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE endusers should use for the `.env`, `local.env`, etc.
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator tools.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Local ISLE Installation: New Site
