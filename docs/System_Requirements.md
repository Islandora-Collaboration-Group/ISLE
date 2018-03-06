ISLE will (using Docker) install and manage a set of virtual "servers" (containers) that will run all the components of Islandora. Docker requires a host server upon which to do this.

This **host server** can be a virtual machine (VM) on your desktop or on the cloud, it can also be a physical server. An SSH connection with a user that has root or admin privileges is necessary. If an IT department is doing this initial setup, see below for sample language to use in requesting the server.

The basic requirements for this host machine vary depending on intended use. A VM on a desktop computer will be sufficient to set up a test or development instance.

**Local: A laptop or workstation running Docker, Docker Compose and ISLE.**

  * 8 GB or RAM is recommended
  * MacOS Sierra 10.12.x or High Sierra 10.13.x
  * Windows 10 Home or Pro
  * Ubuntu 16.04+ Desktop


However, if the intent is a site that will see actual production use, sufficient processor speed, memory, and storage space is required.

**Server Specifications**

 * Ubuntu 16.04 LTS Server or CentOS 7.x server<sup>* </sub>
 * minimum 2 CPU's
 * Sufficient HD or attached storage to hold your collection plus 30-50GB for overhead
 * 24 GB of RAM

<sup>* </sub>Ubuntu 16.04 and CentOS 7.x have been tested with ISLE - currently RHELS does not support Docker Community Edition.

##Sample Request to an IT department
----------------------------------------------------
Dear IT Dept,

Our digital collections team will be testing a Docker implementation of the Islandora digtial repository system called [ISLE](https://islandora-collaboration-group.github.io/ISLE/). We require access to a server meeting the following specifications:

 * Ubuntu 16.04 LTS Server or CentOS 7.x server
 * minimum 2 CPU's
 * Sufficient HD or attached storage to hold your collection plus 30-50GB for overhead
 * 24 GB of RAM

 Our team will either need root access, or a user called "islandora" that has sudo privileges. We will need the following software which we can install following the instructions at [this link](0101_Installation_Migration/01_1_testsite_guide.md)

  * Git, Docker, Docker-Compose, openssl, git, htop, ntp, curl

  ----------------------------------------------------
