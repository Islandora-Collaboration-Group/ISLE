# ISLE Documentation: Style Guide

## MkDocs Configurations
- Site Theme: [MdDocs Themes](https://www.mkdocs.org/user-guide/styling-your-docs/#built-in-themes)
- CSS Overrides: The `extra.css` file overrides some basic CSS styles.
- Javascript Overrides: The `extra.js` file (could) override some basic JS styles.

---

## Files
- Files use dashes, not underscores, and are lowercase.
- Example: `host-hardware-requirements.md`

---

## URLs
- Hyperlinks use the title of their destination page as the text of the URL.
- Example: [Hardware Requirements](../install/host-hardware-requirements.md)

---

## Tables
Sections within a section that is specific to a certain type of circumstance should be put in a table:

| Example Subheader |
| :-------------      |
| Instructions here: |
| - We use this dash since `mkdocs` cannot accomodate true bullets inside tables. |
| - Another dash and another line. |

---

## Headers: Follow the APA Style Guide for Capitalization of All Important Words.
- Solution: Sublime Text 3, install and use this package: `Smart Title Case.sublime-package`

### **Example:**
- H1: Page Title
- H2: Main Subheaders
- H3: Copy Production Data to Your Local

---

## Bullets and Indentations
### **Example:**
Mkdocs is finicky and requires double indentations to create properly indented sub-bullets.

* Item A
    * Item B
    	* Item C

---

## Terms We Use
- personal computer (**we use a generic term and NOT specific variants like:** laptop, workstation, desktop computer, or tablet, etc.)
- repository (not "repo")
- end users (not endusers)
- Demo ISLE Installation
- Local ISLE Installation: New Site
- Local ISLE Installation: Migrate Existing Islandora Site
- Staging ISLE Installation: New Site
- Staging ISLE Installation: Migrate Existing Islandora Site
- Production ISLE Installation: New Site
- Production ISLE Installation: Migrate Existing Islandora Site
- Update ISLE

---
