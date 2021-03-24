<!--- PAGE_TITLE --->

# ISLE Cheat Sheet: Docker Commands

Docker commands that are useful to installing or updating ISLE.


## Docker Containers

### [List Containers](https://docs.docker.com/engine/reference/commandline/ps/)
  * `docker ps`     # Show all running containers
  * `docker ps -a`  # Show all running and stopped containers

### [List Volumes](https://docs.docker.com/engine/reference/commandline/volume_ls/)
* `docker volume ls`  # List all volumes

### [Start Containers](https://docs.docker.com/compose/reference/start/)
  * `docker-compose start` # Starts existing containers for a service
  * `docker start $(docker ps -a -q)` # Start all running and stopped containers

### [Stop Containers](https://docs.docker.com/compose/reference/stop/)
  * `docker-compose stop` # Stops running containers without removing them
  * `docker stop [CONTAINER_NAME(S)]` # Stop one or more named containers
    * **Example:** `docker stop isle-tomcat isle-solr`
  * `docker stop $(docker ps -a -q)` # **WARNING!** This will stop ALL containers running on the machine.

### [Stop and Remove Containers](https://docs.docker.com/compose/reference/down/)
  * `docker-compose down` # Stops containers and removes containers and networks for services defined in the Compose file

### [Pull Containers](https://docs.docker.com/compose/reference/pull/)
  * `docker-compose pull` # Pull down all images from Docker Hub
  * `docker pull [GROUP]/[REPOSITORY]` # Pull one specific image from Docker Hub
    * **Example:** `docker pull islandoracollabgroup/isle-fedora:latest`

### [Up Containers](https://docs.docker.com/compose/reference/up/)
  * `docker-compose up -d` # Launch all containers for this service

### [Remove Volumes](https://docs.docker.com/engine/reference/commandline/volume_rm/)
  * `docker volume rm [VOLUME_NAME]` # Remove one or more volumes. You cannot remove a volume that is in use by a container.
    * **Example:** `docker volume rm isle_fed-data`

### [Remove Containers](https://docs.docker.com/engine/reference/commandline/rm/)
  * `docker rm [CONTAINER_NAME(S)]` # Remove one or more containers
    * **Example:** `docker rm isle_fed-data`

### [Shell Into Docker Container](https://docs.docker.com/v17.12/engine/reference/commandline/exec/)
  * `docker exec -it [CONTAINER_NAME] bash` # Shell Into Docker Container
    * **Example:** `docker exec -it isle-apache-ld bash`
  * `docker exec -it [CONTAINER_NAME] [FILE_NAME]` # Shell Into Docker Container and Run Script
    * **Example:** `docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh`

### [Docker Service](https://docs.docker.com/engine/reference/commandline/docker/)
  * `sudo service docker status` # Show Docker Status
  * `sudo /bin/systemctl restart docker.service` # Restart Docker Service
  * `sudo service docker restart` # Restart Docker Service (alternate)

### [Watching Docker Logs](https://docs.docker.com/engine/reference/commandline/logs/)
  * `docker logs -f --tail 10 [CONTAINER_NAME]` # Show the last 10 lines (`--tail 10`) of a container's logs; `-f` means continuous/live feed
    * **Example:** `docker logs -f --tail 10 isle-apache-ld`

### [Locate Docker Volume](https://docs.docker.com/engine/reference/commandline/volume_inspect/)
  * `docker volume inspect [VOLUME_NAME]` # Locate Docker Volume
    * **Example:** `docker volume inspect isle_fed-data`
    * resultant display is a JSON array that contains a location like this:
        * `"Mountpoint": "/var/lib/docker/volumes/isle_fed-data/_data"`

### [Port Lookups](https://docs.docker.com/engine/reference/commandline/port/)
  * `docker port [CONTAINER_NAME]` # List port mappings or a specific mapping for the container
  * `docker ps | grep 8381` # Query what is using a specific port number
  * `lsof -i :8381` # Query what is using a specific port number

### [Docker Version](https://docs.docker.com/engine/reference/commandline/version/)
  * `docker version` # This will render all version information in an easy to read layout


## Docker: WARNING: BE CAREFUL WITH THESE COMMANDS

### [Delete All Containers, Networks, Volumes and Images](https://docs.docker.com/compose/reference/down/)
  * `docker-compose down -v` # **WARNING!** NEVER run this command in production. ONLY consider doing this on your Demo Local machine. This command will delete all Docker containers, networks, volumes, and images created by `up`. (This command will not delete your git repository or the paths or contents of your bind mounts.)
### [Remove Unused Data](https://docs.docker.com/engine/reference/commandline/system_prune/)
  * `docker system prune --all` # **WARNING!** Remove all unused containers, networks, images and volumes. (Translation: This will remove all stopped containers, all networks not used by a container, all images that lack an associated container, and all build cache.)
### [Remove All Containers](https://docs.docker.com/engine/reference/commandline/rm/)
  * `docker rm $(docker ps -a -q)` # **WARNING!** This will remove ALL containers on the machine!


## UNIX Commands

### Quickly View a File, Using `cat`
  * usage: `cat [FILE_NAME]`
  * **Example:** `cat /etc/hosts`

### Ping to Test Connectivity
  * usage: `ping [DOMAIN]`
  * **Example:** `ping www.google.com`
  * **Example:** `ping isle.localdomain`

### DNS Lookup and Querying Tool
  * usage: `dig [DOMAIN]`
  * **Example:** `dig www.google.com`
  * **Example:** `dig isle.localdomain`

### EXTREME CAUTION: Remove Directory: Recursively and with Force
  * usage: `sudo rm -rf [DIRECTORY_NAME]`
  * **Example:** `sudo rm -rf /var/tmp/scrap/`


# *CAREFUL! BELOW THERE BE DRAGONS*


## How to Remove All Docker Containers and Then Pull Down Fresh Images

1. Stop all current containers
    * `docker-compose stop`
1. **WARNING!** This will remove: all stopped containers;, etc.
    * `docker system prune --all`
1. Delete "data" folder in ISLE-Developer folder so logs are fresh and clean
    * `sudo rm -rf data`
1. Pull down the images from Docker Hub
    * `docker-compose pull`
1. Launch all containers (creates volumes and starts containers)
    * `docker-compose up -d`


## How Testers Can Maintain a Nice Clean Environment

### Show Currently Running and All Past Containers
  * `docker ps -a`

### Let's Say You Discover You Have Two Older Containers (named `isle-web`, `isle-db`) and You Want to Remove Them:
  * Example to stop these 2 containers
    * `docker stop isle-web isle-db`
  * Example to remove these 2 containers
    * `docker rm isle-web isle-db`
