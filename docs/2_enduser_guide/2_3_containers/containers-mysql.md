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

>db:
  build: ./mysql
  image: isle-db:latest
  environment:
    - MYSQL_ROOT_PASSWORD=islemysqlrootpw2017
    - MYSQL_DATABASE=fedora3
    - MYSQL_USER=fedora_admin
    - MYSQL_PASSWORD=islefeddb2017
  ports:
    - "3306:3306"
  volumes:
    - ./data/mysql:/var/lib/mysql
    - ./mysql/initscripts:/docker-entrypoint-initdb.d
  container_name: isle-db

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
* `isle_site` Drupal site database created by build process (empty, default)

Per the guidelines of the official MySQl Docker image [README](https://hub.docker.com/_/mysql/), section `Initializing a fresh instance`:

> *"When a container is started for the first time, a new database with the specified name will be created and initialized with the provided configuration variables. Furthermore, it will execute files with extensions .sh, .sql and .sql.gz that are found in /docker-entrypoint-initdb.d. Files will be executed in alphabetical order. You can easily populate your mysql services by mounting a SQL dump into that directory and provide custom images with contributed data. SQL files will be imported by default to the database specified by the MYSQL_DATABASE variable."*"

By default, the `docker-compose.yml` script creates the `fedora3` database by setting environmental variables in the `environment` section of the `db` service.

The second database `isle_site` is created when the mount process `./mysql/initscripts:/docker-entrypoint-initdb.d` occurs upon container launch.

Within this project directory, there is a directory `initscripts` which contains a SQL script `siteinit.sql`

```
├── README.md
├── data
├── docker-compose.yml
├── docs
├── fedora
├── mkdocs.yml
├── mysql
│   ├── Dockerfile
│   ├── docker-entrypoint.sh
│   └── initscripts
│       └── siteinit.sql
├── site
└── web
```

The default contents of this script are:

**Example**

`CREATE DATABASE isle_site CHARACTER SET utf8 COLLATE utf8_general_ci;`  
`CREATE USER islandora_user@'127.0.0.1' IDENTIFIED BY 'islandoraisledb2017';`  
`GRANT SELECT, INSERT, UPDATE ON islandora_docker.* TO 'islandora_user'@'127.0.0.1'";`  

This script can be modified to create more databases upon container launch.

---

| User         | Password            | Database         | Description                                                                  |
------------   | -------------       | :-------------:  | -------------                                                                |
root           | islemysqlrootpw2017 | ALL              | has access to all databases with all db privileges                           |
fedora_admin   | islefeddb2017       | fedora3          | has access to only the `fedora3` Fedora database. No grant privileges.       |
islandora_user | islandoraisledb2017 | isle_site         | has access to only the `isle_site` Drupal site database. No grant privileges. |

### Ports
Port 3306 on the MySQL container is mapped to 3306 on Host

### Volumes
* `./data/mysql:/var/lib/mysql`

This volume is created to allow MySQL databases to persist. All MySQL data is stored here. The container can be turned off, destroyed or upgraded and this data will remain.

* `./mysql/initscripts:/docker-entrypoint-initdb.d`

This volume is created to allow for the creation of additional databases beyond the initial `fedora3` creation. A default empty Drupal site database is also supplied using a SQL script file called `siteinit.sql` found within the `initscripts` directory. By mounting this directory directly into the container's `docker-entrypoint-initdb.d` directory, the MySQL service is able to run the SQL script automatically upon container launch.

### Notes
*Please only review this if building containers manually:*  

The `docker-entrypoint.sh` file contained within the `mysql` folder next to the Dockerfile needs to have `755` file permissions prior to the build.  

**Example:**  
Run `chmod 755 docker-entrypoint.sh` to fix PRIOR to the build.
