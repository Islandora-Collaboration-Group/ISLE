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
    - `apt-get update && upgrade`
    - `apt-get install -y openssl git htop ntp wget curl nano apt-transport-https ca-certificates software-properties-common`

### Step 2: Install Docker

- Add the Docker Repository.
```
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
```

```
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
```
- Update package list.
    - `apt-get update`
- Install Docker.
    - `apt-get install -y docker-ce`

- Enable and start Docker.
    - `systemctl enable docker && systemctl start docker`

### Step 3: Install Docker-Compose

- Copy and paste the command below.
<!--- Why do we hardcode this version of docker-compose? Is there a way to get the latest stable release instead? Or do we create a list of software versions to update periodically? Note: This occurs 2 times in this file. --->
```
curl -L https://github.com/docker/compose/releases/download/1.23.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose
```

- Test the Installation.
    - `docker-compose --version`

- **Example output:** `docker-compose version 1.23.1, build 1110ad01`

### Step 4: Add Your User to the `docker` Group
Allow your user to run Docker commands and to launch the entire ISLE stack.

- If you are still `root` (`whoami`), type `exit` to become your normal user.

- Add yourself to the `docker` group.
    - `sudo usermod -aG docker $USER`

- Type `exit` and then reconnect (this allows your effective groups to update).

### Step 5: Clone ISLE Repository
**Please note:** The location you clone the repository to becomes your project directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your normal user (not `root`):

* Clone the repository:
    * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

* Change to the directory containing ISLE:
    * `cd ISLE`

Your host server is now configured and ready to install ISLE.

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
    - `yum install -y epel-release`

- Install the following:
    - `yum install -y openssl git htop ntp wget curl nano`

    - `yum install -y yum-utils device-mapper-persistent-data lvm2`

### Step 2: Install Docker

- Add the Docker Repository.
    - `yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo`

- Install Docker.
    - `yum install -y docker-ce`

### Step 3: Install Docker-Compose

- Copy and paste the command below.
```
curl -L https://github.com/docker/compose/releases/download/1.23.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && chmod +x /usr/local/bin/docker-compose
```

- Test the Installation.
    - `docker-compose --version`

- **Example output:** `docker-compose version 1.23.1, build 1110ad01`

### Step 4: Add Your User to the `docker` Group
Allow your user to run Docker commands and to launch the entire ISLE stack.

- If you are still `root` (`whoami`), type `exit` to become your normal user.

- Add yourself to the `docker` group.
    - `sudo usermod -aG docker $USER`

- Type `exit` and then reconnect (this allows your effective groups to update).

### Step 5: Clone ISLE Repository
**Please note:** The location you clone the repository to becomes your project directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your normal user (not `root`):

* Clone the repository:
    * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

* Change to the directory containing ISLE:
    * `cd ISLE`

Your host server is now configured and ready to install ISLE.

**Please continue by selecting your type of installation:**

- [Demo ISLE Installation](../install/install-demo.md)
- [Local ISLE Installation: New Site](../install/install-local-new.md)
- [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)

---

## Mac

### Step 1: Install Git
Git must be installed to get a copy (called a `clone`) of the current ISLE project. (Git is a software version control system for tracking changes in computer files and coordinating work on those files among multiple people.)

* Open a `terminal` (launch Spotlight, type "Terminal," double-click result "Terminal")

* Enter: `git --version`

* If git is already installed, the above command will output the installed version number.
    * **Example:** "git version 2.15.1"

* If git is not installed, the above command may trigger the "Install Command Line Developer Tools" prompt to appear. If so:
    * Click on the blue `Install` button for the license agreement.
    * Click the white `Agree` button.
    * The package will take 1-2 minutes to download.
    * Click the `Done` button.

* If git is not installed and there is no prompt to "Install Command Line Developer Tools", then use this tutorial to [Install Git on Mac OS X](https://www.atlassian.com/git/tutorials/install-git).

* Enter the following to fix a potential issue with long filenames:
    * `git config --system core.longpaths true`

### Step 2: Install Docker for Mac

* Click [Docker Desktop](https://www.docker.com/products/docker-desktop) and follow these instructions to download and install:
    * Click the button: `Download for Mac`
    * Click the button: `Please Login to Download`
        * Click: `Sign In` or `Sign Up`
    * Click the button: `Get Docker`
        * The `Docker.dmg` file should start to download. Check your `Downloads` directory.
    * Double-click the `Docker.dmg` file. The file should open and mount in a new window or prompt.
    * As instructed within the prompt, drag and drop the whale icon to the right towards the `Applications` directory shortcut, a tiny green plus sign should appear, now let go from the mouse.
    * The application should start to copy data to the `Applications` directory, this process may take 1-5 minutes depending on the speed of your hard-drive.
* Important Docker installation notes:
    * If Docker prompts you to restart the personal computer, please do so.
    * If Docker prompts you to install additional Docker updates, please do so.
    * If Docker prompts you (with a popup dialogue) to `Login with your Docker ID`, you may do so with your Docker.com account information, or you may skip it and close the window as Docker is already running; you do not need to login to use it.
    * If Docker prompts you for access to your computer's filesystem, please do so, then close the dialogue window.
* When the installation is finished:
    * Launch the "Docker Desktop" application from the `Applications` directory
    * This process should may take 2-5 minutes depending on the speed of your hard-drive.
* When "Docker Desktop" is fully started, a whale icon will appear at top of the screen.  Clicking on this icon should show the message: "Docker Desktop is running".

### Step 3: Install Docker-Compose

- The Docker installation has installed the newest version of `docker-compose`.

### Step 4: Clone ISLE Repository
**Please note:** The location you clone the repository to becomes your project directory. It can be located anywhere and will include your configuration and log output of the containers.

Please run these steps as your normal user (not `root`):

* Clone the repository:
    * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

* Change to the directory containing ISLE:
    * `cd ISLE`

_To improve performance on Mac OSX:_

* Depending on your current desired type of installation, open one of the two following files in a text editor:
    * `docker-compose.demo.yml`
    * `docker-compose.local.yml`
* Go to the the "apache" section, and under "volumes" find the following line:
    * `- ./mnt/html:/var/www/html`
    * Change it to be:
    * `- ./mnt/html:/var/www/html:cached`

Your host server is now configured and ready to install ISLE.

**Please continue by selecting your type of installation:**

- [Demo ISLE Installation](../install/install-demo.md)
- [Local ISLE Installation: New Site](../install/install-local-new.md)
- [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)

---

## Windows

### Step 1: Install "Git for Windows"

"Git for Windows" will install both "Git" and "Git Bash". Git must be installed so you may get a copy (called a "clone") of the current ISLE project. Git is a software version control system for tracking changes in computer files and coordinating work on those files among multiple people. Git Bash is a useful command line interface that behaves similarly to LINUX and UNIX environments; you will use this to enter commands provided below.

* Let's begin by checking to see if you have "Git Bash" already installed.
    * Press the Windows key.
    * Type `Git Bash`
* Is "Git Bash" installed?
    * If you see "Git Bash" listed, then it is installed. 
    * If "Git Bash" is not installed, please install "Git for Windows" now:
        * Download: [Git for Windows](https://gitforwindows.org/)
        * Click `Download`; `Save` this file to your Desktop; `double-click` that file to install; then click `Yes` to the prompt.
        * Click `Next` and accept ALL of the installer's default selections, with the one following exception:
            * **Choosing the default editor used by Git: Which editor would you like Git to use?**
            * Select your preferred text editor (we recommend "Atom").
            * If you selected "Atom" and do not yet have it installed, please [install Atom](https://atom.io/) now, then continue below.
* Press the Windows key
* Type `Git Bash`
* RIGHT-CLICK the "Git Bash" app to open it; select `Run as administrator`; then click `Yes` to the prompt.
* In the Git Bash terminal:
    * Enter: `git --version`
    * The above command will output the installed version number. This confirms that git is properly installed.
    * **Example:** "git version 2.23.0.windows.1"
* Check for updates:
    * Enter: `git update-git-for-windows`
* Enter the following to fix a potential issue with long filenames:
    * `git config --system core.longpaths true`

### Step 2: Install "Docker Desktop for Windows"

**Important: Docker requires Windows Professional or Windows Enterprise**

* Click [Docker Desktop for Windows](https://store.docker.com/editions/community/docker-ce-desktop-windows) and follow these instructions to download and install:
    * Click the button: `Please Login to Download`
        * Click: `Sign In` or `Sign Up`
    * Click the button: `Get Docker`
        * `Save` this file to your Desktop; `double-click` that file to install; then click `Yes` to the prompt.
    * Click `OK` or `Next` and accept ALL of the installer's default selections.
* Important Docker installation notes:
    * If Docker prompts you to restart the personal computer, please do so.
    * If Docker prompts you to install additional Docker updates, please do so.
    * If Docker prompts you to enable `Hyper-V and Containers features`, click `OK`.
    * If Docker prompts you (with a popup dialogue) to `Login with your Docker ID`, you may do so with your Docker.com account information, or you may skip it and close the window as Docker is already running; you do not need to login to use it.
* When the installation is finished:
    * Press the Windows key
    * Type `Docker Desktop`
    * Click the "Docker Desktop" app to open it
* When "Docker Desktop" is fully started, a whale icon will appear in the Windows "Notification Area". Hovering over this icon should show the message: "Docker Desktop is running".

* Enable Docker Shared Drives
    * Right-click on the Docker whale icon
    * Select "Settings"
    * Select "Shared Drives"
    * Check the box for your local "C" drive
    * Click "Apply"
    * If Docker prompts you for access to your computer's filesystem, please do so, then close the dialogue window.

### Step 3: Install Docker-Compose

- The Docker installation has installed the newest version of `docker-compose`.

### Step 4: Clone ISLE Repository
**Please note:** The location you select to clone the ISLE repository becomes your project directory. We recommend using the default user home directory; this location will include your configuration and log output of the Docker containers. (You may choose a different location, but it must not be a protected folder such as system or root directory.)

#### Use "Git Bash"

* Enter the following to change to the user's home directory:
    * `cd ~`
* Clone the repository:
    * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`
* Change to the directory containing ISLE:
    * `cd ISLE`
* Enter the following command to display the present working directory (full path) of your ISLE project.
    * `pwd`
* You will use this full path (output of the above command) in the next step.
    * **Example output:** "/c/Users/somebody/ISLE"

#### Edit "demo.env" or "local.env"

* Press the Windows key.
* Type `Notepad`.
* In the search results, RIGHT-CLICK `Notepad`, select `Run as administrator`, and enter `Yes` to prompt.
* Select `File -> Open`
* At top of dialog window, navigate to your ISLE project (refer to the "full path" displayed in previous step).
* At right side of dialog window, use the dropdown menu to change `Text Documents (*.txt)` to `All Files (*.*)`
    * (Optional: see [How to show hidden files](https://support.microsoft.com/en-us/help/4028316/windows-view-hidden-files-and-folders-in-windows-10)).
* Select the file based on your current desired type of installation: `demo.env` or `local.env`
    * Click `Open`.
* Find the following line:
    * `# COMPOSE_CONVERT_WINDOWS_PATHS=1`
* In the above line, delete the first two characters (`# `) so as to uncomment the line. It should now look like this:
    * `COMPOSE_CONVERT_WINDOWS_PATHS=1`
* Click `File > Save`, and then `File -> Exit`.

Your host server is now configured and ready to install ISLE.

**Please continue by selecting your type of installation:**

- [Demo ISLE Installation](../install/install-demo.md)
- [Local ISLE Installation: New Site](../install/install-local-new.md)
- [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)
