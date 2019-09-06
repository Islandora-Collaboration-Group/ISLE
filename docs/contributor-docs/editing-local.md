<!--- PAGE_TITLE --->

### Overview
* Run the `mkdocs` software
* Edit documentation files locally
* Commit changes to git
* Push committed changes to the online ISLE Github.com git repository
* Create pull request for review on ISLE Github.com

#### Assumptions / Prerequisites

* `mkdocs` and `mkdocs-material` theme are installed following the [Installing Mkdocs](../contributor-docs/mkdocs.md) documentation.
* The ISLE project directory has been cloned to the end user's personal computer in an appropriate Code or Project Directory.
   * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`
* Familiarity or comfortability with git and cli commands.
* End user has a web browser opened to this URL: [http://127.0.0.1:8000/](http://127.0.0.1:8000/)
* End user has a terminal open with `mkdocs serve` running.

### Create a New Git Branch

As there is a review process of any new changes to the ISLE code or documentation projects by the Committers, using git pull requests are ideal to avoid issues of missing materials, inappropriately edited files or incorrect overwrites of critical data.

* While `mkdocs serve` is running in the terminal, open up a new terminal or terminal tab and `cd to pathto/ISLE project directory`

You will now create a new git branch which will allow you to make changes without fear of losing the original code / file versions kept in `master`. The new branch name can be anything you like but we suggest that you use a convention that identifies you clearly e.g. `githubusername-docs-fix2` or `janedoe-doc-fixes1`

* `git checkout -b githubusername-docs-fix2`

* Proceed to edit or create new files as necessary using a text editor of your choice.

### Adding New Pages
If you are adding new pages to the Documentation, in addition to creating the new page, you'll need to edit the `mkdocs.yml` (YAML) file at the root (base) of the ISLE project directory in order for the page to show up in the documentation.

The example used below is how to create a new documentation page that will be about building giant ISLE robots with the title of "Building Giant ISLE Robots" and be located in the Development section.

* Create a new empty file in the `docs/contributor-docs` directory e.g. `building_giant_robots.md`

* Open the `mkdocs.yml` file in a text editor.

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

* The end user can now review the contents of this new page at http://127.0.0.1:8000/contributor-docs/building_giant_robots/

* Edit the contents of `building_giant_robots.md` and save the file each time. The associated webpage will refresh with the new changes.

### Commit Changes to Git

Once finished with adding all new pages and editing files, it is time to add everything to git and then push to the remote ISLE github.com repository for review by ISLE Committers.

#### Git Status
* Within the open terminal, enter `git status`

The example git status output below displays that several files have been modified (edited or changed) and that there is a new file called `building_giant_robots.md` has been created.

```
git status

On branch githubusername-docs-fix2
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

  modified:   docs/specifications/diagram-network.md
  modified:   docs/glossary.md
  modified:   mkdocs.yml

Untracked files:
  (use "git add <file>..." to include in what will be committed)

  docs/contributor-docs/building_giant_robots.md

no changes added to commit (use "git add" and/or "git commit -a")
```
####  Git Add Files

You now need to add these changed / new files to git.

* `git add docs/specifications/diagram-network.md`
* `git add docs/glossary.md`
* `git add mkdocs.yml`
* `git add docs/contributor-docs/building_giant_robots.md`

####  Git Commit with Message

Now it is time to package up all these additions with a git commit command. Please ensure that you commit with a descriptive message of changes, etc.

* `git commit -m "Tweaked some specs in diagram-network.md. Updated the Glossary with more terms. Made new Building Giant Robots page. I love big robots. Added new files and titles to mkdocs.yml "`

###  Git Push to ISLE Github Repository

* `git push origin githubusername-docs-fix2`

### Create a Pull Request on ISLE Github.com Repository

In order for the ISLE Committers to review suggested changes a "pull request" will need to be created. This process is a way for Committers to see a summary of the suggested changes to be made with any helpful comments and a catalog of potential differences between the original files and the new ones.

If you run into trouble, this Github.com support page may be of use: [https://help.github.com/articles/creating-a-pull-request/](https://help.github.com/articles/creating-a-pull-request/)

* On GitHub, navigate to the main page of the ISLE repository.

* In the "Branch" menu, choose the branch that contains your commits e.g. `githubusername-docs-fix2`

* To the right of the Branch menu, click `New pull request`.

* Use the base branch dropdown menu to select the branch you'd like to merge your changes into (`master`), then use the compare branch drop-down menu to choose the topic branch you made your changes in.(`githubusername-docs-fix2`)

  * `base:master` <--- `compare:githubusername-docs-fix2`

* Type a title and description for your pull request.

* Click the green `Create pull request` button at the bottom.

This pull request should now appear in the [Pull Requests](https://github.com/Islandora-Collaboration-Group/ISLE/pulls) section of the ISLE repository, ready for review, commentary or inclusion by the ISLE Committers.
