### Host Local Setup Guide - MacOS (Vagrant / Virtualbox)

This is a guide to setup and install all ISLE pre-requisites on a MacOS local laptop or workstation.

**Please note:** This guide is for using `Vagrant / Virtualbox` software only.

**2/7/2018** There appears to be a networking bug found in `Docker For Mac` software that stops Fedoragsearch from properly starting in Tomcat.

### Assumptions / Pre-Requisites

* Enduser has a local laptop / workstation that conforms to the specifications outlined in the [ISLE MVP Host Specifications Guide](../mvpspecs.md)

### Install process overview

* Install git
* Install VirtualBox
* Install VirtualBox Extensions
* Install Vagrant
* Install Vagrant plugin
* Clone the ISLE project repository
* Start the ISLE Host VM (using vagrant)
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

### Step 2: VirtualBox Installation

VirtualBox is a general-purpose full virtualizer for x86 hardware, targeted at server, desktop and embedded use.

* In a web browser, navigate to https://www.virtualbox.org/wiki/Downloads to download the latest version. Click on the `OS X hosts` link to start.

* The `VirtualBox-x.x.xxx.dmg` file download process will start and depending on the enduser's Internet connection will take between 5 - 15 mins. This file once downloaded will be found in the endusers `Downloads` folder.

* Double-click the `VirtualBox-x.x.xxx.dmg` file in the `Downloads` folder.

* A `VirtualBox` install prompt will appear. Within the prompt, Double click on the `VirtualBox.pkg` icon as also instructed in the prompt.

* A `Install Oracle VM VirtualBox` prompt will appear click on the `Continue` button.

* The `Standard Install on...` window appears, click on the `Install` button at the lower right hand corner.

* The `Installer is trying to install new software` prompt appears. The enduser's User Name should already be filled out in the top field. Enter the associated password in the second field and then click the `Install Software Button`.

* A `The installation was successful` window should now appear. Click the `Close` button.

* A final prompt may appear asking the enduser if they want to move the `Oracle VM VirtualBox Installer to the Trash`. Click the `Move to Trash` button.

* Open a new `Finder` window and click on the `^` symbol to eject the `VirtualBox` install mount. This will close the previous `VirtualBox` install prompt.

### Step 3: VirtualBox Extensions for VirtualBox Installation

* In a web browser, navigate to https://www.virtualbox.org/wiki/Downloads to download the latest version. Click on the Oracle VM VirtualBox Extension Pack `All supported platforms` link to start.

* The `Oracle_VM_VirtualBox_Extension_Pack-x.x.xxx.vbox-extpack` file download process will start and depending on the enduser's Internet connection will take between 5 - 15 mins. This file once downloaded will be found in the endusers `Downloads` folder.

* Double-click the `Oracle_VM_VirtualBox_Extension_Pack-x.x.xxx.vbox-extpack` file in the `Downloads` folder.

* This should automatically start VirtualBox and a dropdown window will appear directing the enduser to either install or upgrade the Extension pack. Click on the appropriate install or upgrade button. Either will have the same effect.

* A VirtualBox license agreement prompt should appear. Using the scrollbar on the right hand side of the prompt, scroll to the bottom (after reading of course ;) ) and click the `I agree` button.

*  The enduser will be asked for their user name and password in a new prompt. Enter accordingly.

* The extension pack should now install (<10 seconds) and a new prompt with `The extension pack Oracle VM VirtualBox Extension Pack was installed successfully.` should appear. Click the `OK` button to close. The enduser can also now close the VirtualBox window / application.

### Step 4: Vagrant Installation

Vagrant is a tool for building and managing virtual machine environments in a single workflow. Endusers can read more [here](https://www.vagrantup.com/intro/index.html)

* In a web browser, navigate to https://www.vagrantup.com/downloads.html to download the latest version. Click on the `64-bit` link in the Mac OS X section.

* The `vagrant_x.x.x_x86_64.dmg` file download process will start and depending on the enduser's Internet connection will take between 5 - 15 mins. This file once downloaded will be found in the endusers `Downloads` folder.

* Double-click the `vagrant_x.x.x_x86_64.dmg` file in the `Downloads` folder.

* A `Vagrant` install prompt will appear. Within the prompt, Double click on the `vagrant.pkg` icon.

* A `Install Vagrant` prompt will appear, click on the `Continue` button.

* The `Standard Install on...` window appears, click on the `Install` button at the lower right hand corner.

* The `Installer is trying to install new software` prompt appears. The enduser's User Name should already be filled out in the top field. Enter the associated password in the second field and then click the `Install Software Button`.

* A `The installation was successful` window should now appear. Click the `Close` button.

* A final prompt may appear asking the enduser if they want to move the `Vagrant Installer to the Trash`. Click the `Move to Trash` button.

* Open a new `Finder` window and click on the `^` symbol to eject the `Vagrant` install mount. This will close the previous `Vagrant` install prompt.


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

Some endusers create a new directory e.g. `Projects` or `Code` within their `Documents` directory. It is entirely up to the enduser to decide. The steps below are an example.

The built-in terminal for MacOs can be found in `~/Applications/Utilities/Terminal`

* Open a terminal

* `cd ~/Documents`

* `git clone -b new-dev2 https://github.com/Islandora-Collaboration-Group/ISLE`

   * _This process will take 3 - 5 minutes depending on internet bandwidth_

* A new directory `ISLE` should now be visible. The enduser can check by either:

  * using the `Finder` application to navigate to this directory

  * Or within the same terminal, enter `ls -lha`

### Step 7: Start the ISLE Host Vagrant Ubuntu VM

* Open a terminal and enter the following:

   * `cd ~/path_to/ISLE/vagrant/`
   * `vagrant up`

This process should start the ISLE Host Vagrant Ubuntu VM and should take anywhere from 10 - 20 minutes for the software to download and start.

### Step 8: Install Docker, Docker Compose and ISLE on the ISLE Host Vagrant Ubuntu VM

Once this process finishes and the ISLE Host Vagrant Ubuntu VM is running.

* The enduser should enter in the terminal:

    * `vagrant ssh`

This command shells in the vagrant user to the ISLE Host.

The enduser now has a choice to follow these one of these two guides to install the dependencies.

* **Choice 1:** [Host Local - Ubuntu Setup Guide](host_local_setup_ubuntu.md)
  * start on **Line 34 - #### Docker setup for Ubuntu 16.04 LTS** to perform this process manually on the ISLE Host Vagrant Ubuntu VM

* **Choice 2** [Host Local - Ansible Setup Guide](host_local_setup_ansible.md)
  * start on **Line 392 - ### Host Local Setup - Ansible MacOS ONLY** to setup Ansible, use the supplied Ansible script and automatically install Docker, Docker-Compose on the ISLE Host Vagrant Ubuntu VM.


### Step 9: Next steps
Once this process has finished one chose to do one of the following:

* Continue next steps with the [Testsite Guide](testsite_guide.md) (isle.localdomain)

* Continue next steps with the [Newsite Guide](newsite_guide.md)

* Continue next steps with the [Migration Guide](migration_guide.md)

* Continue next steps with the [Developer Build Guide for Linux](dev_build_guide_linux.md)

* Continue next steps with the [Developer Build Guide for MacOS](dev_build_guide_mac.md)
---
