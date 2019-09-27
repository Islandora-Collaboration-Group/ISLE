# Local ISLE Installation: New Site

_Expectations:  It takes an average of **1-2 hours** to read this documentation and complete this installation._

This Local ISLE Installation creates an un-themed Drupal website and an empty Fedora repository (similar to the Demo installation) but one that you can use to develop and design your Drupal site and Fedora collections with the end goal of deploying to ISLE Staging and Production environments for public use.

This Local ISLE Installation will allow you to locally view this site in your browser with the domain of your choice (**Example:** "https://yourprojectnamehere.localdomain"), instead of being constrained to the Demo URL ("https://isle.localdomain").

While this installation provides you a brand new local development site, it is **not** intended as a migration process of a previously existing Islandora site. If you need to build a local environment to migrate a previously existing Islandora site, please use the [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md) instead.

This document also has directions on how you can save newly created ISLE and Islandora code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Staging to Production. The [ISLE Installation: Environments](../install/install-environments.md) helps explain the ISLE workflow structure, the associated files, and what values ISLE endusers should use for the ".env", "local.env", etc.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Local ISLE installation is intended for a brand new ISLE site for further Drupal theme development, ingest testing, etc. on a personal computer.

* You will be using ISLE version **1.2.0** or higher.

* You are using Docker-compose **1.24.0** or higher.

* You have git installed on your personal computer.

* You have access to a private git repository in [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com), etc.
    * If you do not, please contact your IT department for git resources, or else create an account with one of the above providers.
    * **WARNING:** Only use **Private** git repositories given the sensitive nature of the configuration files. **DO NOT** share these git repositories publicly.

* **For Microsoft Windows:**
    * You have installed [Git for Windows](../install/host-software-dependencies.md#windows) and will use its provided "Git Bash" as your command line interface; this behaves similarly to LINUX and UNIX environments. Git for Windows also installs "openssl.exe" which will be needed to generate self-signed SSL certs. (Note: Powershell is not recommended as it is unable to run UNIX commands or execute bash scripts without a moderate degree of customization.)
    * Set your text editor to use UNIX style line endings for files. (Text files created on DOS/Windows machines have different line endings than files created on Unix/Linux. DOS uses carriage return and line feed ("\r\n") as a line ending, which Unix uses just line feed ("\n").)

---

## Index of Instructions

* Step 1: Choose a Project Name
* Step 1.5: Edit "/etc/hosts" File
* Step 2: Setup Git for the ISLE Project
* Step 3: Edit the ".env" File to Point to the Local Environment
* Step 4: Create New Users and Passwords by Editing "local.env" File
* Step 5: Create New Self-Signed Certs for Your Project
* Step 6: Download the ISLE Images
* Step 7: Launch Process
* Step 8: Run Islandora Drupal Site Install Script
* Step 9: Test the Site
* Step 10: Ingest Sample Objects
* Step 11: Check-In the Newly Created Islandora Drupal Site Code Into a Git Repository

---

## Step 1: Choose a Project Name

Please choose a project name (concatenated, with no spaces) that describes your institution or your collection platform. You will substitute in your preferred project name whenever the documentation refers to "yourprojectnamehere". (Be creative. Some real-life examples include: arminda, dhinitiative, digital, digitalcollections, digitallibrary, unbound, etc.)

---

## Step 1.5: Edit "/etc/hosts" File

Enable the Local ISLE Installation to be viewed locally on a personal computer browser using "yourprojectnamehere" (e.g. "https://yourprojectnamehere.localdomain").

* Please use these instructions to [Edit the "/etc/hosts" File](../install/install-demo-edit-hosts-file.md).

---

## Step 2: Setup Git for the ISLE Project

**Please note:** The commands given below are for command line usage of git. (GUI based clients such as the [SourceTree App](https://www.sourcetreeapp.com/) may be easier for endusers to use for the git process.)

Each "suggested git repository name" (below) serves to clearly name and distinguish your ISLE code from your Islandora code. It's very important to understand that these are two separate code repositories, and not to confuse them.

Create the following two new, empty, private git repositories within your git repository hosting service (e.g [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com)):

1. ISLE project configuration repository
    * **Suggested git repository name:** `yourprojectnamehere-isle`
    * This Git repository will hold your copy of the ISLE code along with your environment-specific customizations. Storing this in a private repository and following the workflow below will save you a lot of time and confusion.
2. Islandora Drupal code repository
    * **Suggested git repository name:** `yourprojectnamehere-islandora`
    * This Git repository will hold your copy of the Islandora Drupal code along with your site specific customizations. Storing this in a private repository and following the workflow below will save you a lot of time and confusion.

These steps will show you how to clone this newly created ISLE project (from your git repository hosting service) to your personal computer:

* Open a `terminal` (Windows: open `Git Bash`)

* Use the "cd" command to navigate to a directory that will become your new ISLE project directory. We recommend using the default user home directory. (You may choose a different location, but it must not be a protected folder such as system or root directory.)
    * **Example (Mac):** `cd ~`
    * **Example (Windows):** `cd /c/Users/somebody/`

* Clone your new ISLE project to your personal computer:
    * **Example:** `git clone https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git`

* Navigate within the directory created by the above clone operation:

    * `cd ISLE`

* Add the ICG ISLE git repository as a git upstream:

    * `git remote add icg-upstream https://github.com/Islandora-Collaboration-Group/ISLE.git`

* View your remotes:

    * `git remote -v`

* You should now see the following:

```bash  
icg-upstream	https://github.com/Islandora-Collaboration-Group/ISLE.git (fetch)
icg-upstream	https://github.com/Islandora-Collaboration-Group/ISLE.git (push)
origin	https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (fetch)
origin	https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git (push)
```

* Run a git fetch

    * `git fetch icg-upstream`

* Pull down the ICG ISLE "master" branch into your local project's "master" branch

    * `git pull icg-upstream master`

* View the ISLE code you now have in this directory:

    * `ls -lha`

* Push this code to your online git provider ISLE

    * `git push -u origin master`
    * This will take 2-5 minutes depending on your internet speed.

Now you have the current ISLE project code checked into git as a foundation to make changes on specific to your local and project needs. You'll use this git "icg-upstream" process in the future to pull updates and releases from the main ISLE project.

---

## Step 3: Edit the ".env" File to Point to the Local Environment

* Navigate to your ISLE project directory.

* Open the ".env" file in a text editor.

* Change only the following lines in the ".env" file so that the resulting values look like the following: **Please note: the following below is an example not actual values you should use. Use one word to describe your project and follow the conventions below accordingly**
    * `COMPOSE_PROJECT_NAME=yourprojectnamehere_local`
    * `BASE_DOMAIN=yourprojectnamehere.localdomain`
    * `CONTAINER_SHORT_ID=ld` _leave default setting of `ld` as is. Do not change._
    * `COMPOSE_FILE=docker-compose.local.yml`

* Save and close the file.

**Please note:** If you want to use a MySQL client with a GUI to import the Production MySQL Drupal database you'll need to uncomment the `ports` section of the MySQL service within the `docker-compose.local.yml` to make it so you can access port `3306` from the host computer. If you are already running MySQL on your personal computer, you'll have a port conflict and will need to either shutdown that service prior to running ISLE or change `3306:3306` to something like `9306:3306`. Please double-check.

---

## Step 4: Create New Users and Passwords by Editing "local.env" File

* Open the "local.env" file in a text editor.
    * Find each comment that begins with: `# Replace this comment with a ...` and follow the commented instructions to edit the passwords, database and user names.
        * **Review carefully** as some comments request that you replace with `...26 alpha-numeric characters` while others request that you create an `...easy to read but short database name`.
        * In many cases the username is already pre-populated. If it doesn't have a comment directing you to change or add a value after the `=`, then don't change it.
    * Once finished, save and close the file.

* Open the "config/apache/settings_php/settings.local.php" file in a text editor.
    * Find the first comment that begins with: `# ISLE Configuration` and follow the commented instructions to edit the database, username and password.
    * Find the second comment that begins with: `# ISLE Configuration` and follow the instructions to edit the Drupal hash salt.

* **For Microsoft Windows:**
    * Find the following line:
        * `# COMPOSE_CONVERT_WINDOWS_PATHS=1`
    * In the above line, delete the first two characters (`# `) so as to uncomment the line. It should now look like this:
        * `COMPOSE_CONVERT_WINDOWS_PATHS=1`

* Once finished, save and close the file.

---

## Step 5: Create New Self-Signed Certs for Your Project

* Open the appropriate file in a text editor:
    * **For Mac/Ubuntu/CentOS/etc:** "./scripts/proxy/ssl-certs/local.sh"
    * **For Microsoft Windows:** "./scripts/proxy/ssl-certs/local-windows-only.sh"

* Follow the in-line instructions to add your project's name to the appropriate areas.
    * Once finished, save and close the file.

* _Using the same open terminal:_, navigate to "/pathto/yourprojectnamehere/scripts/proxy/ssl-certs/"
    * `cd ./scripts/proxy/ssl-certs/`

* Change the permissions on the script to make it executable
    * **For Mac/Ubuntu/CentOS/etc:** `chmod +x local.sh`
    * **For Microsoft Windows:** `chmod +x local-windows-only.sh`

* Run the following command to generate new self-signed SSL keys using your "yourprojectnamehere.localdomain" domain. This now secures the local site.
    * **For Mac/Ubuntu/CentOS/etc:** `./local.sh`
    * **For Microsoft Windows:** `./local-windows-only.sh`
    * The generated keys can now be found in:
        * `cd ../../../config/proxy/ssl-certs`

* Add the SSL .pem and .key file names generated from running the above script to the `./config/proxy/traefik.local.toml` file.
    * `cd ..`
    * Open `traefik.local.toml` in a text editor.
    * **Example:**
        * `certFile = "/certs/yourprojectnamehere.localdomain.pem"`
        * `keyFile = "/certs/yourprojectnamehere.localdomain.key"`

---

## Step 6: Download the ISLE Images

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_).
* _Using the same open terminal:_
    * Navigate to the root of your ISLE project
    * `cd ~/pathto/yourprojectnamehere`
    * `docker-compose pull`

## Step 7: Launch Process

_Using the same open terminal:_

* Run the docker containers:
    * `docker-compose up -d`

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* _Using the same open terminal:_
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with "isle-" are expected to have a "STATUS" of "Up" (for x time).
      * **If any of these are not "UP", then use [Non-Running Docker Containers](../install/install-troubleshooting.md#non-running-docker-containers) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, oe (c) older exited ISLE containers that maybe should be removed. --->

---

## Step 8: Run Islandora Drupal Site Install Script

We highly recommend that you first review the contents of the "docker-compose.local.yml" file as the ISLE Apache service uses bind mounts for the intended Drupal Code instead of using default Docker volumes. This allows users to perform Local Islandora Drupal site development with an IDE. This line is a suggested path and users are free to change values to the left of the `:` to match their Apache data folder of choice. We recommend starting out with the default setting: "- ./data/apache/html:/var/www/html:cached"

Run the install site script on the Apache container by copying and pasting the appropriate command:

* **For Mac/Ubuntu/CentOS/etc:** `docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh"`
* **For Microsoft Windows:** `winpty docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh"`

The above process may take 10-20 minutes (_depending on system and internet speeds_)

* You should see a lot of green [ok] messages.
* If the script appears to pause or prompt for "y/n", DO NOT enter any values; the script will automatically answer for you.


| For Microsoft Windows: |
| :-------------      |
| You may be prompted by Windows to: |
| - Share the C drive with Docker.  Click Okay or Allow.|
| - Enter your username and password. Do this.|
| - Allow vpnkit.exe to communicate with the network.  Click Okay or Allow (accept default selection).|
| - If the process seems to halt, check the taskbar for background windows.|

* **Proceed only after this message appears:** "Done. 'all' cache was cleared."

---

## Step 9: Test the Site

* In your web browser, enter this URL: `https://yourprojectnamehere.localdomain`
<!--- TODO: Add error message and how to proceed (click 'Advanced...') --->
* Note: You may see an SSL error warning that the site is unsafe. It is safe, it simply uses "self-signed" SSL certs. Ignore the error and proceed to the site.
* Log in to the local Islandora site with the credentials ("DRUPAL_ADMIN_USER" and "DRUPAL_ADMIN_PASS") you created in "local.env".

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

## Step 11: Check-In the Newly Created Islandora Drupal Site Code Into is Git Repository

*_Using to where same open terminal to where Bitbucket, Github or Git:    )* `cd data/apache/html`
* Create a local git repository:
    * `git init`

* Add all of the files to this Local git repository:
    * `git add .`

* Commit these files to your Local environment with an appropriate message to preserve these changes:
    * `git commit -m "Setting up Drupal site"`

* Add the git "remote" (this is your git repository hosting service to where you'll push changes, such as Bitbucket, Github or Gitlab):
    * **Example:** `git remote add origin https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-islandora.git`

* Push the changes to the remote git repository on the "master" branch
    * `git push -u origin master`

* Your Local install is now complete.

---

## Next Steps

Once you are ready to deploy your finished Drupal site, you may progress to:

* [Staging ISLE Installation: New Site](../install/install-staging-new.md)

---

## Additional Resources
* [ISLE Installation: Environments](../install/install-environments.md) helps explain the ISLE workflow structure, the associated files, and what values ISLE endusers should use for the ".env", "local.env", etc.
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator testing.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Local ISLE Installation: New Site
