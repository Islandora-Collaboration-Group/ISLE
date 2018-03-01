FROM tomcat:8.0-jre8
## 
LABEL "io.github.islandora-collaboration-group.name"="isle-solr" 
LABEL "io.github.islandora-collaboration-group.description"="ISLE Solr container, a powerful search engine responsible for handling user searches through your Islandora collections."
LABEL "io.github.islandora-collaboration-group.license"="Apache-2.0" 
LABEL "io.github.islandora-collaboration-group.vcs-url"="git@github.com:Islandora-Collaboration-Group/ISLE.git" 
LABEL "io.github.islandora-collaboration-group.vendor"="Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com"
LABEL "io.github.islandora-collaboration-group.maintainer"="Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com" 
##

###
# Copy over tomcat files for configuration
COPY tomcat/tomcat-users.xml /usr/local/tomcat/conf/tomcat-users.xml
COPY tomcat/server.xml /usr/local/tomcat/conf/server.xml
COPY tomcat/web.xml /usr/local/tomcat/conf/web.xml

###
# Solr Installation
#

# Set up environmental variables for tomcat & dependencies installation

ENV JAVA_HOME=/docker-java-home/jre \
     CATALINA_HOME=/usr/local/tomcat \
     CATALINA_BASE=/usr/local/tomcat \
     CLASSPATH=$JAVA_HOME/jre/lib \
     JRE_HOME=/docker-java-home/jre \
     JAVA_OPTS="-Djava.awt.headless=true -Xmx1024m -XX:MaxPermSize=256m -XX:+UseConcMarkSweepGC -Djava.net.preferIPv4Stack=true -Djava.net.preferIPv4Addresses=true" \
     JRE_PATH=$PATH:/docker-java-home/jre/bin:/usr/lib/jvm/java-8-openjdk-amd64/jre/bin \
     PATH=$PATH:/docker-java-home/jre/bin:/usr/lib/jvm/java-8-openjdk-amd64/jre/bin \
     PATH=$PATH:/$CATALINA_HOME/bin:$PATH \
     CATALINA_OPTS="-Djava.net.preferIPv4Stack=true" \
     LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CATALINA_HOME/lib

###
# https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
    && cd /tmp \
    && wget "http://archive.apache.org/dist/lucene/solr/4.10.4/solr-4.10.4.tgz" \
    && tar -xvzf /tmp/solr-4.10.4.tgz \
    && cp -v /tmp/solr-4.10.4/dist/solr-4.10.4.war /usr/local/tomcat/webapps/solr.war \
    && /usr/bin/unzip -o /usr/local/tomcat/webapps/solr.war -d /usr/local/tomcat/webapps/solr/ \
    && cp -rv /tmp/solr-4.10.4/example/solr/. /usr/local/solr/ \
    && cp -rv /tmp/solr-4.10.4/example/lib/ext/. /usr/local/tomcat/webapps/solr/WEB-INF/lib/ \
    && cp -v /tmp/solr-4.10.4/contrib/analysis-extras/lib/icu4j-53.1.jar /usr/local/tomcat/webapps/solr/WEB-INF/lib/ \
    && cp -v /tmp/solr-4.10.4/contrib/analysis-extras/lucene-libs/lucene-analyzers-icu-4.10.4.jar /usr/local/tomcat/webapps/solr/WEB-INF/lib/ \
    && rm -rf /tmp/solr-4.10.4

COPY solr/solr.xml /usr/local/tomcat/conf/Catalina/localhost/solr.xml
COPY solr/log4j.properties /usr/local/tomcat/webapps/solr/WEB-INF/classes/log4j.properties
COPY solr/solrconfig.xml /usr/local/solr/collection1/conf/solrconfig.xml
COPY solr/schema.xml /usr/local/solr/collection1/conf/schema.xml
COPY solr/stopwords.txt /usr/local/solr/collection1/conf/stopwords.txt

VOLUME /usr/local/solr

EXPOSE 8080 8983
CMD ["catalina.sh", "run"]
