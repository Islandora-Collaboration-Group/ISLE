# Remote Server ISLE Installation

_Expectations:  It may take at least **4-6 hours or more** to read this documentation and complete this installation. Please proceed slowly._

This Remote Server ISLE Installation creates an Islandora environment with a unique domain (URL) on your host server, virtual machine, or cloud hosted platform.
You may also use this documentation to install ISLE on several host servers, virtual machines, or cloud hosted platforms to create multiple Islandora environments (e.g. development, staging, and production). This process will result in an un-themed Drupal website and an empty Fedora repository. (Please refer back to the [Hardware Requirements](../install/host-hardware-requirements.md) as the development environment needs fewer resources than the staging and production environments; the latter two should mirror each other in resource usage and setup.)

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

---

## Assumptions / Prerequisites

* You have already completed the [Hardware Requirements](../install/host-hardware-requirements.md) and the [Software Dependencies](../install/host-software-dependencies.md) for your host server. 

* SSL Certificates: You will either ask your IT department to provision [SSL Certificates](../appendices/glossary.md#systems) or you will use the [Let's Encrypt: Free SSL Certificates](../appendices/configuring-lets-encrypt.md) to generate SSL certificates for the ISLE environment.

* **Never ever share or post your .env files publicly.** The .env and tomcat.env files ("Docker Environment files") are your primary resources for customizing your ISLE stack. These .env files contain passwords and usernames and must be treated with the utmost care.

---

## THIS SECTION IS IN DEVELOPMENT. PLEASE COME BACK SOON.

---

## Step 1: Setup GIT Workflow

### Create Two Private Institutional GIT Repositories

Note: Since public forks can’t be made private, we will duplicate (instead of forking) the repository to enable them to be private.

1. Create a new PRIVATE repository on your institution's GitHub (or Bitbucket) account.
2. Name it: `[project-name]-ISLE` (example: `digital-ISLE`). Leave this repository empty.
3. Create another new PRIVATE repository on your institution's GitHub (or Bitbucket) account.
4. Name it: `[project-name]-islandora` (example: `digital-islandora`). Leave this repository empty.

### Set your Remotes: Upstream and Origin

* Open a terminal and navigate to `/opt/ISLE` (or where you cloned the ISLE directory).
* Type `git remote -v`
* You want to have four lines returned, two that describe your private repository (called `origin`) and two that describe the source ISLE github site (called `upstream`):

```
origin  https://github.com/your-github-name/ISLE.git (fetch)
origin  https://github.com/your-github-name/ISLE.git (push)
upstream        https://github.com/Islandora-Collaboration-Group/ISLE.git (fetch)
upstream        https://github.com/Islandora-Collaboration-Group/ISLE.git (push)
```

* If the name of the `https://github.com/Islandora-Collaboration-Group/ISLE.git` is not `upstream`, then please correct that:
    * Remove the non-upstream name: `git remote rm non-upstream-name-here`
    * Rename it to be "upstream": `git remote add upstream https://github.com/Islandora-Collaboration-Group/ISLE`
  
* Add your private insitutional repository as the remote `origin`.
    * In the line below, substitute `your-institution` and "project-name", then enter it in your terminal.
    `git remote add origin https://github.com/your-institution/project-name-ISLE`

* Pull from the remote upstream master and update the remote origin master branch.
    * Pull from upstream: `git pull upstream master`
    * Git will ask you to authenicate with username and password:
        * $> Username for 'https://github.com': your-github-name
        * $> Password for 'https://your-github-name@github.com': xxxxx
        * TODO: Lookup how to have your server remember your pwd and link to that here.
        * TODO: Do you have 2-factor auth set up for github?
            * No - okay, enter your github account username and pwd
            * Yes - have to setup keys (link to how to do that..)

    * Push to origin: `git push -u origin master` (-u is a one-time command that sets up tracking for branches)

* Done: You have now setup your server to consider ICG as upstream and "[project-name]-ISLE" as origin.
  
* General outline of Git Workflow:
    * Work on local
    * Pull from upstream master
    * Push to origin master
    * On development, staging, and production environments: pull from your single institutional git repository (e.g. williamscollege/unbound-ISLE)


## Step 2: Edit the Docker Environment Files

The .env and tomcat.env Docker files are located in the folder with the docker-compose.yml, and are key to configuring the ISLE stack to suit your needs.

Edit `.env` (found in root of ISLE directory). For example, a production server might be described as:

  * `COMPOSE_PROJECT_NAME=isle-production`
      * This variable is appended to Docker objects like volume and network names.
  * `BASE_DOMAIN=mydomain.edu`
      * This variable specifies your domain name.
  * `CONTAINER_SHORT_ID=ld`
      * Okay to leave this as is. Keep this variable short as it is appended to the end of all running containers.

Edit `tomcat.env` (found in root of ISLE directory). For example, a production server might be described as:

  * `TOMCAT_ADMIN_USER=admin`
  * `TOMCAT_ADMIN_PASS=xxxxxxxxxxxxxx` (enter a secure password)
  * `TOMCAT_MANAGER_USER=manager`
  * `TOMCAT_MANAGER_PASS=xxxxxxxxxxxxxx` (enter a secure password)

<!-- 
TODO: 

FLOW of Customization:

If New Installation....
- Explain that ISLE creates the mysql, fedora, etc. servers from Docker images, and that data for these can be stored with persistence on a bind mount.
  - Explain how to update usernames and passwords for mysql, fedora, etc.

If Migrate to ISLE Environment...
- Explain how to find pwds, how to rsync data:
  - mysql, fedora installations on bind mount
  - usernames and passwords for mysql, fedora, etc.

Explain Git Workflow and how to setup

Explain how to setup bind mounts OR volumes? (are these docker volumes?)

-->

[ISLE Environment Files](../appendices/environment-files.md) - This page lists all the .env variables and corresponding user names and passwords used throughout the Islandora stack.

---

## Step 5: Edit File `docker-compose.yml`

**For Production and Staging Servers Only:** Open the `docker-compose.yml` file and modify the environment variables called JAVA_MAX_MEM and JAVA_MIN_MEM for fedora, solr, and image-services.

```yaml
fedora:
  ...
  environment:
    - JAVA_MAX_MEM=2048M
    - JAVA_MIN_MEM=256M
...

solr:
  ...
  environment:
    - JAVA_MAX_MEM=2048M
    - JAVA_MIN_MEM=256M
...

image-services:
  ...
  environment:
    - JAVA_MAX_MEM=2048M
    - JAVA_MIN_MEM=128M
```

---

## Step 7: Config Directory

The `./ISLE/config/` directory, along with the `proxy` and `ssl-certs` subdirectories, store the customized configuration files necessary (i.e. proxy settings, SSL Certificate and Key) for the Traefik proxy to interact properly with your web domain (URL). The Docker environment file (.env) bind mounts these subdirectories to communicate with precise locations within the Docker containers. 


### SSL Certificates

A Remote Server ISLE Installation requires that you provision and place a complete [SSL Certificate Chain](https://support.dnsimple.com/articles/what-is-ssl-certificate-chain/) into the `./ISLE/config/proxy/ssl-certs` directory. These SSL certificates are used by the `apache` & `proxy` Docker containers.

**Generate SSL certificates by using ONE of the following two methods:**

#### Method 1:
1. Ask your IT department to provision a complete [SSL Certificate Chain](https://support.dnsimple.com/articles/what-is-ssl-certificate-chain/).
2. Copy these SSL certificates into the `./ISLE/config/proxy/ssl-certs` directory.
    * You are required to have exactly two files: (names will differ from examples)
        * (1) SSL Certificate Key File (file extension may be: ".key" or ".pem")
            * **Example:** `sample-key.key` or `sample-key.pem`
        * (1) SSL Certificate File (file extension may be: ".cer", ".crt" or ".pem")
            * **Example:** `sample.cer` or `sample.crt` or `sample.pem`
3. Edit the `./ISLE/config/proxy/traefik.toml` file.
    * Change lines:
      *  `certFile = "/certs/sample.cert"`  ## Change to reflect your ".cer", ".crt" or ".pem"
      *  `keyFile = "/certs/sample-key.key"`  ## Change to reflect your ".key" or ".pem"
4. Proceed to the section below: [Step 9: Spin up ISLE Containers](../install/_obsolete_install-server.md#step-9-spin-up-isle-containers).

####Method 2:
1. Use [Let's Encrypt](https://letsencrypt.org/), a free, automated, and open Certificate Authority for generating SSL certificates for your ISLE environment. In order to get a certificate for your website’s domain from Let’s Encrypt, you have to demonstrate control over the domain. With Let’s Encrypt, you do this using software that uses the ACME protocol, which typically runs on your web host.
2. Complete the [Let's Encrypt: Free SSL Certificates](../appendices/configuring-lets-encrypt.md) ISLE guide.
3. Proceed to the section below: [Step 9: Spin up ISLE Containers](../install/_obsolete_install-server.md#step-9-spin-up-isle-containers).

---

## Step 9: Spin up ISLE Containers

* Download and start all ISLE Docker images (_~6 GB of data may take 5-10 minutes_):
```
docker-compose up -d
```

* Wait for the stack to completely initialize.


This process may take 10-20 minutes (_depending on system and internet speeds_)

* Run the install site script on the Apache container by copying and pasting this command:
[TODO: A server install should not be told to run this script: we want the install to be persistent, not ephemeral.]
    * `docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh`

* Check the newly created and running new site by opening a browser and navigating to your site domain e.g. `https://project-name.yourdomain.edu`, you should now see an un-themed Drupal site.

---


(Good points for somewhere else:)

- ISLE removes the need to manually edit the more complex config files that are part of the Islandora stack. 
- The `./ISLE/config/proxy/ssl-certs` directory for a Demo ISLE Installation is what holds our proxy configs and is where we store our SSL Certificate and Key.

---

## Additional Resources
* TBD: [Demo ISLE Installation: Resources](../install/install-demo-resources.md) contains Docker container passwords and URLs for administrator testing.
* TBD: [Demo ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

## End of Remote Server ISLE Installation.

---

## Migrate to ISLE Environment 
* Please click [Migrate to ISLE Environment]() if you are installing ISLE to migrate your existing production Islandora 7.x environment to an ISLE environment. This documentation will help you identify and copy your institution's preexisting Islandora data, files, and themes (including your data volume, Drupal site(s) and theme(s), and commonly customized xml and xslt files) to your ISLE environment.