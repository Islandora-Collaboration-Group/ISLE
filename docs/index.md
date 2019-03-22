# Islandora Enterprise (ISLE)

ISLE is a community developed project that supports a set of maintained [Docker](https://docker.com) images that provide a fully functioning [Islandora](https://islandora.ca) platform for development, staging, and production digital repositories. ISLE runs as set of Docker containers (virtual servers) that will run all necessary components of Islandora. Please use the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) to ask questions about ISLE.

This documentation provides friendly and clear instructions on how to do the following types of installations and updates:

* [Demo ISLE Site](#testdemo-isle): quickly create a new Islandora platform on your laptop or workstation (also good for _local_ development)

* [New ISLE Site](#new-isle): create a new Islandora platform on a production, staging, or development server (i.e., starting from scratch)

* [Migrate to ISLE Site](#migrate-to-isle): migrate a (non-ISLE) Islandora 7.x platform to ISLE on a production, staging, or development server

* [Update ISLE Site](#update-isle): update your existing ISLE instance with improvements and security updates


---

## Demo ISLE Site

1. [Hardware Requirements](01_installation_host_server/hardware-requirements.md#testdemo-isle) - Ensure your host server meets these minimum requirements.

2. [Software Dependencies](01_installation_host_server/software-dependencies.md) - Install required software dependencies on host server.

3. [Demo ISLE Site Installation](02_installation_demo_site/demo_installation.md) - Install ISLE, run it, and view your site.


---

## New ISLE Site

Follow these instructions if you **do not** have an existing Islandora installation you wish to migrate. New sites can be used as production, staging, or development environments.

1. [Hardware Requirements](01_installation_host_server/hardware-requirements.md) - Ensure your host server meets these minimum requirements.

2. [Software Dependencies](01_installation_host_server/software-dependencies.md) - Install required software dependencies on host server.

3. Install ISLE, run it, and view your site.
    - [New Site Installation: Single ISLE Environment](03_installation_new_site/new_site_installation_single.md) - Setup **one** ISLE site within a single environment (e.g. production only) or run all of your ISLE environments on separate ISLE host servers or VMs.
    - [New Site Installation: Multiple ISLE Environments](03_installation_new_site/new_site_installation_multiple.md) - Setup **multiple** ISLE sites using multiple environments (e.g. development, staging, and production) on one ISLE Host server or VM.
        - Note: setting up multiple ISLE sites and environments on one ISLE host server will require adequate resources. A rough estimate is to use the [Hardware Requirements](01_installation_host_server/hardware-requirements.md) and multiply by three. Please note that a `dev` environment needs fewer resources, but `stage` and `prod` environments should mirror each other in resource usage and setup.

---


## Migrate to ISLE Site

Migrated sites can be used in production, staging or development environments.

1. [Hardware Requirements](01_installation_host_server/hardware-requirements.md) - Ensure your host server meets these minimum requirements.

2. [Software Dependencies](01_installation_host_server/software-dependencies.md) - Install required software dependencies on host server.

3. [Migration Guide to ISLE](04_installation_migration/migration_installation_guide.md) - Copy your data, configure and run ISLE, and view your site.

---

## Update ISLE Site

1. [Update ISLE Site](07_appendices/update_isle.md) - Update an existing ISLE installation to install the newest improvements and security updates, regardless of type of installation or environment. This process is intended to be backwards compatible with your existing ISLE site.
2. [Release Notes](09_release_notes/release_notes.md) - Please always read the release notes.
