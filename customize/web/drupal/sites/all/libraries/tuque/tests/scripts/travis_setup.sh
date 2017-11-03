#!/bin/bash

# Java 8, if needed.
if [ $FEDORA_VERSION = "3.8.1" ]; then
  sudo add-apt-repository -y ppa:webupd8team/java
  sudo apt-get update
  sudo apt-get install -y oracle-java8-installer oracle-java8-set-default
  sudo update-java-alternatives -s java-8-oracle
  export JAVA_HOME=/usr/lib/jvm/java-8-oracle
fi

mysql -u root -e "create database fedora;"
mysql -u root -e "GRANT ALL PRIVILEGES ON fedora.* To 'fedora'@'localhost' IDENTIFIED BY 'fedora';"
wget http://alpha.library.yorku.ca/islandora_tomcat.$FEDORA_VERSION.tar.gz
tar xf islandora_tomcat.$FEDORA_VERSION.tar.gz
cd islandora_tomcat
export CATALINA_HOME='.'
export FEDORA_HOME=fedora
export JAVA_OPTS="-Xms1024m -Xmx1024m -XX:MaxPermSize=512m -XX:+CMSClassUnloadingEnabled -Djavax.net.ssl.trustStore=$CATALINA_HOME/fedora/server/truststore -Djavax.net.ssl.trustStorePassword=tomcat"

# Needed for Fedora 3.8.1
if [ $FEDORA_VERSION = "3.8.1" ]; then
  ./fedora/server/bin/fedora-rebuild.sh -r org.fcrepo.server.utilities.rebuild.SQLRebuilder
fi

./bin/startup.sh
cd ..
sleep 10
