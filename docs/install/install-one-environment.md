# New ISLE Installation

_Expectations:  It may take at least **4 - 6 hours or more** to read this documentation and complete this installation. Please proceed slowly._

This New ISLE Installation creates an Islandora environment with a unique domain (URL) on your host server, virtual machine, or cloud hosted platform.
You may also use this documentation to install ISLE on several host servers, virtual machines, or cloud hosted platforms to create multiple Islandora environments (e.g. development, staging, and production). This process will result in an un-themed Drupal website and an empty Fedora repository. (Please refer back to the [Hardware Requirements](../install/host-hardware-requirements.md) as the development environment needs fewer resources than the staging and production environments; the latter two should mirror each other in resource usage and setup.)

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

---


## Assumptions / Prerequisites

* You have already completed the [Hardware Requirements](../install/host-hardware-requirements.md) and the [Software Dependencies](../install/host-software-dependencies.md) for your host server(s). 

* SSL Certificates: Use the [Let's Encrypt guide](../appendices/configuring-lets-encrypt.md) to generate SSL Certificates or ask your IT resource to provision [SSL Certificates](../appendices/glossary.md#systems) for the web domain.

* **Never ever share or post your .env files publicly.** The .env and tomcat.env files ("Docker Environment files") are your primary resources for customizing your ISLE stack. These .env files contain passwords and usernames and must be treated with the utmost care.




## Docker Environment Files


### Master Section
    * COMPOSE_PROJECT_NAME to something unique (e.g. `COMPOSE_PROJECT_NAME=isle-production-collections`)
      * This variable is appended to Docker objects like: volume names, network names.
    * BASE_DOMAIN to your domainname (e.g. `BASE_DOMAIN=digital-collections.example.edu`)
      * This variable specifies your domain name!
    * CONTAINER_SHORT_ID to something unique (e.g. `CONTAINER_SHORT_ID=prod`).
      * This variable is appended to the end of all running containers, keep it _short_!


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


### Database Section
| .env Variable                   | Purpose                               | ISLE Services updated  | What it does                                                                                                                            |
|-------------------------------  |-------------------------------------  |------------------------------------ |---------------------------------------------------------------------------------------------------------------------------------------  |
| MYSQL_ROOT_PASSWORD             | Set the `root` password for MySQL     | MySQL, Fedora, Apache               | Allows Fedora and Apache to update their relevant databases as well as to configure themselves to work together.                        |
| FEDORA_DB                       | Set the name of Fedora database       | MySQL, Fedora                       | Specifies which database to create or use for Fedora data.                                                                              |
| DRUPAL_DB                       | Set the name of the Drupal database   | MySQL, Apache, Fedora               | Specifies which database to create or use for Fedora data. Updates components of Fedora so it can read the Drupal database for users.   |
| FEDORA_DB_USER DRUPAL_DB_USER   | Sets the MySQL user                   | MySQL, Apache, Fedora               | Specifies names of Database users.                                                                                                      |
| FEDORA_DB_PASS DRUPAL_DB_PASS   | Sets MySQL user passwords             | MySQL, Apache, Fedora               | Specifies passwords of Database users.                                                                                                  |

### Islandora (Drupal) Section
| .env Variable       | Purpose                                     | ISLE Services updated  | What it does                                                                                                                                                                                    |
|-------------------- |-------------------------------------------  |------------------------------------ |------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| DRUPAL_SITE_NAME    | Sets the Drupal Site Name variable          | Drupal                              | Sets the name of your Islandora Website; e.g.:  "<Your Group's Name> Digital Collections"                                                                                                       |
| DRUPAL_ADMIN_USER   | Set the name of Drupal admin                | Drupal                              | Specifies the 'admin user' for your Islandora website.                                                                                                                                          |
| DRUPAL_ADMIN_PASS   | Set the password of Drupal admin            | Drupal                              | Specifies the password of 'admin user' for your Islandora website.                                                                                                                              |
| DRUPAL_ADMIN_EMAIL  | Set the email of Drupal admin               | Drupal                              | Specifies the email address of the 'admin user' for your Islandora site.                                                                                                                        |
| DRUPAL_HASH_SALT    | Secures your installation by hashing data   | Drupal                              | Secures your install of Drupal (Islandora) by hashing (obscuring) key data. Use [password generator tool](https://passwordsgenerator.net/) to create a HASH_SALT, remember alphanumeric characters ONLY (no special characters).  |

### Fedora Repository Section
| .env Variable         | Purpose                           | ISLE Services updated           | What it does                                                                                                                                                |
|---------------------  |---------------------------------- |-------------------------------  |------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| FEDORA_ADMIN_USER     | Set the name of Fedora admin      | Fedora                          | Sets the master administrator username to login to the Fedora repository. You login with this user.                                                         |
| FEDORA_ADMIN_PASS     | Set the password of Fedora admin  | Fedora                          | Sets the master administrator password to login to the Fedora repository.                                                                                   |
| FEDORA_GSEARCH_USER   | Sets the username for FGS         | Fedora, Fedora Generic Search   | Sets the Fedora Generic Search (FGS) username used to login to the Fedora repository. FGS uses this user to connect to Fedora to update your search index.  |
| FEDORA_GSEARCH_PASS   | Sets the password for FGS         | Fedora, Fedora Generic Search   | Sets the password for FGS to gain access to your repository.                                                                                                |
| FEDORA_INTCALL_USER   | Sets the internal call user       | Fedora                          | The internal call username                                                                                                                                  |
| FEDORA_INTCALL_PASS   | Sets the internal call password   | Fedora                          | The internal call password                                                                                                                                  |

### Image Services Section
| .env Variable                       | ISLE Services updated   | What it does                                                                                                                          |
|-----------------------------------  |-----------------------  |-------------------------------------------------------------------------------------------------------------------------------------- |
| CANTALOUPE_ADMIN_INTERFACE_ENABLE   | ImageServices           | Enables or Disables the Cantaloupe IIIF /admin control panel. Located at http://hostip:8083/cantaloupe/admin when true, else false.  |
| CANTALOUPE_ADMIN_USER               | ImageServices           | Set the admin username to login to the admin panel.                                                                                   |
| CANTALOUPE_ADMIN_PASS               | ImageServices           | Set the admin password to login to the admin panel.                                                                                   |

### Tomcat.env Applies to All Tomcat Instances
| Tomcat.env Variable   | ISLE Services updated         | What it does                                            |
|---------------------  |-----------------------------  |-------------------------------------------------------  |
| TOMCAT_ADMIN_USER     | Fedora, Solr, ImageServices   | Set the admin username to login to the admin panel.     |
| TOMCAT_ADMIN_PASS     | Fedora, Solr, ImageServices   | Set the admin password to login to the admin panel.     |
| TOMCAT_MANAGER_USER   | Fedora, Solr, ImageServices   | Set the manager username to login to the admin panel.   |
| TOMCAT_MANAGER_PASS   | Fedora, Solr, ImageServices   | Set the manager password to login to the admin panel.   |

---

## Config Directory

The config directory has many purposes like holding customized configuration files mounted to specific containers (which we have no covered here), but for a single simple site we only use it to hold our proxy configs and as a place to store our SSL Certificate and Key.

---

## Edit File `docker-compose.yml`

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

## Proxy Directory

If need be, please refer to the **Systems** section of the [Glossary](../appendices/glossary.md) for relevant terms to help guide installation.

The `proxy` subdirectory contains all specific configurations necessary for the Traefik proxy to function properly with your changes.

If need be, please refer to the **SSL certificate** section of the [Glossary](../appendices/glossary.md) for relevant terms to help guide installation.

There are also additional links for the end user to learn how to combine the SSL Certificate File with any available SSL Certificate Chain File for the `proxy` process to work properly.

**If you followed the [Let's Encrypt](../appendices/configuring-lets-encrypt.md) configuration guide, you can skip to the next section: Spin up ISLE containers**

* Copy your SSL certificates for the ISLE Proxy into `config/proxy/ssl-certs`. They will and should have different names than the examples provided below.

    * There can only be 2 files involved in this process.

        * 1 x SSL Certificate Key File e.g. `newsite-sample-key.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.key` or `.pem`

        * 1 x SSL Certificate File e.g. `newsite-sample.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

* Edit the `config/proxy/traefik.toml` file:
  * Change line 27 and 28:
    *  `certFile = "/certs/newsite-sample.cert"`  ## Change to reflect your CERT, CRT, or PEM
    *  `keyFile = "/certs/newsite-sample-key.key"`  ## Change to reflect your KEY, or PEM.


---

## Spin up ISLE Containers!

* Run `docker-compose up -d`

* Wait for the stack to completely initialize, about 5 minutes (typically much less).

* Run `docker exec -it isle-apache-<CONTAINER_SHORT_ID> bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh`
     * Using the <CONTAINER_SHORT_ID> value from the .env file here (_typically: isle-apache-prod, or -stage, -dev, etc._)

* Give this process 15 - 25 minutes (_depending on the speed of the ISLE Host server internet connection_)

* Check the newly created and running new site by opening a browser and navigating to your site domain e.g. `https://digital-collections.example.edu`, you should now see an un-themed Drupal site.

---


(Good points for somewhere else:)

- ISLE removes the need to manually edit the more complex config files that are part of the Islandora stack. 

---

## Additional Resources
* [Migrate to ISLE Environment](../migrate/install-migration.md) will help you migrate your existing production Islandora 7.x environment to an ISLE environment.
* [Demo ISLE Installation: Resources](../install/install-demo-resources.md) contains Docker container passwords and URLs for administrator tools.
* [Demo ISLE Installation: Troubleshooting](../install/install-demo-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

## End of New ISLE Installation.
