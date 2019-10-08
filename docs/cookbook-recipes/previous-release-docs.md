<!--- PAGE_TITLE --->

# Using Documentation for an Earlier Release

The documentation at https://islandora-collaboration-group.github.io/ISLE/ will always correspond to the latest release.  While the ISLE maintainers suggest always running the latest release to ensure the most up-to-date production environment, some situations may require a user to need to review or follow the documentation for a prior release.  When a previous release is checked out of git (ex: `git checkout ISLE-1.2.0`) the documentation for that release is included in the repository in the /docs directory.  This page assumes you have already [cloned the ISLE repository locally](/install/install-local-new/#step-2-setup-git-project-repositories).

Documentation can be viewed a few ways:

  - The raw markdown text files can be viewed directly.
  - Many text editors include a markdown viewer, or one can be installed as an add-on.  This will interpret the markdown, so formatting will be correct and embedded links should work.  This method is simple, but because the ISLE pages are compiled with MkDocs, some formatting will differ from the original, and the navigation menu will not be rendered at all.
  - **Preferred** MkDocs can be installed and run locally.  Documentation pages will appear exactly as they do on the GitHub.io site, and the navigation menu will function properly.  Importantly, some obsolete pages may still be in the /docs directory but will not be linked to on the MkDocs version which can help avoid confusion. Read further for instructions for using MkDocs.

## About MkDocs

MkDocs performs two basic functions:  
  - It uses a theme template and YAML file to generate static HTML pages.
  - It functions as a local web server so the resulting HTML pages can be viewed in a browser.

## Prerequisites

  - MkDocs requires Python.  The [MkDocs website](https://www.mkdocs.org/) has good documentation on how to set up Python and is a good place to start, especially Windows users as it contains useful advice necessary to make the commands below work smoothly.  MkDocs generally runs well on Windows, MacOSX and Linux.
  - Once MkDocs is installed and working properly, install the Material theme:
    - `pip install mkdocs-material`
    - or `pip install mkdocs-material --user` if you get permissions errors on Linux

## Running MkDocs

  - In a terminal (PowerShell or Git Bash in Windows), cd to the directory you have cloned ISLE into:
    - `cd /path/to/ISLE`
    - Run the MkDocs server:
    - `mkdocs serve`
    - Windows users may need to run `python -m mkdocs serve` as described on the MkDocs website.
  - If the command runs successfully, you will see something like the following:

```
   $ mkdocs serve 
   INFO    -  Building documentation...
   INFO    -  Cleaning site directory 
   INFO    -  The following pages exist in the docs directory, but are not included in the "nav" configuration:
     - about/project-history.md
     - appendices/environment-files.md
     - contributor-docs/ISLE-v.1.1.2-buildnotes.md
     - contributor-docs/ansible-guide.md
     - contributor-docs/build-guide-v111.md
     - contributor-docs/build-guide.md
     - contributor-docs/editing-local.md
     - contributor-docs/editing-online.md
     - contributor-docs/making-pr-guide.md
     - contributor-docs/overview.md
     - contributor-docs/policies.md
     - cookbook-recipes/preview-release-docs.md
     - install/_obsolete_install-server.md
     - install/install-demo-edit-hosts-file.md
     - install/install-demo-resources.md
     - install/install-local-resources.md
     - specifications/diagram-network.md
     - specifications/docker-containers-images.md
     - specifications/dockerfiles.md
     - specifications/resource-matrix.md
     - specifications/supported-drupal-modules-matrix.md
     - specifications/supported-software-matrix.md 
   [I 191008 12:04:07 server:296] Serving on http://127.0.0.1:8000
   [I 191008 12:04:07 handlers:62] Start watching changes
   [I 191008 12:04:07 handlers:64] Start detecting changes
```

 - You can safely ignore the INFO about pages not in the "nav" configuration.  The important piece of information here is the local web address for the site, usually [http://127.0.0.1:8000](http://127.0.0.1:8000). Paste that into your browser to view the documentation.