**Please select your desired type of ISLE installation:**

* [New ISLE](#new-isle): create a new ISLE instance on a production, staging or development server

* [Update ISLE](#update-isle): update a preexisting ISLE instance with improvements and security updates

* [Migrate to ISLE](#migrate-to-isle): migrate a non-ISLE Islandora 7x instance to ISLE on a production, staging or development server

* [Test/Demo ISLE](#test-and-demo-isle): create a new ISLE instance on my laptop or workstation

**Please note:** There is a [Glossary](glossary.md) with relevant terms to help guide installation.

---

## New ISLE

Typically new sites are used in production, staging or shared development environments.  If you do not have an existing Islandora installation you need to migrate, follow these instructions. 

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

**Please Note**:  These instructions assume you have already installed the base operating system (Ubuntu 18.04 or Centos 7.x) and have ssh and root/sudo access for the user `islandora`.  The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

First, follow one of the guides below to configure your server for ISLE:

* [Ubuntu 18.04](01_installation_host_server/install_on_ubuntu_1804.md)

* [Centos 7.x](01_installation_host_server/install_on_centos.md)

Once the prerequisites are installed, you will need to install Islandora:

* If you are only going to setup one ISLE site within a single environment (e.g. production only) or run all of your ISLE environments on separate ISLE host servers or VMs please using the: [New ISLE Site - Single Env](03_installation_new_site/new_site_installation_guide_single.md) once you have set up the host server.

* If you are going to setup multiple ISLE sites using multiple environments (e.g. dev, stage, and prod etc.) on one ISLE Host server or VM please using the: [New ISLE Sites - Multi Envs](03_installation_new_site/new_site_installation_guide_multi.md) once you have set up the host server.
    * Note: setting up multiple ISLE sites and environments on one ISLE host server will require adequate resources. A good rule of thumb is to take the specifications outlined in the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) and divide by three as an initial concept. Bear in mind, that any `dev` environment doesn't need as much resource but any potential `stage` and `prod` environments should mirror each other in resource usage and setup.

---

## Update ISLE

The following offers a method for updating an existing ISLE installation to install the newest improvements and security updates, regardless of environment.

This process is backwards compatible with your existing ISLE site.

- DETAILS COMING SOON...

---

## Migrate to ISLE

The recommended environments for migrating a current production Islandora site to a new ISLE environment are with the following type of servers. (_not desktop or workstation_)

Typically migrated sites are used in production, staging or shared development environments.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

**Please Note**:  These instructions assume you have already installed the base operating system (Ubuntu 18.04 or Centos 7.x) and have ssh and root/sudo access for the user `islandora`.  The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

First, follow one of the guides below to configure your server for ISLE:

* [Ubuntu 18.04](01_installation_host_server/install_on_ubuntu_1804.md)

* [Centos 7.x](01_installation_host_server/install_on_centos.md)

Once the prerequisites are installed, you will need to install Islandora:

* [Migrate to ISLE](04_installation_migration/migration_installation_guide.md)

---

## Test/Demo ISLE

The recommended non-production environments for using the prebuilt ISLE test site (`isle.localdomain`) are with the following type of laptops or workstations.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

ISLE is designed and tested to work on the following OSes and environments.  Follow the link for your preferred system for instructions on setting up the testing environment.

* [Mac OSX High Sierra and Docker](01_installation_host_server/install_docker_for_mac.md)

* [Mac OSX High Sierra and Vagrant & VirtualBox](01_installation_host_server/install_macos_vagrant.md)

* [Windows and Docker](01_installation_host_server/install_docker_for_windows.md)

* [Ubuntu 18.04](01_installation_host_server/install_on_ubuntu_1804.md)

* [Centos 7.x](01_installation_host_server/install_on_centos.md)

Advice for installing a test instance of ISLE on other platforms may be available from the [ISLE ISLE Google Group](https://groups.google.com/forum/#!forum/islandora-isle).


