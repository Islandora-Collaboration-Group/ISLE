#!/bin/bash


echo "Changing directory to tmp/isle_drupal_build_tools"
cd /tmp/isle_drupal_build_tools || exit

echo "Using Drush makefile to create sample Drupal site within /tmp/drupal_install"
/usr/local/bin/drush make --prepare-install /tmp/isle_drupal_build_tools/drupal.drush.make /tmp/drupal_install

echo "Using Islandora makefile for Islandora Modules for sample Drupal site within /tmp/drupal_install"
/usr/local/bin/drush make --no-core /tmp/isle_drupal_build_tools/islandora.drush.make /tmp/drupal_install

echo "Remove blank settings.php from /tmp/drupal_install"
rm -f /tmp/drupal_install/sites/default/settings.php

echo "Copy /tmp/drupal_install contents to /var/www/html"
cp -rv /tmp/drupal_install/. /var/www/html/

echo "Fix Openseadragon & Change directory to /var/www/html/sites/all/libraries"
cd /var/www/html/sites/all/libraries || exit

echo "Delete current broken openseadragon library"
rm -rf /var/www/html/sites/all/libraries/openseadragon

echo "Download Openseadragon Library 2.2.1.zip"
wget https://github.com/openseadragon/openseadragon/releases/download/v2.2.1/openseadragon-bin-2.2.1.zip

echo "Unzip Openseadragon Library"
unzip /var/www/html/sites/all/libraries/openseadragon-bin-2.2.1.zip

echo "mv openseadragon-bin-2.2.1 openseadragon"
mv /var/www/html/sites/all/libraries/openseadragon-bin-2.2.1 /var/www/html/sites/all/libraries/openseadragon

echo "Delete Openseadragon zipfile"
rm /var/www/html/sites/all/libraries/openseadragon-bin-2.2.1.zip
