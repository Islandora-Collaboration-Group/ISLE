### ISLE MVP Host Specifications Guide

Currently as this software is in the Alpha stage, the following is recommended for Hosts (Local or Remote). These specifications are subject to change as the project matures and further testing is performed.

The "Host" is defined as the system that is running Docker which in turn runs the ISLE containers independent of type (_see next section_).

---

#### Host Infrastructure (Types)

* **Local**: A laptop or workstation running ISLE.

  * Typically this setup is used for development, testing or otherwise exploring ISLE and Islandora development environments with smaller sample sets.

* **Remote**: A Bare metal (standalone physical server) or a Virtual Machines (VMWare, VSphere, Virtualbox etc) running in an institutional datacenter or cloud platform e.g. Amazon, Google etc.

    * Typically this setup is used for multiple ISLE and Islandora environments (e.g. production, staging and development) with large sized repositories.

---

#### Host Operating Systems

* **Local**: A laptop or workstation running Docker, Docker Compose and ISLE.
   * MacOS Sierra 10.12.x or High Sierra 10.13.x
   * Windows 10 Home or Pro
   * Ubuntu 16.04+ Desktop

* **Remote**: A server or VM running Docker, Docker Compose and ISLE.
   * CentOS 7
   * Ubuntu 16.04 LTS

* **Please note:** At this time, Red Hat Enterprise Server 7 has been tested but there is a distinct lack of clarity as to what version of Docker is running and/or is supported. Unless a license for Docker EE is purchased, the project owners do not recommend using it at this time. Currently ISLE has only been built, tested and approved using the free edition of Docker Community Edition (`docker-ce`)

   * **Challenge 1:** _Docker Community Edition not supported on or by Red Hat_
     * https://docs.docker.com/install/ (see matrix)
     * https://www.docker.com/community-edition

   * **Challenge 2:** _Red Hat wants endusers to pay for Docker EE license_
     * https://www.docker.com/enterprise-edition

   * **Challenge 3:** _Choices for alternate free Docker versions supported by Red Hat are not clear in features comparison or support roadmap._
     * https://docs.docker.com/release-notes/docker-ce/
     * https://access.redhat.com/articles/2317361

---

#### Host Memory

_If an institution chooses to run more than one ISLE platform (e.g. production, staging and development configurations) all on one host server, increasing RAM is recommended)_

* **Local:** minimum 8 GB RAM or higher.

* **Remote:** minimum 16 GB RAM or higher.

---

#### Host CPU

_If an institution chooses to run more than one ISLE platform (e.g. production, staging and development configurations) all on one host server, increasing CPU cores and speed is recommended)_

* **Local or Remote:** minimum of 2 cores running at 2.0 GHz or higher.

---

#### Host Storage

While `ISLE` and its configs, images, software etc are typically cloned to `/opt/ISLE` and controlled from that location some data can be stored in `/var/docker`.
  * Please ensure that both the host server's `/opt` and `/var` directories are on disks, mounts or storage systems that can accommodate at least 200 GB in size. In some cases, institutions during the Alpha testing of ISLE increased the size of the host server's `opt` directory by:
  * Increasing the overall size of the host server VM beyond 200GB
  * Attaching separate storage capacity directly to `/opt` and `/var`
  * Additional documentation will be forthcoming on how mounts, volumes and directories can be setup to avoid shortages and for data growth.

* **Local:** minimum 128 GB or higher for data storage

    * _Can be added as separate mounts / volumes_

* **Remote:** minimum 2 TB or higher for data storage     

    * _Can be added as separate mounts / volumes_

    * _If migrating an existing Islandora Production site, review current production storage and replicate this accordingly_
