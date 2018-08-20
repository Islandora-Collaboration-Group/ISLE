**Windows** desktop version to use: `10` or higher

This is a detailed operating system-specific guide to setup and install all ISLE pre-requisites on a Windows 10 local laptop or workstation using Docker for Windows.

Called "the host" - this is the base computer upon which the entire ISLE stack is built.

**Please note:** This guide is for using `Docker for Windows` software only on a Windows 10 system instead of Vagrant or a Virtual Machine (VM).  The current version of `Docker for Windows` clashes with Virtualbox.  If you with to test ISLE on a Windows 10 computer that must have Virtualbox installed on it, it is suggested that you install ISLE on a guest virtual machine running [Centos](install_on_centos.md) or [Ubuntu](install_on_ubuntu_1804.md).

### Assumptions / Pre-Requisites

* Enduser has a local laptop / workstation that conforms to the specifications outlined in the [Host Server Specifications](host_server_system_specifications.md)

* Enduser **must** have administrator rights on the Windows 10 computer to proceed or the installation will fail.

* This guide is not intended for production usage.

### Install process overview

* Install git
* Install Docker for Windows
* Clone the ISLE project repository
* Pull down the ISLE images from Dockerhub
* Next steps

### Step 1: Git Installation
In order to get a copy (clone) of the current ISLE project, git will need to be installed. [Git for Windows](https://gitforwindows.org/) is a software version control system for tracking changes in computer files and coordinating work on those files among multiple people.

Dowload the installer and run.  The installer will prompt for several choices.  Generally, you should be okay with defaults except:

* Change "use vi" to either Notepad++ (if installed) or nano.  Vi can be difficult to learn and Notepad++ and nano are simpler choices for those unfamiliar with vi.

If git is already installed, then please proceed to the next section.

### Step 2: Docker for Windows Installation

* Go to [https://store.docker.com/editions/community/docker-ce-desktop-windows](https://store.docker.com/editions/community/docker-ce-desktop-windows)

* Choose the "Get Docker CE for Windows (stable)" link to download the installer. 

* Run the installer and follow the prompts.  

* You will be required to logout when the installation is complete.  

* After logging back in, Docker will run automatically.  

* If you are asked to enable Hyper-V and Containers, click to continue.  

* Your system will reboot after a few minutes.

* Once fully started, one can see a whale icon in the notification area.  

* Please note: This process also installs the newest version of `Docker-Compose`.

### Step 3: Clone the ISLE repository

In this section, the enduser will need to decide on an appropriate area on their laptop to clone the current ISLE project software to. Some endusers create a new directory e.g. `Projects` or `Code` within their `Documents` directory. It is entirely up to the enduser to decide. In the steps below, we will create a directory called `ISLE` in `Documents`.

**Please note:** Windows PowerShell is already installed on Windows 10.  

* Start Launch PowerShell

    * Click on the Windows/Start icon and start typing "powershell".  The icon for PowerShell will appear in the menu. Double-click the icon to start.

* `cd ~\Documents`

* `git clone https://github.com/Islandora-Collaboration-Group/ISLE`

   * _This process will take 3 - 5 minutes depending on internet bandwidth_

* A new directory `ISLE` should now be visible. The enduser can check by either:

    * using the `File Explorer` application to navigate to this directory

    * Or within the same terminal, enter `ls`  and verifying that `ISLE` appears in the list.

---

### Step 4: Pull down the ISLE images from Dockerhub

This process should may take 15 - 60+ mins depending on the speed of your Internet connection.

* In PowerShell enter the following commands, one at a time:
* `cd ~\Documents\ISLE `
* `docker-compose pull`

### Next steps

* Return to the [Install Start Here section](../install_start_here.md) for additional instructions or set up a [test version of ISLE](02_installation_test/ild_installation_guide/).
