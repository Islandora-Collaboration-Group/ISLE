# Data Persistence in Docker

**Please note this is a simplified explanation of how Docker stores data in relation to a typical ISLE installation.** This document assumes the use of the docker-compose command and docker-compose.yml, though there are other ways to specify how docker uses volumes and bind mounts.  For more complete descriptions of bind mounts and volumes, and their management, please see the official docker documentation.

**Data**, in this context, can include digital objects, files, logs, code, or information stored in mysql, solr, or fedora.  Essentially, anything typically written to or read from disk in some format, is data. 

Each time a docker container is brought up using a command like `docker-compose up -d` the container is recreated using the base container image. When a process in the container creates or changes a file (for example, an apache log file) those changes only exist while the container is running.  If the container is brought down and back up again, it is recreated using the base image, which will not include the apache log. In order to preserve certain data--for example, the Fedora datastore, Drupal’s database and files, etc, ISLE uses volumes and/or bind mounts defined in 'docker-compose.yml'.  

Data written to volumes and bind mounts are different to other data in Docker in two important ways: 

- It is persistent, so it will survive when there is an update to the base ISLE docker images
- It can be read and written from the host easily and safely

## Bind mounts vs volumes:

Docker provides two different ways to persist data: **Volumes** and **bind mounts**, described below.  For simplicity, in this document, **persisted** refers to any data or directory stored in either a bind mount or volume.

### Volumes

Volumes can be named in the docker-compose.yml file and are managed by docker.  In the host system, the directories will be created in the Docker store (on Linux, usually /var/lib/docker).  If the volume does not already exist, it will be created on docker-compose up.

Below is an example docker-compose.yml directive for an apache container using a volume:

    volumes:
    apache-data:/var/www/html


  Explanation: If not already created, docker will create a directory on the host at '/var/lib/docker' and use that for the apache container’s '/var/www/html' directory.

### Bind Mounts

Bind mounts map a directory in the container to a directory anywhere on the host and are not managed directly by docker.  

Below is an example 'docker-compose.yml' directive for an apache container using a bind mount:

    volumes:
    /opt/ISLE/apache/html:/var/www/html

  Explanation:  Data in the host’s /opt/ISLE/apache/html directory will be in the apache container’s /var/www/html.  If the directory does not exist on the host, it will NOT be created on `docker-compose up -d`, so be sure to create any necessary directories on the host manually.  

  If the container’s '/var/www/html/' directory has content, it will be masked by the contents of the host’s '/opt/ISLE/apache/html/' directory.

## What should be persisted this way?

Data and directories that store data unique to your Islandora, such as the Fedora datastore, Solr index, and the full Drupal directory, should be persisted, as should the mysql data directories and some configuration files (those not generated via the .env files).  This will ensure that your customizations and content remain even after ISLE containers are updated.

It may be useful to write logs to a persisted directory to make accessing and managing the logs easier, and to provide sufficient capacity for log storage.  

If your workflows involve moving digital objects onto the Islandora server for local ingest, it may be useful to have a persisted directory designated for those objects.  The alternative is to put them on the host server, then use `scp` or `docker cp` to copy them into the container, which adds steps to the process.

## What should NOT be persisted?

Application data that is not meant to be customized or unique to your institution should not be persisted, but instead managed within the docker container (as it is by default).   Example:  The ISLE team updates the apache container to apache2’s latest security release.  If the directory containing apache’s binary is in a persisted directory, then updating ISLE will NOT automatically provide the latest apache, and you will continue to run the older, insecure apache until you manually update it inside the container.  This negates many of the benefits of using ISLE.  

## Which should I use, bind mounts or volumes?

The answer for this is “it depends”.   Bind mounts may be easier to get to on a host machine as you have more control over where they are.  If you are migrating an existing installation, you will almost certainly want to copy over things like your /var/www/html directory and Fedora store and use a bind mount to point to them.  The solr index, however, should not be migrated over but rather rebuilt.  It doesn’t need to exist before the containers do. It is also unlikely that a user would want to navigate to the solr directory via command line on the host and edit files directly.  A named volume would probably work fine in this case, and be more easily managed by docker. 
