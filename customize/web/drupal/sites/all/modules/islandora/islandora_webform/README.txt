Description
-----------
This module permits a site manager to create webforms that are associated with
islandora objects. A user will see a link on the object page to inviting him
or her to contribute information pertaining to this object. Once the user has
submitted this information, repository curators can review all submissions for
a given webform, or for a given repository object.

The Islandora Webform Ingest module, which is provided with this module and can
be enabled separately, provides digital repository curators the ability to directly
ingest webform submissions into a datastream of the associated object.

Online documentation
----------------------
For more complete documentation of how to use islandora_webform and islandora_webform_ingest modules;
see https://sites.google.com/site/islandoraconsortiagroup/home/iw-module

Requirements
------------
Drupal 7.x
Webform 7.4.x
Islandora 7.x with Islandora Collections and at least one Solution Pack enabled.


Installation
------------
1. Copy the entire islandora_webform directory into the Drupal sites/all/modules
   directory (or subdirectory).

2. Login as an administrator. Enable the Islandora Webform module in the
   "Administer" -> "Modules"

3. Create a webform node at node/add/webform.

4. Configure Islandora settings for the webform to define which repository objects
   can show a link to this webform.

5. If manual webform linking is selected, go to each object that you want to link
   to a webform, and click the link to add it.


Configuration
-------------
Configuration is found at Islandora > Configuration > Islandora Webform (/admin/islandora/configure/iw).

1. "Webform link behavior": If not using inline webforms with webform_ajax, this
   controls whether clicking on a webform link opens a new page/tab or replaces
   the current window.
2. "Append webform links to bottom of object view?": Uncheck this if you want to
   override the normal positioning of the webform links below the object display.


Permissions
-----------------------------
This module defines the following permissions:
1. Manage Islandora Webform - enable for roles that should be able
   to add webform links to objects generally.
2. Link islandora objects to webforms - when manual webform linking is selected,
   enable for roles to add webform links on individual objects.



SOLR/GSearch Configuration
--------------------------
When a Fedora object is manually linked to a webform, enabling the link to appear
on the object page, a new datastream "WF" is created on the object, containing a
list of the node ids of all associated webforms. In order for a site builder to
make a view of all objects that are associated with a given webform, SOLR and
GSearch need to be configured to index the contents of that datastream.
TODO Instructions for how to do this are needed!


Enabling and configuring Islandora Webform Ingest
-------------------------------------------------
See the README.txt inside the submodules/islandora_webform_ingest folder for
installation and usage instructions for the Islandora Webform Ingest module.


Upgrading from previous versions
--------------------------------

1. MAKE A DATABASE BACKUP. Upgrading this module may entail a number of database
   changes. If you encounter an error and need to downgrade, you must
   restore the previous database. You can make a database backup with your
   hosting provider, using the Backup and Migrate module, or from the command
   line.

2. Copy the entire islandora_webform directory the Drupal modules directory, replacing
   the old copy. DO NOT KEEP THE OLD COPY in the same directory or
   anywhere Drupal could possibily find it. Delete it from the server.

3. Login as an administrative user or change the $update_free_access in
   update.php to TRUE.

4. Run update.php (at http://www.example.com/update.php).

Support
-------
TODO

Credits and thanks
------------------
TODO
