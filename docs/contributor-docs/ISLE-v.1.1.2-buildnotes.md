## ISLE 1.1.2 Build notes

* Use the Born-Digital fork - `ISLE-v.1.1.2.dev` branch on all things except the `isle-mysql` image

* ISLE config
  * `git clone -b ISLE-v.1.1.2.dev https://github.com/Born-Digital-US/ISLE.git`
  * `git clone -b ISLE-v.1.1.2.dev https://github.com/Born-Digital-US/isle-apache.git`
  * `git clone https://github.com/Born-Digital-US/ISLE-Drupal-Build-Tools.git` (_master and ISLE-v.1.1.2 are at sync, I had to use master due to prior failures._)
  * `git clone -b ISLE-v.1.1.2.dev https://github.com/Born-Digital-US/isle-imageservices.git`
  * `git clone -b ISLE-v.1.1.2.dev https://github.com/Born-Digital-US/isle-fedora.git`
  * `git clone -b ISLE-v.1.1.2.dev https://github.com/Born-Digital-US/isle-solr.git`
  * `git clone -b ISLE-v.1.1.2.dev https://github.com/Born-Digital-US/isle-tomcat.git`
  * `git clone https://github.com/Born-Digital-US/isle-mysql.git` (_No code changes just an image rebuild_)

* `cd ISLE`
  * Launch the ISLE stack:
    * `docker-compose up -d`
  * Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.
  * Install Islandora on the isle-apache-ld container:
  * `docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh`
    * Please note this will run the `https://github.com/Born-Digital-US/ISLE-Drupal-Build-Tools.git` version which has changes to the installer vsets.

* Recommend a test of all ingest and content types and especially Large Image format using the open-seadragon viewer.
  * Do things display properly etc?
  * Check that within the Large Image format Solution Pack configuration page that Kakadu is "turned off" and the checkbox to use it is unchecked. https://isle.localdomain/node#overlay=admin/islandora/solution_pack_config/large_image
  * Can one ingest properly tiffs with this setting on and off?

* Recommend a test with ingesting a large set of data / objects that need OCR.
  * As the batch ingest proceeds or perhaps when it has finished:
    * `docker exec -it isle-apache-ld bash`
    * `cd /tmp`
    * Are there chunk, tmp, tmp.txt, etc. files as described in:
      * https://github.com/Islandora-Collaboration-Group/isle-apache/pull/5
      * https://github.com/Islandora-Collaboration-Group/ISLE/issues/96
      * https://github.com/Islandora-Collaboration-Group/ISLE/issues/206
    * If no, then this patch has fixed the issue.
    * If yes, then report findings and re-test to ensure this patch is specific enough to cover all content types.

## Backing code out prior to official builds

### Apache Image / Dockerfile

After testing and **PRIOR** to building the official ISLE images after pulling all of the PRs, backout the following

* In line 7 of https://github.com/Born-Digital-US/isle-apache/blob/ISLE-v.1.1.2-dev/Dockerfile
  * Change back `ISLE_BUILD_TOOLS_REPO=${ISLE_BUILD_TOOLS_REPO:-https://github.com/Born-Digital-US/ISLE-Drupal-Build-Tools.git} \` to `ISLE_BUILD_TOOLS_REPO=${ISLE_BUILD_TOOLS_REPO:-https://github.com/Islandora-Collaboration-Group/isle_drupal_build_tools.git} \` only.

---

### ISLE .env

* Within the main `ISLE` project, within the `.env` file at line `43`

Currently:

```bash
#ISLE_BUILD_TOOLS_REPO=https://github.com/Islandora-Collaboration-Group/ISLE-Drupal-Build-Tools.git
#ISLE_BUILD_TOOLS_BRANCH=7.x-1.11
ISLE_BUILD_TOOLS_REPO=https://github.com/Born-Digital-US/ISLE-Drupal-Build-Tools.git
ISLE_BUILD_TOOLS_BRANCH=master
```

should revert to:

```bash
ISLE_BUILD_TOOLS_REPO=https://github.com/Islandora-Collaboration-Group/ISLE-Drupal-Build-Tools.git
ISLE_BUILD_TOOLS_BRANCH=7.x-1.11
```

---

* There shouldn't be any other changes that need to be backed out prior to the official building process
