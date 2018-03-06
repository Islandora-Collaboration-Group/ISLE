FROM nginx:latest

## 
LABEL "io.github.islandora-collaboration-group.name"="isle-proxy" 
LABEL "io.github.islandora-collaboration-group.description"="ISLE Proxy securely routes the internal isle-network to the external world using SSL-encryption. \
This is a lightweight NGINX reverse proxy with an intermediate/strong SSL cipher requirement." 
LABEL "io.github.islandora-collaboration-group.license"="Apache-2.0" 
LABEL "io.github.islandora-collaboration-group.vcs-url"="git@github.com:Islandora-Collaboration-Group/ISLE.git" 
LABEL "io.github.islandora-collaboration-group.vendor"="Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com"
LABEL "io.github.islandora-collaboration-group.maintainer"="Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com" 
##

# master nginx configuration specifies both the number of children process and worker connections. 2 works and 1024 connections per is the default for ISLE.
COPY config/nginx.conf /etc/nginx/nginx.conf
# overwrite and create new conf.d folder. in this folder there is the default "proxy headers" to add to requests and compression and content caching. 
COPY config/conf.d /etc/nginx/conf.d
# upstream to internal services
COPY config/upstreams.d /etc/nginx/upstreams.d
COPY config/sites-enabled /etc/nginx/sites-enabled
COPY config/ssl-certs /certs

VOLUME /etc/nginx/sites-enabled /etc/nginx/conf.d /etc/nginx/upstreams.d /certs

EXPOSE 80/tcp 443/tcp

CMD ["nginx", "-g", "daemon off;"]
