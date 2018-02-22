### Contribute

If you would like to contribute code or documentation to the project, please review the following suggested process below.

Assumption: You are comfortable with Git.

* After cloning this repo to your local laptop / workstation.

* Create a new working git branch

**Example only**

```
$ git checkout -b newbranchname
Switched to a new branch 'newbranchname'
```

* Make your changes to the appropriate files or area

* Once finished, add the edited file(s)

**Example only**

```
$ git add filename
```

* Commit the added files with an appropriate commit message explaining the change and impacted file(s).

**Example only**

```
$ git commit -m "I've updated the filename with these changes."
```

* Push the file to ISLE project.

**Example only**

```
git push origin newbranchname
```

* Navigate to https://github.com/Islandora-Collaboration-Group/ISLE in a browser
 * Click on the `New pull request` button (next to the Branch drop down list within the <>Code section)
 * The `base: master` should be on the left, select the `compare` dropdown and choose your branchname
 * The interface should now display if there are any potential conflicts within the pull request.
 * If there are no conflicts, go ahead and click the green `Create pull request` button.

* The ISLE maintainer and documentation groups will now review and discuss prior to approval. Should there be further questions or concerns, the team will respond directly on the pull request. Or they'll simply approve it and merge the code or documentation.
