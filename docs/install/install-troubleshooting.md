# ISLE Installation: Troubleshooting

**Please select a topic:**

- [Port Conflicts](#port-conflicts)
- [Non-Running Docker Containers](#non-running-docker-containers)

---

## Port Conflicts
If you encounter an error like this:

`Error starting userland proxy: Bind for 0.0.0.0:xxxx failed: port is already allocated`

then ISLE may have encountered a conflict with the `xxxx` port identified in the error message.

In MacOS, this can frequently be caused by a local Apache or Nginx web server, or local MySQL server.  

You may need to remove or disable these local web servers before you can successfully install ISLE.

* If you have a local Apache web server that ships with most MacOS machines may conflict with port 80, and can usually be disabled from a terminal using these commands:

    * `sudo apachectl stop`
    * `sudo launchctl unload -w /System/Library/LaunchDaemons/org.apache.httpd.plist 2>/dev/null`

* If you have a local `Nginx` web server it may conflict with port 8080, and it can usually be disabled from a terminal using:

    * `sudo nginx -s stop`

Once your web server(s) have been disabled, resume the ISLE install process by repeating your last installation command, presumably `docker-compose up -d`.


**Return to [Demo ISLE Installation](../install/install-demo.md).**

---

## Non-Running Docker Containers
If you don't see all containers running, then stop the running containers with `docker-compose down` and start the containers one at a time following the instructions below:

*  MySQL image pull & container launch

    `docker pull islandoracollabgroup/isle-mysql:1.2.0`

    `docker-compose up -d mysql`

*  Fedora image pull & container launch

    `docker pull islandoracollabgroup/isle-fedora:1.2.0`

    `docker-compose up -d fedora`

    (Optional but recommended troubleshooting step)

    * Please note the Tomcat service requires about  one to three minutes to startup and as such if the end user rushes to the URL supplied below, the service page may time out or be reported as unreachable. Give it a little time.
    * After spinning up fedora container, check if the Fedora service is running prior to advancing.
    * Navigate to http://hostip:8081/manager/html a popup login prompt should appear.
    * Enter the user name of `admin` and the password of `isle_admin`
    * Upon login a large display of running Tomcat applications should display, scroll down to fedora
    * The application state / status should be true
    * If false appears instead, attempt to restart the fedora service manually.
    * Select the restart button to the right of the status area.
    * If it still fails, review the mounted fedora logs. The docker-compose.yml file will indicate where the logs are located.
    * Using terminal and then entering a command like `tail -n 300 - <path to ISLE project/data/fedora/log/tomcat:/usr/local/tomcat/logs/fedora.log` should display enough information to troubleshoot or restart the entire startup process.

* Solr image pull & container launch

    `docker pull islandoracollabgroup/isle-solr:1.2.0`

    `docker-compose up -d solr`

* Apache image pull & container launch

    `docker pull islandoracollabgroup/isle-apache:1.2.0`

    `docker-compose up -d apache`

## Viewing Logs in ISLE 1.2.0

As of ISLE release logging to physical file has been turned off, stdout & stderr are to console only no more physical files. This means if you need to view logs for debugging, here are some methods:

* [Docker method:](https://docs.docker.com/engine/reference/commandline/logs/) (_when using the Docker json driver and TICK is not on_)
  * Single container: `docker logs -f <container-name>`
  * All containers: `docker logs --tail=0 --follow`

* [Docker-compose method:(https://docs.docker.com/compose/reference/logs/)] (when using the Docker json driver and TICK is not on)
Single container: docker-compose logs -f  <container-name>
All containers: docker-compose logs --tail=0 --follow

* Use the [TICK Log viewer](../optional-components/tickstack.md) if TICK is setup and using the Docker syslog driver (Production / Staging only) 

---

Pick one of the following:
**Return to [Demo ISLE Installation](../install/install-demo.md).**
**Return to [Local ISLE Installation: New site](../install/install-local-new.md).**
**Return to [Local ISLE Installation: Migrate existing Islandora site](../install/install-local-migrate.md).**
**Return to [Staging ISLE Installation: New site](../install/install-staging-new.md).**
**Return to [Staging ISLE Installation: Migrate existing Islandora site](../install/install-staging-migrate.md).**
**Return to [Production ISLE Installation: New site](../install/install-production-new.md).**
**Return to [Production ISLE Installation: Migrate existing Islandora site](../install/install-production-migrate.md).**