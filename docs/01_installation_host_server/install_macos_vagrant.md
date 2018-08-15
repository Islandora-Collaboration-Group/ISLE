**MacOS** desktop version to use: `10.13.x+`

This is a detailed operating system-specific guide to setup and install all ISLE pre-requisites on a MacOS local laptop or workstation using Vagrant and VirtualBox.

**Please note:** This guide is for using `Vagrant / Virtualbox` software only on a MacOS system instead of `Docker for Mac`.

### Assumptions / Pre-Requisites

* Enduser has a local laptop / workstation that conforms to the specifications outlined in the [Host Server Specifications](host_server_system_specifications.md)

* Enduser **must** have administrator rights on the MacOS computer to proceed or the installation will fail.

* This guide is not intended for production usage.

### Install process overview

* Install git
* Install VirtualBox
* Install VirtualBox Extensions
* Install Vagrant
* Install Vagrant plugin
* Clone the ISLE project repository
* Start the ISLE Host VM (using vagrant)
* Next steps

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

  * If git is not installed and the prompt doesn't show, then follow one of the recommended methods for installing git in this nice [tutorial](https://www.atlassian.com/git/tutorials/install-git).

If git is already installed, then please proceed to the next section.

### Step 2: VirtualBox Installation

VirtualBox is a general-purpose full virtualizer for x86 hardware, targeted at server, desktop and embedded use.

* Go to [https://www.virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads) to download and install the latest platform package for OS X hosts. 


### Step 3: VirtualBox Extensions for VirtualBox Installation

* Return to [https://www.virtualbox.org/wiki/Downloads](https://www.virtualbox.org/wiki/Downloads)  to download the latest version. Click on the Oracle VM VirtualBox Extension Pack `All supported platforms` link to start.

* Once the `Oracle_VM_VirtualBox_Extension_Pack-x.x.xxx.vbox-extpack` download has completed, open the download folder and double click on the file.

* Follow the prompts to install the Extension Pack.

### Step 4: Vagrant Installation

Vagrant is a tool for building and managing virtual machine environments in a single workflow. Endusers can read more [here](https://www.vagrantup.com/intro/index.html)

* Go to [https://www.vagrantup.com/downloads.html](https://www.vagrantup.com/downloads.html) to download and install the latest version for Mac OS X. 


### Step 5: Vagrant Plugin Installation

* Open a terminal and enter the following:

   * `vagrant plugin install vagrant-hostsupdater`

   * This process should take 10 - 30 seconds depending on Internet speed

   **Example**

```
vagrant plugin install vagrant-hostsupdater

Installing the 'vagrant-hostsupdater' plugin. This can take a few minutes...

Installed the plugin 'vagrant-hostsupdater (1.0.2)'!
```   


### Step 6: Clone the ISLE repository

In this section, the enduser will need to decide on an appropriate area on their laptop to clone the current ISLE project software to.

Some endusers create a new directory e.g. `Projects` or `Code` within their `Documents` directory. It is entirely up to the enduser to decide. The steps below are an example and will install ISLE in `~/Documents/ISLE`.

The built-in terminal for MacOs can be found in `~/Applications/Utilities/Terminal`

* Open a terminal

* `cd ~/Documents`

* `git clone https://github.com/Islandora-Collaboration-Group/ISLE`

   * _This process will take 3 - 5 minutes depending on internet bandwidth_

* A new directory `ISLE` should now be visible. The enduser can check by either:

  * using the `Finder` application to navigate to this directory

  * Or within the same terminal, enter `ls -lha`

### Step 7: Start the ISLE Host Vagrant VM and ssh in

Endusers have a choice of using a CentOS or Ubuntu Vagrant VM.

Within the `ISLE/vagrant` directory, there are two child directories:

* CentOS - contains a Vagrantfile configured for a CentOS 7.x+ server with the IP of `10.10.10.30`

* Ubuntu - contains a Vagrantfile configured for a CentOS 7.x+ server with the IP of `10.10.10.30`

* Open a terminal and enter the following:

   * `cd ~/path_to/ISLE/vagrant/`
   * `vagrant up`

This process should start the ISLE Host Vagrant VM and should take anywhere from 10 - 20 minutes for the software ([Vagrant box](https://www.vagrantup.com/intro/getting-started/boxes.html)) to download and start.

Once this process finishes and the ISLE Host Vagrant VM is running.

* The enduser should enter in the terminal:

    * `vagrant ssh`

This command shells in the vagrant user to the ISLE Host.


### Next steps

* Return to the [Install Start Here section](../install_start_here.md) for additional instructions.
