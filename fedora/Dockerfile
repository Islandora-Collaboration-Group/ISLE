FROM ubuntu:14.04
MAINTAINER "Gavin Morris <gavin@born-digital.com>"
### - ISLE MVP 10/2017 Fedora

###
# https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections


###
# Update apt
RUN apt-get update


###
# Add software to provide add-apt-repository
RUN apt-get install -y software-properties-common python-software-properties


###
# Install required software
RUN apt-get update \
    && apt-get install -y \
    ntp \
    openssh-client \
    unzip \
    libtool \
    dkms \
    bzip2 \
    perl \
    apt-utils \
    tmpreaper \
    mysql-client \
    python-mysqldb \
    libmysqlclient-dev \
    default-jdk \
    maven \
    git \
    apt-transport-https \
    apache2 \
    openssl \
    libapache2-mod-proxy-html \
    libxml2-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*


# Configure apache 1/2
COPY apache/site.conf /etc/apache2/sites-available/site.conf
COPY apache/servername.conf /etc/apache2/conf-available/servername.conf


###
# Add the islandora user
RUN useradd -ms /bin/bash islandora

# Configure apache 2/2
RUN a2enconf servername \
    && mkdir -p /var/www/html \
    && chown islandora:www-data /var/www/html \
    && chmod 0644 /etc/apache2/sites-available/site.conf \
    && a2ensite site.conf \
    && a2enmod rewrite deflate headers expires proxy proxy_http proxy_html proxy_connect xml2enc \
    && a2dissite 000-default \
    && a2dissite default-ssl

###
# Configure tmpreaper
COPY tmpreaper/cron /etc/cron.d/tmpreaper-cron
RUN chmod 0644 /etc/cron.d/tmpreaper-cron \
    && touch /var/log/cron.log


###
# Install Oracle Java 8
RUN echo "deb http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee -a /etc/apt/sources.list \
    && echo "deb-src http://ppa.launchpad.net/webupd8team/java/ubuntu trusty main" | tee -a /etc/apt/sources.list \
    && echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | /usr/bin/debconf-set-selections \
    && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys EEA14886 \
    && apt-get update \
    && apt-get install -y curl dnsutils oracle-java8-installer oracle-java8-set-default ca-certificates \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*


###
# Make sure ports 80, 8080 are available
EXPOSE 80 8080


###
# Set up environmental variables for tomcat7 & dependencies installation

ENV JAVA_HOME=/usr/lib/jvm/java-8-oracle \
     CATALINA_HOME=/usr/share/tomcat7 \
     CATALINA_BASE=/var/lib/tomcat7 \
     KAKADU_HOME=/opt/adore-djatoka-1.1/bin/Linux-x86-64 \
     CLASSPATH=$JAVA_HOME/jre/lib \
     FEDORA_HOME=/usr/local/fedora \
     JRE_HOME=/usr/lib/jvm/java-8-oracle/jre \
     JAVA_OPTS="-Djava.awt.headless=true -Xmx1024m -XX:MaxPermSize=256m -XX:+UseConcMarkSweepGC -Djava.net.preferIPv4Stack=true -Djava.net.preferIPv4Addresses=true -Dkakadu.home=/opt/adore-djatoka-1.1/bin/Linux-x86-64 -Djava.library.path=/opt/adore-djatoka-1.1/lib/Linux-x86-64 -DLD_LIBRARY_PATH=/opt/adore-djatoka-1.1/lib/Linux-x86-64" \
     JRE_PATH=$PATH:/usr/lib/jvm/java-8-oracle/bin:/usr/lib/jvm/java-8-oracle/jre/bin \
     FEDORA_PATH="$PATH:$FEDORA_HOME/server/bin:$FEDORA_HOME/client/bin" \
     KAKADU_LIBRARY_PATH=/opt/adore-djatoka-1.1/lib/Linux-x86-64 \
     KAKADU_HOME=/opt/adore-djatoka-1.1/lib/Linux-x86-64 \
     LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CATALINA_HOME/lib \
     PATH=$PATH:$FEDORA_HOME/server/bin:$FEDORA_HOME/client/bin \
     PATH=$PATH:/usr/lib/jvm/java-8-oracle/bin:/usr/lib/jvm/java-8-oracle/jre/bin \
     CATALINA_OPTS="-Djava.net.preferIPv4Stack=true -Dkakadu.home=/opt/adore-djatoka-1.1/bin/Linux-x86-64 -Djava.library.path=/opt/adore-djatoka-1.1/lib/Linux-x86-64 -DLD_LIBRARY_PATH=/opt/adore-djatoka-1.1/lib/Linux-x86-64" \
     LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CATALINA_HOME/lib


###
# Install tomcat7
RUN apt-get update
RUN apt-get install -y \
    tomcat7 \
    tomcat7-admin \
    libtcnative-1


###
# Fix perms & symlinks on tomcat7
RUN chmod -R 755 /var/cache/tomcat7 \
    && chown -R tomcat7:tomcat7 /var/cache/tomcat7 \
    && chown -R tomcat7:tomcat7 /var/log/tomcat7 \
    && chown -R tomcat7:tomcat7 /usr/share/tomcat7/lib \
    && chown -R tomcat7:tomcat7 /etc/tomcat7 \
    && chmod -R 775 /etc/tomcat7 \
    && ln -s /var/lib/tomcat7/common /usr/share/tomcat7/common \
    && chown -h tomcat7:tomcat7 /usr/share/tomcat7/common \
    && ln -s /var/lib/tomcat7/conf /usr/share/tomcat7/conf \
    && chown -h tomcat7:tomcat7 /usr/share/tomcat7/conf \
    && ln -s /var/lib/tomcat7/server /usr/share/tomcat7/server \
    && chown -h tomcat7:tomcat7 /usr/share/tomcat7/server \
    && ln -s /var/lib/tomcat7/shared /usr/share/tomcat7/shared \
    && chown -h tomcat7:tomcat7 /usr/share/tomcat7/shared \
    && ln -s /var/lib/tomcat7/webapps /usr/share/tomcat7/webapps \
    && chown -h tomcat7:tomcat7 /usr/share/tomcat7/webapps \
    && ln -s /var/log/tomcat7 /usr/share/tomcat7/logs \
    && chown -h tomcat7:tomcat7 /usr/share/tomcat7/logs \
    && mkdir /usr/share/tomcat7/temp \
    && chown -R tomcat7:tomcat7 /usr/share/tomcat7/temp \
    && chmod -R 775 /usr/share/tomcat7/temp \
    && mkdir /var/lib/tomcat7/temp \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/temp \
    && chmod -R 775 /var/lib/tomcat7/temp \
    && chown -R tomcat7:tomcat7 /usr/share/tomcat7 \
    && chown -R tomcat7:tomcat7 /usr/share/tomcat7-admin


###
# COPY over tomcat files for configuration
COPY tomcat/tomcat-users.xml /var/lib/tomcat7/conf/tomcat-users.xml
COPY tomcat/server.xml /var/lib/tomcat7/conf/server.xml
COPY tomcat/web.xml /var/lib/tomcat7/conf/web.xml
COPY tomcat/tomcat7 /etc/default/tomcat7
COPY tomcat/setenv.sh /usr/share/tomcat7/bin/setenv.sh


###
# Fix permissions on copied tomcat config files
RUN chown tomcat7:tomcat7 /var/lib/tomcat7/conf/tomcat-users.xml \
    && chmod 640 /var/lib/tomcat7/conf/tomcat-users.xml \
    && chown tomcat7:tomcat7 /var/lib/tomcat7/conf/server.xml \
    && chmod 644 /var/lib/tomcat7/conf/server.xml \
    && chown tomcat7:tomcat7 /var/lib/tomcat7/conf/web.xml \
    && chmod 644 /var/lib/tomcat7/conf/web.xml \
    && chown tomcat7:tomcat7 /etc/default/tomcat7 \
    && chmod 644 /etc/default/tomcat7 \
    && chown tomcat7:tomcat7 /usr/share/tomcat7/bin/setenv.sh \
    && chmod 755 /usr/share/tomcat7/bin/setenv.sh


###
# Fedora Installation

COPY fedora/install.properties /usr/local/install.properties
COPY fedora/fcrepo-installer-3.8.1.jar /usr/local/fcrepo-installer-3.8.1.jar


RUN /usr/bin/java -jar /usr/local/fcrepo-installer-3.8.1.jar /usr/local/install.properties \
    && chown -R tomcat7:tomcat7 /usr/local/fedora \
    && sudo -s -u tomcat7 /usr/share/tomcat7/bin/startup.sh \
    && sleep 70


COPY fedora/deny-apim-if-not-localhost.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-apim-if-not-localhost.xml


RUN chown -R tomcat7:tomcat7 /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default \
     && mkdir /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora \
     && chown -R tomcat7:tomcat7 /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora \
     && chmod -R 755 /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora \
     && rm /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-policy-management-if-not-administrator.xml \
     && rm /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-purge-datastream-if-active-or-inactive.xml \
     && rm /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-purge-object-if-active-or-inactive.xml


COPY fedora/permit-apim-to-authenticated-user.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/permit-apim-to-authenticated-user.xml
COPY fedora/permit-getDatastream-unrestricted.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/
COPY fedora/permit-getDatastreamHistory-unrestricted.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/
COPY fedora/permit-upload-to-authenticated-user.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/
COPY fedora/fedora-users.xml /usr/local/fedora/server/config/fedora-users.xml
COPY fedora/logback.xml /usr/local/fedora/server/config/logback.xml


RUN chown -R tomcat7:tomcat7 /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora \
    && chmod -R 755 /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora \
    && chown tomcat7:tomcat7 /usr/local/fedora/server/config/fedora-users.xml \
    && chmod -R 755 /usr/local/fedora/server/config/fedora-users.xml \
    && chown tomcat7:tomcat7 /usr/local/fedora/server/config/logback.xml \
    && chmod -R 644 /usr/local/fedora/server/config/logback.xml \
    && /bin/chown tomcat7:tomcat7 /etc/tomcat7/Catalina/localhost/fedora.xml \
    && rm /usr/local/fcrepo-installer-3.8.1.jar \
    && rm /usr/local/install.properties


###
# Drupalfilter
#

COPY drupalfilter/fcrepo-drupalauthfilter-3.8.1.jar /var/lib/tomcat7/webapps/fedora/WEB-INF/lib/fcrepo-drupalauthfilter-3.8.1.jar
COPY drupalfilter/filter-drupal.xml /usr/local/fedora/server/config/filter-drupal.xml
COPY drupalfilter/jaas.conf /usr/local/fedora/server/config/jaas.conf


RUN chown -R tomcat7:tomcat7 var/lib/tomcat7/webapps/fedora/WEB-INF/lib \
    && /bin/chown -R tomcat7:tomcat7 /usr/local/fedora/server/config


###
# Djatoka Tomcat
#

RUN cd /opt \
    && wget "http://downloads.sourceforge.net/project/djatoka/djatoka/1.1/adore-djatoka-1.1.tar.gz" \
    && tar -xzf adore-djatoka-1.1.tar.gz \
    && rm adore-djatoka-1.1.tar.gz


COPY djatoka/env.sh /opt/adore-djatoka-1.1/bin/env.sh
COPY djatoka/envinit.sh /opt/adore-djatoka-1.1/bin/envinit.sh
COPY djatoka/kdu_libs.conf /etc/ld.so.conf.d/kdu_libs.conf


RUN chown -R tomcat7:tomcat7 /opt/adore-djatoka-1.1 \
    && chmod -R g+rwx /opt/adore-djatoka-1.1 \
    && chmod 655 /opt/adore-djatoka-1.1/bin/env.sh \
    && chown tomcat7:tomcat7 /opt/adore-djatoka-1.1/bin/env.sh \
    && chmod 655 /opt/adore-djatoka-1.1/bin/envinit.sh \
    && chown tomcat7:tomcat7 /opt/adore-djatoka-1.1/bin/envinit.sh \
    && chown root:root /etc/ld.so.conf.d/kdu_libs.conf \
    && chmod 444 /etc/ld.so.conf.d/kdu_libs.conf \
    && ln -s /opt/adore-djatoka-1.1/bin/Linux-x86-64/kdu_compress /usr/local/bin/kdu_compress \
    && ln -s /opt/adore-djatoka-1.1/bin/Linux-x86-64/kdu_expand /usr/local/bin/kdu_expand \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_a60R.so /usr/local/lib/libkdu_a60R.so \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_jni.so /usr/local/lib/libkdu_jni.so \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_v60R.so /usr/local/lib/libkdu_v60R.so \
    && chown -h tomcat7:tomcat7 /usr/local/bin/kdu_compress \
    && chown -h tomcat7:tomcat7 /usr/local/bin/kdu_expand \
    && chown -h tomcat7:tomcat7 /usr/local/lib/libkdu_a60R.so \
    && chown -h tomcat7:tomcat7 /usr/local/lib/libkdu_jni.so \
    && chown -h tomcat7:tomcat7 /usr/local/lib/libkdu_v60R.so \
    && /sbin/ldconfig \
    && /bin/cp /opt/adore-djatoka-1.1/dist/adore-djatoka.war /var/lib/tomcat7/webapps/adore-djatoka.war \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps \
    && chmod 644 /var/lib/tomcat7/webapps/adore-djatoka.war \
    && sudo -s -u tomcat7 /usr/share/tomcat7/bin/startup.sh \
    && sleep 70

COPY djatoka/index.html /var/lib/tomcat7/webapps/adore-djatoka/index.html
COPY djatoka/log4j.properties var/lib/tomcat7/webapps/adore-djatoka/WEB-INF/classes/log4j.properties

###
# Gsearch
#

COPY gsearch/fedoragsearch-2.8.1.zip /tmp/fedoragsearch-2.8.1.zip

RUN  /usr/bin/unzip -o /tmp/fedoragsearch-2.8.1.zip -d /tmp \
    && /bin/cp -v /tmp/fedoragsearch-2.8.1/fedoragsearch.war /var/lib/tomcat7/webapps/ \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps \
    && chmod 0644 /var/lib/tomcat7/webapps/fedoragsearch.war \
    && /usr/bin/unzip -o /var/lib/tomcat7/webapps/fedoragsearch.war -d /var/lib/tomcat7/webapps/fedoragsearch/ \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps

COPY gsearch/fgsconfig-basic-for-islandora.properties /var/lib/tomcat7/webapps/fedoragsearch/FgsConfig/fgsconfig-basic-for-islandora.properties
COPY gsearch/fgsconfig-basic.xml /var/lib/tomcat7/webapps/fedoragsearch/FgsConfig/fgsconfig-basic.xml
COPY gsearch/log4j.xml /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/log4j.xml


RUN chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch/FgsConfig/fgsconfig-basic-for-islandora.properties \
    && chmod 644 /var/lib/tomcat7/webapps/fedoragsearch/FgsConfig/fgsconfig-basic-for-islandora.properties \
    && chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch/FgsConfig/fgsconfig-basic.xml \
    && chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/log4j.xml \
    && /usr/bin/ant -f /var/lib/tomcat7/webapps/fedoragsearch/FgsConfig/fgsconfig-basic.xml \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch/ \
    && cp -Rv /var/lib/tomcat7/webapps/fedoragsearch/FgsConfig/configForIslandora/fgsconfigFinal/. /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/

COPY gsearch/foxmlToSolr.xslt /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/foxmlToSolr.xslt
COPY gsearch/index.properties /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/index.properties


RUN chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch/ \
    && /usr/bin/git clone https://github.com/discoverygarden/dgi_gsearch_extensions.git /tmp/dgi_gsearch_extensions \
    && cd /tmp/dgi_gsearch_extensions \
    && /usr/bin/mvn package \
    && /bin/cp /tmp/dgi_gsearch_extensions/target/gsearch_extensions-0.1.3-jar-with-dependencies.jar /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/lib/gsearch_extensions-0.1.3-jar-with-dependencies.jar \
    && /bin/chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/lib/gsearch_extensions-0.1.3-jar-with-dependencies.jar \
    && /usr/bin/git clone -b master https://github.com/discoverygarden/islandora_transforms.git /tmp/islandora_transforms \
    && sed -i 's#/usr/local/fedora/tomcat#/var/lib/tomcat7#g' /tmp/islandora_transforms/*.xslt \
    && /bin/cp -Rv /tmp/islandora_transforms /var/lib/tomcat7/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch \
    && chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/fedoragsearch.war \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/ \
    && sudo -s -u tomcat7 /usr/share/tomcat7/bin/startup.sh \
    && sleep 70


ADD tomcat/runservices.sh /opt/runservices.sh
RUN chmod +x /opt/runservices.sh

ENTRYPOINT ["/opt/runservices.sh"]
