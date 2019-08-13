#!/bin/bash
openssl req -x509 -nodes -days 3650 -newkey rsa:2048 -keyout ../../../config/proxy/ssl-certs/isle.localdomain.key -out ../../../config/proxy/ssl-certs/isle.localdomain.cert \
	-subj "/C=US/O=Islandora Collaboration Group/OU=Domain Control Validated/CN=isle.localdomain"
