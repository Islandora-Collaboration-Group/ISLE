#!/bin/bash
openssl req -x509 -nodes -days 3650 -newkey rsa:2048 -keyout isle.localdomain.key -out isle.localdomain.pem \
	-subj "/C=US/O=Islandora Collaboration Group/OU=Domain Control Validated/CN=isle.localdomain"
