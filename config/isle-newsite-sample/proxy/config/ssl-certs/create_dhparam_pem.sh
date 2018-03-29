# bin/bash!

# to generate your dhparam.pem file, run in the terminal on your local laptop, this process will take 10 - 15 mins depending.
# change the path to the correct ISLE project directory prior to running if necessary
openssl dhparam -out /opt/ISLE/config/newsite-sample/proxy/config/ssl-certs/dhparam.pem 2048
