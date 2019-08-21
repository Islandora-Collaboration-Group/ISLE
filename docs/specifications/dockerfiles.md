<!--- PAGE_TITLE --->

The ISLE Images are created by the use of Dockerfiles, text documents that contain all the commands an ISLE contributor could call on the command line to assemble an image. Using docker build users e.g. ISLE Contributors or Developers can create an automated build that executes several command-line instructions in succession.

## Assumptions / Prerequisites

* ISLE Contributors, Developers have pre-existing knowledge or familiarity with Dockerfile

* [Dockerfiles](https://docs.docker.com/engine/reference/builder/), their editing process and compositions are familiar or known concepts.


## Dockerfile Locations

These files can be found within the cloned ISLE project directory.

Each subdirectory is grouped by the image / container software type and each contains a respective Dockerfile.

* apache
* fedora
* mysql
* proxy
* solr


```
├── README.md
├── ansible
├── apache
│   ├── Dockerfile
|   └── ...
├── config
│   └── isle-sample
├── docker-compose-build.yml
├── docker-compose.yml
├── fedora
│   ├── Dockerfile
|   └── ...
├── mysql
│   ├── Dockerfile
|   └── ...
├── proxy
│   ├── Dockerfile
|   └── ...
├── solr
│   ├── Dockerfile
|   └── ...
└── vagrant
```

## Dockerfile Conventions

* Making changes within these files will alter the contents and functionality of the ISLE images.

* Ensure that each Dockerfile does not have a file suffix or extension.

* Each command needs to be terminated with a `\` unless it is the final command.

* Each new command or action added will increase the size of the image once built. Attempt to balance size with functionality.

* All ISLE images use official Docker build images. Review the [Docker Containers & Images](../specifications/docker-containers-images.md) Specifications Page for a listing of those builds.

## Dockerfile Composition

Following [standard Docker convention](https://docs.docker.com/engine/reference/builder/), all ISLE Dockerfiles have similar structures. Examples displayed are from the [Apache Dockerfile](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/apache/Dockerfile)

* FROM command indicating which official Docker image is used as the base build.
    * The `Apache` image uses two base images to first compile `ffmpeg and ghostscript` and then copying the resulting binaries and libraries to a new clean base image so that the development tools are not installed on the final image. Reduces size.
    * `FROM ubuntu:14.04 AS ffmpeg_base` is the first base image used.
    * `FROM ubuntu:14.04` is the second base image used.

* LABEL - For ISLE Committers to version control image descriptions, functionality and additional meta-data. (_optional_)

* RUN - Execute commands e.g. `RUN apt-get update`, etc.

* COPY - Within each ISLE image directory there are additional files or subdirectories are copied from the personal computer to the image itself.

* ENV - sets any environmental variables necessary for software to run or be configured properly.

* VOLUME - Exposes a volume for data necessary for the build to continue. e.g. `VOLUME /var/www/html`

* EXPOSE - Opens up ports (80,443) on the temporary build containers created and to persist in further usage. Can be overridden by any `docker-compose.yml` file

* ENTRYPOINT - Allows you to configure a container that will run as an executable.

* Within the [apache Dockerfile](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/apache/Dockerfile), the server is told to "run" using the `apache2ctl` utility by this last line: `ENTRYPOINT ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]`

## Dockerfile Commenting

For ISLE Contributors when possible it is highly recommended that all sections be commented appropriately to explain functionality or actions to all Developers looking to customize or curious end users.

Example:

```
###
# Update apt
RUN apt-get update

###
# Add software to provide add-apt-repository
RUN apt-get install -y software-properties-common \
    python-software-properties
```
