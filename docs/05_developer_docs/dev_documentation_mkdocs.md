### Assumptions / Prerequisites

* Python 2.7+ or higher is installed on the enduser's laptop / workstation
* The ISLE Documentation project directory has been cloned to the enduser's laptop / workstation in an appropriate Code or Project Directory.
   * `git clone https://github.com/Islandora-Collaboration-Group/ISLE-Documentation.git`
* Familiarity or comfortability with git and cli commands.

### Step 1. Install mkdocs

* Please follow the instructions found here: [http://www.mkdocs.org/#installation](http://www.mkdocs.org/#installation)

**Example: (All OS)**

* `sudo -H pip install mkdocs`

---

### Step 2. Install the mkdocs-material theme

The ISLE Documentation Theme can be found here: [https://github.com/squidfunk/mkdocs-material](https://github.com/squidfunk/mkdocs-material)

**Example: (All OS)**

* sudo -H pip install mkdocs-material

---

### Step 3. Run the mkdocs software

* Open a terminal and `cd to pathto/ISLE-Documentation project directory`
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
* Use the terminal output for debugging issues or missing files, links etc.
* To stop `mkdocs`, click the `Cntrl` and `c` key at the same time. `(Cntrl-C)`
