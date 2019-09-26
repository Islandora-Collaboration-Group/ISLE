# Library of Congress standards proxy - High Volume Ingest

## Installation

### Assumptions

* Previously installed and running Local, Production and Staging ISLE Host systems are in place already

* You'll need to use the ISLE images tagged as `1.3.0` and higher.

---

### Installation Instructions

To cache the Library of Congress (LOC) MODS standards, please take these steps below:

* Shut down all running ISLE containers.

#### Edit 1: Add a new Apache Alias

* Within your `docker-compose.*.yml`, add the following alias code snippet to the `Apache` service section under the `networks:` area so that:

**Code snippet:**

```bash
aliases:
          - www.loc.gov
```          

**Example**

```bash
    networks:
      isle-internal:
    depends_on:
```

becomes

**Example**
```bash
    networks:
      isle-internal:
        aliases:
          - www.loc.gov
    depends_on:
```

* Please note the alignment and positioning of the snippet.

---

#### Edit 2: Add a new Apache Bind mount

There is a new directory within the ISLE `config/apache/` directory called `loc_proxy` which contains a file named `loc-proxy-cache.conf`. This file is a new Apache site configruation to enable caching and proxying for `https://www.loc.gov/standards`.

* Within your `docker-compose.*.yml`, add the following bind-mount code snippet to the `Apache` service section under the `volumes` area so that:

**Code snippet:**

```bash
- ./config/apache/loc-proxy-cache.conf:/etc/apache2/sites-enabled/loc-proxy-cache.conf
```          

**Example on a Production system**

```bash
      # Ensure that the values from the production.env have been inserted in the corresponding ISLE configuration sections of the settings.production.php
      - ./config/apache/settings_php/settings.production.php:/var/www/html/sites/default/settings.php
      ## (Optional): Uncomment line below to allow bind mounting of the php.ini file for editing of php memory, upload and max_post values.
      #- ./config/apache/php_ini/php.production.ini:/etc/php/7.1/apache2/php.ini
```

becomes

**Example on a Production system**
```bash
      # Ensure that the values from the production.env have been inserted in the corresponding ISLE configuration sections of the settings.production.php
      - ./config/apache/settings_php/settings.production.php:/var/www/html/sites/default/settings.php
      ## (Optional): Uncomment line below to allow bind mounting of the php.ini file for editing of php memory, upload and max_post values.
      #- ./config/apache/php_ini/php.production.ini:/etc/php/7.1/apache2/php.ini
      - ./config/apache/loc-proxy-cache.conf:/etc/apache2/sites-enabled/loc-proxy-cache.conf
```

* Please note: The examples above are for a Production system but this can be added to any ISLE environment.

---

#### Edit 3: Add a new Apache Host for www.loc.gov

* Within your `docker-compose.*.yml`, add the following host code snippet to the `Apache` service section under the `labels` area so that:

**Code snippet:**

```bash
    ,Host:www-loc.gov
```          

**Example on a Production system**

```bash
    labels:
      - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
      - traefik.enable=true
      - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /cantaloupe"
```

becomes

**Example on a Production system**
```bash
    labels:
      - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
      - traefik.enable=true
      - "traefik.frontend.rule=Host:${BASE_DOMAIN},Host:www-loc.gov; PathPrefix: /, /cantaloupe"
```
* Please note: There is no space between `${BASE_DOMAIN},Host:www-loc.gov;`

---

* Start up the ISLE Docker containers again. `docker-compose up -d`

---

### Testing

* Shell into the Apache container
  * `docker exec -it isle-apache-{{ container id }} bash`
  * `curl -O http://www.loc.gov/standards/mods/xml.xsd`
  * `curl http://www.loc.gov/standards/mods/xml.xsd`
  * `ls -lha /var/cache/apache2/mod_cache_disk/` - Is there a new hash created?
  * `echo "TEST 123 TEST 123" >> /var/cache/apache2/mod_cache_disk/Fr/m6/gz8YIqKWS_tQjfBGgw.header.vary/HM/wl/M1FJKOuWg0YokaKfCA.data` - Check if the file is being pulled locallly from the cache by editing it.
  * `curl http://www.loc.gov/standards/mods/xml.xsd` - you should now see `TEST 123 TEST 123` at the bottom of the file.

---

## Need help?

* Please use the following as resources for institutions or endusers needing support

  * [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody!
    * The [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT

  * [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support

  * [ISLE Github Issues queue](https://github.com/Islandora-Collaboration-Group/ISLE/issues) - Post your issues, bugs and requests for technical documentation here.

---