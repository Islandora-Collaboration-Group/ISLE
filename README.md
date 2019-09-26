# ISLE: Islandora Enterprise

## Introduction
ISLE uses replaceable Docker images to streamline and largely automate the process of installing and maintaining the entire Islandora 7.x stack, while at the same time enabling institutions to create customizations that persist separately from core code. The result is the ability to easily, quickly and regularly update an institution’s entire Islandora stack. Maintaining ISLE requires significantly less time and staff, and also reduces the dependency on expert technical staff and outside vendors.

The ISLE [project maintainers](#maintainers) keep your Islandora stack smoothly running with minimal effort by regularly releasing updated (and tested) Docker images that contain up-to-date Islandora releases, software patches, security updates and feature improvements.

ISLE is quite flexible and may be run on an institution’s servers or in the cloud, or as a hybrid; similarly, it may be maintained by an institution’s staff, by a vendor, or as a shared project.

## Documentation
Please use the [ISLE Documentation](https://islandora-collaboration-group.github.io/ISLE) for using ISLE to install Islandora on server environments.

## Quick Start Guide
**Important:** This "Quick Start Guide" is for testing ISLE and **should not** be used in production.

### Requirements  
* Docker-CE or EE version `19.03.x`+
* [Docker-compose](https://docs.docker.com/compose/install/) version `1.24.1`+
* Git `2.0+`
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
    * **Please note:** the “ -d” argument stands for “detached” meaning the command will persist even if you close your terminal or your computer sleeps etc…)
6. Please wait a few moments for the stack to fully come up.  Approximately 3-5 minutes.
7. Install Islandora on the isle-apache-ld container:
    - `docker exec -it isle-apache-ld bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh`
8. To wrap up testing:
    - In the folder with the docker-compose.yml `docker-compose down -v` (nb: the -v removes all volumes, and will delete any work. This option **does not persist your data**)

### Quick Stop and Cleanup 
If you have been testing the stack extensively you may want to `prune` your Docker daemon as you test.
1. In the folder with the `docker-compose.demo.yml`
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
* Make sure your /etc/hosts points isle.localdomain to 127.0.0.1. See original docs on [how-to](docs/install/install-demo-edit-hosts-file.md)
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

## Maintainers
* Bethany Seeger (Lead), Amherst College
* Mark Sandford (Lead), Colgate University
* Francesca Baird, Wesleyan University
* David Keiser-Clark, Williams College
* Gavin Morris, Born-Digital
* Shaun Trujillo, Mount Holyoke College

## Former Contributors
* Carolyn Moritz, Vassar College
* Benjamin Rosner (Lead Maintainer 2018-19), Barnard College
* Steve Young, Hamilton College

## Contributing to ISLE
* [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody! [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT
* [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support$