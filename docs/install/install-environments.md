# Overview: ISLE Installation Environments

As of the ISLE `1.2.0` release, ISLE has the option to use clearly defined but different environments based on enduser's needs. Depending on the environment choice, additional configuration changes will need to be made to ISLE configuration files, domain names, etc. We recommend that you install ISLE using the workflow and suggested order below.

* **Demo** (Default) - Used for trying out ISLE for the first time, having a sandbox for tests etc

* **Local** - Used for Drupal site development, more extensive metadata, Solr and Fedora development work on a Local personal computer

* **Staging** - Used for remote (non-local or cloud) hosting of a Staging site. Islandora Drupal site code here is almost finished but viewed privately prior to pushing to public Production. Fedora data might have tests collections.

* **Production** - Used for remote (non-local or cloud) hosting of a Production site. Site and collections are public and accessible to the world. Drupal site code is finished, functional and polished. Fedora data has full non test collections. Test collections are not recommended here.

* **Test** - Used by ISLE Maintainers for testing ISLE releases, pull releases and ISLE development, etc.

## ISLE Project Structure

The ISLE project `.env` file enables you to be define and launch a variety of ISLE environments. On each ISLE environment, you will edit the `.env` file to point to a corresponding `docker-compose.*.yml` file configured for that environment. The root of the ISLE project (ISLE version 1.2.0+) comes with multiple, pre-configured examples of `docker-compose` and `.env` files to match the variety of environments shown below. The outline below lists the files that you may have to edit, change or add to support each ISLE environment. 

* `.env` - 
  
    * **Demo** (Default)
        * `docker-compose.demo.yml`
            * `demo.env`
            * `config/apache/php_ini/php.demo.ini`
            * `config/mysql/ISLE.cnf`
            * `config/proxy/ssl-certs/isle.localdomain.cert`
            * `config/proxy/ssl-certs/isle.localdomain.key`
            * `config/proxy/traefik.demo.toml`
    
    * **Local** 
        * `docker-compose.local.yml`
            * `local.env`
            * `config/apache/php_ini/php.local.ini`
            * `config/apache/settings_php/settings.local.php`
            * `config/mysql/ISLE.cnf`
            * `config/proxy/ssl-certs/yourprojectnamehere.localdomain.cert`
            * `config/proxy/ssl-certs/yourprojectnamehere.localdomain.key`
            * `config/proxy/traefik.local.toml`
    
    * **Staging**
        * `docker-compose.staging.yml`
            * `staging.env`
            * `config/apache/php_ini/php.local.ini`
            * `config/apache/settings_php/settings.local.php`
            * `config/mysql/ISLE.cnf`
            * `config/proxy/ssl-certs`
                * If using Let's Encrypt:
                    * `acme.json`
                * If using Commercial SSL Certs:
                    * `yourprojectnamehere.domain-staging.cert`
                    * `yourprojectnamehere.domain-staging.key`
            * `config/proxy/traefik.staging.toml`

    * **Production**
        * `docker-compose.production.yml`
            * `production.env`
            * `config/apache/php_ini/php.production.ini`
            * `config/apache/settings_php/settings.production.php`
            * `config/mysql/ISLE.cnf`
            * `config/proxy/ssl-certs`
                * If using Let's Encrypt:
                     * `acme.json`
                * If using Commercial SSL Certs:
                     * `yourprojectnamehere.domain-production.cert`
                     * `yourprojectnamehere.domain-production.key`
            * `config/proxy/traefik.production.toml`

    * **Test**
        * `docker-compose.test.yml`
            * `test.env`
            * `config/apache/php_ini/php.test.ini`
            * `config/apache/settings_php/settings.test.php`
            * `config/mysql/ISLE.cnf`
            * `config/proxy/ssl-certs/isle.localdomain.cert`
            * `config/proxy/ssl-certs/isle.localdomain.key`
            * `config/proxy/traefik.test.toml`

---

## ISLE Environments ".env" reference

This reference includes a combination of suggestions and actual values. You may use these environment descriptions or settings in each of your corresponding ISLE project `.env` files.

### Demo

* Used for trying out ISLE for the first time, having a sandbox for tests, etc.

* The Demo environment is the "default" activated environment.

```bash
COMPOSE_PROJECT_NAME=isle_demo
BASE_DOMAIN=isle.localdomain
CONTAINER_SHORT_ID=ld
COMPOSE_FILE=docker-compose.demo.yml
```

---

### Local

* Used for Drupal site development, more extensive metadata, Solr and Fedora development work on a personal computer

* To use this environment, change the values in the Activated ISLE environment to match the values described below and save the file.

* Inline comments below are merely suggestions except for the mandatory `COMPOSE_FILE` value. Otherwise change however you see fit. Do not copy the comments (#) below when editing the main `.env`, simply add the suggested or mandatory values as directed to the right of any `=`.

```bash
COMPOSE_PROJECT_NAME=                 # (Suggested) Add an identifiable project or institutional name plus environment e.g. acme_digital_local
BASE_DOMAIN=                          # (Suggested) This is not a real domain and can be anything, so add your institution name only e.g. acme.localdomain
CONTAINER_SHORT_ID=ld                 # (Mandatory)
COMPOSE_FILE=docker-compose.local.yml # (Mandatory) The docker-compose file used for configuring and launching the Local environment.
```

---

### Staging

* Used for remote (non-local or cloud) hosting of a Staging site. Code here is almost finished but viewed privately prior to pushing to public Production.

* Please read through the `docker-compose.staging.yml` file as there are bind mount points that need to be configured on the host machine, to ensure data persistence. There are suggested bind mounts that the end-user can change to fit their needs or they can setup additional volumes or disks to match the suggestions.

* To use this environment, change the values in the Activated ISLE environment lines to match the values described below and save the file.

* Inline comments below are merely suggestions except for the mandatory `COMPOSE_FILE` value. Otherwise change however you see fit. Do not copy the comments (#) below when editing the main `.env`, simply add the suggested or mandatory values as directed to the right of any `=`.

```bash
COMPOSE_PROJECT_NAME=                   # (Suggested) Add an identifiable project or institutional name plus environment e.g. acme_digital_stage
BASE_DOMAIN=                            # (Suggested) Add the full production domain here e.g. digital-staging.institution.edu
CONTAINER_SHORT_ID=                     # (Suggested) Make an easy to read acronym from the letters of your institution and collection names plus environment e.g. (acme digitalcollections staging) is acdcs
COMPOSE_FILE=docker-compose.staging.yml # (Mandatory) The docker-compose file used for configuring and launching the Staging environment.
```

---

### Production

* Used for remote (non-local or cloud) hosting of a Production site. Site and collections are public and accessible to the world.

* Please read through the `docker-compose.production.yml` file as there are bind mount points that need to be configured on the host machine, to ensure data persistence. There are suggested bind mounts that the end-user can change to fit their needs or they can setup additional volumes or disks to match the suggestions.

* To use this environment, change the values in the Activated ISLE environment to match the values described below and save the file.

* Inline comments below are merely suggestions except for the mandatory `COMPOSE_FILE` value. Otherwise change however you see fit. Do not copy the comments (#) below when editing the main `.env`, simply add the suggested or mandatory values as directed to the right of any `=`.

```bash
COMPOSE_PROJECT_NAME=                      # (Suggested) Add an identifiable project or institutional name plus environment e.g. acme_digital_prod
BASE_DOMAIN=                               # (Suggested) Add the full production domain here e.g. digital.institution.edu
CONTAINER_SHORT_ID=                        # (Suggested) Make an easy to read acronym from the letters of your institution and collection names plus environment e.g. (acme digitalcollections prod) is acdcp
COMPOSE_FILE=docker-compose.production.yml # (Mandatory) The docker-compose file used for configuring and launching the Production environment.
```

---

### Test

* Used by ISLE Maintainers for testing ISLE releases, pull releases and ISLE development, etc.

* To use this environment, change the values in the Activated ISLE environment to match the values described below and save the file.

* All values below are mandatory. Do not change. Simply fill them in as directed. Do not copy the comments (#) below when editing the main `.env`, simply add the suggested or mandatory values as directed to the right of any `=`.

```bash
COMPOSE_PROJECT_NAME=isle_test       # (Mandatory)
BASE_DOMAIN=isle.localdomain         # (Mandatory)
CONTAINER_SHORT_ID=td                # (Mandatory)
COMPOSE_FILE=docker-compose.test.yml # (Mandatory) The docker-compose file used for configuring and launching the Test environment.
```

---
