# Installing Required Software to Run ISLE
The following pieces of software are required to run ISLE:

- Docker CE or EE (https://docker.com)
- Docker-compose (https://docs.docker.com/compose/install/)
- Git (https://github.com) (https://git-scm.com/)
 
This document will walk you through the installation of these components based on your operating system:

- [Ubuntu](#ubuntu)
- [CentOS](#centos)
- [Mac](#mac)
- [Windows](#windows)


---


## Ubuntu

### Step 1: Install Server Prerequisites and Git

- Open a terminal on your local laptop or workstation and ssh to the server / VM:

- You need to become root first
  - If you are not already root, use either `sudo -s` or `sudo su` to become root.

- Update and Install the following required software:
```bash
 apt-get update && upgrade
```
```bash
 apt-get install -y openssl git htop ntp wget curl nano apt-transport-https ca-certificates software-properties-common
```

### Step 2: Install Docker

- Add the Docker Repository
```bash
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
```
```bash
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
```
- Update package list
```bash
apt-get update
```
- Install Docker
```bash
apt-get install -y docker-ce
```

- Enable and Start Docker
```bash
systemctl enable docker && systemctl start docker
```

### Step 3: Install Docker-Compose

- Copy and paste the command below
```bash
curl -L https://github.com/docker/compose/releases/download/1.23.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose
```

- Test the Installation
```bash
docker-compose --version
```
**Example output:**

`docker-compose version 1.23.1 ...`

### Step 4: Add your user to the `docker` group
This will allow your user to run docker commands, including the ones required to launch the entire ISLE stack.

- If you are still `root` (`whoami`) type `exit` to become your normal user.

- Add yourself to the `docker` group.
```bash
sudo usermod -aG docker $USER
```

- Disconnect `exit` and reconnect for your effective groups to update.

### Step 5: Clone ISLE repository
**Please note:** The location you clone the repo to becomes your poject directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your user.

- Clone the repo by running
```bash
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

- Change to the directory containing ISLE.
```bash
cd ISLE
```

Your host server is now configured and ready to run ISLE. Return to the [homepage](../index.md) and continue with step 3 for your type of deployment (or pick from the list):

- [Test/Demo ISLE Deployment Guide](../02_installation_test/ild_installation_guide.md)
- [New ISLE Site - Single Env](../03_installation_new_site/new_site_installation_guide_single.md)
- [New ISLE Sites - Multi Envs](../03_installation_new_site/new_site_installation_guide_multi.md)
- [Migration Guide to ISLE](../04_installation_migration/migration_installation_guide.md)


---


## CentOS

### Step 1: Install Server Prerequisites and Git

- Open a terminal on your local laptop or workstation and ssh to the server / VM:

- You need to become root first
  + If you are not already root, use either `sudo -s` or `sudo su` to become root.

- Add the RHEL/CENTOS epel-release package repository first
```bash
yum install -y epel-release
```

- Install the following:
```bash
yum install -y openssl git htop ntp wget curl nano
```
```bash
yum install -y yum-utils device-mapper-persistent-data lvm2
```
### Step 2: Install Docker

- Add the Docker Repository
```bash
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo`
```

- Install Docker
```bash
yum install -y docker-ce
```

### Step 3: Install Docker-Compose

- Copy and paste the command below
```bash
curl -L https://github.com/docker/compose/releases/download/1.23.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose
```

- Test the Installation
```bash
docker-compose --version
```
**Example output:**
`docker-compose version 1.23.1 ...`

### Step 4: Add your user to the `docker` group
This will allow your user to run docker commands, including the ones required to launch the entire ISLE stack.

- If you are still `root` (`whoami`) type `exit` to become your normal user.

- Add yourself to the `docker` group.
```bash
sudo usermod -aG docker $USER
```

- Disconnect `exit` and reconnect for your effective groups to update.

### Step 5: Clone ISLE repository
**Please note:** The location you clone the repo to becomes your poject directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your user.

- Clone the repo by running
```bash
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

- Change to the directory containing ISLE.
```bash
cd ISLE
```

Your host server is now configured and ready to run ISLE. Return to the [homepage](../index.md) and continue with step 3 for your type of deployment (or pick from the list):

- [Test/Demo ISLE Deployment Guide](../02_installation_test/ild_installation_guide.md)
- [New ISLE Site - Single Env](../03_installation_new_site/new_site_installation_guide_single.md)
- [New ISLE Sites - Multi Envs](../03_installation_new_site/new_site_installation_guide_multi.md)
- [Migration Guide to ISLE](../04_installation_migration/migration_installation_guide.md)


---


## Mac

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

* Open a browser and navigate to [Docker Desktop](https://www.docker.com/products/docker-desktop)

* Click the `Download for Mac` button in the center of the page

* Click the `Please Login to Download` button on the right of the page (Login or click `Create Account`)

* Click the `Get Docker` button on the right of the page

* The `Docker.dmg` file should start to download. Check your `Downloads` directory

* Double-click the `Docker.dmg` file. The file should open and mount in a new window / prompt.

* As instructed within the prompt, drag and drop the whale icon to the right towards the `Applications` directory shortcut, a tiny green plus sign should appear, now let go from the mouse or trackpad.

* The application should start to copy data to the `Applications` directory, this process may take 1 - 5 mins depending on the speed of your hard-drive.

* Launch the `Docker` application from the `Applications` directory

* This process should may take 2 -5 mins depending on the speed of your hard-drive.

* Once fully started, one can see a whale icon at the top of their screen. If this is clicked, a dropdown should appear indicating that Docker is now running.

* Please note: This process also installs the newest version of `docker-compose`.

### Step 3: Clone ISLE repository
**Please note:** The location you clone the repo to becomes your poject directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your user.

- Clone the repo by running
```bash
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

- Change to the directory containing ISLE.
```bash
cd ISLE
```
_To improve performance on Mac OSX:_

* Open `docker-compose.yml` in a text editor and go to the the `apache` section
* Under `volumes` find the following line:
    * `- ./mnt/html:/var/www/html`
    * Change to: `- ./mnt/html:/var/www/html:cached`

Your host server is now configured and ready to run ISLE. Return to the [homepage](../index.md) and continue with step 3 for your type of deployment (or pick from the list):

- [Test/Demo ISLE Deployment Guide](../02_installation_test/ild_installation_guide.md)
- [New ISLE Site - Single Env](../03_installation_new_site/new_site_installation_guide_single.md)
- [New ISLE Sites - Multi Envs](../03_installation_new_site/new_site_installation_guide_multi.md)
- [Migration Guide to ISLE](../04_installation_migration/migration_installation_guide.md)


---


## Windows

### Step 1: Git Installation
In order to get a copy (clone) of the current ISLE project, git will need to be installed. [Git for Windows](https://gitforwindows.org/) is a software version control system for tracking changes in computer files and coordinating work on those files among multiple people.

Dowload the installer and run.  The installer will prompt for several choices.  Generally, you should be okay with defaults except:

* Change "use vi" to either Notepad++ (if installed) or nano.  Vi can be difficult to learn and Notepad++ and nano are simpler choices for those unfamiliar with vi.

If git is already installed, then please proceed to the next section.

### Step 2: Docker for Windows Installation

* Go to [https://store.docker.com/editions/community/docker-ce-desktop-windows](https://store.docker.com/editions/community/docker-ce-desktop-windows)

* Choose the "Get Docker CE for Windows (stable)" link to download the installer. You may need to create an account on Docker.com to continue.

* Run the installer and follow the prompts.  The default settings should be okay.

* You will be required to logout when the installation is complete.  

* After logging back in, Docker will run automatically.  

* If you are asked to enable Hyper-V and Containers, click to continue.  

* Your system will reboot after a few minutes.

* Once fully started, one can see a whale icon in the notification area.  

* If you are prompted to log in to Docker, you can choose to do so with your Docker.com account information, or you can simply close the window.  Docker is running and you do not need to log in to use it.

* Please note: This process also installs the newest version of `docker-compose`.

### Step 3: Clone ISLE repository
**Please note:** The location you clone the repo to becomes your poject directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your user.

- Clone the repo by running
```bash
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

- Change to the directory containing ISLE.
```bash
cd ISLE
```

Your host server is now configured and ready to run ISLE. Return to the [homepage](../index.md) and continue with step 3 for your type of deployment (or pick from the list):

- [Test/Demo ISLE Deployment Guide](../02_installation_test/ild_installation_guide.md)
- [New ISLE Site - Single Env](../03_installation_new_site/new_site_installation_guide_single.md)
- [New ISLE Sites - Multi Envs](../03_installation_new_site/new_site_installation_guide_multi.md)
- [Migration Guide to ISLE](../04_installation_migration/migration_installation_guide.md)
