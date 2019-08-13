#!/bin/bash

echo "Installing all Islandora modules"
cd /var/www/html/sites/all/modules || exit

## Enable all modules
echo "Resinstalling the Islandora module with Solution Packs"
/usr/local/bin/drush -u 1 -y ispiro --module=islandora --force
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_audio
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_basic_collection
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_basic_image
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_book
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_compound_object
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_large_image
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_newspaper
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_pdf
sleep 10s #wait 10 seconds
/usr/local/bin/drush -u 1 -y ispicm --module=islandora_video
sleep 10s #wait 10 seconds

echo "Drush script finished! ...exiting"
exit