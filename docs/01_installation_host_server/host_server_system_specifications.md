ISLE will (using Docker) install and manage a set of virtual "servers" (containers) that will run all the components of Islandora. Docker requires a host server upon which to do this.

This **host server** can be a virtual machine (VM) on your desktop or on the cloud, it can also be a physical server. An SSH connection with a user that has root or admin privileges is necessary. If an IT department is doing this initial setup, see below for sample language to use in requesting the server.

The basic requirements for this host machine vary depending on intended use. A VM on a desktop computer will be sufficient to set up a test or development instance.

## Specifications for Desktop or Laptops

**Desktop: A laptop or workstation running Docker, Docker Compose and ISLE.**

  * 8 - 16 GB RAM is recommended
  * Minimum 1 - 2 CPUs (w/ 1 - 4 cores each)  
  * 128 - 500GB for the Desktop OS
  * Sufficient HD space to hold a test collection
  * MacOS High Sierra 10.13.x
  * Windows 10 Home or Pro
  * Ubuntu 16.04+ Desktop

## Specifications for Production Servers

However, if the intent is a new or migrated site that will see actual production use then it is recommended that increased processor speed, memory, and storage space will be required.

**Server Specifications**

* Ubuntu 16.04 LTS or CentOS 7.x running on a server or VM
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead
* 16 - 32 GB of RAM is recommended

<sup>* </sub>Ubuntu 16.04 and CentOS 7.x have been tested with ISLE - _currently RHELS does not support Docker Community Edition (CE), see Note about Red Hat section below_

## Note about Red Hat (warning)

At this time, Red Hat Enterprise Server 7 has been tested but there is a distinct lack of clarity as to what version of Docker is running and/or is supported.

It appears that unless a license for Docker EE is purchased, the project owners do not recommend using Red Hat at this time due to unanticipated cost.

Currently ISLE has only been built, tested and supported using the free edition of Docker Community Edition (`docker-ce`).

   * **Challenge 1:** _Docker Community Edition not clearly supported on or by Red Hat_
     * [https://docs.docker.com/install/](https://docs.docker.com/install/) (see matrix)
     * [https://www.docker.com/community-edition](https://www.docker.com/community-edition)

   * **Challenge 2:** _Red Hat seems to direct endusers to pay for Docker EE license instead of using Docker Community Edition_
     * [https://www.docker.com/enterprise-edition](https://www.docker.com/enterprise-edition)

   * **Challenge 3:** _Choices for alternate free Docker versions supported by Red Hat are not clear in features comparison or support roadmap._
     * [https://docs.docker.com/release-notes/docker-ce/](https://docs.docker.com/release-notes/docker-ce/)
     * [https://access.redhat.com/articles/2317361](https://access.redhat.com/articles/2317361)


---

## Note about Production Host Storage (optional)

While `ISLE` and its configs, images, software etc are typically cloned to `/opt/ISLE` and controlled from that location some data can be stored in `/var/docker`.

Please ensure that both the host server's `/opt` and `/var` directories are on disks, mounts or storage systems that can accommodate at least 200 GB in size. In some cases, institutions during the Alpha testing of ISLE increased the size of the host server's `opt` directory by:

  * Potentially increasing the overall size of the host server VM beyond 200GB
  * Attaching separate storage capacity directly to the `/opt` and `/var` directories

## Sample Request to an IT department (optional)

Dear IT Dept,

Our digital collections team will be testing a Docker implementation of the Islandora digital repository system called [ISLE](https://islandora-collaboration-group.github.io/ISLE/).

We require access to a server meeting the following specifications:

 * Ubuntu 16.04 LTS or CentOS 7.x running on a server or VM
 * Minimum 2 CPU's (w/1-4 cores each)
 * Sufficient HD or attached storage to hold your collection
 * 30 - 50GB for the Server OS & overhead
 * 16 - 32 GB of RAM

Our team will either need root access, or a user called "islandora" that has sudo privileges.

We will need the following software which we can install following the instructions at [this link](host_server_system_specifications.md)

 * Git, Docker, Docker-Compose, openssl, git, htop, ntp, curl

Thank you,  
Requesting Dept.
