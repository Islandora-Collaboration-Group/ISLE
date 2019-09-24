#!/bin/bash
## ISLE - Varnish Drupal Module Installer Script (optional)
## Installs and configures the Varnish Drupal module https://www.drupal.org/project/varnish
##
## Example steps for running this script
## On the host server, copy this script to the apache container
## docker cp isle_varnish_drupal_installer.sh isle-apache-{{ container_short_id }}:/var/www/html/isle_varnish_drupal_installer.sh
## Change permissions on the script
## docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html/ && chmod +x isle_varnish_drupal_installer.sh"
## Run the script
## docker exec isle-apache-{{ container_short_id }} bash -c "cd /var/www/html && ./isle_varnish_drupal_installer.sh"

## Install Varnish related Drupal Modules
echo "Download, install and enable the Drupal varnish module"
drush -u 1 -y en varnish

echo "Download, install and enable the Drupal purge module"
drush -u 1 -y en purge

echo "Download, install and enable the Drupal expire module"
drush -u 1 -y en expire

## Drush vset of all settings
echo "Drush vset of Drupal varnish module configurations"
drush -u 1 -y vset varnish_bantype '0'
drush -u 1 -y vset varnish_cache_clear '1'
drush -u 1 -y vset varnish_control_key "$VARNISH_SECRET"
drush -u 1 -y vset varnish_control_key_appendnewline '1'
drush -u 1 -y vset varnish_control_terminal 'varnish:6082'
drush -u 1 -y vset varnish_flush_cron '0'
drush -u 1 -y vset varnish_front_domains ''
drush -u 1 -y vset varnish_socket_timeout '100'
drush -u 1 -y vset varnish_version '4'

echo "Drush vset of Drupal purge module configurations"
drush -u 1 -y vset purge_proxy_urls 'http://varnish:6081'

echo "Drush vset of Drupal expire module configurations"
drush -u 1 -y vset --format=json expire_comment_actions '{"1":0,"2":0,"3":0,"4":0,"5":0}'
drush -u 1 -y vset expire_comment_comment_page '1'
drush -u 1 -y vset expire_comment_custom '0'
drush -u 1 -y vset expire_comment_custom_pages ''
drush -u 1 -y vset expire_comment_front_page '0'
drush -u 1 -y vset expire_comment_node_page '1'
drush -u 1 -y vset expire_comment_node_term_pages '0'
drush -u 1 -y vset expire_debug '0'
drush -u 1 -y vset --format=json expire_file_actions '{"1":1,"2":2}'
drush -u 1 -y vset expire_file_custom '0'
drush -u 1 -y vset expire_file_custom_pages ''
drush -u 1 -y vset expire_file_file '1'
drush -u 1 -y vset expire_file_front_page '1'
drush -u 1 -y vset expire_include_base_url '1'
drush -u 1 -y vset --format=json expire_menu_link_actions '{"1":1,"2":2,"3":3}'
drush -u 1 -y vset --format=json expire_menu_link_override_menus '{"main-menu":2,"management":2,"navigation":2,"user-menu":2}'
drush -u 1 -y vset --format=json expire_node_actions '{"1":1,"2":2,"3":3}'
drush -u 1 -y vset expire_node_custom '0'
drush -u 1 -y vset expire_node_custom_pages ''
drush -u 1 -y vset expire_node_front_page '0'
drush -u 1 -y vset expire_node_node_page '1'
drush -u 1 -y vset expire_node_term_pages '1'
drush -u 1 -y vset expire_status '2'
drush -u 1 -y vset --format=json expire_user_actions '{"1":0,"2":0,"3":0,"4":0}'
drush -u 1 -y vset expire_user_custom '0'
drush -u 1 -y vset expire_user_custom_pages ''
drush -u 1 -y vset expire_user_front_page '0'
drush -u 1 -y vset expire_user_term_pages '0'
drush -u 1 -y vset expire_user_user_page '1'

## Exit script
echo "This script will now exit"

exit