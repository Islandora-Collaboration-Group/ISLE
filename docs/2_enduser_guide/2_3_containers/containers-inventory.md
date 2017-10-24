## [STUB] Container Inventory

- include environmental variables specific to containers
- include accounts and passwords specific to containers.

1. [MySQL](containers-mysql.md)  
    a. has the Drupal site (tbd name) database  
    b. has the fedora3 database  
2. [Apache](containers-apache.md)  
    a. capable of running multi-sites  
3. [Fedora](containers-fedora.md)  
    a. single repository  
    b. has Djatoka dependency  
    c. Runs Tomcat service  
4. [Gsearch](containers-gsearch.md)  
    a. Discovery Garden "patched version"  
    b. Runs Tomcat service  
    c. Uses Discovery Garden Islandora Transforms (XSLTs)  
    d. Uses Discovery Garden Search Extensions  
5. [Solr](containers-solr.md)  
    a. Uses Discovery Garden basic-solr-config  
    b. Runs Tomcat service  
6. [Certbot](containers-certbot.md)  
    a. Runs Let's Encrypt  
