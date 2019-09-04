<!--- PAGE_TITLE --->

| ISLE Image                          | Container    | Software / Service | Version       | Logs Path on Container |
| ------------                        | ------------ | ------------       | ------------- | -------------          |
| [islandoracollabgroup/isle-apache](https://hub.docker.com/r/islandoracollabgroup/isle-apache/) | isle-apache  | Docker Tag | [latest, 2.4](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/apache/Dockerfile) |                        |
|                |              | Apache             | 2.4           | /var/log/apache2/      |
|                |              | Djatoka            | 1.1           |                        |
|                                     |              | Drupal             | 7.5x+         |                        |
|                                     |              | Islandora          | 7.x-1.1x      |                        |
|                                     |              | OpenJDK            | JDK 8.x        |                        |
|                                     |              | PHP                | 5.6           |                        |


| ISLE Image                          | Container    | Software / Service | Version       | Logs Path on Container |
| ------------                        | ------------ | ------------       | ------------- | -------------          |
| [islandoracollabgroup/isle-fedora](https://hub.docker.com/r/islandoracollabgroup/isle-fedora/) | isle-fedora | Docker Tag | [latest, 3.8.1](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/fedora/Dockerfile) |
| [tomcat:8.0-jre8](https://hub.docker.com/_/tomcat/)|  |Drupal filter              | 3.8.1              |                                                                 |
|                                     |              | Fedora                       | 3.8.1              | /usr/local/fedora/server/logs/fedora.log                        |
|                                     |              | Gsearch                      | DG Patched (2.8.x) | /usr/local/fedora/server/logs/fedoragsearch.daily.log           |
|                                     |              | GSearch Extensions           | 0.1.3              |                                                                 |
|                                     |              | Islandora Transforms (XSLTs) | latest XSLTs from DG [repository](https://github.com/discoverygarden/islandora_transforms) |
|                                     |              | MySQL (client)               | 5.6                |                                                                 |
|                                     |              | OpenJDK                      | JDK 8.x            |                                                                 |
|                                     |              | Tomcat                       | 8.x                | /usr/local/tomcat/logs/                                         |


| ISLE Image                          | Container    | Software / Service | Version            | Logs Path on Container |
| ------------                        | ------------ | ------------       | -------------      | -------------          |
| [islandoracollabgroup/isle-mysql](https://hub.docker.com/r/islandoracollabgroup/isle-mysql/) | isle-mysql | Docker Tag | [latest, 5.6](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/mysql/Dockerfile) | |
|  uses the official Docker base image |             | MySQL (server)     | 5.6                | /var/log/mysql/  |
| [mysql:5.6](https://hub.docker.com/_/mysql/)|      |                    |                    |                  |

| ISLE Image                          | Container    | Software / Service | Version         | Logs Path on Container          |
| ------------                        | ------------ | ------------       | -------------   | -------------                   |
| [islandoracollabgroup/isle-solr](https://hub.docker.com/r/islandoracollabgroup/isle-solr/)| isle-solr | Docker Tag | [latest, 4.10.4](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/solr/Dockerfile)  |                                 |
| uses the official Docker base image |              | OpenJDK            | JDK 8.x         |                                 |
| [tomcat:8.0-jre8](https://hub.docker.com/_/tomcat/)| | Solr             | 4.10.4          | /usr/local/tomcat/logs/solr.log |
|                                     |              | Tomcat             | 8.x             | /usr/local/tomcat/logs/         |

| ISLE Image                          | Container    | Software / Service | Version       | Logs Path on Container |
| ------------                        | ------------ | ------------       | ------------- | -------------          |
| [islandoracollabgroup/isle-proxy](https://hub.docker.com/r/islandoracollabgroup/isle-proxy/) | isle-proxy | Docker Tag | [latest, 1.13](https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/proxy/Dockerfile) |                        |
| uses the official Docker base image |              | Nginx              | 1.13          |                        |
| [nginx:latest](https://hub.docker.com/_/nginx/)    |                    |               |                        |
