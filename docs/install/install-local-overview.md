## Local Instructions

* Explanation

* Assumptions / Prerequisites

* **Local - Migration** - Step 0. Data needs and gathering

* Steps 1 - Edit etc/hosts

* Step 2 - Setup git workflow
  * Separate document
  * Creating git repo
  * Creating git upstream to ISLE project
  * Pulling data and checking into local master
  * Pushing to remote
  * Creating dev branch
  * merges into master

* Step 3 - Edit the ISLE/.env File to select the Local Environment only

* Step 4 - Create new users and passwords
  * Separate document
  * includes config files

* Step 5: Download the ISLE images

* Step 6: Launch Process

* Step 7: Run Islandora / Drupal site Install Script
  * **Local - Migration** - Run migration scripts
  
* Step 8: Test the Site

* Step 9: Ingest Sample Objects

* Step 10: Check-in the new Drupal code into Git (_Not needed in Local - Migration_)
  * Create Islandora repo
  * how to git init, git add etc
  * Push to new Islandora repo
  
---

* Add steps below to update doc but link out to it within each environment doc.
  * Upgrade ISLE local using git
    * Create `icg-upstream`remote
    * check out new dev branch from local master
    * Pull icg-upstream into new dev branch
    * work out merge conflicts
    * commit to new branch
    * merge into master

* Separate document
  * Deploy to staging process (commits)