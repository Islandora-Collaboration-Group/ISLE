# TICK Stack

## Use Cases & User Stories

* As a repository administrator, I expect to be able to view real-time and some historic systems/performance metrics without digging around in server logs.
* As a repository administrator, I should be able to use the dashboard to troubleshoot the repository (_like when it’s hanging or objects are slow to load_)

* As a repository owner/manager, I expect logging to be handled in a way that does not endanger my systems health and performance over time.

* As a repository owner/manager, I expect to be able to set logging levels for all relevant log streams.

* As a repository administrator, I expect to be able to view in a web dashboard:
    * Real-time and recent historic systems/performance metrics
    * Real-time and recent historic system and application logs

* As a repository administrator, I expect to be able to use monitoring tools that do not burden the target server thus I can setup monitoring tools remotely and pull data from the ISLE server for review.

---

## New Functionality

The "TICK" stack is a suite of open-source applications that are used together for the storing, visualization and monitoring of data (time series metrics) gathered from running systems. The TICK stack was created and is developed by [Influxdata](https://www.influxdata.com) which is _"...dedicated to open source and truly cares about helping developers get to results faster with less complexity and less code."_

The TICK stack is used with ISLE to:

* Monitor running systems in Development, Staging or Production
    * Do note that users can monitor **ANY** system or systems with these tools, not just ISLE-related platforms.

* Create alerts that can be sent to email, text or Slack™ to warn users, system-administrators of potential issues, storage or memory challenges and much more.

* As a one-stop "search-engine" of log and metric data for all ISLE containers, services and applications, host servers, etc.

* Help ISLE users to better understand how their systems work, how Islandora works and to greatly improve visibility on applications, services and processes within ISLE

The use and setup of TICK within the ISLE platform is as an optional [sidecar](https://docs.microsoft.com/en-us/azure/architecture/patterns/sidecar) to ISLE. There are various ways one can setup TICK in general as a software, as services on the ISLE Host server or to be run as Docker containers.

* TICK is comprised of the following main services and software:

    * [**T**elegraf](https://www.influxdata.com/time-series-platform/telegraf/) - a plugin-driven server agent for collecting and sending metrics and events from databases, systems, and IoT sensors.

    * [**I**nfluxdb](https://www.influxdata.com/time-series-platform/) - the open source time series database   is designed to handle high write and query loads and provides a SQL-like query language called InfluxQL   for interacting with data.
    
    * [**C**hronograf](https://www.influxdata.com/time-series-platform/chronograf/) - the user interface and   administrative component for using templates and libraries to rapidly build dashboards with real-time   visualizations.
    
    * [**K**apacitor](https://www.influxdata.com/time-series-platform/kapacitor/) - the native data processing engine which can process both stream and batch data from InfluxDB, acting on this data in real-time via its programming language TICKscript.

---

## System Requirements

* [ISLE](https://github.com/Islandora-Collaboration-Group/ISLE) release version `1.2.0`
    * `git clone https://github.com/Islandora-Collaboration-Group/ISLE.git`

### ISLE Images

* Following the installation steps below, an enduser will configure  / edit their ISLE running system(s) to ultimately use the following images and tags from Docker-Hub:


| Service | Repository | Tag |
| ---     | ---        | --- | 
| Apache | [islandoracollabgroup/isle-apache](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-apache/tags) | `1.2.0`|
| Fedora | [islandoracollabgroup/isle-fedora](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-fedora/tags) | `1.2.0`|
| Image-services | [islandoracollabgroup/isle-imageservices](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-imageservices) | `1.2.0` |
| MySQL | [islandoracollabgroup/isle-mysql](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-mysql) | `1.2.0` |
| Portainer | [portainer/portainer](https://hub.docker.com/r/portainer/portainer) | `latest` |
| Solr  | [islandoracollabgroup/isle-solr](https://cloud.docker.com/u/islandoracollabgroup/repository/docker/islandoracollabgroup/isle-solr/tags) | `1.2.0` |
| Traefik | [traefik/traefik](https://hub.docker.com/_/traefik) | `1.7.9` |

* Additional systems overhead, including:

* For any system that is running the full TICK stack as a side car
    * Recommend an additional 2-3 GB of RAM
    * Recommend an additional 5-10 GB of storage disk space

* For any system that is running the Telegraf agent only
    * Recommend an additional 512MB to 1 GB of RAM

* Use the `docker-compose-tick.yml` file for setup and/or modification. This file will have not only the correct setup with the TICK stack but the necessary `syslog` driver code and tagging.

* Use the `latest` tags for the TICK base images
    * `image: telegraf:latest`
    * `image: influxdb:latest`
    * `image: chronograf:latest`
    * `image: kapacitor:latest`

* [Rsyslog](https://www.rsyslog.com/) has to be installed on any ISLE Host server that is to be monitored whether that system is running fullstack TICK or simply the Telegraf agent.

    * Rsyslog is used to help the Telegraf agent forward log messages from Docker, the running ISLE containers and the host system to Influxdb.

    * Use the default Ansible script in the ISLE repository that can install `rsyslog` on any host server.

    * Manual options
        * [Red Hat / CentOS](https://www.rsyslog.com/rhelcentos-rpms/)
        * [Ubuntu / Debian](https://www.rsyslog.com/ubuntu-repository/)

* (_Optional_) Use some of the "canned" `.tick` (_tickscript_) templates to create dashboards and alerts provided in the `./config/tick/chronograf/tickscripts` directory. See the section Templates for more detail. (TO DO)

---

### Telegraf Agent Reporting Plugins

The Telegraf agent used for ISLE has a default configuration which runs the following plugins to monitor various services whether it is on a Production or Staging system.

There are three `.conf` files found within `./config/tick/telegraf`
  * `telegraf.conf` - Use this as a genric configuration for any system you'd like to monitor. Use this template file as a method to monitor ISLE environments that are not Staging or Production
  * `telegraf.staging.conf` - Edit this file to run a full TICK stack on your ISLE Staging server which can monitor both your Staging and Production systems. 
  * `telegraf.production.conf` - Edit this file to point to your full TICK stack running on your ISLE Staging server.

* To review or configure plugins, edit the appropriate `telegraf.conf` file for your environment and navigate to the `INPUT PLUGINS` section.

If an ISLE User would like to add a plugin to monitor additional services, please review the additional information below on Telegraf plugins which can be found in two places:

* Telegraf Plugins Documentation https://docs.influxdata.com/telegraf/v1.10/plugins/inputs/
* Individual plugin's README.md files on Github.com https://github.com/influxdata/telegraf

#### List of Telegraf Agent Plugins used for ISLE:

* `inputs.cpu` - [Read metrics about cpu usage](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/cpu/README.md)

* `inputs.disk` - [Read metrics about disk usage by mount point](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/disk/README.md)

* `inputs.diskio` - [Read metrics about disk IO by device](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/diskio/README.md)

* `inputs.kernel` - [Get kernel statistics from /proc/stat](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/kernel/README.md)

* `inputs.mem` - [Read metrics about memory usage](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/mem/README.md)

* `inputs.swap` - [Read metrics about swap memory usage](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/swap/README.md)

* `inputs.system` - [Read metrics about system load & uptime](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/system/README.md)

* `inputs.docker` - [Read metrics about docker containers](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/docker/README.md)

* `inputs.mysql` -  [Gathers the statistics data from MySQL servers](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/mysql/README.md)

* `inputs.net` - [Gathers metrics about network interface usage (Linux only).](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/net/NET_README.md)

* `inputs.solr` - [Gathers Solr stats](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/solr/README.md)

* `inputs.syslog` - [Gather metrics from the host syslog service and for log sending](https://github.com/influxdata/telegraf/tree/master/plugins/inputs/syslog)

* `inputs.varnish` - [Gather metrics from a Varnish cache](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/varnish/README.md)

---

## Adoption Process Overview

The installation instructions below will walk you through how to run the full TICK stack on a Staging ISLE host system but only run  a Telegraf container (agent) on the Production system. This will be referred to from now on as the "sidecar" setup.

The data from both systems will be collected, analyzed and accessed on / from the the Staging host system via a dashboard called Chronograf. You can also setup alerts from this dashboard too.

* You'll start by backing up all important data as needed.

* You'll stop any running containers

* You'll download new ISLE images tagged as `1.2.0`


- You'll copy over a new configuration file for a service called `rsyslog`. 
    * This will allow TICK to get information from the ISLE Host server `syslog` logger.
    * You may also need to ensure that the `rsyslog` service and software is installed on your ISLE Host Server as well.

* You'll copy over a new Docker `daemon.json` file to get Docker the right log driver; which will change from `json` to `syslog`.

* You'll make additional edits and modifications to the following ISLE configuration files:
    * `docker-compose.staging.yml`
    * `docker-compose.production.yml`  
    * `staging.env`
    * `production.env`  

* You'll edit the Staging Telegraf Agent's empty or default settings to properly monitor the various local ISLE services and send metrics to the local Influxdb database on the Staging system.

* You'll restart your containers with the new services having been added and configured.

* Using a web browser, you'll navigate to the new Chronograf dashboard on the Staging server and complete the configuration of the Influxdb, Chronograf and Kapacitor services.
    * You'll have the option to create new dashboards
    * If desired, you'll add additional alerts

- You'll repeat the steps to edit the Telegraf Agent's configuration for your Production system only. You do not need to install the remaining "ICK" stack on Production though.

* You can then review the `Using Chronograf` section to learn how to review the new added hosts, dashboards and Log Viewer to review ISLE services log streams.

* Lastly and optionally, you can setup alerts in Chronograf to warn you and your team of when certain events or issues have occurred on your newly monitored systems.

---

## Installation

### Assumptions

* Previously installed and running Production and Staging ISLE Host systems are in place already

* You'll need to use the ISLE images tagged as `1.2.0` and higher for the syslog driver changes to be in place.

* That the "sidecar" method will be the installation type running on the Staging system to receive data from Staging and Production.

* A firewall configuration that allows incoming public traffic to port `8086` traffic

* A firewall configuration that allows incoming private traffic access to port `8888`. It is recommended that this not be general public access but whitelisted to a select range of IP addresses given this will be the `Chronograf` dashboard port. Trusted users only please.

* Only installation on Linux based host systems e.g. Debian / Ubuntu 18.04+ and Red Hat / CentOS 7.x+
    * Attempting to run TICK locally on a MacOS system can fail https://github.com/docker/for-mac/issues/3303

* Other TICK / ISLE configurations are possible but **are not supported or detailed** as of yet in this documentation. Endusers seeking alternative setup possibilities are encouraged to further review and mine from the official Influxdata [documentation](https://docs.influxdata.com/platform/introduction/).

---

### Installation Instructions

* Shut down all running ISLE containers, Staging first, then production second once Staging is up and running again.

---

#### 01. Docker syslog driver setup

* You will need to use the `sudo` command or become the `root` user for the steps below. Copying this file allows the Docker Daemon to use the `syslog` driver for log files and reporting to the `telegraf` agent.

* Check if you already have an existing `/etc/docker/daemon.json` file. 
    * If **YES**, add / merge  the contents of `./config/tick/docker/daemon.json` with your current `/etc/docker/daemon.json` file.
    * If **NO**, copy `./config/tick/docker/daemon.json` to `/etc/docker/`
        * **Example:** `sudo cp ./config/tick/docker/daemon.json /etc/docker/`

* Restart the Docker service
    * `sudo service docker restart` or `sudo systemctl restart docker`
    * You can check if the service is running with `sudo service docker status`

---

#### 02. Rsyslog setup

* You will need to use the `sudo` command or become the `root` user for the steps below. Copying this file allows the information from the host `syslog` be forwarded to the `telegraf` agent.

* Copy the Telegraf configuration file to the Rsyslog.d directory so Rsyslog will forward logs to the Telegraf agent with correct formatting. Assumes you are in the ISLE project root directory.
  
    * `sudo cp ./config/tick/rsyslog/50-telegraf.conf /etc/rsyslog.d/`

* Please note on `Ubuntu` systems, you will need to also reset the `/var/spool/rsyslog` permissions due to a bug. This has not been observed behavior on `CentOS` Host servers _yet_.
  
  * Reset permissions process:
      * `sudo chown -Rv syslog:adm /var/spool/rsyslog`
  
  * Restart the `rsyslog` service 
      * `sudo service rsyslog restart` or `sudo systemctl restart rsyslog`
  
  * You can check if the service is running
      * `sudo service rsyslog status`

**Example output:**

```bash

● rsyslog.service - System Logging Service
   Loaded: loaded (/lib/systemd/system/rsyslog.service; enabled; vendor preset: enabled)
   Active: active (running) since Tue 2019-06-04 10:56:26 EDT; 3s ago
     Docs: man:rsyslogd(8)
           http://www.rsyslog.com/doc/
 Main PID: 11815 (rsyslogd)
    Tasks: 5 (limit: 4634)
   CGroup: /system.slice/rsyslog.service
           └─11815 /usr/sbin/rsyslogd -n

Jun 04 10:56:26 ip-172-31-40-28 systemd[1]: Starting System Logging Service...
Jun 04 10:56:26 ip-172-31-40-28 rsyslogd[11815]: imuxsock: Acquired UNIX socket '/run/systemd/journal/syslog' (fd 3) from systemd.  [v8.32.0]
Jun 04 10:56:26 ip-172-31-40-28 systemd[1]: Started System Logging Service.
Jun 04 10:56:26 ip-172-31-40-28 rsyslogd[11815]: rsyslogd's groupid changed to 106
Jun 04 10:56:26 ip-172-31-40-28 rsyslogd[11815]: rsyslogd's userid changed to 102
Jun 04 10:56:26 ip-172-31-40-28 rsyslogd[11815]:  [origin software="rsyslogd" swVersion="8.32.0" x-pid="11815" x-info="http://www.rsyslog.com"] start

```

* You may find that that rsyslog starts to show errors, this is typically due to the containers not having been start up yet. You will restart this service in a later step to resolve these potential errors. 

```bash
sudo service rsyslog status

● rsyslog.service - System Logging Service
   Loaded: loaded (/lib/systemd/system/rsyslog.service; enabled; vendor preset: enabled)
   Active: active (running) since Tue 2019-08-13 17:15:46 UTC; 8min ago
     Docs: man:rsyslogd(8)
           http://www.rsyslog.com/doc/
 Main PID: 9995 (rsyslogd)
    Tasks: 5 (limit: 4915)
   CGroup: /system.slice/rsyslog.service
           └─9995 /usr/sbin/rsyslogd -n

Aug 13 17:23:22 ip-172-31-69-204 rsyslogd[9995]: action 'action 8' resumed (module 'builtin:omfwd') [v8.32.0 try http://www.rsyslog.com/e/2359 ]
Aug 13 17:23:32 ip-172-31-69-204 rsyslogd[9995]: omfwd: TCPSendBuf error -2027, destruct TCP Connection to 127.0.0.1:6514 [v8.32.0 try http://www.rsyslog.com/e/2027 ]
Aug 13 17:23:32 ip-172-31-69-204 rsyslogd[9995]: action 'action 8' suspended (module 'builtin:omfwd'), retry 0. There should be messages before this one giving the reason for suspension. [v8.32.0 try http://www.rsyslog.
```

---

#### 03. Edits - docker-compose.staging.yml file

* Edit the `docker-compose.staging.yml` file and uncomment lines to enable the new `TICK` services. There is a section called `# Start - TICK stack services section`, underneath that is all of the `TICK` code to be uncommented. (_remove the # before and ensure all code aligns properly like the services above it._)

* At the end of every service definition (_mysql, fedora, solr etc_) within the `docker-compose.staging.yml`, uncomment the following:

```bash
#    logging:
#      driver: syslog
#      options:
#        tag: "{{.Name}}"
```

Uncommented example:

```bash
  mysql:
    image: islandoracollabgroup/isle-mysql:1.2.0
    container_name: isle-mysql-${CONTAINER_SHORT_ID}
    networks:
      - isle-internal
    ports:
      - "3306:3306"      
    volumes:
      - isle-db-data:/var/lib/mysql
      - isle-mysql-log:/var/log/mysql
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

* Uncomment the lines below for the TICK stack data volumes to the end of the `volumes` section of the `docker-compose.staging.yml` file.

Uncommented TICK volumes example:

```bash
volumes:
  isle-db-data:
  isle-solr-data:
  isle-portainer-data:
  isle-fedora-resourceIndex:
  isle-fedora-activemq:
  isle-fedora-XACML:
  isle-influxdb-data:
  isle-kapacitor-data:
  isle-chronograf-data:
```

---

#### 04. Telegraf Agent configuration on Staging

**Please note:** These instructions are for configuring the Telegraf Agent that runs on the same system as the "ICK" stack i.e. Influxdb, Chronograf and Kapacitor do, typically your ISLE Staging system.

The instructions to setup only the Telegraf Agent on an ISLE system e.g. your Production system are found below in the section called `06. Telegraf Agent (Agent only) installation on Production`.


* You'll need to edit the `./config/tick/telegraf/telegraf.staging.conf` to add the following:
    * `hostname = ""`
        * Enter the name of the server you will be monitoring e.g. `isle-server-staging` or `icg-staging` etc. This value can be a [FQDN](https://kb.iu.edu/d/aiuv), an IP or any name really.
    * `database = "telegraf"`
        * By default, the ISLE TICK setup assumes this database as it is the easiest but pools all data received by individual monitored hosts into one database. For first time users, recommend leaving this value in place.
        * Users are free to change this to any value to segregate data by systems, group etc.
    * Change `servers = ["root:<ISLE_ROOT_PASSWORD_HERE>@tcp(mysql:3306)/?tls=false"]` to use your Staging MySQL Root password. Typically this value is in your `staging.env` file. Swap out the `<ISLE_ROOT_PASSWORD_HERE>` with your Staging MySQL Root password.


* Start up the ISLE Docker containers again. `docker-compose up -d`

* Depending on your internet connection, this startup process may take a few minutes, as the new TICK images are being downloaded and started.

* As a precaution, restart the `rsyslog` service once the ISLE Docker containers have reported that they have started up without issue.
    * `sudo service rsyslog restart` or `sudo systemctl restart rsyslog`
  
  * You can check if the service is running
      * `sudo service rsyslog status`
      * Note the any previous errors have stopped.

```bash

sudo service rsyslog status
● rsyslog.service - System Logging Service
   Loaded: loaded (/lib/systemd/system/rsyslog.service; enabled; vendor preset: enabled)
   Active: active (running) since Tue 2019-08-13 17:24:49 UTC; 8s ago
     Docs: man:rsyslogd(8)
           http://www.rsyslog.com/doc/
 Main PID: 28257 (rsyslogd)
    Tasks: 5 (limit: 4915)
   CGroup: /system.slice/rsyslog.service
           └─28257 /usr/sbin/rsyslogd -n

Aug 13 17:24:49 ip-172-31-69-204 systemd[1]: Starting System Logging Service...
Aug 13 17:24:49 ip-172-31-69-204 rsyslogd[28257]: imuxsock: Acquired UNIX socket '/run/systemd/journal/syslog' (fd 3) from systemd.  [v8.32.0]
Aug 13 17:24:49 ip-172-31-69-204 rsyslogd[28257]: rsyslogd's groupid changed to 106
Aug 13 17:24:49 ip-172-31-69-204 systemd[1]: Started System Logging Service.
Aug 13 17:24:49 ip-172-31-69-204 rsyslogd[28257]: rsyslogd's userid changed to 102
Aug 13 17:24:49 ip-172-31-69-204 rsyslogd[28257]:  [origin software="rsyslogd" swVersion="8.32.0" x-pid="28257" x-info="http://www.rsyslog.com"] start
```

---

#### 05. Chronograf Dashboard setup on Staging

* To configure and setup your new TICK stack, navigate to http://**<replacethiswithyourdomain_here>**:8888

* Click the `Get Started` button on the `Welcome to Chronograf` page.

* The `InfluxDB connection` page should now appear. Change the values within the following fields.

    * `Connection URL` - Change `http://localhost:8086` to `http://influxdb:8086`

    * `Connection Name` - Change `Influx 1` to simply `Influx`

    * Add a `Username` and / or `Password`. This can be any `username` & `password` combination of your choice. If you are just testing, you can leave this blank to move forward but this is not recommended.

    * `Telegraf Database Name`: Keep this as the default `telegraf`. You can add additional databases and hosts as needed later.

    * `Default Retention Policy`: Keep this blank as the default.

    * Click the blue `Add connection` button at the bottom of the page.

* The `Dashboards` page should now appear. If using the default ISLE TICK setup, then we 
  
    * In the `Suggested Dashboards for your Source` section, single click all three of the following:
        * `MySQL`
        * `Docker`
        * `System`
    * They will get highlighted in green with a checkmark in the top left hand corner.
    * Click the blue `Create 3 Dashboards` button

* The `Kapacitor Connection` page should now appear. Change the values within the following fields.
    * `Kapacitor URL` - Change this from `http://influxdb:9092/` to `http://kapacitor:9092/`

    * `Name` - Change `New Kapacitor` to simply `Kapacitor`

    * Add a `Username` and / or `Password`. This can be any `username` & `password` combination of your choice. If you are just testing, you can leave this blank to move forward but this is not recommended.
    
    * Click the blue `Continue` button

* The `Setup Complete` page should now appear. If everything is working properly, there should be green checkmarks below.

* Click the `View All Connections` button

* The `Connections` page should now appear. If everything is working properly, there should be in green `Influx (Default)` under the `InfluxDB Connection` section.

---

#### 06. Telegraf Agent (Agent only) installation on Production

#### Adoption Process Overview

* Use these instructions to add an additional Docker Telegraf container on a Production system. Once you've followed the instructions below, a new Production host will appear in your Chronograf dashboard for you to review alongside your previously configured Staging host.

#### Assumptions

* A previously installed and running ISLE Host Production system is in place already.

* You'll need to use ISLE version `1.2.0` for the syslog driver changes to be in place.

* That the "sidecar" TICK installation is already in place on the Staging server prior.

* Your firewall allows outbound port `8086` traffic

####  Installation

In the recommended setup and usage of TICK, the Staging server will be running the full stack of TICK as a collection point for all Staging data and any Production system will only need the Telegraf agent installed.

To install the Telegraf agent on a Production system:

* Shut down all running ISLE containers on the Production system

- You'll need to edit the `./config/tick/telegraf/telegraf.production.conf` to add the following:
    * `hostname = ""`
        * Enter the name of the server you will be monitoring e.g. `isle-server-prod` or `icg-production` etc. This value can be a [FQDN](https://kb.iu.edu/d/aiuv) i.e. `production-server.domain.edu`, an IP or any name really. Recommend a short name to be easily read and understood.
    * `urls = ["http://influxdb:8086"]`
        * Replace the `influxdb` value with the [FQDN](https://kb.iu.edu/d/aiuv) of the Staging server that is running the full TICK stack and that this Telegraf agent will be sending data to e.g.
      `urls = ["http://staging-server.domain.edu:8086"]`
    * `database = "telegraf"`
        * By default, the ISLE TICK setup assumes this database as it is the easiest but pools all data received by individual monitored hosts into one database. For first time users, recommend leaving this value in place.
        * Users are free to change this to any value to segregate data by systems, group etc.
    * Change `servers = ["root:<ISLE_ROOT_PASSWORD_HERE>@tcp(mysql:3306)/?tls=false"]` to use your Production MySQL Root password. Typically this value is in your `production.env` file. Swap out the `<ISLE_ROOT_PASSWORD_HERE>` with your Production MySQL Root password.


#### Edits - docker-compose.production.yml file 

* Edit the `docker-compose.production.yml` file and uncomment lines to enable the new `TICK - Telegraf` agent / service. There is a section called `# Start - TICK stack services section`, underneath that is the `TICK` code to be uncommented. (_remove the # before and ensure all code aligns properly like the services above it._)

* At the end of every service definition (_mysql, fedora, solr etc_) within the `docker-compose.production.yml`, uncomment the following:

```bash
#    logging:
#      driver: syslog
#      options:
#        tag: "{{.Name}}"
```

Uncommented example:

```bash
  mysql:
    image: islandoracollabgroup/isle-mysql:1.2.0
    container_name: isle-mysql-${CONTAINER_SHORT_ID}
    networks:
      - isle-internal
    ports:
      - "3306:3306"      
    volumes:
      - isle-db-data:/var/lib/mysql
      - isle-mysql-log:/var/log/mysql
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

* Copy the Telegraf configuration file to the Rsyslog.d directory so Rsyslog will forward logs to the Telegraf agent with correct formatting. Assumes you are in the ISLE project root directory.
    * **Example:** `sudo cp ./config/tick/rsyslog/50-telegraf.conf /etc/rsyslog.d/50-telegraf.conf`

* Copy over the Docker log driver configuration and then restart the Docker service.
    * **Example:** `sudo cp ./config/tick/docker/daemon.json /etc/docker/daemon.json`
    * `sudo service docker restart`

* Start up the ISLE Docker containers again. `docker-compose up -d`

* Depending on your internet connection, this startup process may take a few minutes, as the new TICK images are being downloaded and started.

* Once the Production containers are back online, the services are running e.g. you see the Drupal site, etc. navigate to the `Hosts` section within the `Chronograf` dashboard. You should now see your new Production server name. This means the Telegraf Agent on your Production ISLE system is now reporting properly. 

---

## Troubleshooting

If you are not seeing your server(s) appear in the `Hosts` section nor seeing log information flowing into the `Log Viewer` section, recommend the following:

* Check that any potential firewalls are allowing communication between the Production and Staging servers on port `8086`

* Ensure that all configuration files have been copied over:
    * The new Docker log driver configuration should be found here `/etc/docker/daemon.json`
    * The new rsyslog / Telegraf configuration should be found here: `/etc/rsyslog.d/50-telegraf.conf`

* Check that the TICK related containers are running without exit codes. 
    * `docker ps -a`
  
* Check that the new `Telegraf` docker container is running on Production

* You can check if the Telegraf agent is communicating properly with the Influxdb database and if the Influxdb database is running by using the following command in your terminal:

    * Shell into the telegraph container
        * `docker exec -it isle-telegraf-ld bash`
        * `curl -I http://influxdb:8086/ping`

The output should confirm connectivity by looking like this:

```bash

root@b2e3471b3bbb:/# curl -I http://influxdb:8086/ping

HTTP/1.1 204 No Content
Content-Type: application/json
Request-Id: 9d832e9b-8924-11e9-9314-0242ac180005
X-Influxdb-Build: OSS
X-Influxdb-Version: 1.7.6
X-Request-Id: 9d832e9b-8924-11e9-9314-0242ac180005
Date: Fri, 31 May 2019 13:03:16 GMT
```

* Repeat these instructions from the start if need be.

---

## Using Chronograf

### Adoption Process Overview

* Use these instructions below to learn how to view all monitored hosts, their services and log streams within Chronograf using dashboards and the built-in Log viewer. There are additional optional steps for setting up alerts to warn you and your team by email of when certain events happen e.g. running out of disk space, server crashes, etc.

### Viewing Host Systems

* Using a web browser, navigate to your new Chronograf dashboard e.g. http://**<replacethiswithyourdomain_here>**:8888

* To view hosts, click the `Host List` button to the left. It should appear as an eye symbol in the left navigational panel. Your host(s) should appear in the `Host List`. You can click on the name to investigate the system further.
  
* By default in the `Apps` section, the three dashboards that you just setup i.e. `system`, `docker` and `mysql` should also appear. Click on any one of them to view stats specific to that reporting plugin.

---

### Viewing Logs

* To view logs ordered by hosts, click the `Log Viewer` button to the left. It should appear as cut log symbol in the left navigational panel. Your host(s) should appear in the `Log Viewer`.
    * **Please note:** This will be a lot of information at any given time especially if you connect multiple hosts.

---

### View logs by specific host

Within the `Log Viewer` page:

* Scroll at the bottom of the page over to the `Host` column in the viewer and click on the name e.g. `your-hostname-here`

    * This should now filter out all other host information

    * At the top left hand corner below the search bar, the following appears `host == your-hostname-here`. This is the syntax used for searching by host e.g. `host == your-hostname-here` this is the same shorthand name used to configure the Telegraf agent.

    * Alternatively, you can now use this value `host == your-hostname-here` within the _Search logs using keywords or regular expressions_ search bar.

    * You can even single click into the `host == your-hostname-here` posted query and change the name to a different host.

* You can also search using ISLE key service words to drill down on log reporting. e.g. `apache`, `fedora`, `varnish` etc

---

### Setting up Alerts in Chronograf

#### Adoption Process Overview

The instructions below are going to:

* Setup an "alert handler" in the Chronograf dashboard for delivery of alert messages. We'll use email as the example alert handler service.

* Show you how to set up two basic alerts using your new alert handler
    * `Disk Space` Alert - Alerts you when your disks are running out of space
    * `Deadman` Alert - Alerts you when the ISLE host server is unresponsive, has crashed or is "dead."

#### Assumptions

* Ability to send emails from the ISLE host server has already been setup.
    * You may already be using an existing off host email server or service that is integrated with the ISLE Drupal site.
    * Alternatively your IT dept may provide you with an existing email server or account details as well. 
    * We recommend setting up a [SendGrid](https://sendgrid.com/) or [MailGun](https://www.mailgun.com/) account which are cloud-based email delivery platforms. While both these services have "free" tiers, both are commercial products. However, the ease of use in setting up, using and keeping your ISLE system free of additional complex email service or software installations are why they're recommended here.
    
* Additional alerts may be contributed by the ISLE maintainers and/or community over time.

### Chronograf Alert Handler Setup

While TICK supports a wide variety of alert types and delivery mechanisms we're going to configure the basic alerts using email.

* Prior to starting, you will need to have your SMTP credentials (user name, password and location of the 3rd party email server)  at the ready. This information may have been provided to you by your IT department or from when you may have used one of the recommended cloud-based email delivery platforms;  [SendGrid](https://sendgrid.com/) or [MailGun](https://www.mailgun.com/).

- When you have these credentials, click on the `Configuration` (wrench icon) button on the left hand side of the Chronograf dashboard.

* Under the `Kapacitor Connection` column
    * click into the Kapacitor settings dropdown
    * click on the pencil symbol (_Not the trashcan_)
        * If that doesn't work, try navigating to http://yourdomainhere:8888/sources/1/kapacitors/1/edit 

- Within the `Configure Alert Endpoints` page, click on the `SMTP` section / button and fill in your creds.

    * `SMTP Host` - change `localhost` to the correct host name
    * `SMTP Port` - change `25` to the correct port
    * `From Email` - enter the email account you'll be using to send alerts **with**
    * `To Email` - enter the email account you'll be sending alerts **to**
    * `User` - enter the username for the email account you'll be using to send alerts **with**
    * `Password` - enter the password for the email account you'll be using to send alerts **with**
    * Click the `Configuration Enabled` checkbox
    * Click the blue `Save Changes` button
    * Click the `Send Test Alert` and confirm that a new test email has been sent to the email account you'll be receiving alerts **to /at**

You can now use this delivery mechanism when you set up individual alerts.

---

#### Sendgrid Email:

These instructions are for Sendgrid users only but demonstrate the easy of use in setup and lack of additional infrastructural overhead.

* Create the API Key for the ISLE host server.
    * **Please note**; you should repeat this process for each server e.g. Production and Staging sending email.
* Log into your Sendgrid account
* Navigate to Settings > API Keys
* Click on the `Create API Key` button
* Set the level of API Key Permissions (_recommend Restricted Access_)
    * Set the appropriate level of permissions for each section
* Copy the generated API Key to a password manager. This is the token you'll use to send email with. You'll need to use it again below.

* Log into the Chronograf Dasboard and repeat the steps above to configure the SMTP Alert handler.
    * `SMTP Host` - change `localhost` to `smtp.sendgrid.net`
    * `SMTP Port` - change `25` to `587`
    * `From Email` - enter the email account you'll be using to send alerts **with**
    * `To Email` - enter the email account you'll be sending alerts **to**
    * `User` - Enter the username of `apikey`
    * `Password` - Enter the long API key generated from the above steps 
    * Click the `Configuration Enabled` checkbox
    * Click the blue `Save Changes` button
    * Click the `Send Test Alert` and confirm that a new test email has been sent to the email account you'll be receiving alerts **to /at**

##### Sendgrid Resource

* Sendgrid SMTP [Documentation](https://sendgrid.com/docs/API_Reference/SMTP_API/integrating_with_the_smtp_api.html)

---

### Chronograf Alerts Setup

#### Disk Space Alert

[COMING SOON in the 1.2.1 release]

* This alert is designed to warn you when one of the ISLE host server disk(s) is/are almost full. Please note this setting should not alert you when the disk is already full. We suggest you use a lower value (e.g. 75%) such that you can safely shutdown containers, backup data and then expand the affected disk's capacity as needed.

#### Deadman Alert

[COMING SOON in the 1.2.1 release]

* This alert is designed to warn you when one of the ISLE host server disk(s) is unresponsive or offline.

---

#### Alerts Resources

* Official [Documentation](https://docs.influxdata.com/chronograf/v1.7/guides/create-alert-rules/) for setting up alerts in Chronograf

---

## Release Notes

Additional changes were made to the ISLE base images to allow for:

* Log levels to be set by users in the project in the respective environment's `.env` file (_staging.env or production.env_)

* Logging in ISLE is now set to `stdout` and `stderr` by default instead of log files.

    * Choice in Docker log drivers e.g. `syslog` vs `json`, etc.

    * TICK uses the Docker `syslog` log driver by default.

    * Flexibility for users using monitoring tools other than TICK

---

## Additional Resources

* [Influxdata](https://www.influxdata.com/) - Company Website
* Telegraf [Documentation](https://docs.influxdata.com/telegraf/v1.10/)
* Influxdb [Documentation](https://docs.influxdata.com/influxdb/v1.7/)
* Chronograf [Documentation](https://docs.influxdata.com/chronograf/v1.7/)
* Kapacitor [Documentation](https://docs.influxdata.com/kapacitor/v1.5/)

---

## Need help?

* Please use the following as resources for institutions or endusers needing support


    * [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody!
        * The [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT

    * [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support

    * [ISLE Github Issues queue](https://github.com/Islandora-Collaboration-Group/ISLE/issues) - Post your issues, bugs and requests for technical documentation here.

 --- 

