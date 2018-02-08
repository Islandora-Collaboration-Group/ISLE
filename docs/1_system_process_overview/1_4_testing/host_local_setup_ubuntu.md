### Host Local Setup Guide - Ubuntu

```
TO DO

Modify for Local Virtualbox settings Please

```


* This alpha build uses `isle.localdomain` as the test domain along with the Docker Compose service names e.g. `mysql, fedora, apache` etc.
     * If your institution's server uses real DNS, then skip the next step. If not, please proceed with the following:
     * To ensure this domain resolves properly, one will need to edit the servers and/or their own local `/etc/hosts` file.
       * Open up a terminal and enter: `sudo vi /etc/hosts`
         * Add one or more of the following: (**Examples ONLY**)

         * Ubuntu 16.04 LTS Host Server (or VM)  
         ```
         127.0.0.1       localhost isle.localdomain fedora apache fedora.isle.localdomain apache.isle.localdomain
         ```

         * Enduser local laptop
         ```
         127.0.0.1 localhost   
         192.156.123.26       isle.localdomain fedora apache fedora.isle.localdomain apache.isle.localdomain
         ```

#### Install / run on Ubuntu 16.04 LTS server  
* Virtualbox Virtual Machine (VM)  
* a Vagrant image (recommend using [bento/ubuntu-16.04](https://app.vagrantup.com/bento/boxes/ubuntu-16.04))  
* Cloud based server e.g. Digital Ocean or Amazon Web Services  
* Post-installation, add the appropriate ssh keys to the root user's `.ssh/authorized_keys` file.  

#### Docker setup for Ubuntu 16.04 LTS

* Open a terminal, ssh to the Ubuntu server as root and install the following:

     * If you are not root, `sudo -s`

     * `apt-get update`  

     * `apt-get install openssl git htop ntp curl`  

#### Install Docker on Ubuntu 16.04 LTS
* `curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -`  

* `add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"`  

* `apt-get update`  

* `apt-get install -y docker-ce`  

* Check if running `systemctl status docker`  
     * You may need to PRESS Shift-Z twice to exit out.

#### Create islandora user (as root)  
* `adduser islandora`

* `passwd islandora`

* `isle2017`

* `echo "islandora ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/islandora`

* Create Docker group `groupadd docker`  

* Add islandora user to docker group `usermod -aG docker islandora`  

* Create a ssh key for the islandora user
   * `mkdir /home/islandora/.ssh`  

   * `chmod -Rv 700 /home/islandora/.ssh`  

   * `cd /home/islandora/.ssh`  

   * `ssh-keygen`  
      * Follow the prompts to save these files to `/home/islandora/.ssh`

* Create an `authorized_keys` file in `/home/islandora/.ssh`using one of the following tools: `nano`, `pico`, `vi` or `emacs`

   * Example:
     ```
     vi /home/islandora/.ssh/authorized_keys
    ```

* Copy in the existing ssh key from the enduser's laptop /workstation. This will allow key based ssh access for the enduser.

      * `cat /Users/endusername/.ssh/id_rsa.pub`

      * Copy and paste this value exactly into the `/home/islandora/.ssh/authorized_keys` file.

* Exit out of the session as the root user.
    * `exit`
    * `su islandora`  

#### Install Docker-Compose (version 1.17.1 as of 11/16/2017) as islandora-user on Ubuntu 16.04 LTS
* `cd /home/islandora`

* ``sudo curl -L https://github.com/docker/compose/releases/download/1.17.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose``

* `sudo chmod +x /usr/local/bin/docker-compose`  


#### Clone ISLE repository
* Please note in some Linux Distributions, one might need to create the `/opt` directory (optional)  
    * One can `ls -lha /` to see if an `/opt` directory exists  

        * If missing, `sudo mkdir /opt`  

        * If not missing, proceed to next step.  

* `cd /opt`

* `sudo git clone -b new-dev2 https://github.com/Islandora-Collaboration-Group/ISLE.git`

   * This process will take 1 - 3 minutes depending on internet bandwidth

* `sudo chown -Rv islandora:islandora ISLE`

* `cd /opt/ISLE`


#### Next steps
Once this process has finished one chose to do one of the following:

* Continue next steps with the [Test Site Guide - isle.localdomain](testsite_guide.md)

* Continue next steps with [Migration Guide](migration_guide.md)

---
