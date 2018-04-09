_Expectations:  It can take an up to an average of **2 -4 hours** to read this entire document and perform the installation as prescribed. Highly dependent on the Developer's local laptop and Internet speed._

**Caution**: This guide is recommended for ISLE maintainers or Developers only and not for end-users.

ISLE maintainers are to follow this guide to build the five Docker ISLE images using the `docker-compose-build.yml` file.

Developers can follow this guide if they have edited their Dockerfiles with custom software and they then can push to their own Docker image repository.

These images will contain the necessary base code, scripts and files to run the default sample `isle.localdomain`.

This test ISLE / Islandora environment (`isle.localdomain`) includes an un-themed Drupal website and empty Fedora repository for endusers to develop code, test ingests, test metadata, update fields in SOLR indexing and otherwise "kick the tires" on ISLE prior for further usages e.g. creating a new ISLE production site or migrating a current Islandora production site to ISLE.

While this checklist will attempt to point out most of the usage challenges or pitfalls, ISLE assumes no responsibility or liability in this matter should an enduser have customizations beyond what this guide outlines.

**Please note:** There is a [Glossary](../glossary.md) with relevant terms to help guide installation.

### Assumptions / Prerequisites

* Laptop / workstation that conforms to the specifications outlined in the [Host Server Specifications](../01_installation_host_server/host_server_system_specifications.md)

* This image build site guide is designed for a local laptop / workstation that has already followed the appropriate setup and configuration instructions in the `Test ISLE` section of the [Install Start Here](../install_start_here.md) guide.

* Instructions below assume a MacOS or Linux laptop or workstation. Windows users may have to adjust / swap out various tools as needed.

* The ISLE project has been cloned from the ISLE project repository using the `master` branch. Do not build ISLE images with untested and developmental code. Only stable releases please.

* Having a cleaned up system prior to any build is key. Ensure that all local development containers, volumes and images associated with the build are deleted. Otherwise previously unaltered data or configurations could break or alter negatively the new images.

* ISLE maintainers, Developers have pre-existing knowledge or familiarity with [Dockerfiles](https://docs.docker.com/engine/reference/builder/), the [Docker Build](https://docs.docker.com/engine/reference/commandline/build/) process and [pushing Docker images](https://docs.docker.com/engine/reference/commandline/push/) to repositories.

* [Dockerfiles](https://docs.docker.com/engine/reference/builder/), their editing process and compositions are familiar or known concepts.

* ISLE maintainers should already have access to the [ISLE Dockerhub](https://hub.docker.com/r/islandoracollabgroup) image repository.

* Developers should have already setup their own separate Docker image repository. e.g. [Dockerhub](https://hub.docker.com) following that site's instructions.

* Developers are free to mix official ISLE images with your own custom image build. Best practice would be to create new Dockerfiles that use the official ISLE images as the base image to build from.

---

### Step 1: Ensure there are no local ISLE Docker images present

If performing a brand new image build, the ISLE maintainer will need to ensure there are no previous image builds or cached build data.

* `docker image ls`

* Remove any ISLE images by selecting their `IMAGE ID`

Example:
```
docker image ls

REPOSITORY                         TAG                 IMAGE ID            CREATED             SIZE
islandoracollabgroup/isle-proxy    1.13                7cbd80c8b043        13 days ago         109MB
islandoracollabgroup/isle-proxy    latest              7cbd80c8b043        13 days ago         109MB
islandoracollabgroup/isle-apache   2.4                 ba6505e7a428        13 days ago         2.25GB
islandoracollabgroup/isle-apache   latest              ba6505e7a428        13 days ago         2.25GB
islandoracollabgroup/isle-apache   <none>              9bc727f301b8        2 weeks ago         2.25GB
islandoracollabgroup/isle-proxy    <none>              23cb1c73c232        2 weeks ago         109MB
```

*  `docker image rm -f 7cbd80c8b043`
*  `docker image rm -f ba6505e7a428`
*  `docker image rm -f 7cbd80c8b043`
*  `docker image rm -f 9bc727f301b8`
*  `docker image rm -f 23cb1c73c232`

### Step 2: Rename the docker-compose.yml file

* Rename the `docker-compose.yml` file to `docker-compose.bak`

### Step 3: Rename the docker-compose-build.yml file

* Rename the `docker-compose-build.yml` file to `docker-compose.yml`

### Step 3a: Alternative for non-ISLE maintainers only e.g. Developers

For **non-ISLE maintainers only e.g. Developers (only)**  you'll need to change the following lines in the newly renamed `docker-compose.yml` file.
```
    image: islandoracollabgroup/isle-mysql:latest
    image: islandoracollabgroup/isle-fedora:latest
    image: islandoracollabgroup/isle-apache:latest
    image: islandoracollabgroup/isle-solr:latest
    image: islandoracollabgroup/isle-proxy:latest

    to

    image: yourdockerimagerepo_orgname/isle-mysql:latest
    image: yourdockerimagerepo_orgname/isle-fedora:latest
    image: yourdockerimagerepo_orgname/isle-apache:latest
    image: yourdockerimagerepo_orgname/isle-solr:latest
    image: yourdockerimagerepo_orgname/isle-proxy:latest

```

Change only the images you need to rebuild or customize.

### Step 4: Build the images in this suggested order

Please perform these steps one at time. If any errors occur during the build process, troubleshoot and edit the associated Dockerfile.

* `docker-compose build mysql`

* `docker-compose build fedora`

* `docker-compose build apache`

* `docker-compose build proxy`

* `docker-compose build solr`

### Step 5: Tag the resulting images

This renamed `docker-compose-build.yml` file will tag all builds by default with the `latest` tag.

Review the current tags found on the [Docker Containers & Images](../06_specifications/specs_docker_containers_images.md) Specifications Page. Should the software version change, update this document with the new tags. You will need to tag each image further as needed. There are always two tags for every image, e.g. `latest` and the specific version of the primary software package used on the image or container. It is possible there will be more tags for different image versions in the future.

For example if the installed proxy nginx software changes from `1.13` to `1.14`, change or add the new tag of `1.14`

To learn more about tagging, read the official Docker [tag](https://docs.docker.com/engine/reference/commandline/tag/) documentation.

To tag images:

* `docker image ls` (_to see the new IMAGE IDs_)
* `docker tag IMAGE ID image/TAG`

IMAGE ID is the 12-character identification string for the image (listed from the Docker images command), and TAG is our newly created versioning tag. So our command to tag a new `proxy` image would be:

* `docker tag 7cbd80c8b043 islandoracollabgroup/isle-proxy:1.13`

Please note: your IMAGE ID will be different than the examples above.

Developers, please note you are welcome to use tags that are useful for your institution e.g. `docker tag 7cbd80c8b043 yourdockerimagerepo_orgname/isle-proxy:1.13`

### Step 6: Push the images to the image repository

**Please note:** These steps assume use of Dockerhub. If you use a different Docker image repository, you'll need to change the following steps per their specific instructions.

`docker login`

Example output:
```
    Login with your Docker ID to push and pull images from Docker Hub. If you don't have a Docker ID, head over to https://hub.docker.com to create one.
    Username (username_here):
```
* Enter your Dockerhub username
* Enter your Dockerhub user password.

Push the images one at a time. **Please note:** _Depending on your Internet connection's upload speed this process may take several hours._

* `docker push islandoracollabgroup/isle-apache:latest`
* `docker push islandoracollabgroup/isle-fedora:latest`
* `docker push islandoracollabgroup/isle-mysql:latest`
* `docker push islandoracollabgroup/isle-proxy:latest`
* `docker push islandoracollabgroup/isle-solr:latest`

Repeat the process with any additional tags. **Please note:** _Depending on your Internet connection's upload speed this process may take several minutes._

* `docker push islandoracollabgroup/isle-apache:2.4`
* `docker push islandoracollabgroup/isle-fedora:3.8.1`
* `docker push islandoracollabgroup/isle-mysql:5.6`
* `docker push islandoracollabgroup/isle-proxy:1.13`
* `docker push islandoracollabgroup/isle-solr:4.10.4`

### Step 7: Rename the docker-compose-build.yml file

* Rename the `docker-compose.yml` file to `docker-compose-build.yml`

### Step 8: Rename the docker-compose.yml file

* Rename the `docker-compose.bak` file to `docker-compose.yml`

---

Steps below are mandatory for any ISLE maintainer but optional (though recommended) for any Developer.

### Step 9: Delete all recently built and pushed ISLE images on local laptop

* `docker image ls`

Example commands to repeat for each type of image.

* `docker rm -f IMAGE ID islandoracollabgroup/TAG`
* `docker rm -f 7cbd80c8b043 islandoracollabgroup/isle-proxy:1.13`

### Step 10: Test the images pushed.

Pull down the newly pushed images.

* `docker pull islandoracollabgroup/isle-apache:latest`
* `docker pull islandoracollabgroup/isle-fedora:latest`
* `docker pull islandoracollabgroup/isle-mysql:latest`
* `docker pull islandoracollabgroup/isle-proxy:latest`
* `docker pull islandoracollabgroup/isle-solr:latest`

Follow the instructions within the `docker-compose.yml` file or within the [Test Site Installation Guide](../02_installation_test/ild_installation_guide.md) to start up containers and install the ISLE Test Site (`isle.localdomain`).

Ensure that all components, connections etc are working, otherwise troubleshoot as required, making the appropriate fixes to the associated Dockerfiles, pushing changes to the ISLE project git repository and then REPEATING this entire build and push process.

Always test prior and post to ensure there are no issues pushed to endusers.
