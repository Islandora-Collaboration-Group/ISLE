

ISLE consists of 5 "images" accessible from the Dockerhub cloud repository. These images were built by the ISLE project in collaboration with the vendor, Born-Digital. ISLE project members maintain these images with software and hardware patches - the newest set on Dockerhub are always tagged `latest`.

The images are spun up using Docker and create "containers" - a type of virtual server networked together by Docker internal networking. Together with the customization files in the `config` folder, they form the software stack for Islandora.

  * isle-apache [https://hub.docker.com/r/islandoracollabgroup/isle-apache/](https://hub.docker.com/r/islandoracollabgroup/isle-apache/)  

  * isle-fedora [https://hub.docker.com/r/islandoracollabgroup/isle-fedora/](https://hub.docker.com/r/islandoracollabgroup/isle-fedora/)  

  * isle-mysql [https://hub.docker.com/r/islandoracollabgroup/isle-mysql/](https://hub.docker.com/r/islandoracollabgroup/isle-mysql/)  

  * isle-solr  [https://hub.docker.com/r/islandoracollabgroup/isle-solr/](https://hub.docker.com/r/islandoracollabgroup/isle-solr/)  

  * isle-proxy  [https://hub.docker.com/r/islandoracollabgroup/isle-proxy/](https://hub.docker.com/r/islandoracollabgroup/isle-proxy/)  




---

#### 1. MySQL container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| mysql                | isle-mysql      | MySQL 5.6     | 3306          |


| Account        | Password              | Database         | Perms                         |
| -------------  | -------------         | -------------    | -------------                 |      
| root           | ild_mysqlrt_2018      | **ALL**          | **ALL**                       |
| fedora_admin   | ild_feddb_2018        | fedora3          | **All** except `Grant` option |
| isle_ld_user   | isle_ld_db2018        | isle_ld          | **All** except `Grant` option |

  ---

#### 2. Fedora container
| Compose Service Name | Container Name  | Software      | Ports                                            |
| :-------------:      | :-------------: | ------------- | -------------                                    |      
| fedora               | isle-fedora     | see below     | 8080, 80 (on container) mapped to 8777 (on host) |


| Software                         | Version           |
| -------------                    | -------------     |
| Fedora                           | 3.8.1             |
| Apache                           | 2.4.7             |
| Drupalfilter                     | 3.8.1             |
| Gsearch (w/remote SOLR plugin)   | 2.8+ (DG patched) |
| (DG) GSearch Extensions          | 0.13              |
| (DG) Islandora Transforms (XSLTs)| as of 1/2018      |
| Tomcat                           | 7.x               |  
| Oracle JDK                       | 8.x               |
| Djatoka                          | 1.1               |

| Account           | Password                      | Service       | URL           |
| -------------     | -------------                 | ------------- | ------------- |      
| fedoraAdmin       | ild_fed_admin_2018            | Fedora        | http://hostip:8080/fedora/describe                          |
| fedoraIntCallUser | ild_fed_IntCallUser_2018      | Fedora        | http://hostip:8080/fedora/objects                           |
| anonymous         | anonymous                     | Fedora        | ---                                                            |
| fgsAdmin          | ild_fgs_admin_2018            | Gsearch       | http://hostip:8080/fedoragsearch/rest?operation=updateIndex |
| admin             | ild_tc_adm_2018               | Tomcat        | http://hostip:8080/manager/html                             |
| manager           | ild_tc_man_2018               | Tomcat        | http://hostip:8080/manager/html                             |
| --                | --                            | Djatoka       | http://hostip:8080/adore-djatoka/                           |
| --                | --                            | Apache        | http://hostip:8777                                          |

___

#### 3. Solr container

| Compose Service Name | Container Name  | Software      | Ports                                              |
| :-------------:      | :-------------: | ------------- | -------------                                      |      
| solr                 | isle-solr       | see below     | 8993, 8080 (on container) mapped to 8091 (on host) |


| Software               | Version       |
| -------------          | ------------- |
| Solr                   | 4.10.4        |
| Tomcat                 | 7.x           |  
| Oracle JDK             | 8.x           |
| (DG) Basic Solr Config | 4.10.x branch |

| Account           | Password        | Service       | URL                                |
| -------------     | -------------   | ------------- | -------------                      |
| admin             | ild_tc_adm_2018 | Tomcat        | http://hostip:8091/manager/html |
| manager           | ild_tc_man_2018 | Tomcat        | http://hostip:8091/manager/html |
| --                | --              | Solr          | http://hostip:8091/solr/        |


___

#### 4. Apache container

| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| apache               | isle-apache        | see below  | 80            |


| Software      | Version       |
| ------------- | ------------- |
| Apache        | 2.4.7         |
| Oracle JDK    | 8.x           |
| Djatoka       | 1.1           |
| Drupal        | 7.56          |
| PHP           | 5.6           |
| Islandora     | 7.x           |

| Account                | Password                      | Service               | URL                     |
| -------------          | -------------                 | -------------         | -------------           |
| isle_localdomain_admin | isle_localdomain_adminpw2018  | Drupal site admin     | http://isle.localdomain |
