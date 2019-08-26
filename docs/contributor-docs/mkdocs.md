# Documentation: MkDocs

## Assumptions / Prerequisites

* Python 2.7+ or higher is installed on the end user's personal computer
* The ISLE project directory has been cloned to the end user's personal computer in an appropriate Code or Project Directory.
   * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`
* Familiarity or comfortability with git and cli commands.

## Step 1. Install `mkdocs`

* Please follow the instructions found here: [http://www.mkdocs.org/#installation](http://www.mkdocs.org/#installation)

	* Mac/Unix: `sudo -H pip install mkdocs`
	* Windows: `pip install mkdocs` (PowerShell: Run as administrator)

---

## Step 2. Install the `mkdocs-material` Theme

* The ISLE Documentation Theme can be found here: [https://github.com/squidfunk/mkdocs-material](https://github.com/squidfunk/mkdocs-material)

	* Mac/Unix: `sudo -H pip install mkdocs-material`
	* Windows OS: `pip install mkdocs-material` (PowerShell: Run as administrator)

---

## Step 3. Run the `mkdocs` Software

* Open a terminal and `cd to pathto/ISLE project directory`
* `mkdocs serve`

```
INFO    -  Building documentation...
INFO    -  Cleaning site directory
[I 180310 15:50:39 server:283] Serving on http://127.0.0.1:8000
[I 180310 15:50:39 handlers:60] Start watching changes
[I 180310 15:50:39 handlers:62] Start detecting changes
[I 180310 15:50:42 handlers:133] Browser Connected: http://127.0.0.1:8000/
```

* Open up a browser and navigate to [http://127.0.0.1:8000/](http://127.0.0.1:8000/)
* It is critical for this terminal to keep running
    * as mkdocs will report file changes and any potential issues.
    * To review changes in real time via the browser
* Use the terminal output for debugging issues or missing files, links, etc.
* To stop `mkdocs`, click the `Cntrl` and `c` key at the same time. `(Cntrl-C)`

---

## How to Upgrade `mkdocs` version

* [MkDocs Release Notes](https://www.mkdocs.org/about/release-notes/#release-notes)
* To upgrade MkDocs to the latest version, use pip:
  `pip install -U mkdocs`
* You can determine your currently installed version using
  `mkdocs --version`
