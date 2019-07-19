#!/bin/bash
# Replace the following with your intended Local environment values
# Add the name of your local site name to .localdomain.key e.g acme.localdomain.key
# Add the name of your local site name to .localdomain.pem e.g acme.localdomain.pem
# Add the name of your local site name to CN=.localdomain e.g CN=acme.localdomain

openssl req -x509 -nodes -days 3650 -newkey rsa:2048 -keyout .localdomain.key -out .localdomain.pem \
	-subj "/C=US/O=Islandora Collaboration Group/OU=Domain Control Validated/CN=.localdomain"
