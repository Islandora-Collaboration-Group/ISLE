This section is a checklist of materials to **COPY** from the current running institutional Islandora Production server(s) to the appropriate storage location / directory on the new ISLE directory.

**Please Note:** _Ubuntu / Debian style paths are used for all examples file locations below, endusers might have different locations for these files HOWEVER the file and directory names etc should be roughly the same._

**Caution**: While the ISLE Project recommends use of export methods or tools such as rsync, scp etc., it assumes that endusers are familiar with them and are aware of possible dangers of improperly exporting or copying production data. Ensure adequate backups of any production system(s) are made prior to any attempts. If you are not familiar or are uncomfortable with these processes, it is highly advisable to work with an appropriate IT resource.

**Finally also please note:** Instructions from this checklist and the [Migration Installation Guide](migration_installation_guide.md) may call for you to **COPY** data from your running Islandora environment to a newly created folder: `current_prod_islandora_config` located on your ISLE Host Server or local workstation called `local ISLE config laptop`. You will then work from this copy for future steps.

In some cases, you'll need to copy configurations down to your laptop / workstation (`local ISLE config laptop`) and merge contents as directed. (_if necessary_) In other cases, due to the size of the data e.g. Fedora data you may only be able to copy production data to the target ISLE Host server (`Remote ISLE Host server`) instead of your local laptop.

* Contents of the production Islandora Apache `html` directory should be copied to a new directory called `yourdomain-data/apache/html/` on your remote ISLE host server in the appropriate storage area.

* Contents of the  production Islandora Fedora `data` directory should be copied to a new directory called `yourdomain-data/fedora/data/` on your remote ISLE host server in the appropriate storage area.

Please attempt to balance as necessary when putting together the `docker-compose.yml` and config directory. Most instructions will attempt to direct you to copy to a local workstation. ultimately you will be putting this config in a git repository to deploy to the remote ISLE host server for everything to work with both your copied data and merged production settings.

**Recommend:** Having adequate storage space available for the ISLE host server to accommodate a working copy of a production Islandora's associated configurations and data.

---

### Apache

* Copy the following below from the Islandora Production Server(s) to the suggested directories `/current_prod_islandora_config/apache/` or `/data/apache/` on the ISLE directories located on both the local ISLE config laptop and remote ISLE Host server as designated.

    * `/current_prod_islandora_config/apache/` should be a directory used for configurations to be merged or edited on an enduser's laptop (`local ISLE config laptop`)

    * `/data/apache/`should be a directory used for apache website code and data  to be copied to a storage area on the target ISLE Host server (`Remote ISLE Host server`)  due to size.

This data and these configurations will be used in conjunction with an Apache container.

| Data          | Description                 | Possible Location             | Suggested ISLE Path Destination        | Copy location |
| ------------- | -------------               | -------------                 | -------------                          | ------------- |
| html          | Islandora/Drupal Website    | /var/www/                     | yourdomain-data/apache/html/           | Remote ISLE Host server  |
| settings.php  | Drupal settings.php file    | /var/www/html/sites/default/  | /current_prod_islandora_config/apache/ | Local ISLE config laptop |
| php.ini       | PHP configuration file      | /etc/                         | /current_prod_islandora_config/apache/ | Local ISLE config laptop |
| yoursite.conf | Apache webserver vhost file | /etc/apache2/sites-enabled/   | /current_prod_islandora_config/apache/ | Local ISLE config laptop |


#### Apache Notes

* /var/www/`html`

    * _Entire contents unless size prohibits should be copied to a directory on the remote ISLE host server._

    * _If `html` is not used, then substitute with the appropriate directory for the Islandora / Drupal site_

* `settings.php`

    * _If running multi-sites separate or rename appropriately e.g. multisite2_name_settings.php, multisite3_name_settings.php and so on..._

    * **WARNING:** Be sure to move this file from `yourdomain-data/apache/html/` only and not from your production Islandora site PRIOR to copying.

* `yoursite.conf`

    * _File will have different name but this should be the enabled Apache vhost file of your production Islandora website._
    * _There may also be a seperate vhost that uses SSL and https. Copy that too if available._


---

### Fedora

Copy the following below from the Islandora Production Server(s) to the suggested directory `current_prod_islandora_config/fedora/` on the ISLE directories located on both the local ISLE config laptop and remote ISLE Host server as designated.


This data will be used in conjunction with a Fedora container.  

| File / Directory      | Description                   | Possible Location                | Suggested ISLE Path Destination                   | Copy location            |
| -------------         | -------------                 | -------------                    | -------------                                     | -------------            |
| datastreamStore       | Entire Fedora data directory  | /usr/local/fedora/data/          | yourdomain-data/fedora/data/datastreamStore       | Remote ISLE Host server  |
| fedora-xacml-policies | Entire Fedora data directory  | /usr/local/fedora/data/          | yourdomain-data/fedora/data/fedora-xacml-policies | Remote ISLE Host server  |
| objectStore           | Entire Fedora data directory  | /usr/local/fedora/data/          | yourdomain-data/fedora/data/objectStore           | Remote ISLE Host server  |
| fedora.fcfg       | Fedora repository config file | /usr/local/fedora/server/config/ | /current_prod_islandora_config/fedora/ | Local ISLE config laptop |
| fedora-users.xml  | Fedora users config file      | /usr/local/fedora/server/config/ | /current_prod_islandora_config/fedora/ | Local ISLE config laptop |
| filter-drupal.xml | Fedora Drupal filter file     | /usr/local/fedora/server/config/ | /current_prod_islandora_config/fedora/ | Local ISLE config laptop |

#### Fedora Notes

* Do not copy the following directories from the production Islandora fedora `/usr/local/fedora/data` directory.
        * /usr/local/fedora/`data/activemq-data`
        * /usr/local/fedora/`data/resourceIndex`
---

### Gsearch

Copy the following below from the Islandora Production Server(s) to the suggested directory `current_prod_islandora_config/gsearch/` on the ISLE directory located on the local ISLE config laptop.

This data will be used in conjunction with a Fedora container.

| File / Directory | Description   | Possible Location   | Suggested ISLE Path Destination         | Copy location |
| -------------    | ------------- | -------------       | -------------                           | ------------- |
| fedoragsearch.properties | Gsearch config file | /var/lib/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/ | /current_prod_islandora_config/gsearch/ | Local ISLE config laptop |
| fgsconfig-basic-configForIslandora.properties | Gsearch config file | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/ | /current_prod_islandora_config/gsearch/ | Local ISLE config laptop |
| fgsconfigObjects.properties | Gsearch config file | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/ | /current_prod_islandora_config/gsearch/ | Local ISLE config laptop |
| repository.properties | Gsearch config file | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/repository/FgsRepos/ | /current_prod_islandora_config/gsearch/ | Local ISLE config laptop |
| islandora_transforms | Transformation XSLTs directory | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/ | /current_prod_islandora_config/gsearch/ | Local ISLE config laptop |
| foxmlToSolr.xslt | "top-level" transformational XSLT | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/ | /current_prod_islandora_config/gsearch/ | Local ISLE config laptop |

---

### MySQL

Copy the following below from the Islandora Production Server(s) to the suggested directory `current_prod_islandora_config/mysql/` on the ISLE directory located on the local ISLE config laptop.

This data will be used in conjunction with a MySQL container.

| Data          | Description              | Possible Location  | Suggested COPY Method                   | Suggested ISLE Path Destination       | Copy location            |
| ------------- | -------------            | -------------      | -------------                           | -------------                         | -------------            |
| Databases     | Drupal website databases |  /var/lib/mysql    | CLI, MySQL Workbench or Sequel Pro etc. | /current_prod_islandora_config/mysql/ | Local ISLE config laptop |
| my.cnf        | MySQL server config file |  /etc/mysql/my.cnf | rsync / scp                             | /current_prod_islandora_config/mysql/ | Local ISLE config laptop |

#### MySQL Notes

* _Drupal website databases can have a multitude of names and conventions. Confer with the appropriate IT resources for your institution's database naming conventions._

* _CLI == Command line_

* _Recommended that the production databases be exported using the `.sql` and `.gz` file formats e.g. `drupal_site_2018.sql.gz` for better compression and minimal storage footprint._

* _If the enduser is running multi-sites, there will be additional databases to export._

* _Do not export the `fedora3` database as it will be recreated by the SQL index in later steps of the Migration Guide_

* _If possible, on the production Apache webserver, run `drush cc all` from the command line on the production server in the appropriate sites directory PRIOR to db export(s)_
  * _Otherwise issues can occur on import due to all cache tables being larger than `innodb_log_file_size` allows_

#### MySQL Tools for Export
Here are a few pieces of documentation specific for the tasks above.

**Caution**: While the ISLE Project recommends use of export methods or tools such as mysqldump etc., it assumes that endusers are familiar with them and are aware of possible dangers of improperly exporting or copying production data. Ensure adequate backups of any production system(s) are made prior to any attempts. If you are not familiar or are uncomfortable with these processes, it is highly advisable to work with an appropriate IT resource.

* [Official MySQL documentation](https://dev.mysql.com/doc/)
    * [mysqldump console utility documentation](https://dev.mysql.com/doc/refman/5.7/en/mysqldump.html)
    * [Digital Ocean quick start for mysqldump export](https://www.digitalocean.com/community/tutorials/how-to-import-and-export-databases-in-mysql-or-mariadb)
* [Official MySQL GUI app - Workbench](https://www.mysql.com/products/workbench/) _For Linux, MacOS and Windows_
* [Sequel Pro](https://sequelpro.com/) _MacOS only_

---

##Tomcat

Copy the following below from the Islandora Production Server(s) to the suggested directory to the ISLE directory `current_prod_islandora_config/tomcat` located on the local ISLE config laptop.

This data will be used in conjunction with the Tomcat service found on a Fedora or SOLR container.

| Data             | Description               | Possible Location                      | Suggested ISLE Path Destination    | Copy location |
| -------------    | -------------             | -------------                          | -------------                      | ------------- |
| tomcat-users.xml | Tomcat server config file | /var/lib/tomcat7/conf/tomcat-users.xml | /current_prod_islandora_config/tomcat/ | Local ISLE config laptop |

---

##Solr

Copy the following below from the Islandora Production Server(s) to the suggested directory to the ISLE directory `current_prod_islandora_config/solr` located on the local ISLE config laptop.

This data will be used in conjunction with a SOLR container.

| Data           | Description                              | Possible Location                                             | Suggested ISLE Path Destination     | Copy location |
| -------------  | -------------                            | -------------                                                 | -------------                       | ------------- |
| schema.xml     | Solr schema file                         | /var/lib/tomcat7/webapps/solr/collection1/conf/schema.xml     | /current_prod_islandora_config/solr | Local ISLE config laptop |
| solrconfig.xml | Solr config file                         | /var/lib/tomcat7/webapps/solr/collection1/conf/solrconfig.xml | /current_prod_islandora_config/solr | Local ISLE config laptop |
| stopwords.txt  | Solr file for filtering out common words | /var/lib/tomcat7/webapps/solr/collection1/conf/stopwords.txt  | /current_prod_islandora_config/solr | Local ISLE config laptop |
