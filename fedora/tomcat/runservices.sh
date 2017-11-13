#!/bin/bash

apachectl start

/bin/bash /usr/local/tomcat/bin/catalina.sh run

#Run bash to keep container running and provide interactive mode
bash
