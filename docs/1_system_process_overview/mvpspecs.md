### MVP Specs

Currently as this software is in the Alpha stage, we recommend the following for host Servers

#### Host servers infrastructure can be:

* Bare metal (standalone physical server)
* Virtual Machines (VMWare, VSphere, Virtualbox etc)

#### Host Server Operating Systems

* CentOS 7 (or RHEL equivalent)
* Unbunt 16.04 LTS (or Debian equivalent)

#### Host Server Memory

* **Recommended** - Minimum baseline of 16 GB RAM or higher

_If an institution chooses to run more than one ISLE platform (e.g. production, staging and development configurations) all on one host server, increasing RAM is recommended)_

##### Host Server CPU

* **Recommended** - Minimum baseline of 2 cores or more


##### Host Server Storage

* **Recommended** - Minimum baseline of 2 - 4 TB for data storage (_can be added as separate mounts / volumes_)
* _If migrating an existing Islandora Production site, review current production storage and replicate this accordingly_

* **Recommended** - While `ISLE` and its configs, images, software etc are typically cloned to `/opt/ISLE` and controlled from that location some data can be stored in `/var/docker`.
  * Please ensure that both the host server's `/opt` and `/var` directories are on disks, mounts or storage systems that can accommodate at least 200 GB in size. In some cases, institutions during the Alpha testing of ISLE increased the size of the host server's `opt` directory by:
  * Increasing the overall size of the host server VM beyond 200GB
  * Attaching separate storage capacity directly to `/opt` and `/var`
  * Additional documentation will be forthcoming on how mounts, volumes and directories can be setup to avoid shortages and for data growth.
