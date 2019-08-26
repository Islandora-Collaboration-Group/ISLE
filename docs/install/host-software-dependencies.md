# Software Dependencies

**Please select your operating system:**

- [Ubuntu](#ubuntu)
- [CentOS](#centos)
- [Mac](#mac)
- [Windows](#windows)

---

## Ubuntu

### Step 1: Install Server Prerequisites and Git

- Open a terminal on your personal computer and ssh to the server or VM.

- You must have `root` level permissions.
    - If you are not already `root`, enter either `sudo -s` or `sudo su` to become root.

- Update and install the following required software:
```
 apt-get update && upgrade
```

```
 apt-get install -y openssl git htop ntp wget curl nano apt-transport-https ca-certificates software-properties-common
```

### Step 2: Install Docker

- Add the Docker Repository.
```
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
```

```
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
```
- Update package list.
```
apt-get update
```
- Install Docker.
```
apt-get install -y docker-ce
```

- Enable and start Docker.
```
systemctl enable docker && systemctl start docker
```

### Step 3: Install Docker-Compose

- Copy and paste the command below.
<!--- Why do we hardcode this version of docker-compose? Is there a way to get the latest stable release instead? Or do we create a list of software versions to update periodically? Note: This occurs 2 times in this file. --->
```
curl -L https://github.com/docker/compose/releases/download/1.23.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose
```

- Test the Installation.
```
docker-compose --version
```

- **Example output:**
`docker-compose version 1.23.1, build 1110ad01`

### Step 4: Add Your User to the `docker` Group
Allow your user to run Docker commands and to launch the entire ISLE stack.

- If you are still `root` (`whoami`), type `exit` to become your normal user.

- Add yourself to the `docker` group.
```
sudo usermod -aG docker $USER
```

- Type `exit` and then reconnect (this allows your effective groups to update).

### Step 5: Clone ISLE Repository
**Please note:** The location you clone the repository to becomes your project directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your normal user (not `root`):

* Clone the repository.
```
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

- Change to the directory containing ISLE.
```
cd ISLE
```

Your host server is now configured and ready to run ISLE.

**Please continue by selecting your type of installation:**

- [Demo ISLE Installation](../install/install-demo.md)
- [Local ISLE Installation: New Site](../install/install-local-new.md)
- [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)

---

## CentOS

### Step 1: Install Server Prerequisites and Git

- Open a terminal on your personal computer and ssh to the server or VM.

- You must have `root` level permissions.
    - If you are not already `root`, enter either `sudo -s` or `sudo su` to become root.

- Add the CentOS/RHEL epel-release package repository.
```
yum install -y epel-release
```

- Install the following:
```
yum install -y openssl git htop ntp wget curl nano
```

```
yum install -y yum-utils device-mapper-persistent-data lvm2
```
### Step 2: Install Docker

- Add the Docker Repository.
```
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
```

- Install Docker.
```
yum install -y docker-ce
```

### Step 3: Install Docker-Compose

- Copy and paste the command below.
```
curl -L https://github.com/docker/compose/releases/download/1.23.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose
```

- Test the Installation.
```
docker-compose --version
```

- **Example output:**
`docker-compose version 1.23.1, build 1110ad01`

### Step 4: Add Your User to the `docker` Group
Allow your user to run Docker commands and to launch the entire ISLE stack.

- If you are still `root` (`whoami`), type `exit` to become your normal user.

- Add yourself to the `docker` group.
```
sudo usermod -aG docker $USER
```

- Type `exit` and then reconnect (this allows your effective groups to update).

### Step 5: Clone ISLE Repository
**Please note:** The location you clone the repository to becomes your project directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your normal user (not `root`):

* Clone the repository.
```
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

- Change to the directory containing ISLE.
```
cd ISLE
```

Your host server is now configured and ready to run ISLE.

**Please continue by selecting your type of installation:**

- [Demo ISLE Installation](../install/install-demo.md)
- [Local ISLE Installation: New Site](../install/install-local-new.md)
- [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)

---

## Mac

### Step 1: Install Git
Git must be installed to get a copy (called a `clone`) of the current ISLE project. (Git is a software version control system for tracking changes in computer files and coordinating work on those files among multiple people.)

 * Open a `terminal` (launch Spotlight, type "Terminal," double-click result "Terminal")
 * Enter: `git --version`.
 * If git is already installed, the above command will output the installed version number. For example:  

```
$git --version
git version 2.15.1
```

* If git is not installed, the above command may trigger the `Install Command Line Developer Tools` prompt to appear. If so:
    * Click on the blue `Install` button for the license agreement.
    * Click the white `Agree` button.
    * The package will take 1-2 minutes to download.
    * Click the `Done` button once finished.

* If git is not installed and the prompt does not show, then use this tutorial to [Install Git on Mac OS X](https://www.atlassian.com/git/tutorials/install-git).

### Step 2: Install Docker for Mac

* Open a browser and navigate to [Docker Desktop](https://www.docker.com/products/docker-desktop)

* Click the `Download for Mac` button in the center of the page

* Click the `Please Login to Download` button on the right of the page (click: `Sign In` or `Create Account`)

* Click the `Get Docker` button on the right of the page

* The `Docker.dmg` file should start to download. Check your `Downloads` directory

* Double-click the `Docker.dmg` file. The file should open and mount in a new window / prompt.

* As instructed within the prompt, drag and drop the whale icon to the right towards the `Applications` directory shortcut, a tiny green plus sign should appear, now let go from the mouse or trackpad.

* The application should start to copy data to the `Applications` directory, this process may take 1 - 5 mins depending on the speed of your hard-drive.

* Launch the `Docker` application from the `Applications` directory

* This process should may take 2 -5 mins depending on the speed of your hard-drive.

* Once fully started, a whale icon will appear at top of the screen. If clicked, a dropdown should appear indicating that Docker is now running.

### Step 3: Install Docker-Compose

- The Docker installation has installed the newest version of `docker-compose`.

### Step 4: Clone ISLE Repository
**Please note:** The location you clone the repository to becomes your project directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your normal user (not `root`):

* Clone the repository.
```
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

- Change to the directory containing ISLE.
```
cd ISLE
```
_To improve performance on Mac OSX:_

* Open `docker-compose.yml` in a text editor and go to the the `apache` section
* Under `volumes` find the following line:
    * `- ./mnt/html:/var/www/html`
    * Change to: `- ./mnt/html:/var/www/html:cached`

Your host server is now configured and ready to run ISLE.

**Please continue by selecting your type of installation:**

- [Demo ISLE Installation](../install/install-demo.md)
- [Local ISLE Installation: New Site](../install/install-local-new.md)
- [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)

---

## Windows

### Step 1: Install Git
Git must be installed to get a copy (called a `clone`) of the current ISLE project. (Git is a software version control system for tracking changes in computer files and coordinating work on those files among multiple people.)

* Press the Windows key.
* Type `PowerShell`.
* In the search results, RIGHT-CLICK `Windows PowerShell`, select `Run as administrator`, and enter `Yes` to prompt.
* Enter: `git --version`.
* If git is already installed, the above command will output the installed version number. For example:  

```
$git --version
git version 2.15.1
```

* If git is not installed, then
    * [Download Git for Windows](https://gitforwindows.org/).
    * Click `Download`, `Save` the file to your Desktop, `double-click` that file to install, then click `Yes` to the prompt.
    * Click `Next` to accept all of the installer's default selections.

### Step 2: Install Docker Desktop for Windows

**Important: Docker requires Windows Professional or Windows Enterprise**

* [Download Docker Desktop for Windows](https://store.docker.com/editions/community/docker-ce-desktop-windows)

* Click the `Please Login to Download` button on the right of the page (click: `Sign In` or `Create Account`)

* Click the `Get Docker` button on the right of the page, `Save` the file to your Desktop, `double-click` that file to install, then click `Yes` to the prompt.

* Click `OK` or `Next` to accept all of the installer's default selections.

* You will be required to `Close and log out` when the installation is complete.

* The computer will reboot; please sign in.

* If prompted to enable `Hyper-V and Containers features`, click `OK`.

* If prompted with a Docker popup dialogue to `Login with your Docker ID`, you may do so with your Docker.com account information, or you may simply close the window.  Docker is running and you do not need to login to use it.

* Once fully started, a whale icon will appear in the notification area. If clicked, a dropdown should appear indicating that Docker is now running.

* Enable Docker Shared Drives
    * Right-click on the Docker whale icon
    * Select "Settings"
    * Select "Shared Drives"
    * Check the box for your local "C" drive
    * Click "Apply"

### Step 3: Install Docker-Compose

- The Docker installation has installed the newest version of `docker-compose`.

### Step 4: Clone ISLE Repository
**Please note:** The location you select to clone the ISLE repository becomes your project directory. We recommend using the default user home directory; this location will include your configuration and log output of the Docker containers. (You may choose a different location, but it must not be a protected folder such as system or root directory.)

* Use `PowerShell` (and remember to `Run as administrator`).
* Enter `cd ~` (to change to the user's home directory).
* Clone the repository.

```
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```

* Change to the directory containing ISLE.
```
cd ISLE
```

* Copy the path of the present working directory.
```
pwd
```
* Use mouse/trackpad to highlight the full path (i.e. `C:\Users\somebody\ISLE`) and Click `Enter` to copy to clipboard.

* Edit .env file.
  * Press the Windows key.
  * Type `Notepad`.
  * In the search results, RIGHT-CLICK `Notepad`, select `Run as administrator`, and enter `Yes` to prompt.
  * Select `File -> Open`
  * In the `File name:` input box, paste the above copied path. Click `Enter`.
    * Use dropdown on right to change `Text Documents (*.txt)` to `All Files (*.*)` (if needed, see [How to show hidden files](https://support.microsoft.com/en-us/help/4028316/windows-view-hidden-files-and-folders-in-windows-10)).
    * Select the `.env` file and click `Open`.
    * Find the following line and uncomment it (by deleting the preceding `#` character):

    ```# COMPOSE_CONVERT_WINDOWS_PATHS=1```

    * to:

    ```COMPOSE_CONVERT_WINDOWS_PATHS=1```

    * Click `File > Save`, and then `File -> Exit`.

Your host server is now configured and ready to run ISLE.

**Please continue by selecting your type of installation:**

- [Demo ISLE Installation](../install/install-demo.md)
- [Local ISLE Installation: New Site](../install/install-local-new.md)
- [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)
