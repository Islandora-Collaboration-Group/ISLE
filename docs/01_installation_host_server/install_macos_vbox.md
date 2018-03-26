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
