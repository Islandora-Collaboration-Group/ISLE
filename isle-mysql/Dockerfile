FROM mysql/mysql-server:latest

ENV MYSQL_RANDOM_ROOT_PASSWORD=yes

COPY config /etc/mysql/conf.d
COPY entrypoint /docker-entrypoint-initdb.d

LABEL version="0.0.1-preA" \
description="ISLE MVP \
11/2017 MySQL Server for Islandora Stack." \
authors="Gavin Morris <gavin@born-digital.com>, Benjamin Rosner <brosner@barnard.edu>" \
maintainer="Gavin Morris <gavin@born-digital.com>"
