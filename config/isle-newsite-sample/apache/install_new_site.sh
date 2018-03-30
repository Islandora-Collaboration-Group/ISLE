#!/bin/bash

echo "Changing directory to tmp/isle_drupal_build_tools"
cd /tmp/isle_drupal_build_tools || exit

echo "Using Drush makefile to create sample Drupal site within /tmp/drupal_install"
/usr/local/bin/drush make --prepare-install /tmp/isle_drupal_build_tools/drupal.drush.make /tmp/drupal_install

echo "Using Islandora makefile for Islandora Modules for sample Drupal site within /tmp/drupal_install"
/usr/local/bin/drush make --no-core /tmp/isle_drupal_build_tools/islandora.drush.make /tmp/drupal_install

echo "Remove blank settings.php from /tmp/drupal_install"
rm -f /tmp/drupal_install/sites/default/settings.php

echo "Remove basic ubuntu index.html page"
rm -f /var/www/html/index.html

echo "Copy /tmp/drupal_install contents to /var/www/html"
cp -rv /tmp/drupal_install/. /var/www/html/

echo "Copy /tmp/settings.php to /var/www/html/sites/default/settings.php"
cp -v /tmp/settings.php /var/www/html/sites/default/

echo "Installing all Islandora modules"
cd /var/www/html/sites/all/modules || exit

## Site install
echo "Installing Drupal Site"
/usr/local/bin/drush site-install -y --account-name=newsite_admin --account-pass=newsite_adminpw --account-mail=admin@newsite.com --site-name="ISLE New Site Sample"

## Drush vset of all settings
echo "Drush vset of Drupal Site configurations"
/usr/local/bin/drush -u 1 -y vset islandora_paged_content_djatoka_url "/adore-djatoka"
/usr/local/bin/drush -u 1 -y vset islandora_base_url "http://fedora:8080/fedora"
/usr/local/bin/drush -u 1 -y vset islandora_solr_url "solr:8080/solr"
/usr/local/bin/drush -u 1 -y vset imagemagick_convert "/usr/bin/convert"
/usr/local/bin/drush -u 1 -y vset image_toolkit "imagemagick"
/usr/local/bin/drush -u 1 -y vset islandora_ocr_tesseract "/usr/bin/tesseract"
/usr/local/bin/drush -u 1 -y vset islandora_checksum_checksum_type "SHA-1"
/usr/local/bin/drush -u 1 -y vset islandora_checksum_enable_checksum "TRUE"
/usr/local/bin/drush -u 1 -y vset islandora_pdf_create_fulltext "1"
/usr/local/bin/drush -u 1 -y vset islandora_batch_java "/usr/bin/java"
/usr/local/bin/drush -u 1 -y vset islandora_lame_url "/usr/bin/lame"
/usr/local/bin/drush -u 1 -y vset islandora_paged_content_gs "/usr/local/bin/gs"
/usr/local/bin/drush -u 1 -y vset islandora_video_ffmpeg_path "/usr/local/bin/ffmpeg"
/usr/local/bin/drush -u 1 -y vset islandora_video_ffmpeg2theora_path "/usr/bin/ffmpeg2theora"
/usr/local/bin/drush -u 1 -y vset islandora_use_kakadu "TRUE"
/usr/local/bin/drush -u 1 -y vset islandora_kakadu_url "/usr/local/bin/kdu_compress"
/usr/local/bin/drush -u 1 -y vset islandora_pdf_path_to_pdftotext "/usr/bin/pdftotext"
/usr/local/bin/drush -u 1 -y vset islandora_fits_executable_path "/usr/local/fits/fits.sh"
/usr/local/bin/drush -u 1 -y vset --format=json islandora_openseadragon_settings '{"debugMode":0,"djatokaServerBaseURL":"\/adore-djatoka\/resolver","animationTime":"1.5","blendTime":"0.1","alwaysBlend":0,"autoHideControls":1,"immediateRender":0,"wrapHorizontal":0,"wrapVertical":0,"wrapOverlays":0,"panHorizontal":1,"panVertical":1,"showNavigator":1,"minZoomImageRatio":"0.8","maxZoomPixelRatio":"2","visibilityRatio":"0.5","springStiffness":"5.0","imageLoaderLimit":"5","clickTimeThreshold":"300","clickDistThreshold":"5","zoomPerClick":"2.0","zoomPerScroll":"1.2","zoomPerSecond":"2.0"}'
/usr/local/bin/drush -u 1 -y vset --format=json islandora_audio_viewers '{"name":{"none":"none","islandora_videojs":"islandora_videojs"},"default":"islandora_videojs"}'
/usr/local/bin/drush -u 1 -y vset --format=json islandora_video_viewers '{"name":{"none":"none","islandora_videojs":"islandora_videojs"},"default":"islandora_videojs"}'
/usr/local/bin/drush -u 1 -y vset --format=json islandora_book_viewers '{"name":{"none":"none","islandora_internet_archive_bookreader":"islandora_internet_archive_bookreader"},"default":"islandora_internet_archive_bookreader"}'
/usr/local/bin/drush -u 1 -y vset --format=json islandora_book_page_viewers '{"name":{"none":"none","islandora_openseadragon":"islandora_openseadragon"},"default":"islandora_openseadragon"}'
/usr/local/bin/drush -u 1 -y vset --format=json islandora_large_image_viewers '{"name":{"none":"none","islandora_openseadragon":"islandora_openseadragon"},"default":"islandora_openseadragon"}'
/usr/local/bin/drush -u 1 -y vset --format=json islandora_newspaper_issue_viewers '{"name":{"none":"none","islandora_internet_archive_bookreader":"islandora_internet_archive_bookreader"},"default":"islandora_internet_archive_bookreader"}'
/usr/local/bin/drush -u 1 -y vset --format=json islandora_newspaper_page_viewers '{"name":{"none":"none","islandora_openseadragon":"islandora_openseadragon"},"default":"islandora_openseadragon"}'

## Enable all modules
echo "Enabling all Drupal modules, Islandora modules first"

drush -y -u 1 en php_lib
drush -y -u 1 en islandora
sleep 10s #wait 10 seconds
drush -y -u 1 en islandora_basic_collection
sleep 10s #wait 10 seconds
drush -y -u 1 en objective_forms
drush -y -u 1 en islandora_solr
drush -y -u 1 en islandora_solr_metadata
drush -y -u 1 en islandora_solr_facet_pages
drush -y -u 1 en islandora_solr_views
drush -y -u 1 en islandora_pdf
drush -y -u 1 en islandora_audio
drush -y -u 1 en islandora_book
drush -y -u 1 en islandora_compound_object
drush -y -u 1 en islandora_disk_image
drush -y -u 1 en islandora_entities
drush -y -u 1 en islandora_entities_csv_import
drush -y -u 1 en islandora_basic_image
drush -y -u 1 en islandora_large_image
drush -y -u 1 en islandora_newspaper
drush -y -u 1 en islandora_video
drush -y -u 1 en islandora_web_archive
drush -y -u 1 en islandora_premis
drush -y -u 1 en islandora_checksum
drush -y -u 1 en islandora_checksum_checker
drush -y -u 1 en islandora_book_batch
drush -y -u 1 en islandora_pathauto
drush -y -u 1 en islandora_pdfjs
drush -y -u 1 en islandora_videojs
drush -y -u 1 en islandora_jwplayer
drush -y -u 1 en xml_forms
drush -y -u 1 en xml_form_builder
drush -y -u 1 en xml_schema_api
drush -y -u 1 en xml_form_elements
drush -y -u 1 en xml_form_api
drush -y -u 1 en jquery_update
drush -y -u 1 en zip_importer
drush -y -u 1 en islandora_basic_image
drush -y -u 1 en islandora_bibliography
drush -y -u 1 en islandora_compound_object
drush -y -u 1 en islandora_google_scholar
drush -y -u 1 en islandora_scholar_embargo
drush -y -u 1 en islandora_solr_config
drush -y -u 1 en citation_exporter
drush -y -u 1 en doi_importer
drush -y -u 1 en endnotexml_importer
drush -y -u 1 en pmid_importer
drush -y -u 1 en ris_importer
drush -y -u 1 en islandora_fits
drush -y -u 1 en islandora_ocr
drush -y -u 1 en islandora_oai
drush -y -u 1 en islandora_marcxml
drush -y -u 1 en islandora_simple_workflow
drush -y -u 1 en islandora_xacml_api
drush -y -u 1 en islandora_xacml_editor
drush -y -u 1 en xmlsitemap xmlsitemap_custom
drush -y -u 1 en islandora_xmlsitemap
drush -y -u 1 en colorbox
drush -y -u 1 en islandora_internet_archive_bookreader
drush -y -u 1 en islandora_bagit
drush -y -u 1 en islandora_batch_report
drush -y -u 1 en islandora_usage_stats
drush -y -u 1 en islandora_form_fieldpanel
drush -y -u 1 en islandora_altmetrics
drush -y -u 1 en islandora_populator
drush -y -u 1 en islandora_newspaper_batch
drush -y -u 1 en islandora_openseadragon
drush -y -u 1 en views_ui
drush -y -u 1 en webform
drush -y -u 1 en webform_ajax
drush -y -u 1 en webform_bonus
drush -y -u 1 en islandora_webform
drush -y -u 1 en islandora_webform_ingest
drush openseadragon-plugin
drush videojs-plugin
drush pdfjs-plugin
drush iabookreader-plugin
# Due to Islandora Paged Content Module install hook, the islandora_paged_content_gs variable is overwritten by the install / enabling of the module back to /usr/bin/gs
echo "Rerunning drush vset to ensure that Ghostscript works for the PDF DERIVATIVE SETTINGS"
/usr/local/bin/drush -u 1 -y vset islandora_paged_content_gs "/usr/local/bin/gs"

echo "Re-running the islandora_video_mp4_audio_codec vset!"
/usr/local/bin/drush @sites -u 1 -y vset islandora_video_mp4_audio_codec "aac"

echo "Enable module script finished!"

## Cron job setup
#echo "Cron job setup every 3 hours"
#crontab -l > crondrupal
#sudo crontab -u islandora -e
#echo "0 0,3,6,9,12,15,18,21 * * * /usr/local/bin/drush cron -u 1 --root=/var/www/html —uri=http:/isle.localdomain" >> crondrupal
#crontab crondrupal
#rm crondrupal

# Fix site directory permissions
echo "Running fix-permissions script"
/bin/bash /tmp/isle_drupal_build_tools/fix-permissions.sh --drupal_path=/var/www/html --drupal_user=islandora --httpd_group=www-data

echo "Drush script finished! ...exiting"
exit
