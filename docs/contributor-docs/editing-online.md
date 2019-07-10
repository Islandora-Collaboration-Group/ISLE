<!--- PAGE_TITLE --->

## Overview
* Navigate to online ISLE repository
* Edit existing pages
* Adding new pages
    * Updating `mkdocs.yml`
* Commit changes using a new branch
* Create pull request for review

**Please note:** You'll need to create separate pull requests for each type of change whether editing or adding new pages with a `mkdocs.yml` file update. Using the online editor means you can only do one thing at a time.

If you need to edit more than one file or want to create more than one new page of content, it is recommended to instead follow the steps contained within the [Documentation Editing - Local](../contributor-docs/editing-local.md) Guide.

---

### Navigate to Online ISLE Repository
* In a web browser navigate to [https://github.com/Islandora-Collaboration-Group/ISLE](https://github.com/Islandora-Collaboration-Group/ISLE)

---

### Edit Existing Pages

* Click into the `docs` directory and navigate to the appropriate `.md` file you would like to edit.

* Once the file has been selected, you should see on the right hand side of the page, a navigation bar that will display `Raw | Blame | History`, computer monitor icon, a pen / pencil icon and a trashcan icon. Click the pen / pencil icon to edit the file.

* The page should change indicating that it can now be edited.

* Make the appropriate edits as needed.

* When finished and ready to "save" the changes, follow the instructions immediately in the `Commit changes` section below.

---

### Adding New Pages
**Please note:** If you are adding new pages to the Documentation, in addition to creating the new page, you'll need to edit the `mkdocs.yml` (YAML) file at the root (base) of the ISLE project directory in order for the page to show up in the documentation after adding the new page.

The example used below is how to create a new documentation page that will be about building giant ISLE robots with the title of "Building Giant ISLE Robots" and be located in the Development section.

* Click into the `docs/contributor-docs` directory

* On the right hand side of the page, a navigation bar that will display `Create new file | Upload files | Find File History`, click the `Create new file` button.

* In the empty `Name your file...` field at the top of the page, enter `building_giant_robots.md`

* Enter the appropriate content to the page.

* When finished and ready to "save" the changes, follow the instructions immediately in the `Commit changes` section below.

---

#### Updating `mkdocs`

If you have added a new page to the Documentation, you'll need to edit the `mkdocs.yml` (YAML) file at the root (base) of the ISLE project directory in order for the page to show up in the documentation.

* Click into the `mkdocs.yml` file.

* Once the file has been selected, you should see on the right hand side of the page, a navigation bar that will display `Raw | Blame | History`, computer monitor icon, a pen / pencil icon and a trashcan icon. Click the pen / pencil icon to edit the file.

* The page should change indicating that it can now be edited.

* Make the appropriate edits as needed.

In order for `mkdocs` to understand that there is a new page and page title to display in the rendered documentation, one must use the YAML code convention (syntax) to enter.

**YAML Code Convention Breakdown:**

- 'Page Title': '/section/filename.md'

* Within the `Contributor Docs` section of the open `mkdocs.yml` file, add this new line in the appropriate order.

`- 'Building Giant ISLE Robots': 'contributor-docs/building_giant_robots.md'`

```
- 'Contributor Docs':
  - 'Building Giant ISLE Robots': 'contributor-docs/building_giant_robots.md'
  - 'Contributing to Project': 'contributor-docs/contributing_to_project.md'
  - 'Documentation Overview': 'contributor-docs/overview.md'
```

* When finished and ready to "save" the changes, follow the instructions immediately in the `Commit changes` section below.

---

### Commit Changes

* When finished editing, adding or updating, scroll down to the `Commit changes` section.
  * In the first empty field, enter a short description of the file edited e.g. `Update glossary.md`

  * In the second larger empty field, enter a larger description of the edits
  made to the file. Two to five sentences should suffice.

  * Once finished, select the second radio button at the bottom
     * Its label displays: `Create a new branch for this commit and start a pull request`
     * Change the branch name as needed or leave as the default.
     * Click the green `Propose File Change`

**DO NOT** SELECT THE 1ST RADIO BUTTON - `Commit directly to the master branch` as this will cause issues though it is selected by default.

### Create Pull Request for Review

The "Open a pull request" page should now appear along with text like `The change you just made was written to a new branch named githubusername-docs-fix2. Create a pull request below to propose these changes.`

* The first and second text fields may contain part of the short description. Either keep the default text or change accordingly.
* Click the green `Create pull request` button.

This pull request should now appear in the [Pull Requests](https://github.com/Islandora-Collaboration-Group/ISLE/pulls) section of the ISLE repository, ready for review, commentary or inclusion by the ISLE Committers.
