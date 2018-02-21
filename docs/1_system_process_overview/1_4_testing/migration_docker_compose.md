#### Alpha Migration Docker Compose Edit Guidelines

**Mysql Service section**
```
services:
  mysql:
```

* In the `image:` section, change to appropriate image name if needed otherwise leave as `latest`.

* In the `environment:` section, change the MYSQL_ROOT_PASSWORD

* In the `volumes:` section, change the following:
      - /pathto/isle_production_data_storage/data/mysql:/var/lib/mysql
      - /pathto/isle_production_data_storage/data/mysql/log:/var/log/mysql

    * **Please Note:** _The docker bind-mount process will automatically create the `/data/mysql` directories_

* In the `container_name:` section, change to `isle-mysql-institution` or `isle-mysql-v1`

* In the `extra_hosts:` section, change: (**this is NOT to be literally copied, example only**)
      - "isle.institution.edu:isle_host_server_IP"  to "yourislesite.institution.edu:10.10.10.10"

**Fedora Service section**
```
fedora:
```

```
**TO DO:**  remaining edits for fedora

**TO DO:**  solr service edits

TO DO : apache service edits

**TO DO:**  git commit changes & push to repo

**TO DO:**  git clone from the institutional git repo e.g. `ISLE-config` to the /opt/ISLE/config/ (on ISLE host server using terminal)

```
