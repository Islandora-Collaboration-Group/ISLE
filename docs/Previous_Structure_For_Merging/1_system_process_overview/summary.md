### Project Overview

ISLE public repository: https://github.com/Islandora-Collaboration-Group/ISLE

### What is Islandora Enterprise (ISLE)?
Islandora Enterprise (ISLE) is a new project in development that will address one of the most significant pain-points in Islandora: installation and maintenance.

### How does it work?
ISLE separates an institution’s customizations from core code, and moves that core code into containers that are easily updated, simplifying and largely automating the process of installation and updates/maintenance of Islandora. ISLE also bundles together the best shared modules into a common, production-ready and security-hardened platform.

### Why is it needed?
Islandora is a powerful digital repository comprised of more than 80 different open-source software libraries. This complex ecosystem makes Islandora difficult and expensive to install, maintain and customize. ISLE lowers the barrier to entry for new schools while allowing existing institutions to reallocate funds towards development or ingestion instead of maintenance.

### High Level Objective
To deliver a secure and production-ready Islandora stack (“ISLE”) that reduces the involved support time and effort for installation, maintenance and customization.

### ISLE Functional Definition
By ISLE, it is meant a system that will:  

- Function as both an initial installer and maintenance updater for Islandora stacks with up-to-date components suitable for use as staging, development or production environments, as well as for use on personal computers for development work.
- Consist of a public, tagged set of Docker containers.
- Include use of an orchestration tool such as Docker Composer.
- Allow for the replacement of older containers with newer versions.
- Be updated to include new component security patch releases within one calendar month.
- Support institution-specific customizations (XSLTs/Islandora Transforms, Solr, schemas, config.xml, MODS configurations/forms, XACML policies) able to persist through container upgrades and replacements.
    - These will be stored in a git repository and a mechanism will be provided to reintegrate with the installed Docker containers as part of an upgrade.
- Support for either institutional hosting or Amazon Web Services (AWS) for the Docker host server.
- Support data store flexibility on institutional hosting or an AWS cloud environment.
- Encompass critical security patches, and full stack software updates, plus dependencies for commonly installed features and all available solution packs.
- Include an integrated ISLE/Islandora test suite (expected to utilize TravisCI) to be utilized, developed in conjunction with the Islandora Foundation and the current Islandora release managers
- Include documentation that covers end usage, a how-to guide to set up an initial ISLE installation, a how-to guide for an established installation to upgrade containers to the latest versions, and a functional description of mechanics, administrative system management and the process for future ISLE development
