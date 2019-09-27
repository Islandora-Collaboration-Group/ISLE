# Staging ISLE Installation: New Site

_Expectations:  It takes an average of **2-4+ hours** to read this documentation and complete this installation._

This Staging ISLE Installation will use the themed Drupal website created during the [Local ISLE Installation: New Site](../install/install-local-new.md) process and will create an empty Fedora repository for remote (non-local or cloud) hosting of a Staging site. Islandora Drupal site code here should be considered almost finished but hosted here for last touches and team review privately prior to pushing to public Production. Fedora data might have tests collections or collections that should then be synced to the Production site. It is recommended that this remote site not be publicly accessible.

While this installation will get you a brand new Staging site, it is **not** intended as a migration process of a previously existing Islandora site. If you need to build a Staging environment to migrate a previously existing Islandora site, please use the [Staging ISLE Installation: Migrate Existing Islandora Site](../install/install-staging-migrate.md) instead.

As this Staging domain will require a real domain name or [FQDN](https://kb.iu.edu/d/aiuv), you will need to ask your IT department or appropriate resource for an "A record" to be added for your domain to "point" to your Staging Host Server IP address in your institution's DNS records. We recommend that this sub-domain use `-staging` to differentiate it from the Production site.

Example:`https://yourprojectnamehere-staging.institution.edu`

Once this has been completed, if you do not want to use Let's Encrypt, you can also request commercial SSL certificates from your IT department for this domain as well. Please note the DNS records will need to exist prior to the creation of any SSL certificate (Commercial or Let's Encrypt.)

Unlike the Local and Demo setups, you will not have to edit `/etc/localhosts` to view your domain given that DNS is now involved. Your new domain will no longer use the `.localdomain` but instead something like `https://yourprojectnamehere-staging.institution.edu`

This document also has directions on how you can save newly created ISLE and Islandora code into a git software repository as a workflow process designed to manage and upgrade the environments throughout the development process from Local to Staging to Production. The [ISLE Installation: Environments](../install/install-environments.md) documentation can also help with explaining the new ISLE structure, the associated files and what values ISLE end-users should use for the `.env`, `staging.env`, etc.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Staging ISLE installation is intended for a brand new ISLE site for further Drupal theme development, ingest testing, etc. on a remote ISLE host server.
    * All materials are to be "migrated" from the work you performed on your personal computer from the prior steps and processes in [Local ISLE Installation: New Site](../install/install-local-new.md) instructions.

* You will be using ISLE version **1.2.0** or higher.

* You are using Docker-compose **1.24.0** or higher.

* You have git installed on your personal computer as well as the remote ISLE host server.

* You have already provisioned a remote ISLE hosts server and have documented its IP address.
    * You may have used the [ISLE Ansible script](https://github.com/Islandora-Collaboration-Group/ISLE-Ansible) to accomplish this.
    * If doing this manually, please review the following to ensure the remote Staging ISLE host server has all dependencies e.g. CPU, memory and disk space prior to deploying the ISLE Staging environment profile for deploy
        * [Hardware Requirements](host-hardware-requirements.md)
        * [Software Dependencies](host-software-dependencies.md)
    * This server should be running at the time of deploy.
    * This server has enough disk space to store a large Fedora repository e.g. 1-5 TB or larger depending on how many objects you plan on ingesting.  

* You have already created the two private git repositories for your projects ISLE and Islandora code in [Github](https://github.com), [Bitbucket](https://bitbucket.org/), [Gitlab](https://gitlab.com), etc. having followed Step 2 from the [Local ISLE Installation: New Site](../install/install-local-new.md) instructions. You will continue to use these two git repositories for all environments.
    1. ISLE project config - e.g. `yourprojectnamehere-isle`
    2. Islandora Drupal site code - e.g. `yourprojectnamehere-islandora`

* You have already have the appropriate A record entered into your institutions DNS system and can resolve the Staging domain (https://yourprojectnamehere-staging.institution.edu) using a tool like https://www.whatsmydns.net/

* You have reviewed the [ISLE Installation: Environments](../install/install-environments.md) for more information about suggested Staging values.

* You are familiar with using tools like `scp, cp or rsync` to move configurations, files and data from your local to the remote Staging server.

---

## Index of Instructions

This process will differ slightly from previous builds in that there is work to be done on the local to then be pushed to the Staging ISLE Host server with additional followup work to be performed on the remote Staging ISLE Host server.

The instructions that follow below will have either a `On Local` or a `On Remote Staging` pre-fix to indicate where the work and focus should be. In essence, the git workflow established during the local build process will be extended for deploying on Staging and for future ISLE updates and upgrades.

**Steps 1-6: On Local - Configure the ISLE Staging Environment Profile for Deploy to Remote**

  * Step 1: On Local - Export the Local MySQL Database
      * Export the Local MySQL Islandora Drupal Database
  * Step 2: On Local - Shutdown Any Local Containers & Review Local Code
  * Step 3: On Local - Create New Users and Passwords by Editing "staging.env"
  * Step 4: On Local - Review and Edit "docker-compose.staging.yml"
  * Step 4A: On Local - (Optional) Changes for "docker-compose.staging.yml"
  * Step 5: On Local Staging - If Using Commercial SSLs
  * Step 6: On Local - Commit ISLE Code to Git Repository

**Steps 7-17: On Remote Staging - Configure the ISLE Staging Environment Profile for Launch and Usage**

  * Step 7: On Remote Staging - Git Clone the ISLE Repository to the Remote Staging ISLE Host Server
  * Step 8: On Remote Staging - Create the Appropriate Local Data Paths for Apache, Fedora and Log Data
  * Step 9: On Remote Staging - Clone Your Islandora Code
  * Step 10: On Remote Staging - Copy Over the Local Islandora Drupal Files Directory
  * Step 11: On Remote Staging - If Using Let's Encrypt
  * Step 12: On Remote Staging - Edit the ".env" File to Change to the Staging Environment
  * Step 13: On Remote Staging - Download the ISLE Images
  * Step 14: On Remote Staging - Start Containers
  * Step 15: On Remote Staging - Import the Local MySQL Drupal Database
  * Step 16: On Remote Staging - Run the "fix-permissions.sh" Script
  * Step 17: On Remote Staging - Review and Test the Drupal Staging Site

---

## On Local - Configure the ISLE Staging Environment Profile for Deploy to Remote

## Step 1: On Local - Export the Local MySQL Database

**Prior to attempting this step, please consider the following:**

* If the end user is running multi-sites, there will be additional databases to export.

* Do not export the `fedora3` database

* If possible, on the production Apache webserver, run `drush cc all` from the command line on the local Apache container in the `/var/www/html` directory PRIOR to any db export(s). Otherwise issues can occur on import due to all cache tables being larger than `innodb_log_file_size` allows

### Export the Local MySQL Islandora Drupal Database

* Export the MySQL database for the current Local Drupal site in use and copy it on your local in an easy to find place. In later steps you'll be directed to import this file. **Please be careful** performing any of these potential actions below as the process impacts your newly created and themed new Islandora site. Refer to your `local.env` for the usernames and passwords used below.
    * You can use a MySQL GUI client for this process instead but the command line directions are only included below.
    * Shell into your currently running Local MySQL container
        * `docker exec -it your-mysql-containername bash`
    * Export the Local Islandora Drupal database. Replace the "DRUPAL_DB_USER" and "DRUPAL_DB" in the command below with the values found in your "local.env" file.
        * `mysqldump -u DRUPAL_DB_USER -p DRUPAL_DB > local_drupal_site_082019.sql`
        * Enter the appropriate password: value of `DRUPAL_DB_PASS` in the "local.env")
        * Upon completion, exit the MySQL container  
    * Copy this file from the MySQL container to a location on your personal computer.
        * `docker cp your-mysql-containername:/local_drupal_site_082019.sql /path/to/location`

---

## Step 2: On Local - Shutdown Any Local Containers & Review Local Code

* Ensure that your ISLE and Islandora git repositories have all the latest commits and pushes from the development process that took place on your personal computer. If you haven't yet finished, do not proceed until everything is completed.

* Once finished, open a `terminal` (Windows: open `Git Bash`)
    * Navigate to your Local ISLE repository
    * Shut down any local containers e.g. `docker-compose down`

---

## Step 3: On Local - Create New Users and Passwords by Editing "staging.env"

* Open the "staging.env" file in a text editor.
    * Find each comment that begins with: `# Replace this comment with a ...` and follow the commented instructions to edit the passwords, database and user names.
        * **Review carefully** as some comments request that you replace with `...26 alpha-numeric characters` while others request that you create an `...easy to read but short database name`.
        * It is okay if you potentially repeat the values previously entered for your local `(DRUPAL_DB)` & `(DRUPAL_DB_USER)` in this Staging environment but we strongly recommend not reusing all passwords for environments e.g. `(DRUPAL_DB_PASS)` & `(DRUPAL_HASH_SALT)` should be unique values for each environment.
        * In many cases the username is already pre-populated. If it doesn't have a comment directing you to change or add a value after the `=`, then don't change it.
    * Once finished, save and close the file.

* Open the `config/apache/settings_php/settings.staging.php` file.
    * Find the first comment that begins with: `# ISLE Configuration` and follow the commented instructions to edit the database, username and password.
    * Find the second comment that begins with: `# ISLE Configuration` and follow the instructions to edit the Drupal hash salt.
    * Once finished, save and close the file.

---

## Step 4: On Local - Review and Edit "docker-compose.staging.yml"

* Review the disks and volumes on your remote Staging ISLE Host server to ensure they are of an adequate capacity for your collection needs and match what has been written in the `docker-compose.staging.yml` file.

* Please read through the `docker-compose.staging.yml` file as there are bind mount points that need to be configured on the host machine, to ensure data persistence. There are suggested bind mounts that the end-user can change to fit their needs or they can setup additional volumes or disks to match the suggestions.
    * In the `fedora` service section
        * `- /mnt/data/fedora/datastreamStore:/usr/local/fedora/data/datastreamStore`
        * `- /opt/data/fedora/objectStore:/usr/local/fedora/data/objectStore`
    * In the `apache` service section
        * `- /opt/data/apache/html:/var/www/html`

* Depending on your choice of SSL type (Commercial SSL files or the Let's Encrypt service), you'll need to uncomment only one line of the `traefik` services section. There are also inline instructions to this effect in the `docker-compose.staging.yml` file.
    * To use `Let's Encrypt for SSL`, uncomment:
        * `- ./config/proxy/acme.json:/acme.json`

    * To use commercial SSLs, uncomment:
        * `- ./config/proxy/ssl-certs:/certs:ro`
        * Additionally you'll need to add your SSL certs (.cert, .pem, .key) files to `./config/proxy/ssl-certs`

* Based on the choice of SSL type made above, you'll need to refer to to the `./config/proxy/traefik.staging.toml` file for further configuration instructions.

## Step 4A: On Local - (Optional) Changes for "docker-compose.staging.yml"

This section is for optional changes for the `docker-compose.staging.yml`, end-users do not have feel like they have to make any choices here and can continue to **Step 4** as needed.

The options include PHP settings, Java Memory Allocation, MySQL configuration and use of the [TICK Stack](../optional-components/tickstack.md)

* _(Optional)_ - You can change PHP settings such as file upload limits and memory usage by uncommenting
    * In the `apache` services section, this line:
      * `- ./config/apache/php_ini/php.staging.ini:/etc/php/7.1/apache2/php.ini`
    * You'll then need to make edits in the `./config/apache/php_ini/php.staging.ini` file.

* _(Optional)_ - This line is already uncommented by default in ISLE but we're calling it out here that you can changes to the suggested levels or values within the `./config/mysql/ISLE.cnf` file if needed. When setting up for the first time, it is best practice to leave these settings in place. Over time, you can experiment with further tuning and experimentation based on your project or system needs.

* _(Optional)_ - You can change the suggested `JAVA_MAX_MEM` & `JAVA_MIN_MEM` levels but do not exceed more than 50% of your system memory. When setting up for the first time, it is best practice to leave these settings in place as they are configured for a Staging ISLE Host Server using 16 GB of RAM. Over time, you can experiment with further tuning and experimentation based on your project or system needs.

* _(Optional)_ - You can opt to uncomment the TICK stack settings for monitoring but you'll need to follow the [TICK Stack](../optional-components/tickstack.md) instructions prior to committing changes to your ISLE git repository.
    * All TICK related code can be found at the end of all ISLE services within the `docker-compose.staging.yml` file.
    * **Example:**
```bash
  ## _(Optional)_: Uncomment lines below to run ISLE with the TICK monitoring system
  logging:
    driver: syslog
    options:
      tag: "{{.Name}}"
```

  * Uncomment the lines found in the new TICK stack services section of the `docker-compose.staging.yml` file for hosting of that monitoring service on the Staging ISLE Host server.
      * There are additional configurations to be made to files contained within `./config/tick` but you'll need to follow the [TICK Stack](../optional-components/tickstack.md) instructions prior to committing changes to your ISLE git repository.
  * Uncomment the TICK stack data volumes as well at the bottom of the file.

---

## Step 5: On Local Staging - If Using Commercial SSLs

If you are going to use Let's Encrypt instead, you can skip this step and move onto the next one. There will be additional steps further in this document, to help you configure it.

If you have decided to use Commercial SSL certs supplied to you by your IT team or appropriate resource, please continue following this step.

* Add your Commercial SSL certificate and key files to the `./config/proxy/ssl-certs` directory
    * **Example:**
    * `./config/proxy/ssl-certs/yourprojectnamehere-staging.domain.cert`
    * `./config/proxy/ssl-certs/yourprojectnamehere-staging.domain.key`

* Edit the `./config/proxy/traefik.staging.toml` and follow the in-line instructions. Replace the .pem & .key with the name of your Staging SSL certificate and associated key. Do note the positioning of the added lines. Third character indentation.

**Please note** despite the instruction examples differing on file type, (`.pem` or `cert`), either one is compatible, use what you have been given. Merely change the file type suffix accordingly.

**Example: .cert**
```bash
    [entryPoints.https.tls]
      [[entryPoints.https.tls.certificates]]
      certFile = "/certs/yourprojectnamehere-staging.domain.cert"
      keyFile = "/certs/yourprojectnamehere-staging.domain.key"
```

**Example: .pem**
```bash
    [entryPoints.https.tls]
      [[entryPoints.https.tls.certificates]]
      certFile = "/certs/yourprojectnamehere-staging.institution.edu.pem"
      keyFile = "/certs/yourprojectnamehere-staging.institution.edu.key"
```

---

## Step 6: On Local - Commit ISLE Code to Git Repository

* Once you have made all of the appropriate changes to your Staging profile. Please note the steps below are suggestions. You might use a different git commit message. Substitute `<changedfileshere>` with the actual file names and paths. You may need to do this repeatedly prior to the commit message.
    * `git add <changedfileshere>`
    * `git commit -m "Changes for Staging"`
    * `git push origin master`

---

## On Remote Staging - Configure the ISLE Staging Environment Profile for Launch and Usage

## Step 7: On Remote Staging - Git Clone the ISLE Repository to the Remote Staging ISLE Host Server

* This assumes you have setup an `Islandora` deploy user. If not use a different non-root user for this purpose.

* You will also need to ensure that any `/home/islandora/.ssh/id_rsa.pub` key has been added to your git repository admin panel to allow for cloning from your two private git repositories.

Since the `/opt` directory might not let you do this at first, we suggest the following workaround which you'll only need to do once. Future ISLE updates will not require this step.

* Shell into your Staging ISLE host server as the `Islandora` user.

* Clone your ISLE project repository with the newly committed changes for Staging to the `Islandora` user home directory.
    * `git clone https://yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-isle.git /home/islandora/`
    * This may take a few minutes (2-4) depending on your server's Internet connection.

* Move the newly cloned directory to the `/opt` directory as the root user
    * `sudo mv /home/islandora/yourprojectnamehere-isle /opt/yourprojectnamehere-isle`

* Fix the permissions so that the `islandora` user has access.
    * `sudo chown -Rv islandora:islandora /opt/yourprojectnamehere-isle`

---

## Step 8: On Remote Staging - Create the Appropriate Local Data Paths for Apache, Fedora and Log Data

* Create the `/opt/data` directory
    * `sudo mkdir -p /opt/data`
* Change the permissions to the Islandora user.
    * `sudo chown -Rv islandora:islandora /opt/data`

---

## Step 9: On Remote Staging - Clone Your Islandora Code

* `git clone git@yourgitproviderhere.com/yourinstitutionhere/yourprojectnamehere-islandora.git /opt/data/apache/html`

* Fix the permissions so that the `islandora` user has access.
    * `sudo chown -Rv islandora:islandora /opt/yourprojectnamehere-islandora`

---

## Step 10: On Remote Staging - Copy Over the Local Islandora Drupal Files Directory

This `files` directory should be on your personal computer and should be copied to the remote Staging server.

* Copy `~/yourprojectnamehere-isle/data/apache/html/sites/default/files` to `/opt/data/apache/html/sites/default/files`

* Fix the permissions so that the `islandora` user has access.
    * `sudo chown -Rv islandora:islandora /opt/data/apache/html/sites/default/files`

---

## Step 11: On Remote Staging - If Using Let's Encrypt

If you are using Commercial SSLs, then please stop and move onto the next step.

If using Let's Encrypt, please continue to follow this step.

* Create an empty `acme.json` within the `./config/proxy/ssl-certs/` directory of your ISLE project.
    * `touch /opt/yourprojectnamehere/config/proxy/ssl-certs/acme.json`
    * `chmod 600 /opt/yourprojectnamehere/config/proxy/ssl-certs/acme.json`
    * This file will be ignored by git and won't cause any errors with checking in code despite the location
    * Do note that you may need to open your firewall briefly to allow the SSL certs to be added to the `acme.json` file. This will be indicated in the following steps.
    * Open your firewall to ports 80, 443 prior to starting up the containers to ensure SSL cert creation.

---

## Step 12: On Remote Staging - Edit the ".env" File to Change to the Staging Environment

This step is a multi-step, involved process that allows an end-user to make appropriate changes to the `.env` and then commit it locally to git. This local commit that never gets pushed back to the git repository is critical to allow future ISLE updates or config changes.

* Edit the .env, remove the `local` settings and then commit locally
    * `cd /opt/yourprojectnamehere`
    * `vi / nano / pico /opt/yourprojectnamehere/.env`
    * Edit `COMPOSE_PROJECT_NAME=` and replace the `local` settings with:
      * `COMPOSE_PROJECT_NAME=`  (Suggested) Add an identifiable project or institutional name plus environment e.g. acme_digital_stage`
    * Edit `BASE_DOMAIN=` and replace the `local` settings with:
      * `BASE_DOMAIN=`            (Suggested) Add the full staging domain here e.g. digital-staging.institution.edu
    * Edit `CONTAINER_SHORT_ID=` and replace the `local` settings with:
      * `CONTAINER_SHORT_ID=`     (Suggested) Make an easy to read acronym from the letters of your institution and collection names plus environment e.g. (acme digitalcollections staging) is acdcs
    * Edit `COMPOSE_FILE` change `local` to `staging`
      * `COMPOSE_FILE=docker-compose.staging.yml`
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
    * `git commit -m "Added the edited .env configuration file for Staging. DO NOT PUSH BACK TO UPSTREAM REPOSITORY - Jane Doe 8/2019"`
        * This is a suggested warning for users **NOT TO**  push back this configuration change to the main git repository. If that were done it could conflict with other setups.

* You may run into the following:

```bash
*** Please tell me who you are.

Run

  git config --global user.email "you@example.com"
  git config --global user.name "Your Name"

to set your account's default identity.
Omit --global to set the identity only in this repository.

fatal: empty ident name (for <islandora@yourprojectnamehere-staging.institution.edu>) not allowed  
```

* Configure your server git client but don't use the `--global` setting as that could interfere with other git repositories e.g. your Islandora Drupal code.
    * **Example:** Within `/opt/yourprojectnamehere`
    * `git config user.email "jane@institution.edu"`
    * `git config user.name "Jane Doe"`

* Now re-run the commit command:

```bash
git commit -m "Added the edited .env configuration file for Staging. DO NOT PUSH BACK TO UPSTREAM REPOSITORY - Jane Doe 8/2019"
[master 7ab3fcf9] Added the edited .env configuration file for Staging. DO NOT PUSH BACK TO UPSTREAM REPOSITORY - Jane Doe 8/2019
 1 file changed, 4 insertions(+), 4 deletions(-)
```

---

## Step 13: On Remote Staging - Download the ISLE Images

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_):
* _Using the same open terminal:_
    * Navigate to the root of your ISLE project
    * `cd ~/opt/yourprojectnamehere`
    * `docker-compose pull`

---

## Step 14: On Remote Staging - Start Containers

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

* In your web browser, enter your Staging site URL: `https://yourprojectnamehere-staging.institution.edu`
    * **Please note:** You should not see any errors with respect to the SSL certifications, you should see a nice green lock padlock for the site security. If you see a red error or unknown SSL cert provider, you'll need to shut the containers down and review the previous steps taken especially if using Let's Encrypt. You may need to repeat those steps to get rid of the errors.

---

## Step 15: On Remote Staging - Import the Local MySQL Drupal Database

**Prior to attempting this step, please consider the following:**

* If the end user is running multi-sites, there will be additional databases to export.

* Do not import the `fedora3` database

---

### Import the Local MySQL Islandora Drupal Database

* Copy the `local_drupal_site_082019.sql` created in Step 1 to the Remote Staging server.

* Import the exported Local MySQL database for use in the current Staging Drupal site. Refer to your `staging.env` for the usernames and passwords used below.
    * You can use a MySQL GUI client for this process instead but the command line directions are only included below.
    * Run `docker ps` to determine the MySQL container name
    * Shell into your currently running Staging MySQL container
        * `docker exec -it your-mysql-containername bash`
    * Import the Local Islandora Drupal database. Replace the "DRUPAL_DB_USER" and "DRUPAL_DB" in the command below with the values found in your "staging.env" file.
        * `mysql -u DRUPAL_DB_USER -p DRUPAL_DB < local_drupal_site_082019.sql`
        * Enter the appropriate password: value of `DRUPAL_DB_PASS` in the "staging.env")
        * This might take a few minutes depending on the size of the file.
        * Type `exit` to exit the container

---

## Step 16: On Remote Staging - Run the "fix-permissions.sh" Script

* You'll need to fix the Drupal site permissions by running the `/fix-permissions.sh` script from the Apache container
    * Run `docker ps` to determine the Apache container name
    * Shell into your currently running Staging Apache container
        * `docker exec -it your-apache-containername bash`
        * `sh /utility-scripts/isle_drupal_build_tools/drupal/fix-permissions.sh --drupal_path=/var/www/html --drupal_user=islandora --httpd_group=www-data`
        * This process will take 2-5 minutes
        * You should see a lot of green [ok] messages.
        * If the script appears to pause or prompt for `y/n`, DO NOT enter any values; the script will automatically answer for you.

| For Microsoft Windows: |
| :-------------      |
| You may be prompted by Windows to: |
| - Share the C drive with Docker.  Click Okay or Allow.|
| - Enter your username and password. Do this.|
| - Allow vpnkit.exe to communicate with the network.  Click Okay or Allow (accept default selection).|
| - If the process seems to halt, check the taskbar for background windows.|

* **Proceed only after this message appears:** "Done. 'all' cache was cleared."

---

## Step 17: On Remote Staging - Review and Test the Drupal Staging Site

* In your web browser, enter this URL: `https://yourprojectnamehere-staging.institution.edu`
    * Please note: You should not see any errors with respect to the SSL certifications. If so, please review your previous steps especially if using Let's Encrypt. You may need to repeat those steps to get rid of the errors.

* Log in to the local Islandora site with the credentials ("DRUPAL_ADMIN_USER" and "DRUPAL_ADMIN_PASS") you created in "staging.env".
    * **Please note:** You are free to use previously Drupal admin or user accounts created during the Local site development process.

* You can decide to further QC and review the site as you wish or start to add digital collections and objects.
    * You could also further test using the [Islandora Sample Objects](https://github.com/Islandora-Collaboration-Group/islandora-sample-objects) as you may have done in the previous Local installation.

---

## Next Steps

Once you are ready to deploy your finished Drupal site, you may progress to:

* [Production ISLE Installation: New Site](../install/install-production-new.md)

---

## Additional Resources
* [ISLE Installation: Environments](../install/install-environments.md) help with explaining the ISLE structure, the associated files, and what values ISLE end-users should use for the ".env", "local.env", etc.
* [Local ISLE Installation: Resources](../install/install-local-resources.md) contains Docker container passwords and URLs for administrator testing.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Staging ISLE Installation: New Site
