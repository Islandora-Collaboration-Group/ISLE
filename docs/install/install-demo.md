# Demo ISLE Installation

_Expectations:  It takes an average of **45 minutes** to read this documentation and complete this installation._

This Demo ISLE Installation creates an Islandora environment on your workstation that you can view locally in your browser as `https://isle.localdomain`. This process includes an un-themed Drupal website and an empty Fedora repository so you may ingest test objects, add or edit metadata, update fields in SOLR indexing, develop code, and otherwise "kick the tires" on ISLE.

Please post questions to the public [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle), or subscribe to receive emails. The [Glossary](../appendices/glossary.md) defines terms used in this documentation.

## Assumptions / Prerequisites

* This Demo ISLE Installation is intended for a local workstation.
* Using ISLE version `1.2.0` or higher
* You have already git cloned the ISLE Project to your local workstation.

---

## Step 1: Edit `/etc/hosts` File

Enable the Demo ISLE Installation to be viewed locally on workstation browser as: `https://isle.localdomain`.

* Please use these instructions to [Edit the "/etc/hosts" File](../install/install-demo-edit-hosts-file.md).

---

## Step 2: Download the ISLE images

* Open a `terminal` (Windows: open `PowerShell`)

* Navigate to your ISLE project directory. (You may already be in this directory if you are coming from the [Software Dependencies](../install/host-software-dependencies.md).)

* Download all of the latest ISLE Docker images (_~6 GB of data may take 5-10 minutes_): 
  * `docker-compose pull`

---

## Step 3: Launch Process

* _Using the same open terminal / Powershell_
  * `docker-compose up -d`

* Please wait a few moments for the stack to fully come up. Approximately 3-5 minutes.

* After the above process is completed using the already open terminal or Powershell again.
    * View only the running containers: `docker ps`
    * View all containers (both those running and stopped): `docker ps -a`
    * All containers prefixed with `isle-` are expected to have a `STATUS` of `Up` (for x time).
      * **If any of these are not `UP`, then use [Demo ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) to solve before continuing below.**
      <!---TODO: This could be confusing if (a) there are other, non-ISLE containers, or (b) the isle-varnish container is installed but intentionally not running, oe (c) older exited ISLE containers that maybe should be removed. --->

---

## Step 4: Run Islandora / Drupal site Install Script

This process may take 10 - 20 minutes (_depending on system and internet speeds_)

* Run the install site script on the Apache container by copying and pasting this command:
```
docker exec -it isle-apache-ld bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh
```


| For Windows Users only |
| :-------------      |
| You may be prompted by Windows to: |
| - Share the C drive with Docker.  Click Okay or Allow.|
| - Enter your username and password. Do this.|
| - Allow vpnkit.exe to communicate with the network.  Click Okay or Allow (accept default selection).|
| - If the process seems to halt, check the taskbar for background windows.|

* You should see a lot of green [ok] messages.
* If the script appears to pause or prompt for `y/n`, DO NOT enter any values; the script will automatically answer for you.
* **Proceed only after this message appears:** `Clearing Drupal Caches. 'all' cache was cleared.`

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

## Additional Resources
* [Demo ISLE Installation: Resources](../install/install-demo-resources.md) contains Docker container passwords and URLs for administrator tools.
* [ISLE Installation: Troubleshooting](../install/install-troubleshooting.md) contains help for port conflicts, non-running Docker containers, etc.

---

## End of Demo ISLE Installation.
