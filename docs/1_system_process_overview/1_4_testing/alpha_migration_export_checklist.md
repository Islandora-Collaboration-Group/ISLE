### Migration Export Checklist
This section is a checklist of materials to **COPY** from the current running institutional Islandora Production server(s) to the appropriate storage location / directory on the new ISLE Host Server.

**Please Note:** _Ubuntu / Debian style paths are used for all examples file locations below, endusers might have different locations for these files HOWEVER the file and directory names etc should be roughly the same._

**Caution**: While the ISLE Project recommends use of export methods or tools such as rsync, scp etc., it assumes that endusers are familiar with them and are aware of possible dangers of improperly exporting or copying production data.
Ensure adequate backups of any production system(s) are made prior to any attempts. If you are not familiar or are uncomfortable with these processes, it is highly advisable to work with an appropriate IT resource.

ISLE assumes no responsibility or liability in this matter.

---

#### Apache

Copy the following below to the suggested directory `/path_to/islandora_production_data_storage/apache/` on the ISLE Host server.

This data will be used in conjunction with the Apache container (`isle-apache`).

| Data          | Description                 | Possible Location             | Suggested ISLE Path Destination            | Notes         |
| ------------- | -------------               | -------------                 | -------------                              | ------------- |
| html          | Islandora/Drupal Website    | /var/www/                     | /islandora_production_data_storage/apache/ | --            |
| settings.php  | Drupal settings.php file    | /var/www/html/sites/default/  | /islandora_production_data_storage/apache/ | _see below_   |
| php.ini       | PHP configuration file      | /etc/                         | /islandora_production_data_storage/apache/ | _see below_   |
| yoursite.conf | Apache webserver vhost file | /etc/apache2/sites-available/ | /islandora_production_data_storage/apache/ | _see below_   |


**Apache Notes**:

* /var/www/`html` (_Entire contents unless size prohibits_)

  * (_If `html` is not used, then substitute with the appropriate directory for the Islandora / Drupal site_)


* `settings.php` (_if running multi-sites separate or rename appropriately e.g. multisite2_name_settings.php, multisite3_name_settings.php and so on..._)

* `yoursite.conf` (_file will have different name_)

---

#### Fedora

Copy the following below to the suggested directory `/path_to/islandora_production_data_storage/fedora/` on the ISLE Host server.

This data will be used in conjunction with the Apache container (`isle-fedora`).  

| Data              | Description                   | Possible Location                | Suggested ISLE Path Destination            | Notes         |
| -------------     | -------------                 | -------------                    | -------------                              | ------------- |
| yoursite.conf     | Apache webserver vhost file   | /etc/apache2/sites-available/    | /islandora_production_data_storage/fedora/ | _see below_   |
| data              | Entire Fedora data directory  | /usr/local/fedora/               | /islandora_production_data_storage/fedora/ | --            |
| fedora.fcfg       | Fedora repository config file | /usr/local/fedora/server/config/ | /islandora_production_data_storage/fedora/ | --            |
| fedora-users.xml  | Fedora users config file      | /usr/local/fedora/server/config/ | /islandora_production_data_storage/fedora/ | --            |
| filter-drupal.xml | Fedora Drupal filter file     | /usr/local/fedora/server/config/ | /islandora_production_data_storage/fedora/ | --            |

**Fedora Notes**:

* `yoursite.conf` (_if you use Adore-Djatoka with a reverse proxy otherwise it is possible this file is not necessary_)

---

#### Gsearch

Copy the following below to the suggested directory `/path_to/islandora_production_data_storage/gsearch/` on the ISLE Host server.

This data will be used in conjunction with the Apache container (`isle-fedora`).

| Data                                          | Description                       | Possible Location                                                                          | Suggested ISLE Path Destination             | Notes         |
| -------------                                 | -------------                     | -------------                                                                              | -------------                               | ------------- |
| fedoragsearch.properties                      | Gsearch config file               | /var/lib/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/                      | /islandora_production_data_storage/gsearch/ | --            |
| fgsconfig-basic-configForIslandora.properties | Gsearch config file               | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/                     | /islandora_production_data_storage/gsearch/ | --            |
| fgsconfigObjects.properties                   | Gsearch config file               | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/                     | /islandora_production_data_storage/gsearch/ | --            |
| repository.properties                         | Gsearch config file               | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/repository/FgsRepos/ | /islandora_production_data_storage/gsearch/ | --            |
| islandora_transforms                          | Transformation XSLTs directory    | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/      | /islandora_production_data_storage/gsearch/ | --            |
| foxmlToSolr.xslt                              | "top-level" transformational XSLT | /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/      | /islandora_production_data_storage/gsearch/ | --            |


#### MySQL

Copy the following below to the suggested directory `/path_to/islandora_production_data_storage/mysql/` on the ISLE Host server.

This data will be used in conjunction with the Apache container (`isle-mysql`).

| Data          | Description              | Possible Location  | Suggested COPY Method                   | Suggested ISLE Path Destination           | Notes         |
| ------------- | -------------            | -------------      | -------------                           | -------------                             | ------------- |
| Databases     | Drupal website databases |  /var/lib/mysql    | CLI, MySQL Workbench or Sequel Pro etc. | /islandora_production_data_storage/mysql/ | _see below_   |
| my.cnf        | MySQL server config file |  /etc/mysql/my.cnf | rsync / scp                             | /islandora_production_data_storage/mysql/ | --            |

**MySQL Notes**:

* _Drupal website databases can have a multitude of names and conventions. Confer with the appropriate IT resources for your institution's database naming conventions._

* _CLI == Command line_

* _Recommended that the production databases be exported using the `.sql` and `.gz` file formats e.g. `drupal_site_2018.sql.gz` for better compression and minimal storage footprint._

* _If the enduser is running multi-sites, there will be additional databases to export._

* _Do not export the `fedora3` database as it will be recreated by the SQL index in later steps of the Migration Guide_

* _If possible, on the production Apache webserver, run `drush cc all` from the command line on the production server in the appropriate sites directory PRIOR to db export(s)_
  * _Otherwise issues can occur on import due to all cache tables being larger than `innodb_log_file_size` allows_

**MySQL Tools for Export**

* [Official MySQL documentation](https://dev.mysql.com/doc/)
* [Digital Ocean quick start for cli export](https://www.digitalocean.com/community/tutorials/how-to-import-and-export-databases-in-mysql-or-mariadb)
* [Official MySQL GUI app - Workbench](https://www.mysql.com/products/workbench/) _For Linux, MacOS and Windows_
* [Sequel Pro](https://sequelpro.com/) _MacOS only_


**Tomcat**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/tomcat`

This data will be used in conjunction with the Apache containers (`isle-fedora` & `isle-solr`).

| Data             | Description               | Possible Location                      | Suggested ISLE Path Destination            | Notes         |
| -------------    | -------------             | -------------                          | -------------                              | ------------- |
| server.xml       | Tomcat server config file | /var/lib/tomcat7/conf/server.xml       | /islandora_production_data_storage/tomcat/ | --            |
| tomcat-users.xml | Tomcat server config file | /var/lib/tomcat7/conf/tomcat-users.xml | /islandora_production_data_storage/tomcat/ | --            |


**Solr**

* Copy from the following locations below on the current running Islandora Production server(s) to the ISLE Host server `/pathto/islandora_production_data_storage/solr`

This data will be used in conjunction with the Apache containers (`isle-solr`).

| Data           | Description                              | Possible Location                                             | Suggested ISLE Path Destination         | Notes         |
| -------------  | -------------                            | -------------                                                 | -------------                           | ------------- |
| schema.xml     | Solr schema file                         | /var/lib/tomcat7/webapps/solr/collection1/conf/schema.xml     | /islandora_production_data_storage/solr | _see below_   |  
| solrconfig.xml | Solr config file                         | /var/lib/tomcat7/webapps/solr/collection1/conf/solrconfig.xml | /islandora_production_data_storage/solr | _see below_   |  
| stopwords.txt  | Solr file for filtering out common words | /var/lib/tomcat7/webapps/solr/collection1/conf/stopwords.txt  | /islandora_production_data_storage/solr | _see below_   |  
