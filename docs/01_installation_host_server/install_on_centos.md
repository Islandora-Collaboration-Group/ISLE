**CentOS** server version to use: `7.x+`

This is a detailed operating system-specific guide including commands to copy/paste for all the software you may need to run ISLE on a `CentOS` ISLE Host Server.

Called "the host" - this is the base computer upon which the entire ISLE stack is built - this can be a virtual machine on a laptop or a server you connected to via ssh.

The following setup will be the same if you are setting up an ISLE host server:

   * on your laptop / desktop using Vagrant or Virtualbox VM for the test site (isle.localdomain).

   * or using a physical server or cloud hosted setup e.g. (AWS or GCP) to run an ISLE Host server for a new site or a migrated site.

In all these cases you'll be establishing a CentOS server with the following dependencies below.

### Install process overview

* Install server prerequisites
* Install Docker
* Create islandora user and group
* Setup SSH access for islandora user on VM / server (Part 1)
    * (optional) Create ssh keys on enduser's laptop / workstation
* Setup SSH access for islandora user on VM / server (Part 2)
    * (optional) Create ssh keys on enduser's laptop / workstation
* Create Docker group
* Add islandora user to wheel group
* Install Docker-Compose
    * (optional) Alternative install method for Docker-compose
* Start up and enable Docker
* Clone ISLE repository
* Next steps

#### Step 1: Install server prerequisites

* Open a terminal on your local laptop or workstation and ssh to the CentOS server / VM:

* You need to become root first

    * `sudo su`  

* Add the RHEL/CENTOS epel-release package repository first

     * `yum install -y epel-release`

* Install the following:

     * `yum install -y openssl git htop ntp wget curl nano`

#### Step 2: Install Docker

* `yum install -y yum-utils device-mapper-persistent-data lvm2`

* `yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo`

* `yum install -y docker-ce`


#### Step 3: Create islandora user and group

* `groupadd -g 10000 islandora`

* `useradd -m -d /home/islandora -s /bin/bash -u 10000 -g 10000 islandora`

* `passwd <somepassword>`

    * Please change `<somepassword>` to something else.

    * Enter `<somepassword>` for password

    * Re-enter `<somepassword>` to confirm

* Add islandora user to sudoers

    * `echo "islandora ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/islandora`

#### Step 4a: Setup SSH access for islandora user on VM / server (Part 1)

Create a ssh key for the islandora user _(this will also enable us to push code out to a repository later)_

   * `mkdir -p /home/islandora/.ssh`

   * `chmod -Rv 700 /home/islandora/.ssh`

   * `chown -Rv islandora:islandora /home/islandora/.ssh`

   * `cd /home/islandora/.ssh`

   * `su islandora`

   * `ssh-keygen -t RSA -b 4096 -C "ISLE Islandora" -f /home/islandora/.ssh/`  
	   * Follow the prompts to save your key.  You do not need to specify a password to secure your key in development;  in production you must protect your keys with strong passwords.
     * Hit the `Enter / Return` key at all of the prompts.
          * There should be no need to enter a file name. _(However some users may find when prompted they may need to enter: `id_rsa`)_
      * Do not enter a passphrase when asked twice, simply hit the `Enter / Return` key and move on.

**Example Output**

```
ssh-keygen -t RSA -b 4096 -C "ISLE Islandora" -f /home/islandora/.ssh/

   Generating public/private rsa key pair.

   Enter file in which to save the key (/home/islandora/.ssh/id_rsa):

   Enter passphrase (empty for no passphrase):

   Enter same passphrase again:

   Your identification has been saved in /home/islandora/.ssh/id_rsa.

   Your public key has been saved in /home/islandora/.ssh/id_rsa.pub.

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

* If you would like to login to the host machine using key-pairs, please ensure the following first.

   * Check if your laptop / workstation has existing ssh keys. If you do not know, use the `Create ssh keys on enduser's laptop / workstation (optional)` section below before advancing to `Part 2`.

   * If you already have existing SSH keys, move onto the `Setup SSH access for islandora user on VM / server (Part 2)` section.

---

##### (optional) Create ssh keys on enduser's laptop / workstation

Some endusers may or may not have created local SSH keys on their laptop or workstation prior to the next step. Please use the instructions below if this is the case.

* [Creating ssh keys on MacOS High Sierra](https://www.vultr.com/docs/generating-ssh-keys-on-macos-sierra-10-12-and-high-sierra-10-13)

* [Creating ssh keys on Windows 10](https://tutorials.ubuntu.com/tutorial/tutorial-ssh-keygen-on-windows#0)

* [Creating ssh keys on Ubuntu Desktop 16.04](https://www.fullstackpython.com/blog/ssh-keys-ubuntu-linux.html)

---

#### Step 4b: Setup SSH access for islandora user on VM / server (Part 2)

* If you would like to login to the host machine using key-pairs, continued from _(Part 1)_. Copy in the existing ssh key from the enduser's laptop /workstation. This is entirely dependent on the local computer being used to connect to the Islandora server. In some cases, cloud hosting based servers e.g. Google Cloud Platform may have different requirements. This will allow key based ssh access for the enduser.
    * Enduser's laptop /workstation running **MacOS example**:

            * Open a terminal and enter: `cat /Users/enduser/.ssh/id_rsa.pub` & copy the key to the clipboard. (_On the enduser laptop not the VM_)

        * Enduser's laptop /workstation running **Windows example**:

        * Open the Command Prompt application and enter `notepad C:\Users\enduser\.ssh\id_rsa.pub` & copy the key to the clipboard. (_On the enduser laptop not the VM_)

    * Enduser's laptop /workstation running **Ubuntu Desktop example**:

            * Open a terminal and enter: `cat /home/enduser/.ssh/id_rsa.pub` & copy the key to the clipboard. (_On the enduser laptop not the VM_)

* For all OS types, copy and paste this value exactly into the `/home/islandora/.ssh/authorized_keys` file using the `nano` tool on the CentOS VM (_On the CentOS VM not the enduser laptop_)
   * `nano /home/islandora/.ssh/authorized_keys`
   * Hit the `Cntrl` key and then the `o` key to write to the file.
   * Hi the `Cntrl` key and then the `x` key to exit the file.

* Exit out of the ssh session from the host server as the root user `exit`

* ssh back in as `islandora` to test if that process worked.

**Example**

```
   ssh islandora@isle-host-server.com

   -or-

   ssh islandora@10.10.10.130
```

---

##### (optional) Note for Vagrant and local VM users only

**For local VM and Vagrant users only, not steps to follow in a production environment**

Should the enduser have difficulty adding the .ssh key for the islandora user or if the enduser doesn't even have a local id_rsa.pub file, that step above can be avoided.

Simply continue to access the local ISLE Host VM via opening a terminal on the enduser laptop or workstation.

   * `cd /pathto/ISLE/vagrant/`

   * `vagrant ssh`

   * `sudo su`

   * `su islandora`

This process will switch one to islandora with out having to use a password.

---

#### Step 5: Create Docker group

* Open a terminal and ssh back into the CentOS Host Server/VM as the `islandora` user and **become root again.**

    * `sudo su`  

Create the Docker group. (_You may receive a notice that group docker already exist. This fine. Proceed with instructions._)

* `groupadd docker`  

Add the islandora user to the new docker group

* `usermod -aG docker islandora`  

#### Step 6: Add islandora user to wheel group

* `usermod -aG wheel islandora`


#### Step 7: Install Docker-Compose

* Copy and paste the command below

```

  curl -L https://github.com/docker/compose/releases/download/1.20.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose

```

* `chmod +x /usr/local/bin/docker-compose`

* Test the Installation

    * `docker-compose --version`

**Example output:**
```
docker-compose version 1.20.1, build 1719ceb
```

---

##### (optional) Alternative install method for Docker-compose

In case the commands in the steps above fail, please use this alternative install.

* As always, ensure you're the root user.

    * `sudo su`  

* Install Python Pip (package manager for Python Scripting Language)

    * `yum install -y python-pip`

* Upgrade Python to latest version

    * `yum upgrade python*`

* Upgrade Python Pip

    * `pip install --upgrade pip`

* Install Docker compose

    * `pip install docker-compose`

---

#### Step 8: Start up and enable Docker

* Enable the Docker service to start on host server boot
    * `systemctl enable docker.service`

* Start the Docker service
    * `systemctl start docker`

#### Step 9: Clone ISLE repository
* **Please note:** In some Linux Distributions, one might need to create the `/opt` directory _(optional)_   

    * One can `ls -lha /` to see if an `/opt` directory exists  

        * If missing, `mkdir /opt`  

        * If not missing, proceed to next step.  

* `cd /opt`

* `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

   * This process will take 2 - 4 minutes depending on internet bandwidth

* `chown -Rv islandora:islandora ISLE`

* `cd /opt/ISLE`

### Next steps

* Return to the [Install Start Here section](../install_start_here.md) for additional instructions.
