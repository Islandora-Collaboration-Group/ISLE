Description
-----------
This module adds AJAX support to Webform (3.0 and above).
As a first step it focusses to simply add form AJAX paging and submit.
Especially for multipage forms, this leads to faster loading (less data transfer),
better usability and finally smaller server load.

Requirements
------------
Drupal 7.x
Webform 3.17 or newer.

Installation
------------
1. Copy the entire webform_ajax directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Create or edit a webform node and check "AJAX mode" at node/%nid/webform/configure.

Upgrading from previous versions
--------------------------------
There is no upgrade path from Drupal 6. You just have to set the "AJAX mode"
property on each webform.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/webform_ajax

