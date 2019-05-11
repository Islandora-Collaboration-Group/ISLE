# Migration Merge Checklist

This section is to serve as a new checklist for the editing or merging of the copied Islandora Production server(s) data and config files to the appropriate config directory on the enduser's local laptop and ultimately copied / deployed back to the new ISLE Host Server.

The suggested workflow is for endusers to review the Production file(s) first, make note of any settings and then make appropriate edits within the `yourdomain-config` directory to change values, add passwords or usernames etc on your local laptop with the ultimate goal of checking all results into a git repository for deploy later on the ISLE Host server.

The only change is that unless otherwise directed e.g. Apache `html` and Fedora `data`. all changes should be made on the local laptop in the `yourdomain-config` directory.

Please note as per the migration guide instructions the name of this directory shouldn't literally be "yourdomain-config" or "yourdomain-data" so replace "yourdomain" with the name of your intended Islandora site's domain.

While this checklist will attempt to point out most of the merge challenges or pitfalls, you may encounter unique situations depending on the edits and customizations made to your Islandora environment in the past. This is a good place to reach out to the Islandora community for assistance.

**Please note:**

* In some cases, some of the configuration files copied from your running production Islandora may have comments (#) in them to help guide endusers to make the appropriate edits e.g. (# end user edit here)

* In most cases, many of the configuration files copied from ISLE repository to `yourdomain-config` will have fake or empty settings in them. Please remove, edit or enter new values as advised.

---
## Apache

Compare, edit, merge or copy the following from the source directory `current-production-config/apache/` to:

* `yourdomain-config/apache/` on your local laptop.

| Data          | Description                 | Production Data Copy              | Merge, Copy or Edit Location / Destination | Copy location            |
| ------------- | -------------               | -------------                     | -------------                              | -------------            |
| html          | Islandora/Drupal Website    | current-production-config/apache/ | yourdomain-data/apache/                    | Remote ISLE Host server  |
| settings.php  | Drupal settings.php file    | current-production-config/apache/ | yourdomain-config/apache/                  | Local ISLE config laptop |
| site.conf     | Apache webserver vhost file | current-production-config/apache/ | reference file but do not copy             | Local ISLE config laptop |


### Apache Edits

* `html` - endusers will have **copied** this entire directory **instead** to a new directory called `yourdomain-data/apache/html/` on your remote ISLE host server in the appropriate storage area.

* `settings.php` - endusers will want to edit database and user names for Drupal sites to connect properly.

    * Lines 251 -253: Change the appropriate settings for the Drupal website database, associate database user name and password. Do not change the `host` settings.  

    * Line 288: _Recommend adding a Drupal hash value here of 25+ alpha-numeric characters_

    * Line 312: `$base_url` should be commented out as it isn't used due to the proxy.

### Apache - Sites-Enabled

Please note that endusers will take values from the `site.conf` file and flow the information as needed into the to be renamed `newsite-sample-ssl.conf` & `newsite-sample.conf` files accordingly with the domain name of your choice. This file will not be copied to yourdomain-config/apache/ for any usage.

* Within the `sites-enabled` directory, rename the files `newsite-sample-ssl.conf` and `newsite-sample.conf` to your domain names - example:
    * `digital-collections.example.edu_ssl.conf`

    * `digital-collections.example.edu.conf`

* Edit the previously named `newsite-sample.conf` file and change lines 3 and 4 to point to the location of your apache logs on the container - example:

    * `ErrorLog /var/log/apache2/digital-collections.example.edu.ssl.error.log`

    * `CustomLog /var/log/apache2/digital-collections.example.edu.ssl.access.log combined`

* Edit the previously named `newsite-sample-ssl.conf` file and change lines 4 and 5 to point to the location of your apache logs on the container - example:

    * `ErrorLog /var/log/apache2/digital-collections.example.edu.ssl.error.log`

    * `CustomLog /var/log/apache2/digital-collections.example.edu.ssl.access.log combined`

* Edit the previously named `newsite-sample-ssl.conf` file and change lines 12, 13 and 14 to point to the location of your certs on the `apache` container - example:

    ```

        SSLCertificateFile	/certs/newsite-sample.pem
        SSLCertificateChainFile /certs/newsite-sample-interm.pem
        SSLCertificateKeyFile /certs/newsite-sample-key.pem

    ```

* If there are any additional customizations required, you'll need to copy them into these two vhost files accordingly.

### Apache Optional Edits

| Data          | Description                 | Production Data Copy              | Merge, Copy or Edit Location / Destination | Copy location            |
| ------------- | -------------               | -------------                     | -------------                              | -------------            |
| php.ini       | PHP configuration file      | current-production-config/apache/ | yourdomain-config/apache/                  | Local ISLE config laptop |

* `php.ini` - endusers can make appropriate edits within `yourdomain-config/apache/php.ini` to increase the upload settings, memory etc. as needed. Otherwise leaving the default values should work.

* **Please note:** an additional line will have to be added to the associated `docker-compose.yml` in the Apache `volumes:` section for this edit to work e.g. `- ./apache/php.ini:/etc/php.ini`

| Data          | Description           | Production Data Copy              | Merge, Copy or Edit Location / Destination  | Copy location            |
| ------------- | -------------         | -------------                     | -------------                               | -------------            |
| tmpreaper     | Cronjob for tmpreaper | current-production-config/apache/ | yourdomain-config/apache/                   | Local ISLE config laptop |

* `tmpreaper` - (optional) endusers may want to edit this tmpreaper cron job for different locations and/or times. The `docker-compose.yml` file will need an associated bind-mount for this change.

* **Please note:** an additional line will have to be added to the associated `docker-compose.yml` in the Apache `volumes:` section for this edit to work e.g. `- ./apache/tmpreaper/cron:/etc/cron.d/tmpreaper-cron`


### Apache - SSL-Certs

If need be, please refer to the **SSL certificate** section of the [Glossary](../appendices/glossary.md) for relevant terms to help guide installation.

* Copy your original production SSL certificates for Apache into the `apache/ssl-certs` subdirectory. They will and should have different names than the examples provided below dependent on the ISLE environment you are setting up e.g. (_production, staging, or development_).

    * There can be up to 2 - 3 files involved in this process.

        * 1 x SSL Certificate Key File e.g. `newsite-sample-key.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.key` or `.pem`

        * 1 x SSL Certificate File e.g. `newsite-sample.pem`
            * This file is required.
            * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

        * 1 x SSL Certificate Chain File e.g. `newsite-sample-interm.pem`
            * This file may be **optional** in some setups but is generally recommended for use by the `apache` container when available.
            * It will not be used by the `proxy` container.
            * Please also note that the file extensions can also be: `.cer`, `.crt` or `.pem`

### Apache - SSL-Certs (multi)

* When creating multiple environments for ISLE, please change all of the file and key names accordingly to reflect the environment e.g. adding (`-prod, -stage, -dev` to file names). Later on, this process will assist in organizing proper filing of files for the `proxy` container and stop any situation where a file gets overwritten or improperly referenced by the wrong environment.

**Example:**

    * 1 x SSL Certificate Key File e.g. `newsite-dev-key.pem`
    * 1 x SSL Certificate File e.g. `newsite-dev.pem`
    * 1 x SSL Certificate Chain File e.g. `newsite-dev-interm.pem`

## Fedora

Compare, edit, merge or copy the following from the suggested directory `current-production-config/fedora/` to:

* `yourdomain-config/fedora/` on your local laptop.

| Data                  | Description                   | Possible Location                | Merge, Copy or Edit Location / Destination | Copy location |
| -------------         | -------------                 | -------------                    | -------------                              | ------------- |
| datastreamStore       | Entire Fedora data directory  | /usr/local/fedora/data/          | yourdomain-data/fedora/data/datastreamStore       | Remote ISLE Host server  |
| fedora-xacml-policies | Entire Fedora data directory  | /usr/local/fedora/data/          | yourdomain-data/fedora/data/fedora-xacml-policies | Remote ISLE Host server  |
| objectStore           | Entire Fedora data directory  | /usr/local/fedora/data/          | yourdomain-data/fedora/data/objectStore           | Remote ISLE Host server  |
| fedora.fcfg           | Fedora repository config file | /usr/local/fedora/server/config/ | yourdomain-config/fedora/ |  Local ISLE config laptop |
| fedora-users.xml      | Fedora users config file      | /usr/local/fedora/server/config/ | yourdomain-config/fedora/ |  Local ISLE config laptop |
| filter-drupal.xml     | Fedora Drupal filter file     | /usr/local/fedora/server/config/ | yourdomain-config/fedora/ |  Local ISLE config laptop |
| repository-policies   | Fedora Drupal filter file     | /usr/local/fedora/server/config/ | yourdomain-config/fedora/ |  Local ISLE config laptop |

### Fedora Edits

* The outlined contents (above) of the production Islandora Fedora `data` directory should be copied to a new directory called `yourdomain-data/fedora/data/` on your remote ISLE host server in the appropriate storage area.

    * Do not copy the following directories from the production Islandora fedora `/usr/local/fedora/data` directory.
        * /usr/local/fedora/`data/activemq-data`
        * /usr/local/fedora/`data/resourceIndex`

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

## MySQL

The `mysql` subdirectory contains all specific configurations and overrides necessary for the ISLE mysql image and resulting container to function properly with your changes. This is the Mysql database server that will contain at least two databases, one for the Islandora / Drupal website and the other for the Fedora repository. If you are running Drupal multi-sites, you'll need to create the additional users and database creation scripts.

* (_Optional_) Edit the Mysql configuration file `my.cnf` as needed otherwise leave alone.


#### Mysql - initscripts

This subdirectory houses SQL scripts necessary for a one time creation of your associated new site and `fedora3` database.

You'll want to rename `newsite_sample_db.sql` to the database or domain name of your choice.

* Edit the contents of `newsite_sample_db.sql` to create the new drupal site database and user.

    * Line 1: Change the database name from `newsite_sample_db` to the database name of your choice.

    * Line 2: Change the database user name from `newsite_sample_db_user` to the database user name of your choice.

    * Line 3: At almost the end of the line, change the value of `newsite_sample_db.*` to the to the database name of your choice ensuring the `.*` remain without a space.

    * Line 3: At the end of the line, change the value of `newsite_sample_db_user'` to the to the database user name of your choice ensuring the values remain with in the `''`quotes without spaces. Do not alter the remaining code (`@'%';'`) beyond that point.

* Edit the contents of `fedora3` to change the `fedora_admin` user password only.

    * Line 2: Change the `fedora_admin` user password from `newsite_sample_fedora_admin_pw` to the password of your choice.

    * It is not recommended to change anything else.

## Solr

Compare, edit, merge or copy the following from the source directory `current-production-config/solr/` to:

* `yourdomain-config/solr/` on your local laptop.

| Data           | Description               | Possible Location        | Merge, Copy or Edit Location / Destination | Copy location |
| -------------  | -------------             | -------------            | -------------                              | -------------   |
| schema.xml     | Solr index config file    | ../solr/collection1/conf | yourdomain-config/solr/                    | Local ISLE config laptop |
| solrconfig.xml | Solr config file          | ../solr/collection1/conf | yourdomain-config/solr/                    | Local ISLE config laptop |
| stopwords.txt  | solr webserver vhost file | ../solr/collection1/conf | yourdomain-config/solr/                    | Local ISLE config laptop |

### Solr Edits

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

* `stopwords.txt`
  * _Using the stopwords.txt file, one can avoid the common words of a language, which do not add a significant value to any search.
  * _For example, a, an, the, you, I, am, and so on. One can specify words to be removed from the Solr search in this file line-by-line._

* Additional Locations:
  * /opt/solr
  * /usr/local/solr
  * /var/lib/tomcat7/webapps/solr
  * /usr/share/tomcat/webapps/solr

---

## Proxy Directory

If need be, please refer to the **Systems** section of the [Glossary](../appendices/glossary.md) for relevant terms to help guide installation.

This directory and service will not exist on any current islandora production systems.

* Please follow the [New ISLE Installation](../install/install-one-environment.md), `### Proxy Directory` section, lines 317 - 395.
