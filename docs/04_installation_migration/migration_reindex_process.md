When migrating any non-ISLE Islandora site, it is crucial to rebuild (reindex) the following three indices from the FOXML and datastream files on disk.

**Fedora's indices:**

* Resource Index - The Resource Index is the Fedora module that provides the infrastructure for indexing relationships among objects and their components.

* SQL database - `fedora3` contains information vital for the Drupal site to connect to Fedora correctly.

**Solr index**

* Solr Index - Solr an open source enterprise search platform works in conjunction with the Islandora Solr module to provide a way to configure the Islandora search functions, the search results display, and the display of metadata on object pages. The index serves as a list of those objects for fast searching across large collections.

You can use the command-line interactive utility `fedora-rebuild.sh` on the `fedora` container to rebuild all indices when the Fedora (not Tomcat) server is offline.

Depending on the size of your repository, this entire process may take minutes (thousands of objects) or hours (millions of objects) to complete.

### Assumptions / Pre-Requisites

* All containers are currently running

* `tomcat_admin_password` = The tomcat admin password from the Fedora container.

* `ISLE_HOST_IP` = The IP address of your ISLE Host server.

##### Shutdown Fedora Method 1:

* Open a browser and navigate to `http://ISLE_HOST_IP:8080/manager/html`
    * User:     `admin`
    * Password: `tomcat_admin_password`

* Scroll down to the `fedora` row and on the right in the `Commands` column, click the `stop` button.

* Wait 20 seconds.        

* Move onto the Reindex Fedora RI (1 of 3) section.

##### Shutdown Fedora Method 2:

`tomcat_admin_password` = Change this to your tomcat admin password from the Fedora container.

Shell into the running `fedora` container
* `docker exec -it compass-fedora-dev bash`

* `wget "http://admin:tomcat_admin_password:8080/manager/text/stop?path=/fedora" -O - -q`

* Type: `exit` to exit the `fedora` container.

* Move onto the Reindex Fedora RI (1 of 3) section.

#### Reindex Fedora RI (1 of 3)

* Shell into the `fedora` container: `docker exec -it compass-fedora-dev bash`

* `cd /usr/local/fedora/server/bin`

* * This command will take from 10 - 30+ mins to complete depending on the size of your Fedora collection.

    * If it fails immediately, check the log it creates for details: `tail -f /usr/local/tomcat/logs/fedora_ri.log`

    * `/bin/sh fedora-rebuild.sh -r org.fcrepo.server.resourceIndex.ResourceIndexRebuilder > /usr/local/tomcat/logs/fedora_ri.log 2>&1`

* Stay within the `fedora` container and continue to the next section Reindex SQL database (2 of 3).

---

#### Reindex SQL database (2 of 3)

`mysqlrootpassword` = Change this to your MySQL root password from the MySQL container.

Connect to the mysql container from the fedora container to truncate `fedora3` tables.

* `mysql -h mysql -u root -pmysqlrootpassword`

* **Please note:** ^ there should be no space between the last command `-p` and your mysql root password. (mysqlrootpassword)

The `mysql` prompt should now appear. You are now connected to the `mysql` container via the mysql client on the `fedora` container.

Select the `fedora3` database.

* `use fedora3;`

The mysql prompt should now display something similar to this: `MySQL [fedora3]>`

Display all of the tables of the `fedora3` database.

* `show tables;`

Example:

```
+---------------------+
| Tables_in_fedora3   |
+---------------------+
| dcDates             |
| doFields            |
| doRegistry          |
| fcrepoRebuildStatus |
| modelDeploymentMap  |
| pidGen              |
+---------------------+
6 rows in set (0.00 sec)
```

* Enter the following commands:
    * `truncate table dcDates;`
    * `truncate table doFields;`
    * `truncate table doRegistry;`
    * `truncate table fcrepoRebuildStatus;`
    * `truncate table modelDeploymentMap;`
    * `truncate table pidGen;`

Return to the terminal with `fedora` container shelled in.

* `exit`

The mysql prompt should disappear and the prompt for `fedora` should reappear. You've been returned to the `fedora` container.

* `cd /usr/local/fedora/server/bin`

* This command will take from 10 - 30+ mins to complete depending on the size of your Fedora collection.

    * If it fails immediately, check the log it creates for details:  `tail -f /usr/local/tomcat/logs/sql_ri.log`

    * `/bin/sh fedora-rebuild.sh -r org.fcrepo.server.utilities.rebuild.SQLRebuilder > /usr/local/tomcat/logs/sql_ri.log 2>&1`

* Stay within the `fedora` container and continue to the next section Reindex Solr (3 of 3).

---

#### Reindex Solr (3 of 3)

`fgsAdmin_password`  = Change this to your Fedora Gsearch (fgsAdmin) password from the Fedora container.

**WARNING**
This reindex process takes the longest of all three, with up to **1 - 30 or more hours** to complete depending on the size of your Fedora collection.

As such, it is recommended starting a screen session prior to running the following command. Learn more about [screen here](https://www.tecmint.com/screen-command-examples-to-manage-linux-terminals/)

* Whether you use `screen` prior or not, shell into the `fedora` container: `docker exec -it compass-fedora-dev bash`

* `cd /usr/local/tomcat/webapps/fedoragsearch/client`

* `/bin/sh runRESTClient.sh localhost:8080 updateIndex fromFoxmlFiles`
    * User: `fgsAdmin`
    * Password: `fgsAdmin_password`

* Again this process will take up to **1 - 30 or more hours** to complete depending on the size of your Fedora collection.

    * You can tail either Fedora Gsearch or Solr logs, ([locations here](../../06_specifications/specs_docker_containers)) to watch the indexing process (_if necessary_).

* Type `exit` when finished to exit the container.
