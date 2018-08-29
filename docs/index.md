# ISLE: Islandora Enterprise

ISLE is a set of resources for building and updating a fully functioning Islandora system quickly using a system building tool called [Docker](https://docker.com). The purpose of this documentation is to provide clear instructions on how to do one, or all, of the following:

* [Test ISLE](#test-isle) installation on my laptop or workstation

* [Create a new ISLE site](#create-a-new-isle-site-or-sites) on a production, staging or development server

* [Migrate to ISLE](#migrate-to-isle) on a production, staging or development server

---

## Test ISLE

Below are the recommended specifications for a laptop or desktop running a TEST version of ISLE:

  * <span style='font-size:medium'>8 - 16 GB RAM is recommended</span>
  * <span style='font-size:medium'>Minimum 1 - 2 CPUs (w/ 1 - 4 cores each)  </span>
  * <span style='font-size:medium'>128 - 500GB for the Desktop OS</span>
  * <span style='font-size:medium'>Sufficient HD space to hold a test collection</span>

ISLE is designed and tested to work on the following operating systems.  Follow the link for your system for instructions on setting up the testing environment.

* [Mac OSX High Sierra](01_installation_host_server/install_docker_for_mac.md)

* [Windows](01_installation_host_server/install_docker_for_windows.md)

* [Ubuntu 18.04](01_installation_host_server/install_on_ubuntu_1804.md)

* [Centos 7.x](01_installation_host_server/install_on_centos.md)

Advice for installing a test instance of ISLE on other platforms may be available from the [ISLE ISLE Google Group](https://groups.google.com/forum/#!forum/islandora-isle).

---

## Create a new ISLE site or site(s)

Typically new sites are used in production, staging or shared development environments.  If you do not have an existing Islandora installation you need to migrate, follow these instructions. 

Below are the recommended minimum specifications for a production server. The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

* <span style='font-size:medium'>Ubuntu 18.04 LTS or CentOS 7.x running on a server or VM</span>
* <span style='font-size:medium'>Minimum 2 CPU's (w/ 1 - 4 cores each)</span>
* <span style='font-size:medium'>Sufficient HD or attached storage to hold your collection</span>
* <span style='font-size:medium'>30 - 50GB for the Server OS & overhead</span>
* <span style='font-size:medium'>16 - 32 GB of RAM is recommended</span>

First, follow one of the guides below to configure your server for ISLE (**Please Note**:  These instructions assume you have already installed the base operating system (Ubuntu 18.04 or Centos 7.x) and have ssh and root/sudo access for the user `islandora`):

* [Ubuntu 18.04](01_installation_host_server/install_on_ubuntu_1804.md)

* [Centos 7.x](01_installation_host_server/install_on_centos.md)

Once the prerequisites are installed, you will need to install Islandora:

* If you are only going to setup one ISLE site within a single environment (e.g. production only) or run all of your ISLE environments on separate ISLE host servers or VMs please using the: [New ISLE Site - Single Env](03_installation_new_site/new_site_installation_guide_single.md) once you have set up the host server.

* If you are going to setup multiple ISLE sites using multiple environments (e.g. dev, stage, and prod etc.) on one ISLE Host server or VM please using the: [New ISLE Sites - Multi Envs](03_installation_new_site/new_site_installation_guide_multi.md) once you have set up the host server.
    * Note: setting up multiple ISLE sites and environments on one ISLE host server will require adequate resources. A good rule of thumb is to take the specifications outlined in the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) and divide by three as an initial concept. Bear in mind, that any `dev` environment doesn't need as much resource but any potential `stage` and `prod` environments should mirror each other in resource usage and setup.


---

## Migrate to ISLE

Typically migrated sites are used in production, staging or shared development environments.

Below are the recommended minimum specifications for a production server. The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or VM
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead
* 16 - 32 GB of RAM is recommended

First, follow one of the guides below to configure your server for ISLE (**Please Note**:  These instructions assume you have already installed the base operating system (Ubuntu 18.04 or Centos 7.x) and have ssh and root/sudo access for the user `islandora`):

* [Ubuntu 18.04](01_installation_host_server/install_on_ubuntu_1804.md)

* [Centos 7.x](01_installation_host_server/install_on_centos.md)

Once the prerequisites are installed, you will need to install Islandora:

* [Migrate to ISLE](04_installation_migration/migration_installation_guide.md)