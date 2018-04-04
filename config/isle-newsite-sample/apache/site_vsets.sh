#!/bin/bash

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

echo "Re-running the islandora_paged_content_gs vset!"
/usr/local/bin/drush -u 1 -y vset islandora_paged_content_gs "/usr/local/bin/gs"

echo "Re-running the islandora_video_mp4_audio_codec vset!"
/usr/local/bin/drush -u 1 -y vset islandora_video_mp4_audio_codec "aac"

### Cron job setup
#echo "Cron job setup every 3 hours"
#crontab -l > crondrupal
#sudo crontab -u islandora -e

#echo "0 0,3,6,9,12,15,18,21 * * * /usr/local/bin/drush cron -u 1 --root=/var/www/html —uri=https:/isle-dev.williams.edu" >> crondrupal
#crontab crondrupal
#rm crondrupal

# Fix site directory permissions
echo "Running fix-permissions script"
/bin/bash /tmp/isle_drupal_build_tools/fix-permissions.sh --drupal_path=/var/www/html --drupal_user=islandora --httpd_group=www-data

echo "Drush script finished! ...exiting"
exit
