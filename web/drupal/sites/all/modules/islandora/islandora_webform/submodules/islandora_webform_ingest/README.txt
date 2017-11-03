Description
-----------
This module extends the functionality of the Islandora Webform module to
provide digital repository curators the ability to ingest data collected from
webform submissions. This data may be ingested into the islandora object that
is the subject of the webform submission. Or it may be used to create a new
object which is related to the original object. Related objects can be displayed
in a block along with the object.


Requirements
------------
Islandora Webform module 7.x
(See Islandora Webform module for further requirements)


Installation and Configuration
------------------------------
1. See installation instructions for Islandora Webform module if not already
   installed and configured.

2. Login as an administrator. Enable the Islandora Webform Ingest module at
   "Administer" -> "Modules".

3. Go to Islandora Settings for a webform. In addition to the settings provided
   by the Islandora Webform module, Islandora Ingest settings are added. These
   permit you to enable ingest, and to configure other ingest settings: the
   destination for the ingested data (existing object or new object); in the
   case of new object, its content model, relationship to the existing object,
   and namespace.
   NOTE that if you will be ingesting to the current object, a content model must
   be selected in the Islandora Settings section above.
   NOTE also that any changes in the destination content model will cause any
   previously saved ingest mappings for this form (see next) to be lost.

4. For each webform component that you wish to serve as a source of ingested
   data, edit the component and expand the "Islandora Ingest Mapping" section of
   the form. Under "Ingest?", select either "Append" or "Replace" (see detailed
   explanation below). Then you can select from the list of compatible
   destination datastreams for this component. If the datastream uses a form
   (e.g. MODS), select the field on that form that you wish to ingest to.
   "File" components can only be mapped to binary datastreams with compatible mime
   types.

5. If you are ingesting to new objects, go to "Administer" -> "Structure"
   -> "Blocks" and configure the block corresponding to the relationship you
   defined above (in step #3). In addition to setting which region on the page
   the related blocks should appear in, you can set the block title, the format
   of the  objects (grid or list with thumbnails, or links), how they should be
   sorted, and how many should appear in the block.


Permissions
-----------------------------
This module defines the following permissions:
1. Ingest Islandora Webform Submissions - enable for roles that should be able
   to ingest webform submissions.


Ingesting Webform Submissions
-----------------------------
1. Once you have received some webform submissions, you will see them listed under the
   "Submissions" tab for the object that the submissions were in reference to. At the
   right side of this table, an "Ingest" column has been added, and if a submission
   can be ingested, an "Ingest" link will appear there.

2. Clicking on the "Ingest" link will bring up a page that displays the datastreams that
   were configured as targets for webform values, with those values entered. If
   a destination datastream provides an edit form (e.g. MODS), then that form will
   be displayed, and you can edit the values shown there. Once you are satisfied with
   the content and metadata, you can click "Ingest this Submission". This will cause
   the data to be ingested, either to the original object, or to a new object. If
   ingested to a new object, it will be created with a rels-ext relation to the
   original object.

3. After a submission has been ingested once, the link in the Submissions table
   will change to "Re-ingest". If the ingestion is configured to update the existing
   object, then "Append" and "Replace" will work as you might expect, except that
   when re-ingesting, there are already values in the mapped fields. So if you
   repeatedly re-ingest to an existing object, you may find multi-value fields and
   tab sets multiplying! If the ingestion is configured to create a new object, then



Behavior of "Append" and "Replace"
----------------------------------
"Append" and "Replace" affect what happens when you write text to a datastream on the
destination object, whether it is a new object or the existing object. If there is
already data in that datastream, or in the case of an xml datastream in the field,
then the submitted text will append to or replace the existing text. If the xml field
is a multi-value field, "Append" will cause a new value to be created. If the xml
field is in a tab set, then "Append" will cause one new tab to be created.


More about related objects
--------------------------
1. Related objects created during ingestion use rels-ext relations that you define.
   There is limited built-in support in Islandora for displaying and manipulating objects
   that have arbitrary relationships. The object that the ingested objects reference
   is not a collection, but it has a collection-like relationship to the ingested
   objects. As already mentioned, the related objects block lets you see the "children"
   for a particular object. But since it is not a collection, the "Collection" tab
   is not available for managing these children. You have to manage the child objects
   one at a time, by clicking their "Manage" tab (move to collections, delete, etc).

2. When a "parent" object is deleted, the "child" objects will be deleted too (the
   corresponding webform submission is not deleted).

3. If you change the relation name in the webform's islandora configuration page, existing
   objects with the old relation name will be updated to use the new relation name, and the
   related objects block will be updated as well. However, if you set the relation name to
   blank (you must also disable ingest), then the child objects will lose their relation
   to the original object. In the future we will try to improve this, perhaps providing an
   "unrelated objects" management page.


Theming the related objects block
---------------------------------
See the README.txt file in islandora_webform_ingest/examples/theme_related_objects_block
for an example of how to theme this block to, for example, show the contents of
selected MODS fields.


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