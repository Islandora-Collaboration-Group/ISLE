## New Site Sample Cheatsheet

To be filled out by an enduser while creating a new site. (_optional_)

* ISLE Host Server: `isle-host-server.com`

* The `islandora` user on the ISLE Host Server uses the password of `islandora`.

#### 1. MySQL container
| Compose Service Name | Container Name      | Software      | Ports         |
| :-------------:      | :-------------:     | ------------- | ------------- |      
| mysql                | isle-mysql-newsite  | MySQL 5.6     | 3306          |

| Account                | Password                       | Database          | Perms                         |
| -------------          | -------------                  | -------------     | -------------                 |
| root                   | newsite_sample_root_pw         | **ALL**           | **ALL**                       |
| fedora_admin           | newsite_sample_fedora_admin_pw | fedora3           | **All** except `Grant` option |
| newsite_sample_db_user | newsite_sample_db_pw           | newsite_sample_db | **All** except `Grant` option |


---

#### 2. Fedora container
| Compose Service Name | Container Name      | Software      | Ports         |
| :-------------:      | :-------------:     | ------------- | ------------- |      
| fedora               | isle-fedora-newsite | see below     | 8080          |


| Software                         | Version           |
| -------------                    | -------------     |
| Fedora                           | 3.8.1             |
| Drupalfilter                     | 3.8.1             |
| Gsearch (w/remote SOLR plugin)   | 2.8+ (DG patched) |
| (DG) GSearch Extensions          | 0.13              |
| (DG) Islandora Transforms (XSLTs)| as of 1/2018      |
| Tomcat                           | 7.x               |  
| Oracle JDK                       | 8.x               |
| Djatoka                          | 1.1               |


| Account           | Password      | Service       | URL           |
| -------------     | ------------- | ------------- | ------------- |      
| fedoraAdmin       |               | Fedora        | http://ISLE-HOST-IP:8080/fedora/describe                          |
| fedoraIntCallUser |               | Fedora        | http://ISLE-HOST-IP:8080/fedora/objects                           |
| anonymous         | anonymous     | Fedora        | ---                                                               |
| fgsAdmin          |               | Gsearch       | http://ISLE-HOST-IP:8080/fedoragsearch/rest?operation=updateIndex |
| admin             |               | Tomcat        | http://ISLE-HOST-IP:8080/manager/html                             |
| manager           |               | Tomcat        | http://ISLE-HOST-IP:8080/manager/html                             |
| --                | --            | Djatoka       | http://ISLE-HOST-IP:8080/adore-djatoka/                           |

---

#### 3. Solr container

| Compose Service Name | Container Name    | Software      | Ports                                        |
| :-------------:      | :-------------:   | ------------- | -------------                                |      
| solr                 | isle-solr-newsite | see below     | 8080 (on container) mapped to 8091 (on host) |


| Software               | Version       |
| -------------          | ------------- |
| Solr                   | 4.10.4        |
| Tomcat                 | 7.x           |  
| Oracle JDK             | 8.x           |
| (DG) Basic Solr Config | 4.10.x branch |

| Account           | Password             | Service       | URL                                   |
| -------------     | -------------        | ------------- | -------------                         |
| admin             |                      | Tomcat        | http://ISLE-HOST-IP:8091/manager/html |
| manager           |                      | Tomcat        | http://ISLE-HOST-IP:8091/manager/html |
| --                | --                   | Solr          | http://ISLE-HOST-IP:8091/solr/        |


---

#### 4. Apache container

| Compose Service Name | Container Name      | Software      | Ports         |
| :-------------:      | :-------------:     | ------------- | ------------- |      
| apache               | isle-apache-newsite | see below     | 80 / 443      |


| Software      | Version       |
| ------------- | ------------- |
| Apache        | 2.4.7         |
| Oracle JDK    | 8.x           |
| Djatoka       | 1.1           |
| Drupal        | 7.56          |
| PHP           | 5.6           |
| Islandora     | 7.x           |

| Account         | Password             | Service           | URL                           |
| -------------   | -------------        | -------------     | -------------                 |
| isle_dev_admin  |                      | Drupal site admin | https://newsite-sample |
