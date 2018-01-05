FROM mysql:5.6
LABEL org.label-schema.name = "isle-mysql" \
      org.label-schema.description: "ISLE Base Image with sample site isle.localdomain mysql Dockerfile for non-production usage."
      org.label-schema.license": "Apache-2.0" \
      org.label-schema.vcs-url: "git@github.com:Islandora-Collaboration-Group/ISLE.git" \
      org.label-schema.vendor": "Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com" \
      org.label-schema.schema-version = "1.0"

COPY mysql/config/my.cnf /etc/alternatives/my.cnf
COPY mysql/config/init_dbs/ /docker-entrypoint-initdb.d/

EXPOSE 3306
CMD ["mysqld"]
