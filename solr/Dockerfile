FROM ubuntu:14.04
MAINTAINER "Gavin Morris <gavin@born-digital.com>"
### - ISLE MVP 10/2017 Solr

###
# https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections


###
# Update apt
RUN apt-get update


###
# Add software to provide add-apt-repository
RUN apt-get install -y software-properties-common \
    python-software-properties \
    ntp \
    openssh-client \
    unzip \
    libtool \
    dkms \
    bzip2 \
    perl \
    apt-utils \
    default-jdk \
    apt-transport-https \
    openssl \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

###
# Make sure ports 8080 & 8993 are available
EXPOSE 8080 8983


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
     PATH=$PATH:/$CATALINA_HOME/bin:$PATH \
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
# Copy over tomcat files for configuration
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
# Solr Installation
#

COPY solr/solr.xml /var/lib/tomcat7/conf/Catalina/localhost/solr.xml


RUN cd /tmp \
    && wget "http://archive.apache.org/dist/lucene/solr/4.10.4/solr-4.10.4.tgz" \
    && tar -xvzf /tmp/solr-4.10.4.tgz \
    && cp -v /tmp/solr-4.10.4/dist/solr-4.10.4.war /var/lib/tomcat7/webapps/solr.war \
    && chown tomcat7:tomcat7 /var/lib/tomcat7/conf/Catalina/localhost/solr.xml \
    && chmod 644 /var/lib/tomcat7/conf/Catalina/localhost/solr.xml \
    && chown tomcat7:tomcat7 /usr/local/solr \
    && chmod 755 /usr/local/solr \
    && /usr/bin/unzip -o /var/lib/tomcat7/webapps/solr.war -d /var/lib/tomcat7/webapps/solr/ \
    && cp -rv /tmp/solr-4.10.4/example/solr/. /usr/local/solr/ \
    && cp -rv /tmp/solr-4.10.4/example/lib/ext/. /var/lib/tomcat7/webapps/solr/WEB-INF/lib/ \
    && cp -v /tmp/solr-4.10.4/contrib/analysis-extras/lib/icu4j-53.1.jar /var/lib/tomcat7/webapps/solr/WEB-INF/lib/ \
    && cp -v /tmp/solr-4.10.4/contrib/analysis-extras/lucene-libs/lucene-analyzers-icu-4.10.4.jar /var/lib/tomcat7/webapps/solr/WEB-INF/lib/ \
    && wget "https://www.slf4j.org/dist/slf4j-1.6.6.tar.gz" \
    && cp -v /tmp/slf4j-1.6.6/slf4j-jdk14-1.6.6.jar /usr/share/tomcat7/lib/slf4j-jdk14-1.6.6.jar \
    && cp -v /tmp/slf4j-1.6.6/log4j-over-slf4j-1.6.6.jar /usr/share/tomcat7/lib/log4j-over-slf4j-1.6.6.jar \
    && chown tomcat7:tomcat7 /var/lib/tomcat7/webapps/solr.war \
    && chmod 644 /var/lib/tomcat7/webapps/solr.war \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps/solr
    && mkdir -p /var/lib/tomcat7/webapps/solr/WEB-INF/classes \
    && chown -R tomcat7:tomcat7 /var/lib/tomcat7/webapps/solr/WEB-INF \
    && chmod -R 755 /var/lib/tomcat7/webapps/solr/WEB-INF/classes \
    && /bin/chown -R tomcat7:tomcat7 /usr/share/tomcat7/lib/.

COPY solr/log4j.properties /var/lib/tomcat7/webapps/solr/WEB-INF/classes/log4j.properties
COPY solr/solrconfig.xml /usr/local/solr/collection1/conf/solrconfig.xml
COPY solr/schema.xml /usr/local/solr/collection1/conf/schema.xml
COPY solr/stopwords.txt /usr/local/solr/collection1/conf/stopwords.txt

RUN chown -R tomcat7:tomcat7 /var/lib/tomcat7 \
    && chown -R tomcat7:tomcat7 /usr/local/solr \
    && chmod 644 /var/lib/tomcat7/webapps/solr/WEB-INF/classes/log4j.properties \
    && chown -R tomcat7:tomcat7 /usr/share/tomcat7 \
    && chmod 644 /usr/share/tomcat7/lib/slf4j-jdk14-1.6.6.jar \
    && chmod 644 /usr/share/tomcat7/lib/log4j-over-slf4j-1.6.6.jar \
    && sudo -s -u tomcat7 /usr/share/tomcat7/bin/startup.sh \
    && sleep 70

ADD solr/runservices.sh /opt/runservices.sh
RUN chmod +x /opt/runservices.sh

ENTRYPOINT ["/opt/runservices.sh"]
