#### MacOS 10.x - specific version of the setup required on the Host Server

**WORK IN PROGRESS**

Until recently, a networking bug made the Docker-for-Mac setup for ISLE an unreliable practice. Thanks to major efforts from the open-source software community (esp. Diego - thanks Diego!) the source of this bug (an extraneous jar file) has been located.

The latest versions of ISLE have not yet been tested using Docker-for-Mac. Once that has been done and the experience is documented - the rest of this page will be more accurate. Currently this page is merely a suggestion of steps, not a proper walk-through. If you do successfully install ISLE w/ Docker-for-Mac and feel like contributing your documented steps - please send them to the ISLE contact, post an issue on github or make a pull request on documentation files.


* **Host Server**: Also called "the host" - this is the base computer upon which the entire ISLE stack is built - this can be a virtual machine on a laptop (LOCAL), or a server you connected to via ssh (REMOTE).

* In this case the MacOS will serve as the host. It's better if the Mac is not already running development software including versions of the Apache web server, fedora, solr, tomcat, or any other software that may interfere with ISLE containers.


#### Docker setup for MacOS

Assumptions: Docker is installed, any servers or other software that may interfere have been disabled.


* Check if running `systemctl status docker`  
     * You may need to PRESS Shift-Z twice to exit out.

#### Create islandora user (as root)  
* `adduser islandora`

* `passwd islandora`



#### Install Docker-Compose


https://github.com/docker/compose/releases/download/1.17.1/docker-compose




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
