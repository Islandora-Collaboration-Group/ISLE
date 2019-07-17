# Update ISLE

Update an existing ISLE installation to install the newest improvements and security updates. This process is intended to be backwards compatible with your existing ISLE site.

**Important information:**

- These instructions assume you have already installed a version of ISLE using git.
- As with any enterprise system, it is strongly suggested you run these update steps in a test environment before updating your production server.
- Please always read the latest [Release Notes](../release-notes/release-1-1-2.md).

**Update your version of ISLE to the latest release:**

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

The new containers should start up and your Islandora site should be available, without any loss of existing content. 

If you run into trouble, please see the [Cleanup section of the quick start guide](https://github.com/Islandora-Collaboration-Group/ISLE#quick-stop-and-cleanup).

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails.
