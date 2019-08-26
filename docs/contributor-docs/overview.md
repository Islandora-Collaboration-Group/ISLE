<!--- PAGE_TITLE --->

The ISLE project uses `mkdocs` a static site generator designed for building project documentation. Documentation source files are written in `Markdown`, and configured with a single `YAML` configuration file. The `mkdocs` software then creates static `HTML` pages for hosting on GitHub Pages. This software can also be used locally on an end user's personal computer for review and editing of materials.

Please note there are two key elements to the ISLE Documentation.

1. ISLE Documentation Github.com Pages **(static HTML):**  
    * Used by endusers for Documentation
    * Clean, crisp nicely formatted hosted ISLE Documentation with navigation.
    * [https://islandora-collaboration-group.github.io/ISLE/](https://islandora-collaboration-group.github.io/ISLE/)


2. ISLE Documentation Github.com Repository **(Markdown files):**  
    * Used by ISLE Committers to create and manage ISLE Documentation
    * `Markdown` files (`.md`) stored in an online git repository that when rendered by the `mkdocs` software produce the above mentioned nicely formatted static HTML pages.
    * [https://github.com/Islandora-Collaboration-Group/ISLE](https://github.com/Islandora-Collaboration-Group/ISLE)


_Please refer to the **Documentation** section of the [Glossary](../appendices/glossary.md) if terms used are not clear._


A very handy primer for writing documentation using `MkDocs` has been written here: [http://www.mkdocs.org/user-guide/writing-your-docs/](http://www.mkdocs.org/user-guide/writing-your-docs/)

---

## Project Directory Structure

```

├── docs
│   ├── about
│   ├── appendices
│   ├── assets
│   ├── contributor-docs
│   ├── cookbook-recipes
│   ├── install
│   ├── migrate
│   ├── release-notes
│   ├── specifications
│   ├── update
│   ├── extra.css
│   ├── extra.js
│   └── index.md
├── /.aspell.en.pws
├── .gitignore
├── mkdocs.yml
├── README.md

```

| Directory or file | Description |
| -------------     | ------------- |
| README.md | Description of the documentation project contents and how to use them |
| docs | where all documentation sections and topical pages are kept |
| about-isle.md | Description of the ISLE project, its contributors and this documentation repository |
| assets | Image or logo files used in documentation |
| glossary.md | A list of terms used in documentation |
| index.md | The homepage (first page of the documentation |
| mkdocs.yml | YAML file that acts as the section and page directory |

### Sections and Pages

Within the `docs` directory, there are 6 major topical sections.

* All sections are have numbered prefixes e.g. `01_` , `02_` to indicate order of documentation progression or narrative.

* Sections contain relevant topics and pages.

* All pages have relevant prefixes to their sections.

* Page file names may differ from their Page titles in the `mkdocs.yml` file.

* All topic of the page short and concise to one or two keywords  

* Please keep all filenames lowercase

* Please ensure that all files are Markdown (`.md`) files otherwise the documentation system will fail.
