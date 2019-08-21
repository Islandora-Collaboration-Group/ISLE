<!--- PAGE_TITLE --->

[Islandora Enterprise (ISLE)](https://github.com/Islandora-Collaboration-Group/ISLE) is a new project in development that will address two of the most significant pain-points in Islandora: installation and maintenance.

[Islandora](http://islandora.ca/) is an open-source software [framework](https://github.com/Islandora) designed to help institutions and organizations and their audiences collaboratively manage, and discover digital assets using a best-practices framework.

ISLE is an orchestrated group of Docker containers to be run on a server, personal computer or cloud instance with a minimal suite of tools enabling an efficient deploy and release of an updated production-ready Islandora environment.

ISLE functions as both an initial installer and maintenance updater with the ability to build new production, staging, or development environments as distributable, lightweight and fast “containers” in a [Docker Engine](https://docs.docker.com/engine/) environment.

This process allows folks to choose between hosting:

* locally on a personal computer for development work
* on site in an institutional data center
* in a cloud environment such as [Amazon Web Services](https://aws.amazon.com/) or [Google Cloud Platform](https://cloud.google.com/).

ISLE’s production-ready platform will be updated monthly with:

* critical security patches
* full stack software updates
* a suite of commonly installed features and solution packs.

ISLE is designed to be an evolving but trusted environment to help create a managed process for migrating to the next major release of Islandora (termed CLAW).


## Project Overview

ISLE public repository: [https://github.com/Islandora-Collaboration-Group/ISLE](https://github.com/Islandora-Collaboration-Group/ISLE)

## How Does It Work?
ISLE separates an institution’s customizations from core code, and moves that core code into containers that are easily updated, simplifying and largely automating the process of installation and updates of Islandora. ISLE also bundles together the best shared modules into a common, production-ready and security-hardened platform.

## Why Is It Needed?
Islandora is a powerful digital repository comprised of more than 80 different open-source software libraries. This complex ecosystem makes Islandora difficult and expensive to install, maintain and customize. ISLE lowers the barrier to entry for new schools while allowing existing institutions to reallocate funds towards development or ingestion instead of maintenance.

## High Level Objective
To deliver a secure and production-ready Islandora stack (“ISLE”) that reduces the involved support time and effort for installation, maintenance and customization.

## ISLE Functional Definition
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

## ISLE Documents
* [ISLE Executive Summary](https://docs.google.com/document/d/17tAFxR6_b7sxXkE1teNDQZv0UZ0LLSkX8K05-U6A6nw/edit?usp=sharing) (a concise overview and history of the ISLE project)
* [ISLE Specifications](https://docs.google.com/document/d/1iTXYbBMtQ3TaujPXon01Hp6hVwnxYvsVXYa2G79SuWc/edit#) (based on the older, now closed, [ISLE MVP](https://docs.google.com/document/d/1s_qWkRgHlRAH6SWuXid6dOYzBjcbqU6PV_gZ1sUu2iY/edit?usp=sharing) doc)
* [ISLE Supporters](https://docs.google.com/document/d/1ycx5ATbeWpUWvpZ6bwXws490CMgi0dyB9SBfPYUDEjk/edit?usp=sharing) (list of institutions)
