# Update ISLE

Update your version of ISLE to the latest release by doing the following:

These instructions assume you have already installed a version of ISLE using git.

* In the command line, navigate to the ISLE directory.  This directory should contain the `docker-compose.yml` file.
* Stop and remove your existing ISLE containers
    * `docker-compose down`
* Update the docker files via git
    * `git pull`
* Update the docker containers
    * `docker-compose pull`
    * This may take a few minutes depending on your network connection
* Run the new docker containers
    * `docker-compose up -d`

The new containers should start up and your Islandora site should be available, without any loss of existing content.  If you run into trouble, please see the [Cleanup section of the quick start guide](https://github.com/Islandora-Collaboration-Group/ISLE#quick-stop-and-cleanup).
