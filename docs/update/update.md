# Update ISLE to the Latest Release

Update an existing ISLE installation to install the newest improvements and security updates. This process is intended to be backwards compatible with your existing ISLE site.

We strongly recommend that you begin the update process on your Local environment so that you may test and troubleshoot before proceeding to update your Staging and Production environments.

## Important Information

- These instructions assume you have already installed either the [Local ISLE Installation: New Site](../install/install-local-new.md) or the [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md) on your Local personal computer and are using that described git workflow.
- Please test these updates on your Local and Staging environments before updating your Production server.
- Always read the [Release Notes](../release-notes/release-1-1-2.md) for any version(s) newer than that which you are currently running.
- **Docker Desktop Update:** If Docker prompts that updates are available for your personal computer, please follow these steps:
    1. Go to your Local ISLE site: `docker-compose down`
    2. Install the new updated version(s) of Docker Desktop.
    3. Go to your Local ISLE site: `docker-compose up -d`

## Update Local (personal computer)

* On your Local (personal computer), open a terminal (Windows: open Git Bash) and navigate to your Local ISLE repository (this contains the "docker-compose.local.yml" file):
    * **Example:** `cd /path/to/your/repository`

* Stop and remove the existing ISLE containers:
    * `docker-compose down`

* Check your git remotes:
    * `git remote -v`
* If you do not have a remote named "icg-upstream" then create one:
    * `git remote add icg-upstream https://github.com/Islandora-Collaboration-Group/ISLE.git`

* Run a git fetch from the ICG upstream:
    * `git fetch icg-upstream`

* Checkout a new git branch as a precaution for performing the update on your project. This way your "master" branch stays safe and untouched until you have tested thoroughly and are ready to merge in changes from the recent update. You may select any name for your new local branch.
    * **Example:** `git checkout -b isle-update-numberhere`

* Pull down the ICG ISLE "master" branch into your Local project's new "isle-update-numberhere" branch:
    * `git pull icg-upstream master`
* In your ISLE directory, you may view the newest release of ISLE code by entering:
    * `ls -lha`
* Now that you have pulled down the latest code, there are likely to be conflicts between your existing code and the newer code. Run this command to determine if there are git merge conflicts:
    * `git status`
* If there are any merge conflicts, then use a text editor (or IDE) to resolve them. (The [Atom](https://atom.io/) text editor offers green and red buttons to facilitate this process.) Some releases will have more merge conflicts than others. Carefully progress through this process of resolving merge conflicts. Changes will usually include but are not limited to:
    * new configuration files
    * new ISLE services optional or otherwise
    * new ISLE docker image tags
    * new comments
    * new documentation

* After all merge conflicts are resolved, then add and commit your edits to your Local environment:
    * **Example:** `git add <changedfileshere>`
    * **Example:** `git commit -m "ISLE update from version #X to version #Y"`

* Optional: If you want to backup this new branch to your origin, then run this command: (Ultimately after testing on your Local, you'll merge to `master` and then deploy the new code to your Staging and Production environments.)
    * **Example:** `git push origin isle-update-numberhere`

* Using the same open terminal, ensure you are in the root of your ISLE project directory:
    * **Example:** `cd /path/to/your/repository`
* Download the new ISLE docker images to the Local (personal computer):
    * `docker-compose pull`

* Run the new docker containers (and new code) on your Local environment:
    * `docker-compose up -d`

* In your web browser, enter the URL of your Local site:
    * **Example:** `https://yourprojectnamehere.localdomain`
* Quality control (QC) the Local site and ensure the following:
    * The site appears and functions as it did prior to these updates.
    * You can ingest test objects without any issue.
    * You can modify existing object data without any issue.
    * All services are functional and without apparent ERROR warnings in the browser log console output.
        * **Example:** In Chrome, press the F12 button to open the [Console](https://developers.google.com/web/tools/chrome-devtools/console/), then select the "Console" tab.

* When you have completed testing and have no further adjustments to make, switch from your "isle-update-numberhere" branch of code to your "master" branch:
    * `git checkout master`
* Merge your "isle-update-numberhere" branch of code to "master".
    * **Example:** `git merge isle-update-numberhere`

* Push this code to your online git provider ISLE
    * `git push -u origin master`
    * This will take 2-5 minutes depending on your internet speed.

* You now have the current ISLE project code checked into git; this will be the foundation for making changes to your Staging and Production servers.

## Update Staging Server

* SSH into your Staging ISLE Host Server:
    * **Example:** `ssh islandora@yourstagingserver.institution.edu`
    * **Example:** `cd /opt/yourprojecthere`

* Stop and remove the existing ISLE containers:
    * `docker-compose down`

* Update the docker files via git:
    * `git pull origin master`

<!-- TODO: Break this down into simpler steps -->
* You must now again fix the `.env` file as you did in the `On Remote Staging - Edit the ".env" File to Change to the Staging Environment` step of either the [Staging ISLE Installation: New Site](../install/install-staging-new.md) or the [Staging ISLE Installation: Migrate Existing Islandora Site](../install/install-staging-migrate.md) instructions. As described, this step is a multi-step, involved process that allows an end-user to make appropriate changes to the `.env` and then commit it locally to git. This local commit will never be pushed back to the git repository and this is critical because it allows future ISLE updates and/or configuration changes. In other words, you are restoring what you had in the `.env` to the Staging settings in case they are overwriten.

* Download the new ISLE docker images to the remote Staging environment:
    * `docker-compose pull`
    * This may take a few minutes depending on your network connection

* Run the new docker containers (and new code) on your Staging environment:
    * `docker-compose up -d`

The new containers should start up and your Staging Islandora site should be available, without any loss of existing content.

* QC the Staging site and ensure the following:
    * The site appears and functions as it did prior to these updates.
    * You can ingest test objects without any issue.
    * You can modify existing object data without any issue.
    * All services are functional and without apparent ERROR warnings in the browser log console output.

* We recommend that you observe the above Staging installation for a few days or a week.

## Update Production Server

 When you are confident that your Staging installation is working as expected: 

 * Repeat the same above "Update Staging Server" process but do so on your Production server environment.


## Additional Resources

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails.
