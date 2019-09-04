<!--- TODO: Unsure if this page is obsolete. Ask a Committer. --->

You have the choice of using Ansible to deploy the Docker Host server dependencies instead of performing manual commands on either a CentOS or Ubuntu OS. The Ansible script and configuration files can be found in the root folder of the ISLE git repository in a directory named [ansible](https://github.com/Islandora-Collaboration-Group/ISLE/tree/master/ansible). Additionally, the Ansible script is designed to detect the chosen operating system and then install the appropriate dependencies with minimal user interaction or prior configuration.

Ansible is an free open source automation platform / tool which runs on Linux, Mac or BSD, doesn’t use local or remote agents and is relatively easy to setup. Ansible can help with server configuration management, application deployment, task automation and IT orchestration (_running tasks in sequence on several different servers or devices_).

If you are not familiar with the Ansible, it is recommended to start with their documentation.

* [Ansible website](https://www.ansible.com/)
* [What is Ansible?](https://www.ansible.com/overview/how-ansible-works)
* [Ansible Documentation](http://docs.ansible.com/ansible/latest/intro.html)

**Notes:**

* For MacOS users using a local ISLE Host VM on their personal computer, please scroll down to line **395** the section called
`Using Ansible on MacOS`.

* Windows sadly as of yet [cannot be used](http://docs.ansible.com/ansible/latest/intro_installation.html#control-machine-requirements) as a Ansible control machine  

* Do not use this script if running `Docker for Mac` or `Docker for Windows` as the dependencies are not required.

* This script can be used with the supplied Vagrant profiles (Ubuntu or CentOS) found within the [vagrant](https://github.com/Islandora-Collaboration-Group/ISLE/tree/master/vagrant) directory of the ISLE repository.

### Prerequisites / Assumptions

* Familiarity or knowledge of Ansible

* Note: The "Ansible Control Node" is the machine where Ansible is installed.

* Ansible Control Node Requirements: Currently Ansible can be run from any machine with Python 2 (version 2.7) or Python 3 (versions 3.5 and higher) installed. Windows isn’t supported for the control node. This includes Red Hat, Debian, CentOS, macOS, any of the BSDs, and so on.
    * `Ansible 2.2+` (choice of [install methods](http://docs.ansible.com/ansible/latest/intro_installation.html#installing-the-control-machine))
    * `openssh` (2.2.x+)
    * `git` (2.15.1+)
    * `python` (2.7+)
    * The [ISLE project repository](https://github.com/Islandora-Collaboration-Group/ISLE) cloned to an appropriate location.

* A previously created ISLE Host Server running either of these operating systems:
    * Ubuntu 16.04 LTS
    * CentOS 7   

* An account with ssh access to the ISLE Host Server

* An account with password-less sudo access on the ISLE Host Server

* The IP of the ISLE Host server

---

### Ansible Deploy Script Explanation

Within the `docker_install.yml` Ansible playbook, there are the following roles which perform different actions or cause different events on the ISLE Host server.

| Ansible Role                | Action / Event                                                  |
| -------------               | -------------                                                   |
| `dependencies`              | _Installs software dependencies and tools_     |
| `users_groups`              | _Adds users, groups and updates permissions as described above_ |
| `docker`                    | _Installs Docker_                                               |
| `docker_compose`            | _Installs Docker Compose_                                       |
| `git_clone`                 | _Clones the current ISLE git repository to `/opt/ISLE`_         |
| `docker_images`             | _Pulls down the latest ISLE Docker Images from Dockerhub.com_   |


#### Installs Software Dependencies and Tools

The Ansible script will deploy the following to the ISLE Host server:

| Ubuntu / Debian             | CentOS            |
| -------------               | -------------     |
| software-properties-common  | libselinux-python |
| python-software-properties  | libsemanage-python|
| --                          | yum-utils         |
| ntp                         | ntp               |
| openssh-client              | openssh-clients   |
| rsync                       | rsync             |
| curl                        | curl              |
| wget                        | wget              |
| git                         | git               |
| zip                         | zip               |
| unzip                       | unzip             |
| vim                         | vim               |
| nano                        | nano              |
| emacs24-nox                 | emacs             |
| htop                        | htop              |
| apt-transport-https         | --                |
| ca-certificates             | --                |


#### Makes ISLE Host Service Changes

**Please Note:** _Any of these services can be re-enabled post installation see appropriate documentation for opening ports on firewalls, etc._

| Ubuntu / Debian             | CentOS                        |
| -------------               | -------------                 |
| _enables ntp service_       | _enables ntp service_         |
| _disables ufw_              | _disables iptables_           |
| --                          | _sets selinux to permissive_  |

#### Adds Users, Groups and Updates Permissions

* Creates the `islandora` user
  * Along with an appropriate password
  * generates a ssh key
  * bestows sudo password-less access
  * Gives the `islandora` user the ability to run Docker & Docker Compose by adding the account to the Docker group.  

* Disables `root` user access via ssh
  * Disables use of passwords with ssh.
  * _key based access only to the ISLE Host server_

#### Installs Additional Software
* Docker

* Docker Compose

* Git clones the ISLE repository to `/opt/ISLE`

* Pulls down the most recent ISLE images

#### Ansible Script Directory Structure

```
ansible
├── docker_install.yml
├── host_vars
│   ├── host_local_macos_isle_localdomain.yml
│   └── isle-prod-project.institution.yml
├── inventory
└── roles
    ├── dependencies
    │   └── tasks
    │       ├── ubuntu.yml
    │       ├── main.yml
    │       └── centos.yml
    ├── docker
    │   └── tasks
    │       ├── install.yml
    │       └── main.yml
    ├── docker_compose
    │   └── tasks
    │       ├── install.yml
    │       └── main.yml
    ├── docker_images
    │   └── tasks
    │       ├── images_pull.yml
    │       └── main.yml
    ├── git_clone
    │   └── tasks
    │       ├── git_clone.yml
    │       └── main.yml
    └── users_groups
        ├── files
        │   └── islandora
        └── tasks
            ├── groups.yml
            ├── main.yml
            ├── secure.yml
            ├── ssh.yml
            └── users.yml
```



### Configure Ansible Script to Deploy to ISLE Host

To configure the Ansible script to deploy to one's institutional ISLE Host Server, please review and edit the following files within the `ansible` directory.

- `ansible/docker_install.yml`
- `ansible/inventory`
- `ansible/host_vars/isle-prod-project.institution.yml`

Basically wherever `isle-prod-project.institution` appears as a value in these three files, please replace with the appropriate ISLE Host server fully qualified domain name (**fqdn**) e.g. `yourislesite.institution.com`

One can open up and edit all files in a text editor e.g. Atom, Textedit, Textwrangler, etc.

#### `ansible/docker_install.yml`

_This is the Ansible playbook necessary to deploy software to the ISLE host server._

* At the top of the file remove `isle-prod-project.institution` and replace with the appropriate **fqdn**.  


#### `ansible/inventory.yml`
_This is the possible list of server(s) to deploy to using Ansible and its associated playbook(s)._

**Please Note:** These instructions below are repeated within the file itself.

* Line 7: remove `isle-prod-project.institution` from in between the brackets and replace with the appropriate **fqdn**

* Line 8: Add the appropriate ISLE Host server user account that has `sudo` passwordless permissions to the end of `ansible_ssh_user=`

     **Example:**: `ansible_ssh_user=janesmith`

* Line 8: Add the appropriate path to this ISLE Host Server user accounts public ssh key to the end of `ansible_ssh_private_key_file=`

  * **Example:** of inventory using settings for local Ansible deploy personal computer:

```
ansible_ssh_private_key_file=/home/janesmith/.ssh/id_rsa.pub
```

**Please Note:** This key is typically found on the local control (Ansible deploy personal computer) system and should have been previously copied to the appropriate ISLE Host server user account's `/home/islehostserver_user/.ssh/authorized_keys` file.


  * **Final Example** end result for inventory file with all settings above

```
[yourislesite.institution.com]   
yourislesite.institution.com ansible_connection=ssh ansible_ssh_user=janesmith ansible_ssh_private_key_file=/home/janesmith/.ssh/id_rsa.pub
```


#### `ansible/isle-prod-project.institution.yml`

* Copy this file and rename the copy to with the appropriate **fqdn**

  * **Example:** of how `host_vars` directory should now contain two files:  

```
ansible
├── docker_install.yml
├── host_vars
│   ├── isle-prod-project.institution.yml
│   └── yourislesite.institution.com.yml
├── inventory
└── roles
```

* Edit the following lines within the newly created file i.e. `yourislesite.institution.com.yml` and remove the comments (#) as asked

* **Please Note:** _The **fqdn** in the `inventory` file should match this filename as well._

**Examples ONLY** (_Do not enter these literal values_)  

 * Line 9: `ansible_ssh_host: 192.168.1.16`
    * _This is the IP address of the ISLE Host server_

 * Line 10: `ansible_ssh_user: janesmith`  
    * _This is the appropriate ISLE Host server user account that has `sudo` passwordless permissions._

  * Line 15: `pub_locale: /home/janesmith/.ssh/id_rsa.pub`
    * _This is the key found on the local control (Ansible deploy personal computer) system which has been previously copied to the appropriate ISLE Host server user account's `/home/islehostserver_user/.ssh/authorized_keys` file._

---

#### Ansible Commands to Test Connection

* Test if the Ansible control personal computer can connect to the ISLE Host server by running these commands. On the local Ansible control personal computer, open a terminal window and enter the following:

```
cd /path/to/ISLE/repository/ansible

ansible -i inventory isle-host-server -m ping
```

  * **Example:** output of above command (_IGNORE THE WARNING_)

```
[WARNING]: Found both group and host with same name: isle-host-server   

isle-host-server | SUCCESS => {
     "changed": false,
     "ping": "pong"
   }
```


**Please Note:** _If SUCCESS does not appear as a value or if the wording of the prompt is in RED with "host does not exist ...", review all steps above and check the settings. Do not advance until the **Example:** output above matches._  


### Ansible Commands for Deploy

* To deploy to the ISLE Host Server, run this command.

     `ansible-playbook -i inventory docker_install.yml`

Ansible will start displaying output within the terminal. If any turn red and the script terminates, please review the above settings and connectivity to the server. Attempt to rerun the script.

### QC Review Checklist

To ensure a successful deploy to the ISLE Server, please review the following to ensure that the deploy to the Islandora Host server was successful.

Open a terminal window on the Ansible control personal computer and `ssh` into the Islandora Host server using the appropriate end user account setup prior to the deploy.

**Example:**:

   `ssh enduser@isle-prod-project.institution`

---

#### Docker  

* To check where Docker has been installed to, enter:  

    `which docker`    

    **Example:** output:      

    `/usr/bin/docker`

* To check which version of Docker has been installed, enter:

    `docker --version`

    **Example:** output:

    `Docker version 17.12.0-ce, build c97c6d6`

* To check if Docker is running, enter:

    `service docker status`   

    **Example:** output:

```
Redirecting to /bin/systemctl status docker.service
● docker.service - Docker Application Container Engine
  Loaded: loaded (/usr/lib/systemd/system/docker.service; enabled; vendor preset: disabled)
  Active: active (running) since Thu 2018-03-18 14:36:55 EST; 1 weeks 4 days ago
    Docs: https://docs.docker.com
Main PID: 23066 (dockerd)
  Memory: 55.3M
....
```

* To check if the ISLE images have been downloaded on the ISLE Host server, enter:

    `docker image ls`

     **Example:** output:  

```
REPOSITORY                         TAG                 IMAGE ID            CREATED             SIZE
islandoracollabgroup/isle-fedora   latest              6e2749ca2c2f        2 weeks ago         2.43GB
islandoracollabgroup/isle-apache   latest              82fe4ae16932        2 weeks ago         2.18GB
islandoracollabgroup/isle-solr     latest              7fba26c10433        2 weeks ago         784MB
islandoracollabgroup/isle-mysql    latest              f34162f1e0f8        2 weeks ago         299MB
islandoracollabgroup/isle-proxy    latest              5gdc45f1a074        2 weeks ago         199MB
```


#### Docker Compose

* To check where Docker-compose has been installed to, enter:  

    `which docker-compose`

    **Example:** output:

    `/usr/local/bin/docker-compose`


* To check which version of Docker-compose has been installed, enter:

    `docker-compose -version`

    **Example:** output:

    `docker-compose version 1.20.1, build 1719ceb`


#### `islandora` User

* To check if the `islandora` user has been created, enter:

    `cat /etc/passwd`  

    **Example:** output:

    `islandora:x:1002:1002::/home/islandora:/bin/bash`


#### ISLE Project Directory

* To check if the ISLE project git repository has been cloned to `/opt/ISLE`, enter:

    `ls -lh /opt/ISLE`

    **Example:** output:


```
total 24K
drwxr-xr-x. 3 islandora islandora  110 Jan 17 09:17 apache
drwxr-xr-x. 5 islandora islandora   84 Jan 23 13:44 config
-rwxr-xr-x. 1 islandora islandora 2.0K Jan 18 16:58 docker-compose.yml
drwxr-xr-x. 7 islandora islandora  143 Jan 17 09:17 docs
drwxr-xr-x. 7 islandora islandora  102 Jan 17 09:17 fedora
-rwxr-xr-x. 1 islandora islandora  12K Jan 17 09:17 mkdocs.yml
drwxr-xr-x. 3 islandora islandora   54 Jan 17 09:17 mysql
drwxr-xr-x. 3 islandora islandora   54 Jan 17 09:17 proxy
-rwxr-xr-x. 1 islandora islandora 4.1K Jan 17 09:17 README.md
drwxr-xr-x. 4 islandora islandora   50 Jan 17 09:17 solr
```

Once this script has finished one can:

  * Add any appropriate public ssh keys to `/home/islandora/.ssh/authorized_keys` prior to attempting to ssh to the Islandora Host server as the `islandora` user.

  * Add the `/home/islandora/.ssh/id_rsa.pub` key to any git repository contained within the Migration Guide. (Additional instructions appear in that guide if this process is unfamiliar.)

Proceed to bottom of this page for next steps.

---

### Walkthrough: Using Ansible on a MacOS Control Machine

* To install `Ansible` on the end user's MacOS personal computer.

    * Install Ansible via Python pip
        * Follow here [http://docs.ansible.com/ansible/latest/intro_installation.html#latest-releases-via-pip](http://docs.ansible.com/ansible/latest/intro_installation.html#latest-releases-via-pip)

    * Or install Ansible via Homebrew (better)
        * `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
        * `brew install ansible`

* There is a ready made Ansible script for the MacOS hosted isle.localdomain called `ansible/host_vars/host_local_macos_isle_localdomain.yml`

    * Use `vagrant` for the `ansible_ssh_user=` value in the `ansible/inventory` file

    * Use `~/Users/enduser/.vagrant.d/insecure_private_key` for the `ansible_ssh_private_key_file` value in the `ansible/inventory` file

**Example:**
```    
[host_local_macos_isle_localdomain]
host_local_macos_isle_localdomain ansible_connection=ssh ansible_ssh_user=vagrant ansible_ssh_private_key_file=/Users/enduser/.vagrant.d/insecure_private_key
```

* **Line 13** in the `ansible/host_vars/host_local_macos_isle_localdomain.yml` file will need to be edited with the correct path to the end user's id_rsa.pub  file. Change the `enduser` value to the real username. (_assumes the_ `id_rsa.pub` _file exists!_)

* **Line 3** in the `ansible/docker_install.yml` file will need to be edited, change the value of `isle-prod-project.institution` to `host_local_macos_isle_localdomain`

* Once these files have been edited, open a terminal and enter:

    *  `cd path_to/ISLE/ansible/`

* Test the connection to the ISLE Host Vagrant Ubuntu VM

    * `ansible -i inventory host_local_macos_isle_localdomain -m ping`

**Example:** output of above command (_IGNORE THE WARNING_)
```
[WARNING]: Found both group and host with same name: host_local_macos_isle_localdomain   
host_local_macos_isle_localdomain | SUCCESS => {
   "changed": false,
   "ping": "pong"
 }
```
**Please Note:** _If SUCCESS does not appear as a value or if the wording of the prompt is in RED with "host does not exist ...", review all steps above and check the settings. Do not advance until the **Example:** output above matches._  


* To deploy to the ISLE Host Server, run this command.

   `ansible-playbook -i inventory docker_install.yml`

Ansible will start displaying output within the terminal. If any turn red and the script terminates, please review the above settings and connectivity to the server. Attempt to rerun the script.

---
