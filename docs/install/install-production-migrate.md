# Production ISLE Installation: Migrate Existing Islandora Site

_Expectations:  It takes an average of **2-4+ hours** to read this documentation and complete this installation._

This Production ISLE Installation will be similar to the [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md) instructions you just followed but in addition to using a copy of your currently running Production themed Drupal website, a copy of the Production Fedora repository will also be needed for you to continue migrating to ISLE with the end goal of first deploying to an ISLE Production environment and then cut over from the existing non-ISLE Production and Production servers to their new ISLE counterparts.

Islandora Drupal site code here should be finished and ready for public consumption. Fedora data will be a mirror of your currently running non-ISLE Production Fedora repository. It is recommended that this remote site not be publicly accessible until you are ready to cutover and give public access.

This installation builds a Production environment for the express purpose of migrating a previously existing Islandora site onto the ISLE platform. If you need to build a brand new Production site for development and are not migrating an existing Islandora site, then please **stop** and use the [Production ISLE Installation: New Site](../install/install-production-new.md) instead.

As this Production domain will require a real domain name or [FQDN](https://kb.iu.edu/d/aiuv), you will need to ask your IT department or appropriate resource for an "A record" to be added for your domain to "point" to your Production Host Server IP address in your institution's DNS records.

Example:`https://yourprojectnamehere.institution.edu`

Once this has been completed, if you do not want to use Let's Encrypt, you can also request commercial SSL certificates from your IT department for this domain as well. Please note the DNS records will need to exist prior to the creation of any SSL certificate (Commercial or Let's Encrypt.)

* If you already have pre-existing Production commercial SSL certificates, they can certainly be reused and copied into the ISLE project as directed.

Unlike the Local and Demo setups, you will not have to edit `/etc/localhosts` to view your domain given that DNS is now involved. Your new domain will no longer use the `.localdomain` but instead something like `https://yourprojectnamehere.institution.edu`

This document also has directions on how you can save newly updated ISLE code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Production and finally to Production. The [ISLE Installation: Environments](../install/install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE end-users should use for the `.env`, `production.env`, etc.

This document **does not** have directions on how you can save previously existing Islandora Drupal code into a git repository and assumes this step has already happened. The directions below will explain how to clone Islandora Drupal code from a previously existing Islandora Drupal git repository that should already be accessible to you.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Production ISLE installation is intended for an existing Production Islandora Drupal site to be imported along with a copy of the current Production Fedora Repository for further ISLE migration testing, Drupal theme development, ingest testing, etc. on a remote ISLE host server.
    * Some materials are to be "migrated" from the work you performed on your personal computer from the prior steps and processes in [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md) instructions.

* You will be using ISLE version **1.2.0** or higher.

* You are using Docker-compose **1.24.0** or higher.

* You have git installed on your personal computer as well as the remote ISLE host server.

* You have already provisioned a remote ISLE hosts server and have documented its IP address.
      * You may have used the [ISLE Ansible script](https://github.com/Islandora-Collaboration-Group/ISLE-Ansible) to accomplish this.
      * If doing this manually, please review the following to ensure the remote Production ISLE host server has all dependencies e.g. CPU, memory and disk space prior to deploying the ISLE Production environment profile for deploy
          * [Hardware Requirements](host-hardware-requirements.md)
          * [Software Dependencies](host-software-dependencies.md)
      * This server should be running at the time of deploy.
      * **Critical** - This Production server has the same amount of disk space as your current Production Fedora server does in order to store a copy of the Fedora repository. Please ensure that these sizes match. Please also plan on adding additional capacity as needed for any potential ingest testing, etc.

* You have a previously existing private Islandora Drupal git repository.

* You have access to a private git repository in [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com), etc.
    * If you do not, please contact your IT department for git resources, or else create an account with one of the above providers.
    * **WARNING:** Only use **Private** git repositories given the sensitive nature of the configuration files. **DO NOT** share these git repositories publicly.

* You have already have the appropriate A record entered into your institutions DNS system and can resolve the Production domain (https://yourprojectnamehere.institution.edu) using a tool like https://www.whatsmydns.net/

* **RECOMMENDATION:** We recommend that you use a temporary domain name e.g. `https://yourprojectnamehere-newprod.institution.edu` to check the new Production ISLE site while an existing non-ISLE Production site is still running and compare for differences. This will also allow endusers to still access the old Production system while the work for the new ISLE system is ongoing.
    * You'll need to add an additional A record for `yourprojectnamehere-newprod.institution.edu` to point to the same ISLE Production Host Server IP.
    * Please adjust all involved domain names and configuration files accordingly. 
    * When ready to launch the new ISLE production site publicly, you can remove the `-newprod` from all domain references and configuration files.
        * Update the DNS records to repoint the current non-ISLE Production server A record for `yourprojectnamehere.institution.edu` to the new ISLE host server IP.
        * Remove the temporary DNS A record for `yourprojectnamehere-newprod.institution.edu`
        * If using commercial SSLs, you'll also need to copy them over to the `./config/proxy/ssl-certs` directory and adjust the `traefik.production.toml` file accordingly with the new file names.

* You have reviewed the [ISLE Installation: Environments](../install/install-environments.md) for more information about suggested Production values.

* You are familiar with using tools like `scp, cp or rsync` to move configurations, files and data from your local to the remote Production server.

* You have access to your Production Islandora Drupal, Solr and Fedora data and copy from your servers to the new ISLE Production server.

* You will schedule a content freeze for all Production Fedora ingests and additions to your Production website. This will allow you to get up to date data from Production to Production.

---

## Index of Instructions

This process will differ slightly from previous builds in that there is work to be done on the local to then be pushed to the Production ISLE Host server with additional followup work to be performed on the remote Production ISLE Host server.

The instructions that follow below will have either a `On Local` or a `On Remote Production` pre-fix to indicate where the work and focus should be. In essence, the git workflow established during the local build process will be extended for deploying on Production and for future ISLE updates and upgrades.

**Steps 1-6: On Local - Configure the ISLE Production Environment Profile for Deploy to Remote**

  * Step 1: Copy Production Data to Your Local
  * Step 2: On Local - Shutdown Any Local Containers & Review Local Code
  * Step 3: On Local - Create New Users and Passwords by Editing "production.env"
  * Step 4: On Local - Review and Edit "docker-compose.production.yml"
  * Step 4A: On Local - (Optional) Changes for "docker-compose.production.yml"
  * Step 5: On Local Production - If Using Commercial SSLs
  * Step 6: On Local - Commit ISLE Code to Git Repository

**Steps 7-18: On Remote Production - Configure the ISLE Production Environment Profile for Launch and Usage**

  * Step 7: On Remote Production - Git Clone the ISLE Repository to the Remote Production ISLE Host Server
  * Step 8: On Remote Production - Create the Appropriate Local Data Paths for Apache, Fedora and Log Data
  * Step 9: On Remote Production - Clone Your Production Islandora Code
  * Step 10: On Remote Production - Copy Over the Production Data Directories
  * Step 11: On Remote Production - If Using Let's Encrypt
  * Step 12: On Remote Production - Edit the ".env" File to Change to the Production Environment
  * Step 13: On Remote Production - Download the ISLE Images
  * Step 14: On Remote Production - Start Containers
  * Step 15: On Remote Production - Import the Production MySQL Drupal Database
  * Step 16: On Remote Production - Run ISLE Scripts
  * Step 17: On Remote Production - Re-Index Fedora & Solr
  * Step 18: On Remote Production - Review and Test the Drupal Production Site

---

## Step 1: Copy Production Data to Your Local

### Drupal Site Database

You are repeating this step given that data may have changed on the Production site since creating your local. It is critical that Production be a mirror or close to exact copy of Production.

**Prior to attempting this step, please consider the following:**

* Drupal website databases can have a multitude of names and conventions. Confer with the appropriate IT departments for your institution's database naming conventions.

* Recommended that the production databases be exported using the `.sql` /or `.gz` file formats (e.g. "prod_drupal_site_082019.sql.gz") for better compression and minimal storage footprint.

* If the end user is running multi-sites, there will be additional databases to export.

* Do not export the `fedora3` database

* If possible, on the production Apache webserver, run `drush cc all` from the command line on the production server in the `/var/www/html` directory PRIOR to any db export(s). Otherwise issues can occur on import due to all cache tables being larger than `innodb_log_file_size` allows

#### Export the Production MySQL Islandora Drupal Database

* Export the MySQL database for the current Production Islandora Drupal site in use and copy it to your local in an easy to find place. In later steps you'll be directed to import this file. **Please be careful** performing any of these potential actions below as the process impacts your Production site. If you are not comfortable or familiar with performing these actions, we recommend that you instead work with your available IT resources to do so.
    * To complete this process, you may use a MySQL GUI client or, if you have command line access to the MySQL database server, you may run the following command, substituting your actual user and database names:
    * **Example:** `mysqldump -u username -p database_name > prod_drupal_site_082019.sql`
    * Copy this file down to your personal computer.

---

## Step 2: On Local - Shutdown Any Local Containers & Review Local Code

* Ensure that your ISLE and Islandora git repositories have all the latest commits and pushes from the development process that took place on your personal computer. If you haven't yet finished, do not proceed until everything is completed.

* Once finished, open a `terminal` (Windows: open `Git Bash`)
    * Navigate to your Local ISLE repository
    * Shut down any local containers e.g. `docker-compose down`

---

## Step 3: On Local - Create New Users and Passwords by Editing "production.env"

* Open the "production.env" file in a text editor.
    * Find each comment that begins with: `# Replace this comment with a ...` and follow the commented instructions to edit the passwords, database and user names.
        * **Review carefully** as some comments request that you replace with `...26 alpha-numeric characters` while others request that you create an `...easy to read but short database name`.
        * It is okay if you potentially repeat the values previously entered for your local `(DRUPAL_DB)` & `(DRUPAL_DB_USER)` in this Production environment but we strongly recommend not reusing all passwords for environments (e.g. `(DRUPAL_DB_PASS)` & `(DRUPAL_HASH_SALT)` should be unique values for each environment).
        * In many cases the username is already pre-populated. If it doesn't have a comment directing you to change or add a value after the `=`, then don't change it.
    * Once finished, save and close the file.

* Open the `config/apache/settings_php/settings.production.php` file.
    * Find the first comment that begins with: `# ISLE Configuration` and follow the commented instructions to edit the database, username and password.
    * Find the second comment that begins with: `# ISLE Configuration` and follow the instructions to edit the Drupal hash salt.
    * Once finished, save and close the file.

---

## Step 4: On Local - Review and Edit "docker-compose.production.yml"

* Review the disks and volumes on your remote Production ISLE Host server to ensure they are of an adequate capacity for your collection needs and match what has been written in the `docker-compose.production.yml` file.

* Please read through the `docker-compose.production.yml` file as there are bind mount points that need to be configured on the host machine, to ensure data persistence. There are suggested bind mounts that the end-user can change to fit their needs or they can setup additional volumes or disks to match the suggestions.
    * In the `fedora` services section
        * `- /mnt/data/fedora/datastreamStore:/usr/local/fedora/data/datastreamStore`
        * `- /opt/data/fedora/objectStore:/usr/local/fedora/data/objectStore`
    * In the `apache` services section
        * `- /opt/data/apache/html:/var/www/html`

* Review the your `docker-compose.local.yml` file for custom edits made and copy them to the `docker-compose.production.yml` file as needed, this can include changes to Fedora Gsearch Transforms, Fedora hash size and more.

### SSL Certificates

* Depending on your choice of SSL type (Commercial SSL files or the Let's Encrypt service), you'll need to uncomment only one line of the `traefik` services section. There are also inline instructions to this effect in the `docker-compose.production.yml` file.
    * To use `Let's Encrypt for SSL`, uncomment:
        * `- ./config/proxy/acme.json:/acme.json`

    * To use commercial SSLs, uncomment:
        * `./config/proxy/ssl-certs:/certs:ro`
        * Additionally you'll need to add your SSL certs (.cert, .pem, .key) files to `config/proxy/ssl-certs`

* Based on the choice of SSL type made above, you'll need to refer to the `/config/proxy/traefik.production.toml` file for further configuration instructions.

---

## Step 4A: On Local - (Optional) Changes for "docker-compose.production.yml"

This section is for optional changes for the `docker-compose.production.yml`, end-users do not have feel like they have to make any choices here and can continue to **Step 4** as needed.

The options include PHP settings, Java Memory Allocation, MySQL configuration and use of the [TICK Stack](../optional-components/tickstack.md)


* _(Optional)_ - You can change PHP settings such as file upload limits and memory usage by uncommenting the following in the `apache` services section.
    * `- ./config/apache/php_ini/php.production.ini:/etc/php/7.1/apache2/php.ini`
    * You'll then need to make edits in the `./config/apache/php_ini/php.production.ini` file.

* _(Optional)_ - This line is already uncommented by default in ISLE but we're calling it out here that you can changes to the suggested levels or values within the `./config/mysql/ISLE.cnf` file if needed. When setting up for the first time, it is best practice to leave these settings in place. Over time, you can experiment with further tuning and experimentation based on your project or system needs.

* _(Optional)_ - You can change the suggested `JAVA_MAX_MEM` & `JAVA_MIN_MEM` levels but do not exceed more than 50% of your system memory. When setting up for the first time, it is best practice to leave these settings in place as they are configured for a Production ISLE Host Server using 16 GB of RAM. Over time, you can experiment with further tuning and experimentation based on your project or system needs.

* _(Optional)_ - You can opt to uncomment the TICK stack settings for monitoring but you'll need to follow the [TICK Stack](../optional-components/tickstack.md) instructions prior to committing changes to your ISLE git repository.
  * All TICK related code can be found at the end of all ISLE services within the `docker-compose.production.yml` file.
  * **Example:**

```bash
  ## _(Optional)_: Uncomment lines below to run ISLE with the TICK monitoring system
  logging:
    driver: syslog
    options:
      tag: "{{.Name}}"
```

  * Uncomment the lines found in the new TICK stack services section of the `docker-compose.production.yml` file for hosting of that monitoring service on the Production ISLE Host server.
      * There are additional configurations to be made to files contained within `./config/tick` but you'll need to follow the [TICK Stack](../optional-components/tickstack.md) instructions prior to committing changes to your ISLE git repository.
  * Uncomment the TICK stack data volumes as well at the bottom of the file.

---

## Step 5: On Local Production - If Using Commercial SSLs

If you are going to use Let's Encrypt instead, you can skip this step and move onto the next one. There will be additional steps further in this document, to help you configure it.

If you have decided to use Commercial SSL certs supplied to you by your IT team or appropriate resource, please continue following this step.

* Add your Commercial SSL certificate and key files to the `./config/proxy/ssl-certs` directory
    * **Example:**
    * `./config/proxy/ssl-certs/yourprojectnamehere.domain.cert`
    * `./config/proxy/ssl-certs/yourprojectnamehere.domain.key`

* Edit the `./config/proxy/traefik.production.toml` and follow the in-line instructions. Replace the .pem & .key with the name of your Production SSL certificate and associated key. Do note the positioning of the added lines. Third character indentation.

**Please note** despite the instruction examples differing on file type, (`.pem` or `cert`), either one is compatible, use what you have been given. Merely change the file type suffix accordingly.

**Example: .cert**
```bash
    [entryPoints.https.tls]
      [[entryPoints.https.tls.certificates]]
      certFile = "/certs/yourprojectnamehere.domain.cert"
      keyFile = "/certs/yourprojectnamehere.domain.key"
```

**Example: .pem**
```bash
    [entryPoints.https.tls]
      [[entryPoints.https.tls.certificates]]
      certFile = "/certs/yourprojectnamehere.institution.edu.pem"
      keyFile = "/certs/yourprojectnamehere.institution.edu.key"
```

---

## Step 6: On Local - Commit ISLE Code to Git Repository

* Once you have made all of the appropriate changes to your Production profile. Please note the steps below are suggestions. You might use a different git commit message. Substitute `<changedfileshere>` with the actual file names and paths. You may need to do this repeatedly prior to the commit message.
    * `git add <changedfileshere>`
    * `git commit -m "Changes for Production"`
    * `git push origin master`

---

## On Remote Production - Configure the ISLE Production Environment Profile for Launch and Usage

## Step 7: On Remote Production - Git Clone the ISLE Repository to the Remote Production ISLE Host Server

* This assumes you have setup an `Islandora` deploy user. If not use a different non-root user for this purpose.

* You will also need to ensure that any `/home/islandora/.ssh/id_rsa.pub` key has been added to your git repository admin panel to allow for cloning from your two private git repositories.

Since the `/opt` directory might not let you do this at first, we suggest the following workaround which you'll only need to do once. Future ISLE updates will not require this step.

* Shell into your Production ISLE host server as the `Islandora` user.

* Clone your ISLE project repository with the newly committed changes for Production to the `Islandora` user home directory.
    * `git clone https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git /home/islandora/`
    * This may take a few minutes (2-4) depending on your server's Internet connection.

* Move the newly cloned directory to the `/opt` directory as the root user
    * `sudo mv /home/islandora/yourprojectnamehere-isle /opt/yourprojectnamehere-isle`

* Fix the permissions so that the `islandora` user has access.
    * `sudo chown -Rv islandora:islandora /opt/yourprojectnamehere-isle`

---

## Step 8: On Remote Production - Create the Appropriate Local Data Paths for Apache, Fedora and Log Data

* Create the `/opt/data` directory
    * `sudo mkdir -p /opt/data`
* Change the permissions to the Islandora user.
  * `sudo chown -Rv islandora:islandora /opt/data`

---

## Step 9: On Remote Production - Clone Your Production Islandora Code

Please clone from your existing Production Islandora git repository.

* `git clone git@yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-islandora.git /opt/data/apache/html`

* Fix the permissions so that the `islandora` user has access.
    * `sudo chown -Rv islandora:islandora /opt/yourprojectnamehere-islandora`

---

## Step 10: On Remote Production - Copy Over the Production Data Directories

* It is recommended that you schedule a content freeze for all Production Fedora ingests and additions to your Production website. This will allow you to get up to date data from Production to Production.

* As you may have made some critical decisions potentially from "Step 0: Copy Production Data to Your Local" of the [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md) instructions, you need to re-follow the steps to get your:
    * Production Drupal site `files` directory
    * `Solr schema & Islandora transforms`
        * If you picked **Easy** option:
            * then you don't need to do anything here for the `Solr schema & Islandora transforms`
        * If you picked the **Intermediate** or **Advanced** options:
            * You'll need to copy in the customizations and files you created during the `local` environment into the `docker-compose.production.yml`. Ensure that one set of transforms and schema are used across all environments.
    * Production Fedora `datastreamStore` directory
        * You'll need to adjust the paths below in case your setup differs on either the non-ISLE Production server or the ISLE Production server.
        * Copy your `/usr/local/fedora/data/datastreamStore` data to the suggested path of `/mnt/data/fedora/datastreamStore`
            * You may need to change the permissions to `root:root` on the Production `/mnt/data/fedora/datastreamStore` directory above after copying so the Fedora container can access properly. Do not do this on your existing Production system please.
    * Production Fedora `objectStore`. 
        * Copy your `/usr/local/fedora/data/objectStore` data to the suggested path of `/opt/data/fedora/objectStore`
            * You may need to change the permissions to `root:root` on the Production `/opt/data/fedora/objectStore` above after copying so the Fedora container can access properly. Do not do this on your existing Production system please.

---

## Step 11: On Remote Production - If Using Let's Encrypt

If you are using Commercial SSLs, then please stop and move onto the next step.

If using Let's Encrypt, please continue to follow this step.

* Create an empty `acme.json` within the `./config/proxy/ssl-certs/` directory of your ISLE project.
    * `touch /opt/yourprojectnamehere/config/proxy/ssl-certs/acme.json`
    * `chmod 600 /opt/yourprojectnamehere/config/proxy/ssl-certs/acme.json`
    * This file will be ignored by git and won't cause any errors with checking in code despite the location
    * Do note that you may need to open your firewall briefly to allow the SSL certs to be added to the `acme.json` file. This will be indicated in the following steps.
    * Open your firewall to ports 80, 443 prior to starting up the containers to ensure SSL cert creation.

---

## Step 12: On Remote Production - Edit the ".env" File to Change to the Production Environment

This step is a multi-step, involved process that allows an end-user to make appropriate changes to the `.env` and then commit it locally to git. This local commit that never gets pushed back to the git repository is critical to allow future ISLE updates or config changes.

* Edit the .env, remove the `local` settings and then commit locally
    * `cd /opt/yourprojectnamehere`
    * `vi / nano / pico /opt/yourprojectnamehere/.env`
    * Edit `COMPOSE_PROJECT_NAME=` and replace the `local` settings with:
        * `COMPOSE_PROJECT_NAME=`  (Suggested) Add an identifiable project or institutional name plus environment e.g. acme_digital_production`
    * Edit `BASE_DOMAIN=` and replace the `local` settings with:
        * `BASE_DOMAIN=`            (Suggested) Add the full production domain here e.g. digital.institution.edu
    * Edit `CONTAINER_SHORT_ID=` and replace the `local` settings with:
        * `CONTAINER_SHORT_ID=`     (Suggested) Make an easy to read acronym from the letters of your institution and collection names plus environment e.g. (acme digitalcollections production) is acdcp
    * Edit `COMPOSE_FILE` change `local` to `production`
        * `COMPOSE_FILE=docker-compose.production.yml`
    * Save the file

    * Enter `git status` - You'll now see the following:

```bash
On branch master
Your branch is up to date with 'origin/master'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   .env
```    

* You'll need to add this file and commit it in git to be able to get future updates from ISLE as a process.
    * `git add .env`
    * `git commit -m "Added the edited .env configuration file for Production. DO NOT PUSH BACK TO UPSTREAM REPOSITORY - Jane Doe 8/2019"`
        * This is a suggested warning for users **NOT TO**  push back this configuration change to the main git repository. If that were done it could conflict with other setups.

* You may run into the following:

```bash
*** Please tell me who you are.

Run

  git config --global user.email "you@example.com"
  git config --global user.name "Your Name"

to set your account's default identity.
Omit --global to set the identity only in this repository.

fatal: empty ident name (for <islandora@yourprojectnamehere.institution.edu>) not allowed  
```

* Configure your server git client but don't use the `--global` setting as that could interfere with other git repositories e.g. your Islandora Drupal code.
    * **Example:** Within `/opt/yourprojectnamehere`
    * `git config user.email "jane@institution.edu"`
    * `git config user.name "Jane Doe"`

* Now re-run the commit command:

```bash
git commit -m "Added the edited .env configuration file for Production. DO NOT PUSH BACK TO UPSTREAM REPOSITORY - Jane Doe 8/2019"
[master 7ab3fcf9] Added the edited .env configuration file for Production. DO NOT PUSH BACK TO UPSTREAM REPOSITORY - Jane Doe 8/2019
 1 file changed, 4 insertions(+), 4 deletions(-)
```

---

## Step 13: On Remote Production - Download the ISLE Images

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_).
* _Using the same open terminal:_
    * Navigate to the root of your ISLE project
    * `cd ~/opt/yourprojectnamehere`
    * `docker-compose pull`

---

## Step 14: On Remote Production - Start Containers

**Please note:** Prior to starting the launch process, it is recommended that you briefly open your firewall to allow ports 80 and 443 access to the world. You'll only need to keep this open for 3 -5 minutes and then promptly close access once the Let's Encrypt SSL certificates have been generated.

* _Using the same open terminal:_
    * `docker-compose up -d`

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* _Using the same open terminal:_
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with `isle-` are expected to have a `STATUS` of `Up` (for x time).
      * **If any of these are not `UP`, then use [ISLE Installations: Troubleshooting](install-troubleshooting.md) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, or (c) older exited ISLE containers that maybe should be removed. --->

* In your web browser, enter your Production site URL: `https://yourprojectnamehere.institution.edu`
    * **Please note:** You should not see any errors with respect to the SSL certifications, you should see a nice green lock padlock for the site security. If you see a red error or unknown SSL cert provider, you'll need to shut the containers down and review the previous steps taken especially if using Let's Encrypt. You may need to repeat those steps to get rid of the errors.

---

## Step 15: On Remote Production - Import the Local MySQL Drupal Database

**Prior to attempting this step, please consider the following:**

* If the end user is running multi-sites, there will be additional databases to export.

* Do not import the `fedora3` database

---

### Import the Local MySQL Islandora Drupal Database

* Copy the `local_drupal_site_082019.sql` created in Step 1 to the Remote Production server.

* Import the exported Local MySQL database for use in the current Production Drupal site. Refer to your `production.env` for the usernames and passwords used below.
    * You can use a MySQL GUI client for this process instead but the command line directions are only included below.
    * Run `docker ps` to determine the MySQL container name
    * _Using the same open terminal:_  
    * Shell into your currently running Production MySQL container
        * `docker exec -it your-mysql-containername bash`
    * Import the Local Islandora Drupal database. Replace the "DRUPAL_DB_USER" and "DRUPAL_DB" in the command below with the values found in your "production.env" file.
        * `mysql -u DRUPAL_DB_USER -p DRUPAL_DB < local_drupal_site_082019.sql`
        * Enter the appropriate password: value of `DRUPAL_DB_PASS` in the "production.env")
        * This might take a few minutes depending on the size of the file.
        * Type `exit` to exit the container

---

## Step 16: On Remote Production - Run ISLE Scripts

This step will show you how to run the "migration_site_vsets.sh" script on the Apache container to change Drupal database site settings for ISLE connectivity.

 _Using the same open terminal:_  

* Run `docker ps` to determine the apache container name
* Copy the "migration_site_vsets.sh" to the root of the Drupal directory on your Apache container
    * `docker cp ./scripts/apache/migration_site_vsets.sh your-apache-containername:/var/www/html/migration_site_vsets.sh`
* Change the permissions on the script to make it executable
    * `docker exec -it your-apache-containername bash -c "chmod +x /var/www/html/migration_site_vsets.sh"`
* Run the script
    * `docker exec -it your-apache-containername bash -c "cd /var/www/html && ./migration_site_vsets.sh"`

This step will show you how to shell into your currently running Production Apache container, and run the "fix-permissions.sh" script to fix the Drupal site permissions.

* `docker exec -it your-apache-containername bash`
* `sh /utility-scripts/isle_drupal_build_tools/drupal/fix-permissions.sh --drupal_path=/var/www/html --drupal_user=islandora --httpd_group=www-data`
* This process will take 2-5 minutes
* You should see a lot of green [ok] messages.
* If the script appears to pause or prompt for `y/n`, DO NOT enter any values; the script will automatically answer for you.
* Type `exit` to exit the container

| For Microsoft Windows: |
| :-------------      |
| You may be prompted by Windows to: |
| - Share the C drive with Docker.  Click Okay or Allow.|
| - Enter your username and password. Do this.|
| - Allow vpnkit.exe to communicate with the network.  Click Okay or Allow (accept default selection).|
| - If the process seems to halt, check the taskbar for background windows.|

---

## Step 17: On Remote Production - Re-Index Fedora & Solr

When migrating any non-ISLE Islandora site, it is crucial to rebuild (reindex) the following three indices from the FOXML and datastream files on disk.

* **Fedora's indices:**
    * Resource Index - The Resource Index is the Fedora module that provides the infrastructure for indexing relationships among objects and their components.
    * SQL database - `fedora3` contains information vital for the Drupal site to connect to Fedora correctly.

* **Solr index** - Solr an open source enterprise search platform works in conjunction with the Islandora Solr module to provide a way to configure the Islandora search functions, the search results display, and the display of metadata on object pages. The index serves as a list of those objects for fast searching across large collections.

You can use the command-line interactive utility `fedora-rebuild.sh` on the `fedora` container to rebuild all indices when the Fedora (not Tomcat) server is offline.

Depending on the size of your repository, this entire process may take minutes (thousands of objects) or hours (millions of objects) to complete.

### Reindex Fedora RI & Fedora SQL Database (2/3)

Since this command can take minutes or hours depending on the size of your repository, As such, it is recommended starting a screen session prior to running the following commands. Learn more about [screen here](https://www.tecmint.com/screen-command-examples-to-manage-linux-terminals/) 

**Please note:** The method described below is a longer way of doing this process to onboard users.

* Shell into your currently running Production Fedora container
* Run `docker ps` to determine the Fedora container name
    * `docker exec -it your-fedora-containername bash`

* Navigate to the `utility_scripts` directory
    * `cd utility_scripts`

* Run the `rebuildFedora.sh` script. This script will give you output like the example below.
    * `./rebuildFedora.sh`

```bash
  OK - Stopped application at context path [/fedora]
Starting the rebuild process in the background. This may take a while depending on your Fedora repository size.
To watch the log and process run: tail -f $CATALINA_HOME/logs/fedora-rebuild.out
Truncating old SQL tables.
mysql: [Warning] Using a password on the command line interface can be insecure.
Automatically tailing the log file...
Press CTRL+C to stop watching at any time. This will NOT stop the rebuild process
```

* After a good period of time, again depending on the size of your Fedora collection there should be output like the example below. This indicates that the Fedora RI & SQL reindex process was sucessful. The number of objects rebuilt will vary. You can hit the CNTRL and C keys to exit out of the process, if need be. Do not exit the Fedora container yet, one more index to go; Solr.

```bash
Adding object #31: islandora:sp_web_archive_collection
Adding object #32: islandora:sp_web_archive
Adding object #33: islandora:newspaperPageCModel
Adding object #34: islandora:compound_collection
Adding object #35: islandora:newspaperCModel
Adding object #36: islandora:newspaperIssueCModel
Adding object #37: ir:citationCollection
Adding object #38: islandora:sp_basic_image_collection
SUCCESS: 38 objects rebuilt.
OK - Started application at context path [/fedora]
```

### Reindex Solr (3/3)

**WARNING** - This reindex process takes the longest of all three, with up to **1-30 or more hours** to complete depending on the size of your Fedora collection. As such, it is recommended starting a screen session prior to running the following command. Learn more about [screen here](https://www.tecmint.com/screen-command-examples-to-manage-linux-terminals/)

* Still staying within the `utility_scripts` directory on the Fedora container or reenter the Fedora container having started a new screen session, now run the `updateSolrIndex.sh` script. This script will give you output like the example below.
    * `./updateSolrIndex.sh`

```bash
FedoraGenericSearch (FGS) update Solr index from Fedora helper script.
Starting to reindex your Fedora repository. This process runs in the background and may take some time.
Checked and this operation is still running. You may disconnect and the process will continue to run.
Find logs at /usr/local/tomcat/logs/fgs-update-foxml.out and /usr/local/tomcat/logs/fgs-update-foxml.err.
You can watch log file 'tail -f /usr/local/tomcat/logs/fedoragsearch.daily.log' as the process runs.
```

**Please note:** Within this output, options to tail logs and watch progress are offered. Depending on the size of your collection this process may take hours, however it is okay to exit out of the container and even log off the remote Production server. You can check back frequently by running `tail -f /usr/local/tomcat/logs/fgs-update-foxml.out` on the Fedora container. If you visit your Drupal site and run a Solr search, you should start to see objects and facets start to work. The number of objects will increase over time.

* After a good period of time, again depending on the size of your Fedora collection, when the Solr re-index process finishes, output like the example below will appear in the `/usr/local/tomcat/logs/fgs-update-foxml.out` log. This indicates that the Solr reindex process was completed. The number of objects rebuilt will vary. You can hit the CNTRL and C keys to exit out of the tail process, if need be.

```bash
 tail -f /usr/local/tomcat/logs/fgs-update-foxml.out
Args
0=http://localhost:8080
1=updateIndex
2=fromFoxmlFiles
<?xml version="1.0" encoding="UTF-8"?>
<resultPage operation="updateIndex" action="fromFoxmlFiles" value="" repositoryName="FgsRepos" indexNames="" resultPageXslt="" dateTime="Thu Aug 08 20:43:12 GMT 2019">
<updateIndex xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:foxml="info:fedora/fedora-system:def/foxml#" xmlns:zs="http://www.loc.gov/zing/srw/" warnCount="0" docCount="13" deleteTotal="0" updateTotal="13" insertTotal="0" indexName="FgsIndex"/>
</resultPage>
```

* Type `exit` when finished to exit the container.

---

## Step 18: On Remote Production - Review and Test the Drupal Production Site

* In your web browser, enter this URL: `https://yourprojectnamehere.institution.edu`
    * Please note: You should not see any errors with respect to the SSL certifications. If so, please review your previous steps especially if using Let's Encrypt. You may need to repeat those steps to get rid of the errors.

* Log in to the local Islandora site with the credentials ("DRUPAL_ADMIN_USER" and "DRUPAL_ADMIN_PASS") you created in "production.env".
    * **Please note:** You are free to use previously Drupal admin or user accounts created during the Local site development process.

* You can decide to further QC and review the site as you wish or start to add digital collections and objects.
    * You could also further test using the [Islandora Sample Objects](https://github.com/Islandora-Collaboration-Group/islandora-sample-objects) as you may have done in the previous Local installation.

---

## Next Steps

Once you are ready to deploy your finished Drupal site, you may progress to launching the Production site publicly which could involve the following steps depending on choices made earlier in this document and process:

* If you followed the use of the temporary `-newprod` suggestion in the `Assumptions` section, remove the `-newprod` from all domain references and configuration files and recommit the change on the remote server in git.
    * If using commercial SSLs, you'll also need to copy them over to the `./config/proxy/ssl-certs` directory and adjust the `traefik.production.toml` file accordingly with the new file names.
    * If using Let's Encrypt, upon restart with the new settings, the acme.json file contents will change automatically.
    * Update the DNS records to repoint the current non-ISLE Production server A record for `yourprojectnamehere.institution.edu` to the new ISLE host server IP.
    * Remove the temporary DNS A record for `yourprojectnamehere-newprod.institution.edu`

* Lift any firewall restrictions and allowing full Internet access on http (port 80) and https (443).
    * Do not allow any other port to be publicly accessible.

---

## Additional Resources
* [ISLE Installation: Environments](../install/install-environments.md) help with explaining the ISLE structure, the associated files, and what values ISLE end-users should use for the ".env", "local.env", etc.
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator testing.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Production ISLE Installation: New Site
