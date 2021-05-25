# utf 8 database conversion
# https://www.drupal.org/project/utf8mb4_convert
#! /bin/bash

echo "Moving to /var/www/html"
cd /var/www/html/ || exit;

echo "Download the Drupal module"
drush @none dl utf8mb4_convert-7.x

echo "Clear drush cache"
drush cc drush

echo "Put Drupal site into Maintenance Mode"
drush vset maintenance_mode 1

echo "Convert databases to utf8. Please note this process can take 15-30 mins depending."
drush utf8mb4-convert-databases

echo "Clear Drupal site caches"
drush cc all

echo "Take Drupal site out of Maintenance Mode"
drush vset maintenance_mode 0

echo "Script complete"
exit