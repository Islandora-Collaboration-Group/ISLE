#!/bin/bash
# Replace the following with your intended Local environment values
# Add the name of your local site name to .localdomain.key (e.g. yourprojectnamehere.localdomain.key)
# Add the name of your local site name to .localdomain.pem (e.g. yourprojectnamehere.localdomain.pem)
# Add the name of your local site name to CN=.localdomain (e.g. CN=yourprojectnamehere.localdomain)
# Optional: You may change the organization ("O=") name (e.g. O=Your Organization)

openssl req -x509 -nodes -days 3650 -newkey rsa:2048 -keyout ../../../config/proxy/ssl-certs/.localdomain.key -out ../../../config/proxy/ssl-certs/.localdomain.pem \
	-subj "//C=US\O=Islandora Collaboration Group\OU=Domain Control Validated\CN=.localdomain"
