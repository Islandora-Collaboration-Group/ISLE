<!--- PAGE_TITLE --->

# Installing a Module with Composer

_Note: There is always more than one way to do things, like installing a module, with Docker.  This is just one of many approaches for installing a module using Composer._

_Composer_ is a PHP package/dependency manager employed in Drupal 8 and many other PHP-based applications.  Some Drupal 7, and subsequently Islandora 7.x, modules may also employ _Composer_ to assist with installation and module management.  

_Composer_ is included, or 'baked in', to ISLE's Apache container image.  You can open a shell inside a running ISLE Apache container ([see these instructions for opening a terminal in a container](../appendices/open-terminal-in-running-container.md) to use it.  When you open a shell inside the Apache container you'll initially be logged in as 'root'; however, _Composer_ should not be run as 'root' so once the shell is open you should switch to the _islandora_ user.  Do this by entering `sudo su islandora`, and your prompt should change to something like this: `islandora@dd9ee02aa718:/$`.  You can confirm that Composer is installed and accessible by entering `composer --version`, and you should see something like this in response: `Composer version 1.6.3 2018-01-31 16:28:17`.

## Example: Islandora Multi-Importer (IMI)

The *Islandora Multi-Importer* (https://github.com/mnylc/islandora_multi_importer) module uses _Composer_ for installation.  The remainder of this guide will demonstrate how to install and enable the *Islandora Multi-Importer* or _IMI_.  

The installation instructions for the IMI module tell us to do the following in the container shell:

```
cd /var/www/html/sites/all/modules
git clone https://github.com/mnylc/islandora_multi_importer
cd islandora_multi_importer/
composer install
```

If all goes as planned your container now has the Islandora_Multi-Importer installed, but not enabled.  To enable the module use _Drush_ like so:

```
drush en islandora_multi_importer -y
```

It's also a good idea to flush Drupal's cache after installing and enabling new modules.  You can also do this using _Drush_ like so:

```
drush cc all
```

Other modules which use _Composer_ for installation can be obtained in a similar manner.

## Persistent Changes

Modules installed in this manner essentially become part of the container they're installed in.  If the container is deleted the installation may not persist.  However, in most ISLE configurations, like isle.localdomain, the Apache portion of the governing docker-compose.yml file reads something like this:

```
  apache:
    image: islandoracollabgroup/isle-apache:latest
    container_name: isle-apache-ld
    networks:
      isle-internal:
        aliases:
          - isle.localdomain
    tty: true
    depends_on:
      - mysql
      - fedora
      - solr
    volumes:
      - apache-data-ld:/var/www/html
      - ./data/apache/log/apache:/var/log/apache2
```

The first _volumes_ definition in this portion of the file instructs Docker to map the host's `apache-data-ld` directory to be reflected inside the container as `/var/www/html`.   This means that any changes made inside the container's `/var/www/html` directory will also persist in the host's `apache-data-ld` subdirectory.

IMI's compose specification builds all of the module's components in subdirectories below `/var/www/html`, and the effect is that IMI will persist in ISLE as long as the host's `apache-data-ld` folder is maintained.
