The MySQL container will be using the following versions:


* [Latest (5.6) - Dockerhub](https://hub.docker.com/_/mysql/)
* [Latest (5.6) - Github Repository](https://github.com/docker-library/mysql/blob/d8e7c8b6c0ba854fd5ea8257b79a2b3377f668ae/5.6/Dockerfile)

* [Legacy (5.5) - Dockerhub](https://hub.docker.com/_/mysql/)
* [Legacy (5.5) - Github Repository](https://github.com/docker-library/mysql/blob/41cfbdd89323a1230342038757b5b8f7f2f36b74/5.5/Dockerfile)

---

### Docker Compose service description
This container will only create an empty MySQL `fedora3` database (upon launch) using the Docker `environment` setting (displayed above). The Drupal site and/or multi-site databases will need to be created post launch.

** Please note: This container must be created and launched first, prior to all other containers.**

Within the docker-compose.yml file, found at the root of the ISLE project Github repository, this container is described / controlled as follows:

>  db:  
    build: ./mysql  
    image: mysql:5.6  
    environment:  
      - MYSQL_ROOT_PASSWORD=islemysqlrootpw2017  
      - MYSQL_DATABASE=fedora3  
      - MYSQL_USER=fedora_admin  
      - MYSQL_PASSWORD=islefeddb2017  
    ports:  
      - "3306:3306"  
    volumes:  
      - ./data/mysql:/var/lib/mysql  
    container_name: isle-islandora-db  

### Docker Compose commands:
  * To build only this container (no others): `docker-compose build db`  
  * To create only this container (no others): `docker-compose create db`
  * To start only this container (no others): `docker-compose up -d db`  

### Image Name
* Latest `mysql:5.6 (db)`
* Legacy `mysql:5.5.55 (db)`

### Container name(s)
`isle-db`

### Databases

* `fedora3` Fedora database created by build process (empty)
* `islesite` Drupal site database created by build process (empty, default)

| User         | Password            | Database         | Description                                                                  |
------------   | -------------       | :-------------:  | -------------                                                                |
root           | islemysqlrootpw2017 | ALL              | has access to all databases with all db privileges                           |
fedora_admin   | islefeddb2017       | fedora3          | has access to only the `fedora3` Fedora database. No grant privileges.       |
islandora_user | islandoraisledb2017 | isle_site         | has access to only the `islesite` Drupal site database. No grant privileges. |

### Ports
Port 3306 on the MySQL container is mapped to 3306 on Host

### Notes
*Please only review this if building containers manually:*  

The `docker-entrypoint.sh` file contained within the `mysql` folder next to the Dockerfile needs to have `755` file permissions prior to the build.  

**Example:**  
Run `chmod 755 docker-entrypoint.sh` to fix PRIOR to the build.
