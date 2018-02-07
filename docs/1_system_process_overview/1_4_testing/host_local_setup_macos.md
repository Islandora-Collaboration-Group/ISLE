### Host Local Setup Guide - MacOS (Docker For Mac)

This is a guide to setup and install all ISLE pre-requisites on a MacOS local laptop or workstation.

**Please note:** This guide is for using the `Docker For Mac` software only.

If an enduser would prefer not to use `Docker For Mac` and instead use a local VirtualMachine (aka Virtualbox) please refer to one of the following guides:

* [Host Local - Ansible Setup Guide](host_local_setup_ansible.md)
* [Host Local - CentOS Setup Guide](host_local_setup_centos.md)
* [Host Local - Ubuntu Setup Guide](host_local_setup_ubuntu.md)

### Assumptions / Pre-Requisites

* Enduser has a local laptop / workstation that conforms to the specifications outlined in the [ISLE MVP Host Specifications Guide](../mvpspecs.md)


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

### Step 2: Docker Installation

* Click this [link](https://download.docker.com/mac/stable/Docker.dmg) to download the latest version (`Docker.dmg`)
  * _This .dmg file will include Docker & Docker Compose in one package._

  * To read more about this version of Docker, please visit the [Docker store](https://store.docker.com/editions/community/docker-ce-desktop-mac)

* The `Docker.dmg` file download process will start and depending on the enduser's Internet connection will take between 5 - 15 mins. This file once downloaded will be found in the endusers `Downloads` folder.

* Double-click the `Docker.dmg` file in the `Downloads` folder.

* A `Docker` install prompt will appear. Within the prompt, simply drag and drop the `Docker` (whale) icon to the Applications folder.

* A `Copying to Applications` prompt will appear and depending on the enduser's harddisk speed may take between 1 - 5 mins.

* Open a new `Finder` window and click on the `^` symbol to eject the `Docker` install mount. This will close the previous `Docker` install prompt.

* To start `Docker for Mac`, navigate to the `Applications` directory and double click on the `Docker` whale icon.

* The enduser may get a prompt like this:

```
"Docker” is an application downloaded from the Internet.
Are you sure you want to open it?

Chrome downloaded this file today at 3:43 PM from download.docker.com.
```
* Click the `Open` button on the prompt.

* The `Docker for Mac` application should now start up.
   * One can check on the process by looking at the top of the screen, there should be a tiny whale icon with tiny cargo containers bouncing about. If one clicks on the dropdown menu, the orange circle status of `Docker is starting...` should turn to a green circle status of `Docker is running` if installed properly.

* To stop `Docker for Mac`, simply click again on the whale icon at the top of the screen and choose `Quit Docker` or use the ⌘ key and Q.


### Step 3: Clone the ISLE repository

In this section, the enduser will need to decide on an appropriate area on their laptop to clone the current ISLE project software to.

Some endusers create a new directory e.g. `Projects` or `Code` within their `Documents` directory. It is entirely up to the enduser to decide.

The built-in terminal for MacOs can be found in `~/Applications/Utilities/Terminal`

* Open a terminal

* `cd ~/Documents`

* `git clone https://github.com/Islandora-Collaboration-Group/ISLE`

   * _This process will take 3 - 5 minutes depending on internet bandwidth_

* A new directory `ISLE` should now be visible. The enduser can check by either:

  * using the `Finder` application to navigate to this directory

  * Or within the same terminal, enter `ls -lha`

#### Next steps
Once this process has finished one chose to do one of the following:

* Continue next steps with the [Testsite Guide](testsite_guide.md)

* Continue next steps with the [Newsite Guide](newsite_guide.md)

* Continue next steps with the [Migration Guide](migration_guide.md)

* Continue next steps with the [Developer Build Guide for Linux](dev_build_guide_linux.md)

* Continue next steps with the [Developer Build Guide for MacOS](dev_build_guide_mac.md)
---
