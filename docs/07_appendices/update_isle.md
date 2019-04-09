<!--- PAGE_TITLE --->

If you are already running an older version of ISLE, update to the latest version by doing the following:

**WARNING** It is recommended you do not do this on a production server until after the ISLE
1.1 Release Candidate has been fully tested by the community and released as a stable version.

These instructions assume you have already installed a version of ISLE using git.

* In the command line, navigate to the ISLE directory.  This directory should contain the `docker-compose.yml` file.
* Shut down the existing ISLE stack, if running
    * `docker-compose down -v`
* Update the docker files via git
    * `git pull`
* Update the docker containers
    * `docker-compose pull`
    * This may take a few minutes depending on your network connection
* Run the new docker containers
    * `docker-compose up -d`

If all goes well, the new containers should start up and your Islandora site should be available again, with no loss of existing content.  If you run into trouble, please see the [Cleanup section of the quick start guide](https://github.com/Islandora-Collaboration-Group/ISLE#quick-stop-and-cleanup).
