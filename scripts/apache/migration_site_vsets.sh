#!/bin/bash

## Drush vset of all settings - modified not overwrite pre-existing settings and user preferences.
echo "Drush vset of ISLE specific configurations"
drush -u 1 -y vset islandora_base_url "fedora:8080/fedora"
drush -u 1 -y vset islandora_solr_url "solr:8080/solr"
drush -u 1 -y vset imagemagick_convert "/usr/local/bin/convert"
drush -u 1 -y vset image_toolkit "imagemagick"
drush -u 1 -y vset islandora_ocr_tesseract "/usr/bin/tesseract"
drush -u 1 -y vset islandora_batch_java "/usr/bin/java"
drush -u 1 -y vset islandora_lame_url "/usr/bin/lame"
drush -u 1 -y vset islandora_paged_content_gs "/usr/bin/gs"
drush -u 1 -y vset islandora_video_ffmpeg_path "/usr/bin/ffmpeg"
drush -u 1 -y vset islandora_video_ffmpeg2theora_path "/usr/bin/ffmpeg2theora"
drush -u 1 -y vset islandora_use_kakadu "FALSE"
drush -u 1 -y vset islandora_kakadu_url "/usr/local/bin/kdu_compress"
drush -u 1 -y vset islandora_pdf_path_to_pdftotext "/usr/bin/pdftotext"
drush -u 1 -y vset islandora_fits_executable_path "/usr/local/bin/fits"
drush -u 1 -y vset islandora_openseadragon_iiif_identifier '[islandora_openseadragon:pid]~[islandora_openseadragon:dsid]~[islandora_openseadragon:token]'
drush -u 1 -y vset islandora_openseadragon_iiif_token_header '0'
drush -u 1 -y vset islandora_openseadragon_iiif_url 'iiif/2'
drush -u 1 -y vset islandora_openseadragon_tilesource 'iiif'
drush -u 1 -y vset islandora_internet_archive_bookreader_iiif_identifier '[islandora_iareader:pid]~[islandora_iareader:dsid]~[islandora_iareader:token]'
drush -u 1 -y vset islandora_internet_archive_bookreader_iiif_token_header '0'
drush -u 1 -y vset islandora_internet_archive_bookreader_iiif_url 'iiif/2'
drush -u 1 -y vset islandora_internet_archive_bookreader_pagesource 'iiif'

## Enable repo access to anonymous users.
drush rap 'anonymous user' 'view fedora repository objects'

# Fix site directory permissions
echo "Running fix-permissions script"
/bin/bash /utility-scripts/isle_drupal_build_tools/drupal/fix-permissions.sh --drupal_path=/var/www/html --drupal_user=islandora --httpd_group=www-data

## Cron job setup every three hours
echo "Configuring cron job to run every 3 hours"
echo "0 */3 * * * su -s /bin/bash www-data -c 'drush cron --root=/var/www/html --uri=${BASE_DOMAIN} --quiet'" >> crondrupal
crontab crondrupal
rm crondrupal

## Run cron first time, update update-status (rf), clear caches.
echo 'Running Drupal Cron first time and clearing Drupal Caches.'
su -s /bin/bash www-data -c 'drush cron && drush rf && drush cc all'

echo "Drush script finished! ...exiting"
exit