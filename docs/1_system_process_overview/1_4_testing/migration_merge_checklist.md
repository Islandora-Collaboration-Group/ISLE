### Migration Merge Checklist

This section is to serve as a new checklist for the editing or merging of the copied Islandora Production server(s) data and config files to the appropriate config directory on the new ISLE Host Server.

The suggested workflow is for endusers to review the Production file(s) first, make note of any settings and then make appropriate edits within `/config/enduser-renamed-directory.institution/` to change values, add passwords or usernames etc unless otherwise directed e.g. Apache `html` and Fedora `data`.

While this checklist will attempt to point out most of the merge challenges or pitfalls, ISLE assumes no responsibility or liability in this matter should an enduser have customizations beyond what this guide outlines.

**Please note:**

* In most cases, many of the configuration files (copied from `isle-prod-project.institution` to `enduser-renamed-directory.institution`) will have comments (#) in them to help guide endusers to make the appropriate edits e.g. (# enduser edit here)

* In most cases, many of the configuration files (copied from `isle-prod-project.institution` to `enduser-renamed-directory.institution`) will have fake or empty settings in them. Please remove, edit or enter new values as advised.

### Assumptions / Prerequisites
* The enduser has completed Steps 1 -4 of the [Migration Guide](migration_guide.md).
    * Includes the steps outlined within the [Migration Export Checklist](migration_export_checklist.md) to copy over production data and configuration files.

* The enduser is currently on Step 5 of the [Migration Guide](migration_guide.md)

---
#### Apache

Compare, edit, merge or copy the following from the suggested directory `/path_to/islandora_production_data_storage/apache/` on the ISLE Host server to either:

* `/opt/isle/config/enduser-renamed-directory-prod.institution/apache/`

* `/path_to/isle_production_data_storage/enduser-renamed-directory-dev.institution`

This data will be used in conjunction with an Apache container.

| Data          | Description                 | Production Data Copy Location on ISLE Host Server  | Merge, Copy or Edit Location / Destination            | Notes         |
| ------------- | -------------               | -------------                                      | -------------                                         | ------------- |
| html          | Islandora/Drupal Website    | /path_to/islandora_production_data_storage/apache/ | /path_to/isle_production_data_storage/enduser-renamed-directory-dev.institution/apache/ | _see below_   |
| settings.php  | Drupal settings.php file    | /path_to/islandora_production_data_storage/apache/ | /config/enduser-renamed-directory.institution/apache/ | _see below_   |
| site.conf     | Apache webserver vhost file | /path_to/islandora_production_data_storage/apache/ | /config/enduser-renamed-directory.institution/apache/ | _see below_   |

* `html` - endusers will **COPY** this entire directory **instead** to `/path_to/isle_production_data_storage/enduser-renamed-directory-dev.institution/apache/html`

* `settings.php` - endusers will want to edit database and user names for Drupal sites to connect properly.

    * Lines 251 -253: Change the appropriate settings for the Drupal website database, associate database user name and password. Do not change the `host` settings.  

    * Line 288: _Recommend adding a Drupal hash value here of 25+ alpha-numeric characters_

    * Line 312: `$base_url` should be the URL of the ISLE production Drupal website e.g. `https://site.institution.extension`

* `site.conf` - endusers will edit this file as required to setup the Apache webserver on the Apache container.

    * The filename can stay the same or can be changed. Please note that Line 85 of the associated `docker-compose.yml` will need to be updated if the file is renamed

    * Lines 2, 3, 6, 7, 34 - 37, 39, 40, 44 & 45:  endusers will edit this file as directed in the (# enduser please edit this and then remove this comment) instructions.

       * Do remove (# enduser please edit this and then remove this comment) after editing or adding the appropriate values.

#### Apache Optional Edits

| Data          | Description                 | Production Data Copy Location on ISLE Host Server  | Merge, Copy or Edit Location / Destination            | Notes         |
| ------------- | -------------               | -------------                                      | -------------                                         | ------------- |
| php.ini       | PHP configuration file      | /path_to/islandora_production_data_storage/apache/ | /config/enduser-renamed-directory.institution/apache/ | _see below_   |

* `php.ini` - endusers can make appropriate edits within `/config/enduser-renamed-directory.institution/apache/php.ini` to increase the upload settings, memory etc. as needed. Otherwise leaving the default values should work.

* **Please note:** an additional line will have to be added to the associated `docker-compose.yml` in the Apache `volumes:` section for this edit to work e.g. `- ./apache/php.ini:/etc/php.ini`

| Data          | Description                 | Production Data Copy Location on ISLE Host Server  | Merge, Copy or Edit Location / Destination            | Notes         |
| ------------- | -------------               | -------------                                      | -------------                                         | ------------- |
| tmpreaper     | Cronjob for tmpreaper       | /path_to/islandora_production_data_storage/apache/ | /config/enduser-renamed-directory.institution/apache/ | _see below_   |

* `tmpreaper` - (optional) endusers may want to edit this tmpreaper cron job for different locations and/or times. The `docker-compose.yml` file will need an associated bind-mount for this change.

* **Please note:** an additional line will have to be added to the associated `docker-compose.yml` in the Apache `volumes:` section for this edit to work e.g. `- ./apache/tmpreaper/cron:/etc/cron.d/tmpreaper-cron`


#### Fedora


Compare, edit, merge or copy the following from the suggested directory `/path_to/islandora_production_data_storage/fedora/` on the ISLE Host server to either:

* `/opt/isle/config/enduser-renamed-directory-prod.institution/fedora/`

* `/path_to/isle_production_data_storage/enduser-renamed-directory-dev.institution/fedora/`

| Data              | Description                   | Possible Location                | Suggested Destination                      | Notes         |
| -------------     | -------------                 | -------------                    | -------------                              | ------------- |
| data              | Entire Fedora data directory  | /usr/local/fedora/             | /path_to/isle_production_data_storage/enduser-renamed-directory-dev.institution/fedora/ | _see below_ |
| fedora.fcfg       | Fedora repository config file | /usr/local/fedora/server/config/ | /islandora_production_data_storage/fedora/ | _see below_   |
| fedora-users.xml  | Fedora users config file      | /usr/local/fedora/server/config/ | /islandora_production_data_storage/fedora/ | _see below_   |
| filter-drupal.xml | Fedora Drupal filter file     | /usr/local/fedora/server/config/ | /islandora_production_data_storage/fedora/ | _see below_   |
| repository-policies | Fedora Drupal filter file     | /usr/local/fedora/server/config/ | /islandora_production_data_storage/fedora/ | _see below_   |
| site.conf         | Apache webserver vhost file   | /etc/apache2/sites-available/    | /islandora_production_data_storage/fedora/ | _see below_   |

**Fedora Notes**:

* `data` - endusers will **COPY** this entire directory **instead** to `/path_to/isle_production_data_storage/enduser-renamed-directory-dev.institution/fedora/data`

* `fedora.fcfg` - endusers will want to edit the following:
    * Line: 562 (optional) to change the `fedora_admin` username for the `fedora3` database

    * Line: 598 (necessary) to enter the `fedora_admin` user password for the `fedora3` database

* `fedora-users.xml` - endusers will want to edit the following:

    * Lines 3, 8, 14 and 19: Add the appropriate passwords or users as needed.  

* `filter-drupal.xml` - endusers will want to edit

    * Line 3: Add the appropriate Drupal site database name, Drupal site database user and Drupal site database user password in between all `""`

    * **Please note:** For endusers using Drupal multi-sites, please add additional sites as guided in the example below

    **Example**
```
<?xml version="1.0" encoding="UTF-8"?>
<FilterDrupal_Connection>

    <connection server="mysql" port="3306" dbname="drupalsite1" user="drupalsite1_user" password="drupalsite1_user_pw">
    <sql>
       SELECT DISTINCT u.uid AS userid, u.name AS Name, u.pass AS Pass,r.name AS Role FROM (users u LEFT JOIN users_roles ON u.uid=users_roles.uid) LEFT JOIN role r ON r.rid=users_roles.rid WHERE u.name=? AND u.pass=?;
    </sql>
    </connection>

    <connection server="mysql" port="3306" dbname="drupalsite2" user="drupalsite2_user" password="drupalsite2_user_pw">
    <sql>
       SELECT DISTINCT u.uid AS userid, u.name AS Name, u.pass AS Pass,r.name AS Role FROM (users u LEFT JOIN users_roles ON u.uid=users_roles.uid) LEFT JOIN role r ON r.rid=users_roles.rid WHERE u.name=? AND u.pass=?;
    </sql>
    </connection>

</FilterDrupal_Connection>
```


* `fedora/repository-policies` - endusers can edit the files contained within for more granular or customized Fedora user permissions or repository access.

* `site.conf` - endusers will want to edit this Apache vhost for the fedora proxy site name (if using Djatoka)

    * The filename can stay the same or can be changed. Please note that Line 37 of the associated `docker-compose.yml` will need to be updated if the file is renamed

    * Lines 2, 3, 6, 7, 34 - 37, 39, 40, 44 & 45:  endusers will edit this file as directed in the (# enduser please edit this and then remove this comment) instructions.

       * Do remove (# enduser please edit this and then remove this comment) after editing or adding the appropriate values.


#### Solr

       Copy the following below from the Islandora Production Server(s) to the suggested directory `/path_to/islandora_production_data_storage/solr/` on the ISLE Host server.
       This data will be used in conjunction with an Solr container.
       | Data           | Description               | Possible Location        | Suggested ISLE Path Destination          | Notes         |
       | -------------  | -------------             | -------------            | -------------                            | ------------- |
       | schema.xml     | Solr index config file    | ../solr/collection1/conf | /islandora_production_data_storage/solr/ | _see below_   |
       | solrconfig.xml | Solr config file          | ../solr/collection1/conf | /islandora_production_data_storage/solr/ | _see below_   |
       | solr.xml       | Solr config file file     | /opt/solr/               | /islandora_production_data_storage/solr/ | _see below_   |
       | stopwords.txt  | solr webserver vhost file | ../solr/collection1/conf | /islandora_production_data_storage/solr/ | _see below_   |

**Solr Notes**:

* `schema.xml`
   * _Usually the first file one configures when setting up a new Solr installation_
   * The schema declares
     * what kinds of fields there are
     * which field should be used as the unique/primary key
     * which fields are required
     * how to index and search each field
* `solrconfig.xml`
   * _The solrconfig.xml file is the configuration file with the most parameters affecting Solr itself._
   * In solrconfig.xml, one can configure the following:
     * request handlers, which process requests to Solr, e.g. requests to add documents to the index or requests to return results for a query
     * processes that "listen" for particular query-related events; listeners can be used to trigger the execution of special code
     * the Request Dispatcher for managing HTTP communications
     * the Admin Web interface
     * parameters related to replication and duplication
* `solr.xml`
  * _The solr.xml file defines global configuration options that apply to all or many cores._
* `stopwords.txt`
  * _Using the stopwords.txt file, one can avoid the common words of a language, which do not add a significant value to any search.
  * _For example, a, an, the, you, I, am, and so on. One can specify words to be removed from the Solr search in this file line-by-line._

* Additional Locations:
  * /opt/solr
  * /usr/local/solr
  * /var/lib/tomcat7/webapps/solr
  * /usr/share/tomcat/webapps/solr
