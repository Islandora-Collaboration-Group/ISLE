# Migrate your MySQL databases to UTF8 format

* Within the `ISLE/scripts/apache/` directory there is now a `db_ut8f_migration.sh` script that can be run to migrate your site's MySQL databases to UTF8 format.

* More information can be found [here]( https://www.drupal.org/project/utf8mb4_convert)

* To use this script:
  * `docker cp scripts/apache/db_ut8f_migration.sh isle-apache-ld:/var/www/html`
  * `docker exec -it isle-apache-ld bash chmod 755 /var/www/html/db_utf8_migration.sh`
  * `docker exec -it isle-apache-ld bash /var/www/html/db_ut8f_migration.sh`
  * Please note this process can take a minimum of 10-15 mins or more depending on your site size etc.
