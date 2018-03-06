FROM mysql:5.6

## 
LABEL "io.github.islandora-collaboration-group.name"="isle-mysql" 
LABEL "io.github.islandora-collaboration-group.description"="ISLE-specific MySQL server with sample site isle.localdomain preloaded!"
LABEL "io.github.islandora-collaboration-group.license"="Apache-2.0" 
LABEL "io.github.islandora-collaboration-group.vcs-url"="git@github.com:Islandora-Collaboration-Group/ISLE.git" 
LABEL "io.github.islandora-collaboration-group.vendor"="Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com"
LABEL "io.github.islandora-collaboration-group.maintainer"="Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com" 
##

COPY my.cnf /etc/alternatives/
COPY init_dbs/ /docker-entrypoint-initdb.d/

#NB: The VOLUME /var/lib/mysql comes from the base image (https://hub.docker.com/_/mysql/)

EXPOSE 3306

CMD ["mysqld"]
