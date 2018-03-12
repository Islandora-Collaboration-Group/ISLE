### Code4Lib Workshop Setup Guide - MacOS (Vagrant / Virtualbox)

This is a guide to setup and install all ISLE workshop pre-requisites on a MacOS local laptop or workstation.

**Please note:** This guide is for using `Vagrant / Virtualbox` software only.

---
**2/21/2018** Networking bug has been fixed for Ubuntu and Docker For Mac.

---

### Local laptop install process overview

Here is a compressed view of what should be installed on the laptop prior to the conference.

* Install git
* Install VirtualBox
* Install VirtualBox Extensions
* Install Vagrant
* Install Vagrant plugin
* Clone the ISLE project repository
* Start the ISLE Host VM (using vagrant)
* Install the ISLE project and software dependencies using the CentOS (manual) or Ansible (scripted) guides


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

* `git clone https://github.com/Islandora-Collaboration-Group/ISLE`

   * _This process will take 3 - 5 minutes depending on internet bandwidth_

* A new directory `ISLE` should now be visible. The enduser can check by either:

  * using the `Finder` application to navigate to this directory

  * Or within the same terminal, enter `ls -lha`

### Step 7: Start the ISLE Host Vagrant CentOS VM

* Open a terminal and enter the following:

   * `cd ~/path_to/ISLE/vagrant/`
   * `vagrant up`

This process should start the ISLE Host Vagrant CentOS VM and should take anywhere from 10 - 20 minutes for the software to download and start.

### Step 8: Install Docker, Docker Compose and ISLE on the ISLE Host Vagrant CentOS VM

Once this process finishes and the ISLE Host Vagrant CentOS VM is running.

* The enduser should enter in the terminal:

    * `vagrant ssh`

This command shells in the vagrant user to the ISLE Host.

* Add the RHEL/CENTOS epel-release package repository first

    * `sudo yum install epel-release`

Install the following:

* `sudo yum install openssl git htop ntp wget curl nano`

#### Install Docker on CentOS 7 (as root user)

* `sudo yum install -y yum-utils device-mapper-persistent-data lvm2`

* `sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo`

* `sudo yum install docker-ce`

#### Create islandora user (as root)

* `sudo su` (_if you're not root already_)

* `adduser islandora`

* `passwd islandora`

* `islandora`

* `echo "islandora ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/islandora`

* Create Docker group `groupadd docker`

* Add islandora user to docker group `usermod -aG docker islandora`

* Add islandora user to wheel group `sudo usermod -aG wheel islandora`

* Create a ssh key for the islandora user
   * `mkdir /home/islandora/.ssh`

   * `chmod -Rv 700 /home/islandora/.ssh`

   * `chown -Rv islandora:islandora /home/islandora/.ssh`

   * `cd /home/islandora/.ssh`

   * `su islandora`

   * `ssh-keygen`

      * Hit the `Enter / Return` key at all of the prompts.
        * No need to enter a file name.
        * Do not enter a passphrase when asked twice, simply hit the `Enter / Return` key and move on.

        * **Example Output**

```
$ ssh-keygen

Generating public/private rsa key pair.

Enter file in which to save the key (/islandora/.ssh/id_rsa):

Created directory '/islandora/.ssh'.

Enter passphrase (empty for no passphrase):

Enter same passphrase again:

Your identification has been saved in /islandora/.ssh/id_rsa.

Your public key has been saved in /islandora/.ssh/id_rsa.pub.

The key fingerprint is:
SHA256:rTCG6wCablwrIkQTGcrcaChmTZEYAwNrNBLNxLIds+I islandora@isle-host-macos
The key's randomart image is:
+---[RSA 2048]----+
|O%*oo            |
|O*&.             |
|=%.*             |
|Ooo  .   .       |
|+.. . + S .      |
|.E . o o .       |
|= o o   .        |
|++ +             |
|+.. .            |
+----[SHA256]-----+
```

* Create an `authorized_keys` file in `/home/islandora/.ssh` using the `touch` command.

   * Example:

```
touch /home/islandora/.ssh/authorized_keys
```


* Copy in the existing ssh key from the enduser's laptop or workstation. This will allow key based ssh access for the enduser.

   * `cat /Users/endusername/.ssh/id_rsa.pub` (_On the enduser laptop not the VM_)

   * Using the `nano` tool on the CentOS VM, copy and paste this value exactly into the `/home/islandora/.ssh/authorized_keys` file (_On the CentOS VM not the enduser laptop_)
     * `nano /home/islandora/.ssh/authorized_keys`
     * Hit the `Cntrl` key and then the `o` key to write to the file.
     * Hi the `Cntrl` key and then the `x` key to exit the file.

* Exit out of the ssh session from the host server as the root user `exit`

* ssh back in as `islandora` to test if that process worked.

  * **Example**

```
ssh islandora@isle-host-macos

-or-

ssh islandora@10.10.10.130
```

---

**Please note:**

Should the enduser have difficulty adding the .ssh key for the islandora user or if the enduser doesn't even have a local id_rsa.pub file, that step above can be avoided.

Simply continue to access the ISLE Host VM via opening a terminal on the enduser laptop or workstation.

* `cd /pathto/ISLE/vagrant/`

* `vagrant ssh`

* `sudo su`

* `su islandora`

This process will switch one to islandora with out having to use a password.

---

#### Enable the Docker service to start upon reboot of VM.

* Enable the Docker service to start on host server boot
    * `sudo systemctl enable docker.service`

* Start the Docker service
    * `sudo systemctl start docker`

---

#### Install Docker-Compose as islandora-user on CentOS 7
* Open a terminal and ssh back into the CentOS Host Server/VM as the `islandora` user and perform the following:


_Ensure the backtics below are in the command_
```

 sudo curl -L https://github.com/docker/compose/releases/download/1.19.0/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose

```

* `sudo chmod +x /usr/local/bin/docker-compose`

* Test the Installation
    * `docker-compose --version`
**Example output:**

```
docker-compose version 1.19.0, build 1719ceb
```

#### Alternative install method for Docker-compose

In case the commands in the steps above fail, please use this alternative install.

* Install Python Pip (package manager for Python Scripting Language)

    * `sudo yum install -y python-pip`

* Upgrade Python to latest version

    * `sudo yum upgrade python*`

* Upgrade Python Pip

    * `sudo pip install --upgrade pip`

* Install Docker compose

    * `sudo pip install docker-compose`

---

#### Clone ISLE repository
* Please note in some Linux Distributions, one might need to create the `/opt` directory (optional)

    * One can `ls -lha /` to see if an `/opt` directory exists

        * If missing, `sudo mkdir /opt`

        * If not missing, proceed to next step.

* `cd /opt`

* `sudo git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

   * This process will take 1 - 3 minutes depending on internet bandwidth

* `sudo chown -Rv islandora:islandora ISLE`

* `cd /opt/ISLE`


#### To stop the Vagrant VM (if needed)

* exit from the vm `exit` (you may have to exit twice to get back to your own machine's command line)

* `vagrant halt` This stops the guest machine Vagrant is managing
