<!--- PAGE_TITLE --->

# Installing a Module with Drush (and Git)

_Note: There is always more than one way to do things, like installing a module, with Docker.  This is just one of many approaches for installing a module using Drush, and in some cases, Git.  This technique works, in Islandora version 7.x, for most Drupal and Islandora modules._

_Drush_, a "Command Line Shell for Drupal", provides convenient means of installing, configuring, and maintaining a Drupal site (like Islandora).  Please see http://www.drush.org/ for much more detail regarding Drush.

_Git_ is a version control tool that I hope all readers of this document are already familiar with.  If you are not, please consider studying up on it at https://git-scm.com/.

_Drush_ and _Git_ are included, or 'baked in', to ISLE's Apache container image.  You can open a shell inside a running ISLE Apache container ([see these instructions](../appendices/open-terminal-in-running-container.md)) to use them.  When you open a shell inside the Apache container you'll initially be logged in as 'root'; however, most _Drush_ and _Git_ operations should not be performed as 'root' so once the shell is open you should switch to the _islandora_ user.  Do this by entering `sudo su islandora`, and your prompt should change to something like this: `islandora@dd9ee02aa718:/$`.  

You can confirm that Drush is installed and accessible by entering `drush --version`, and you should see something like this in response: `Drush Version   :  7.4.0`.  Likewise, you can confirm that Git is installed and working by entering `git --version`, and you should see something like this in response: `git version 1.9.1`.

## Two Kinds of Drush Module Installations

There may be many kinds of modules that can be installed and configured using Drush, but the most popular are:

* Drupal Contributed Modules - These are modules developed specifically for Drupal and made available from a repository maintained by Drupal.org.  Documentation for all such Drupal 7.x modules can be found at https://www.drupal.org/docs/7/modules.  Note that all modules are version-specific, so you can find Drupal 8.x module documentation at https://www.drupal.org/docs/8/modules.  

* Non-Contributed Modules - Modules that are not offered to, or accepted by, Drupal.org may reside in repositories maintained by the module's author and/or maintainer.  Most Islandora-specific modules take this form.

These two kinds of modules generally employ different techniques for installation and maintenance.  One example of each is presented in the sections that follow.

## Example: `Devel` - A Drupal Contributed Module
_Devel_ is a Drupal contributed module maintained and distributed by Drupal.org.  Its stated purpose, taken directly from the module's README file is: "...contains helper functions and pages for Drupal developers and inquisitive admins...".

The installation instructions for _Devel_ simply tell us to "install as usual", that is generally true of most Drupal.org contributed modules, and the process is extremely simple using Drush.  From a terminal in the container shell:

```
cd /var/www/html/web/sites/default
drush dl devel
drush en devel
drush cc all
```
In this command sequence, the `cd /var/www/html/sites/default` command sets our working directory to correspond with the location of our site's `settings.php` file.  This directory, and file, hold key information about our site, and working from this directory makes subsequent commands easier to use.

The `drush dl devel` command instructs our container to download, `dl` is Drush shorthand for 'download', the module identified as `devel`.  Drush will automatically determine where to put the downloaded module, but it may prompt you for confirmation of location depending on your circumstances.

The third command, `drush en devel`, instructs our container to 'enable', `en` is Drush shorthand for 'enable', the module identified as `devel`.  This command, as specified, must be run from a particular working directory so that Drush can determine in which Islandora Drupal site the module should be applied.  Like the `dl` command, `drush en` may prompt you to confirm certain operations as it attempts to resolve any dependencies that the module may have.  

The final command, `drush cc all`, instructs our container to 'clear all caches', `cc` is Drush shorthand for 'clear cache. This command is not required, but it is highly recommended, since Drupal and Islandora typically cache a great deal of module information, and failing to clear caches may cause temporary, but unexpected results after a new module is installed.

That's it.  Your new module is installed and ready to be configured and used.  You can visit 'https://isle.localdomain/admin/modules' in your browser to manage the module's configuration, permissions, and more.

## Example: Oral Histories Solution Pack - A Non-Contrib Islandora Module

According to its README.md file, the [*Oral Histories Solution Pack*](https://github.com/Islandora-Labs/islandora_solution_pack_oralhistories) "Provides a content model for Oral Histories and a viewer for displaying timed text content (XML or WebVTT) alongside video and audio files."  The remainder of this guide will demonstrate how to install and enable the *Oral Histories Solution Pack* inside your running Apache container using Git and Drush.

The full installation instructions for the Oral Histories Solution Pack are relatively complex since there are a number of dependencies, and some necessary Solr configuration in order to be fully functional.  Please see the [README](https://github.com/Islandora-Labs/islandora_solution_pack_oralhistories/blob/7.x/README.md) for complete details.

To begin the process of downloading, installing and configuring the Oral History Solution Pack do the following at the terminal inside your container:

```
cd /var/www/html/sites/all/modules/contrib
git clone https://github.com/Islandora-Labs/islandora_solution_pack_oralhistories.git
cd /var/www/html/sites/default
drush en islandora_oralhistories -y
drush cc all
```

In this command sequence, the `cd /var/www/html/sites/all/modules/contrib` command sets our working directory to correspond with the location where we want our module to reside.  

The `git clone...` command downloads the Oral History solution pack code from its repository on GitHub. If all goes smoothly this step will create a new `islandora_solution_pack_oralhistories` directory.

The `cd /var/www/html/sites/default` command sets our working directory to the location of our site's `settings.php` file.  This directory, and file, hold key information about our site, and working from this directory makes subsequent commands easier to use.

The command, `drush en islandora_oralhistories -y`, instructs our container to 'enable', `en` is Drush shorthand for 'enable', the module identified as `islandora_oralhistories`.  This command, as specified, must be run from a particular working directory so that Drush can determine in which Islandora Drupal site the module should be applied.  Note that `drush en` may prompt you to confirm certain operations as it attempts to resolve any dependencies that the module may have.  

The final command, `drush cc all`, instructs our container to 'clear all caches', `cc` is Drush shorthand for 'clear cache. This command is not required, but it is highly recommended, since Drupal and Islandora typically cache a great deal of module information, and failing to clear caches may cause temporary, but unexpected results after a new module is installed.

That's it.  Your new module is installed and ready to be configured and used.  You can visit 'https://isle.localdomain/admin/modules' in your browser to manage the module's configuration, permissions, and more.

Other modules which use _Git_ and _Drush_ for installation can be obtained in a similar manner.

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

_Devel_ and _Islandora_Oral_History_Solution_Pack_, like most modules, build all of the module's components and dependencies in subdirectories below `/var/www/html`, and the effect is that these modules will persist in ISLE as long as the host's `apache-data-ld` folder is maintained.
