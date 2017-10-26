# How to edit ISLE Documentation

Thanks in advance for contributions to the ISLE Documentation.

The Islandora Community, the ISLE Team and its stakeholders greatly appreciates all contributor's time and efforts in creating, editing, revising and growing this collected documentation.

The ISLE Team asks that all potential contributors please familiarize themselves with the [Documentation and Style Guide](dev-docs-styleguide.md) and its requirements **PRIOR** to committing documentation changes.

There are two different ways one can edit or create documentation for the ISLE project.

**Please note:** One will need to have signed up for a Github.com account [here](https://github.com/join?source=header-home) prior to attempting either of these two methods.


#### Method 1: Edit page on Github.com

1. Open a web browser and navigate to the ISLE project [repository](https://github.com/Islandora-Collaboration-Group/ISLE/) on Github.com
2. Click on the `docs` directory
3. Navigate to and click on the desired `.md` file (documentation page) for editing
4. Locate and click on the `pencil` icon on the right hand side of the page. (To the right of Raw | Blame | History etc)
5. The page should now be ready for editing and the `Edit file` tab should display at the top. Make changes as needed.
6. To save changes for review by the ISLE project team, simply scroll down to the bottom of the page to the `Commit Changes` section.
7. Within the first empty field, enter a comment like `Edited filename.md with changes to ...` (The key is one sentence with a very minimal description)
8. Within the second empty field, if needed enter a more verbose list of comments with changes.
9. Click the second radio button `Create a new branch for this commit and start a pull request.` **DO NOT CLICK THE 1ST BUTTON PLEASE!**
10. Upon clicking the second radio button, there should be a new field created with an entry e.g. `githubusername-patch-1`
11. Click the green `Propose file change` button
12. A new page opens with the title `Open a pull request`. The previous information from the new branch process should now populate the new fields
13. Click the green `Create pull request` and now the documentation changes have been submitted for review.
14. Assigned ISLE  project members will review, approve or comment on the pull request as time allows.

---

#### Method 2: Edit page via local Mkdocs workflow

1. Please follow the [Documentation Requirements](dev-mkdocs-requirements) page to install Mkdocs and the appropriate `material` theme
2. Open up a terminal and navigate to the ILSE project directory e.g. `cd ~/ISLE`
3. To start Mkdocs: `mkdocs serve`
4. One can view the static pages by opening a browser and navigating to `http://127.0.0.1:8000`
5. Checkout a new branch in git e.g. `git checkout -b new_branch`
6. Edit the appropriate Markdown (.md) file within the `docs` directory and save the edited file. Changes are almost immediate, the page should refresh. Should errors occur and the page does not display check the terminal output for issues.
7. Once ready to push changes to the ISLE repository:  `git add edited-documentation-page.md`  
8. `git commit -m "Your description of changes, why and affected files if possible"`  
9. `git push origin new_branch`  
10. Manually create a pull request on the Github page for the ISLE [repository](https://github.com/Islandora-Collaboration-Group/ISLE/)  
11. Assigned ISLE  project members will review, approve or comment on the pull request as time allows.
