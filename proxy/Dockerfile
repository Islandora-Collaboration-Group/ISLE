FROM nginx:latest
LABEL org.label-schema.name = "isle-proxy" \
      org.label-schema.description: "ISLE Base Image with sample site isle.localdomain proxy Dockerfile for non-production usage." \
      org.label-schema.license": "Apache-2.0" \
      org.label-schema.vcs-url: "git@github.com:Islandora-Collaboration-Group/ISLE.git" \
      org.label-schema.vendor": "Islandora Collaboration Group (ICG) - islandora-consortium-group@googlegroups.com" \
      org.label-schema.schema-version = "1.0"

COPY config/conf.d/compression_and_cache-control.conf /etc/nginx/conf.d/
COPY config/conf.d/proxy-global.conf /etc/nginx/conf.d/
COPY config/sites-enabled /etc/nginx/sites-enabled
COPY config/ssl-certs /certs
COPY config/upstreams.d /etc/nginx/upstreams.d
COPY config/nginx.conf /etc/nginx/nginx.conf

CMD ["nginx", "-g", "daemon off;"]
