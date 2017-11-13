#! /bin/sh
export CATALINA_OPTS="-Djava.net.preferIPv4Stack=true -Dkakadu.home=/opt/adore-djatoka-1.1/bin/Linux-x86-64 -Djava.library.path=/opt/adore-djatoka-1.1/lib/Linux-x86-64 -DLD_LIBRARY_PATH=/opt/adore-djatoka-1.1/lib/Linux-x86-64"

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CATALINA_HOME/lib
export LD_LIBRARY_PATH
