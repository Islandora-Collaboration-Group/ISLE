<!--- PAGE_TITLE --->

# Islandora Enterprise (ISLE)

ISLE is a community developed project that supports a set of maintained [Docker](https://docker.com) images that provide a fully functioning [Islandora](https://islandora.ca) platform for development, staging, and production digital repositories. ISLE runs as set of Docker containers (virtual servers) that will run all necessary components of Islandora. Please use the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) to ask questions about ISLE.

This documentation provides friendly and clear instructions on how to do the following types of installations and updates:

* [Demo ISLE Site](#demo-isle-site): quickly create a new Islandora platform on your laptop or workstation (also good for _local_ development)

* [New ISLE Site](#new-isle-site): create a new Islandora platform on a production, staging, or development server (i.e., starting from scratch)

* [Migrate to ISLE Site](#migrate-to-isle-site): migrate a (non-ISLE) Islandora 7.x platform to ISLE on a production, staging, or development server

* [Update ISLE Site](#update-isle-site): update your existing ISLE instance with improvements and security updates

---

## Demo ISLE Site

1. [Hardware Requirements](install/host-hardware-requirements.md) - Ensure your host server meets these minimum requirements.

2. [Software Dependencies](install/host-software-dependencies.md) - Install required software dependencies on host server.

3. [Demo ISLE Site Installation](install/install.md) - Install ISLE, run it, and view your site.

---

## New ISLE Site

Follow these instructions if you **do not** have an existing Islandora installation you wish to migrate. New sites can be used as production, staging, or development environments.

1. [Hardware Requirements](install/host-hardware-requirements.md) - Ensure your host server meets these minimum requirements.

2. [Software Dependencies](install/host-software-dependencies.md) - Install required software dependencies on host server.

3. Install ISLE, run it, and view your site.
    * [New Site Installation: Single ISLE Environment](install/install-one-environment.md) - Setup **one** ISLE site within a single environment (e.g. production only) or run all of your ISLE environments on separate ISLE host servers or VMs.
    * [New Site Installation: Multiple ISLE Environments](install/install-multiple-environments.md) - Setup **multiple** ISLE sites using multiple environments (e.g. development, staging, and production) on one ISLE Host server or VM.
        - Note: setting up multiple ISLE sites and environments on one ISLE host server will require adequate resources. A rough estimate is to use the [Hardware Requirements](install/host-hardware-requirements.md) and multiply by three. Please note that a `dev` environment needs fewer resources, but `stage` and `prod` environments should mirror each other in resource usage and setup.

---

## Migrate to ISLE Site

Migrated sites can be used in production, staging or development environments.

1. [Hardware Requirements](install/host-hardware-requirements.md) - Ensure your host server meets these minimum requirements.

2. [Software Dependencies](install/host-software-dependencies.md) - Install required software dependencies on host server.

3. [Migration Installation Guide](migrate/install-migration.md) - Copy your data, configure and run ISLE, and view your site.

---

## Update ISLE Site

1. [Update ISLE Site](update/update.md) - Update an existing ISLE installation to install the newest improvements and security updates, regardless of type of installation or environment. This process is intended to be backwards compatible with your existing ISLE site.
2. [Release Notes](release-notes/release.md) - Please always read the release notes.
