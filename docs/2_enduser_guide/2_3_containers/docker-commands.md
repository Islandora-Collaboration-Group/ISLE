## Docker image commands

### How to list Docker images

To list Docker images: `docker image ls`

### How to remove Docker images

To remove a Docker container: `docker image rm -f <image name>`

---

## Docker container commands

### How to list Docker containers

To check if containers are running in Docker:

* Open up a command line terminal on the host machine and enter: `docker ps -a`

Output should display like:

**Example:**  

(use screenshot?)

### How to start or stop Docker containers using Docker-Compose

 To execute the next two commands, you'll need to `cd` into the root of project directory where the `docker-compose.yml` file is within a command line terminal.

* To stop all containers at once: `docker-compose stop`

* To start all containers at once: (once built) `docker-compose up`

---

### How to access running Docker containers
To shell into a container properly without shutting it down:

**Example:**  
`docker exec -it isle-islandora-web bash`

In some cases, one may need to use `Cntrl-D` to exit the container without shutting it down.

---

### How to remove Docker containers

To remove a Docker container: `docker rm -f <container name>`
