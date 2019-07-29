# Demo ISLE Installation: Resources

### Local URL
Always use the `https://isle.localdomain` domain to view and log in to a local site. (Do not use an IP address, such as `https://10.10.10.130 or https://127.0.0.01` as some components may not function correctly.)

---

### Docker Containers: Passwords

* `islandora` user on the ISLE host server uses `islandora` as the password.

<!--- TODO this IP information conflicts with our statement to always use the `https://isle.localdomain` domain. Requires clarification. --->
* Some of the information below is for accessing the non Drupal site admin panels and resources only. (optional). In this context, `hostip` below can mean either
     * the IP address e.g. `http://10.10.10.130` of the Vagrant or Non-Vagrant Host VM (_CentOS / Ubuntu_)
     * the IP address e.g. `127.0.0.1` for `Docker for Mac`

#### 1. MySQL Container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| mysql                | isle-mysql-ld   | MySQL 5.7     | 3306          |


| Account        | Password              | Database         | Perms                         |
| -------------  | -------------         | -------------    | -------------                 |      
| root           | ild_mysqlrt_2018      | **ALL**          | **ALL**                       |
| fedora_admin   | ild_feddb_2018        | fedora3          | **All** except `Grant` option |
| isle_ld_user   | isle_ld_db2018        | isle_ld          | **All** except `Grant` option |

---

#### 2. Fedora Container
| Compose Service Name | Container Name  | Software      | Ports                                            |
| :-------------:      | :-------------: | ------------- | -------------                                    |      
| fedora               | isle-fedora-ld  | see below     | 8080 mapped to 8081 (on host) |


| Software                         | Version           |
| -------------                    | -------------     |
| Fedora                           | 3.8.1             |
| Apache                           | 2.4.7             |
| Drupalfilter                     | 3.8.1             |
| Gsearch (w/remote SOLR plugin)   | 2.8+ (DG patched) |
| (DG) GSearch Extensions          | 0.13              |
| (DG) Islandora Transforms (XSLTs)| as of 7/2019      |
| Tomcat                           | 8.5.x            |  
| Open JDK                         | 8.x               |

| Account           | Password                      | Service       | URL           |
| -------------     | -------------                 | ------------- | ------------- |      
| fedoraAdmin       | ild_fed_admin_2018            | Fedora        | http://hostip:8081/fedora/describe                          |
| fedoraIntCallUser | ild_fed_IntCallUser_2018      | Fedora        | http://hostip:8081/fedora/objects                           |
| anonymous         | anonymous                     | Fedora        | ---                                                         |
| fgsAdmin          | ild_fgs_admin_2018            | Gsearch       | http://hostip:8081/fedoragsearch/rest?operation=updateIndex |
| admin             | isle_admin                    | Tomcat        | http://hostip:8081/manager/html                             |
| manager           | isle_manager                  | Tomcat        | http://hostip:8081/manager/html                             |

---

#### 3. Solr Container

| Compose Service Name | Container Name  | Software      | Ports                                              |
| :-------------:      | :-------------: | ------------- | -------------                                      |      
| solr                 | isle-solr-ld    | see below     | 8080 (on container) mapped to 8082 (on host)       |


| Software               | Version       |
| -------------          | ------------- |
| Solr                   | 4.10.4        |
| Tomcat                 | 8.5.x           |  
| OpenJDK                | 8.x           |
| (DG) Basic Solr Config | 4.10.x branch |

| Account           | Password        | Service       | URL                             |
| -------------     | -------------   | ------------- | -------------                   |
| admin             | isle_admin      | Tomcat        | http://hostip:8082/manager/html |
| manager           | isle_manager    | Tomcat        | http://hostip:8082/manager/html |
| --                | --              | Solr          | http://hostip:8082/solr/        |

---

#### 4. Apache Container

| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| apache               | isle-apache-ld  | see below     | 80, 443       |


| Software      | Version       |
| ------------- | ------------- |
| Apache        | 2.4.7         |
| OpenJDK       | 8.x           |
| Djatoka*      | 1.1           |
| Drupal        | 7.57          |
| PHP           | 7.1           |
| Islandora     | 7.x           |

\* Djatoka is included because it bundles useful Kakadu binaries, but is not running as a service.

| Account                | Password                      | Service               | URL                                                  |
| -------------          | -------------                 | -------------         | -------------                                        |
| isle                  | isle                           | Drupal site admin     | [https://isle.localdomain](https://isle.localdomain) |

---

#### 5. Proxy Container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |
| proxy                | isle-proxy      | Traefik       | 80, 443       |

| Account               | Password      | Service      | URL        |
| :-------------:       | :-------------:   | :-------------: | :-------------: |
| None Required         | None Required                 | Proxy UI     | [https://admin.isle.localdomain](https://admin.isle.localdomain) OR http://hostip:8080  |

* The Proxy Control Panel is available at [admin.isle.localdomain](https://admin.isle.localdomain).  No username/password are required.  This is unsafe for production environments.

---

#### 6. Image Services
| Compose Service Name | Container Name  | Software      | Ports                                            |
| :-------------:      | :-------------: | ------------- | -------------                                    |      
| image-services       |  isle-images-ld | see below     | 8080 (on container) mapped to 8083 (on host)     |


| Software                         | Version           |
| -------------                    | -------------     |
| Cantaloupe IIIF                  | 4.x               |

| Account           | Password                      | Service       | URL           |
| -------------     | -------------                 | ------------- | ------------- |
| admin             | isle_admin                    | Tomcat        | http://hostip:8082/manager/html   |
| manager           | isle_manager                  | Tomcat        | http://hostip:8082/manager/html   |   
| admin             | isle_admin                    | Cantaloupe    | http://hostip:8083/cantaloupe/admin   |

---

#### 7. Portainer

* Portainer (a Docker control panel) is available at [portainer.isle.localdomain](http://portainer.isle.localdomain). No username/password are required.  This is unsafe for production environments.

---

**Return to [Demo ISLE Installation](../install/install-demo.md#step-6-additional-resources).**
