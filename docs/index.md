# ISLE: Islandora Enterprise

ISLE is a community developed project that supports a set of maintained [Docker](https://docker.com) images that provide a fully functioning [Islandora](https://islandora.ca) platform for development, staging, and production digital repositories. ISLE runs as set of Docker containers (virtual servers) that will run all necessary components of Islandora.

The purpose of this documentation is to provide clear instructions on how to do the following types of installations and updates:

* [Test/Demo ISLE](#testdemo-isle): quickly create a new Islandora platform on your laptop or workstation (also good for _local_ development)

* [New ISLE](#new-isle): create a new Islandora platform on a production, staging, or development server (i.e., starting from scratch)

* [Migrate to ISLE](#migrate-to-isle): migrate a (non-ISLE) Islandora 7.x platform to ISLE on a production, staging, or development server

* [Update ISLE](#update-isle): update your existing ISLE instance with improvements and security updates


---

## Test/Demo ISLE

1. Ensure you meet the hardware [Server Requirements](01_installation_host_server/server-requirements.md#testingplayground).

2. Install the Required Software on your host server: [Docker CE, Docker-Compose, and git.](01_installation_host_server/software-dependencies.md)

3. Configure and run ISLE after the prerequisites are installed: [Test/Demo ISLE Deployment Guide](02_installation_test/ild_installation_guide.md).

Advice for installing a test instance of ISLE on other platforms may be available from the [ISLE ISLE Google Group](https://groups.google.com/forum/#!forum/islandora-isle).


---

## New ISLE

Follow these instructions if you **do not** have an existing Islandora installation you wish to migrate. New sites can be used as production, staging, or development environments. 

1. Ensure you meet the hardware [Server Requirements](01_installation_host_server/server-requirements.md).

2. Install the Required Software on your host server: [Docker CE, Docker-Compose, and git.]

3. Configure and run ISLE after the prerequisites are installed.
    - If you are only going to setup one ISLE site within a single environment (e.g. production only) or run all of your ISLE environments on separate ISLE host servers or VMs please using the: [New ISLE Site - Single Env](03_installation_new_site/new_site_installation_guide_single.md) once you have set up the host server.
    - If you are going to setup multiple ISLE sites using multiple environments (e.g. dev, stage, and prod etc.) on one ISLE Host server or VM please using the: [New ISLE Sites - Multi Envs](03_installation_new_site/new_site_installation_guide_multi.md) once you have set up the host server.
        - Note: setting up multiple ISLE sites and environments on one ISLE host server will require adequate resources. A good rule of thumb is to take the specifications outlined in the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) and divide by three as an initial concept. Bear in mind, that any `dev` environment doesn't need as much resource but any potential `stage` and `prod` environments should mirror each other in resource usage and setup.

---


## Migrate to ISLE

Migrated sites can be used in production, staging or development environments.

1. Ensure you meet the hardware [Server Requirements](01_installation_host_server/server-requirements.md).

2. Install the Required Software on your host server: [Docker CE, Docker-Compose, and git.](01_installation_host_server/software-dependencies.md)

3. Copy your data, configure and run ISLE after the prerequisites are installed by following our [Migration Guide to ISLE](04_installation_migration/migration_installation_guide.md)

---

## Update ISLE

The following offers a method for updating an existing ISLE installation to install the newest improvements and security updates, regardless of environment.

This process is backwards compatible with your existing ISLE site.

Please read these  instructions: [Update ISLE](07_appendices/update_isle.md)
