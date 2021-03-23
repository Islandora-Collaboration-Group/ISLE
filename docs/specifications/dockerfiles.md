# Dockerfiles

The ISLE Images are created by the use of Dockerfiles, text documents that contain all the commands an ISLE contributor could call on the command line to assemble an image. Using docker build users e.g. ISLE Contributors or Developers can create an automated build that executes several command-line instructions in succession.

## Assumptions / Prerequisites

* ISLE Contributors, Developers have pre-existing knowledge or familiarity with Dockerfile

* [Dockerfiles](https://docs.docker.com/engine/reference/builder/), their editing process and compositions are familiar or known concepts.


## Dockerfile Locations

These files can be found within their respective Github repository.

Each directory is seperated by the image / container software type and each contains a respective Dockerfile.

* [apache](https://github.com/Islandora-Collaboration-Group/isle-apache)
* [fedora](https://github.com/Islandora-Collaboration-Group/isle-fedora)
* [image-services](https://github.com/Islandora-Collaboration-Group/isle-imageservices)
* [mysql](https://github.com/Islandora-Collaboration-Group/isle-mysql)
* [solr](https://github.com/Islandora-Collaboration-Group/isle-solr)
* [tomcat](https://github.com/Islandora-Collaboration-Group/isle-tomcat)

Please note that `isle-proxy` uses an official [Traefik](https://github.com/traefik/traefik-library-image/blob/304f7cf2cf36f59e0bc93597579c61837ce2ea6f/scratch/Dockerfile) image.

### Optional Dockerfiles

* [blazegraph](https://github.com/Islandora-Collaboration-Group/isle-blazegraph)
* [varnish](https://github.com/Islandora-Collaboration-Group/isle-varnish)

## Dockerfile Conventions

* Making changes within these files will alter the contents and functionality of the ISLE images.

* Ensure that each Dockerfile does not have a file suffix or extension.

* Each command needs to be terminated with a `\` unless it is the final command.

* Each new command or action added will increase the size of the image once built. Attempt to balance size with functionality.

* All ISLE images use official Docker build images. Review the [Docker Containers & Images](../specifications/docker-containers-images.md) Specifications page for a listing of those builds.

### Directory Structure

Example is from the Apache image directory

```bash
├── Dockerfile
├── hooks
│   └── build
├── LICENSE
├── Makefile
├── README.md
└── rootfs
    ├── etc
    │   ├── apache2
    │   │   └── conf-available
    │   │       ├── other-vhosts-access-log.conf
    │   │       ├── remoteip.conf
    │   │       └── servername.conf
    │   ├── confd
    │   │   ├── conf.d
    │   │   │   ├── apache-apache2-conf.toml
    │   │   │   ├── apache-isle-installer.toml
    │   │   │   ├── apache-settings.php.toml
    │   │   │   ├── apache-site-conf.toml
    │   │   │   ├── apache-sql.toml
    │   │   │   ├── fits-log4j.properties.toml
    │   │   │   ├── php-cli-ini.toml
    │   │   │   └── php-ini.toml
    │   │   └── templates
    │   │       ├── apache
    │   │       │   ├── apache2.conf.tpl
    │   │       │   └── site_template.conf.tpl
    │   │       ├── fits
    │   │       │   └── log4j.properties.tpl
    │   │       ├── php
    │   │       │   ├── cli.php.ini.tpl
    │   │       │   └── php.ini.tpl
    │   │       └── sql
    │   │           └── apache.sql.tpl
    │   ├── cont-init.d
    │   │   ├── 00-islandora-uid
    │   │   └── 01-confd-site-enable
    │   ├── php
    │   │   ├── 7.0
    │   │   │   └── mods-available
    │   │   │       └── imagick.ini
    │   │   ├── 7.1
    │   │   │   └── mods-available
    │   │   │       └── imagick.ini
    │   │   ├── 7.2
    │   │   │   └── mods-available
    │   │   │       └── imagick.ini
    │   │   └── 7.3
    │   │       └── mods-available
    │   │           └── imagick.ini
    │   └── services.d
    │       ├── apache
    │       │   └── run
    │       └── cron
    │           └── run
    └── usr
        └── local
            ├── adore-djatoka-1.1
            │   └── bin
            │       ├── env.sh
            │       └── log4j.properties
            └── etc
                └── ImageMagick-7
                    ├── log.xml
                    └── policy.xml

31 directories, 34 files
```

## Dockerfile Composition

Following [standard Docker convention](https://docs.docker.com/engine/reference/builder/), all ISLE Dockerfiles have similar structures. Examples displayed are from the [Apache Dockerfile](https://github.com/Islandora-Collaboration-Group/isle-apache/blob/main/Dockerfile).

* FROM command indicating which official Docker image is used as the base build.
    * The `Apache` image uses one base image to build from as it both contains an `Ubuntu 20.04 LTS` OS and `Adopt-JDK 8.x` for Java
    * `FROM adoptopenjdk:8-jdk-hotspot` is the first base image used.

* LABEL - For ISLE Committers to version control image descriptions, functionality and additional meta-data. (_optional_)

* RUN - Execute commands e.g. `RUN apt-get update`, etc.

* COPY - Within each ISLE image directory there are additional files or subdirectories are copied from the personal computer to the image itself.

* ENV - sets any environmental variables necessary for software to run or be configured properly.

* VOLUME - Exposes a volume for data necessary for the build to continue. e.g. `VOLUME /var/www/html`

* EXPOSE - Opens up ports (80,443) on the temporary build containers created and to persist in further usage. Can be overridden by any `docker-compose.yml` file

* ENTRYPOINT - Allows you to configure a container that will run as an executable.

* Within the [apache Dockerfile](https://github.com/Islandora-Collaboration-Group/ISLE/blob/main/apache/Dockerfile), the server is told to "run" using the `apache2ctl` utility by this last line: `ENTRYPOINT ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]`

## Dockerfile Commenting

For ISLE Contributors when possible it is highly recommended that all sections be commented appropriately to explain functionality or actions to all Developers looking to customize or curious end users.

Example:

```
## Apache, PHP, FFMPEG, and other Islandora Depends.
## Apache && PHP 7.1 from ondrej PPA
## Per @g7morris, ghostscript from repo is OK.
RUN add-apt-repository -y ppa:ondrej/apache2 && \
    add-apt-repository -y ppa:ondrej/php && \
    FFMPEG_PACKS="ffmpeg \
    ffmpeg2theora \
    libavcodec-extra \
    lame \
```
