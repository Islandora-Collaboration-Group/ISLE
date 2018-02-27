FROM tomcat:8.0-jre8
LABEL org.label-schema.name = "isle-fedora" \
      org.label-schema.description: "ISLE Base Image with sample site isle.localdomain fedora Dockerfile for non-production usage." \
      org.label-schema.license": "Apache-2.0" \
      org.label-schema.vcs-url: "git@github.com:Islandora-Collaboration-Group/ISLE.git" \
      org.label-schema.vendor": "Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com" \
      org.label-schema.schema-version = "1.0"

###
# COPY over tomcat config files
COPY tomcat/server.xml /usr/local/tomcat/conf/server.xml
COPY tomcat/web.xml /usr/local/tomcat/conf/web.xml
COPY tomcat/tomcat-users.xml /usr/local/tomcat/conf/tomcat-users.xml


RUN chmod 755 /usr/local/tomcat/bin/runservices.sh


###
# Set up environmental variables for tomcat & dependencies installation

ENV JAVA_HOME=/usr/lib/jvm/java-8-oracle \
     CATALINA_HOME=/usr/local/tomcat \
     CATALINA_BASE=/usr/local/tomcat \
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
# https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
    && echo "deb http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main" | tee /etc/apt/sources.list.d/webupd8team-java.list \
    && echo "deb-src http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main" | tee -a /etc/apt/sources.list.d/webupd8team-java.list \
    && echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | /usr/bin/debconf-set-selections \
    && apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys EEA14886 \
    && apt-get update \
    && apt-get install -y software-properties-common \
    tmpreaper \
    mysql-client \
    python-mysqldb \
    default-libmysqlclient-dev \
    maven \
    git \
    curl \
    dnsutils \
    ca-certificates \
    apt-transport-https \
    openssl \
    libxml2-dev \
    oracle-java8-installer \
    oracle-java8-set-default \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && touch /etc/cron.d/tmpreaper-cron \
    && echo "0 */12 * * * root /usr/sbin/tmpreaper -am 4d /tmp >> /var/log/cron.log 2>&1" /etc/cron.d/tmpreaper-cron \
    && chmod 0644 /etc/cron.d/tmpreaper-cron \
    && touch /var/log/cron.log


###
# Djatoka
#

RUN cd /opt \
    && wget -O /opt/adore-djatoka-1.1.tar.gz "http://downloads.sourceforge.net/project/djatoka/djatoka/1.1/adore-djatoka-1.1.tar.gz" \
    && tar -xzf adore-djatoka-1.1.tar.gz \
    && ln -s /opt/adore-djatoka-1.1/bin/Linux-x86-64/kdu_compress /usr/local/bin/kdu_compress \
    && ln -s /opt/adore-djatoka-1.1/bin/Linux-x86-64/kdu_expand /usr/local/bin/kdu_expand \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_a60R.so /usr/local/lib/libkdu_a60R.so \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_jni.so /usr/local/lib/libkdu_jni.so \
    && ln -s /opt/adore-djatoka-1.1/lib/Linux-x86-64/libkdu_v60R.so /usr/local/lib/libkdu_v60R.so \
    && /sbin/ldconfig \
    && /bin/cp /opt/adore-djatoka-1.1/dist/adore-djatoka.war /usr/local/tomcat/webapps/adore-djatoka.war \
    && /usr/bin/unzip -o /usr/local/tomcat/webapps/adore-djatoka.war -d /usr/local/tomcat/webapps/adore-djatoka/ \
    && rm adore-djatoka-1.1.tar.gz \
    && sed -i 's/DJATOKA_HOME=`pwd`/DJATOKA_HOME=\/opt\/adore-djatoka-1.1/g' /opt/adore-djatoka-1.1/bin/env.sh \
    && sed -i 's|`uname -p` = "x86_64"|`uname -m` = "x86_64"|' /opt/adore-djatoka-1.1/bin/env.sh \
    && touch /etc/ld.so.conf.d/kdu_libs.conf \
    && echo "/opt/adore-djatoka-1.1/lib/Linux-x86-64" > /etc/ld.so.conf.d/kdu_libs.conf \
    && chmod 444 /etc/ld.so.conf.d/kdu_libs.conf \
    && chown root:root /etc/ld.so.conf.d/kdu_libs.conf \
    && sed -i 's/localhost/fedora/g' /usr/local/tomcat/webapps/adore-djatoka/index.html

COPY adore-djatoka/envinit.sh /opt/adore-djatoka-1.1/bin/envinit.sh
COPY adore-djatoka/log4j.properties /usr/local/tomcat/webapps/adore-djatoka/WEB-INF/classes/log4j.properties

###
# Fedora Installation with Drupalfilter

COPY fedora/install.properties /usr/local/install.properties

RUN cd /usr/local/ \
    && wget "https://github.com/fcrepo3/fcrepo/releases/download/v3.8.1/fcrepo-installer-3.8.1.jar" \
    && /usr/bin/java -jar /usr/local/fcrepo-installer-3.8.1.jar /usr/local/install.properties \
    && /usr/local/tomcat/bin/startup.sh \
    && sleep 70

COPY fedora/deny-apim-if-not-localhost.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-apim-if-not-localhost.xml

RUN mkdir /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora \
    && rm /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-policy-management-if-not-administrator.xml \
    && rm /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-purge-datastream-if-active-or-inactive.xml \
    && rm /usr/local/fedora/data/fedora-xacml-policies/repository-policies/default/deny-purge-object-if-active-or-inactive.xml \
    && rm /usr/local/fcrepo-installer-3.8.1.jar \
    && rm /usr/local/install.properties \
    && cd /usr/local/tomcat/webapps/fedora/WEB-INF/lib/ \
    && wget "https://github.com/Islandora/islandora_drupal_filter/releases/download/v7.1.9/fcrepo-drupalauthfilter-3.8.1.jar"

COPY fedora/permit-apim-to-authenticated-user.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/permit-apim-to-authenticated-user.xml
COPY fedora/permit-getDatastream-unrestricted.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/permit-getDatastream-unrestricted.xml
COPY fedora/permit-getDatastreamHistory-unrestricted.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/permit-getDatastreamHistory-unrestricted.xml
COPY fedora/permit-upload-to-authenticated-user.xml /usr/local/fedora/data/fedora-xacml-policies/repository-policies/islandora/permit-upload-to-authenticated-user.xml
COPY fedora/fedora-users.xml /usr/local/fedora/server/config/fedora-users.xml
COPY fedora/logback.xml /usr/local/fedora/server/config/logback.xml
COPY fedora/filter-drupal.xml /usr/local/fedora/server/config/filter-drupal.xml
COPY fedora/jaas.conf /usr/local/fedora/server/config/jaas.conf


###
# Gsearch
#
# Removing log4j-over-slf4j-1.5.10.jar allows gsearch to startup properly.
#
###

RUN wget -O /tmp/fedoragsearch-2.8.1.zip https://github.com/discoverygarden/gsearch/releases/download/v2.8.1/fedoragsearch-2.8.1.zip \
    && /usr/bin/unzip -o /tmp/fedoragsearch-2.8.1.zip -d /tmp \
    && /bin/cp -v /tmp/fedoragsearch-2.8.1/fedoragsearch.war /usr/local/tomcat/webapps/ \
    && /usr/bin/unzip -o /usr/local/tomcat/webapps/fedoragsearch.war -d /usr/local/tomcat/webapps/fedoragsearch/ \
    && rm -f /usr/local/tomcat/webapps/fedoragsearch/WEB-INF/lib/log4j-over-slf4j-1.5.10.jar \
    && rm -rf /tmp/gsearch \
    && rm -rf /tmp/fedoragsearch-2.8.1


COPY gsearch/fgsconfig-basic-for-islandora.properties /usr/local/tomcat/webapps/fedoragsearch/FgsConfig/fgsconfig-basic-for-islandora.properties
COPY gsearch/fgsconfig-basic.xml /usr/local/tomcat/webapps/fedoragsearch/FgsConfig/fgsconfig-basic.xml
COPY gsearch/log4j.xml /usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/log4j.xml


RUN /usr/bin/ant -f /usr/local/tomcat/webapps/fedoragsearch/FgsConfig/fgsconfig-basic.xml \
    && cp -Rv /usr/local/tomcat/webapps/fedoragsearch/FgsConfig/configForIslandora/fgsconfigFinal/. /usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/ \
    && /usr/bin/git clone https://github.com/discoverygarden/dgi_gsearch_extensions.git /tmp/dgi_gsearch_extensions \
    && cd /tmp/dgi_gsearch_extensions \
    && /usr/bin/mvn package \
    && /bin/cp /tmp/dgi_gsearch_extensions/target/gsearch_extensions-0.1.3-jar-with-dependencies.jar /usr/local/tomcat/webapps/fedoragsearch/WEB-INF/lib/gsearch_extensions-0.1.3-jar-with-dependencies.jar \
    && /usr/bin/git clone -b master https://github.com/discoverygarden/islandora_transforms.git /tmp/islandora_transforms \
    && sed -i 's#/usr/local/fedora/tomcat#/usr/local/tomcat#g' /tmp/islandora_transforms/*.xslt \
    && /bin/cp -Rv /tmp/islandora_transforms /usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms \
    && rm -rf /tmp/dgi_gsearch_extensions \
    && rm -rf /tmp/islandora_transforms


COPY gsearch/foxmlToSolr.xslt /usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/foxmlToSolr.xslt
COPY gsearch/index.properties /usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/index.properties

EXPOSE 8080
CMD ["catalina.sh", "run"]
