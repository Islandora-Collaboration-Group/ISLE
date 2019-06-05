# TICK Stack

## Use Cases & User Stories

* As a repository administrator, I expect to be able to view real-time and some historic systems/performance metrics without digging around in server logs.
  * As a repository administrator, I should be able to use the dashboard to troubleshoot the repo (_like when it’s hanging or objects are slow to load_)

* As a repository owner/manager, I expect logging to be handled in a way that does not endanger my systems health and performance over time.

* As a repository owner/manager, I expect to be able to set logging levels for all relevant log streams.

* As a repository administrator, I expect to be able to view in a web dashboard:
  * Real-time and recent historic systems/performance metrics
  * Real-time and recent historic system and application logs

* As a repository administrator, I expect to be able to use monitoring tools that do not burden the target server thus I can setup monitoring tools remotely and pull data from the ISLE server for review.

---

## What is this component?

The "TICK" stack is a suite of open-source applications that are used together for the storing, visualization and monitoring of data (time series metrics) gathered from running systems.

**TICK** is:

* [**T**elegraf](https://www.influxdata.com/time-series-platform/telegraf/) - a plugin-driven server agent for collecting and sending metrics and events from databases, systems, and IoT sensors.

* [**I**nfluxdb](https://www.influxdata.com/time-series-platform/) - the open source time series database is designed to handle high write and query loads and provides a SQL-like query language called InfluxQL for interacting with data.

* [**C**hronograf](https://www.influxdata.com/time-series-platform/chronograf/) - the user interface and administrative component for using templates and libraries to rapidly build dashboards with real-time visualizations.

* [**K**apacitor](https://www.influxdata.com/time-series-platform/kapacitor/) - the native data processing engine which can process both stream and batch data from InfluxDB, acting on this data in real-time via its programming language TICKscript.

TICK was created and is developed by [Influxdata](https://www.influxdata.com) which is _"...dedicated to open source and truly cares about helping developers get to results faster with less complexity and less code."_

---

## Why use this component with ISLE?

* To monitor running systems in Development, Staging or Production
  * Do note that users can monitor **ANY** systems with these tools, not just ISLE-related platforms.

* Create alerts that can be sent to email, text or Slack™ to warn users, system-administrators of potential issues, storage or memory challenges and much more.

* As a one-stop "search-engine" of log and metric data for all ISLE containers, services and applications, host servers etc.

* To help ISLE users to better understand how their systems work, how Islandora works and to greatly improve visibility on applications, services and processes within ISLE

The use and setup of TICK within the ISLE platform is as an optional [sidecar](https://docs.microsoft.com/en-us/azure/architecture/patterns/sidecar) to ISLE. There are various ways one can setup TICK in general (software, services or Docker containers)

The recommended setup is to:

* Run only a Telegraf container (agent)  on the `Production` system.

* Run the full TICK stack on the `Staging` host system.
  * Data from both systems will be collected, analyzed and accessed on / from the the `Staging` host system.

Other TICK / ISLE configurations are possible but **are not supported or detailed** as of yet in this documentation. Endusers seeking alternative setup possibilities are encouraged to further review and mine the official Influxdata [documentation](https://docs.influxdata.com/platform/introduction/).

---

## TICK Requirements

* For any system that is running the full TICK stack as a side car
  * Recommend an additional 2 - 3 GB of RAM
  * Recommend an additional 5 - 10 GB of storage disk space

* Use the `docker-compose-tick.yml` file for setup and/or modification. This file will have not only the correct setup with the TICK stack but the necessary `syslog` driver code and tagging.

* Use the `latest` tags for the TICK base images
  * `image: telegraf:latest`
  * `image: influxdb:latest`
  * `image: chronograf:latest`
  * `image: kapacitor:latest`

* [Rsyslog](https://www.rsyslog.com/) has to be installed on any server that is monitored whether that system is running fullstack TICK or simply the Telegraf agent.
  * Rsyslog is used to help the Telegraf agent forward log messages from Docker, the running ISLE containers and the host system to Influxdb. 
  * Use the default Ansible script in the ISLE repository that can install `rsyslog` on any host server.
  * Manual options
    * [Red Hat / CentOS](https://www.rsyslog.com/rhelcentos-rpms/)
    * [Ubuntu / Debian](https://www.rsyslog.com/ubuntu-repository/)

* (_Optional_) Use some of the "canned" `.tick` (_tickscript_) templates to create dashboards and alerts provided in the `config/tick/chronograf/tickscripts` directory. See the section Templates for more detail. (TO DO)

---

## Telegraf Agent Reporting Plugins

The Telegraf agent used for ISLE has a default configuration which runs the following plugins to monitor various services.

To review or configure plugins, edit the `config/tick/telegraf/telegraf.conf` file and navigate to the `INPUT PLUGINS` section starting Line 154.

If an ISLE User would like to add a plugin to monitor additional services, please review the additional information below on Telegraf plugins which can be found in two places:

* Telegraf Plugins Documentation https://docs.influxdata.com/telegraf/v1.10/plugins/inputs/
* Individual plugin's README.md files on Github.com https://github.com/influxdata/telegraf

### List of Telegraf Agent Plugins used for ISLE:

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

* `inputs.x509_cert` - [Reads metrics from a SSL certificate to check validity, expiration etc](https://github.com/influxdata/telegraf/blob/release-1.10/plugins/inputs/x509_cert/README.md)

---

## Installation 

The instructions below are for the full TICK stack as a side car on a `Staging` system.

### Assumptions

* The instructions below **ASSUME**:

  * A previously installed and running ISLE system is in place already.

  * You'll need to use ISLE v.1.1.x or the `dashboards-dev` branch and higher for the syslog driver changes to be in place.

  * That the "sidecar" method will be the installation type.

  * A firewall configuration that allows incoming public traffic to port `8086` traffic

  * A firewall configuration that allows incoming private traffic access to port `8888`. It is recommended that this not be general public access but whitelisted to a select range of IP addresses given this will be the `Chronograf` dashboard port. Trusted users only please.

  * Only installation on Linux based host systems e.g. Debian / Ubuntu 18.04+ and Red Hat / CentOS 7.x+
    * Currently attempting to run TICK locally on a MacOS system fails https://github.com/docker/for-mac/issues/3303

---

## TICK "Sidecar" Installation

* Shut down all running ISLE containers.

#### Docker Syslog driver setup

* You will need to use the `sudo` command or become the `root` user for the steps below. Copying this file allows the Docker Daemon to use the `syslog` driver for logfiles and reporting to the `telegraf` agent.

* Check if you already have an existing `/etc/docker/daemon.json` file. 
  * If **YES**, add / merge  the contents of `/ISLE/config/proxy/tick/docker/daemon.json` with your current `/etc/docker/daemon.json` file.
  * If **NO**, copy `/ISLE/config/proxy/tick/docker/daemon.json` to `/etc/docker/`
    * Example: `sudo cp /ISLE/config/proxy/tick/docker/daemon.json /etc/docker/`

* Restart the Docker service
  * `sudo service docker restart` or `sudo systemctl restart docker`
  * You can check if the service is running with `sudo service docker status`

#### Rsyslog setup

* You will need to use the `sudo` command or become the `root` user for the steps below. Copying this file allows the information from the host `syslog` be forwarded to the `telegraf` agent.

* Copy the Telegraf configuration file to the Rsyslog.d directory so Rsyslog will forward logs to the Telegraf agent with correct formatting. Your ISLE project location may be different from the example below.
  * Example: `sudo cp /ISLE/config/proxy/tick/rsyslog/50-telegraf.conf /etc/rsyslog.d/`

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

#### Docker-compose.yml edits

* Copy and paste the contents of the `config/tick/tick-config.md` file and insert them into your `docker-compose.yml` in between the `apache` and `traefik` services.

* Copy and paste the contents of the `config/tick/dc-syslog-config.md` file and insert repeatedly into your `docker-compose.yml` at the end of every service definition.
  * mysql
  * fedora
  * solr
  * image-services
  * apache
  * traefik

Text to copy:
```bash
     logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

Text insertion example:

```bash
  mysql:
    image: islandoracollabgroup/isle-mysql:1.1.x
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

* Copy and paste the following values for the TICK stack data volumes to the end of the `volumes` section of the `docker-compose.yml` file. Most likely **Lines 287 -289**.

TICK volumes:

```bash

  isle-influxdb-data:
  isle-kapacitor-data:
  isle-chronograf-data:

```

The volumes section should now look like this:

```bash

volumes:
  isle-db-data:
  isle-mysql-log:
  isle-solr-data:
  isle-apache-data:
  isle-portainer-data:
  isle-fedora-datastreamStore:
  isle-fedora-objectStore:
  isle-fedora-resourceIndex:
  isle-fedora-activemq:
  isle-fedora-XACML:
  isle-influxdb-data:
  isle-kapacitor-data:
  isle-chronograf-data:

```

#### Telegraf Agent configuration edits

* You'll need to edit the `config/tick/telegraf/telegraf.conf` to add the following:
  * **Line 76**: `hostname = ""`
    * Enter the name of the server you will be monitoring e.g. `isle-server-staging` or `icg-staging` etc. This value can be a [FQDN](https://kb.iu.edu/d/aiuv), an IP or any name really.
  * **Line 97**: `database = "telegraf"`
    * By default, the ISLE TICK setup assumes this database as it is the easiest but pools all data received by individual monitored hosts into one database. For first time users, recommend leaving this value in place.
    * Users are free to change this to any value to segregate data by systems, group etc.
  * **Line 290**: Change `servers = ["root:ild_mysqlrt_2018@tcp(mysql:3306)/?tls=false"]` to use your `Staging` MySQL Root password. Typically this value is in your `.env` file. Swap out the `ild_mysqlrt_2018` with your `Staging` MySQL Root password.

* Start up the ISLE Docker containers again. `docker-compose up -d`

* Depending on your internet connection, this startup process may take a few minutes, as the new TICK images are being downloaded and started.

---

### Chronograf Dashboard setup

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

### Chronograf - Viewing Host Systems

To view hosts, click the `Host List` button to the left. It should appear as an eye symbol in the left navigational panel. Your host(s) should appear in the `Host List`. You can click on the name to invetigate the system further. 

By default in the `Apps` section, the three dashboards that you just setup i.e. `system`, `docker` and `mysql` should also appear. Click on any one of them to view stats specific to that reporting plugin.

### Chronograf - Viewing Logs

To view logs ordered by hosts, click the `Log Viewer` button to the left. It should appear as cut log symbol in the left navigational panel. Your host(s) should appear in the `Log Viewer`. 

Please note: This will be a lot of information at any given time especially if you connect multiple hosts. 

### Chronograf - View logs by specific host

Within the `Log Viewer` page:

* Scroll at the bottom of the page over to the `Host` column in the viewer and click on the name e.g. `bd-demo`

  * This should now filter out all other host information

  * At the top left hand corner below the search bar, the following appears `host == bd-demo`. This is the syntax used for searching by host e.g. `host == your-hostname-here` this is the same shorthand name used to configure the Telegraf agent.

  * Alternatively, you can now use this value `host == your-hostname-here` within the _Search logs using keywords or regular expressions_ search bar.

  * You can even single click into the `host == your-hostname-here` posted query and change the name to a different host.

* You can also search using ISLE key service words to drill down on log reporting. e.g. `apache`, `fedora`, `varnish` etc

---

### Using Chronograf for Alerts in ISLE

#### Resources

* Official [Documentation](https://docs.influxdata.com/chronograf/v1.7/guides/create-alert-rules/) for setting up alerts in Chronograf

#### Assumptions

* Ability to send emails from the ISLE host server
  * You may already be using an existing off host email server or service that is integrated with the ISLE Drupal site. 
  * We recommend using [SendGrid](https://sendgrid.com/) or [MailGun](https://www.mailgun.com/) which are cloud-based email delivery platforms. Please note these services are not entirely free and may require additional Drupal modules.

* Chronograf Alert Handlers will need to be setup prior to these steps. 

The ISLE Project can provide the following alert creation processes
  * Disk Space Alert - 
  * RAM Usage Alert
  * CPU Usage Alert
  * Deadman Alert

Additional alerts will be contributed by the ISLE maintainers and/or community.

---

### Disk Space Alert

This alert is designed to warn you when one of the ISLE host server disk(s) is/are almost full. Please note this setting should not alert you when the disk is already full. We suggest you use a lower value (e.g. 80%) such that you can safely shutdown containers, backup data and then expand the affected disk's capacity as needed.

#### To create this Alert:

* Within the TICK Chronograf dashboard, click on the `Alerting` symbol on the left (_this is a triangle with a ! symbol in the middle_). 
  * By default the `Manage Tasks on ...` page should appear
  * Click the blue `+ Build Alert Rule` button on the right hand side.

* Within the displayed fields, please change the following:

* `Name this Alert Rule` - Change Untitled Rule to `Disk Space Alert` or to a name of your choice.

* `Alert Type` - Choose `Threshold` - _this should be selected by default_

* `Time Series` 
  * Within the `DB.RetentionPolicy` column, select `telegraf.autogen`
  * Within the `Measurements & Tags` column, 
    * Select `> disk`
      * Select `host` (_there may be an additional number here e.g. `host - 2`_)
      * Choose the appropriate hostname (_this was the name you used to setup reporting on the Telegraf agent prior_)
   * Within the `Fields` column, select `used_percent`

* `Conditions`
  * You'll notice that a graph now appears in this area with most likely an increasing trend of usage. This graph indicates the current usage by the host of a particular disk, in our example this is the HOST ISLE operating system disk. You can create multiple alerts for additional disks, simply repeat this process as needed.
  * Within the `Send Alert where used_percent is greater than` bar, add the number `80` or a numeric value of your choice. This establishes the percent full the disk has to reach before the alert is sent. 
    * Please note, again we stress the importance of setting this value to 80 or lower to give yourself or your IT staff time to anticipate the growth prior to system challenges.
  * You'll note that the graph may adjust visually to match this new value

* `Alert Handlers`
  * Within the `Send this Alert to:` bar, click the `Add a Handler` dropdown list to add the appropriate Alert Handler. This can be email, slack etc.
  * If the `Alert Handler` is not enabled, the blue `Save Rule and Configure this Alert Handler` button will appear. We recommend that you click it and follow the instructions therein.
  * If the `Alert Handler` is enabled, then the appropriate service 



If you would like to setup ISLE and the TICK sidecar to generate alerts, you'll need to setup the following on the ISLE host server

---

## Telegraf (Agent only) Installation

### Assumptions

* Use these instructions to add an additional Docker container on a `Production` system. Please note this is not the full side-car instructions simply the Telegraf reporting agent.

* The instructions below **ASSUME**:

  * A previously installed and running ISLE system is in place already.

  * You'll need to use ISLE v.1.1.x and higher for the syslog driver changes to be in place.

  * That the "sidecar" TICK installation is already in place on the `Staging` server prior.

  * Your firewall allows outbound port `8086` traffic

---

### Telegraf Agent Installation

In the recommended setup and usage of TICK, the `Staging` server will be running the full stack of TICK as a collection point for all `Staging` data and any `Production` system will only need the Telegraf agent installed.

To install the Telegraf agent on a `Production` system:

* Shut down all running ISLE containers.

* You'll need to edit the `config/tick/telegraf/telegraf.conf` to add the following:
  * **Line 76**: `hostname = ""`
    * Enter the name of the server you will be monitoring e.g. `isle-server-prod` or `icg-production` etc. This value can be a [FQDN](https://kb.iu.edu/d/aiuv) i.e. `production-server.domain.edu`, an IP or any name really. Recommend a short name to be easily read and understood.
  * **Line 94**: `urls = ["http://influxdb:8086"]`
    * Replace the `influxdb` value with the [FQDN](https://kb.iu.edu/d/aiuv) of the `Staging` server that is running the full TICK stack and that this Telegraf agent will be sending data to e.g.
  `urls = ["http://staging-server.domain.edu:8086"]`
  * **Line 97**: `database = "telegraf"`
    * By default, the ISLE TICK setup assumes this database as it is the easiest but pools all data received by individual monitored hosts into one database. For first time users, recommend leaving this value in place.
    * Users are free to change this to any value to segregate data by systems, group etc.
  * **Line 290**: Change `servers = ["root:ild_mysqlrt_2018@tcp(mysql:3306)/?tls=false"]` to use your `Production` MySQL Root password. Typically this value is in your `.env` file. Swap out the `ild_mysqlrt_2018` with your `Production` MySQL Root password.

* Copy and paste only the following of the `config/tick/tick-config.md` file and insert it into your `docker-compose.yml` in between the `apache` and `traefik` services.

```bash

  telegraf:
    image: telegraf:latest
    # Telegraf requires network access to InfluxDB
    container_name: isle-telegraf-${CONTAINER_SHORT_ID}
    volumes:
      # Mount for telegraf configuration
      - ./config/tick/telegraf/telegraf.conf:/etc/telegraf/telegraf.conf:ro
      # Mount for Docker API access
      - /var/run/docker.sock:/var/run/docker.sock:ro
      # To get metrics off the host
      - /:/hostfs:ro
      - /etc:/hostfs/etc:ro
      - /proc:/hostfs/proc:ro
      - /sys:/hostfs/sys:ro
      - /var/run/utmp:/var/run/utmp:ro
    depends_on:
      - influxdb
    networks:
      isle-internal:
    ports:
      # This port should be for rsyslog
      - "6514:6514"
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"

```

* Copy and paste the contents of the `config/tick/dc-syslog-config.md` file and insert repeatedly into your `Production` `docker-compose.yml` at the end of every service definition.
  * mysql
  * fedora
  * solr
  * image-services
  * apache
  * traefik

Text to copy:
```bash
     logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```

Text insertion example:

```bash
  mysql:
    image: islandoracollabgroup/isle-mysql:1.1.x
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

* Copy the Telegraf configuration file to the Rsyslog.d directory so Rsyslog will forward logs to the Telegraf agent with correct formatting. Your ISLE project location may be different from the example below.
  * Example: `sudo cp /opt/ISLE/config/tick/rsyslog/50-telegraf.conf /etc/rsyslog.d/50-telegraf.conf`

* Copy over the Docker log driver configuration and then restart the Docker service.
  * Example: `sudo cp /opt/ISLE/config/tick/docker/daemon.json /etc/docker/daemon.json`
  * `sudo service docker restart`

* Start up the ISLE Docker containers again. `docker-compose up -d`

* Depending on your internet connection, this startup process may take a few minutes, as the new TICK images are being downloaded and started.

* Once the `Production` containers are back online, the services are running e.g. you see the Drupal site etc. navigate to the `Hosts` section within the `Chronograf` dashboard. You should now see your new `Production` server name. This means the Telegraf Agent on your `Production` ISLE system is now reporting properly. 

---

## Troubleshooting

If you are not seeing your server appear in the `Hosts` section nor seeing log information flowing into the `Log Viewer` section, recommend the following:

* Check that any potential firewalls are allowing communication between the `Production` and `Staging` servers on port `8086`

* Ensure that all configuration files have been copied over:
  * The new Docker log driver configuration should be found here `/etc/docker/daemon.json`
  * The new rsyslog / Telegraf configuration should be found here: `/etc/rsyslog.d/50-telegraf.conf`

* Check that the TICK related containers are running without exit codes. 
  * `docker ps -a` 
  
* Check that the new `Telegraf` docker container is running on `Production`

* Repeat these instructions from the start if need be.

---

## Release Notes

Additional changes were made to the ISLE base images to allow for:

* Log levels to be set by users in the project `.env`

* Logging in ISLE is now set to `stdout` and `stderr` by default instead of log files.

  * Choice in Docker log drivers e.g. `syslog` vs `json` etc.

  * TICK uses the Docker `syslog` log driver by default.

  * Flexibility for users using monitoring tools other than TICK

---

## Additional Resources

*
*

---

## Need help?

* Please use the following as resources for institutions or endusers needing support

  * [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings open to everybody!   
    * The [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join) is alternating Wednesdays, 3:00pm EDT

  * [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support

  * [ISLE Github Issues queue](https://github.com/Islandora-Collaboration-Group/ISLE/issues) - Post your issues, bugs and requests for technical documentation here.

 --- 