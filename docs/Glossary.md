## ISLE Basics

In simple terms, ISLE is a set of resources that allow one to build a fully functioning Islandora system fairly quickly using a system building tool called [Docker](https://docker.com)

**Glossary of Terms**

### Systems

* **Root User** - the user name or account that by default has access to all commands and files on a Linux or other Unix-like operating system. It is also referred to as the root account, root, root user and the superuser. [Learn more about root user](http://www.linfo.org/root.html)

* **Sudo** - short for "super user do" and it allows the user to have root permissions when entered in front of a command. [Learn more about sudo](https://www.sudo.ws/intro.html)

* **SSH** - is a cryptographic network protocol for operating network services securely over an unsecured network. The best known example application is for remote login to computer systems by users. [Learn more about SSH](https://en.wikipedia.org/wiki/Secure_Shell)

### Virtualization

* **VM**: Virtual Machine - A virtual machine is a software computer that, similar to a physical computer, runs an operating system and applications comprised of specification and configuration files backed by the resources of a host (the physical computer).

* **Virtualbox**: [VirtualBox](https://www.virtualbox.org/wiki/Downloads) is a general-purpose full virtualizer for x86 hardware, targeted at server, desktop and embedded use.

* **Vagrant**: [Vagrant](https://www.vagrantup.com/) provides easy to configure, reproducible, and portable work environments. Vagrant works on Mac, Linux, Windows, and more. Within the ISLE project there is a vagrant folder.

* **Vagrantfile**: [Vagrantfile](https://www.vagrantup.com/docs/vagrantfile/) used to describe the type of virtual machine required for the ISLE project, and how to configure and provision these virtual  machines. Vagrantfiles are called Vagrantfiles because the actual literal filename for the file is Vagrantfile. Vagrant is meant to run with one Vagrantfile per project which allows endusers to check out the code, run vagrant up, and be on their way. Vagrantfiles are used primarily for use on local workstations and laptops running a Ubuntu or CentOS ISLE Host server.

* **Docker for Mac**: [Docker for Mac](https://www.docker.com/docker-mac) is an easy-to-install desktop app for building, debugging and testing Dockerized apps on a Mac. Docker for Mac is a complete development environment deeply integrated with the MacOS Hypervisor framework, networking and filesystem

---

### Docker

* **Docker**:  used to build ISLE - [Docker](https://www.docker.com/what-docker) is used to create containers (similar to virtual servers) based on pre-built images. A "recipe" file called docker-compose.yml orchestrates the setting up and networking of the containers.

* **Containers**: [Docker containers](https://www.docker.com/what-container) are lightweight, stand-alone, executable packages of a piece of software that includes everything needed to run it: code, runtime, system tools, system libraries, settings. Each major component of Islandora runs in its own container.

* **Images**: [Docker images](https://docs.docker.com/engine/reference/commandline/images/) - source for the containers - these are built and updated by ISLE developers and stored on Dockerhub.

* **Dockerhub**: [website / repository](https://hub.docker.com/website/repository) that provides access to the latest Docker images for the ISLE containers.

* **ISLE on GitHub**: the ISLE repository on github.com contains documentation and configuration files necessary to build ISLE.

* **Host Server**: Also called "the host" - this is the base computer upon which the entire ISLE stack is built - this can be a virtual machine on a laptop (LOCAL), or
a server you connected to via ssh (REMOTE).

* **Volume** a Docker controlled place to hold data on the local file system. Used to persist data across containers.

* **Network** refers to a defined Docker network that is controlled by docker. This has powerful implications in production.
    * ISLE services: `fedora`, `solr`, `apache`, `mysql`, and `proxy` communicate using an internal private stack network. The service proxy also joins an insecure network that is accessible to the WAN (or for testing "WAN" likely means a smaller internal network). Why two networks? Swarms, scaling, replicating.

* **Variables go here...***

---

### Documentation / Development

* **Markdown** is a lightweight markup language with plain text formatting syntax. Markdown allows you to write using an easy-to-read, easy-to-write plain text format, then convert it to structurally valid XHTML (or HTML).
    * [https://en.wikipedia.org/wiki/Markdown](https://en.wikipedia.org/wiki/Markdown)
    * [https://daringfireball.net/projects/markdown/syntax](https://daringfireball.net/projects/markdown/syntax)
    * [http://kirkstrobeck.github.io/whatismarkdown.com/](http://kirkstrobeck.github.io/whatismarkdown.com/)
    * [https://help.github.com/categories/writing-on-github/](https://help.github.com/categories/writing-on-github/)

* **YAML** (YAML Ain't Markup Language) is a human-readable data serialization language. It is commonly used for configuration files, but could be used in many applications where data is being stored (e.g. debugging output) or transmitted (e.g. document headers).
    * [https://en.wikipedia.org/wiki/YAML](https://en.wikipedia.org/wiki/YAML)
    * [http://yaml.org/](http://yaml.org/)

* **CLI / cli** - A command-line interface or command language interpreter (CLI), also known as command-line user interface, console user interface[1] and character user interface (CUI), is a means of interacting with a computer program where the user (or client) issues commands to the program in the form of successive lines of text (command lines). A program which handles the interface is called a command language interpreter or shell.

* **Git** - A free and open source distributed software version control system designed to handle everything from small to very large projects with speed and efficiency. Git is easy to learn and has a tiny footprint with lightning fast performance.
    * [https://git-scm.com/](https://git-scm.com/)
    * The ISLE project and its documentation is hosted by an online git service called [Github.com](https://github.com/)
