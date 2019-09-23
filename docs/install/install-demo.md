# Demo ISLE Installation

_Expectations:  It takes an average of **45 minutes** to read this documentation and complete this installation._

This Demo ISLE Installation creates an Islandora environment on your personal computer that you can view locally in your browser as `https://isle.localdomain`. This process includes an un-themed Drupal website and an empty Fedora repository so you may ingest test objects, add or edit metadata, update fields in SOLR indexing, develop code, and otherwise "kick the tires" on ISLE.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Demo ISLE Installation is intended for a personal computer.

* You will be using ISLE version **1.2.0** or higher.

* You are using Docker-compose **1.24.0** or higher.

* You have already git cloned the ISLE Project to your personal computer.

* **For Microsoft Windows:**
    * You have installed [Git for Windows](../install/host-software-dependencies.md#windows) and will use its provided "Git Bash" as your command line interface; this behaves similarly to LINUX and UNIX environments.
    * In the "demo.env" file, you must uncomment the line "# COMPOSE_CONVERT_WINDOWS_PATHS=1". (See Software Dependencies: [Edit "demo.env" or "local.env"](../install/host-software-dependencies.md#edit-demoenv-or-localenv))

---

## Step 1: Edit "/etc/hosts" File

Enable the Demo ISLE Installation to be viewed locally on a personal computer browser as: `https://isle.localdomain`.

* Please use these instructions to [Edit the "/etc/hosts" File](../install/install-demo-edit-hosts-file.md).

---

## Step 2: Download the ISLE Images

* Open a `terminal` (Windows: open `Git Bash`)

* Navigate to your ISLE project directory. (You may already be in this directory if you are coming from the [Software Dependencies](../install/host-software-dependencies.md).)

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_):
  * `docker-compose pull`

---

## Step 3: Launch Process

* _Using the same open terminal:_
    * `docker-compose up -d`
  * **Please note:** the “ -d” argument stands for “detached” meaning the command will persist even if you close your terminal or your computer sleeps etc…)

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* _Using the same open terminal:_
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with `isle-` are expected to have a `STATUS` of `Up` (for x time).
      * **If any of these are not `UP`, then use [Demo ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, oe (c) older exited ISLE containers that maybe should be removed. --->

---

## Step 4: Run Islandora Drupal Site Install Script

This process may take 10-20 minutes (_depending on system and internet speeds_)

* Run the install site script on the Apache container by copying and pasting the appropriate command:
    * **For Mac/Ubuntu/CentOS/etc:** `docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh"`
    * **For Microsoft Windows:** `winpty docker exec -it isle-apache-ld bash -c "cd /utility-scripts/isle_drupal_build_tools && ./isle_islandora_installer.sh"`


| Microsoft Windows |
| :-------------      |
| You may be prompted by Windows to: |
| - Share the C drive with Docker.  Click Okay or Allow.|
| - Enter your username and password. Do this.|
| - Allow vpnkit.exe to communicate with the network.  Click Okay or Allow (accept default selection).|
| - If the process seems to halt, check the taskbar for background windows.|

* You should see a lot of green [ok] messages.
* If the script appears to pause or prompt for `y/n`, DO NOT enter any values; the script will automatically answer for you.
* **Proceed only after this message appears:** "Done. 'all' cache was cleared."

---

## Step 5: Test the Site

* In your web browser, enter this URL: `https://isle.localdomain`
<!--- TODO: Add error message and how to proceed (click 'Advanced...') --->
* Note: You may see an SSL error warning that the site is unsafe. It is safe, it simply uses "self-signed" SSL certs. Ignore the error and proceed to the site.
* Log in to the local Islandora site:
    * Username: `isle`
    * Password: `isle`

---

## Step 6: Ingest Sample Objects

The Islandora Collaboration Group provides a set of [Islandora Sample Objects](https://github.com/Islandora-Collaboration-Group/islandora-sample-objects) with corresponding metadata for testing Islandora's ingest process. These sample objects are organized by solution pack and are zipped for faster bulk ingestion.

* To download these sample objects, clone them to your computer's desktop:
```
git clone https://github.com/Islandora-Collaboration-Group/islandora-sample-objects.git
```

* Follow these ingestion instructions [How to Add an Item to a Digital Collection](https://wiki.duraspace.org/display/ISLANDORA/How+to+Add+an+Item+to+a+Digital+Collection)
* (Note: [Getting Started with Islandora](https://wiki.duraspace.org/display/ISLANDORA/Getting+Started+with+Islandora) contains explanations about content models, collections, and datastreams.)
* After ingesting content, you will need to add an Islandora Simple Search block to the Drupal structure. (The default search box will only search Drupal content, not Islandora content.)
    * Select from the menu: `Structure > Blocks > Islandora Simple Search`
    * Select: `Sidebar Second`
    * Click: `Save Blocks` at bottom of page
    * You may now search for ingested objects that have been indexed by SOLR

---

## Next Steps

Once you are ready, you may progress to either:

* [Local ISLE Installation: New Site](../install/install-local-new.md)

* [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md)

---

## Additional Resources
* [Demo ISLE Installation: Resources](../install/install-demo-resources.md) contains Docker container passwords and URLs for administrator testing.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

### End of Demo ISLE Installation
