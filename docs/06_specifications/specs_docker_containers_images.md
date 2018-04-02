| ISLE Image                          | Container    | Software / Service | Version       | Logs Path on Container |
| ------------                        | ------------ | ------------       | ------------- | -------------          |
| [islandoracollabgroup/isle-apache](https://hub.docker.com/r/islandoracollabgroup/isle-apache/) | isle-apache  | Docker Tag         | [latest, 2.4](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/apache/Dockerfile)   |                        |
| uses the official Docker base image |              | Apache             | 2.4           | /var/log/apache2/      |
| [ubuntu:14.04](https://hub.docker.com/_/ubuntu/)|              | Djatoka            | 1.1           |                        |
|                                     |              | Drupal             | 7.5x+         |                        |
|                                     |              | Islandora          | 7.x-1.1x      |                        |
|                                     |              | OpenJDK            | JDK 8.x       |                        |
|                                     |              | Oracle Java        | JDK 8.x       |                        |
|                                     |              | PHP                | 5.6           |                        |


| Container    | Software / Service           | Version            | Logs Path on Container                                          |
------------   | ------------                 | -------------      | -------------                                                   |
| isle-fedora  | Docker Tag                   | latest, 2.4        |                                                                 |
|              | Djatoka                      | 1.1                | /usr/local/tomcat/logs/djatoka.log                              |
|              | Drupal filter                | 3.8.1              |                                                                 |
|              | Fedora                       | 3.8.1              | /usr/local/fedora/server/logs/fedora.log                        |
|              | Gsearch                      | DG Patched (2.8.x) | /usr/local/fedora/server/logs/fedoragsearch.daily.log           |
|              | GSearch Extensions           | 0.1.3              |                                                                 |
|              | Islandora Transforms (XSLTs) | latest XSLTs from DG [repo](https://github.com/discoverygarden/islandora_transforms) |
|              | MySQL (client)               | 5.6                |                                                                 |
|              | OpenJDK                      | JDK 8.x            |                                                                 |
|              | Oracle Java                  | JDK 8.x            |                                                                 |
|              | Tomcat                       | 8.x                | /usr/local/tomcat/logs/                                         |


| Container    | Software / Service           | Version            | Logs Path on Container |
------------   | ------------                 | -------------      | -------------          |
| isle-mysql   | Docker Tag                   | latest, 5.6        |                        |
|              | MySQL (server)               | 5.6                | /var/log/mysql/        |

| Container    | Software / Service           | Version            | Logs Path on Container          |
------------   | ------------                 | -------------      | -------------                   |
| isle-solr    | Docker Tag                   | latest, 2.4        |                                 |
|              | OpenJDK                      | JDK 8.x            |                                 |
|              | Solr                         | 4.10.4             | /usr/local/tomcat/logs/solr.log |
|              | Tomcat                       | 8.x                | /usr/local/tomcat/logs/         |

| Container    | Software / Service           | Version            |
------------   | ------------                 | -------------      |
| isle-proxy   | Docker Tag                   | latest, 1.13       |
|              | Nginx                        | 1.13               |
