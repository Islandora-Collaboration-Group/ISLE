# Local ISLE Installation: Migrate Existing Islandora Site

_Expectations:  It takes an average of **2-4+ hours** to read this documentation and complete this installation._

This Local ISLE Installation builds a local environment for the express purpose of migrating a previously existing Islandora site onto the ISLE platform. If you need to build a brand new local development site, please **stop** and use the [Local ISLE Installation: New Site](../install/install-local-new.md) instead.

This Local ISLE Installation will use a copy of a currently running Production Islandora Drupal website and an empty Fedora repository for endusers to test migration to ISLE and do site development and design with the end goal of deploying to ISLE Staging and Production environments for public use. The final goal would be to cut over from the existing non-ISLE Production and Staging servers to their new ISLE counterparts.

This Local ISLE Installation will allow you to locally view this site in your browser with the domain of your choice (**Example:** "https://yourprojectnamehere.localdomain"), instead of being constrained to the Demo URL ("https://isle.localdomain").

This document has directions on how you can save newly created ISLE code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Staging to Production. The [ISLE Installation: Environments](../install/install-environments.md) documentation offers an overview of the ISLE structure, the associated files, and what values ISLE endusers should use for the ".env", "local.env", etc.

This document **does not** have directions on how you can save previously existing Islandora Drupal code into a git repository and assumes this step has already happened. The directions below will explain how to clone Islandora Drupal code from a previously existing Islandora Drupal git repository that should already be accessible to you.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Local ISLE installation expects that an existing Production Islandora Drupal site will be imported on a personal computer for further ISLE migration testing, Drupal theme development, ingest testing, etc.

* You will be using ISLE version **1.2.0** or higher.

* You are using Docker-compose **1.24.0** or higher.

* You have git installed on your personal computer.

* You have a previously existing private Islandora Drupal git repository.

* You have access to a private git repository in [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com), etc.
    * If you do not, please contact your IT department for git resources, or else create an account with one of the above providers.
    * **WARNING:** Only use **Private** git repositories given the sensitive nature of the configuration files. **DO NOT** share these git repositories publicly.

* **For Microsoft Windows:**
    * You have installed [Git for Windows](../install/host-software-dependencies.md#windows) and will use its provided "Git Bash" as your command line interface; this behaves similarly to LINUX and UNIX environments. Git for Windows also installs "openssl.exe" which will be needed to generate self-signed SSL certs. (Note: Powershell is not recommended as it is unable to run UNIX commands or execute bash scripts without a moderate degree of customization.)
    * Set your text editor to use UNIX style line endings for files. (Text files created on DOS/Windows machines have different line endings than files created on Unix/Linux. DOS uses carriage return and line feed ("\r\n") as a line ending, which Unix uses just line feed ("\n").)

---

## Index of Instructions

* Step 0: Copy Production Data to Your Personal Computer
* Step 1: Choose a Project Name
* Step 1.5: Edit "/etc/hosts" File
* Step 2: Setup Git for the ISLE Project
* Step 3: Git Clone the Production Islandora Drupal Site Code
* Step 4: Edit the ".env" File to Change to the Local Environment
* Step 5: Create New Users and Passwords by Editing "local.env" File
* Step 6: Create New Self-Signed Certs for Your Project
* Step 7: Download the ISLE Images
* Step 8: Launch Process
* Step 9: Import the Production MySQL Drupal Database
* Step 10: Run Islandora Drupal Site Scripts
* Step 11: Test the Site
* Step 12: Ingest Sample Objects

---

## Step 0: Copy Production Data to Your Personal Computer

Be sure to run a backup of any current non-ISLE systems prior to copying or exporting any files.

### Drupal Site Files and Code

1. Copy the `/var/www/html/sites/default/files` directory from your Production Apache server to an appropriate storage area on your personal computer. You'll move this directory in later steps.

2. Locate and note the previously existing private Islandora Drupal git repository. You'll be cloning this into place once the ISLE project has been cloned in later steps.

### Drupal Site Database

**Prior to attempting this step, please consider the following:**

* Drupal website databases can have a multitude of names and conventions. Confer with the appropriate IT departments for your institution's database naming conventions.

* Recommended that the production databases be exported using the ".sql" /or ".gz" file formats (e.g. "prod_drupal_site_082019.sql.gz") for better compression and minimal storage footprint.

* If the end user is running multi-sites, there will be additional databases to export.

* Do not export the "fedora3" database or any system tables (such as "information_schema", "performance_schema", "mysql")

* If possible, on the production Apache webserver, run `drush cc all` from the command line on the production server in the `/var/www/html` directory PRIOR to any db export(s). Otherwise issues can occur on import due to all cache tables being larger than "innodb_log_file_size" allows

#### Export the Production MySQL Islandora Drupal Database

* Export the MySQL database for the current Production Islandora Drupal site in use and copy it to your personal computer (local) in an easy to find place. In later steps you'll be directed to import this file. **Please be careful** performing any of these potential actions below as the process impacts your Production site.

* If you are not comfortable or familiar with performing these actions, we recommend that you instead work with your available IT resources to do so.
    * To complete this process, you may use a MySQL GUI client or, if you have command line access to the MySQL database server, you may run the following command, substituting your actual user and database names:
    * **Example:** `mysqldump -u username -p database_name > prod_drupal_site_082019.sql`
    * Copy this file down to your personal computer.

### Fedora Hash Size (Conditional)

* Are you migrating an existing Islandora site that has greater than one million objects? 

* If true, then please carefully read about the [Fedora Hash Size (Conditional)](../install/install-troubleshooting.md#fedora-hash-size-conditional).

### Solr Schema and Islandora Transforms

This data can be challenging depending on the level of customizations to contend with and as such, ISLE maintainers recommends following one of the three (3), "Easy", "Intermediate", and "Advanced" strategies outlined below.

#### Strategy 1: **Easy** - Run "Stock" ISLE

Don't copy any existing production Solr schemas, GSearch .xslt files, etc., and opt instead to use ISLE's default versions. Import some objects from your existing Fedora repository and see if they display properly in searches as you like.

#### Strategy 2: **Intermediate** - Bind Mount in Existing Transforms and Schemas

Bind mount in existing transforms and schemas  to override ISLE settings with your current Production version.

**WARNING** _This approach assumes you are running Solr 4.10.x.; **only attempt** if you are running that version on Production._

* Copy these current Production files and directory to your personal computer in an appropriate location.
    * Solr `schema.xml`
    * GSearch `foxmltoSolr.xslt` file
    * GSearch `islandora_transforms`
    * Keep the files you create during this process; you will need them again for `Step 2a` (below)!

* **Please note:** You may need to further review paths in the files mentioned above, and edit them to match ISLE system paths.  

#### Strategy 3: **Advanced** - Diff and Merge Current Production Customization Edits Into ISLE Configs

* Copy these current production files and directory to your personal computer in an appropriate location.
    * Solr `schema.xml`
    * GSearch `foxmltoSolr.xslt` file
    * GSearch `islandora_transforms`

* Run the Demo ISLE briefly to pull files for modification and correct ISLE system paths.  

* You can find these paths by running the Demo and copying these files out to an appropriate location.  

    * `docker cp isle-solr-ld:/usr/local/solr/collection1/conf/schema.xml schema.xml`  
    * `docker cp isle-fedora-ld:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/foxmlToSolr.xslt foxmlToSolr.xslt`  
    * `docker cp isle-fedora-ld:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms islandora_transforms`

* Using a "diff" tool (_software that allows one to compare and find the differences between two files_), compare:  

    * your Production Solr `schema.xml` file to the ISLE demo `schema.xml` file.  
    * your Production GSearch `foxmltoSolr.xslt` file to the ISLE demo `foxmltoSolr.xslt` file.  
    * your Production GSearch `islandora_transforms` directory to the ISLE demo `islandora_transforms` directory.

* Look for edits and comments that indicate specific customization and make note of the differences.  

    * Merge in the customizations into the ISLE versions.
    * Keep the files you create during this merge process; you will need them again for `Step 2a` (below)!

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

Create the following two new, empty, private git repositories (if they do not already exist) within your git repository hosting service (e.g [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com)):

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

* Now you have the current ISLE project code checked into git as a foundation to make changes specific to your local and project needs. You'll use this git "icg-upstream" process in the future to pull updates and releases from the main ISLE project.

---

### Step 2a: Add Customizations from "Step 0" to the Git Workflow

This step is intended for users who followed either the "**Intermediate**" or "**Advanced**" migration options in "Step 0" above.  If you choose the **Easy** migration option you may safely skip `Step 2a`.

Navigate to your local "ISLE" directory

* `cd /path/to/your/repository`

Create new directories under "./config" to hold the Solr and GSearch files you retrieved in "Step 0".  Do the following::

```
mkdir -p ./config/solr
mkdir -p ./config/fedora/gsearch
```

* Copy your "schema.xml" file from "Step 0" into the new "./config/solr/" directory.

* Copy your "foxmltoSolr.xslt" file and "islandora_transforms" directory from "Step 0" into the "config/fedora/gsearch/" directory.

* Add a new line in the Solr volumes section of your "docker-compose.local.yml"  
```
  - config/solr/schema.xml:/usr/local/solr/collection1/conf/schema.xml`  
```

* Add new lines in the Fedora volumes section of your "docker-compose.local.yml"  
```
  - ./config/fedora/gsearch/islandora_transforms:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms
  - ./config/fedora/gsearch/foxmlToSolr.xslt:/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/foxmlToSolr.xslt
```

Continue the local setup as directed below and ultimately import some objects from your existing Fedora repository and see if they display properly in searches as you like.

---

## Step 3: Git Clone the Production Islandora Drupal Site Code

This step assumes you have an existing Islandora Drupal site checked into a git repository. (If not, then you'll need to check your Drupal site into a git repository following the same commands from [Local ISLE Installation: New Site](../install/install-local-new.md) documentation.)

**Note:** If below you see a "fatal: Could not read from remote repository." error, then please read [Fatal: Could not read from remote repository](../install/install-troubleshooting.md#fatal-could-not-read-from-remote-repository).

_Using the same open terminal:_

* Create a location outside of your ISLE directory where your Islandora Drupal site code will be stored.
  While you may create this location anywhere, we suggest that you put it at the same level as your existing ISLE directory. From your ISLE directory, go up one level:
    * `cd ..`
    * `git clone https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-islandora.git`
    * **Example:** The above process created a folder named "yourprojectnamehere-islandora"
* Now update the "docker-compose.local.yml" in the ISLE repository to create a bind-mount to the Islandora Drupal site code:
    * Search for "apache:"
    * Find the sub-section called "volumes:"
    * Find this line:
        * "- ./data/apache/html:/var/www/html:cached"
    * Edit the above line to be like this:
        * ` - ../yourprojectnamehere-islandora:/var/www/html:cached`
* Your Production Islandora site code is now configured to be used in this local setup.

---

## Step 4: Edit the ".env" File to Change to the Local Environment

* Navigate to your ISLE project directory.

* Open the ".env" file in a text editor.

* Change only the following lines in the ".env" file so that the resulting values look like the following: **Please note: the following below is an example not actual values you should use. Use one word to describe your project and follow the conventions below accordingly**
    * `COMPOSE_PROJECT_NAME=yourprojectnamehere_local`
    * `BASE_DOMAIN=yourprojectnamehere.localdomain`
    * `CONTAINER_SHORT_ID=ld` _leave default setting of `ld` as is. Do not change._
    * `COMPOSE_FILE=docker-compose.local.yml`

* Save and close the file.

* Additionally, depending on your decision from "Step 0", you may need to make additional edits to `docker-compose.local.yml` and move files into place as directed from the (**Intermediate**) and (**Advanced**) sections.

**Please note:** If you want to use a MySQL client with a GUI to import the Production MySQL Drupal database you'll need to uncomment the `ports` section of the MySQL service within the `docker-compose.local.yml` to make it so you can access port `3306` from the host computer. If you are already running MySQL on your personal computer, you'll have a port conflict and will need to either shutdown that service prior to running ISLE or change `3306:3306` to something like `9306:3306`. Please double-check.

**Please note:** We highly recommend that you also review the contents of the `docker-compose.local.yml` file as the Apache service volume section uses bind mounts for the intended Drupal Code instead of using default Docker volumes. This allows users to perform Local Islandora Drupal site development with an IDE. This line is a suggested path and users are free to change values to the left of the `:` to match their Apache data folder of choice. However we recommend starting out with the default setting below.
Default: `- ./data/apache/html:/var/www/html:cached`

---

## Step 5: Create New Users and Passwords by Editing "local.env" File

You can reuse some of the older Production settings in the "local.env" if you like (e.g. the database name "DRUPAL_DB", database user "DRUPAL_DB_USER" even the drupal database user password "DRUPAL_DB_PASS" if that makes it easier). It is important to avoid repeating passwords in the ISLE Staging and Production environments.

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

## Step 6: Create New Self-Signed Certs for Your Project

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

* Add the SSL .pem and .key file names generated from running the above script to the "./config/proxy/traefik.local.toml" file.
    * `cd ..`
    * Open `traefik.local.toml` in a text editor.
    * **Example:**
        * `certFile = "/certs/yourprojectnamehere.localdomain.pem"`
        * `keyFile = "/certs/yourprojectnamehere.localdomain.key"`

---

## Step 7: Download the ISLE Images

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_):
* _Using the same open terminal:_
    * Navigate to the root of your ISLE project
    * `cd ~/pathto/yourprojectnamehere`
    * `docker-compose pull`

## Step 8: Launch Process

* _Using the same open terminal:_
    * `docker-compose up -d`

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* _Using the same open terminal:_
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with "isle-" are expected to have a "STATUS" of "Up" (for x time).
      * **If any of these are not "UP", then use [Non-Running Docker Containers](../install/install-troubleshooting.md#non-running-docker-containers) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, oe (c) older exited ISLE containers that maybe should be removed. --->

## Step 9: Import the Production MySQL Drupal Database

**Method A: Use a MySQL client with a GUI**

* Configure the client with the following:
    * Host = `127.0.0.1`
    * Port: `3306` _or a different port if you changed it_
    * Username: `root`
    * Password: `YOUR_MYSQL_ROOT_PASSWORD` in the "local.env")
* Select the Drupal database "DRUPAL_DB" in the "local.env")
* Click File > Import (_or equivalent_)
* Select your exported Production Islandora Drupal database file (e.g. "prod_drupal_site_082019.sql.gz")
* The import process will take 1 -3 minutes depending on the size.

**Method B: Use the command line**

* Copy the Production Islandora Drupal database file (e.g. "prod_drupal_site_082019.sql.gz") to your ISLE MySQL container
    * Run `docker ps` to determine the mysql container name
    * `docker cp /pathto/prod_drupal_site_082019.sql.gz your-mysql-containername:/prod_drupal_site_082019.sql.gz`
    * **Example:**
        * `docker cp /c/db_backups/prod_drupal_site_082019.sql.gz isle-mysql-ld:/prod_drupal_site_082019.sql.gz`
    * This might take a few minutes depending on the size of the file.

* Shell into the mysql container by copying and pasting the appropriate command:
    * **For Mac/Ubuntu/CentOS/etc:** `docker exec -it your-mysql-containername bash`
    * **For Microsoft Windows:** `winpty docker exec -it your-mysql-containername bash`
* Import the Production Islandora Drupal database. Replace the "DRUPAL_DB_USER" and "DRUPAL_DB" in the command below with the values found in your "local.env" file.
    * `mysql -u DRUPAL_DB_USER -p DRUPAL_DB < prod_drupal_site_082019.sql.gz`
* This might take a few minutes depending on the size of the file.
* Type `exit` to exit the container

---

## Step 10: Run Islandora Drupal Site Scripts

This step will show you how to run the "migration_site_vsets.sh" script on the Apache container to change Drupal database site settings for ISLE connectivity.

 _Using the same open terminal:_
 
* Run `docker ps` to determine the apache container name
* Copy the "migration_site_vsets.sh" to the root of the Drupal directory on your Apache container
    * `docker cp scripts/apache/migration_site_vsets.sh your-apache-containername:/var/www/html/migration_site_vsets.sh`
* Change the permissions on the script to make it executable
    * **For Mac/Ubuntu/CentOS/etc:** `docker exec -it your-apache-containername bash -c "chmod +x /var/www/html/migration_site_vsets.sh"`
    * **For Microsoft Windows:** `winpty docker exec -it your-apache-containername bash -c "chmod +x /var/www/html/migration_site_vsets.sh"`
* Run the script
    * **For Mac/Ubuntu/CentOS/etc:** `docker exec -it your-apache-containername bash -c "cd /var/www/html && ./migration_site_vsets.sh"`
    * **For Microsoft Windows:** `winpty docker exec -it your-apache-containername bash -c "cd /var/www/html && ./migration_site_vsets.sh"`

Since you've imported an existing Drupal database, you must now reinstall the Islandora solution packs so the Fedora repository will be ready to ingest objects.

* Copy the "install_solution_packs.sh" to the root of the Drupal directory on your Apache container
    * `docker cp scripts/apache/install_solution_packs.sh your-apache-containername:/var/www/html/install_solution_packs.sh`
* Change the permissions on the script to make it executable
    * **For Mac/Ubuntu/CentOS/etc:** `docker exec -it your-apache-containername bash -c "chmod +x /var/www/html/install_solution_packs.sh"`
    * **For Microsoft Windows:** `winpty docker exec -it your-apache-containername bash -c "chmod +x /var/www/html/install_solution_packs.sh"`
* Run the script
    * **For Mac/Ubuntu/CentOS/etc:** `docker exec -it your-apache-containername bash -c "cd /var/www/html && ./install_solution_packs.sh"`
    * **For Microsoft Windows:** `winpty docker exec -it your-apache-containername bash -c "cd /var/www/html && ./install_solution_packs.sh"`
* The above process will take a few minutes depending on the speed of your local and Internet connection.
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

## Step 11: Test the Site

* In your web browser, enter this URL: `https://yourprojectnamehere.localdomain`

<!--- TODO: Add error message and how to proceed (click 'Advanced...') --->

* Note: You may see an SSL error warning that the site is unsafe. It is safe, it simply uses "self-signed" SSL certs. Ignore the error and proceed to the site.

* Log in to the local Islandora site with the credentials ("DRUPAL_ADMIN_USER" and "DRUPAL_ADMIN_PASS") you created in "local.env".

    * You can also attempt to use login credentials that the Production server would have stored in its database.
    
* If the newly created Drupal login doesn't work then, you'll need to Shell into the Apache container:

    * **For Mac/Ubuntu/CentOS/etc:** `docker exec -it your-apache-containername bash`
    * **For Microsoft Windows:** `winpty docker exec -it your-apache-containername bash`

* Navigate to this directory
    * `cd /var/www/html`
    
* Create the user found in "DRUPAL_ADMIN_USER" and set its password to the value of "DRUPAL_ADMIN_PASS" as you previously created in "local.env". 

    * In the example below swap-out "DRUPAL_ADMIN_USER" and "DRUPAL_ADMIN_PASS" with those found in "local.env".

    * `drush user-create DRUPAL_ADMIN_USER --mail="youremailaddresshere" --password="DRUPAL_ADMIN_PASS";`
    
    * `drush user-add-role "administrator" DRUPAL_ADMIN_USER`
    
* Type `exit` to exit the container

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
* [ISLE Installation: Environments](../install/install-environments.md) helps explain the ISLE workflow structure, the associated files, and what values ISLE endusers should use for the ".env", "local.env", etc.
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator testing.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Local ISLE Installation: Migrate Existing Islandora Site
