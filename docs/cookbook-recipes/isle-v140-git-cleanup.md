As a result of the ISLE Phase II Sprint F - Optimize Docker Images in Fall 2019, the ISLE repository has had several changes:

* Original ISLE git repo size was ~182 MB and would take a minimum of 10 -15 seconds or more to clone (_depending on one's Internet connection_)

- The cleaned up ISLE git repo size is now 6.6 MB and should take a minimum of less than 3 seconds to clone (_depending on one's Internet connection_)

* a new clean up script called [isle-v140-git-cleanup-script.sh](../assets/isle-v140-git-cleanup-script.sh) has been added along with this documentation created for end users to run on their ISLE repository.

- Any new pull request will have to be created from a forked or cloned ISLE repository that has had this cleanup process performed on it.

---

## **AS OF THE ISLE v.1.4.0 release in November 2019 (MANDATORY)**

Due to removal of older files and folders and git changes in the ISLE git repository as of `November 25, 2019`, please review the following:

* If you are a **new** ISLE user and are cloning or forking this project for the first time after `November 25, 2019`, then you can **ignore this documentation** and move on to the Quick Start section within the README.md as needed.

- If you have **previously forked or cloned** the ISLE project **prior to** `November 25, 2019`, then you **do need to follow the instructions** given in this documentation.

* If you still feel like you're not sure or cannot tell if you need to do this, then please refer to the instructions given in the `Identification Tools` section below within this documentation.

- If you intend to submit pull requests (PRs) of any kind (documentation, code etc) to the ISLE project, ISLE maintainers will **NOT** accept PRs from repos that haven't had the script run on them as using an uncleaned forked repo will cause the removed files to return.

- Please run this script and process **PRIOR** to attempting to follow the normal ISLE update procedure for version `1.4.0`.

There is a brief description of what the script actually does found below in the **Script commands - explained** section.

---

## Git cleanup process


### Git cleanup process - Overview

The steps below in the section `Git cleanup process` will have you perform the following:

* Open up a terminal and perform steps as directed
- Make a directory called `ISLE_140_GIT_CLEANUP` on your laptop or workstation to perform all of these steps and collect all materials
* Download the git cleanup script (`isle-v140-git-cleanup-script.sh`)and move it to the `ISLE_140_GIT_CLEANUP` directory
- `git clone --mirror` your forked or cloned ISLE project directory to the `ISLE_140_GIT_CLEANUP` directory
* Run the git cleanup script. It will prompt you to only enter the name of your forked or cloned ISLE project directory.
- `git push` back the changes to your remote forked or cloned ISLE project directory.
* Clone down your newly changed and smaller forked or cloned ISLE project directory again to ensure that the repositiory is smaller to a location of your choice.
- Remove all previous larger size versions of your forked or cloned ISLE project directory. You can now delete the `ISLE_140_GIT_CLEANUP` directory and its contents.
* Work from the newly resized and smaller forked or cloned ISLE project directory.
  * As needed re-clone and redeploy projects on locals, staging and production servers.

### Git cleanup process - Steps

  * Open up a terminal

  * Change directory to a location of your choice
    * `cd your_path_here`

  * Make a new temporary direcotory called `ISLE_140_GIT_CLEANUP`
    * `mkdir ISLE_140_GIT_CLEANUP`

  * Download the script found and save it to the `ISLE_140_GIT_CLEANUP` directory and fix the permissions.
    * `wget https://raw.githubusercontent.com/Islandora-Collaboration-Group/ISLE/master/docs/assets/isle-v140-git-cleanup-script.sh`
    
    * `chmod 755 isle-v140-git-cleanup-script.sh`

  * Run this git command to clone your forked or cloned ISLE project directory. This is not the usual `git clone` command, note the `--mirror` flag.
    * `git clone --mirror git project url`
    
    * Example:
    
      * `git clone --mirror git@github.com:username/ISLE.git` (_as a fork using the same project name or title_)
      * `git clone --mirror git@github.com:username/yourprojectnamehere-isle.git` (_as a fork or cloned repo using a different project name or title_)
      
    * This will create a new directory called `ISLE.git` or `yourprojectnamehere-isle.git` which is not typical clone or fork output.

  * Run the cleanup script
    * `./isle-v140-git-cleanup-script.sh`

  * At the start of the script, there will be an interactive input prompt asking you to:
  
```bash
"Type in the name of your git repo only e.g. ISLE (do not include .git) and then press the [ENTER] or [RETURN] key to continue"`
Enter the name here:  
```
  * Enter the name of your forked or cloned ISLE git repository without the .git extension e.g `ISLE` or `yourprojectnamehere-isle` etc.

  * The script will continue and the process takes about 30 - 45 seconds depending on the speed of your system with minimal output.

  * Your final step is to push this local smaller repo back to the remote repository
    * `cd ISLE.git` or `cd yourprojectnamehere-isle.git`
    
    * `git push`

  * Clone down your newly changed and smaller forked or cloned ISLE project directory again to ensure that the repositiory is smaller to a location of your choice.

- **Remove all previous versions of your cloned ISLE project directory.**
  * You can now delete the `ISLE_140_GIT_CLEANUP` directory and its contents.

  * Work from a newly cloned `ISLE` or `yourprojectnamehere-isle` project directory and continue with the ISLE update process to `1.4.0` (_if/as needed_)

### Git cleanup process - Example redeploy process

  * To re-deploy on your local
    * **Assumptions:** - You'll have already run the git cleanup steps and script on your forked ISLE git project.
    * cd into your the parent directory that contains your `yourprojectnamehere-isle` directory e.g. `~/Code/`, `Documents`, `Projects`, or `Sites`.
      * This will be the directory you chose to locate your `yourprojectnamehere-isle` directory.
    * `sudo mkdir isle-archive`
    * `mv yourprojectnamehere-isle isle-archive/`
    * `git clone yourprojectnamehere-isle project`
      * If you had any untracked files, you'll need to copy them back into place from `isle-archive/yourprojectnamehere-isle` to the new `yourprojectnamehere-isle`
      * `docker-compose pull` (_just incase you didn't have any of the new images_)  
     * If upgrading to ISLE `1.4.0`, then follow the usual [ISLE update steps](../update/update.md)

  * To re-deploy on your staging & production server(s)
    * **Assumptions:**
      * You'll have already run the git cleanup steps and script on your forked ISLE git project.
      * You have already re-cloned your local
      * You have already followed the usual [ISLE update steps](../update/update.md) for version `1.4.0`
  
    * Start with `Staging` first
      * ssh into your `Staging` ISLE host server
      * `cd /opt/`
      * `sudo mkdir isle-archive`
      * `cd /opt/yourprojectnamehere-isle`
      * `docker-compose down`
      * `sudo chown -Rv islandora:islandora isle-archive`
      * `sudo mv /opt/yourprojectnamehere-isle /opt/isle-archive/`
      * `git clone yourprojectnamehere-isle project to /opt/`
      * If you had any untracked files like `acme.json`, you'll need to copy them back into place from `/opt/isle-archive/yourprojectnamehere-isle`
        * For example: `cp /opt/isle-archive/yourprojectnamehere-isle/config/proxy/acme.json /opt/yourprojectnamehere-isle/config/proxy/acme.json`
      * Edit your .env for the correct environment and commit the change but do not push back.
      * `docker-compose pull` (_just incase you didn't have any of the new images_)
      * `docker-compose up -d` to start up the containers
  
    * Repeat these steps above on the `Production` server

---


## Identification tools

* **Please note:** In some of the examples given below, the title and/or names of the git repository examples use ISLE; your forked or cloned repo may be using a different project name or title, so please replace the `ISLE` for the name of your forked or cloned ISLE project git repository instead e.g. `yourprojectnamehere-isle`.

If you would like to check if the files have been removed or are still present:

  * Clone this [repo](https://github.com/ivantikal/git-tools.git) to pinpoint offending files and folders to the parent directory of where your local forked or cloned ISLE git repository is located. **Do not** clone this tool **into** your local forked or cloned ISLE git repository.
    * `git clone https://github.com/ivantikal/git-tools.git`
    * Your parent directory should now look like this example below: (_again please note your local forked or cloned ISLE git repository might not use the ISLE as the title_)

```bash
  directory/
  ├── git-tools
  └── ISLE
```

  * Run the tools against your existing local forked or cloned ISLE git repository.
    * **Example** usage for finding the 50 biggest files or folders in your local forked or cloned ISLE git repository. You can swap out the `ISLE` for the name of your forked or cloned ISLE project git repository e.g. `yourprojectnamehere-isle`
      * `./git-tools/clean-binaries/get_biggest_files_in_history.sh -r ./ISLE/ -n 50`

  * Review the resulting file to see if any of the files and folders listed in the `Files & Folder removed` section appear.
    * `cat ./git-tools/clean-binaries/get_biggest_files_in_history.sh.tmp/bigtosmall.txt`

  * If none of the files appear, you don't need to run the script.

  * If **any** of the files or folders appear, you **have** to run the script. Scroll back up to the top of this document and follow the steps as provided.

  * More information can be found here on to use this [tool](https://www.tikalk.com/posts/2017/04/19/delete-binaries-from-git-repository/)

---

## Files & Folder removed

* Over 176 MB of errant blobs, binaries and code were identified and removed.

Files removed:

```bash
dg.localdomain.key
dg.localdomain.pem
docker-compose.DG-FEDORA.yml
fcrepo-drupalauthfilter-3.8.1.jar
fcrepo-installer-3.8.1.jar
fedoragsearch-2.8.1.zip
log4j-over-slf4j-1.6.6.jar
search_index.json
sitemap.xml
sitemap.xml.gz
slf4j-jdk14-1.6.6.jar
```

Folders (and files within) removed:
```bash
drupal
solr-4.10.4
```

## Script commands - explained

You are welcome to open up the `isle-v140-git-cleanup-script.sh` in a text editor to review its contents prior to running.

Essentially `isle-v140-git-cleanup-script.sh` will perform the following:

  * Download the [BFG-repo-cleaner](https://rtyley.github.io/bfg-repo-cleaner/) jar file `bfg-1.113.0.jar`

  * Run the appropriate BFG java commands to delete files and folders

  * Run these git commands on your forked or cloned ISLE git repo
    * `git reflog expire --expire=now --all && git gc --prune=now --aggressive`

  * **Please note:** This script will **not** push back to your source repo. That you need to do manually.
