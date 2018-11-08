# ISLE: Islandora Enterprise

### **Skip directly to the [Quick-Start Guide](#quick-start-guide)**

## ISLE Documents
* [ISLE Documentation](https://islandora-collaboration-group.github.io/ISLE-Documentation/) (user-friendly documentation)
* [ISLE Executive Summary](https://docs.google.com/document/d/17tAFxR6_b7sxXkE1teNDQZv0UZ0LLSkX8K05-U6A6nw/edit?usp=sharing) (project overview)

## How to Join
* [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody!  [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT
* [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support

## What is ISLE?
[ISLE](https://github.com/Islandora-Collaboration-Group/ISLE) is a community project—initially funded by [17 academic institutions](https://docs.google.com/document/d/1ycx5ATbeWpUWvpZ6bwXws490CMgi0dyB9SBfPYUDEjk/edit?usp=sharing) and Born-Digital—that addresses two of the most significant pain-points in Islandora: installation and maintenance.

ISLE uses replaceable Docker images to streamline and largely automate the process of installing and maintaining the entire Islandora 7.x stack, while at the same time enabling institutions to create customizations that persist separately from core code. The result is the ability to easily, quickly and regularly update an institution’s entire Islandora stack. Maintaining ISLE requires significantly less time and staff, and also reduces the dependency on expert technical staff and outside vendors.

Perhaps most importantly, the ISLE [project maintainers](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group#project-maintainers) keep your Islandora stack smoothly running with minimal effort by regularly releasing updated (and tested) Docker images that contain up-to-date Islandora releases, software patches, security updates and feature improvements.

ISLE is quite flexible and may be run on an institution’s servers or in the cloud, or as a hybrid; similarly, it may be maintained by an institution’s staff, by a vendor, or as a shared project.

Looking ahead: ISLE is collaborating with the community’s [LASIR](https://github.com/Islandora-Collaboration-Group/LASIR) project to develop and include a robust suite of Institutional Repository (IR) features within Islandora. ISLE’s common, updatable platform will be advantageous to the community as we prepare to move toward the next major version of Islandora CLAW.

## Why Use ISLE?
ISLE cuts in half (compared to pre-ISLE upkeep costs) the time required to install and maintain an Islandora stack. Clear documentation makes it a snap to test installation on a laptop, install it directly on a local host server or cloud service, or update a pre-existing ISLE site. ISLE standardizes best-practice security features and protocols, removing much of the opportunity for human error from configuration and upkeep of Production Islandora systems. Multiple stacks (production, staging, and development) may be installed on a single virtual server, and each may be distinctly configured. Initial use cases are showing ISLE to be dramatically faster (more on that soon).

Islandora is a powerful digital repository comprised of over 80 different, best-of-breed open-source software libraries. Islandora’s strength is that it integrates these packages into one easy to manage system. However, this ecosystem has traditionally been complex enough as to require staff with technical expertise to install and maintain.

ISLE lowers the barrier to entry for new institutions or organizations because it reduces the complexity and expense to the point where non-technical staff can upgrade an Islandora stack in just under an hour per month. This in turn allows institutions to reallocate funds towards development or ingestion instead of maintenance. An ISLE maintained Islandora stack is secure and can be run as a production server straight out of the box.

A note about migration: ISLE contains complete documentation for migrating existing Islandora installations to the ISLE framework. Institutions currently using CONTENTdm, Digital Commons, or other platforms are able to migrate to ISLE by using the well-documented [Move to Islandora Kit](https://github.com/MarcusBarnes/mik) (MIK). It is assumed that some institutions will have custom theming, customizations and metadata formats that, while they are preserved during ISLE updates, will need to be manually refactored during the migration to CLAW.

## Funding
- **ISLE Phase I:** $90,000 crowdfunded ($66,000 from 17 academic institutions; $24,000 in-kind labor from Born-Digital); exceeded original goal of $84,000.
- **ISLE Phase II:** The Andrew W. Mellon Foundation has awarded a [$153,000 grant](https://communications.williams.edu/news-releases/10_30_2018_islandora/) to the Islandora Collaboration Group (ICG) to create “_Islandora for All:_ Extending the Open-Source Platform to Include Broad Scale Institutional Repository Appeal and High-Performance Scalability”
- **ISLE Phase III:** TBD

## Project Sustainability
The ISLE Steering Committee (ISC) oversees the ISLE project and the ISLE Project Maintainers maintain the project with scheduled new releases on [GitHub project boards](https://github.com/Islandora-Collaboration-Group/ISLE/projects), and issue [release notes](https://islandora-collaboration-group.github.io/ISLE-Documentation/09_release_notes/release_notes/). The ISC will submit the resultant development via Islandora’s Licensed Software Acceptance Procedure ([LSAP](https://islandora.ca/developers/lsap)). The development will then become a core part of the Islandora 7x Release (Fall 2019) and will be supported and maintained by the Islandora community as part of the free and publicly shared open-source platform. Through this LSAP method, Islandora has already adopted a large number of externally contributed modules that are sustainably maintained by community members.

## ISLE Steering Committee
- Sarah Goldstein, Mount Holyoke College
- David Keiser-Clark (Chair), Williams College
- Francesca Livermore, Wesleyan University
- Mark McFate, Grinnell College
- Ben Rosner, Barnard College
- Mark Sandford, Colgate University

## ISLE Project Maintainers
- Ben Rosner (Lead), Barnard College
- Mark Sandford (Lead), Colgate University
- David Keiser-Clark, Williams College
- Francesca Livermore, Wesleyan University
- Shaun Trujillo, Mount Holyoke College

## Former Contributors
- Carolyn Moritz, Vassar College
- Steve Young, Hamilton College

## ISLE Funding Institutions
* Amherst College
* Barnard College
* California Institute of Technology
* Colgate University
* Grinnell College
* Hamilton College
* Hampshire College
* Mount Holyoke College
* Rensselaer Polytechnic Institute
* Smith College
* St. Lawrence University
* Tulane University
* University of Manitoba Libraries
* University of Pittsburgh
* Vassar College
* Wesleyan University
* Williams College
* Born-Digital
* Islandora Foundation (Board supports ISLE; not a funder)

## Timeline
- 08/2016 - [Completed] ISLE Phase I conceived with the ICG and Common Media/Born-Digital
- 09/2016 - [Completed] ISLE presented at ICG Hack/Doc @ Wesleyan University
- 05/2017 - [Completed] ISLE presented at Islandoracon 2017 annual conference (Hamilton, Ontario)
- 09/2017 - [Completed] $90,000 crowdfunded from 17 academic institutions to fund ISLE Phase I
- 09/2017 - [Completed] ISLE MVP: Opened 3-week Public Comment Period
- 12/2017 - [Completed] Public prototype demonstration: [view 5-minute video](https://vimeo.com/245777329)
- 12/2017 - [Completed] Alpha Test #1: Grinnell College (lead: Mark McFate)
- 04/2018 - [Completed] Alpha Test #2: Williams College (lead: David Keiser-Clark)
- 03/2018 - [Completed] Alpha Test #3: Rensselaer Polytechnic Institute (lead: Andrea Byrne)
- 03/2018 - [In Process] Community Alpha Test #1: University of Pittsburgh (lead: Clinton Graham)
- 03/2018 - [Completed] Community Alpha Test #2: Hamilton College (lead: Steve Young)
- 03/2018 - [Completed] Community Alpha Test #3: Barnard College (lead: Ben Rosner)
- 05/2018 - [In Process] ISLE Steering Committee approved ISLE (Islandora 7x) hand-off from Born-Digital
- 05/2018 - [Completed] ISLE v1.0 release available as open-source for public use
- 08/2018 - [Completed, in UAC] ISLE v1.1 release candidate available as open-source for public use
- 10/2018 - [Future] ISLE Phase II begins

## Docker Images and Repositories
Docker Image GitHub Repositories that comprise this stack: 
 - [`isle-ubuntu-basebox`](https://github.com/Islandora-Collaboration-Group/isle-ubuntu-basebox) from which all images are derived.
 - [`isle-tomcat`](https://github.com/Islandora-Collaboration-Group/isle-tomcat)
 - [`isle-fedora`](https://github.com/Islandora-Collaboration-Group/isle-fedora/)
 - [`isle-solr`](https://github.com/Islandora-Collaboration-Group/isle-solr/)
 - [`isle-apache`](https://github.com/Islandora-Collaboration-Group/isle-apache/)
 - [`isle-mysql`](https://github.com/Islandora-Collaboration-Group/isle-mysql)
 - [`isle-imageservices`](https://github.com/Islandora-Collaboration-Group/isle-imageservices/)
 - [`traefik` as Proxy](https://traefik.io/)

## Quick Start Guide

### Requirements  
* Docker-CE or EE
* Docker-compose
* Git
* Time required < 30 minutes.
* **Windows Users**: Please open the .env and uncomment `COMPOSE_CONVERT_WINDOWS_PATHS=1`

### Quick Start
1. Please read: [ISLE Release Candidate (RC): How to Test](https://docs.google.com/document/d/1VUiI_bXo6SLqqUjmInVjBg3-cs40Vj7I_92txjFUoQg/edit#heading=h.1e4943m60lsh)
2. Clone this repo
    - `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git` 
3. Change directory to the cloned directory:
    - `cd ISLE` (by default)
4. Pull the latest images:
    - `docker-compose pull`
5. Launch the ISLE stack:
    - `docker-compose up -d`
6. Please wait a few moments for the stack to fully come up.  Approximately 3-5 minutes.
7. Install Islandora on the isle-apache-ld container:
    - `docker exec -it isle-apache-ld bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh`
8. To wrap up testing:
    - In the folder with the docker-compose.yml `docker-compose down -v` (nb: the -v removes all volumes, and will delete any work. This option **does not persist your data**)

### Quick Stop and Cleanup 
If you have been testing the stack extensively you may want to `prune` your Docker daemon as you test.
1. In the folder with the `docker-compose.yml`
    - `docker-compose down -v`
- If you would like to *completely clean your docker-daemon*:
2. If you have no other _stopped_ services that you do not want `pruned` on Docker:
    - **Note running containers are NOT pruned.**
    - `docker system prune --all`
    - answer `Y` to remove all unused volumes, images, and networks.
- OR
2. If you cannot `prune`:
    - `docker ps` and take note of any running ISLE services:
        - `docker down {list of all the running ISLE services, tab auto-complete may work}` (You may add as many containers as needed in one command.)
    - `docker image ls` and take note of all ISLE-related images:
        - `docker image rm {list of all images to be removed, tab auto-complete may work}` (Again, you may add as many as needed.)
    - `docker volume ls` and take note of all ISLE-related volumes:
        - `docker volume rm {list of all volumes to be removed, tab auto-complete may work}` (Again, you may add as many as needed.)
    - `docker network ls` and take note of all ISLE-related networks:
        - `docker network rm {list of all networks to be removed, tab auto-complete may work}` (Again, you may add as many as needed.)


### Important Notes, Ports, Pages and Usernames/Passwords
[Portainer](https://portainer.io/) is a GUI for managing Docker containers. It has been built into ISLE for your convenience.  
**Windows Users**: Please open the .env and uncomment `COMPOSE_CONVERT_WINDOWS_PATHS=1`  
**Note that both HTTP and HTTPS work** Please accept the self-signed certificate for testing when using HTTPS.

#### Locations, Ports:
* Make sure your /etc/hosts points isle.localdomain to 127.0.0.1. See original docs on how-to.
* Islandora is available at http://isle.localdomain
  * **You may need to point directly to the IP address of isle-apache, here's how:**
    - `docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' isle-apache-ld`
    - Copy the IP and browse to it.  `http://{IP}/`
* Traefik is available at http://admin.isle.localdomain OR http://localhost:8080/
* Portainer is available at http://portainer.isle.localdomain OR http://localhost:9010/
* Fedora is available at http://isle.localdomain/fedora OR http://localhost:8081/
* Solr is available at http://isle.localdomain/solr OR http://localhost:8082/
* Image Services are available at http://images.isle.localdomain OR http://localhost:8083/

#### Users and Passwords
Read as username:password

Islandora (Drupal) user and pass (default):
 * `isle`:`isle`

All Tomcat services come with the default users and passwords:
* `admin`:`isle_admin`
* `manager`:`isle_manager`

Portainer's authentication can be configured: 
* By default there is no username or password required to login to Portainer.
* [Portainer Configuration](https://portainer.readthedocs.io/en/stable/configuration.html)
