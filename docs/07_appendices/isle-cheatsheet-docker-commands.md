# ISLE Cheat Sheet: Docker commands

Docker commands that are useful to installing or updating ISLE.


## Docker Containers

### List Containers
  * Show currently running containers
    * `docker ps`
  * Show both currently running and all past containers
    * `docker ps -a`
  * Show names of all existing volumes
    * `docker volume ls`

### Stop Containers
  * Stop all current containers
    * `docker-compose stop`
  * Stop one or more named containers
    * usage: `docker stop [CONTAINER_NAME(S)]`
    * example: `docker stop isle-tomcat isle-solr`
  * Stop ALL containers (including old ones)
    * `docker stop $(docker ps -a -q)`

### Start Containers
  * Start all current containers
    * `docker-compose start`
  * Start ALL containers (including old ones)
    * `docker start $(docker ps -a -q)`

### Pull Containers
  * Pull down all images from Docker Hub
    * `docker-compose pull`
  * Pull one specific image from Docker Hub
    * usage: `docker pull [GROUP]/[REPO]`
    * example: `docker pull islandoracollabgroup/isle-fedora:latest`

### Up Containers
  * Launch all containers (creates volumes and starts containers)
    * `docker-compose up -d`

### Remove Containers
  * Remove All: Stop and remove all containers and all volumes
    * `docker-compose down -v`
  * WARNING! This will remove all stopped containers, all networks not used by a container, all images that lack an associated container, and all build cache.
    * `docker system prune --all`
  * Delete an (unused) volume
    * usage: `docker volume rm [VOLUME_NAME]`
    * example: `docker volume rm isle_fed-data`
  * Remove one or more (stopped) containers
    * usage: `docker rm [CONTAINER_NAME(S)]`
    * example: `docker rm isle_fed-data`
  * WARNING! This will remove ALL containers on the machine!
    * `docker rm $(docker ps -a -q)`

### Watching Docker Logs
  * Show the last 10 lines (`--tail 10`) of a container's logs; `-f` means continuous/live feed
    * usage: `docker logs -f --tail 10 [CONTAINER_NAME]`
    * example: `docker logs -f --tail 10 isle-apache-ld`

### Locate Docker Volume
  * usage: `docker volume inspect [VOLUME_NAME]`
  * example: `docker volume inspect isle_fed-data`
  * resultant display is a JSON array that contains a location like this:
    * "Mountpoint": "/var/lib/docker/volumes/isle_fed-data/_data"

### Port Lookups
  * Query what is using a specific port number
    * `docker ps | grep 8381`
  * Query what is using a specific port number
    * `lsof -i :8381`

### Shell into Docker Container
  * usage: `docker exec -it [CONTAINER_NAME] bash`
  * example: `docker exec -it isle-apache-ld bash`

### Shell into Docker Container and Open a File
  * usage: `docker exec -it [CONTAINER_NAME] [FILE_NAME]`
  * example: `docker exec -it isle-apache-ld bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh`


## Docker Service

### Show Docker Status
  * `sudo service docker status`
### Restart Docker Service
  * `sudo /bin/systemctl restart docker.service`
### Restart Docker Service (Alternate)
  * `sudo service docker restart`


## UNIX Commands

### Quickly View a File, Using Cat
  * usage: `cat [FILE_NAME]`
  * example: `cat /etc/hosts`

### Ping to Test Connectivity
  * usage: `ping [DOMAIN]`
  * example: `ping www.google.com`
  * example: `ping isle.localdomain`

### DNS Lookup and Querying Tool
  * usage: `dig [DOMAIN]`
  * example: `dig www.google.com`
  * example: `dig isle.localdomain`

### EXTREME CAUTION: Remove Directory: Recursively and with Force
  * usage: `sudo rm -rf [DIRECTORY_NAME]`
  * example: `sudo rm -rf /var/tmp/scrap/`


# *CAREFUL! BELOW THERE BE DRAGONS*


## How to Remove ALL Docker Containers and then Pull Down Fresh Images

1. Stop all Current Containers
    * `docker-compose stop`
1. WARNING! This will remove: all stopped containers; etc.
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

### Let's say you discover you have two older containers named `isle-web`, `isle-db`) and you want to remove them:
  * Example to stop these 2 containers
    * `docker stop isle-web isle-db`
  * Example to remove these 2 containers
    * `docker rm isle-web isle-db`
