# Update ISLE

Update an existing ISLE installation to install the newest improvements and security updates. This process is intended to be backwards compatible with your existing ISLE site.

**Important information:**

- These instructions assume you have already installed a version of ISLE using git whether you used the [Local ISLE Installation: New Site](../install/install-local-new.md) or the [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md) on your local personal computer.
- As with any enterprise system, it is strongly suggested you run these update steps in a test environment before updating your production server.
- Please always read the latest [Release Notes](../release-notes/release-1-1-2.md).
- Docker Desktop Update: If Docker alerts you that updates are available for your personal computer, please follow these steps:
    1. Go to your local ISLE site: `docker-compose down`
    2. Install the new updated version of Docker Desktop.
    3. Go to your local ISLE site: `docker-compose up -d`

**Update your version of ISLE to the latest release:**

We strongly recommend that you start the update process on your `Local` environment first to determine what type of testing is required for your projects, troubleshoot any issues and make any necessary adjustments or merges from the git code.

On your Local version of ISLE:

* In the command line (Windows: open Git Bash), navigate to the ISLE directory, which should contain the `docker-compose.local.yml` file.

* Stop and remove your existing ISLE containers
    * `docker-compose down`

* Check your git remotes: `git remote -v`
    * If you do not have a remote named `icg-upstream` then do this:
    * `git remote add icg-upstream https://github.com/Islandora-Collaboration-Group/ISLE.git`

* Run a git fetch from the ICG upstream
    * `git fetch icg-upstream`

* Checkout a new git branch as a precaution for performing the update on your project. This way your `master` branch stays safe and untouched until you have tested thoroughly and are ready to merge in changes from the recent update. The new local branch can be anything. An example is given below.
    * `git checkout isle-update-numberhere`

* Pull down the ICG ISLE `master` branch into your local project's new `isle-update-numberhere` branch
    * `git pull icg-upstream master`
    * if you `ls -lha` in this directory, you'll now have code.

* You'll need to use an text editor or IDE to determine if there are git merges to be worked out. Large ISLE point releases e.g. `1.1.2` to `1.2.0` will more than likely introduce merge conflicts and breaking changes. Move through this update process slowly and carefully. Changes will usually include but are not limited to:
    * new configuration files
    * new explanatory comments
    * new ISLE services optional or otherwise
    * new ISLE docker image tags
    * new comments
    * new documentation

* Make the appropriate changes and then commit to your local
    * `git add <changedfileshere>`
    * `git commit -m "ISLE update from version # to version #"

* You can choose to push this new branch to your remote git or keep it on your local. Ultimately after testing on your local, you'll merge to `master` and then deploy the new code to your `Staging` and `Production` environments.

* On your local (personal computer), using the same open terminal, pull down the new containers. Be sure to be in the root of your ISLE project directory. Download the new ISLE images.
    * `cd ~/yourprojecthere/`
    * `docker-compose pull`

* Start up your existing local ISLE project with the new code and ISLE images.
    * `docker-compose up -d`

* QC your existing site and ensure the following:
    * The site functions as it did before
    * All services are functional and without apparent ERROR warnings in log console output.
    * You can ingest test objects without any issue.
    * You can modify existing object data without any issue.

* Once testing has completed and any further adjustments have been made. Merge your `isle-update-numberhere` branch's code to `master`.
    * `git checkout master`
    * `git merge isle-update-numberhere`

* Push this code to your online git provider ISLE
    * `git push -u origin master`
    * This will take 2-5 mins depending on your internet speed.

* Now you have the current ISLE project code checked into git as foundation to make changes on your `Staging` and `Production` servers.

## Staging Server Update

* SSH into your `Staging` ISLE Host Server
    * `ssh islandora@yourstagingserver.institution.edu`
    * `cd /opt/yourprojecthere`

* Shut down the running containers
    * `docker-compose down`

* Update the docker files via git
    * `git pull`

* You'll need to fix the `.env` file again as you did in the `On Remote Staging - Edit the ".env" File to Change to the Staging Environment` stop of either the [Staging ISLE Installation: New Site](../install/install-staging-new.md) or the [Staging ISLE Installation: Migrate Existing Islandora Site](../install/install-staging-migrate.md) instructions. As described, this step is a multi-step, involved process that allows an end-user to make appropriate changes to the `.env` and then commit it locally to git. This local commit that never gets pushed back to the git repository is critical to allow future ISLE updates or config changes. Basically you are just restoring what you had in the `.env` to the `Staging` settings in case they are overwriten.

* Download the new ISLE docker images on the remote Staging system
    * `docker-compose pull`
    * This may take a few minutes depending on your network connection

* Run the new docker containers
    * `docker-compose up -d`

The new containers should start up and your `Staging` Islandora site should be available, without any loss of existing content.

* QC the existing ISLE `Staging` site and ensure the following:
    * The site functions as it did before
    * All services are functional and without apparent ERROR warnings in log console output.
    * You can ingest test objects without any issue.
    * You can modify existing object data without any issue.

* Recommend giving this installation a few days or a week at the minimum before repeating process on Production.

If you run into trouble, please see the [Cleanup section of the quick start guide](https://github.com/Islandora-Collaboration-Group/ISLE#quick-stop-and-cleanup).

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails.
