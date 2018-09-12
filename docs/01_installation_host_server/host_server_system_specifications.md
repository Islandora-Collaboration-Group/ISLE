ISLE will (using Docker) install and manage a set of virtual "servers" (containers) that will run all the components of Islandora. Docker requires a host server upon which to do this.

This **host server** can be a virtual machine (VM) on your desktop or on the cloud, it can also be a physical server. An SSH connection with a user that has root or admin privileges is necessary. If an IT department is doing this initial setup, see [this page](../07_appendices/sample-it-department-request.md) for sample language to use in requesting the server.

The basic requirements for this host machine vary depending on intended use. A VM on a desktop computer will be sufficient to set up a test or development instance.

## Specifications for Desktop or Laptops

**Desktop: A laptop or workstation running Docker, Docker Compose and ISLE.**

  * 8 - 16 GB RAM is recommended
  * Minimum 1 - 2 CPUs (w/ 1 - 4 cores each)  
  * 128 - 500GB for the Desktop OS
  * Sufficient HD space to hold a test collection
  * MacOS High Sierra 10.13.x
  * Windows 10 Home or Pro
  * Ubuntu 18.04+ Desktop

## Specifications for Production Servers

However, if the intent is a new or migrated site that will see actual production use then it is recommended that increased processor speed, memory, and storage space will be required.

**Server Specifications**

* While these images can be run on any OS (i.e., they're platform agnostic) we suggest a server or VM running:
	* Ubuntu 18.04 LTS  or 
	* CentOS 7.x
	* Please note you are not limited to these, and ISLE should perform equally on all OSes.
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead
* 16 - 32 GB of RAM is recommended

<sup>* </sup>Ubuntu 16.04 and CentOS 7.x have been tested with ISLE - _currently RHELS does not support Docker Community Edition (CE), see Note about Red Hat section below_


---

## Note about Production Host Storage (optional)

While `ISLE` and its configs, images, software etc are typically cloned to `/opt/ISLE` and controlled from that location some data can be stored in `/var/docker`.

Please ensure that both the host server's `/opt` and `/var` directories are on disks, mounts or storage systems that can accommodate at least 200 GB in size. Some institutions have increased the size of the host server's `opt` directory by:

  * Potentially increasing the overall size of the host server VM beyond 200GB
  * Attaching separate storage capacity directly to the `/opt` and `/var` directories


