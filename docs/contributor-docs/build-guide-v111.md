# ISLE Build Process

## Build Setup

* Clear / prune old docker images from local
  * `docker image ls`
  * `docker image rm -f <Docker IMAGE ID>`

* Fork ISLE to personal / company github.com repository 
  * https://github.com/Islandora-Collaboration-Group/ISLE

* Fork all ISLE images to personal / company github.com repositories
  * isle-ubuntu-basebox https://github.com/Islandora-Collaboration-Group/isle-ubuntu-basebox
  * isle-tomcat https://github.com/Islandora-Collaboration-Group/isle-tomcat
  * isle-fedora https://github.com/Islandora-Collaboration-Group/isle-fedora
  * isle-imageservices https://github.com/Islandora-Collaboration-Group/isle-imageservices
  * isle-solr https://github.com/Islandora-Collaboration-Group/isle-solr
  * isle-apache https://github.com/Islandora-Collaboration-Group/isle-apache
  * isle-mysql https://github.com/Islandora-Collaboration-Group/isle-mysql

* Git clone the forked ISLE project git repository to your local development personal computer.

* Git clone the forked ISLE images git repositories into a sibling "images" directory

* Git checkout (make new) release branch in each repository for each image starting with and descending in this order:
  * isle-ubuntu-basebox
  * isle-tomcat
  * isle-fedora
  * isle-imageservices
  * isle-solr
  * isle-apache
  * isle-mysql
  * ISLE (main project)

## Build Steps

### ISLE

* **ISLE** - uncomment all `build / context blocks` in the main `docker-compose.yml` file

* **ISLE** - Edit the `traefik` magic number (Line 133) within the `docker-compose.yml` file

### All Images

* **Images** - update "magic" numbers within each Dockerfile.

* **Images** - Within a terminal, cd to each cloned forked image directory. 
  * Run build command per image one at a time: `docker build - islandoracollabgroup/isle-image:1.1.1`
  * If the build works without error, then the image is tagged, can be tested locally and move onto the next image in order.
  * Repeat this process and command for each image (7 times)

### isle-ubuntu-basebox

Start with this image first before all others.

* Image name: **islandoracollabgroup / isle-ubuntu-basebox**
  * Last build: `1.1 / latest` (Last built 4 months ago)
  * Build date: Sept 27, 2018

* This Dockerfile has the `FROM: ubuntu:bionic` which means it pulls from here:
  * https://hub.docker.com/_/ubuntu
  * https://github.com/tianon/docker-brew-ubuntu-core/blob/bfd753a747344ff1c6838a2c91ff0606e936f0d0/bionic/Dockerfile
  * https://hub.docker.com/_/scratch

#### isle-ubuntu-basebox - Software Dependencies

The following software packages will be handled and updated by `apt-get`. Please note there may be further dependencies and libraries updated.

* `GEN_DEP_PACKS`
  * https://packages.ubuntu.com/bionic/ca-certificates
  * https://packages.ubuntu.com/bionic/dnsutils
  * https://packages.ubuntu.com/bionic/curl
  * https://packages.ubuntu.com/bionic/wget 
  * https://packages.ubuntu.com/bionic/rsync
  * https://packages.ubuntu.com/bionic/git
  * https://packages.ubuntu.com/bionic/unzip

#### isle-ubuntu-basebox - Software updates requiring manual changes and/or lookups for recent versions.

* **S6 Overlay**
  * https://github.com/just-containers/s6-overlay
  * `S6_OVERLAY_VERSION:-1.21.4.0`
  * https://github.com/just-containers/s6-overlay/releases/download/v$S6_OVERLAY_VERSION/s6-overlay-amd64.tar.gz
  * https://github.com/just-containers/s6-overlay/releases

* **Java**
  * Version Lookup URLs:
    * https://www.java.com/en/download/faq/release_dates.xml
    * https://www.oracle.com/technetwork/java/javase/downloads/server-jre8-downloads-2133154.html

  * To determine the newest Java version and values to flow in:
    * Visit https://www.oracle.com/technetwork/java/javase/downloads/server-jre8-downloads-2133154.html
    * Accept the License Agreement
    * On the newest number e.g the bottom section i.e. `Server JRE (Java SE Runtime Environment) 8u202`
      * Right-click on the first Download link for Linux x64 e.g. `server-jre-8u202-linux-x64.tar.gz` and copy the link address.
      * It should look like this: `https://download.oracle.com/otn-pub/java/jdk/8u202-b08/1961070e4c9b4e26a04e7f5a083f551e/server-jre-8u202-linux-x64.tar.gz`
      * `ENV JAVA_VERSION=${JAVA_VERSION:-xxxxx}` should be set to `8u202`
      * `ENV JAVA_BUILD=${JAVA_BUILD:-xxx}` should be set to `b08`
      * `ENV JAVA_SECURITY_BUILD=${JAVA_SECURITY_BUILD:-x.x.x_xxx}` should be set to `1.8.0_202`
        * This isn't particularly obvious from the URL per se. One can simply leave the `1.8.0_` alone and simply add the `202` as needed.
      * `ENV JAVA_SEC_HASH=${JAVA_SEC_HASH:-}` - change or overwrite the value (random numbers) found between `8u202-b08/` & `/server-jre-8u202-linux-x64.tar.gz` in the written down URL from the look up process e.g. `1961070e4c9b4e26a04e7f5a083f551e` after the `-`

#### isle-ubuntu-basebox - Build Actions / Steps

* Start the update process with the `isle-ubuntu-basebox` image
  * Clone down the **FORKED** `isle-ubuntu-basebox` to your local development personal computer.
  * Create a new branch e.g. `ISLE-v.1.1.1`

* Edit the following files with the version changes:
  * `Dockerfile`
    * Line 39: `ENV S6_OVERLAY_VERSION=${S6_OVERLAY_VERSION:-x.xx.x.x}` - change or overwrite with a value like `1.21.7.0` after the `-`
    * Line 45: `ENV JAVA_VERSION=${JAVA_VERSION:-xxxxx}` - change or overwrite with a value like `8u202` after the `-`
    * Line 46: `ENV JAVA_BUILD=${JAVA_BUILD:-xxx}` - change or overwrite with a value like `b08` after the `-`
    * Line 47: `ENV JAVA_SECURITY_BUILD=${JAVA_SECURITY_BUILD:-x.x.x_xxx}` - change or overwrite with a value like `1.8.0_202` after the `-`
      * This isn't particularly obvious from the resource URL per se. One can simply leave the `1.8.0_` alone and simply add the `202` as needed.
    * Line 48: `ENV JAVA_SEC_HASH=${JAVA_SEC_HASH:-}` - change or overwrite with a value like `1961070e4c9b4e26a04e7f5a083f551e` after the `-`
      * This is the random sequence of numbers from the URL e.g. https://download.oracle.com/otn-pub/java/jdk/8u202-b08/1961070e4c9b4e26a04e7f5a083f551e/server-jre-8u202-linux-x64.tar.gz
  
  * `Makefile`
    * `S6_OVERLAY_VERSION=x.xx.x.x ` - change or overwrite with a value like `1.21.7.0` after the `=`
    * `JAVA_VERSION=xxxxx` - change or overwrite with a value like `8u202` after the `=`
    * `JAVA_BUILD=xxx` - change or overwrite with a value like `b08` after the `=`
    * `JAVA_SECURITY_BUILD=x.x.x_xxx` - change or overwrite with a value like `1.8.0_202` after the `=`
    * `JAVA_SEC_HASH=` - change or overwrite with a value like `1961070e4c9b4e26a04e7f5a083f551e` after the `-`
      * This is the random sequence of numbers from the URL e.g. https://download.oracle.com/otn-pub/java/jdk/8u202-b08/1961070e4c9b4e26a04e7f5a083f551e/server-jre-8u202-linux-x64.tar.gz

* Commit the changes to git

* Build the image
  * open a terminal
  * `cd ~/pathto/isle-ubuntu-basebox`
  * `docker build -t islandoracollabgroup/isle-ubuntu-basebox:1.1.1 .`
  * Then move onto building the next image in the sequence prior to testing

### (Optional) Manual method of reviewing image
* `docker run -it --rm islandoracollabgroup/isle-ubuntu-basebox bash`

### isle-ubuntu-basebox - Post QC - Release build image
  * open a terminal
  * `cd ~/pathto/isle-ubuntu-basebox`

#### EXAMPLE / REFERENCE COMMAND

* Review this syntax first to understand how to construct the build command that follows.

```
docker build \
	--build-arg BUILD_DATE=`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	--build-arg VCS_REF=$SOURCE_COMMIT \
	--build-arg VERSION=RC-`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	-t $IMAGE_NAME .
```

#### EXAMPLE / ACTUAL COMMAND W/ FORKED COMMIT HASH

* This will be the command to use and construct to build an image to push to Dockerhub. Please note that `VCS_REF` uses the last git commit hash from the forked branch on the image prior to merge in ISLE `master`.

```
docker build \
    --build-arg BUILD_DATE=`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
    --build-arg VCS_REF=a6f60de7619724485c287f19e7bd59d74190fd60 \
    --build-arg VERSION=RC-`date -u +"%Y-%m-%dT%H:%M:%SZ"` \ 
    -t islandoracollabgroup/isle-ubuntu-basebox:1.1.1 .
```

* Push built images to Dockerhub with new tag
`docker push islandoracollabgroup/isle-ubuntu-basebox:1.1.1`

* Test with new images

### isle-ubuntu-basebox - Tagging outcome

* `1.1` (last image build from 9/27/2018) leave alone
* `1.1.1` (new image build with sec updates 3/22/2019)
* `latest ` (new image build with sec updates 3/22/2019)

* deprecate for future removal `serverjr8` ( last image build from 9/27/2018)

---

### isle-tomcat

* Image name: **islandoracollabgroup / isle-tomcat**
  * Last build: `1.1 / latest` (Last built 4 months ago)
  * Build date: Sept 27, 2018

#### isle-tomcat - Software Dependencies

The following software packages will be handled and updated by `apt-get`. Please note there may be further dependencies and libraries updated.

* `GEN_DEP_PACKS`
  * https://packages.ubuntu.com/bionic/cron
  * https://packages.ubuntu.com/bionic/tmpreaper
  * https://packages.ubuntu.com/bionic/libapr1-dev
  * https://packages.ubuntu.com/bionic/libssl-dev

#### Notes
* `gcc` - is installed for building tomcat from source and then removed from the final image
* `make` - is installed for building tomcat from source and then removed from the final image
* `gcc-7-base` - is installed for building tomcat from source and then removed from the final image

#### isle-tomcat - Software updates requiring manual changes and/or lookups for recent versions.

* **confd**
  * https://github.com/kelseyhightower/confd/releases/download/v0.16.0
  * No upgrade available at this time.

* **tomcat**
  * http://tomcat.apache.org/
  * https://tomcat.apache.org/download-80.cgi
  * `TOMCAT_MAJOR=${TOMCAT_MAJOR:-8}`
  * `TOMCAT_VERSION=${TOMCAT_VERSION:-8.5.34}`
  * `curl -O -L "https://www.apache.org/dyn/closer.cgi?action=download&filename=tomcat/tomcat-$TOMCAT_MAJOR/v$TOMCAT_VERSION/bin/apache-tomcat-$TOMCAT_VERSION.tar.gz" && \`

### isle-tomcat - Build Actions / Steps

Assumptions: The `isle-ubuntu-basebox` has been already updated, built and pushed to Dockerhub. This image will work (build) from that foundational image.

* Start the update process with the `isle-tomcat` image
  * Clone down the **FORKED** `isle-tomcat` to your local development personal computer.
  * Create a new branch e.g. `ISLE-v.1.1.1`

* Edit the following files with the version changes:
  * `Dockerfile`
    * Line 1: `FROM islandoracollabgroup/isle-ubuntu-basebox:serverjre8` change the tag to `1.1.1` e.g.
    `islandoracollabgroup/isle-ubuntu-basebox:1.1.1`
    * Line 44: `TOMCAT_VERSION=${TOMCAT_VERSION:-x.x.xx}` - change or overwrite with a value like `8.5.39` after the `-`

* Commit the changes to git

* Build the image
  * open a terminal
  * `cd ~/pathto/isle-tomcat`
  * `docker build -t islandoracollabgroup/isle-tomcat:1.1.1 .`
  * Then move onto building the next image in the sequence prior to testing

### (Optional) Manual method of reviewing image
* `docker run -p 8080:8080 -it --rm islandoracollabgroup/isle-tomcat:1.1.1 bash`

### isle-tomcat - Post QC - Release Build image

* open a terminal
* `cd ~/pathto/isle-tomcat`

#### EXAMPLE / REFERENCE COMMAND

* Review this syntax first to understand how to construct the build command that follows.

```
docker build \
	--build-arg BUILD_DATE=`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	--build-arg VCS_REF=$SOURCE_COMMIT \
	--build-arg VERSION=RC-`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	-t $IMAGE_NAME .
```

#### EXAMPLE / ACTUAL COMMAND W/ FORKED COMMIT HASH

* This will be the command to use and construct to build an image to push to Dockerhub. Please note that `VCS_REF` uses the last git commit hash from the forked branch on the image prior to merge in ISLE `master`.

```bash

docker build --build-arg BUILD_DATE=`date -u +"%Y-%m-%dT%H:%M:%SZ"` --build-arg VCS_REF=6fb610e9ab1eab68f819fa9ded0bc678c4db385c --build-arg VERSION=RC-`date -u +"%Y-%m-%dT%H:%M:%SZ"` -t islandoracollabgroup/isle-tomcat:1.1.1 .
```

* Push built images to Dockerhub with new tag
`docker push islandoracollabgroup/isle-tomcat:1.1.1`

### isle-tomcat - Tagging outcome

Suggested outcome:
* `1.1` (last image build from 9/27/2018)
* `1.1.1` (new image build with sec updates 3/20/2019)
* `latest ` (new image build with sec updates 3/20/2019)

* deprecate `serverjr8` (last image build from 9/27/2018)

---

### isle-fedora

* Image name: **islandoracollabgroup / isle-fedora**
  * Last build: `1.1` (Last built 4 months ago)
  * Build date: Oct 10, 2018

#### isle-fedora - Software Dependencies

The following software packages will be handled and updated by `apt-get`. Please note there may be further dependencies and libraries updated.

* `GEN_DEP_PACKS`
  * https://packages.ubuntu.com/bionic/mysql-client
  * https://packages.ubuntu.com/bionic/python-mysqldb
  * https://packages.ubuntu.com/bionic/default-libmysqlclient-dev
  * https://packages.ubuntu.com/bionic/openssl
  * https://packages.ubuntu.com/bionic/libxml2-dev

#### isle-fedora - Software updates requiring manual changes and/or lookups for recent versions

* **Fedora**
  * https://github.com/fcrepo3/fcrepo
  * https://github.com/fcrepo3/fcrepo/releases/download/v3.8.1/fcrepo-installer-3.8.1.jar
  * version `3.8.1`
  * **Not eligible for upgrade**

* **Gsearch**
  * `v2.9.0`
  * `git clone https://github.com/discoverygarden/gsearch.git`
  * `Latest commit 701400e  on Dec 11, 2017`
  * **Not eligible for upgrade**

* **DGI Gsearch Extensions**
  * `https://github.com/discoverygarden/dgi_gsearch_extensions.git
  * `v.0.1.3`
  * `/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/lib/gsearch_extensions-0.1.3-jar-with-dependencies.jar`
  * Last commit was June 16, 2017 https://github.com/discoverygarden/dgi_gsearch_extensions/commit/35f37a486cc5ba748fcee9a4f95b98187b1c6731
  * **Not eligible for upgrade**

* **Islandora XACML policies**
  * https://github.com/Islandora/islandora-xacml-policies
  * Previous build (Oct 10, 2018) uses this commit https://github.com/Islandora/islandora-xacml-policies/commit/40663fbb0f8de7e45d14fc1ebc6b7e8dc04cffb9
  * Latest is new commit Jan 2019 https://github.com/Islandora/islandora-xacml-policies/commit/73deba6b90f6de40d72c890233ba6ff0159031de

* **Islandora Drupal Filter**
  * Built Oct 10, 2018
  * https://github.com/Islandora/islandora_drupal_filter
  * Build uses this release * `v7.1.9` https://github.com/Islandora/islandora_drupal_filter/releases/download/v7.1.9/fcrepo-drupalauthfilter-3.8.1.jar  
  * Latest is: `7.1.12` https://github.com/Islandora/islandora_drupal_filter/releases/tag/7.1.12

* **Apache Maven**
  * https://maven.apache.org/index.html
  * version used `3.5.4` on Oct 10, 2018
  * `curl -O -L "https://www.apache.org/dyn/closer.cgi?action=download&filename=maven/maven-$MAVEN_MAJOR/$MAVEN_VERSION/binaries/apache-maven-$MAVEN_VERSION-bin.tar.gz"`

* **Apache Ant**
  * https://ant.apache.org/
  * version used `1.10.5` on Oct 10, 2018
  * `curl -O -L "https://www.apache.org/dyn/closer.cgi?action=download&filename=ant/binaries/apache-ant-$ANT_VERSION-bin.tar.gz"`

* **DGI Basic-Solr Config**
  * The git branch to clone from is: `-b 4.10.x`
  * https://github.com/discoverygarden/basic-solr-config.git
  * Oct 10 was build date so the last commit used (version) was on Sept 21, 2018
    * https://github.com/discoverygarden/basic-solr-config/commit/0282f703a486ab89c7bbded6af846540be234833
  
  * "Magic number" comparison process for release notes:
    * `cd /tmp`
    * `git clone --recursive -b 4.10.x https://github.com/discoverygarden/basic-solr-config.git old`
    * `git clone --recursive -b 4.10.x https://github.com/discoverygarden/basic-solr-config.git new`
    * Check build date on last Dockerhub push e.g. Oct 10, 2018
    * Check commits page https://github.com/discoverygarden/basic-solr-config/commits/4.10.x
      * Determine which commit came on or before Oct 10, 2018 e.g. `Sept 21, 2018 0282f703a486ab89c7bbded6af846540be234833` (_you can click the clipboard to copy this value_)
    * `cd /tmp/old`
    * `git checkout last used build commit` i.e. `git checkout 0282f703a486ab89c7bbded6af846540be234833`
    * Compare both directories using a visual diff tool like `Kaleidoscope` (MAC), `diff` or `KDiff3` (Linux)
    * Note the differences in files and the line numbers for inclusion in release notes.
      * `foxmlToSolr.xslt` - Lines 47 & 48 - New lines added
      * `README.md` - Lines 31 & 32 explanations for the line additions in `foxmlToSolr.xslt`

* **DGI Islandora Transforms**
  * git submodule of **DGI Basic-Solr Config**
  * https://github.com/discoverygarden/islandora_transforms
  * Oct 10 was build date so the last commit used (version) was on Sept 21, 2018
    * https://github.com/discoverygarden/islandora_transforms/commit/ee7e9d32a71381abfa5c3ead13255c0c5b66ed45
  * Newest commit: March 19, 2019 https://github.com/discoverygarden/islandora_transforms/commit/cdfef63b7e2f6740c06e56bfa32d496c09837ae8

  * "Magic number" comparison process for release notes:
    * `cd /tmp`
    * `git clone https://github.com/discoverygarden/islandora_transforms old`
    * `git clone https://github.com/discoverygarden/islandora_transforms new`
    * Check build date on last Dockerhub push e.g. Oct 10, 2018
    * Check commits page https://github.com/discoverygarden/islandora_transforms/commits/master
      * Determine which commit came on or before Oct 10, 2018 e.g. `Sept 21, 2018 ee7e9d32a71381abfa5c3ead13255c0c5b66ed45` (_you can click the clipboard to copy this value_)
    * `cd /tmp/old`
    * `git checkout last used build commit` i.e. `git checkout ee7e9d32a71381abfa5c3ead13255c0c5b66ed45`
    * Compare both directories using a visual diff tool like `Kaleidoscope` (MAC), `diff` or `KDiff3` (Linux)
    * Note the differences in files and the line numbers for inclusion in release notes.
      * `datastream_info_to_solr.xslt` - Lines 35-49 - new lines & content added
      * `RELS-EXT_to_solr.xslt`
        * Line 7 - modified
        * Line 8 - new line & content added
        * Line 42 - new line & content added
        * Line 49 - new line & content added
      * `slurp_all_MODS_to_solr.xslt` - Lines 289-300 new lines & content added

#### isle-fedora - Build Actions / Steps

Assumptions: The `isle-ubuntu-basebox` & `isle-tomcat` images have been already updated, built and pushed to Dockerhub. This image will work (build) from those foundational images.

* Start the update process with the `isle-fedora` image
  * Clone down the **FORKED** `isle-fedora` to your local development personal computer.
  * Create a new branch e.g. `ISLE-v.1.1.1`

* Edit the following files with the version changes:
  * `Dockerfile`
    * Line 1: change the tag from `FROM islandoracollabgroup/isle-tomcat:x` to `isle-tomcat:1.1.1`
    * Line 53: change `vx.x.x` in the curl url to `v.7.1.12`
    * Line 62: change `MAVEN_VERSION:-x.x.x` to `MAVEN_VERSION:-3.6.0`
    * Line 63: change `ANT_VERSION:-x.xx.x` to `ANT_VERSION:-1.9.14`

* Commit the changes to git

* Build the image
  * open a terminal
  * `cd ~/pathto/isle-fedora`
  * `docker build -t islandoracollabgroup/isle-fedora:1.1.1 .`
  * Then move onto building the next image in the sequence prior to testing

### isle-fedora - Post QC - Release Build Image

#### EXAMPLE / REFERENCE COMMAND

* Review this syntax first to understand how to construct the build command that follows.

```
docker build \
	--build-arg BUILD_DATE=`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	--build-arg VCS_REF=$SOURCE_COMMIT \
	--build-arg VERSION=RC-`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	-t $IMAGE_NAME .
```

#### EXAMPLE / ACTUAL COMMAND W/ FORKED COMMIT HASH

* This will be the command to use and construct to build an image to push to Dockerhub. Please note that `VCS_REF` uses the last git commit hash from the forked branch on the image prior to merge in ISLE `master`.

```
docker build --build-arg BUILD_DATE=`date -u +"%Y-%m-%dT%H:%M:%SZ"` --build-arg VCS_REF=180e2382626a6972b0251d18769c45e4a1e2d7cb --build-arg VERSION=RC-`date -u +"%Y-%m-%dT%H:%M:%SZ"` -t islandoracollabgroup/isle-fedora:1.1.1 .
```

### (Optional) Manual method of reviewing image
* `docker run -p 8080:8080 -it --rm islandoracollabgroup/isle-fedora:1.1 bash`

* Push built images to Dockerhub with new tag
`docker push islandoracollabgroup/isle-fedora:1.1.1`

* Test with new images

### isle-fedora -  Tagging Outcome

Suggested outcome:
* `1.0` (9/7/2018)
* `1.1` (last image build from 10/10/2018)
* `1.1.1` (new image build with sec updates 3/20/2019)
* `latest ` (new image build with sec updates 3/20/2019)

* leave `blazegraph` alone
* deprecate `1.dev` _what is this? `Nov5th, 2018`_
* deprecate `3.8.1`

---

### isle-imageservices

* Image name: **islandoracollabgroup / isle-imageservices** 
  * Last Build: `1.1 / latest` (Last built 5 months ago)
  * `1.11` is Sept 15, 2018
  * `latest` is Sept 7, 2018
  ^ Needs fix

#### isle-imageservices - Software Dependencies

The following software packages will be handled and updated by `apt-get`. Please note there may be further dependencies and libraries updated.

* `GEN_DEP_PACKS`
  * https://packages.ubuntu.com/bionic/ffmpeg
  * https://packages.ubuntu.com/bionic/ffmpeg2theora
  * https://packages.ubuntu.com/bionic/libavcodec-extra
  * https://packages.ubuntu.com/bionic/ghostscript
  * https://packages.ubuntu.com/bionic/xpdf
  * https://packages.ubuntu.com/bionic/poppler-utils

* NOTE: `$BUILD_DEPS` means all dev tools are uninstalled after building `openjpeg` from source

#### isle-imageservices - Software updates requiring manual changes and/or lookups

* **openjpeg**
  * https://github.com/uclouvain/openjpeg
  * Last commit used: Sept 5, 2018 https://github.com/uclouvain/openjpeg/commit/31a03b390a77bfbe4b0f140121d1296acb611f76
  * Upgraded **openjpeg** to latest Dec 21,2018 commit https://github.com/uclouvain/openjpeg/commit/51f097e6d5754ddae93e716276fe8176b44ec548

* **Imagemagick**
  * https://www.imagemagick.org
  * `curl -O https://www.imagemagick.org/download/ImageMagick.tar.gz`
  * `/`usr/local/bin/convert -v`
  * `Version: ImageMagick 7.0.8-11 Q16 x86_64 2018-09-15 https://www.imagemagick.org`

* **Adore-Djatoka**
  * https://sourceforge.mirrorservice.org/d/dj/djatoka/djatoka/1.1/adore-djatoka-1.1.tar.gz
  * No upgrade possible

* **Cantaloupe**
  * https://github.com/medusa-project/cantaloupe
  * `v4.0.1`
  * Going to hold off on an update due to `delegates.rb` and `cantaloupe.properties` files not being fully tested in the Islandora community.
  * Don't attempt to upgrade Cantaloupe to `v4.0.3` DUE to potential key changes and confusion over version usage.

### isle-imageservices - Build Steps / Actions

Assumptions: The `isle-ubuntu-basebox` & `isle-tomcat` images have been already updated, built and pushed to Dockerhub. This image will work (build) from those foundational images.

* Start the update process with the `isle-image-services` image
  * Clone down the **FORKED** `isle-image-services` to your local development personal computer.
  * Create a new branch e.g. `ISLE-v.1.1.1`

* Edit the following files with the version changes:
  * `Dockerfile`
    * Line 1: change the tag from `FROM islandoracollabgroup/isle-tomcat:x` to `isle-tomcat:1.1.1`
    * Line 87: change `vx.x.x` in the curl url to `v4.0.1` (Not done on March 25)

* Commit the changes to git

* Build the image
  * open a terminal
  * `cd ~/pathto/isle-image-services`
  * `docker build -t islandoracollabgroup/isle-imageservices:1.1.1 .`
  * Then move onto building the next image in the sequence prior to testing

* Test with new images

### (Optional) Manual method of reviewing image
* `docker run -it -p "8080:8080" --rm islandoracollabgroup/isle-imageservices:1.1 bash`

### isle-imageservices - Post QC - Release Build Image

#### EXAMPLE / REFERENCE COMMAND

* Review this syntax first to understand how to construct the build command that follows.

```
docker build \
	--build-arg BUILD_DATE=`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	--build-arg VCS_REF=$SOURCE_COMMIT \
	--build-arg VERSION=RC-`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	-t $IMAGE_NAME .
```

#### EXAMPLE / ACTUAL COMMAND W/ FORKED COMMIT HASH

* This will be the command to use and construct to build an image to push to Dockerhub. Please note that `VCS_REF` uses the last git commit hash from the forked branch on the image prior to merge in ISLE `master`.

```bash

docker build --build-arg BUILD_DATE=`date -u +"%Y-%m-%dT%H:%M:%SZ"` --build-arg VCS_REF=6e7f82aa405848e05771ff6e926c6dc0f73fbb8f --build-arg VERSION=RC-`date -u +"%Y-%m-%dT%H:%M:%SZ"` -t islandoracollabgroup/isle-imageservices:1.1.1 .
```

* Push built images to Dockerhub with new tag
`docker push islandoracollabgroup/isle-imageservices:1.1.1`

### Tagging

Suggested outcome:
* `1.1` (last image build from 9/2018)
* `1.1.1` (new image build with sec updates 3/20/2019)
* `latest ` (new image build with sec updates 3/20/2019)

---

### isle-solr  

* Image name: **islandoracollabgroup / isle-solr**
  * Last Build: `1.1 / latest` (Last built 3 months ago)
  * November 5th, 2018

* Solr 
  * `v4.10.4`
  * `curl -O -L http://archive.apache.org/dist/lucene/solr/$SOLR_VERSION/solr-$SOLR_VERSION.tgz`
  * http://archive.apache.org/dist/lucene/solr/4.10.4/
  * No apparent updates since `2015-03-03`

* **DGI Basic-Solr Config**
  * The git branch to clone from is: `-b 4.10.x`
  * https://github.com/discoverygarden/basic-solr-config.git
  * But the area of this that is used is 
    * https://github.com/discoverygarden/basic-solr-config/tree/4.10.x/conf
    * this has not changed since Aug 2107 https://github.com/discoverygarden/basic-solr-config/commit/a3457c2ec6a44cb5c74937b5e45056ab5ae99916

### Actions

* Run package management updates but given this builds off the two previous images I'm not sure there are any changes per se.

### (Optional) Manual method of reviewing image
* `docker run -p 8080:8080 -it --rm islandoracollabgroup/isle-solr:1.1 bash`

### Review & Build Process

Assumptions: The `isle-ubuntu-basebox` & `isle-tomcat` images have been already updated, built and pushed to Dockerhub. This image will work (build) from those foundational images.

* Start the update process with the `isle-solr` image
  * Clone down the **FORKED** `isle-solr` to your local development personal computer.
  * Create a new branch e.g. `ISLE-v.1.1.1`

* Edit the following files with the version changes:
  * `Dockerfile`
    * Line 1: change the tag from `FROM islandoracollabgroup/isle-tomcat:x` to `isle-tomcat:1.1.1`

* Commit the changes to git

* Build the image
  * open a terminal
  * `cd ~/pathto/isle-solr`
  * `docker build -t islandoracollabgroup/isle-solr:1.1.1 .`

* Test with new images

#### EXAMPLE / REFERENCE COMMAND

* Review this syntax first to understand how to construct the build command that follows.

```
docker build \
	--build-arg BUILD_DATE=`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	--build-arg VCS_REF=$SOURCE_COMMIT \
	--build-arg VERSION=RC-`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	-t $IMAGE_NAME .
```

#### EXAMPLE / ACTUAL COMMAND W/ FORKED COMMIT HASH

* This will be the command to use and construct to build an image to push to Dockerhub. Please note that `VCS_REF` uses the last git commit hash from the forked branch on the image prior to merge in ISLE `master`.

```bash

docker build --build-arg BUILD_DATE=`date -u +"%Y-%m-%dT%H:%M:%SZ"` --build-arg VCS_REF=730d9ee566a55430d81d0691fb6b3370aa9148ed --build-arg VERSION=RC-`date -u +"%Y-%m-%dT%H:%M:%SZ"` -t islandoracollabgroup/isle-solr:1.1.1 .

```

* Push built images to Dockerhub with new tag
`docker push islandoracollabgroup/isle-solr:1.1.1`

### Tagging

Suggested outcome
* `1.1` (last image build from 9/2018)
* `1.1.1` (new image build with sec updates 3/20/2019)
* `latest ` (new image build with sec updates 3/20/2019)

* deprecate 4.10.4 tag

---

### isle-apache

* Image name: **islandoracollabgroup / isle-apache** 
  * Last Build: `1.1 / latest` (Last built 3 months ago)
  * Last build: November 5th, 2018

#### Software Dependencies

The following software packages will be handled and updated by `apt-get`. Please note there may be further dependencies and libraries updated.

* `GEN_DEP_PACKS`
  * https://packages.ubuntu.com/bionic/language-pack-en-base
  * https://packages.ubuntu.com/bionic/tmpreaper
  * https://packages.ubuntu.com/bionic/cron
  * https://packages.ubuntu.com/bionic/xz-utils
  * https://packages.ubuntu.com/bionic/zip
  * https://packages.ubuntu.com/bionic/bzip2
  * https://packages.ubuntu.com/bionic/openssl
  * https://packages.ubuntu.com/bionic/openssh-client
  * https://packages.ubuntu.com/bionic/mysql-client
  * https://packages.ubuntu.com/bionic/file

* `FFMPEG_PACKS`
  * https://packages.ubuntu.com/bionic/ffmpeg
  * https://packages.ubuntu.com/bionic/ffmpeg2theora
  * https://packages.ubuntu.com/bionic/libavcodec-extra
  * https://packages.ubuntu.com/bionic/ghostscript
  * https://packages.ubuntu.com/bionic/xpdf
  * https://packages.ubuntu.com/bionic/poppler-utils

* `APACHE_PACKS`
  * https://packages.ubuntu.com/bionic/apache2
  * https://packages.ubuntu.com/bionic/python-mysqldb
  * https://packages.ubuntu.com/bionic/libxml2-dev
  * https://packages.ubuntu.com/bionic/libapache2-mod-php5.6
  * https://packages.ubuntu.com/bionic/libcurl3-openssl-dev
  * https://packages.ubuntu.com/bionic/php5.6
  * https://packages.ubuntu.com/bionic/php5.6-cli
  * https://packages.ubuntu.com/bionic/php5.6-json
  * https://packages.ubuntu.com/bionic/php5.6-common
  * https://packages.ubuntu.com/bionic/php5.6-readline
  * https://packages.ubuntu.com/bionic/php-pear
  * https://packages.ubuntu.com/bionic/php5.6-curl
  * https://packages.ubuntu.com/bionic/php5.6-mbstring
  * https://packages.ubuntu.com/bionic/php5.6-xmlrpc
  * https://packages.ubuntu.com/bionic/php5.6-dev
  * https://packages.ubuntu.com/bionic/php5.6-gd
  * https://packages.ubuntu.com/bionic/php5.6-ldap
  * https://packages.ubuntu.com/bionic/php5.6-xml
  * https://packages.ubuntu.com/bionic/php5.6-mcrypt
  * https://packages.ubuntu.com/bionic/php5.6-mysql
  * https://packages.ubuntu.com/bionic/php5.6-soap
  * https://packages.ubuntu.com/bionic/php5.6-xsl
  * https://packages.ubuntu.com/bionic/php5.6-zip
  * https://packages.ubuntu.com/bionic/php5.6-bcmath
  * https://packages.ubuntu.com/bionic/php5.6-intl
  * https://packages.ubuntu.com/bionic/php-uploadprogress
  * https://packages.ubuntu.com/bionic/php-xdebug
  * https://packages.ubuntu.com/bionic/bibutils
  * https://packages.ubuntu.com/bionic/libicu-dev
  * https://packages.ubuntu.com/bionic/tesseract-ocr
  * https://packages.ubuntu.com/bionic/tesseract-ocr-eng
  * https://packages.ubuntu.com/bionic/tesseract-ocr-fra
  * https://packages.ubuntu.com/bionic/tesseract-ocr-spa
  * https://packages.ubuntu.com/bionic/tesseract-ocr-ita
  * https://packages.ubuntu.com/bionic/tesseract-ocr-por
  * https://packages.ubuntu.com/bionic/tesseract-ocr-hin
  * https://packages.ubuntu.com/bionic/tesseract-ocr-deu
  * https://packages.ubuntu.com/bionic/tesseract-ocr-jpn
  * https://packages.ubuntu.com/bionic/tesseract-ocr-rus
  * https://packages.ubuntu.com/bionic/leptonica-progs
  * https://packages.ubuntu.com/bionic/libleptonica-dev
  * https://packages.ubuntu.com/bionic/libbz2-dev
  * https://packages.ubuntu.com/bionic/libdjvulibre-dev
  * https://packages.ubuntu.com/bionic/libexif-dev
  * https://packages.ubuntu.com/bionic/libgif-dev
  * https://packages.ubuntu.com/bionic/libjpeg8
  * https://packages.ubuntu.com/bionic/libjpeg-dev
  * https://packages.ubuntu.com/bionic/liblqr-dev
  * https://packages.ubuntu.com/bionic/libopenexr-dev
  * https://packages.ubuntu.com/bionic/libopenjp2-7-dev
  * https://packages.ubuntu.com/bionic/libpng-dev
  * https://packages.ubuntu.com/bionic/libraw-dev
  * https://packages.ubuntu.com/bionic/librsvg2-dev
  * https://packages.ubuntu.com/bionic/libtiff-dev
  * https://packages.ubuntu.com/bionic/libwmf-dev
  * https://packages.ubuntu.com/bionic/libwebp-dev
  * https://packages.ubuntu.com/bionic/libwmf-dev
  * https://packages.ubuntu.com/bionic/zlib1g-dev

* `IMAGEMAGICK_LIBS_EXTENDED`  
  * https://packages.ubuntu.com/bionic/libfontconfig
  * https://packages.ubuntu.com/bionic/libfreetype6-dev

The following software packages will be handled and updated by `curl`. Please note there may be further dependencies and libraries updated.

* **openjpeg**
  * https://github.com/uclouvain/openjpeg
  * Last commit used: Nov 1, 2018 https://github.com/uclouvain/openjpeg/commit/0bc90e4062a5f9258c91eca018c019b179066c62

* **ImageMagick** 
  * https://www.imagemagick.org
  * `curl -O -L https://www.imagemagick.org/download/ImageMagick.tar.gz`
  * `convert -v`
  * `Version: ImageMagick 7.0.8-13 Q16 x86_64 2018-10-22 https://imagemagick.org`

* **PHP ImageMagick latest (IMagick)**
  * http://pecl.php.net/package/imagick
  * `curl -O -L http://pecl.php.net/get/imagick`
  * Last released `2017-02-01	imagick-3.4.3.tgz`

* Note: All packages within `$BUILD_DEPS` and `software-properties-common` are purged after install of Imagemagick. Not listed here.

The following software packages will be handled and updated by `composer`. Please note there may be further dependencies and libraries updated.

* **drush**
  * https://www.drush.org/
  * https://packagist.org/packages/drush
  * `drush --version`
  * `Drush Version   :  8.1.17`

#### isle-apache - Software updates requiring manual changes and/or lookups

* **confd**
  *  https://github.com/kelseyhightower/confd/releases/download/v0.16.0/  confd-0.16.0-linux-amd64
  * version 0.16.0
    * No upgrade available at moment

* **Adore-Djatoka**
  * https://sourceforge.mirrorservice.org/d/dj/djatoka/djatoka/1.1/adore-djatoka-1.1.tar.gz
  * No upgrade possible

* **composer**
  * https://getcomposer.org/
  * `/usr/local/bin/composer --version`
  * `Composer version 1.7.2 2018-08-16 16:57:12`
  * `wget -O composer-setup.php https://raw.githubusercontent.com/composer/getcomposer.org/2091762d2ebef14c02301f3039c41d08468fb49e/web/installer`
  * You'll need to visit https://getcomposer.org/doc/faqs/how-to-install-composer-programmatically.md
    * You may replace the commit hash by whatever the last commit hash is on https://github.com/composer/getcomposer.org/commits/master
    * Or copy the hash from the last line
    * Add it to the Dockerfile on line 201 after https://raw.githubusercontent.com/composer/getcomposer.org/

* **fits**
  * https://projects.iq.harvard.edu/fits/home
  * `curl -O -L https://projects.iq.harvard.edu/files/fits/files/fits-1.2.0.zip`
  * You'll need to visit https://projects.iq.harvard.edu/fits/home
    * Click the Downloads button
    * Check the version number e.g. `fits-1.4.0.zip` and the release date `Nov 7, 2018`
    * Update the Dockerfile on lines 213 & 214 with the new version.

### Actions

* Upgraded / Rebuilt **openjpeg** to latest Dec 21,2018 commit https://github.com/uclouvain/openjpeg/commit/51f097e6d5754ddae93e716276fe8176b44ec548

* Upgraded Imagemagick to version `ImageMagick-7.0.8-34.x86_64`

* Upgraded Composer to version `1.84`

* Upgraded Drush to `8.2.1` with `8.1.18` as fallback

* Upgraded FITS to `1.4` (Nov 2018)

* Run package updates

### (Optional) Manual method of reviewing image
* `docker run -p 80:80 -it --rm islandoracollabgroup/isle-apache:1.1 bash`

### Review & Build Process

Assumptions: The `isle-ubuntu-basebox` & `isle-tomcat` images have been already updated, built and pushed to Dockerhub. This image will work (build) from those foundational images.

* Start the update process with the `isle-apache` image
  * Clone down the **FORKED** `isle-apache` to your local development personal computer.
  * Create a new branch e.g. `ISLE-v.1.1.1`

* Edit the following files with the version changes:
  * `Dockerfile`
    * Line 1: change the tag from `FROM islandoracollabgroup/isle-ubuntu-basebox:x` to `isle-ubuntu-basebox:1.1.1`
    * Line 195: change the composer download hash `COMPOSER_HASH:-x` to the newest one used on the composer website. (usually a long string of alpha-numeric characters)
    * Line 196: Change `FITS_VERSION:-x.x.x` to `FITS_VERSION:-1.4.0`

* Commit the changes to git

* Build the image
  * open a terminal
  * `cd ~/pathto/isle-apache`
  * `docker build -t islandoracollabgroup/isle-apache:1.1.1 .`

* Test with new images

### Review & Build Process

#### EXAMPLE / REFERENCE COMMAND

* Review this syntax first to understand how to construct the build command that follows.

```
docker build \
	--build-arg BUILD_DATE=`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	--build-arg VCS_REF=$SOURCE_COMMIT \
	--build-arg VERSION=RC-`date -u \+"%Y-%m-%dT%H:%M:%SZ"` \
	-t $IMAGE_NAME .
```

#### EXAMPLE / ACTUAL COMMAND W/ FORKED COMMIT HASH

* This will be the command to use and construct to build an image to push to Dockerhub. Please note that `VCS_REF` uses the last git commit hash from the forked branch on the image prior to merge in ISLE `master`.

```bash

docker build --build-arg BUILD_DATE=`date -u +"%Y-%m-%dT%H:%M:%SZ"` --build-arg VCS_REF=30d63d6db0007e3682037e5759e0ffcd9e47d203 --build-arg VERSION=RC-`date -u +"%Y-%m-%dT%H:%M:%SZ"` -t islandoracollabgroup/isle-apache:1.1.1 .

```

* Push built images to Dockerhub with new tag
`docker push islandoracollabgroup/isle-apache:1.1.1`

### Tagging

Suggested Outcome:
* deprecate `2.4` tag to `1.0` (April 2018)
* `1.1` (last image build from 11/05/2018)
* `1.1.1` (new image build with sec updates 3/22/2019)
* `latest ` (new image build with sec updates 3/20/2019)

* deprecate the `development` tag

---

### isle-mysql

* Image name: **islandoracollabgroup / isle-mysql** 
  * Last Build: `1.1 / latest` (Last built 5 months ago)
  * Sept 15, 2018 was last build
  * This is an official image which uses `mysql 5.7`
    * https://github.com/docker-library/mysql/blob/bb7ea52db4e12d3fb526450d22382d5cd8cd41ca/5.7/Dockerfile
^ Just check for this

* **MySQL**
  * `mysql --version`
  * `mysql  Ver 14.14 Distrib 5.7.23, for Linux (x86_64) using  EditLine wrapper`

### Recommendations

* Upgrade this image by simple reruning the build process. It will pull the newest version (`5.7.25-1debian9`)of the official image.

### (Optional) Manual method of reviewing image
* `docker run -p 3306:3306 -it --rm islandoracollabgroup/isle-mysql:1.1 bash`

### Review & Build Process

* Start the update process with the `isle-mysql` image
  * Clone down the **FORKED** `isle-mysql` to your local development personal computer.
  * Create a new branch e.g. `ISLE-v.1.1.1`

* Edit the following files with the version changes:
  * `Dockerfile`
    * Line: None to be edited at this point

* Commit the changes to git

* Build the image
  * open a terminal
  * `cd ~/pathto/isle-mysql`
  * `docker build -t islandoracollabgroup/isle-mysql:1.1.1 .`

* Test with new images

* Run Makefile with `docker-build`, etc.

```bash

docker build --build-arg BUILD_DATE=`date -u +"%Y-%m-%dT%H:%M:%SZ"` --build-arg VCS_REF=6473095566c24684eaf483b19169bdf87af04be5 --build-arg VERSION=RC-`date -u +"%Y-%m-%dT%H:%M:%SZ"` -t islandoracollabgroup/isle-mysql:1.1.1 .

```

* Push built images to Dockerhub with new tag
`docker push islandoracollabgroup/isle-mysql:1.1.1`

* Test with new image

### Tagging

Suggested outcome:
* `1.0` (last image build from 9/14/2018)
* `1.1` (last image build from 9/15/2018)
* `1.1.1` (new image build with sec updates 3/25/2019)
* `latest ` (new image build with sec updates 3/25/2019)

* deprecate 5.6 (april 2018)
* fix latest from april 2018 to 3/25/2019)

---

## All images - Testing

* Spin up and QC
  * Did all containers start?

* Install site
  * `docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh`

* Test ingest for all content types

---

## All images  - Release

* **Image Code** - git push image repository edits to personal / company remotes

* **ISLE** code comment out build / context blocks and any other local dev edits