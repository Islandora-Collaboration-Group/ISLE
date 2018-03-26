**MacOS** server version to use: `10.3.x+`

This is a detailed operating system-specific guide to setup and install all ISLE pre-requisites on a Windows 10 local laptop or workstation using Docker for Mac.

Called "the host" - this is the base computer upon which the entire ISLE stack is built.

**Please note:** This guide is for using `Docker for Windows` software only on a MacOS 10.3.3 or higher system instead of Vagrant or a Virtual Machine (VM).

### Assumptions / Pre-Requisites

* Enduser has a local laptop / workstation that conforms to the specifications outlined in the [Host Server Specifications](01_installation_host_server/host_server_system_specifications.md)

* Enduser **must** have administrator rights on the MacOS computer to proceed or the installation will fail.

### Install process overview

* Install git
* Install Docker for Mac
* Clone the ISLE project repository
* Pull down the ISLE images from Dockerhub
* Install the ISLE project and software dependencies using the Ubuntu (manual) or Ansible (scripted) guides
* Install and run the ISLE project on the ISLE Host VM using the Testsite Guide

### Step 1: Git Installation
In order to get a copy (clone) of the current ISLE project, git will need to be installed. [Git](https://git-scm.com) is a software version control system for tracking changes in computer files and coordinating work on those files among multiple people.

 * Open a terminal and enter: `git --version`.

   * This command will inform the enduser if git is already installed.  

```
$git --version
git version 2.15.1
```

  * If git is not installed, running that previous command may trigger the `Install Command Line Developer Tools` prompt. If the prompt appears:
       * Click on the blue `Install` button for the license agreement.
       * Click the white `Agree` button.
       * The package will take 1-2 minutes to download.
       * Click the `Done` button once finished.

  * If git is not installed and the prompt doesn't show, then follow one of the recommended methods for installing git in this nice [tutorial](https://www.atlassian.com/git/tutorials/install-git)

If git is already installed, then please proceed to the next section.

### Step 2: Docker for Mac Installation

* Open a browser and navigate to https://www.docker.com/docker-mac

* Click the `Download from Docker Store` button in the center of the page

* Click the blue `Get Docker` button on the right of the page

* The `Docker.dmg` file should start to download. Check your `Downloads` directory

* Double-click the `Docker.dmg` file. The file should open and mount in a new window / prompt.

* As instructed within the prompt, drag and drop the whale icon to the right towards the `Applications` directory shortcut, a tiny green plus sign should appear, now let go from the mouse or trackpad.

* The application should start to copy data to the `Applications` directory, this process may take 1 - 5 mins depending on the speed of your hard-drive.

* Launch the `Docker` application from the `Applications` directory

* This process should may take 2 -5 mins depending on the speed of your hard-drive.

* Once fully started, one can see a whale icon at the top of their screen. If this is clicked, a dropdown should appear indicating that Docker is now running.
  * Please note: This process also installs the newest version of `Docker-Compose`.

* (Optional) - Open a terminal and enter `docker ps -a`, this should return the following:

```

CONTAINER     ID    IMAGE     COMMAND     CREATED     STATUS    PORTS     NAMES

```

### Step 3: Clone the ISLE repository

In this section, the enduser will need to decide on an appropriate area on their laptop to clone the current ISLE project software to.

Some endusers create a new directory e.g. `Projects` or `Code` within their `Documents` directory. It is entirely up to the enduser to decide. The steps below are an example.

The built-in terminal for MacOs can be found in `~/Applications/Utilities/Terminal`

* Open a terminal

* `cd ~/Documents`

* `git clone https://github.com/Islandora-Collaboration-Group/ISLE`

   * _This process will take 3 - 5 minutes depending on internet bandwidth_

* A new directory `ISLE` should now be visible. The enduser can check by either:

  * using the `Finder` application to navigate to this directory


      * Or within the same terminal, enter `ls`  and verifying that `ISLE` appears in the list.

---

### Step 4: Pull down the ISLE images from Dockerhub

This process should may take 15 - 60+ mins depending on the speed of your Internet connection.

* Open a terminal and enter the following commands, one at a time:

* `docker pull islandoracollabgroup/isle-apache:latest`
* `docker pull islandoracollabgroup/isle-fedora:latest`
* `docker pull islandoracollabgroup/isle-mysql:latest`
* `docker pull islandoracollabgroup/isle-proxy:latest`
* `docker pull islandoracollabgroup/isle-solr:latest`


### Next steps

* Return to the [Install Start Here section](../install_start_here.md) for additional instructions

---
