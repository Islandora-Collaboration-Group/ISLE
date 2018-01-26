## Ansible Host Server Setup

Endusers now have the choice of using Ansible to deploy their Docker Host server instead of performing manual commands. The Ansible script and configuration files can be found in the root folder of the ISLE git repo in a directory named `ansible`.

Ansible is an free open source automation platform / tool which runs on Linux, Mac or BSD, doesn’t use local or remote agents and is relatively easy to setup. Ansible can help with server configuration management, application deployment, task automation and IT orchestration (_running tasks in sequence on several different servers or devices_).

If you are not familiar with the Ansible, it is recommended to start with their documentation.

* [Ansible website](https://www.ansible.com/)
* [What is Ansible?](https://www.ansible.com/overview/how-ansible-works)
* [Ansible Documentation](http://docs.ansible.com/ansible/latest/intro.html)

### Prerequisites / Assumptions

* Familiarity or knowledge of Ansible

* Ansible control workstation, laptop or server that runs MacOs, BSD or Linux (Windows doesn't work yet for this)

* `Ansible 2.2+` (choice of [install methods](http://docs.ansible.com/ansible/latest/intro_installation.html#installing-the-control-machine))

* A previously created ISLE Host Server running either of these operating systems:
  * Ubuntu 16.04 LTS / Debian Stretch
  * CentOS 7 / RHEL equivalent  


* An account with ssh access to the ISLE Host Server

* An account with passwordless sudo access on the ISLE Host Server

* The IP of the ISLE Host server

### Ansible Deploy Script Explanation

Within the `docker_install.yml` Ansible playbook, there are the following roles which perform different actions or cause different events on the ISLE Host server.

| Ansible Role                | Action / Event                                                  |
| -------------               | -------------                                                   |
| `dependencies`              | _Installs software dependencies & tools as described above_     |
| `users_groups`              | _Adds users, groups and updates permissions as described above_ |
| `docker`                    | _Installs Docker_                                               |
| `docker_compose`            | _Installs Docker Compose_                                       |
| `git_clone`                 | _Clones the current ISLE git repository to `/opt/ISLE`_         |
| `docker_images`             | _Pulls down the latest ISLE Docker Images from Dockerhub.com_   |



The Ansible script will deploy the following to the ISLE Host server:

#### Installs software dependencies & tools including:

| Ubuntu / Debian             | CentOS / RHEL     |
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


#### Makes ISLE Host service changes

**Please note:** _Any of these services can be re-enabled post installation see appropriate documentation for opening ports on firewalls etc._

| Ubuntu / Debian             | CentOS / RHEL                 |
| -------------               | -------------                 |
| _enables ntp service_       | _enables ntp service_         |
| _disables ufw_              | _disables iptables_           |
| --                          | _sets selinux to permissive_  |

#### Adds users, groups and updates permissions

* Creates the `islandora` user
  * Along with an appropriate password
  * generates a ssh key
  * bestows sudo password-less access
  * Gives the `islandora` user the ability to run Docker & Docker Compose by adding the account to the Docker group.  

* Disables `root` user access via ssh
  * Disables use of passwords with ssh.
  * _key based access only to the ISLE Host server_

#### Installs additional software
* Docker

* Docker Compose

* Git clones the ISLE repo to `/opt/ISLE`

* Pulls down the most recent ISLE images

#### Ansible script directory structure

```
ansible
├── docker_install.yml
├── host_vars
│   └── isle-prod-project.institution.yml
├── inventory
└── roles
    ├── dependencies
    │   └── tasks
    │       ├── debian_ubuntu.yml
    │       ├── main.yml
    │       └── rhel_centos.yml
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



### Configure Ansible Script to deploy to ISLE Host

To configure the Ansible script to deploy to one's institutional ISLE Host Server, please review and edit the following:

* `docker_install.yml` _This is the Ansible playbook_

> **TO DO**
- how to edit the following:
  - docker_install.yml
  - inventory file
  - Changing the isle-prod-project.institution.yml template within the `host_vars` directory


> - testing Ansible connection to the ISLE host server
  - `ansible -i inventory isle-host-server -m ping`

>- running the Ansible script to deploy to the ISLE host server
  - `ansible-playbook -i inventory docker_install.yml`

>- Checking if services are running, images are pulled down etc.  


### Post Server Deploy or next steps
Once this script has finished one can:

* Continue next steps with the [1.4. -Testing - Alpha Quickstart Guide](alpha_quickstart.md)

* Continue next steps with [1.4. -Testing - Migration Guide](alpha_migration_guide.md)
---
