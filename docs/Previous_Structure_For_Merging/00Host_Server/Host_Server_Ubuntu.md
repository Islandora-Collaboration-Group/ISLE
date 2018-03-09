#### Ubuntu 16.04 LTS - specific version of the setup required on the Host Server

* **Host Server**: Also called "the host" - this is the base computer upon which the entire ISLE stack is built - this can be a virtual machine on a laptop (LOCAL), or a server you connected to via ssh (REMOTE).

This is a detailed operating system-specific guides including commands to copy/paste.

The following setup will be the same if you are building a test on a Virtual Machine on your laptop or using a physical server or cloud setup. In all these cases you'll be establishing an Ubuntu 16.04 or CentOS 7 server with the following parameters:


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

* Add islandora user to sudoers `echo "islandora ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/islandora`

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

* Copy in the existing ssh key from your own laptop /workstation. This will allow key based ssh access for the enduser.

      * consult your OS docs for the location of your public key if you don't find it here `cat /Users/endusername/.ssh/id_rsa.pub`

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

* `sudo git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

   * This process will take 1 - 3 minutes depending on internet bandwidth

* `sudo chown -Rv islandora:islandora ISLE`

* `cd /opt/ISLE`




#### Next steps
Once this process has finished one chose to do one of the following:

* To spin up a test site w/ the built in isle.localdomain URL, continue next steps with the [Development Site Guide - isle.localdomain](/01_Installation_Migration/01_1_testsite_guide.md)

* To spin up a new site using your own URL, continue next steps with the [New Site Guide](/01_Installation_Migration/01_3_new_site_guide.md)

* To spin up a site and migrate in an existing Islandora setup, continue next steps with [Migration Guide](/01_Installation_Migration/Migration_Guide.md)
