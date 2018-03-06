## ISLE Basics


In simple terms, ISLE is a set of resources that allow one to build a fully functioning Islandora system fairly quickly using a system building tool called [Docker] (https://docker.com)

**Glossary of Terms**

* **Docker**: System virtualization software used to build ISLE - Docker is used to create virtual servers called containers based on pre-built images. A "recipe" file called docker-compose.yml orchestrates the setting up and networking of the containers.
* **Containers**: virtual "servers" created by Docker - each major component of Islandora runs in its own container.
* **Images**: source for the containers - these are built and updated by ISLE developers and stored on Dockerhub.
* **Dockerhub**: website/repository that provides access to the latest images for the ISLE containers.
* **ISLE on GitHub**: the ISLE repository on github.com contains documentation and configuration files necessary to build ISLE.
* **Host Server**: Also called "the host" - this is the base computer upon which the entire ISLE stack is built - this can be a virtual machine on a laptop (LOCAL), or a server you connected to via ssh (REMOTE).
* **Volume** a Docker controlled place to hold data on the local file system. Used to persist data across containers.
* **Network** refers to a defined Docker network that is controlled by docker. ThIS has powerful implications in production.
    * ISLE services: fedora, solr, apache, mysql, and proxy communicate using an internal private stack network. The service proxy also joins an insecure network that is accessible to the WAN (or for testing "WAN" likely means a smaller internal network). Why two networks? Swarms, scaling, replicating.
    
* **Variables go here...***
