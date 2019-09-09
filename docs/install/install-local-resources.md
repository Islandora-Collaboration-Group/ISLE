# Local ISLE Installation: Resources

### Local URL
Always use the `https://yourprojectnamehere.localdomain` domain to view and log in to a local site. (Do not use an IP address, such as `https://10.10.10.130 or https://127.0.0.01` as some components may not function correctly.)

---

### Docker Containers: Passwords

As instructed in the `## Step 4: Create New Users and Passwords by Editing "local.env" File` section of the [Local ISLE Installation: New Site](../install/install-local-new.md) documentation, you'll need to refer to the edited `local.env` for your password choices.

* `islandora` user on the ISLE host server uses `islandora` as the password. This might not apply to your setup if using local Docker clients.

<!--- TODO this IP information conflicts with our statement to always use the `https://isle.localdomain` domain. Requires clarification. --->
* Some of the information below is for accessing the non Drupal site admin panels and resources only. (optional). In this context, `hostip` below can mean either
     * the IP address e.g. `http://10.10.10.130` of the Vagrant or Non-Vagrant Host VM (_CentOS / Ubuntu_)
     * the IP address e.g. `127.0.0.1` for `Docker for Mac`

#### 1. MySQL Container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |      
| mysql                | Run `docker ps` to determine   | MySQL 5.7     | 3306          |


| Account        | Password              | Database         | Perms                         |
| -------------  | -------------         | -------------    | -------------                 |      
| root           | Based on your `local.env` edits | **ALL**          | **ALL**                       |
| fedora_admin   | Based on your `local.env` edits | fedora3          | **All** except `Grant` option |
| isle_ld_user   | Based on your `local.env` edits        | Based on your `local.env` edits          | **All** except `Grant` option |

---

#### 2. Fedora Container
| Compose Service Name | Container Name  | Software      | Ports                                            |
| :-------------:      | :-------------: | ------------- | -------------                                    |      
| fedora               | Run `docker ps` to determine  | see below     | 8080 mapped to 8081 (on host) |


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
| fedoraAdmin       | Based on your `local.env` edits            | Fedora        | http://yourprojectnamehere.localdomain:8081/fedora/describe                          |
| fedoraIntCallUser | Based on your `local.env` edits      | Fedora        | http://yourprojectnamehere.localdomain:8081/fedora/objects                           |
| anonymous         | anonymous                     | Fedora        | ---                                                         |
| fgsAdmin          | Based on your `local.env` edits            | Gsearch       | http://yourprojectnamehere.localdomain:8081/fedoragsearch/rest?operation=updateIndex |
| admin             | Based on your `local.env` edits                    | Tomcat        | http://yourprojectnamehere.localdomain:8081/manager/html                             |
| manager           | Based on your `local.env` edits                  | Tomcat        | http://yourprojectnamehere.localdomain:8081/manager/html                             |

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
| admin             | Based on your `local.env` edits      | Tomcat        | http://yourprojectnamehere.localdomain:8082/manager/html |
| manager           | Based on your `local.env` edits    | Tomcat        | http://yourprojectnamehere.localdomain:8082/manager/html |
| --                | --              | Solr          | http://yourprojectnamehere.localdomain:8082/solr/        |

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
| Based on your `local.env` edits                  | Based on your `local.env` edits                           | Drupal site admin     | [https://yourprojectnamehere.localdomain](https://yourprojectnamehere.localdomain) |

---

#### 5. Proxy Container
| Compose Service Name | Container Name  | Software      | Ports         |
| :-------------:      | :-------------: | ------------- | ------------- |
| proxy                | isle-proxy      | Traefik       | 80, 443       |

| Account               | Password      | Service      | URL        |
| :-------------:       | :-------------:   | :-------------: | :-------------: |
| None Required         | None Required                 | Proxy UI     | [https://admin.yourprojectnamehere.localdomain](https://admin.yourprojectnamehere.localdomain) OR http://yourprojectnamehere:8080  |

* The Proxy Control Panel is available at [admin.yourprojectnamehere.localdomain](https://admin.yourprojectnamehere.localdomain).  No username/password are required.  This is unsafe for production environments.

---

#### 6. Image Services
| Compose Service Name | Container Name  | Software      | Ports                                            |
| :-------------:      | :-------------: | ------------- | -------------                                    |      
| image-services       |  Run `docker ps` to determine | see below     | 8080 (on container) mapped to 8083 (on host)     |


| Software                         | Version           |
| -------------                    | -------------     |
| Cantaloupe IIIF                  | 4.x               |

| Account           | Password                      | Service       | URL           |
| -------------     | -------------                 | ------------- | ------------- |
| admin             | Based on your `local.env` edits                    | Tomcat        | http://yourprojectnamehere.localdomain:8082/manager/html   |
| manager           | Based on your `local.env` edits                  | Tomcat        | http://yourprojectnamehere.localdomain:8082/manager/html   |   
| admin             | Based on your `local.env` edits                    | Cantaloupe    | http://yourprojectnamehere.localdomain:8083/cantaloupe/admin   |

---

#### 7. Portainer

* Portainer (a Docker control panel) is available at [portainer.isle.localdomain](http://portainer.isle.localdomain). No username/password are required.  This is unsafe for production environments.

---

**Return to [Local ISLE Installation: New Site](../install/install-local-new.md).**
