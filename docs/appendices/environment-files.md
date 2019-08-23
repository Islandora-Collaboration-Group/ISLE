<!--- PAGE_TITLE --->

# ISLE Environment Files
The .env and tomcat.env Docker files are located in the folder with the docker-compose.yml, and are key to configuring the ISLE stack to suit your needs. Here are the variables and their job:

## .env (Docker Environment File)

  * `COMPOSE_PROJECT_NAME=isle-production`
      * This variable is appended to Docker objects like volume and network names.
  * `BASE_DOMAIN=mydomain.edu`
      * This variable specifies your domain name.
  * `CONTAINER_SHORT_ID=ld`
      * Okay to leave this as is. Keep this variable short as it is appended to the end of all running containers.


### Database (SQL)
| .env Variable                   | Purpose                               | ISLE Services updated  | What it does                                                                                                                            |
|-------------------------------  |-------------------------------------  |------------------------------------ |---------------------------------------------------------------------------------------------------------------------------------------  |
| MYSQL_ROOT_PASSWORD             | Set the `root` password for MySQL     | MySQL, Fedora, Apache               | Allows Fedora and Apache to update their relevant databases as well as to configure themselves to work together.                        |
| FEDORA_DB                       | Set the name of Fedora database       | MySQL, Fedora                       | Specifies which database to create or use for Fedora data.                                                                              |
| DRUPAL_DB                       | Set the name of the Drupal database   | MySQL, Apache, Fedora               | Specifies which database to create or use for Fedora data. Updates components of Fedora so it can read the Drupal database for users.   |
| FEDORA_DB_USER DRUPAL_DB_USER   | Sets the MySQL user                   | MySQL, Apache, Fedora               | Specifies names of Database users.                                                                                                      |
| FEDORA_DB_PASS DRUPAL_DB_PASS   | Sets MySQL user passwords             | MySQL, Apache, Fedora               | Specifies passwords of Database users.                                                                                                  |

### Islandora (Drupal)
| .env Variable       | Purpose                                     | ISLE Services updated  | What it does                                                                                                                                                                                    |
|-------------------- |-------------------------------------------  |------------------------------------ |------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| DRUPAL_SITE_NAME    | Sets the Drupal Site Name variable          | Drupal                              | Sets the name of your Islandora Website; e.g.:  "Your Project Name"                                                                                                       |
| DRUPAL_ADMIN_USER   | Set the name of Drupal admin                | Drupal                              | Specifies the 'admin user' for your Islandora website.                                                                                                                                          |
| DRUPAL_ADMIN_PASS   | Set the password of Drupal admin            | Drupal                              | Specifies the password of 'admin user' for your Islandora website.                                                                                                                              |
| DRUPAL_ADMIN_EMAIL  | Set the email of Drupal admin               | Drupal                              | Specifies the email address of the 'admin user' for your Islandora site.                                                                                                                        |
| DRUPAL_HASH_SALT    | Secures your installation by hashing data   | Drupal                              | Secures your install of Drupal (Islandora) by hashing (obscuring) key data. Use [password generator tool](https://passwordsgenerator.net/) to create a HASH_SALT, remember alphanumeric characters ONLY (no special characters).  |

### Fedora Repository
| .env Variable         | Purpose                           | ISLE Services updated           | What it does                                                                                                                                                |
|---------------------  |---------------------------------- |-------------------------------  |------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| FEDORA_ADMIN_USER     | Set the name of Fedora admin      | Fedora                          | Sets the master administrator username to login to the Fedora repository. You login with this user.                                                         |
| FEDORA_ADMIN_PASS     | Set the password of Fedora admin  | Fedora                          | Sets the master administrator password to login to the Fedora repository.                                                                                   |
| FEDORA_GSEARCH_USER   | Sets the username for FGS         | Fedora, Fedora Generic Search   | Sets the Fedora Generic Search (FGS) username used to login to the Fedora repository. FGS uses this user to connect to Fedora to update your search index.  |
| FEDORA_GSEARCH_PASS   | Sets the password for FGS         | Fedora, Fedora Generic Search   | Sets the password for FGS to gain access to your repository.                                                                                                |
| FEDORA_INTCALL_USER   | Sets the internal call user       | Fedora                          | The internal call username                                                                                                                                  |
| FEDORA_INTCALL_PASS   | Sets the internal call password   | Fedora                          | The internal call password                                                                                                                                  |

### Image Services
| .env Variable                       | ISLE Services updated   | What it does                                                                                                                          |
|-----------------------------------  |-----------------------  |-------------------------------------------------------------------------------------------------------------------------------------- |
| CANTALOUPE_ADMIN_INTERFACE_ENABLE   | ImageServices           | Enables or Disables the Cantaloupe IIIF /admin control panel. Locatied at http://hostip:8083/cantaloupe/admin when true, else false.  |
| CANTALOUPE_ADMIN_USER               | ImageServices           | Set the admin username to login to the admin panel.                                                                                   |
| CANTALOUPE_ADMIN_PASS               | ImageServices           | Set the admin password to login to the admin panel.                                                                                   |

## tomcat.env (Docker Environment File)
| Tomcat.env Variable   | ISLE Services updated         | What it does                                            |
|---------------------  |-----------------------------  |-------------------------------------------------------  |
| TOMCAT_ADMIN_USER     | Fedora, Solr, ImageServices   | Set the admin username to login to the admin panel.     |
| TOMCAT_ADMIN_PASS     | Fedora, Solr, ImageServices   | Set the admin password to login to the admin panel.     |
| TOMCAT_MANAGER_USER   | Fedora, Solr, ImageServices   | Set the manager username to login to the admin panel.   |
| TOMCAT_MANAGER_PASS   | Fedora, Solr, ImageServices   | Set the manager password to login to the admin panel.   |

